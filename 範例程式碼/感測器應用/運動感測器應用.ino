/******************************************************************************
 * MPU6050 加速度陀螺儀進階應用範例
 * 
 * 本範例展示如何:
 * - 高精度姿態解算(Pitch, Roll, Yaw)
 * - 運動模式識別(靜止、走路、跑步)
 * - 實現互補濾波器處理陀螺儀漂移
 * - 使用LCD顯示當前姿態與運動狀態
 * 
 * 接線:
 * - MPU6050 VCC -> Arduino 5V/3.3V
 * - MPU6050 GND -> Arduino GND
 * - MPU6050 SCL -> Arduino A5 (SCL)
 * - MPU6050 SDA -> Arduino A4 (SDA)
 * - LCD I2C SDA -> Arduino A4 (與MPU6050共用I2C匯流排)
 * - LCD I2C SCL -> Arduino A5 (與MPU6050共用I2C匯流排)
 * - LCD I2C VCC -> Arduino 5V
 * - LCD I2C GND -> Arduino GND
 * - 警報蜂鳴器 -> Arduino 數位針腳 8
 * - 指示 LED -> Arduino 數位針腳 9
 * 
 * 需要安裝的函式庫:
 * - MPU6050 by Electronic Cats
 * - LiquidCrystal_I2C by Frank de Brabander
 * - Wire (Arduino內建)
 ******************************************************************************/

#include <Wire.h>
#include <MPU6050.h>
#include <LiquidCrystal_I2C.h>

// 定義連接的針腳
#define BUZZER_PIN 8    // 蜂鳴器針腳
#define LED_PIN 9       // LED針腳

// 定義LCD位址與大小
#define LCD_ADDRESS 0x27  // LCD的I2C位址 (可能需要調整為0x3F)
#define LCD_COLS 16       // LCD列數
#define LCD_ROWS 2        // LCD行數

// 運動檢測參數
#define MOVEMENT_THRESHOLD 0.3      // 加速度閾值 (g)
#define ROTATION_THRESHOLD 15.0     // 旋轉閾值 (度/秒)
#define WALK_THRESHOLD 0.8          // 走路加速度閾值
#define RUN_THRESHOLD 1.5           // 跑步加速度閾值
#define FALL_THRESHOLD 2.0          // 跌倒加速度閾值
#define INACTIVE_TIMEOUT 3000       // 靜止超時 (毫秒)

// 創建MPU6050與LCD物件
MPU6050 mpu;
LiquidCrystal_I2C lcd(LCD_ADDRESS, LCD_COLS, LCD_ROWS);

// 姿態角度變量
float pitch = 0;
float roll = 0;
float yaw = 0;

// 運動檢測變量
bool isMoving = false;
unsigned long lastMovementTime = 0;
unsigned long movementDuration = 0;
String activityType = "靜止";

// 濾波參數
const float ALPHA = 0.2; // 低通濾波器參數 (0-1)

// 自定義字元 (活動狀態圖示)
byte standingIcon[8] = {
  B00100,
  B01010,
  B00100,
  B00100,
  B00100,
  B01110,
  B10101,
  B00100
};

byte walkingIcon[8] = {
  B00100,
  B01010,
  B00100,
  B00100,
  B00100,
  B01110,
  B01010,
  B10001
};

byte runningIcon[8] = {
  B01000,
  B10100,
  B01000,
  B01000,
  B01000,
  B01100,
  B10010,
  B00001
};

byte fallIcon[8] = {
  B00000,
  B00000,
  B00000,
  B01110,
  B10001,
  B10001,
  B10001,
  B01110
};

// 更新間隔變量
unsigned long previousMillis = 0;
const long interval = 500;      // 更新間隔 (毫秒)

// 動作統計數據
unsigned long totalSteps = 0;
unsigned long activityStartTime = 0;
bool activityStarted = false;
unsigned long lastStepTime = 0;
const long stepTimeout = 2000;  // 步伐超時 (毫秒)

void setup() {
  // 初始化串口通信
  Serial.begin(115200);
  Wire.begin();
  
  Serial.println(F("MPU6050 運動檢測與姿態解算範例"));
  
  // 初始化LCD
  lcd.init();
  lcd.backlight();
  
  // 建立自定義字元
  lcd.createChar(0, standingIcon);
  lcd.createChar(1, walkingIcon);
  lcd.createChar(2, runningIcon);
  lcd.createChar(3, fallIcon);
  
  // 顯示歡迎訊息
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("MPU6050 Monitor");
  lcd.setCursor(0, 1);
  lcd.print("Initializing...");
  
  // 設置針腳模式
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
  
  // 初始化MPU6050
  while(!mpu.begin(MPU6050_SCALE_2000DPS, MPU6050_RANGE_2G)) {
    Serial.println(F("無法找到MPU6050感測器!"));
    
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("MPU6050 Error!");
    lcd.setCursor(0, 1);
    lcd.print("Check wiring...");
    
    delay(1000);
  }
  
  // 設置低功耗模式
  mpu.setDHPFMode(MPU6050_DHPF_5HZ);
  
  // 校準陀螺儀
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Calibrating...");
  lcd.setCursor(0, 1);
  lcd.print("Keep device still");
  
  Serial.println(F("校準陀螺儀...請保持設備靜止"));
  mpu.calibrateGyro();
  
  Serial.println(F("MPU6050初始化完成"));
  Serial.println(F("數據格式: 姿態角度, 運動狀態, 加速度, 活動時間"));

  // 初始化完成
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Ready!");
  lcd.setCursor(0, 1);
  lcd.print("Move to begin...");
  
  delay(2000);
}

void loop() {
  // 當前時間
  unsigned long currentMillis = millis();
  
  // 讀取加速度和陀螺儀數據
  Vector normAccel = mpu.readNormalizeAccel();
  Vector normGyro = mpu.readNormalizeGyro();
  
  // 計算時間差 (秒)
  float dt = 0.01; // 假設循環時間約10毫秒
  
  // 計算加速度總體大小 (去除重力)
  float accelMagnitude = sqrt(
    normAccel.XAxis * normAccel.XAxis + 
    normAccel.YAxis * normAccel.YAxis + 
    normAccel.ZAxis * normAccel.ZAxis
  );
  
  // 垂直加速度 (去除重力)
  float verticalAccel = abs(accelMagnitude - 1.0);
  
  // 從加速度計計算傾斜角 (方位角)
  float accelPitch = -(atan2(normAccel.XAxis, sqrt(normAccel.YAxis*normAccel.YAxis + normAccel.ZAxis*normAccel.ZAxis))*180.0)/M_PI;
  float accelRoll = (atan2(normAccel.YAxis, normAccel.ZAxis)*180.0)/M_PI;
  
  // 根據陀螺儀積分計算角度變化
  pitch = pitch + normGyro.XAxis * dt;
  roll = roll + normGyro.YAxis * dt;
  yaw = yaw + normGyro.ZAxis * dt;
  
  // 互補濾波: 結合加速度計和陀螺儀數據
  pitch = pitch * (1 - ALPHA) + accelPitch * ALPHA;
  roll = roll * (1 - ALPHA) + accelRoll * ALPHA;
  
  // 檢測運動
  bool movementDetected = false;
  
  // 檢查垂直加速度是否超過閾值
  if (verticalAccel > MOVEMENT_THRESHOLD) {
    movementDetected = true;
  }
  
  // 檢查旋轉是否超過閾值
  if (abs(normGyro.XAxis) > ROTATION_THRESHOLD || 
      abs(normGyro.YAxis) > ROTATION_THRESHOLD || 
      abs(normGyro.ZAxis) > ROTATION_THRESHOLD) {
    movementDetected = true;
  }
  
  // 更新運動狀態
  if (movementDetected) {
    // 如果是第一次檢測到運動，記錄開始時間
    if (!isMoving) {
      isMoving = true;
      Serial.println(F("運動開始檢測到!"));
      activityStarted = true;
      activityStartTime = currentMillis;
      digitalWrite(LED_PIN, HIGH); // 開啟LED指示運動
    }
    lastMovementTime = currentMillis;
    
    // 檢測運動類型
    if (verticalAccel > FALL_THRESHOLD) {
      // 可能是跌倒
      activityType = "跌倒!";
      // 觸發警報蜂鳴器
      tone(BUZZER_PIN, 2000, 500);
    } 
    else if (verticalAccel > RUN_THRESHOLD) {
      activityType = "跑步";
      // 記錄步數 (簡化的方式，可以通過檢測峰值更精確)
      if (currentMillis - lastStepTime > 300) { // 跑步步頻約200-300ms
        totalSteps++;
        lastStepTime = currentMillis;
      }
    } 
    else if (verticalAccel > WALK_THRESHOLD) {
      activityType = "走路";
      // 記錄步數
      if (currentMillis - lastStepTime > 500) { // 走路步頻約500-700ms
        totalSteps++;
        lastStepTime = currentMillis;
      }
    } 
    else {
      activityType = "輕動";
    }
    
    // 計算活動持續時間
    movementDuration = (currentMillis - activityStartTime) / 1000; // 轉換為秒
  } 
  else if (isMoving && (currentMillis - lastMovementTime > INACTIVE_TIMEOUT)) {
    // 如果一段時間沒有檢測到運動，標記為停止
    isMoving = false;
    activityType = "靜止";
    
    Serial.print(F("運動停止。持續時間: "));
    Serial.print(movementDuration);
    Serial.print(F(" 秒, 總步數: "));
    Serial.println(totalSteps);
    
    digitalWrite(LED_PIN, LOW); // 關閉LED
  }
  
  // 每隔一段時間更新LCD顯示和串口輸出
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    
    // 更新LCD顯示
    updateLCDDisplay(pitch, roll, yaw, activityType, movementDuration, totalSteps);
    
    // 輸出數據到串口
    Serial.print(F("姿態: Pitch="));
    Serial.print(pitch);
    Serial.print(F("° Roll="));
    Serial.print(roll);
    Serial.print(F("° Yaw="));
    Serial.print(yaw);
    Serial.print(F("° | 活動: "));
    Serial.print(activityType);
    Serial.print(F(" | 加速度: "));
    Serial.print(verticalAccel);
    Serial.print(F("g | 持續: "));
    Serial.print(movementDuration);
    Serial.print(F("秒 | 步數: "));
    Serial.println(totalSteps);
  }
  
  delay(10); // 短暫延遲，保持穩定的循環時間
}

// 更新LCD顯示
void updateLCDDisplay(float pitch, float roll, float yaw, String activity, unsigned long duration, unsigned long steps) {
  lcd.clear();
  
  // 第一行: 姿態角度
  lcd.setCursor(0, 0);
  lcd.print("P:");
  lcd.print((int)pitch);
  lcd.print(" R:");
  lcd.print((int)roll);
  
  // 顯示活動圖示
  lcd.setCursor(12, 0);
  if (activity == "靜止") {
    lcd.write(byte(0)); // 靜止圖示
  } else if (activity == "走路") {
    lcd.write(byte(1)); // 走路圖示
  } else if (activity == "跑步") {
    lcd.write(byte(2)); // 跑步圖示
  } else if (activity == "跌倒!") {
    lcd.write(byte(3)); // 跌倒圖示
  } else {
    lcd.print("*"); // 其他活動
  }
  lcd.print(" ");
  
  if (activity.length() <= 2) {
    lcd.print(activity);
  } else {
    // 顯示活動前兩個字 (中文)
    char buf[3];
    activity.toCharArray(buf, 3);
    lcd.print(buf);
  }
  
  // 第二行: 活動數據
  lcd.setCursor(0, 1);
  lcd.print("T:");
  lcd.print(duration);
  lcd.print("s");
  
  lcd.setCursor(8, 1);
  lcd.print("Steps:");
  lcd.print(steps);
}