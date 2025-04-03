# Arduino 基礎教學 - PWM 控制 LED 亮度詳細說明

## 範例目標
透過這個範例，您將學習如何使用 Arduino 的 PWM (脈衝寬度調變) 功能來控制 LED 的亮度變化，實現漸亮漸暗的效果。這是 Arduino 類比輸出的入門範例。

## 需要的材料
- Arduino Uno 或相容板
- 麵包板
- LED 一個 (顏色不限)
- 220Ω 電阻一個
- 連接線若干

## 電路連接步驟
1. **連接 LED**
   - LED 長腳（正極）接 Arduino 的 9 號腳位（必須選擇支援 PWM 的腳位，在 Arduino Uno 上有 ~符號標記的腳位：3, 5, 6, 9, 10, 11）
   - LED 短腳（負極）經過 220Ω 電阻後接 Arduino 的 GND

   ![PWM LED連接示意圖](請放入PWM LED連接的圖片連結)

   > **提示**：Arduino Uno 上支援 PWM 的腳位會在板子上標有 "~" 符號，例如 ~9, ~10 等。

## 程式碼說明

```cpp
const int ledPin = 9;  // LED連接到支援PWM的針腳

void setup() {
  pinMode(ledPin, OUTPUT);  // 設置LED針腳為輸出
}

void loop() {
  // 漸亮
  for (int brightness = 0; brightness <= 255; brightness++) {
    analogWrite(ledPin, brightness);  // 設置LED亮度
    delay(10);                       // 小延遲使變化更明顯
  }
  
  // 漸暗
  for (int brightness = 255; brightness >= 0; brightness--) {
    analogWrite(ledPin, brightness);  // 設置LED亮度
    delay(10);                       // 小延遲使變化更明顯
  }
}
```

### 程式碼詳解
- 變數宣告
  - `const int ledPin = 9;` 定義 LED 連接的腳位為 9 號腳位（必須是支援 PWM 的腳位）

- `setup()` 函數
  - `pinMode(ledPin, OUTPUT);` 設定 LED 腳位為輸出模式

- `loop()` 函數
  - **漸亮效果**:
    - 使用 `for` 迴圈讓 `brightness` 變數從 0 逐漸增加到 255
    - `analogWrite(ledPin, brightness);` 使用 PWM 輸出控制 LED 亮度
    - `delay(10);` 短暫延遲，讓亮度變化速度適中
  
  - **漸暗效果**:
    - 使用 `for` 迴圈讓 `brightness` 變數從 255 逐漸減少到 0
    - 同樣使用 `analogWrite()` 和短暫延遲

## PWM 原理簡介
PWM (脈衝寬度調變) 是 Arduino 實現類比輸出的一種方式。Arduino 的數位輸出只能是高電平 (5V) 或低電平 (0V)，但透過 PWM 技術，可以在一段時間內快速切換高低電平，並通過調整高電平的持續時間（稱為"占空比"），產生不同的平均電壓，進而控制 LED 的亮度。

- 占空比為 0% 時，LED 完全熄滅
- 占空比為 100% 時，LED 最亮
- 占空比在中間值時，LED 顯示相應的亮度

在 Arduino 中，`analogWrite()` 函數接受 0-255 的值來設定 PWM 輸出：
- 0 = 0% 占空比（完全關閉）
- 255 = 100% 占空比（完全打開）
- 127 = 約 50% 占空比（半亮）

## 上傳步驟
1. 使用 USB 線將 Arduino 連接到電腦
2. 在 Arduino IDE 中選擇正確的開發板型號和 COM 埠
3. 點擊上傳按鈕 (右箭頭圖示)
4. 等待上傳完成，觀察 LED 是否呈現漸亮漸暗的效果

## 進一步實驗
1. **調整變化速度**：修改 `delay()` 的數值，觀察亮度變化速度的不同
   ```cpp
   delay(5);  // 變化更快
   ```
   或
   ```cpp
   delay(50);  // 變化更慢
   ```

2. **不同的亮度變化模式**：嘗試使用非線性的亮度變化，例如正弦波模式
   ```cpp
   // 在loop函數中
   for (int angle = 0; angle < 360; angle++) {
     // 使用正弦函數產生亮度值，從0到255
     float radian = angle * 3.14159 / 180;  // 角度轉弧度
     int brightness = 128 + 127 * sin(radian);  // 產生0-255之間的值
     
     analogWrite(ledPin, brightness);
     delay(10);
   }
   ```

3. **多個 LED 控制**：連接多個 LED 到不同的 PWM 腳位，產生交錯的漸變效果
   ```cpp
   const int ledPin1 = 9;
   const int ledPin2 = 10;
   
   void setup() {
     pinMode(ledPin1, OUTPUT);
     pinMode(ledPin2, OUTPUT);
   }
   
   void loop() {
     // LED 1 漸亮，LED 2 漸暗
     for (int i = 0; i <= 255; i++) {
       analogWrite(ledPin1, i);
       analogWrite(ledPin2, 255 - i);
       delay(10);
     }
     
     // LED 1 漸暗，LED 2 漸亮
     for (int i = 255; i >= 0; i--) {
       analogWrite(ledPin1, i);
       analogWrite(ledPin2, 255 - i);
       delay(10);
     }
   }
   ```

## 常見問題
1. **LED 不亮或不會變化**
   - 確認 LED 連接的是支援 PWM 的腳位（Arduino Uno 上有 ~符號標記的腳位）
   - 檢查 LED 的正負極是否接對
   - 確認程式已成功上傳到 Arduino

2. **亮度變化太快或太慢**
   - 調整程式中 `delay()` 函數的數值：增加數值使變化更慢，減少數值使變化更快

## 要點總結
- PWM 是 Arduino 實現類比輸出的一種方式，可用於控制 LED 亮度、馬達速度等
- `analogWrite()` 函數接受 0-255 的值來設定 PWM 輸出的占空比
- Arduino Uno 上只有帶有 ~ 符號的腳位支援 PWM 輸出，共 6 個腳位：3, 5, 6, 9, 10, 11
- 迴圈可用於創建漸變效果，配合適當的延遲使變化更加平滑