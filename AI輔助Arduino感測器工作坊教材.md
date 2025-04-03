# AI輔助Arduino新型感測器進階應用工作坊
版本：1.3.0
最後更新：2025/04/03

## AI輔助新型感測器進階應用 (10:00-12:00)

### AI輔助工具與Arduino新型感測器開發介紹 (10:00-10:15)
- AI輔助開發工具在新型感測器應用中的優勢
  - Cursor與GitHub Copilot功能對比
  - 感測器專用AI提示詞優化技巧
- 新型感測器開發常見難題與AI解決方案
  - 使用AI處理多變量感測器數據
  - AI協助排除感測器校準與干擾問題
- 進階AI提示技巧
  - 環境與運動感測器的專業提示詞構建
  - 引導AI生成高品質校準與濾波代碼

### 環境感測技術與數據視覺化 (10:15-10:35)
- 溫濕度感測器技術深入探討
  - DHT11與DHT22原理與差異
  - OneWire溫度感測器數據處理
- 大氣環境參數監測
  - BMP280氣壓感測器使用技巧
  - 綜合環境數據校準與補償
- 環境數據視覺化
  - LCD顯示環境參數與趨勢圖
  - 使用AI設計多參數視覺化界面

### 運動與姿態感測技術 (10:35-11:00)
- MPU6050加速度陀螺儀模組應用
  - 6軸數據獲取與處理原理
  - 加速度與角速度數據融合
- 運動模式識別與分析
  - 使用AI生成運動模式識別算法
  - 動作閾值檢測與觸發
- 特殊感測器協同應用
  - 震動感測器(SW-420)與MPU6050數據融合
  - 觸摸感測器(TTP223B)與姿態感測結合

### 感測器校準與濾波 (11:00-11:20)
- 環境感測器校準技術
  - DHT22溫濕度與BMP280感測器校準方法
  - 使用參考感測器進行交叉校準
- 運動感測器數據處理
  - MPU6050零點漂移補償
  - 姿態解算與互補濾波
- 進階數據濾波技術
  - 卡爾曼濾波器應用於運動數據
  - 低通/高通濾波器與數字濾波

### 互動裝置實作工作坊 (11:20-11:45)
#### 專案：智能環境與運動監測系統
- 專案目標與構思
  - 創建兼具環境監測與運動檢測功能的系統
  - 通過AI輔助設計數據處理邏輯

- 所需材料
  - Arduino Uno/相容板
  - DHT22溫濕度感測器
  - MPU6050加速度陀螺儀模組
  - 土壤濕度感測器或氣體感測器(選用)
  - LCD顯示模組或OLED顯示屏
  - 電阻與配件套件
  - 麵包板與跳線

- AI輔助開發流程
  1. **系統架構設計**
     - 使用AI設計感測器連接與數據流
     - 定義不同感測器功能優先級
  
  2. **環境感測模塊開發**
     - 使用AI生成DHT22與BMP280數據讀取代碼
     - 設計溫濕度與氣壓趨勢分析算法
  
  3. **運動檢測模塊開發**
     - 使用AI開發MPU6050姿態解算代碼
     - 實現運動模式識別與閾值檢測
  
  4. **系統整合與優化**
     - AI協助設計多感測器數據融合算法
     - 優化顯示界面與交互邏輯

### 成果展示與進階技巧分享 (11:45-12:00)
- 學員作品展示與經驗分享
  - 實際應用中的挑戰與解決方案
  - AI輔助開發的經驗反饋
- 新型感測器開發常見問題與解決策略
  - I2C通訊問題排查技巧
  - 多感測器干擾與隔離方法
- 學習資源與未來探索方向
  - 推薦進階感測器學習資源
  - 物聯網與環境監測系統發展趨勢

## 新型感測器進階技術參考代碼

### 1. DHT22溫濕度感測器示例
```cpp
// AI生成的DHT22溫濕度感測器代碼
#include <DHT.h>
#include <LiquidCrystal_I2C.h>

#define DHTPIN 2       // DHT22連接到數位引腳2
#define DHTTYPE DHT22  // 使用DHT22感測器

// 初始化LCD (地址, 列數, 行數)
LiquidCrystal_I2C lcd(0x27, 16, 2);

// 初始化DHT感測器
DHT dht(DHTPIN, DHTTYPE);

// 數據緩衝區用於計算移動平均
float tempReadings[10] = {0};
float humReadings[10] = {0};
int readIndex = 0;

// 溫度單位轉換函數
float celsiusToFahrenheit(float celsius) {
  return celsius * 1.8 + 32.0;
}

void setup() {
  Serial.begin(9600);
  
  // 初始化LCD
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("DHT22 Monitor");
  
  // 初始化DHT感測器
  dht.begin();
  
  Serial.println("DHT22溫濕度監測系統初始化完成");
  Serial.println("格式: 溫度(C), 濕度(%), 露點(C)");
  
  delay(2000);
}

void loop() {
  // 等待2秒以確保穩定讀數
  delay(2000);
  
  // 讀取溫度與濕度
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();
  
  // 檢查讀取是否成功
  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("無法從DHT感測器讀取數據!");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Sensor Error!");
    return;
  }
  
  // 計算露點
  float dewPoint = temperature - (100 - humidity) / 5;
  
  // 更新移動平均緩衝區
  tempReadings[readIndex] = temperature;
  humReadings[readIndex] = humidity;
  readIndex = (readIndex + 1) % 10;
  
  // 計算移動平均
  float avgTemp = 0;
  float avgHum = 0;
  for (int i = 0; i < 10; i++) {
    avgTemp += tempReadings[i];
    avgHum += humReadings[i];
  }
  avgTemp /= 10;
  avgHum /= 10;
  
  // 在LCD顯示溫度和濕度
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Temp: ");
  lcd.print(avgTemp, 1);
  lcd.print((char)223); // 度數符號
  lcd.print("C");
  
  lcd.setCursor(0, 1);
  lcd.print("Humidity: ");
  lcd.print(avgHum, 1);
  lcd.print("%");
  
  // 輸出至序列監視器
  Serial.print("溫度: ");
  Serial.print(temperature);
  Serial.print("°C | ");
  Serial.print(celsiusToFahrenheit(temperature));
  Serial.print("°F | 濕度: ");
  Serial.print(humidity);
  Serial.print("% | 露點: ");
  Serial.print(dewPoint);
  Serial.println("°C");
  
  // 檢查溫度警告條件
  if (temperature > 30) {
    Serial.println("警告: 溫度過高!");
    // 在這裡可以加入警報或其他反應
  }
}
```

### 2. MPU6050姿態感測示例
```cpp
// AI生成的MPU6050姿態感測與運動檢測代碼
#include <Wire.h>
#include <MPU6050.h>

MPU6050 mpu;

// 運動閾值設定
const float MOVEMENT_THRESHOLD = 0.3;  // g力加速度閾值
const float ROTATION_THRESHOLD = 15.0;  // 度/秒閾值

// 姿態變量
float pitch = 0;
float roll = 0;
float yaw = 0;

// 運動檢測變量
bool isMoving = false;
unsigned long lastMovementTime = 0;
int movementDuration = 0;

// 數據過濾參數
const float ALPHA = 0.2; // 低通濾波器參數

void setup() {
  Serial.begin(115200);
  Wire.begin();
  
  Serial.println("初始化MPU6050...");
  
  // 初始化MPU6050
  while(!mpu.begin(MPU6050_SCALE_2000DPS, MPU6050_RANGE_2G)) {
    Serial.println("無法找到MPU6050感測器!");
    delay(500);
  }
  
  // 校準陀螺儀
  mpu.calibrateGyro();
  
  // 設置低功耗模式
  mpu.setDHPFMode(MPU6050_DHPF_5HZ);
  
  Serial.println("MPU6050初始化完成!");
  Serial.println("格式: Pitch, Roll, AccX, AccY, AccZ, GyroX, GyroY, GyroZ, 運動狀態");
}

void loop() {
  // 讀取原始加速度計和陀螺儀數據
  Vector rawAccel = mpu.readRawAccel();
  Vector normAccel = mpu.readNormalizeAccel();
  Vector rawGyro = mpu.readRawGyro();
  Vector normGyro = mpu.readNormalizeGyro();
  
  // 計算加速度總體大小
  float accelMagnitude = sqrt(
    normAccel.XAxis * normAccel.XAxis + 
    normAccel.YAxis * normAccel.YAxis + 
    normAccel.ZAxis * normAccel.ZAxis
  );
  
  // 檢測是否存在運動
  bool movementDetected = false;
  
  // 檢查加速度是否超過閾值
  if (abs(accelMagnitude - 1.0) > MOVEMENT_THRESHOLD) {
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
    if (!isMoving) {
      isMoving = true;
      Serial.println("運動開始檢測到!");
    }
    lastMovementTime = millis();
  } else if (isMoving && (millis() - lastMovementTime > 1000)) {
    // 如果1秒內沒有檢測到運動，標記為停止
    isMoving = false;
    movementDuration = (millis() - lastMovementTime) / 1000;
    Serial.print("運動停止。持續時間: ");
    Serial.print(movementDuration);
    Serial.println(" 秒");
  }
  
  // 計算姿態角
  // 使用簡化的互補濾波
  float dt = 0.01; // 假設循環時間約10毫秒
  
  // 從加速度計計算傾斜角
  float accelPitch = -(atan2(normAccel.XAxis, sqrt(normAccel.YAxis*normAccel.YAxis + normAccel.ZAxis*normAccel.ZAxis))*180.0)/M_PI;
  float accelRoll = (atan2(normAccel.YAxis, normAccel.ZAxis)*180.0)/M_PI;
  
  // 根據陀螺儀積分計算角度變化
  pitch = pitch + normGyro.XAxis * dt;
  roll = roll + normGyro.YAxis * dt;
  yaw = yaw + normGyro.ZAxis * dt;
  
  // 互補濾波: 結合加速度計和陀螺儀數據
  pitch = pitch * (1 - ALPHA) + accelPitch * ALPHA;
  roll = roll * (1 - ALPHA) + accelRoll * ALPHA;
  
  // 輸出處理後的數據
  Serial.print("Pitch: ");
  Serial.print(pitch);
  Serial.print("° Roll: ");
  Serial.print(roll);
  Serial.print("° Yaw: ");
  Serial.print(yaw);
  Serial.print("° | ");
  
  Serial.print("Accel: ");
  Serial.print(normAccel.XAxis);
  Serial.print(", ");
  Serial.print(normAccel.YAxis);
  Serial.print(", ");
  Serial.print(normAccel.ZAxis);
  Serial.print(" | ");
  
  Serial.print("Gyro: ");
  Serial.print(normGyro.XAxis);
  Serial.print(", ");
  Serial.print(normGyro.YAxis);
  Serial.print(", ");
  Serial.print(normGyro.ZAxis);
  Serial.print(" | ");
  
  Serial.print("狀態: ");
  Serial.println(isMoving ? "運動中" : "靜止");
  
  delay(10);
}
```

### 3. 土壤濕度與氣體感測示例
```cpp
// AI生成的土壤濕度與氣體感測器融合代碼
#include <LiquidCrystal_I2C.h>

// 感測器連接PIN
#define SOIL_MOISTURE_PIN A0  // 土壤濕度感測器模擬輸入
#define GAS_SENSOR_PIN A1     // MQ氣體感測器模擬輸入
#define DIGITAL_SOIL_PIN 7    // 土壤濕度數位輸出（乾燥閾值）
#define BUZZER_PIN 8          // 蜂鳴器
#define LED_PIN 9             // 提示LED

// LCD初始化 (I2C地址, 列數, 行數)
LiquidCrystal_I2C lcd(0x27, 16, 2);

// 感測器閾值設定
#define SOIL_DRY_THRESHOLD 700     // 乾燥閾值（模擬值）
#define SOIL_WET_THRESHOLD 400     // 濕潤閾值（模擬值）
#define GAS_WARNING_THRESHOLD 500  // 氣體濃度警告閾值

// 數據採集變量
int soilMoistureValue = 0;
int gasSensorValue = 0;
int soilMoisturePercent = 0;

// 數據緩衝區
const int BUFFER_SIZE = 10;
int soilBuffer[BUFFER_SIZE];
int gasBuffer[BUFFER_SIZE];
int bufferIndex = 0;

// 警報狀態
bool moistureAlarmState = false;
bool gasAlarmState = false;

// 時間變量
unsigned long lastReadTime = 0;
unsigned long lastDisplayTime = 0;
unsigned long lastLogTime = 0;

void setup() {
  Serial.begin(9600);
  
  // 初始化LCD
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Soil & Gas");
  lcd.setCursor(0, 1);
  lcd.print("Monitoring System");
  
  // 設置引腳模式
  pinMode(DIGITAL_SOIL_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
  
  // 初始化緩衝區
  for (int i = 0; i < BUFFER_SIZE; i++) {
    soilBuffer[i] = 0;
    gasBuffer[i] = 0;
  }
  
  // 等待感測器穩定
  delay(2000);
  
  Serial.println("土壤濕度與氣體濃度監測系統初始化完成");
  Serial.println("格式: 土壤濕度(原始值), 土壤濕度(%), 土壤狀態, 氣體濃度, 氣體狀態");
}

void loop() {
  unsigned long currentMillis = millis();
  
  // 每500毫秒讀取一次感測器數據
  if (currentMillis - lastReadTime >= 500) {
    lastReadTime = currentMillis;
    
    // 讀取土壤濕度感測器
    soilMoistureValue = analogRead(SOIL_MOISTURE_PIN);
    bool digitalSoilState = digitalRead(DIGITAL_SOIL_PIN);
    
    // 讀取氣體感測器
    gasSensorValue = analogRead(GAS_SENSOR_PIN);
    
    // 將土壤濕度轉換為百分比 (假設0-1023對應0-100%)
    // 注意：某些土壤濕度感測器在乾燥時值較高，潮濕時值較低
    soilMoisturePercent = map(soilMoistureValue, 1023, 0, 0, 100);
    // 限制在0-100範圍內
    soilMoisturePercent = constrain(soilMoisturePercent, 0, 100);
    
    // 更新緩衝區
    soilBuffer[bufferIndex] = soilMoistureValue;
    gasBuffer[bufferIndex] = gasSensorValue;
    bufferIndex = (bufferIndex + 1) % BUFFER_SIZE;
    
    // 計算平均值進行濾波
    int soilAvg = 0;
    int gasAvg = 0;
    for (int i = 0; i < BUFFER_SIZE; i++) {
      soilAvg += soilBuffer[i];
      gasAvg += gasBuffer[i];
    }
    soilAvg /= BUFFER_SIZE;
    gasAvg /= BUFFER_SIZE;
    
    // 檢查土壤濕度警報條件
    if (soilAvg > SOIL_DRY_THRESHOLD && !moistureAlarmState) {
      moistureAlarmState = true;
      triggerAlarm(true, "土壤過於乾燥!");
    } 
    else if (soilAvg < SOIL_WET_THRESHOLD && moistureAlarmState) {
      moistureAlarmState = false;
      stopAlarm("土壤濕度已回復正常");
    }
    
    // 檢查氣體感測器警報條件
    if (gasAvg > GAS_WARNING_THRESHOLD && !gasAlarmState) {
      gasAlarmState = true;
      triggerAlarm(true, "空氣品質異常!");
    } 
    else if (gasAvg < GAS_WARNING_THRESHOLD - 50 && gasAlarmState) {
      gasAlarmState = false;
      stopAlarm("空氣品質已回復正常");
    }
  }
  
  // 每1000毫秒更新一次LCD顯示
  if (currentMillis - lastDisplayTime >= 1000) {
    lastDisplayTime = currentMillis;
    
    // 計算平均值
    int soilAvg = 0;
    int gasAvg = 0;
    for (int i = 0; i < BUFFER_SIZE; i++) {
      soilAvg += soilBuffer[i];
      gasAvg += gasBuffer[i];
    }
    soilAvg /= BUFFER_SIZE;
    gasAvg /= BUFFER_SIZE;
    
    // 第一行顯示土壤濕度
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Soil: ");
    lcd.print(soilMoisturePercent);
    lcd.print("%");
    
    // 第二行顯示氣體感測器讀數與狀態
    lcd.setCursor(0, 1);
    lcd.print("Gas: ");
    lcd.print(gasAvg);
    
    lcd.setCursor(11, 1);
    if (gasAvg > GAS_WARNING_THRESHOLD) {
      lcd.print("WARN!");
    } else {
      lcd.print("OK");
    }
    
    // 每10秒記錄一次數據
    if (currentMillis - lastLogTime >= 10000) {
      lastLogTime = currentMillis;
      
      Serial.print("土壤濕度: ");
      Serial.print(soilAvg);
      Serial.print(" (");
      Serial.print(soilMoisturePercent);
      Serial.print("%)");
      
      if (soilAvg > SOIL_DRY_THRESHOLD) {
        Serial.print(" - 乾燥");
      } 
      else if (soilAvg < SOIL_WET_THRESHOLD) {
        Serial.print(" - 濕潤");
      } 
      else {
        Serial.print(" - 適中");
      }
      
      Serial.print(" | 氣體濃度: ");
      Serial.print(gasAvg);
      
      if (gasAvg > GAS_WARNING_THRESHOLD) {
        Serial.println(" - 警告!");
      } else {
        Serial.println(" - 正常");
      }
    }
  }
  
  // 如果有任何警報，控制蜂鳴器
  if (moistureAlarmState || gasAlarmState) {
    digitalWrite(LED_PIN, ((currentMillis / 500) % 2) ? HIGH : LOW); // LED閃爍
    if ((currentMillis / 1000) % 2 == 0) { // 每2秒蜂鳴一次
      tone(BUZZER_PIN, 1000, 100);
    }
  } else {
    digitalWrite(LED_PIN, LOW);
    noTone(BUZZER_PIN);
  }
}

// 觸發警報函數
void triggerAlarm(bool playSound, String message) {
  Serial.print("警報觸發: ");
  Serial.println(message);
  
  if (playSound) {
    tone(BUZZER_PIN, 1000, 1000);
  }
}

// 停止警報函數
void stopAlarm(String message) {
  Serial.print("警報解除: ");
  Serial.println(message);
  noTone(BUZZER_PIN);
}
```

## AI提示範例（新型感測器應用）

1. "使用DHT22溫濕度感測器創建環境監測系統，當溫度超過28度或濕度超過80%時發出警報。"

2. "設計一個使用MPU6050加速度陀螺儀模組的姿態檢測系統，能夠識別靜止、走路和跑步三種狀態。"

3. "創建一個智能植物監測站，使用土壤濕度感測器和DHT22監測植物生長環境，通過LCD顯示器顯示數據。"

4. "實現一個使用震動感測器(SW-420)的安全監控系統，能夠檢測異常振動並通過LCD顯示和蜂鳴器提示。"

5. "開發一個空氣品質監測系統，使用MQ-135氣體感測器測量空氣污染，並通過LED指示不同污染等級。"

6. "設計一個人體互動裝置，使用觸摸感測器(TTP223B)和MPU6050檢測不同的手勢和動作，控制LED的不同顯示模式。"

7. "實現一個氣象站，結合DHT22溫濕度感測器和BMP280氣壓感測器，顯示當前氣象數據並預測天氣變化。"

8. "開發一個智能門禁系統，使用霍爾效應感測器檢測磁性物體靠近，結合溫濕度感測器監控環境，並通過LCD顯示狀態。"

9. "設計一個運動數據記錄器，使用MPU6050記錄運動軌跡和強度，並使用SD卡儲存數據以供後續分析。"

10. "創建一個環境感應藝術裝置，根據DHT22溫濕度感測器和觸摸感測器的輸入，控制LED燈的亮度、顏色和閃爍模式。"

---

*最後更新：2025年4月3日*