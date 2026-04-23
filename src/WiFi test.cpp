#include <Arduino.h>
#include <WiFi.h>

// 替换为你的 Wi-Fi 凭证
const char* ssid = "Netcore-2";
const char* password = "560099999";

void setup() {
  Serial.begin(115200);
  delay(5000); // 等待串口稳定
  Serial.println("\n\nESP32-S3 网络信息获取程序开始...");

  // 1. 获取并打印 MAC 地址
  //    初始化Wi-Fi在Station模式，这是读取MAC地址的必要前提[reference:1]。
  WiFi.mode(WIFI_STA);
  
  // WiFi.macAddress() 获取的是 Station 接口的 MAC 地址，常用于连接路由器[reference:2]。
  // ESP.getEfuseMac() 获取芯片出厂时烧录的默认MAC地址，是唯一的硬件标识[reference:3]。
  Serial.print("Station MAC 地址: ");
  Serial.println(WiFi.macAddress());
  
  uint64_t chipId = ESP.getEfuseMac();
  Serial.printf("芯片默认 MAC 地址: %04X%08X\n", (uint16_t)(chipId >> 32), (uint32_t)chipId);
  
  Serial.println("\n开始连接 Wi-Fi...");

  // 2. 连接到 Wi-Fi 网络
  WiFi.begin(ssid, password);

  // 等待连接成功
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWi-Fi 连接成功！");

  // 3. 获取并打印网络信息
  //    成功连接后，ESP32-S3 会通过 DHCP 自动获取到这些信息。
  Serial.println("\n--- 网络配置信息 ---");
  Serial.print("本机 IP 地址: ");
  Serial.println(WiFi.localIP());      // 获取 ESP32-S3 被分配的 IP 地址[reference:4]
  Serial.print("子网掩码: ");
  Serial.println(WiFi.subnetMask());   // 获取子网掩码
  Serial.print("默认网关: ");
  Serial.println(WiFi.gatewayIP());    // 获取网关地址（通常是路由器的 IP）[reference:5]
  Serial.print("DNS 服务器: ");
  Serial.println(WiFi.dnsIP());        // 获取 DNS 服务器地址
  Serial.print("所连 Wi-Fi SSID: ");
  Serial.println(WiFi.SSID());         // 获取所连接 Wi-Fi 的名称
  Serial.println("--------------------\n");
}

void loop() {
  // 你的其他逻辑可以放在这里
  // delay(10000);
}