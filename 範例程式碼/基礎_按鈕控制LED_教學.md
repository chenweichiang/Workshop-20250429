# Arduino 基礎教學 - 按鈕控制 LED 詳細說明

## 範例目標
透過這個範例，您將學習如何使用按鈕作為輸入裝置來控制 LED 的開關。這是 Arduino 數位輸入的基本入門範例，讓您了解如何讀取外部按鈕的狀態並做出對應的動作。

## 需要的材料
- Arduino Uno 或相容板
- 麵包板
- LED 一個 (顏色不限)
- 220Ω 電阻一個 (用於 LED)
- 按鈕開關一個 (壓動式按鈕)
- 連接線若干

## 電路連接步驟
1. **連接 LED**
   - LED 長腳（正極）接 Arduino 的 13 號腳位
   - LED 短腳（負極）經過 220Ω 電阻後接 Arduino 的 GND

2. **連接按鈕**
   - 按鈕的一端接 Arduino 的 2 號腳位
   - 同一端也接一條線到 Arduino 的 5V（如果使用外部上拉電阻）
   - 按鈕的另一端接 Arduino 的 GND

   > **注意**：本範例使用 Arduino 的內部上拉電阻，因此不需要外部上拉電阻。如果使用內部上拉電阻，按鈕僅需連接 2 號腳位和 GND。

   ![按鈕控制LED連接示意圖](請放入按鈕LED連接的圖片連結)

## 程式碼說明

```cpp
const int buttonPin = 2;  // 按鈕連接針腳
const int ledPin = 13;    // LED連接針腳
int buttonState = 0;      // 用於儲存按鈕狀態

void setup() {
  pinMode(ledPin, OUTPUT);          // 設置LED針腳為輸出
  pinMode(buttonPin, INPUT_PULLUP); // 設置按鈕針腳為輸入，啟用內部上拉電阻
}

void loop() {
  buttonState = digitalRead(buttonPin); // 讀取按鈕狀態
  
  if (buttonState == LOW) {  // 如果按鈕被按下（因為使用上拉電阻，按下時為LOW）
    digitalWrite(ledPin, HIGH); // 點亮LED
  } else {
    digitalWrite(ledPin, LOW);  // 關閉LED
  }
}
```

### 程式碼詳解
- **變數宣告**
  - `const int buttonPin = 2;` 定義按鈕連接的腳位為 2 號腳位
  - `const int ledPin = 13;` 定義 LED 連接的腳位為 13 號腳位
  - `int buttonState = 0;` 用來儲存按鈕的當前狀態

- **`setup()` 函數**
  - `pinMode(ledPin, OUTPUT);` 設定 LED 腳位為輸出模式
  - `pinMode(buttonPin, INPUT_PULLUP);` 設定按鈕腳位為輸入模式，並啟用內部上拉電阻
    > 使用內部上拉電阻可以確保按鈕未按下時，輸入腳位為 HIGH 狀態，按下時為 LOW 狀態

- **`loop()` 函數**
  - `buttonState = digitalRead(buttonPin);` 讀取按鈕的狀態 (HIGH 或 LOW)
  - `if (buttonState == LOW)` 判斷按鈕是否被按下
    - 若按鈕被按下（狀態為 LOW），點亮 LED
    - 若按鈕未被按下（狀態為 HIGH），關閉 LED

## 上拉電阻說明
在這個範例中，我們使用了 Arduino 內建的上拉電阻（透過 `INPUT_PULLUP` 模式）。上拉電阻的作用是確保在按鈕未按下時輸入針腳維持在一個確定的狀態（HIGH），避免浮動狀態帶來的不穩定讀數。

當按鈕未按下時：
- 內部上拉電阻將輸入腳位拉高到 5V（HIGH）
  
當按鈕被按下時：
- 接腳直接連接到 GND，變成 LOW 狀態

這種設計使得程式中判斷按鈕狀態更加簡單和可靠。

## 上傳步驟
1. 使用 USB 線將 Arduino 連接到電腦
2. 在 Arduino IDE 中選擇正確的開發板型號和 COM 埠
3. 點擊上傳按鈕 (右箭頭圖示)
4. 等待上傳完成，測試按鈕功能：按下按鈕時 LED 應該亮起，放開時 LED 應該熄滅

## 進一步實驗
1. **切換模式**：修改程式使 LED 在按下按鈕後切換狀態（按一下開，再按一下關）
   ```cpp
   const int buttonPin = 2;
   const int ledPin = 13;
   int buttonState = 0;
   int lastButtonState = 0;
   int ledState = LOW;
   
   void setup() {
     pinMode(ledPin, OUTPUT);
     pinMode(buttonPin, INPUT_PULLUP);
   }
   
   void loop() {
     buttonState = digitalRead(buttonPin);
     
     // 檢測按鈕狀態變化 (從未按下到按下)
     if (buttonState == LOW && lastButtonState == HIGH) {
       ledState = !ledState;  // 切換 LED 狀態
       digitalWrite(ledPin, ledState);
       delay(50);  // 消除彈跳
     }
     
     lastButtonState = buttonState;  // 儲存當前按鈕狀態以便下次比較
   }
   ```

2. **按鈕計數器**：記錄按鈕被按下的次數，並用 LED 閃爍相應次數
   ```cpp
   const int buttonPin = 2;
   const int ledPin = 13;
   int buttonState = 0;
   int lastButtonState = 0;
   int counter = 0;
   
   void setup() {
     pinMode(ledPin, OUTPUT);
     pinMode(buttonPin, INPUT_PULLUP);
     Serial.begin(9600);  // 初始化序列通訊
   }
   
   void loop() {
     buttonState = digitalRead(buttonPin);
     
     // 檢測按鈕狀態變化
     if (buttonState == LOW && lastButtonState == HIGH) {
       counter++;  // 按鈕計數增加
       Serial.print("按鈕計數: ");
       Serial.println(counter);
       
       // 閃爍 LED 顯示計數
       for (int i = 0; i < counter; i++) {
         digitalWrite(ledPin, HIGH);
         delay(200);
         digitalWrite(ledPin, LOW);
         delay(200);
       }
       
       delay(50);  // 消除彈跳
     }
     
     lastButtonState = buttonState;
   }
   ```

## 常見問題
1. **按鈕不靈敏或誤觸發**
   - 這可能是由於按鈕彈跳（bounce）引起的。當機械按鈕按下或釋放時，接觸點可能會產生多次瞬間的接通/斷開。
   - 解決方法：在代碼中增加去彈跳延遲（例如 `delay(50);`）或使用軟體去彈跳技術。

2. **LED 狀態與按鈕操作相反**
   - 檢查程式中的邏輯條件。使用內部上拉電阻時，按鈕按下為 LOW，未按下為 HIGH。

3. **電路連接問題**
   - 確保所有連線正確，尤其是按鈕的連接方式
   - 檢查 LED 的正負極是否接對

## 要點總結
- `pinMode(pin, INPUT_PULLUP)` 啟用 Arduino 的內部上拉電阻，簡化按鈕連接
- `digitalRead()` 用於讀取數位輸入腳位的狀態 (HIGH 或 LOW)
- 使用上拉電阻時，按鈕按下為 LOW 狀態，未按下為 HIGH 狀態
- 按鈕是一種常見的輸入設備，可用於控制 Arduino 項目中的各種操作