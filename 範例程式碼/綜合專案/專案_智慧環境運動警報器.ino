// 專案_智慧環境運動警報器.ino
// 本範例可搭配AI輔助工具（Copilot、Gemini、Cursor）協助程式生成、除錯與優化
// 必用材料：DHT22、MPU6050、蜂鳴器、10kΩ電阻、麵包板、跳線

#include <Wire.h>
#include <MPU6050.h>
#include <DHT.h>

#define DHTPIN 2
#define DHTTYPE DHT22
#define BUZZER_PIN 8

// [AI輔助] 初始化DHT22與MPU6050感測器，可請Copilot或Gemini自動生成
DHT dht(DHTPIN, DHTTYPE);
MPU6050 mpu;

void setup() {
  // [AI輔助] 初始化序列埠與感測器
  Serial.begin(115200);
  Wire.begin();
  dht.begin();
  mpu.begin(MPU6050_SCALE_2000DPS, MPU6050_RANGE_2G);
  mpu.calibrateGyro();
  pinMode(BUZZER_PIN, OUTPUT);
  Serial.println("溫度,濕度,姿態,運動狀態,警報");
}

void loop() {
  // [AI輔助] 讀取感測器數據與判斷警報條件
  float temp = dht.readTemperature();
  float hum = dht.readHumidity();
  Vector acc = mpu.readNormalizeAccel();
  float accelMag = sqrt(acc.XAxis*acc.XAxis + acc.YAxis*acc.YAxis + acc.ZAxis*acc.ZAxis);
  String posture = "直立";
  bool danger = false;
  if (temp > 30 || hum > 70) danger = true;
  if (accelMag > 2.0) { posture = "跌倒"; danger = true; }
  else if (accelMag > 1.5) posture = "劇烈運動";
  else if (accelMag > 1.1) posture = "走路";
  else posture = "靜止";

  // [AI輔助] 警報觸發與蜂鳴器控制
  if (danger) tone(BUZZER_PIN, 1000, 300);
  else noTone(BUZZER_PIN);

  // [AI輔助] 數據即時顯示於Serial Monitor
  Serial.print(temp); Serial.print(",");
  Serial.print(hum); Serial.print(",");
  Serial.print(posture); Serial.print(",");
  Serial.print(accelMag,2); Serial.print(",");
  Serial.println(danger ? "警報" : "正常");
  delay(500);
}

// [AI輔助] 可請AI協助優化警報判斷、增加LCD顯示或更多狀態分類 