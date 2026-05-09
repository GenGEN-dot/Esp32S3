#include <Arduino.h>

// 定义光敏传感器连接的引脚
#define LIGHT_SENSOR_PIN 1  // 光敏模块 S 脚接 GPIO1

// 存储读取到的光照值
int lightValue = 0;

void setup() {
  // 初始化串口波特率 115200
  Serial.begin(115200);
}

void loop() {
  // 读取模拟值 (ESP32 ADC 范围：0 ~ 4095)
  lightValue = analogRead(LIGHT_SENSOR_PIN);

  // 打印结果
  Serial.print("光照强度模拟值: ");
  Serial.print(lightValue);
  
  // 根据数值判断亮/暗（可自己调整阈值）
  if (lightValue < 1000) {
    Serial.println("  → 光线很亮");
  } else if (lightValue < 3000) {
    Serial.println("  → 光线适中");
  } else {
    Serial.println("  → 光线很暗/遮挡");
  }

  delay(500); // 500ms 刷新一次
}