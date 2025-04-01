# Arduino 基礎教學 - LED 閃爍範例詳細說明

## 範例目標
透過這個範例，您將學習如何控制 Arduino 上的 LED 燈閃爍，這是 Arduino 程式設計的基本入門範例，讓您了解 Arduino 的基本運作方式。

## 需要的材料
- Arduino Uno 或相容板
- 麵包板
- LED 一個 (顏色不限，建議使用紅色)
- 220Ω 電阻一個
- 連接線若干

## 電路連接步驟
1. **連接 LED**
   - LED 有兩支腳，一長一短。長腳為正極，短腳為負極
   - LED 長腳（正極）接 Arduino 的 13 號腳位
   - LED 短腳（負極）經過 220Ω 電阻後接 Arduino 的 GND

   ![LED連接示意圖](請放入LED連接的圖片連結)

   > **提示**：如果沒有額外的 LED，也可以使用 Arduino 上板載的 LED，它連接到數位腳位 13，此時不需要額外接線。

## 程式碼說明

```cpp
void setup() {
  // 初始化數位針腳 13 為輸出
  pinMode(13, OUTPUT);
}

void loop() {
  digitalWrite(13, HIGH);  // 開啟 LED
  delay(1000);             // 等待 1 秒
  digitalWrite(13, LOW);   // 關閉 LED
  delay(1000);             // 等待 1 秒
}
```

### 程式碼詳解
- `setup()` 函數
  - 在 Arduino 啟動時只會執行一次
  - `pinMode(13, OUTPUT)` 設定 13 號腳位為輸出模式，用來控制 LED

- `loop()` 函數
  - 在 `setup()` 執行完後會不斷重複執行
  - `digitalWrite(13, HIGH)` 將 13 號腳位設為高電平 (5V)，LED 點亮
  - `delay(1000)` 暫停程式 1000 毫秒 (1 秒)
  - `digitalWrite(13, LOW)` 將 13 號腳位設為低電平 (0V)，LED 熄滅
  - `delay(1000)` 再次暫停程式 1 秒
  - 程式重複執行，使 LED 不斷閃爍

## 上傳步驟
1. 使用 USB 線將 Arduino 連接到電腦
2. 在 Arduino IDE 中選擇正確的開發板型號和 COM 埠
   - 工具 > 開發板 > Arduino Uno
   - 工具 > 埠 > (選擇已連接的 Arduino 埠)
3. 點擊上傳按鈕 (右箭頭圖示)
4. 等待上傳完成，查看 LED 是否按照設定閃爍

## 進一步實驗
1. **修改閃爍速度**：嘗試改變 `delay()` 的時間值，讓 LED 以不同的速度閃爍
   ```cpp
   digitalWrite(13, HIGH);
   delay(500);  // 改為 0.5 秒
   digitalWrite(13, LOW);
   delay(500);  // 改為 0.5 秒
   ```

2. **閃爍模式**：嘗試創建不同的閃爍模式，例如 SOS 摩斯電碼
   ```cpp
   // S (三短)
   for (int i = 0; i < 3; i++) {
     digitalWrite(13, HIGH);
     delay(200);
     digitalWrite(13, LOW);
     delay(200);
   }
   delay(300);
   
   // O (三長)
   for (int i = 0; i < 3; i++) {
     digitalWrite(13, HIGH);
     delay(600);
     digitalWrite(13, LOW);
     delay(200);
   }
   delay(300);
   
   // S (三短)
   for (int i = 0; i < 3; i++) {
     digitalWrite(13, HIGH);
     delay(200);
     digitalWrite(13, LOW);
     delay(200);
   }
   delay(1000);
   ```

## 常見問題
1. **LED 不亮**
   - 檢查 LED 的正負極是否接對
   - 確認 220Ω 電阻是否正確連接
   - 確認程式已成功上傳到 Arduino

2. **上傳失敗**
   - 檢查 Arduino 板型是否選對
   - 檢查 COM 埠是否選對
   - 嘗試重啟 Arduino IDE 或重新連接 Arduino

## 要點總結
- LED 是控制的基本元件，在實際應用中常用於狀態指示
- `pinMode()` 用於設定腳位的輸入/輸出模式
- `digitalWrite()` 用於控制數位腳位的高低電平
- `delay()` 用於控制程式的暫停時間，單位為毫秒