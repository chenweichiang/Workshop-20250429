# AI輔助Arduino感測器進階應用工作坊
版本：1.2.0
最後更新：2025/04/03

## AI輔助感測器進階應用 (10:00-12:00)

### AI輔助工具與Arduino感測器開發介紹 (10:00-10:15)
- AI輔助開發工具在感測器應用中的優勢
  - Cursor與GitHub Copilot功能對比
  - AI提示詞優化技巧與示範
- 感測器開發常見難題與AI解決方案
  - 使用AI分析感測器數據模式
  - AI協助排除感測器故障與干擾
- 進階AI提示技巧
  - 感測器開發的專業提示詞構建
  - 引導AI生成高品質代碼的關鍵要素

### 感測器資料分析與視覺化 (10:15-10:35)
- 感測器數據特性分析
  - 識別噪音與有效數據
  - 使用AI分析數據趨勢與模式
- 數據視覺化技術
  - 使用AI生成序列圖表顯示代碼
  - 設計資料閾值與觸發條件
- 實時數據處理技巧
  - 使用移動平均和其他濾波方法
  - AI輔助數據異常檢測算法設計

### 多感測器融合技術 (10:35-11:00)
- 感測器協同工作原理
  - 多感測器同步讀取技術
  - 不同感測器數據標準化方法
- AI輔助多感測器整合
  - 使用AI設計感測器協同邏輯
  - 解決感測器衝突與優先級問題
- 實用案例解析
  - 光線與動作感測結合
  - 溫度與濕度綜合監測系統

### 感測器校準與濾波 (11:00-11:20)
- 感測器校準原理與方法
  - 線性和非線性校準技術
  - 使用AI生成自動校準代碼
- 數據濾波與平滑處理
  - 使用AI設計數字濾波器
  - 突波抑制與雜訊消除技巧
- 高精度讀取技術
  - 過採樣與數據平均
  - AI輔助信號處理與增強

### 互動裝置實作工作坊 (11:20-11:45)
#### 專案：智能環境監測與反應系統
- 專案目標與構思
  - 使用多種感測器監測環境
  - 根據感測器數據觸發智能反應

- 所需材料
  - Arduino Uno/相容板
  - 光敏電阻（環境光線檢測）
  - 按鈕或傾斜感測器（互動觸發）
  - LED（多種輸出反應）
  - 電阻套件
  - 麵包板與跳線

- AI輔助開發流程
  1. **專案架構設計**
     - 使用AI繪製系統架構
     - 定義感測器與執行器間的邏輯關係
  
  2. **多感測器協同工作**
     - 使用AI生成多感測器同步讀取代碼
     - 設計感測器數據優先級處理邏輯
  
  3. **智能反應機制**
     - 使用AI設計環境適應性算法
     - 實現複雜的條件反應邏輯

  4. **優化與調試**
     - AI協助識別效能瓶頸
     - 改進代碼結構與感測器響應時間

### 成果展示與進階技巧分享 (11:45-12:00)
- 學員作品展示與經驗分享
  - 實際應用中的挑戰與解決方案
  - AI輔助開發的心得交流

- 感測器開發常見問題與解決策略
  - 感測器飽和與範圍擴展技巧
  - 電源穩定與參考電壓優化

- 學習資源與未來探索方向
  - 推薦進階感測器學習資源
  - 感測器與機器學習結合的可能性

## 感測器進階技術參考代碼

### 1. 多感測器數據融合示例
```cpp
// AI生成的多感測器融合代碼
const int lightSensorPin = A0;    // 光敏電阻
const int tempSensorPin = A1;     // 溫度感測器
const int buttonPin = 2;          // 按鈕輸入
const int ledPin = 9;             // LED輸出

// 感測器閾值設定
const int LIGHT_THRESHOLD = 500;  // 光線閾值
const int TEMP_THRESHOLD = 30;    // 溫度閾值（攝氏度）

// 數據處理變量
int lightValues[10];              // 光線數據緩衝區
int tempValues[10];               // 溫度數據緩衝區
int bufferIndex = 0;              // 緩衝區索引

void setup() {
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(ledPin, OUTPUT);
  Serial.begin(9600);
  
  Serial.println("多感測器融合系統初始化完成");
  Serial.println("格式: 光線值, 溫度值, 按鈕狀態, LED狀態");
}

void loop() {
  // 讀取感測器數據
  int lightValue = analogRead(lightSensorPin);
  
  // 將溫度感測器讀數轉換為攝氏度 (適用於TMP36感測器)
  int tempReading = analogRead(tempSensorPin);
  float voltage = tempReading * 5.0 / 1024.0;
  float tempCelsius = (voltage - 0.5) * 100;
  
  int buttonState = digitalRead(buttonPin);
  
  // 更新數據緩衝區
  lightValues[bufferIndex] = lightValue;
  tempValues[bufferIndex] = tempCelsius;
  bufferIndex = (bufferIndex + 1) % 10;  // 循環緩衝區
  
  // 計算平均值進行濾波
  int avgLight = calculateAverage(lightValues, 10);
  float avgTemp = calculateAverage(tempValues, 10);
  
  // 融合邏輯決策
  boolean ledState = false;
  
  // 條件1: 光線暗且溫度高
  if (avgLight < LIGHT_THRESHOLD && avgTemp > TEMP_THRESHOLD) {
    ledState = true;
  }
  
  // 條件2: 按鈕按下同時推翻之前的決定
  if (buttonState == LOW) {
    ledState = !ledState;
  }
  
  // 執行決策
  digitalWrite(ledPin, ledState);
  
  // 輸出狀態
  Serial.print(avgLight);
  Serial.print(", ");
  Serial.print(avgTemp);
  Serial.print("°C, ");
  Serial.print(buttonState == LOW ? "Pressed" : "Released");
  Serial.print(", ");
  Serial.println(ledState ? "ON" : "OFF");
  
  delay(100);  // 適度延遲以穩定讀數
}

// 計算數組平均值的輔助函數
float calculateAverage(int values[], int size) {
  long sum = 0;
  for (int i = 0; i < size; i++) {
    sum += values[i];
  }
  return (float)sum / size;
}
```

### 2. 感測器自動校準示例
```cpp
// AI生成的感測器自動校準代碼
const int sensorPin = A0;      // 感測器連接針腳
const int ledPin = 13;         // 校準指示燈

// 校準變量
int minValue = 1023;           // 記錄最小值，初始為最大可能值
int maxValue = 0;              // 記錄最大值，初始為最小可能值
int calibratedMin = 0;         // 校準後的最小值
int calibratedMax = 1023;      // 校準後的最大值
bool isCalibrating = true;     // 校準狀態
unsigned long calibrationStartTime = 0;  // 校準開始時間
const unsigned long CALIBRATION_DURATION = 5000;  // 校準持續5秒

void setup() {
  pinMode(ledPin, OUTPUT);
  Serial.begin(9600);
  Serial.println("感測器校準系統啟動");
  Serial.println("請在LED閃爍期間展示感測器的全部範圍...");
  
  // 開始校準
  calibrationStartTime = millis();
  digitalWrite(ledPin, HIGH);  // 打開LED表示校準中
}

void loop() {
  // 檢查是否在校準模式
  if (isCalibrating) {
    // 讀取感測器並更新範圍
    int sensorValue = analogRead(sensorPin);
    if (sensorValue < minValue) minValue = sensorValue;
    if (sensorValue > maxValue) maxValue = sensorValue;
    
    // 每500ms閃爍LED指示校準進行中
    digitalWrite(ledPin, ((millis() - calibrationStartTime) / 250) % 2);
    
    // 顯示校準進度
    Serial.print("校準中: ");
    Serial.print(sensorValue);
    Serial.print(" (");
    Serial.print(minValue);
    Serial.print("-");
    Serial.print(maxValue);
    Serial.println(")");
    
    // 檢查校準是否完成
    if (millis() - calibrationStartTime >= CALIBRATION_DURATION) {
      isCalibrating = false;
      digitalWrite(ledPin, LOW);  // 關閉LED表示校準完成
      
      // 設置校準範圍，加入少量邊距
      calibratedMin = minValue + (maxValue - minValue) * 0.05;  // 5%邊距
      calibratedMax = maxValue - (maxValue - minValue) * 0.05;  // 5%邊距
      
      Serial.println("\n校準完成!");
      Serial.print("感測器範圍: ");
      Serial.print(minValue);
      Serial.print(" - ");
      Serial.println(maxValue);
      Serial.print("實際使用範圍: ");
      Serial.print(calibratedMin);
      Serial.print(" - ");
      Serial.println(calibratedMax);
      Serial.println("現在進入正常操作模式...\n");
    }
  } 
  else {
    // 正常操作模式
    int rawValue = analogRead(sensorPin);
    
    // 應用校準 - 將原始值映射到0-100範圍
    int calibratedValue = map(rawValue, calibratedMin, calibratedMax, 0, 100);
    
    // 限制在有效範圍內
    calibratedValue = constrain(calibratedValue, 0, 100);
    
    // 顯示結果
    Serial.print("原始值: ");
    Serial.print(rawValue);
    Serial.print(", 校準後百分比: ");
    Serial.print(calibratedValue);
    Serial.println("%");
    
    delay(200);  // 減少輸出頻率
  }
}
```

### 3. 進階數據濾波器示例
```cpp
// AI生成的進階數字濾波器
const int sensorPin = A0;       // 感測器輸入
const int outputPin = 9;        // 輸出針腳

// 濾波器配置
const int WINDOW_SIZE = 16;     // 移動窗口大小
int readings[WINDOW_SIZE];      // 讀數數組
int readIndex = 0;              // 當前索引
long total = 0;                 // 總和

// 中值濾波器參數
const int MEDIAN_SIZE = 5;      // 中值過濾窗口大小
int medianValues[MEDIAN_SIZE];  // 中值過濾數據

// 卡爾曼濾波參數
float kalmanEstimate = 0;       // 估計值
float kalmanError = 1;          // 估計誤差
float kalmanMeasurement = 0;    // 測量值
float kalmanQ = 0.01;           // 過程噪聲
float kalmanR = 0.1;            // 測量噪聲
float kalmanGain = 0;           // 卡爾曼增益

void setup() {
  pinMode(outputPin, OUTPUT);
  Serial.begin(9600);
  
  // 初始化讀數數組
  for (int i = 0; i < WINDOW_SIZE; i++) {
    readings[i] = 0;
  }
  
  Serial.println("進階數字濾波器系統啟動");
  Serial.println("格式: 原始值, 移動平均, 中值, 卡爾曼濾波");
}

void loop() {
  // 讀取感測器原始值
  int rawValue = analogRead(sensorPin);
  
  // 1. 移動平均濾波
  total = total - readings[readIndex];
  readings[readIndex] = rawValue;
  total = total + readings[readIndex];
  readIndex = (readIndex + 1) % WINDOW_SIZE;
  int movingAverage = total / WINDOW_SIZE;
  
  // 2. 中值濾波
  for (int i = 0; i < MEDIAN_SIZE-1; i++) {
    medianValues[i] = medianValues[i+1];
  }
  medianValues[MEDIAN_SIZE-1] = rawValue;
  int medianValue = calculateMedian(medianValues, MEDIAN_SIZE);
  
  // 3. 卡爾曼濾波
  kalmanMeasurement = rawValue;
  
  // 預測步驟
  // 這裡使用簡化的卡爾曼濾波器，假設系統模型為靜態
  
  // 更新步驟
  kalmanError = kalmanError + kalmanQ;
  kalmanGain = kalmanError / (kalmanError + kalmanR);
  kalmanEstimate = kalmanEstimate + kalmanGain * (kalmanMeasurement - kalmanEstimate);
  kalmanError = (1 - kalmanGain) * kalmanError;
  
  // 輸出所有濾波結果以進行比較
  Serial.print(rawValue);
  Serial.print(", ");
  Serial.print(movingAverage);
  Serial.print(", ");
  Serial.print(medianValue);
  Serial.print(", ");
  Serial.println(kalmanEstimate);
  
  // 使用卡爾曼濾波結果控制輸出（比如LED亮度）
  analogWrite(outputPin, map(kalmanEstimate, 0, 1023, 0, 255));
  
  delay(50);  // 適度延遲
}

// 計算中值的輔助函數
int calculateMedian(int values[], int size) {
  // 創建臨時數組進行排序而不修改原數組
  int temp[size];
  for (int i = 0; i < size; i++) {
    temp[i] = values[i];
  }
  
  // 簡單的冒泡排序
  for (int i = 0; i < size-1; i++) {
    for (int j = 0; j < size-i-1; j++) {
      if (temp[j] > temp[j+1]) {
        int swap = temp[j];
        temp[j] = temp[j+1];
        temp[j+1] = swap;
      }
    }
  }
  
  // 返回中間值
  return temp[size/2];
}
```

## AI提示範例（感測器進階應用）

1. "設計一個自動校準系統，能夠在啟動時自動校準光敏電阻的最大和最小值。"

2. "開發一個數字濾波器，能同時使用移動平均、中值濾波和卡爾曼濾波處理感測器數據，並比較不同結果。"

3. "創建一個系統同時讀取光敏電阻和傾斜感測器，當環境變暗且檢測到傾斜時，以不同頻率閃爍LED。"

4. "設計一個自適應閾值算法，能夠根據環境條件的變化自動調整感測器觸發閾值。"

5. "開發一個多模式感測器監測系統，有'節能模式'、'標準模式'和'高精度模式'，每種模式有不同的採樣頻率和處理方法。"

6. "實現一個感測器數據異常檢測系統，能夠識別並過濾感測器突發的不合理數據。"

7. "設計一個基於感測器數據的預測算法，能夠提前預測環境變化趨勢並做出反應。"

---

*最後更新：2025年4月3日*