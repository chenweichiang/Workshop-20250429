/******************************************************************************
 * 智能植物監測系統專案
 * 
 * 本專案展示如何:
 * - 使用多種感測器監測植物生長環境
 * - 土壤濕度、環境溫濕度與光照強度數據收集
 * - 通過序列監視器顯示各種環境參數
 * - 根據環境狀況提供植物照顧建議
 * 
 * 接線:
 * - 土壤濕度感測器 VCC -> Arduino 5V
 * - 土壤濕度感測器 GND -> Arduino GND
 * - 土壤濕度感測器 信號針 -> Arduino A0
 * - DHT22 VCC -> Arduino 5V
 * - DHT22 GND -> Arduino GND
 * - DHT22 DATA -> Arduino 數位針腳 2
 * - 光敏電阻 一端 -> Arduino 5V
 * - 光敏電阻 另一端 -> Arduino A1 和 10K歐姆電阻 (電阻另一端接GND)
 * - 蜂鳴器 正極 -> Arduino 數位針腳 8
 * - 蜂鳴器 負極 -> Arduino GND
 * 
 * 需要安裝的函式庫:
 * - DHT sensor library by Adafruit
 * - Adafruit Unified Sensor by Adafruit
 ******************************************************************************/

#include <DHT.h>

// 定義連接的針腳
#define SOIL_MOISTURE_PIN A0  // 土壤濕度感測器
#define LIGHT_SENSOR_PIN A1   // 光敏電阻
#define DHT_PIN 2             // DHT溫濕度感測器
#define BUZZER_PIN 8          // 蜂鳴器

// 定義DHT類型
#define DHT_TYPE DHT22        // DHT 22 (AM2302)

// 定義閾值
#define SOIL_DRY_THRESHOLD 300        // 土壤乾燥閾值
#define SOIL_WET_THRESHOLD 700        // 土壤濕潤閾值
#define TEMP_HIGH_THRESHOLD 30.0      // 高溫閾值 (攝氏度)
#define TEMP_LOW_THRESHOLD 10.0       // 低溫閾值 (攝氏度)
#define HUMIDITY_LOW_THRESHOLD 40.0   // 低濕度閾值 (%)
#define LIGHT_LOW_THRESHOLD 300       // 低光照閾值
#define LIGHT_HIGH_THRESHOLD 800      // 高光照閾值

// 讀取間隔變量 (毫秒)
const unsigned long READ_INTERVAL = 5000;  // 每5秒讀取一次數據
unsigned long lastReadTime = 0;

// 狀態變數
bool alertActive = false;             // 警報狀態
unsigned long lastAlertTime = 0;      // 上次警報時間
const unsigned long ALERT_INTERVAL = 300000;  // 警報間隔 (5分鐘)

// 創建DHT物件
DHT dht(DHT_PIN, DHT_TYPE);

void setup() {
  // 初始化串口通信
  Serial.begin(9600);
  Serial.println(F("智能植物監測系統啟動中..."));
  
  // 初始化針腳
  pinMode(SOIL_MOISTURE_PIN, INPUT);
  pinMode(LIGHT_SENSOR_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  
  // 初始化DHT感測器
  dht.begin();
  
  // 啟動提示音
  tone(BUZZER_PIN, 1000, 200);
  delay(300);
  tone(BUZZER_PIN, 1500, 200);
  
  // 系統啟動完成提示
  Serial.println(F("系統啟動完成!"));
  Serial.println(F("智能植物監測系統已就緒"));
  Serial.println(F("-------------------------------------"));
  Serial.println(F("監測參數: 土壤濕度、溫度、濕度、光照"));
  Serial.println(F("輸出格式: 時間(秒),土壤濕度,溫度(°C),濕度(%),光照強度"));
  Serial.println(F("-------------------------------------"));
  
  delay(2000);  // 等待感測器穩定
}

void loop() {
  unsigned long currentMillis = millis();
  
  // 每隔一段時間讀取一次數據
  if (currentMillis - lastReadTime >= READ_INTERVAL) {
    lastReadTime = currentMillis;
    
    // 讀取所有感測器數據
    int soilMoisture = readSoilMoisture();
    float humidity = dht.readHumidity();
    float temperature = dht.readTemperature();
    int lightLevel = readLightLevel();
    
    // 檢查DHT感測器讀取是否失敗
    if (isnan(humidity) || isnan(temperature)) {
      Serial.println(F("無法從DHT感測器讀取數據!"));
    } else {
      // 輸出時間和所有感測器數據
      Serial.print(currentMillis / 1000); // 將毫秒轉換為秒
      Serial.print(F(","));
      Serial.print(soilMoisture);
      Serial.print(F(","));
      Serial.print(temperature);
      Serial.print(F(","));
      Serial.print(humidity);
      Serial.print(F(","));
      Serial.println(lightLevel);
      
      // 顯示可讀性更好的數據
      Serial.print(F("土壤濕度: "));
      Serial.print(soilMoisture);
      Serial.print(F(" ("));
      Serial.print(getMoistureStatus(soilMoisture));
      Serial.println(F(")"));
      
      Serial.print(F("溫度: "));
      Serial.print(temperature);
      Serial.println(F("°C"));
      
      Serial.print(F("濕度: "));
      Serial.print(humidity);
      Serial.println(F("%"));
      
      Serial.print(F("光照強度: "));
      Serial.print(lightLevel);
      Serial.print(F(" ("));
      Serial.print(getLightStatus(lightLevel));
      Serial.println(F(")"));
      
      // 分析植物生長環境狀況
      analyzeEnvironment(soilMoisture, temperature, humidity, lightLevel);
      
      Serial.println(F("-------------------------------------"));
    }
  }
  
  // 檢查是否需要發出警報
  checkAlerts();
}

// 讀取土壤濕度
int readSoilMoisture() {
  // 讀取土壤濕度感測器的模擬值
  int value = analogRead(SOIL_MOISTURE_PIN);
  
  // 有些感測器是反向工作的（值越大越乾），如果需要，在此處進行反轉
  // value = 1023 - value;
  
  // 將0-1023的讀數映射到0-100的百分比
  return map(value, 0, 1023, 0, 100);
}

// 讀取光照強度
int readLightLevel() {
  // 讀取光敏電阻的模擬值
  int value = analogRead(LIGHT_SENSOR_PIN);
  
  // 將0-1023的讀數映射到0-100的百分比
  return map(value, 0, 1023, 0, 100);
}

// 獲取土壤濕度狀態描述
String getMoistureStatus(int value) {
  if (value < SOIL_DRY_THRESHOLD / 10) {
    return "非常乾燥";
  } else if (value < SOIL_DRY_THRESHOLD / 5) {
    return "乾燥";
  } else if (value < SOIL_WET_THRESHOLD / 10) {
    return "適中";
  } else if (value < SOIL_WET_THRESHOLD / 5) {
    return "濕潤";
  } else {
    return "過濕";
  }
}

// 獲取光照強度狀態描述
String getLightStatus(int value) {
  if (value < LIGHT_LOW_THRESHOLD / 10) {
    return "光照不足";
  } else if (value < LIGHT_HIGH_THRESHOLD / 10) {
    return "適中";
  } else {
    return "光照充足";
  }
}

// 分析環境並提供建議
void analyzeEnvironment(int soilMoisture, float temperature, float humidity, int lightLevel) {
  Serial.println(F("環境分析:"));
  
  // 檢查是否有任何警告條件
  bool hasWarning = false;
  
  // 土壤濕度檢查
  if (soilMoisture < SOIL_DRY_THRESHOLD / 10) {
    Serial.println(F("* 警告: 土壤過於乾燥，需要澆水"));
    hasWarning = true;
  } else if (soilMoisture > SOIL_WET_THRESHOLD / 5) {
    Serial.println(F("* 警告: 土壤過於濕潤，減少澆水"));
    hasWarning = true;
  }
  
  // 溫度檢查
  if (temperature > TEMP_HIGH_THRESHOLD) {
    Serial.println(F("* 警告: 溫度過高，可能影響植物生長"));
    hasWarning = true;
  } else if (temperature < TEMP_LOW_THRESHOLD) {
    Serial.println(F("* 警告: 溫度過低，可能影響植物生長"));
    hasWarning = true;
  }
  
  // 濕度檢查
  if (humidity < HUMIDITY_LOW_THRESHOLD) {
    Serial.println(F("* 警告: 空氣過於乾燥，建議增加濕度"));
    hasWarning = true;
  }
  
  // 光照檢查
  if (lightLevel < LIGHT_LOW_THRESHOLD / 10) {
    Serial.println(F("* 警告: 光照不足，建議移至更明亮的位置"));
    hasWarning = true;
  } else if (lightLevel > LIGHT_HIGH_THRESHOLD / 8 && temperature > 25) {
    Serial.println(F("* 提示: 光照強且溫度較高，某些植物可能需要遮陰"));
    hasWarning = true;
  }
  
  // 如果有警告，設置警報狀態
  if (hasWarning) {
    alertActive = true;
  } else {
    Serial.println(F("* 植物生長環境良好"));
    alertActive = false;
  }
}

// 檢查是否需要發出聲音警報
void checkAlerts() {
  unsigned long currentMillis = millis();
  
  // 如果警報處於活動狀態且距上次警報已過足夠時間
  if (alertActive && (currentMillis - lastAlertTime >= ALERT_INTERVAL)) {
    lastAlertTime = currentMillis;
    
    // 發出警報聲 - 三短嗶聲
    for (int i = 0; i < 3; i++) {
      tone(BUZZER_PIN, 2000, 200);
      delay(300);
    }
  }
}