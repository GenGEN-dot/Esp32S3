#include <Arduino.h>

<<<<<<< HEAD
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
=======
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
>>>>>>> c7cd455871949d601bb41a3f031d25478df5a0f6
}