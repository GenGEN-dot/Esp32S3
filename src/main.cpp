#include <Arduino.h>

// 状态变量
bool ledState = false;
bool lastButtonState = HIGH; // 按键上一次的状态，初始为高电平（因为上拉）
bool buttonState; // 按键当前状态
unsigned long lastDebounceTime = 0; // 上次抖动时间
const unsigned long debounceDelay = 50; // 防抖延时(毫秒)

void setup() {
  pinMode(1, OUTPUT); // LED引脚
  pinMode(2, INPUT_PULLUP); // 启用内部上拉电阻
}

void loop() {
  bool reading = digitalRead(2); // 读取当前按键值
  
  // 防抖逻辑核心：如果读数发生变化，则重置防抖计时器
  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }
  
  // 如果经过防抖延时后，状态稳定变化了
  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading != buttonState) {
      buttonState = reading;
      
      // 检测下降沿（因为使用上拉，按下时是高电平->低电平）
      if (buttonState == LOW) {
        ledState = !ledState; // 翻转LED状态
        digitalWrite(1, ledState ? HIGH : LOW);
      }
    }
  }
     
  lastButtonState = reading; // 保存本次状态，用于下次比较
}