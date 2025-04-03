/******************************************************************************
 * DHT溫濕度感測器基礎範例
 * 
 * 本範例展示如何:
 * - 讀取DHT11/DHT22溫濕度感測器數據
 * - 顯示在序列監視器上
 * - 透過序列繪圖器繪製溫度和濕度數據圖表
 * 
 * 接線:
 * - DHT11/DHT22 VCC -> Arduino 5V
 * - DHT11/DHT22 GND -> Arduino GND
 * - DHT11/DHT22 DATA -> Arduino 數位針腳 2
 * (建議在DATA和VCC之間接一個10K電阻)
 * 
 * 需要安裝的函式庫:
 * - DHT sensor library by Adafruit
 * - Adafruit Unified Sensor by Adafruit
 ******************************************************************************/

#include <DHT.h>

// 定義連接的針腳
#define DHT_PIN 2      // DHT11/DHT22連接的針腳

// 定義DHT類型 (取消註解你使用的類型)
#define DHT_TYPE DHT11   // DHT 11
//#define DHT_TYPE DHT22   // DHT 22 (AM2302, AM2321)
//#define DHT_TYPE DHT21   // DHT 21 (AM2301)

// 定義DHT物件
DHT dht(DHT_PIN, DHT_TYPE);

// 讀取間隔變量 (DHT11/DHT22讀取頻率不應超過2秒)
unsigned long previousMillis = 0;
const long interval = 2000;  // 讀取間隔 (毫秒)

void setup() {
  // 初始化串口通信
  Serial.begin(9600);
  Serial.println(F("DHT溫濕度感測器示範"));
  
  // 初始化DHT感測器
  dht.begin();
  
  // 等待感測器準備就緒
  delay(2000);
  
  // 序列繪圖器標題
  Serial.println("溫度 (°C),濕度 (%)");
}

void loop() {
  // 當前時間
  unsigned long currentMillis = millis();
  
  // 每隔一段時間讀取數據
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    
    // 讀取濕度
    float humidity = dht.readHumidity();
    // 讀取溫度 (攝氏度)
    float temperatureC = dht.readTemperature();
    // 讀取溫度 (華氏度)
    float temperatureF = dht.readTemperature(true);
    
    // 檢查讀取是否失敗
    if (isnan(humidity) || isnan(temperatureC) || isnan(temperatureF)) {
      Serial.println(F("無法從DHT感測器讀取數據!"));
      return;
    }
    
    // 計算熱指數 (華氏度)
    float heatIndexF = dht.computeHeatIndex(temperatureF, humidity);
    // 計算熱指數 (攝氏度)
    float heatIndexC = dht.computeHeatIndex(temperatureC, humidity, false);
    
    // 輸出數據到序列監視器
    Serial.print(F("濕度: "));
    Serial.print(humidity);
    Serial.print(F("% 溫度: "));
    Serial.print(temperatureC);
    Serial.print(F("°C "));
    Serial.print(temperatureF);
    Serial.print(F("°F 熱指數: "));
    Serial.print(heatIndexC);
    Serial.print(F("°C "));
    Serial.print(heatIndexF);
    Serial.println(F("°F"));
    
    // 為序列繪圖器輸出格式化數據 (逗號分隔)
    Serial.print(temperatureC);
    Serial.print(",");
    Serial.println(humidity);
  }
}