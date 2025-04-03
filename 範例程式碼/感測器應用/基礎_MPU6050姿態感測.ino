/******************************************************************************
 * MPU6050姿態感測器基礎範例
 * 
 * 本範例展示如何:
 * - 初始化MPU6050感測器
 * - 讀取加速度與陀螺儀數據
 * - 計算簡單的傾角
 * - 在序列監視器上顯示數據
 * 
 * 接線:
 * - MPU6050 VCC -> Arduino 5V/3.3V (MPU6050支援3.3V或5V)
 * - MPU6050 GND -> Arduino GND
 * - MPU6050 SCL -> Arduino A5 (或SCL)
 * - MPU6050 SDA -> Arduino A4 (或SDA)
 * - MPU6050 INT -> 不連接(本範例未使用中斷)
 * 
 * 需要安裝的函式庫:
 * - 'Adafruit MPU6050' by Adafruit
 * - 'Adafruit Unified Sensor' by Adafruit
 * - 'Adafruit BusIO' by Adafruit
 ******************************************************************************/

#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

// 創建MPU6050物件
Adafruit_MPU6050 mpu;

// 讀取間隔變量
unsigned long previousMillis = 0;
const long interval = 100;  // 讀取間隔 (毫秒)

// 簡單互補濾波器的參數
float accelAngleX = 0, accelAngleY = 0;
float gyroAngleX = 0, gyroAngleY = 0, gyroAngleZ = 0;
float compAngleX = 0, compAngleY = 0;
float gyroXrate = 0, gyroYrate = 0, gyroZrate = 0;
float dt = 0.01;  // 時間增量，與interval相匹配
float alpha = 0.96;  // 濾波器係數

void setup() {
  // 初始化串口通信
  Serial.begin(115200);
  while (!Serial)
    delay(10); // 等待串口通信建立

  Serial.println(F("MPU6050姿態感測器示範"));

  // 初始化I2C通信
  Wire.begin();

  // 嘗試初始化MPU6050
  if (!mpu.begin()) {
    Serial.println(F("找不到MPU6050晶片"));
    while (1) {
      delay(10);
    }
  }
  Serial.println(F("MPU6050找到了!"));

  // 設定感測器範圍
  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);

  Serial.println("");
  delay(100);

  // 顯示配置信息
  displaySensorDetails();
}

void loop() {
  // 當前時間
  unsigned long currentMillis = millis();
  
  // 每隔一段時間讀取數據
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    
    // 獲取新數據
    sensors_event_t a, g, temp;
    mpu.getEvent(&a, &g, &temp);
    
    // 計算加速度計角度 (使用atan2計算更準確)
    accelAngleX = atan2(a.acceleration.y, sqrt(a.acceleration.x * a.acceleration.x + a.acceleration.z * a.acceleration.z)) * 180 / PI;
    accelAngleY = atan2(-a.acceleration.x, sqrt(a.acceleration.y * a.acceleration.y + a.acceleration.z * a.acceleration.z)) * 180 / PI;
    
    // 從陀螺儀獲取角速率 (deg/s)
    gyroXrate = g.gyro.x * 180 / PI;
    gyroYrate = g.gyro.y * 180 / PI;
    gyroZrate = g.gyro.z * 180 / PI;
    
    // 使用陀螺儀數據計算角度變化
    gyroAngleX += gyroXrate * dt;
    gyroAngleY += gyroYrate * dt;
    gyroAngleZ += gyroZrate * dt;
    
    // 互補濾波器 - 結合加速度計和陀螺儀數據
    compAngleX = alpha * (compAngleX + gyroXrate * dt) + (1 - alpha) * accelAngleX;
    compAngleY = alpha * (compAngleY + gyroYrate * dt) + (1 - alpha) * accelAngleY;
    
    // 輸出原始加速度數據
    Serial.print(F("加速度 X: "));
    Serial.print(a.acceleration.x);
    Serial.print(F(", Y: "));
    Serial.print(a.acceleration.y);
    Serial.print(F(", Z: "));
    Serial.print(a.acceleration.z);
    Serial.println(F(" m/s^2"));
    
    // 輸出原始陀螺儀數據
    Serial.print(F("陀螺儀 X: "));
    Serial.print(g.gyro.x);
    Serial.print(F(", Y: "));
    Serial.print(g.gyro.y);
    Serial.print(F(", Z: "));
    Serial.print(g.gyro.z);
    Serial.println(F(" rad/s"));
    
    // 輸出計算的傾角
    Serial.print(F("角度 X: "));
    Serial.print(compAngleX);
    Serial.print(F(", Y: "));
    Serial.print(compAngleY);
    Serial.print(F(", Z: "));
    Serial.print(gyroAngleZ);
    Serial.println(F(" deg"));
    
    // 輸出溫度數據
    Serial.print(F("溫度: "));
    Serial.print(temp.temperature);
    Serial.println(F(" degC"));
    
    Serial.println(F(""));
  }
}

// 顯示感測器詳情
void displaySensorDetails() {
  sensor_t accel, gyro, temp;
  mpu.getSensor(&accel, &gyro, &temp);
  
  Serial.println(F("------------------------------------"));
  Serial.println(F("加速度計:"));
  Serial.print(F("感測器: ")); Serial.println(accel.name);
  Serial.print(F("驅動版本: ")); Serial.println(accel.version);
  Serial.print(F("唯一ID: ")); Serial.println(accel.sensor_id);
  Serial.print(F("最大值: ")); Serial.print(accel.max_value); Serial.println(F(" m/s^2"));
  Serial.print(F("最小值: ")); Serial.print(accel.min_value); Serial.println(F(" m/s^2"));
  Serial.print(F("解析度: ")); Serial.print(accel.resolution); Serial.println(F(" m/s^2"));
  Serial.println(F("------------------------------------"));
  Serial.println(F(""));
  
  Serial.println(F("------------------------------------"));
  Serial.println(F("陀螺儀:"));
  Serial.print(F("感測器: ")); Serial.println(gyro.name);
  Serial.print(F("驅動版本: ")); Serial.println(gyro.version);
  Serial.print(F("唯一ID: ")); Serial.println(gyro.sensor_id);
  Serial.print(F("最大值: ")); Serial.print(gyro.max_value); Serial.println(F(" rad/s"));
  Serial.print(F("最小值: ")); Serial.print(gyro.min_value); Serial.println(F(" rad/s"));
  Serial.print(F("解析度: ")); Serial.print(gyro.resolution); Serial.println(F(" rad/s"));
  Serial.println(F("------------------------------------"));
  Serial.println(F(""));
  
  Serial.println(F("------------------------------------"));
  Serial.println(F("溫度:"));
  Serial.print(F("感測器: ")); Serial.println(temp.name);
  Serial.print(F("驅動版本: ")); Serial.println(temp.version);
  Serial.print(F("唯一ID: ")); Serial.println(temp.sensor_id);
  Serial.print(F("最大值: ")); Serial.print(temp.max_value); Serial.println(F(" degC"));
  Serial.print(F("最小值: ")); Serial.print(temp.min_value); Serial.println(F(" degC"));
  Serial.print(F("解析度: ")); Serial.print(temp.resolution); Serial.println(F(" degC"));
  Serial.println(F("------------------------------------"));
  Serial.println(F(""));
}