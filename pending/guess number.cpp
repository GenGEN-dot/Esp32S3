// 猜数字游戏 - ESP32-C3版本
// 功能：随机生成1-100的数字，通过串口交互提示用户猜测

#include <Arduino.h>

int targetNumber;      // 目标数字
bool gameActive;       // 游戏进行状态标志

void startNewGame(); 
void handleGuess();

void setup() {
  Serial.begin(115200);          // 初始化串口通信
  delay(5000);
  randomSeed(analogRead(0));     // 用未连接的模拟引脚噪声初始化随机种子
  startNewGame();                // 开始新游戏
}

void loop() {
  // 只有当游戏处于激活状态时处理输入
  if (gameActive && Serial.available() > 0) {
    handleGuess();               // 处理用户猜测
  }
}

// 开始新游戏
void startNewGame() {
  targetNumber = random(1, 101); // 生成1-100的随机数
  gameActive = true;
  
  Serial.println("\n=== 新游戏开始 ===");
  Serial.println("我已想好一个1-100之间的数字");
  Serial.println("请输入你的猜测（输入q退出）：");
}

// 处理用户猜测
void handleGuess() {
  String input = Serial.readStringUntil('\n'); // 读取直到换行符
  input.trim();                                // 去除首尾空白字符

  // 退出检测
  if (input.equalsIgnoreCase("q")) {
    Serial.println("游戏已退出");
    gameActive = false;
    return;
  }

  // 转换为数字
  if (!input.length()) {
    Serial.println("错误：请输入有效数字");
    return;
  }

  int guess = input.toInt();
  
  // 验证输入有效性
  if (guess < 1 || guess > 100) {
    Serial.println("错误：请输入1-100之间的数字");
    return;
  }

  // 判断猜测结果
  if (guess == targetNumber) {
    Serial.println("★ 恭喜！猜对了 ★");
    startNewGame();  // 开始新游戏
  } else if (guess > targetNumber) {
    Serial.println("提示：猜大了！");
  } else {
    Serial.println("提示：猜小了！");
  }
}
