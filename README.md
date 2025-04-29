# 智慧型運動姿態監測器工作坊

這是一個為期2小時的Arduino感測器應用工作坊教材，專注於打造智慧型運動姿態監測器。

## 工作坊內容

- AI輔助開發工具介紹
- DHT22溫濕度感測器應用
- MPU6050運動感測器應用
- 即時姿態監測系統開發

## 教材檔案

- [教材內容](AI輔助Arduino感測器工作坊教材.md)
- [材料清單](材料清單.md)
- [線上簡報](https://gamma.app/docs/2050AI-tbznsh1o86gqkpl)

## 系統需求

### 硬體需求
- Arduino Uno 或相容板
- DHT22 溫濕度感測器
- MPU6050 加速度陀螺儀模組
- 麵包板與跳線
- 基本電子元件（電阻等）

### 軟體需求
- Arduino IDE 2.2.1+
- 必要函式庫：
  - DHT sensor library
  - MPU6050 library
- Cursor編輯器
- GitHub Copilot（建議）

## 安裝指南

1. 前往 [GitHub註冊頁面](https://github.com/join) 建立免費帳號。
2. 學生建議申請 [GitHub Student Developer Pack](https://education.github.com/pack) 可免費啟用 Copilot。
3. 申請通過後，進入 [Copilot設定頁](https://github.com/settings/copilot) 啟用 Copilot。
4. 在 VS Code 或 Cursor 編輯器安裝 Copilot 擴充功能，登入 GitHub 帳號即可使用。

## VS Code Insiders 下載安裝教學

1. 前往 [VS Code Insiders官方下載頁](https://code.visualstudio.com/insiders/) 下載對應作業系統版本。
2. 安裝後啟動 VS Code Insiders。
3. 進入 Extensions 市集，搜尋並安裝「Arduino」、「C/C++」、「GitHub Copilot」等擴充功能。
4. 登入 GitHub 帳號以啟用 Copilot。
5. 建議同步安裝 [Cursor編輯器](https://cursor.sh/) 以體驗更多AI輔助功能。

## 接線圖

### DHT22接線
- VCC 接 5V
- DATA 接 Digital Pin 2（需要10kΩ上拉電阻）
- GND 接 GND

### MPU6050接線
- VCC 接 5V
- GND 接 GND
- SCL 接 A5
- SDA 接 A4

## 學生專案列表

| 學號 | 姓名 | GitHub 儲存庫 |
|------|------|--------------|
| B11235020 | 宇涵 | https://github.com/legggg1114/ai-tech |
| B11235011 | 李昱賢 | https://github.com/zc15va/0429 |
| B11235012 | 鄧丞惠 | https://github.com/sjdydch/20250429 |
| B11235051 | 宜廷 | https://github.com/sha1123/ai-pilipala.git |
| B11235048 | 蔡欣妤 | https://github.com/kt311024/ai-tech |
| B11235007 | 沈彥辰 | https://github.com/e1bmd225/0429_AI-tech |
| B11235035 | 游佳慧 | https://github.com/star0302/0429 |
| B11235028 | 林紫渝 | https://github.com/Jiziyan1211/text |
| B11235037 | 李宜庭 | https://github.com/8ace16/AI123 |
| B11235004 | 陳鵬年 | https://github.com/CPNIAN/123456789 |
| B11235049 | 林修禕 | https://github.com/mpmaaoo/429 |
| B11235026 | 邱愉絜 | https://github.com/yqiu94116/b11235026 |
| B11136025 | 黃湘如 | https://github.com/skyh8658/123.git |
| B11235030 | 派洛飯捲 | https://github.com/Chaanhan77839/0429 |
| B11235010 | 林承翰 | https://github.com/koketama/ai_workshop |
| - | 張修齊 | https://github.com/cxz555444/dsiogfiowae4ugtouiw.git |
| B11235055 | 林榆皓 | https://github.com/LimYuHaw/ArduinoWorkShop |
| B11235043 | 洪詩涵 | https://github.com/ErrorExists666/pilicapililalabobolinabebeludo |
| B11235050 | 竹 | https://github.com/Zhuzhu0223/Zhuzhu |
| B11235042 | 張弘毅 | https://github.com/ze8495918/zzz-b11235042 |
| M11235015 | ㄚ宸 | https://github.com/Cooga730/AI-tech1 |

## 授權

本教材採用 MIT 授權條款。

## 貢獻

歡迎提交Issue或Pull Request來改善教材內容。

## 作者

[您的名字/組織名稱]

## 最後更新

2025年4月29日