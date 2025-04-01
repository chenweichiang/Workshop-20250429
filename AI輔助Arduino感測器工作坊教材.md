# AI輔助Arduino感測器互動工作坊 - 單日課程
版本：1.1.0
最後更新：2025/04/01

## Arduino+AI基礎與實作 (10:00-12:00)

### Cursor與GitHub Copilot AI工具介紹 (10:00-10:20)
- AI輔助開發工具介紹
  - Cursor：基於VS Code的AI增強版編輯器
  - GitHub Copilot：程式碼生成與協助工具
- 工具安裝與設置
  - Cursor下載與安裝
  - GitHub Copilot設置與登入
- 基本AI提示技巧
  - 使用自然語言描述需求
  - 使用註解引導AI生成程式碼
  - AI對話功能（Ctrl+K/Cmd+K開啟）

### Arduino基礎概念與環境設置 (10:20-10:40)
- Arduino簡介
  - 開源電子原型平台
  - 適合初學者的互動原型開發
- 硬體認識
  - Arduino Uno板子介紹
  - 數位針腳與類比針腳
  - 麵包板使用入門
- 環境設置
  - Arduino IDE與Cursor連接設定
  - 選擇正確的開發板和連接埠

### 使用AI生成基礎Arduino程式碼 (10:40-11:00)
- AI程式碼生成實例
  - 向AI描述：「寫一個LED閃爍程式」
  - 向AI提問：「如何讀取類比感測器？」
- 使用AI解釋程式碼
  - 選擇不熟悉的程式碼段落
  - 要求AI解釋每一行的功能
- 使用AI修改現有程式
  - 變更LED閃爍頻率
  - 增加按鈕控制功能

### 基礎感測器互動示範 (11:00-11:20)
- LED控制示範
  - 基本接線方式
  - 使用AI生成不同閃爍模式
- 按鈕輸入示範
  - 按鈕接線與防彈跳技巧
  - 使用AI生成按鈕控制程式
- 光敏電阻應用
  - 接線方式與原理說明
  - AI協助解讀感測器數值
  
### 光感應互動裝置實作 (11:20-11:45)
#### 專案說明
設計一個根據環境光線自動調整亮度的燈光裝置，並使用AI工具協助開發。

#### 所需材料
- Arduino Uno/相容板
- LED（紅色或RGB）
- 光敏電阻
- 按鈕 × 1
- 220Ω 電阻 × 1（LED用）
- 10KΩ 電阻 × 1（光敏電阻用）
- 麵包板與跳線

#### 實作步驟
1. **硬體連接**（由AI提供指導）
   - 使用AI繪製接線圖
   - 按照圖示連接元件

2. **AI輔助程式開發**
   - 向AI描述專案需求
   - 讓AI生成基本程式架構
   - 根據實際需求調整程式

3. **測試與優化**
   - 測試裝置功能
   - 使用AI協助除錯
   - 增加新功能（如閃爍模式）

### 成果展示與總結 (11:45-12:00)
#### 展示與分享
- 學員展示完成的互動裝置
- 分享使用AI工具的經驗與心得

#### 總結與後續學習建議
- 回顧工作坊重要概念
- 討論AI輔助Arduino開發的可能性
- 提供延伸學習資源與建議

## 實用參考範例

### 1. LED控制範例
```cpp
// AI生成的LED控制程式
const int ledPin = 13;  // 使用13號針腳

void setup() {
  pinMode(ledPin, OUTPUT);  // 設置為輸出模式
}

void loop() {
  digitalWrite(ledPin, HIGH);  // 開啟LED
  delay(1000);                 // 延遲1秒
  digitalWrite(ledPin, LOW);   // 關閉LED
  delay(1000);                 // 延遲1秒
}
```

### 2. 光敏電阻讀取範例
```cpp
// AI生成的光敏電阻讀取程式
const int sensorPin = A0;  // 連接至A0針腳
int sensorValue = 0;       // 存儲讀取值

void setup() {
  Serial.begin(9600);  // 初始化序列通訊
}

void loop() {
  sensorValue = analogRead(sensorPin);  // 讀取類比值
  Serial.print("光線值: ");
  Serial.println(sensorValue);          // 輸出至序列監視器
  delay(500);                           // 延遲0.5秒
}
```

### 3. 光控LED亮度範例
```cpp
// AI生成的光控LED亮度程式
const int ledPin = 9;      // 使用支援PWM的針腳
const int sensorPin = A0;  // 光敏電阻連接至A0
int sensorValue = 0;       // 存儲光敏電阻讀值
int brightness = 0;        // 存儲LED亮度值

void setup() {
  pinMode(ledPin, OUTPUT);  // 設定LED針腳為輸出
  Serial.begin(9600);       // 初始化序列通訊
}

void loop() {
  sensorValue = analogRead(sensorPin);            // 讀取光線值
  brightness = map(sensorValue, 0, 1023, 255, 0); // 將光線值映射為亮度值
                                                  // 光線越弱，LED越亮
  analogWrite(ledPin, brightness);                // 設定LED亮度
  Serial.print("光線值: ");
  Serial.print(sensorValue);
  Serial.print(", 亮度: ");
  Serial.println(brightness);
  delay(100);                                     // 小延遲使輸出更穩定
}
```

## AI提示範例
以下是在Cursor中可以使用的AI提示範例：

1. "使用光敏電阻控制LED的亮度，當光線越暗時，LED越亮。"

2. "當按下按鈕時，LED開始閃爍，再次按下時停止閃爍。"

3. "設計一個有三種模式的LED控制程式：常亮模式、閃爍模式和呼吸模式。"

4. "讀取光敏電阻的數值，並在序列監視器上顯示'明亮'、'適中'或'黑暗'。"

5. "使用按鈕控制的簡易電子琴，有四個按鈕分別對應不同音調。"

---

*最後更新：2025年4月1日*