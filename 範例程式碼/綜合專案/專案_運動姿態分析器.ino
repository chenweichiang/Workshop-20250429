/******************************************************************************
 * 運動姿態分析器專案
 * 
 * 本專案展示如何:
 * - 使用MPU6050加速度計和陀螺儀感測器
 * - 分析和識別基本運動姿態
 * - 通過序列監視器顯示姿態數據和狀態
 * - 透過LED指示燈顯示不同的運動狀態
 * 
 * 接線:
 * - MPU6050 VCC -> Arduino 5V或3.3V
 * - MPU6050 GND -> Arduino GND
 * - MPU6050 SCL -> Arduino A5 (SCL)
 * - MPU6050 SDA -> Arduino A4 (SDA)
 * - 紅色LED -> Arduino pin 8 (透過220歐姆電阻)
 * - 綠色LED -> Arduino pin 9 (透過220歐姆電阻)
 * - 藍色LED -> Arduino pin 10 (透過220歐姆電阻)
 * 
 * 需要安裝的函式庫:
 * - MPU6050 by Electronic Cats
 * - Wire library (內建於Arduino IDE)
 ******************************************************************************/

#include <Wire.h>
#include <MPU6050.h>

// 創建MPU6050物件
MPU6050 mpu;

// LED針腳定義
#define RED_LED 8
#define GREEN_LED 9
#define BLUE_LED 10

// 狀態定義
#define STATE_IDLE 0
#define STATE_WALKING 1
#define STATE_RUNNING 2
#define STATE_FALLING 3

// 全域變數
int currentState = STATE_IDLE;
unsigned long lastStateChangeTime = 0;
unsigned long stateHoldTime = 500; // 狀態需要保持的最小時間(毫秒)
unsigned long lastPrintTime = 0;
unsigned long printInterval = 500; // 打印間隔時間(毫秒)

void setup() {
  // 初始化串口通信
  Serial.begin(9600);
  
  // 初始化LED針腳
  pinMode(RED_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(BLUE_LED, OUTPUT);
  
  // 設定所有LED為關閉狀態
  digitalWrite(RED_LED, LOW);
  digitalWrite(GREEN_LED, LOW);
  digitalWrite(BLUE_LED, LOW);
  
  Serial.println("初始化MPU6050...");
  
  // 初始化I2C通信
  Wire.begin();
  
  // 初始化MPU6050
  while(!mpu.begin(MPU6050_SCALE_2000DPS, MPU6050_RANGE_16G)) {
    Serial.println("無法找到MPU6050感測器!");
    delay(500);
  }
  
  // 設置MPU6050
  mpu.setAccelPowerOnDelay(MPU6050_DELAY_3MS);
  
  // 校準陀螺儀和加速度計
  mpu.calibrateGyro();
  mpu.setThreshold(3);
  
  // 檢查連接
  Serial.println("MPU6050測試...");
  Serial.print("陀螺儀: ");
  Serial.println(mpu.testGyro() ? "成功" : "失敗");
  Serial.print("加速度計: ");
  Serial.println(mpu.testAccel() ? "成功" : "失敗");
  Serial.print("溫度計: ");
  Serial.println(mpu.testTemp() ? "成功" : "失敗");
  
  Serial.println("系統初始化完成!");
  Serial.println("運動姿態分析器已就緒");
  Serial.println("------------------------------------------");
  Serial.println("識別狀態:");
  Serial.println("- 靜止: 綠燈");
  Serial.println("- 行走: 藍燈");
  Serial.println("- 跑步: 藍綠交替閃爍");
  Serial.println("- 跌倒: 紅燈");
  Serial.println("------------------------------------------");
  
  // 啟動完成信號
  indicateReady();
}

void loop() {
  // 讀取正常化的值(單位: 重力g)
  Vector normAccel = mpu.readNormalizeAccel();
  Vector normGyro = mpu.readNormalizeGyro();
  
  // 計算加速度大小和姿態特徵
  float accelMagnitude = sqrt(normAccel.XAxis * normAccel.XAxis + 
                             normAccel.YAxis * normAccel.YAxis + 
                             normAccel.ZAxis * normAccel.ZAxis);
  
  float gyroMagnitude = sqrt(normGyro.XAxis * normGyro.XAxis + 
                             normGyro.YAxis * normGyro.YAxis + 
                             normGyro.ZAxis * normGyro.ZAxis);
  
  // 姿態判斷邏輯
  int newState = determineState(accelMagnitude, gyroMagnitude, normAccel);
  
  // 只有在狀態維持一段時間後才改變顯示狀態，避免快速抖動
  unsigned long currentTime = millis();
  if (newState != currentState && currentTime - lastStateChangeTime > stateHoldTime) {
    lastStateChangeTime = currentTime;
    currentState = newState;
    updateLEDs(currentState);
  }
  
  // 定期輸出數據到串口監視器
  if (currentTime - lastPrintTime > printInterval) {
    lastPrintTime = currentTime;
    printData(normAccel, normGyro, accelMagnitude, gyroMagnitude);
  }
  
  // 小延遲提高穩定性
  delay(50);
}

// 根據感測器數據確定運動狀態
int determineState(float accelMag, float gyroMag, Vector normAccel) {
  // 跌倒檢測 - 檢測突然大的加速度變化以及異常的姿態
  if (accelMag > 3.0 || abs(normAccel.ZAxis) < 0.3) {
    return STATE_FALLING;
  }
  
  // 跑步檢測 - 較大的上下運動和振動
  if (accelMag > 1.7 && gyroMag > 50) {
    return STATE_RUNNING;
  }
  
  // 行走檢測 - 適度的加速度變化
  if (accelMag > 1.2 || gyroMag > 30) {
    return STATE_WALKING;
  }
  
  // 默認為靜止狀態
  return STATE_IDLE;
}

// 根據當前狀態更新LED顯示
void updateLEDs(int state) {
  // 清除所有LED
  digitalWrite(RED_LED, LOW);
  digitalWrite(GREEN_LED, LOW);
  digitalWrite(BLUE_LED, LOW);
  
  // 根據狀態設置LED
  switch (state) {
    case STATE_IDLE:
      digitalWrite(GREEN_LED, HIGH);
      Serial.println("狀態: 靜止");
      break;
      
    case STATE_WALKING:
      digitalWrite(BLUE_LED, HIGH);
      Serial.println("狀態: 行走");
      break;
      
    case STATE_RUNNING:
      // 跑步狀態用藍綠交替閃爍表示，通過loop函數中的延遲實現
      digitalWrite(BLUE_LED, HIGH);
      digitalWrite(GREEN_LED, HIGH);
      Serial.println("狀態: 跑步");
      break;
      
    case STATE_FALLING:
      digitalWrite(RED_LED, HIGH);
      Serial.println("警告: 檢測到異常動作/可能跌倒!");
      break;
  }
}

// 啟動指示序列
void indicateReady() {
  for (int i = 0; i < 3; i++) {
    digitalWrite(RED_LED, HIGH);
    delay(100);
    digitalWrite(RED_LED, LOW);
    
    digitalWrite(GREEN_LED, HIGH);
    delay(100);
    digitalWrite(GREEN_LED, LOW);
    
    digitalWrite(BLUE_LED, HIGH);
    delay(100);
    digitalWrite(BLUE_LED, LOW);
  }
  
  // 最後亮綠燈表示就緒
  digitalWrite(GREEN_LED, HIGH);
}

// 打印數據到串口監視器
void printData(Vector normAccel, Vector gyro, float accelMag, float gyroMag) {
  Serial.print("加速度(g): X=");
  Serial.print(normAccel.XAxis);
  Serial.print(" Y=");
  Serial.print(normAccel.YAxis);
  Serial.print(" Z=");
  Serial.print(normAccel.ZAxis);
  Serial.print(" 合力=");
  Serial.println(accelMag);
  
  Serial.print("角速度(度/秒): X=");
  Serial.print(gyro.XAxis);
  Serial.print(" Y=");
  Serial.print(gyro.YAxis);
  Serial.print(" Z=");
  Serial.print(gyro.ZAxis);
  Serial.print(" 合力=");
  Serial.println(gyroMag);
  
  Serial.print("當前狀態: ");
  switch (currentState) {
    case STATE_IDLE:
      Serial.println("靜止");
      break;
    case STATE_WALKING:
      Serial.println("行走");
      break;
    case STATE_RUNNING:
      Serial.println("跑步");
      break;
    case STATE_FALLING:
      Serial.println("跌倒警告!");
      break;
  }
  Serial.println("-----------------------");
}