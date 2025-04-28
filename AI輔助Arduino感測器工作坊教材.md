# AI輔助Arduino新型感測器進階應用工作坊
版本：1.3.2
最後更新：2025/04/03

## 課程重點與目標

本工作坊將引導學生運用所有指定材料，結合AI輔助開發工具（包含GitHub Copilot、Cursor與Gemini AI API），設計並實作一個開放式專題。學生需發揮創意，解決生活中實際問題或創造新體驗，並於課程結束時展示成果。

### 必用材料
- Arduino Uno 相容板 ×1
- USB 連接線 ×1
- 麵包板（400孔）×1
- 跳線（公對公）×10
- DHT22 溫濕度感測器 ×1
- MPU6050 加速度陀螺儀模組 ×1
- 10kΩ 電阻 ×2
- 蜂鳴器 ×1

---

## 課程流程

### 1. 開場與目標設定（10分鐘）
- 介紹所有材料的功能與應用情境。
- 強調本次專題為「開放式挑戰」：請學生運用所有材料，設計一個能解決生活中某個問題或創造新體驗的智慧裝置。

### 2. 感測器與元件基礎操作（30分鐘）
- DHT22：環境溫濕度監測，數據讀取與警報應用。
- MPU6050：動作/姿態偵測，數據視覺化。
- 蜂鳴器：警示或互動提示。
- 麵包板/跳線/電阻：電路連接實作。

### 3. AI輔助開發實作（20分鐘）
- 使用Cursor、GitHub Copilot或Gemini AI API協助感測器程式碼生成與整合。
- 示範如何用AI優化數據處理與警報邏輯。
- **Gemini AI API應用說明**：
  - Gemini AI API是Google推出的生成式AI服務，能協助自動生成程式碼、解釋邏輯、優化專案設計。
  - 學生可註冊Google帳號並申請Gemini API金鑰，於[Google AI Studio](https://aistudio.google.com/)獲取教學與API文件。
  - 可透過Python、Node.js等語言呼叫API，將專題需求、程式註解或錯誤訊息發送給Gemini，獲得即時AI建議。
  - 範例：
    ```python
    import google.generativeai as genai
    genai.configure(api_key="你的API金鑰")
    model = genai.GenerativeModel('gemini-pro')
    response = model.generate_content("請幫我產生一段Arduino讀取DHT22與MPU6050的程式碼")
    print(response.text)
    ```
  - 建議將Gemini AI API與Copilot、Cursor等工具搭配使用，提升AI輔助開發的多元性。

### 4. 開放式專題設計挑戰（40分鐘）
- 學生分組討論，必須用到所有材料，構思一個主題（如：健康監測、環境警報、互動裝置等）。
- 引導學生思考：
  - 如何結合溫濕度、動作偵測與警報？
  - 生活中有哪些場景可以應用這些感測器？
  - 如何讓裝置有「互動性」或「自動反應」？

### 5. 專題實作與測試（30分鐘）
- 學生動手組裝、撰寫程式、測試功能。
- 鼓勵學生善用AI工具解決程式問題。

### 6. 成果展示與回饋（10分鐘）
- 各組簡短分享設計理念與成果。
- 教師給予優化建議與實際應用延伸討論。

---

## 專題題目範例（僅供參考，學生可自由發揮）

1. 智慧環境安全警報器：溫度/濕度異常或偵測到劇烈動作時，蜂鳴器發出警報。
2. 個人健康運動監測站：結合姿態感測與環境監控，記錄運動狀態並在環境不適時提醒休息。
3. 互動型桌上小幫手：當偵測到有人靠近（動作感測）且環境過熱/潮濕時，蜂鳴器提醒並顯示狀態。

---

## AI輔助開發提示範例

- "請幫我生成一段能同時讀取DHT22與MPU6050數據並根據條件觸發蜂鳴器的Arduino程式碼。"
- "如何讓蜂鳴器在溫度超過28度或偵測到劇烈運動時自動響起？"
- "請協助整合DHT22、MPU6050與蜂鳴器的功能，並將結果顯示於Serial Monitor。"
- "如何用Gemini AI API優化我的Arduino感測器專題設計？"

---

## 綜合範例：智慧環境與運動安全警報器

### 專案目標
結合DHT22溫濕度感測器、MPU6050加速度陀螺儀、蜂鳴器，利用麵包板、跳線與10kΩ電阻，打造一個能即時監控環境與運動狀態並自動警報的智慧裝置。全程可搭配AI輔助工具（如Gemini API、Copilot、Cursor）協助程式設計與除錯。

### 功能說明
- 監測環境溫度、濕度，超過閾值自動警報
- 偵測使用者運動狀態（靜止/走路/劇烈運動/跌倒）
- 當偵測到危險（高溫、過濕、跌倒等）時蜂鳴器自動響起
- 所有數據即時顯示於Serial Monitor

### 材料與接線
- DHT22 DATA → Arduino Pin 2（與VCC間加10kΩ上拉電阻）
- DHT22 VCC → 5V
- DHT22 GND → GND
- MPU6050 SDA → A4，SCL → A5，VCC → 5V，GND → GND
- 蜂鳴器 → Pin 8
- 其餘連接用麵包板與跳線

### 程式設計重點
- 使用DHT與MPU6050函式庫分別讀取環境與運動數據
- 設定溫濕度與運動狀態警報閾值
- 判斷條件時觸發蜂鳴器
- 利用AI輔助工具優化程式結構、除錯與提示

### AI輔助應用建議
- 用Gemini API或Copilot生成感測器初始化與數據判斷程式碼
- 用Cursor快速整合多感測器程式片段
- 遇到錯誤時將錯誤訊息丟給AI協助除錯
- 可請AI協助優化警報邏輯或數據視覺化

### 範例程式片段（可用AI生成完整程式）
```cpp
#include <Wire.h>
#include <MPU6050.h>
#include <DHT.h>

#define DHTPIN 2
#define DHTTYPE DHT22
#define BUZZER_PIN 8
DHT dht(DHTPIN, DHTTYPE);
MPU6050 mpu;

void setup() {
  Serial.begin(115200);
  Wire.begin();
  dht.begin();
  mpu.begin(MPU6050_SCALE_2000DPS, MPU6050_RANGE_2G);
  mpu.calibrateGyro();
  pinMode(BUZZER_PIN, OUTPUT);
  Serial.println("溫度,濕度,姿態,運動狀態,警報");
}

void loop() {
  float temp = dht.readTemperature();
  float hum = dht.readHumidity();
  Vector acc = mpu.readNormalizeAccel();
  Vector gyro = mpu.readNormalizeGyro();
  float accelMag = sqrt(acc.XAxis*acc.XAxis + acc.YAxis*acc.YAxis + acc.ZAxis*acc.ZAxis);
  String posture = "直立";
  bool danger = false;
  if (temp > 30 || hum > 70) danger = true;
  if (accelMag > 2.0) { posture = "跌倒"; danger = true; }
  else if (accelMag > 1.5) posture = "劇烈運動";
  else if (accelMag > 1.1) posture = "走路";
  else posture = "靜止";
  if (danger) tone(BUZZER_PIN, 1000, 300);
  else noTone(BUZZER_PIN);
  Serial.print(temp); Serial.print(",");
  Serial.print(hum); Serial.print(",");
  Serial.print(posture); Serial.print(",");
  Serial.print(accelMag,2); Serial.print(",");
  Serial.println(danger ? "警報" : "正常");
  delay(500);
}
```

### 延伸挑戰
- 請學生用AI輔助工具優化警報判斷、增加更多狀態分類或串接LCD顯示
- 鼓勵學生將專題需求、程式片段或錯誤訊息丟給Gemini API或Copilot獲得最佳化建議

---

*最後更新：2025年4月3日*