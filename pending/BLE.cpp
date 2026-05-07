// 确保ESP32-S3开发板在PlatformIO中正确配置
#include <HardwareSerial.h>

// 定义与HC-05连接的硬件串口及KEY引脚
#define BT_SERIAL Serial2
#define BT_KEY_PIN 4

void sendATCommand(String command) {
  BT_SERIAL.println(command);
  Serial.print("Sent: ");
  Serial.println(command);
  delay(200);
  
  // 读取并打印HC-05的响应，便于调试
  while (BT_SERIAL.available()) {
    String response = BT_SERIAL.readString();
    Serial.print("Response: ");
    Serial.println(response);
  }
}

void setup() {
  Serial.begin(115200);         // 用于调试信息的串口，连接电脑
  BT_SERIAL.begin(38400);       // 用于与HC-05通信的串口，波特率需与模块匹配
  pinMode(BT_KEY_PIN, OUTPUT);
  digitalWrite(BT_KEY_PIN, LOW); // 确保KEY引脚为低电平，使模块工作在传输模式

  Serial.println("ESP32-S3 Bluetooth Master is ready.");
  delay(2000);

  // 1. 发送AT指令，确保模块响应
  sendATCommand("AT");
  delay(1000);

  // 2. 将模块重置为主设备模式
  sendATCommand("AT+ROLE=1");
  delay(500);

  // 3. 扫描周围的蓝牙设备，寻找掌控板
  // 指令的返回结果格式大致为：+INQ:设备地址,设备类型,信号强度(RSSI)
  sendATCommand("AT+INQ");
  // 注意：实际项目中需要解析返回结果，从中筛选出掌控板的地址
  // 这里简化处理，建议在扫描前知道掌控板的MAC地址
  delay(5000); // 等待扫描结果
}

void loop() {
  // 假设我们已经知道掌控板的蓝牙MAC地址，格式需用冒号分隔，例如：AT+BIND=12:34:56:78:90:AB
  sendATCommand("AT+BIND=12:34:56:78:90:AB");
  delay(1000);
  
  // 发起连接
  sendATCommand("AT+LINK=12:34:56:78:90:AB");

  // 连接成功后，等待并处理数据
  Serial.println("Waiting for connection and data...");
  while (true) {
    if (BT_SERIAL.available()) {
      String data = BT_SERIAL.readString();
      Serial.print("Received from HC-05: ");
      Serial.println(data);
    }
    if (Serial.available()) {
      String dataToSend = Serial.readString();
      BT_SERIAL.print(dataToSend);
      Serial.println("Data sent to HC-05");
    }
  }
}
