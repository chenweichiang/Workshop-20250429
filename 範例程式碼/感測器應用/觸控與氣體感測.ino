/******************************************************************************
 * 觸控感測器與氣體感測器整合範例
 * 
 * 本範例展示如何:
 * - 使用觸控感測器(TTP223B)切換不同工作模式
 * - 讀取氣體感測器(MQ系列)數據
 * - 利用觸控操作實現人機界面
 * - 使用RGB LED指示不同的空氣品質等級
 * 
 * 接線:
 * - 觸控感測器(TTP223B) SIG -> Arduino 數位針腳 7
 * - 觸控感測器 VCC -> Arduino 5V
 * - 觸控感測器 GND -> Arduino GND
 * - MQ氣體感測器 A0 -> Arduino A0
 * - MQ氣體感測器 D0 -> Arduino 數位針腳 6 (可選)
 * - MQ氣體感測器 VCC -> Arduino 5V
 * - MQ氣體感測器 GND -> Arduino GND
 * - RGB LED 紅色 -> Arduino 數位針腳 9 (PWM)
 * - RGB LED 綠色 -> Arduino 數位針腳 10 (PWM)
 * - RGB LED 藍色 -> Arduino 數位針腳 11 (PWM)
 * - LCD I2C SDA -> Arduino A4
 * - LCD I2C SCL -> Arduino A5
 * - LCD I2C VCC -> Arduino 5V
 * - LCD I2C GND -> Arduino GND
 * - 蜂鳴器 -> Arduino 數位針腳 8
 * 
 * 需要安裝的函式庫:
 * - LiquidCrystal_I2C by Frank de Brabander
 * - Wire (Arduino內建)
 ******************************************************************************/

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// 定義連接的針腳
#define TOUCH_PIN 7        // 觸控感測器針腳
#define GAS_ANALOG_PIN A0  // 氣體感測器類比針腳
#define GAS_DIGITAL_PIN 6  // 氣體感測器數位針腳 (閾值輸出)
#define RGB_RED_PIN 9      // RGB LED 紅色針腳
#define RGB_GREEN_PIN 10   // RGB LED 綠色針腳
#define RGB_BLUE_PIN 11    // RGB LED 藍色針腳
#define BUZZER_PIN 8       // 蜂鳴器針腳

// 定義LCD位址與大小
#define LCD_ADDRESS 0x27  // LCD的I2C位址 (可能需要調整為0x3F)
#define LCD_COLS 16       // LCD列數
#define LCD_ROWS 2        // LCD行數

// 定義氣體感測器參數
#define GAS_LEVEL_EXCELLENT 300    // 空氣品質優秀 (類比值，範圍0-1023)
#define GAS_LEVEL_GOOD 400         // 空氣品質良好
#define GAS_LEVEL_FAIR 600         // 空氣品質一般
#define GAS_LEVEL_POOR 800         // 空氣品質差

// 定義觸控感測器參數
#define TOUCH_DEBOUNCE_MS 100         // 觸控去抖動時間 (毫秒)
#define TOUCH_LONG_PRESS_MS 2000      // 長按時間 (毫秒)
#define MODE_DISPLAY 0                // 顯示模式
#define MODE_SETTING 1                // 設定模式
#define MODE_SLEEP 2                  // 休眠模式

// 創建LCD物件
LiquidCrystal_I2C lcd(LCD_ADDRESS, LCD_COLS, LCD_ROWS);

// 運行模式與觸控變數
int currentMode = MODE_DISPLAY;
bool touchState = false;
bool lastTouchState = false;
unsigned long touchStartTime = 0;
unsigned long lastTouchChangeTime = 0;
bool longPressDetected = false;

// 氣體感測數據變數
int gasValue = 0;
String airQuality = "正在檢測...";
int airQualityLevel = 0;  // 0=優秀, 1=良好, 2=一般, 3=差

// 用於計算移動平均的變量
const int numReadings = 10;
int gasReadings[numReadings];     // 讀數的陣列
int gasReadIndex = 0;             // 當前讀數的索引
int gasTotal = 0;                 // 讀數總和
int gasAverage = 0;               // 平均讀數

// 警報變數
bool alarmEnabled = true;
bool isAlarm = false;

// 自定義字元 (空氣品質圖示)
byte excellentAirIcon[8] = {
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B11111,
  B11111,
  B11111
};

byte goodAirIcon[8] = {
  B00000,
  B00000,
  B00000,
  B00000,
  B11111,
  B11111,
  B11111,
  B11111
};

byte fairAirIcon[8] = {
  B00000,
  B00000,
  B00000,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111
};

byte poorAirIcon[8] = {
  B00000,
  B00000,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111
};

// 更新間隔變量
unsigned long previousMillis = 0;
const long displayInterval = 1000;    // 顯示更新間隔 (毫秒)
const long backlightTimeout = 30000;  // LCD背光超時 (毫秒)
unsigned long lastActivityTime = 0;    // 最後活動時間

void setup() {
  // 初始化串口通信
  Serial.begin(9600);
  Serial.println(F("觸控與氣體感測器整合範例"));
  
  // 設置針腳模式
  pinMode(TOUCH_PIN, INPUT);
  pinMode(GAS_DIGITAL_PIN, INPUT);
  pinMode(RGB_RED_PIN, OUTPUT);
  pinMode(RGB_GREEN_PIN, OUTPUT);
  pinMode(RGB_BLUE_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  
  // 初始化LCD
  lcd.init();
  lcd.backlight();
  
  // 建立自定義字元
  lcd.createChar(0, excellentAirIcon);
  lcd.createChar(1, goodAirIcon);
  lcd.createChar(2, fairAirIcon);
  lcd.createChar(3, poorAirIcon);
  
  // 顯示歡迎訊息
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Air Quality");
  lcd.setCursor(0, 1);
  lcd.print("  Initializing...");
  
  // 初始化移動平均陣列
  for (int i = 0; i < numReadings; i++) {
    gasReadings[i] = 0;
  }
  
  // 播放啟動音
  tone(BUZZER_PIN, 1000, 100);
  delay(150);
  tone(BUZZER_PIN, 1500, 100);
  
  // 測試RGB LED
  setRGB(255, 0, 0);  // 紅色
  delay(500);
  setRGB(0, 255, 0);  // 綠色
  delay(500);
  setRGB(0, 0, 255);  // 藍色
  delay(500);
  setRGB(0, 0, 0);    // 關閉
  
  // 更新最後活動時間
  lastActivityTime = millis();
  
  Serial.println(F("初始化完成"));
}

void loop() {
  // 當前時間
  unsigned long currentMillis = millis();
  
  // 讀取觸控感測器
  handleTouchSensor();
  
  // 讀取氣體感測器
  readGasSensor();
  
  // 根據操作模式執行不同操作
  switch (currentMode) {
    case MODE_DISPLAY:
      // 每隔一段時間更新顯示
      if (currentMillis - previousMillis >= displayInterval) {
        previousMillis = currentMillis;
        updateDisplayMode();
      }
      break;
      
    case MODE_SETTING:
      // 設定模式顯示與操作
      updateSettingMode();
      break;
      
    case MODE_SLEEP:
      // 休眠模式，只在喚醒時顯示
      if (touchState) {
        wakeUpFromSleep();
      }
      break;
  }
  
  // 檢查LCD背光超時
  if (currentMode != MODE_SLEEP && (currentMillis - lastActivityTime >= backlightTimeout)) {
    Serial.println(F("進入休眠模式..."));
    enterSleepMode();
  }
  
  // 處理警報
  handleAlarm();
  
  delay(50); // 短暫延遲，避免讀取過於頻繁
}

// 處理觸控感測器輸入
void handleTouchSensor() {
  // 讀取當前觸控狀態
  bool currentTouch = digitalRead(TOUCH_PIN);
  unsigned long currentTime = millis();
  
  // 去抖動處理
  if (currentTouch != lastTouchState && (currentTime - lastTouchChangeTime) > TOUCH_DEBOUNCE_MS) {
    lastTouchChangeTime = currentTime;
    lastTouchState = currentTouch;
    
    if (currentTouch) {
      // 觸控開始
      touchStartTime = currentTime;
      touchState = true;
      longPressDetected = false;
      
      // 記錄活動，重設超時
      lastActivityTime = currentTime;
      
      Serial.println(F("觸控檢測到"));
    } else {
      // 觸控結束
      touchState = false;
      
      // 如果不是長按且不在休眠模式，則切換模式
      if (!longPressDetected && currentMode != MODE_SLEEP) {
        switchMode();
      }
    }
  }
  
  // 檢測長按
  if (touchState && !longPressDetected && (currentTime - touchStartTime > TOUCH_LONG_PRESS_MS)) {
    longPressDetected = true;
    Serial.println(F("檢測到長按"));
    
    // 長按動作: 切換警報開關
    alarmEnabled = !alarmEnabled;
    
    // 播放確認音
    if (alarmEnabled) {
      tone(BUZZER_PIN, 2000, 100);
      delay(100);
      tone(BUZZER_PIN, 2000, 100);
    } else {
      tone(BUZZER_PIN, 1000, 200);
    }
    
    // 顯示警報狀態
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("警報已");
    lcd.setCursor(0, 1);
    lcd.print(alarmEnabled ? "啟用" : "禁用");
    delay(1000);
  }
}

// 切換操作模式
void switchMode() {
  currentMode = (currentMode + 1) % 3;
  
  // 播放確認音
  tone(BUZZER_PIN, 1500, 50);
  
  Serial.print(F("切換至模式: "));
  Serial.println(currentMode);
  
  // 清除LCD顯示
  lcd.clear();
  
  // 顯示新模式名稱
  lcd.setCursor(0, 0);
  switch (currentMode) {
    case MODE_DISPLAY:
      lcd.print("顯示模式");
      break;
    case MODE_SETTING:
      lcd.print("設定模式");
      break;
    case MODE_SLEEP:
      lcd.print("休眠模式");
      // 進入休眠模式
      enterSleepMode();
      break;
  }
  
  delay(500);
}

// 讀取氣體感測器
void readGasSensor() {
  // 讀取氣體感測器類比值
  int rawGasValue = analogRead(GAS_ANALOG_PIN);
  
  // 更新移動平均
  gasTotal = gasTotal - gasReadings[gasReadIndex];
  gasReadings[gasReadIndex] = rawGasValue;
  gasTotal = gasTotal + gasReadings[gasReadIndex];
  gasReadIndex = (gasReadIndex + 1) % numReadings;
  gasAverage = gasTotal / numReadings;
  
  // 更新氣體值
  gasValue = gasAverage;
  
  // 決定空氣品質等級
  if (gasValue < GAS_LEVEL_EXCELLENT) {
    airQuality = "優秀";
    airQualityLevel = 0;
    setRGB(0, 255, 0);  // 綠色
    isAlarm = false;
  } else if (gasValue < GAS_LEVEL_GOOD) {
    airQuality = "良好";
    airQualityLevel = 1;
    setRGB(0, 255, 255);  // 青色
    isAlarm = false;
  } else if (gasValue < GAS_LEVEL_FAIR) {
    airQuality = "一般";
    airQualityLevel = 2;
    setRGB(255, 255, 0);  // 黃色
    isAlarm = false;
  } else if (gasValue < GAS_LEVEL_POOR) {
    airQuality = "較差";
    airQualityLevel = 3;
    setRGB(255, 128, 0);  // 橙色
    isAlarm = false;
  } else {
    airQuality = "很差";
    airQualityLevel = 4;
    setRGB(255, 0, 0);  // 紅色
    isAlarm = true;
  }
}

// 顯示模式下的LCD更新
void updateDisplayMode() {
  // 只有在顯示模式下才更新
  if (currentMode != MODE_DISPLAY) return;
  
  lcd.clear();
  
  // 第一行顯示空氣品質
  lcd.setCursor(0, 0);
  lcd.print("空氣品質: ");
  lcd.print(airQuality);
  
  // 第二行顯示氣體值和品質條
  lcd.setCursor(0, 1);
  lcd.print("值:");
  lcd.print(gasValue);
  
  // 顯示品質等級條
  lcd.setCursor(8, 1);
  lcd.print("[");
  
  // 根據空氣品質顯示不同的條
  for (int i = 0; i <= airQualityLevel && i < 4; i++) {
    lcd.write(i);
  }
  
  for (int i = airQualityLevel + 1; i < 4; i++) {
    lcd.print(" ");
  }
  
  lcd.print("]");
  
  // 輸出到串口
  Serial.print(F("氣體值: "));
  Serial.print(gasValue);
  Serial.print(F(" | 空氣品質: "));
  Serial.println(airQuality);
}

// 設定模式的LCD更新與操作
void updateSettingMode() {
  // 第一次進入設定模式時顯示選項
  static bool settingScreenUpdated = false;
  
  if (!settingScreenUpdated) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("設定模式");
    lcd.setCursor(0, 1);
    lcd.print("1.警報 2.校準");
    
    settingScreenUpdated = true;
  }
  
  // 在設定模式切換回其他模式時重置標誌
  if (currentMode != MODE_SETTING) {
    settingScreenUpdated = false;
  }
}

// 進入休眠模式
void enterSleepMode() {
  currentMode = MODE_SLEEP;
  
  // 關閉LCD背光
  lcd.noBacklight();
  
  // 關閉RGB LED
  setRGB(0, 0, 0);
  
  // 顯示休眠信息
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("系統休眠中");
  lcd.setCursor(0, 1);
  lcd.print("觸摸感測器喚醒");
  
  Serial.println(F("進入休眠模式"));
}

// 從休眠模式喚醒
void wakeUpFromSleep() {
  // 開啟LCD背光
  lcd.backlight();
  
  // 切換到顯示模式
  currentMode = MODE_DISPLAY;
  
  // 播放喚醒音
  tone(BUZZER_PIN, 2000, 50);
  delay(100);
  tone(BUZZER_PIN, 2500, 50);
  
  // 更新最後活動時間
  lastActivityTime = millis();
  
  Serial.println(F("從休眠模式喚醒"));
}

// 設置RGB LED顏色
void setRGB(int red, int green, int blue) {
  analogWrite(RGB_RED_PIN, red);
  analogWrite(RGB_GREEN_PIN, green);
  analogWrite(RGB_BLUE_PIN, blue);
}

// 處理警報
void handleAlarm() {
  // 如果警報未啟用或不需要警報，直接返回
  if (!alarmEnabled || !isAlarm) {
    return;
  }
  
  // 只在顯示模式和氣體值超過閾值時警報
  if (currentMode == MODE_DISPLAY && gasValue >= GAS_LEVEL_POOR) {
    // 每隔一段時間發出警報聲
    unsigned long currentTime = millis();
    if ((currentTime / 1000) % 2 == 0) {
      // 閃爍RGB LED (紅色)
      if ((currentTime / 250) % 2 == 0) {
        setRGB(255, 0, 0);
      } else {
        setRGB(0, 0, 0);
      }
      
      // 發出警報聲
      if ((currentTime / 500) % 2 == 0) {
        tone(BUZZER_PIN, 2000, 100);
      }
    }
  }
}