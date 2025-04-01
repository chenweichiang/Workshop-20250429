# Arduino+AI 互動感測器工作坊

![version](https://img.shields.io/badge/版本-1.1.0-blue)
![update](https://img.shields.io/badge/更新日期-2025.04.01-green)

![Arduino Logo](https://www.arduino.cc/en/uploads/Trademark/ArduinoCommunityLogo.png)

# 雲科 AI輔助Arduino感測器工作坊教材
版本：1.1.0
最後更新：2025/04/01

## 🚀 快速開始

### ⚡ 課前準備（請在工作坊前完成）
1. **註冊帳號**
   - 註冊 [GitHub 帳號](https://github.com/signup)（免費）
   - 使用學校 Email 申請 [GitHub 學生開發者套件](https://education.github.com/pack)
   - 按照[學生帳號申請指南](GitHub_Copilot學生帳號申請指南.md)啟用免費 Copilot

2. **安裝必要軟體**
   - 安裝 [Arduino IDE](https://www.arduino.cc/en/software)（2.2.1 或更新版本）
   - 安裝 [VS Code Insiders](https://code.visualstudio.com/insiders/)（開發環境，推薦）
   - 安裝 [Cursor 編輯器](https://cursor.sh/)（AI 輔助開發工具）
   - 詳細步驟請參考[環境設定說明](環境設定說明.md)

3. **準備硬體設備**
   - 查看[材料清單](材料清單.md)確認所需電子元件
   - 攜帶筆記型電腦與充電器
   - 準備一條 USB A-to-B 線（Arduino 連接線）

### 💡 工作坊學習路徑
1. 檢查[材料清單](材料清單.md)，確認所有設備
2. 依照[環境設定說明](環境設定說明.md)確認軟體運作正常
3. 了解[AI工具使用方法](Cursor與GitHub_Copilot指南.md)
4. 閱讀[完整課程教材](AI輔助Arduino感測器工作坊教材.md)
5. 按照下方學習路徑開始實作！

## 📚 課程簡介
這是一個結合AI與Arduino的一日互動感測器工作坊，透過AI輔助工具幫助學員快速入門Arduino感測器應用，即使沒有程式設計經驗也能輕鬆完成互動裝置的製作。

## 📖 課程內容
### 上午：Arduino+AI基礎與環境設置
- Cursor與GitHub Copilot AI工具介紹與實作 (09:00-10:00)
- Arduino基礎概念與環境設置 (10:00-10:30)
- 使用AI生成基礎Arduino程式碼 (10:30-11:30)
- 基礎感測器互動示範 (11:30-12:00)
  - LED控制
  - 按鈕輸入
  - 光敏電阻應用

### 下午：AI輔助感測器專案實作
- 使用AI解決Arduino程式問題 (13:00-14:00)
- 光感應互動裝置實作 (14:00-15:30)
  - 利用AI生成程式碼
  - 感測器資料解讀與調校
  - 即時測試與除錯
- 創意專案發想與實作 (15:30-17:00)
  - 小組協作
  - 成果展示與分享

## 📑 學習路徑與課程資源

### 1️⃣ 課前準備
- [材料清單](材料清單.md)：了解所需硬體設備與電子元件
- [環境設定說明](環境設定說明.md)：安裝並設定 Arduino IDE、VS Code 和相關軟體
- [Cursor與GitHub Copilot指南](Cursor與GitHub_Copilot指南.md)：AI輔助開發工具的使用方法

### 2️⃣ Arduino 基礎學習
- [基礎_LED閃爍.ino](範例程式碼/基礎_LED閃爍.ino)：第一個 Arduino 程式
- [基礎_LED閃爍_教學.md](範例程式碼/基礎_LED閃爍_教學.md)：詳細說明與練習
- [基礎_按鈕控制LED.ino](範例程式碼/基礎_按鈕控制LED.ino)：數位輸入與輸出範例
- [基礎_按鈕控制LED_教學.md](範例程式碼/基礎_按鈕控制LED_教學.md)：按鈕控制詳解
- [基礎_PWM控制LED亮度.ino](範例程式碼/基礎_PWM控制LED亮度.ino)：類比輸出示範
- [基礎_PWM控制LED亮度_教學.md](範例程式碼/基礎_PWM控制LED亮度_教學.md)：PWM原理說明

### 3️⃣ 感測器應用
- [基礎_光敏電阻讀取.ino](範例程式碼/基礎_光敏電阻讀取.ino)：類比輸入範例
- [光敏電阻感測器.ino](範例程式碼/光敏電阻感測器.ino)：完整光敏電阻應用
- [光敏電阻感測器_教學.md](範例程式碼/光敏電阻感測器_教學.md)：詳細教學與實驗
- [基礎_水銀開關傾斜感測.ino](範例程式碼/基礎_水銀開關傾斜感測.ino)：傾斜感測範例

### 4️⃣ 綜合實作專案
- [專案_互動式燈光裝置.ino](範例程式碼/專案_互動式燈光裝置.ino)：光感應互動燈光系統
- [專案_互動式燈光裝置_教學.md](範例程式碼/專案_互動式燈光裝置_教學.md)：詳細專案實作指南
- [專案_互動樂器.ino](範例程式碼/專案_互動樂器.ino)：互動式電子樂器設計

### 5️⃣ 完整課程教材
- [AI輔助Arduino感測器工作坊教材.md](AI輔助Arduino感測器工作坊教材.md)：整合所有教學內容的主教材

## 📁 檔案結構說明
```
├── README.md                       # 本指南檔案
├── 材料清單.md                      # 硬體與電子元件清單
├── 環境設定說明.md                  # 軟體安裝與設定指南
├── AI輔助Arduino感測器工作坊教材.md  # 完整課程教材
├── Cursor與GitHub_Copilot指南.md    # AI工具使用指南
└── 範例程式碼/                     # 範例程式與教學文件
    ├── 基礎階段/
    │   ├── 基礎_LED閃爍.ino             # LED基本控制
    │   ├── 基礎_LED閃爍_教學.md         # LED控制詳解
    │   ├── 基礎_按鈕控制LED.ino         # 數位輸入輸出範例
    │   ├── 基礎_按鈕控制LED_教學.md     # 按鈕控制教學
    │   ├── 基礎_PWM控制LED亮度.ino      # PWM控制範例
    │   └── 基礎_PWM控制LED亮度_教學.md  # PWM控制教學
    ├── 感測器應用/
    │   ├── 基礎_光敏電阻讀取.ino        # 光感測基礎
    │   ├── 光敏電阻感測器.ino           # 完整光感測應用
    │   ├── 光敏電阻感測器_教學.md       # 光感測詳解
    │   └── 基礎_水銀開關傾斜感測.ino    # 傾斜感測範例
    └── 綜合專案/
        ├── 專案_互動式燈光裝置.ino      # 互動燈光專案程式
        ├── 專案_互動式燈光裝置_教學.md  # 互動燈光專案指南
        └── 專案_互動樂器.ino            # 互動樂器專案程式
```

## 🛠️ 開發環境需求
### 必要軟體
1. [Arduino IDE](https://www.arduino.cc/en/software)
2. [Cursor](https://cursor.sh/)（推薦）

### 擴充功能
- Arduino 擴充功能
- C/C++ 擴充功能
- GitHub Copilot

### 硬體需求
- Arduino Uno 開發板（或兼容板）
- USB 連接線
- 基本電子元件（詳見[材料清單](材料清單.md)）

## 🔧 安裝說明
詳細的軟體安裝與環境設定步驟，請參考：
1. [環境設定說明](環境設定說明.md)
2. [Cursor與GitHub Copilot指南](Cursor與GitHub_Copilot指南.md)

## 🎯 專案範例
### 1. AI輔助光感應裝置
- 功能：根據環境光線自動調整LED亮度
- 使用元件：LED、光敏電阻、按鈕
- AI工具：Cursor + GitHub Copilot
- 程式碼：[專案_互動式燈光裝置.ino](範例程式碼/專案_互動式燈光裝置.ino)
- 教學文件：[專案_互動式燈光裝置_教學.md](範例程式碼/專案_互動式燈光裝置_教學.md)

### 2. AI生成互動樂器
- 功能：透過AI協助完成互動樂器的程式設計
- 使用元件：蜂鳴器、按鈕、LED、水銀開關
- AI提示範例：「設計一個使用按鈕控制的簡易樂器」
- 程式碼：[專案_互動樂器.ino](範例程式碼/專案_互動樂器.ino)

## 📄 授權資訊
本教材採用 MIT 授權條款。

## 📅 更新紀錄
### v1.0.0 (2025/04/01)
- 重新設計為一日AI輔助Arduino工作坊
- 優化材料清單，選用經濟實惠元件
- 加強AI工具應用內容
- 簡化專案難度，適合初學者

## 工作坊時間與地點
- 日期：2025年4月29日 (星期二)
- 時間：10:00-12:00
- 地點：國立雲林科技大學
- 教室：設計學院創新工作室

---

*最後更新：2025年4月1日*