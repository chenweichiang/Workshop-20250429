/******************************************************************************
 * 土壤濕度感測器基礎範例
 * 
 * 本範例展示如何:
 * - 讀取土壤濕度感測器數據
 * - 將讀數轉換為百分比
 * - 判斷不同濕度狀態
 * - 設置自動澆水觸發閾值
 * 
 * 接線:
 * - 土壤濕度感測器 A0 -> Arduino A0
 * - 土壤濕度感測器 D0 -> Arduino 數位針腳 7 (可選，數位閾值輸出)
 * - 土壤濕度感測器 VCC -> Arduino 5V/3.3V
 * - 土壤濕度感測器 GND -> Arduino GND
 * - LED -> Arduino 數位針腳 8 (指示乾燥狀態)
 * 
 * 說明: 大多數土壤濕度感測器在乾燥時輸出較高的值，濕潤時輸出較低的值
 ******************************************************************************/

// 定義連接的針腳
#define SOIL_MOISTURE_ANALOG_PIN A0  // 土壤濕度感測器類比輸入
#define SOIL_MOISTURE_DIGITAL_PIN 7  // 土壤濕度感測器數位輸入 (部分型號有此功能)
#define LED_PIN 8                   // 指示LED

// 定義濕度閾值 (根據您的感測器和土壤類型調整)
#define SOIL_DRY_VALUE 800   // 乾燥土壤的閾值 (類比值，範圍0-1023)
#define SOIL_WET_VALUE 300   // 濕潤土壤的閾值 (類比值，範圍0-1023)

// 用於計算移動平均的變量
const int numReadings = 10;
int readings[numReadings];      // 讀數的陣列
int readIndex = 0;              // 當前讀數的索引
int total = 0;                  // 讀數總和
int average = 0;                // 平均讀數

// 狀態顯示更新間隔
unsigned long previousMillis = 0;
const long interval = 2000;     // 更新間隔 (毫秒)

void setup() {
  Serial.begin(9600);
  Serial.println(F("土壤濕度感測器測試範例"));
  
  // 設置針腳模式
  pinMode(SOIL_MOISTURE_DIGITAL_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);
  
  // 初始化移動平均陣列
  for (int i = 0; i < numReadings; i++) {
    readings[i] = 0;
  }
}

void loop() {
  // 讀取土壤濕度感測器類比值
  int moistureRaw = analogRead(SOIL_MOISTURE_ANALOG_PIN);
  
  // 讀取土壤濕度感測器數位值 (如果有)
  bool moistureDigital = digitalRead(SOIL_MOISTURE_DIGITAL_PIN);
  
  // 更新移動平均
  total = total - readings[readIndex];
  readings[readIndex] = moistureRaw;
  total = total + readings[readIndex];
  readIndex = (readIndex + 1) % numReadings;
  average = total / numReadings;
  
  // 計算濕度百分比 (轉換類比值範圍)
  // 注意: 大多數土壤濕度感測器在乾燥時輸出較高的值，所以我們反轉映射
  int moisturePercent = map(average, SOIL_DRY_VALUE, SOIL_WET_VALUE, 0, 100);
  
  // 限制百分比範圍為0-100
  moisturePercent = constrain(moisturePercent, 0, 100);
  
  // 判斷土壤濕度狀態
  String moistureStatus;
  if (average > SOIL_DRY_VALUE) {
    moistureStatus = "乾燥";
    digitalWrite(LED_PIN, HIGH);  // 點亮LED表示乾燥
  } 
  else if (average < SOIL_WET_VALUE) {
    moistureStatus = "濕潤";
    digitalWrite(LED_PIN, LOW);   // 關閉LED
  } 
  else {
    moistureStatus = "適中";
    digitalWrite(LED_PIN, LOW);   // 關閉LED
  }
  
  // 每隔一段時間顯示狀態
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    
    // 輸出讀數資訊
    Serial.print(F("土壤濕度原始值: "));
    Serial.print(average);
    Serial.print(F(" | 百分比: "));
    Serial.print(moisturePercent);
    Serial.print(F("% | 狀態: "));
    Serial.print(moistureStatus);
    
    // 如果有數位輸出，也顯示它的狀態
    Serial.print(F(" | 數位狀態: "));
    Serial.println(moistureDigital ? F("乾燥") : F("濕潤"));
    
    // 未來擴展：如果達到乾燥閾值，可以在這裡添加自動澆水功能
    // if (moisturePercent < 20) {
    //   // 啟動澆水裝置
    // }
  }
  
  delay(100); // 短暫延遲，避免讀取過於頻繁
}