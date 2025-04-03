/******************************************************************************
 * DHT22 溫濕度感測器數據視覺化進階範例
 * 
 * 本範例展示如何:
 * - 讀取DHT22溫濕度感測器數據
 * - 使用LCD顯示器視覺化溫濕度數據
 * - 計算舒適度指數與露點
 * - 實現數據平滑化與警報功能
 * 
 * 接線:
 * - DHT22 數據針腳 -> Arduino 數位針腳 2
 * - DHT22 VCC -> Arduino 5V
 * - DHT22 GND -> Arduino GND
 * - LCD I2C SDA -> Arduino A4
 * - LCD I2C SCL -> Arduino A5
 * - LCD I2C VCC -> Arduino 5V
 * - LCD I2C GND -> Arduino GND
 * - 警報蜂鳴器 -> Arduino 數位針腳 8
 * - 狀態LED -> Arduino 數位針腳 9
 * 
 * 需要安裝的函式庫:
 * - DHT sensor library by Adafruit
 * - LiquidCrystal_I2C by Frank de Brabander
 ******************************************************************************/

#include <DHT.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

// 定義連接的針腳與感測器類型
#define DHTPIN 2        // DHT22連接到數位針腳2
#define DHTTYPE DHT22   // DHT 22 (AM2302, AM2321)
#define BUZZER_PIN 8    // 蜂鳴器針腳
#define LED_PIN 9       // LED針腳

// 定義LCD位址與大小
#define LCD_ADDRESS 0x27  // LCD的I2C位址 (可能需要調整為0x3F)
#define LCD_COLS 16       // LCD列數
#define LCD_ROWS 2        // LCD行數

// 定義警報閾值
#define TEMP_HIGH_THRESHOLD 30.0  // 高溫警報閾值 (°C)
#define HUMID_HIGH_THRESHOLD 70.0 // 高濕度警報閾值 (%)
#define HUMID_LOW_THRESHOLD 25.0  // 低濕度警報閾值 (%)

// 初始化DHT感測器與LCD
DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal_I2C lcd(LCD_ADDRESS, LCD_COLS, LCD_ROWS);

// 計算移動平均的變量
const int numReadings = 10;
float tempReadings[numReadings] = {0};  // 溫度讀數陣列
float humReadings[numReadings] = {0};   // 濕度讀數陣列
int readIndex = 0;                     // 當前讀數索引

// 自定義字元 (溫度、濕度、警告圖示)
byte tempIcon[8] = {
  B00100,
  B01010,
  B01010,
  B01010,
  B01010,
  B10001,
  B10001,
  B01110
};

byte humidIcon[8] = {
  B00100,
  B00100,
  B01010,
  B01010,
  B10001,
  B10001,
  B10001,
  B01110
};

byte alertIcon[8] = {
  B00000,
  B00100,
  B01110,
  B01110,
  B01110,
  B00100,
  B00000,
  B00100
};

// 更新間隔變量
unsigned long previousMillis = 0;
const long interval = 2000;     // 更新間隔 (毫秒)

// 警報狀態
bool alarmActive = false;

void setup() {
  // 初始化串口通信
  Serial.begin(9600);
  Serial.println(F("DHT22溫濕度數據視覺化範例"));
  
  // 初始化LCD
  lcd.init();
  lcd.backlight();
  
  // 建立自定義字元
  lcd.createChar(0, tempIcon);
  lcd.createChar(1, humidIcon);
  lcd.createChar(2, alertIcon);
  
  // 顯示歡迎訊息
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("DHT22 Monitor");
  lcd.setCursor(0, 1);
  lcd.print("Initializing...");
  
  // 設置針腳模式
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
  
  // 初始化DHT感測器
  dht.begin();
  
  // 等待感測器穩定
  delay(2000);
}

void loop() {
  // 檢查是否到了更新時間
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    
    // 讀取溫度與濕度數據
    float humidity = dht.readHumidity();
    float temperature = dht.readTemperature();
    
    // 檢查讀取是否成功
    if (isnan(humidity) || isnan(temperature)) {
      Serial.println(F("無法從DHT感測器讀取數據!"));
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Sensor Error!");
      return;
    }
    
    // 更新移動平均
    tempReadings[readIndex] = temperature;
    humReadings[readIndex] = humidity;
    readIndex = (readIndex + 1) % numReadings;
    
    // 計算移動平均值
    float avgTemp = 0;
    float avgHum = 0;
    for (int i = 0; i < numReadings; i++) {
      avgTemp += tempReadings[i];
      avgHum += humReadings[i];
    }
    avgTemp /= numReadings;
    avgHum /= numReadings;
    
    // 計算舒適度指數 (簡化版)
    // 基於溫度與濕度的線性組合，範圍從0(不舒適)到100(舒適)
    int comfortIndex = 100 - (abs(avgTemp - 25) * 3 + abs(avgHum - 50) / 2);
    comfortIndex = constrain(comfortIndex, 0, 100);
    
    // 計算露點溫度
    float dewPoint = avgTemp - ((100 - avgHum) / 5);
    
    // 檢查警報條件
    bool tempAlarm = avgTemp > TEMP_HIGH_THRESHOLD;
    bool humidHighAlarm = avgHum > HUMID_HIGH_THRESHOLD;
    bool humidLowAlarm = avgHum < HUMID_LOW_THRESHOLD;
    
    // 設置警報狀態
    alarmActive = tempAlarm || humidHighAlarm || humidLowAlarm;
    
    // 更新LCD顯示
    updateLCDDisplay(avgTemp, avgHum, dewPoint, comfortIndex);
    
    // 更新警報輸出
    updateAlarm(alarmActive);
    
    // 輸出詳細數據到串口
    Serial.print(F("溫度: "));
    Serial.print(avgTemp);
    Serial.print(F("°C, 濕度: "));
    Serial.print(avgHum);
    Serial.print(F("%, 露點: "));
    Serial.print(dewPoint);
    Serial.print(F("°C, 舒適度: "));
    Serial.print(comfortIndex);
    Serial.println(F("%"));
    
    // 如果有警報，輸出警報信息
    if (alarmActive) {
      Serial.print(F("警報: "));
      if (tempAlarm) Serial.print(F("高溫 "));
      if (humidHighAlarm) Serial.print(F("高濕度 "));
      if (humidLowAlarm) Serial.print(F("低濕度 "));
      Serial.println();
    }
  }
  
  // 如果警報處於活動狀態，控制蜂鳴器和LED
  if (alarmActive) {
    // 每500毫秒閃爍一次LED
    digitalWrite(LED_PIN, ((millis() / 500) % 2) ? HIGH : LOW);
    
    // 每2秒發出一次短蜂鳴聲
    if ((millis() / 2000) % 2 == 0) {
      tone(BUZZER_PIN, 2000, 100);
    }
  } else {
    // 關閉警報輸出
    digitalWrite(LED_PIN, LOW);
    noTone(BUZZER_PIN);
  }
  
  delay(100); // 短暫延遲，避免過於頻繁的讀取
}

// 更新LCD顯示
void updateLCDDisplay(float temp, float humid, float dewPoint, int comfort) {
  lcd.clear();
  
  // 第一行: 溫度與濕度
  lcd.setCursor(0, 0);
  lcd.write(byte(0)); // 溫度圖示
  lcd.print(" ");
  lcd.print(temp, 1);
  lcd.print((char)223); // 度數符號
  lcd.print("C ");
  
  lcd.setCursor(9, 0);
  lcd.write(byte(1)); // 濕度圖示
  lcd.print(" ");
  lcd.print(humid, 1);
  lcd.print("%");
  
  // 第二行: 舒適度與警報狀態
  lcd.setCursor(0, 1);
  lcd.print("Cmf:");
  lcd.print(comfort);
  lcd.print("%");
  
  // 如果有警報，顯示警報圖示
  if (alarmActive) {
    lcd.setCursor(8, 1);
    lcd.write(byte(2)); // 警報圖示
    lcd.print(" Alert!");
  } else {
    lcd.setCursor(8, 1);
    lcd.print("DewP:");
    lcd.print(dewPoint, 1);
  }
}

// 更新警報狀態
void updateAlarm(bool active) {
  if (active) {
    // 警報啟動時的操作
    digitalWrite(LED_PIN, HIGH);
  } else {
    // 警報關閉時的操作
    digitalWrite(LED_PIN, LOW);
    noTone(BUZZER_PIN);
  }
}