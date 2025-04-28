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

1. 安裝Arduino IDE
2. 安裝必要的函式庫：
   - 在Arduino IDE中選擇「工具」>「管理程式庫」
   - 搜尋並安裝 DHT sensor library
   - 搜尋並安裝 MPU6050 library

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

## 授權

本教材採用 MIT 授權條款。

## 貢獻

歡迎提交Issue或Pull Request來改善教材內容。

## 作者

[您的名字/組織名稱]

## 最後更新

2025年4月3日