#include <Arduino.h>

// 建议不要使用 GPIO1 (TX0) 作为 LED 引脚 — 它是串口 TX，会干扰串口输出。
const int LED_PIN = 2; // 可按板子实际情况换成其他空闲 GPIO

void setup(){
  Serial.begin(115200);                 // 初始化串口，波特率115200
  delay(2000);                          // 等待主机枚举/监视器连接
  Serial.println("Serial started");   // 启动诊断打印

  pinMode(LED_PIN, OUTPUT);             // 设置 LED 引脚为输出模式
}

void loop(){
  Serial.println("Hello, Arduino!");    // 打印信息到串口监视器
  digitalWrite(LED_PIN, HIGH);            // 设置 LED 为高电平
  delay(1000);                            // 延时1秒
  digitalWrite(LED_PIN, LOW);             // 设置 LED 为低电平
  delay(1000);                            // 延时1秒
}