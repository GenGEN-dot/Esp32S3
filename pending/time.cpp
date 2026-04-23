#include <WiFi.h>
#include "time.h"
#include <U8g2lib.h>

// ------------------- WiFi 配置 -------------------
const char* ssid = "Netcore-2";        // 建议改为英文 SSID，避免兼容性问题
const char* password = "560099999";

// ------------------- NTP 配置 -------------------
const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 28800;       // UTC+8
const int   daylightOffset_sec = 0;

// ------------------- 屏幕配置 -------------------
U8G2_SSD1306_128X64_NONAME_1_SW_I2C u8g2(U8G2_R0, /*clock=*/5, /*data=*/4, /*reset=*/U8X8_PIN_NONE);

// 存储时间的结构体
struct tm timeinfo;

// 非阻塞定时变量
unsigned long lastPrintTime = 0;   // 上次打印/刷新的毫秒时间戳

// ------------------- 函数声明 -------------------
void drawMessage(const char* msg);
void displayTime(struct tm * timeinfo);

// ------------------- setup -------------------
void setup() {
  Serial.begin(115200);
  
  u8g2.begin();
  u8g2.setFont(u8g2_font_6x10_tf);
  u8g2.setFlipMode(0);

  drawMessage("Connecting WiFi...");
  
  WiFi.begin(ssid, password);
  int attempt = 0;
  while (WiFi.status() != WL_CONNECTED && attempt < 20) {
    delay(500);
    Serial.print(".");
    attempt++;
  }
  
  if (WiFi.status() != WL_CONNECTED) {
    drawMessage("WiFi Failed!");
    Serial.println("WiFi连接失败，请检查账号密码");
    while(1);
  }
  
  drawMessage("WiFi OK, Syncing...");
  Serial.println("\nWiFi已连接，正在同步时间...");
  
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  
  int retry = 0;
  while (!getLocalTime(&timeinfo) && retry < 20) {
    delay(500);
    Serial.print(".");
    retry++;
  }
  
  if (!getLocalTime(&timeinfo)) {
    drawMessage("Time Sync Failed");
    Serial.println("时间同步失败");
    while(1);
  }
  
  drawMessage("Time Ready!");
  delay(1000);
  Serial.println("时间同步成功");
  
  lastPrintTime = millis();   // 初始化定时基准
}

// ------------------- loop (非阻塞，每秒执行) -------------------
void loop() {
  unsigned long now = millis();
  
  // 每隔 1000 毫秒执行一次（精确间隔）
  if (now - lastPrintTime >= 1000) {
    lastPrintTime = now;   // 更新时间戳
    
    if (getLocalTime(&timeinfo)) {
      // 1. 串口打印
      char timeStr[30];
      strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S", &timeinfo);
      Serial.println(timeStr);
      
      // 2. 屏幕显示
      displayTime(&timeinfo);
    } else {
      drawMessage("Time Error!");
      Serial.println("获取时间失败");
    }
  }
  
  // 让出 CPU，避免空转（可选，但推荐）
  delay(1);
}

// ------------------- 函数实现 -------------------
void displayTime(struct tm * timeinfo) {
  u8g2.firstPage();
  do {
    u8g2.setCursor(0, 12);
    u8g2.print("Date: ");
    u8g2.print(timeinfo->tm_year + 1900);
    u8g2.print("-");
    u8g2.print(timeinfo->tm_mon + 1);
    u8g2.print("-");
    u8g2.println(timeinfo->tm_mday);
    
    u8g2.setCursor(0, 26);
    u8g2.print("Week: ");
    const char* weekdays[] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
    u8g2.println(weekdays[timeinfo->tm_wday]);
    
    u8g2.setFont(u8g2_font_helvB12_tf);
    u8g2.setCursor(0, 48);
    u8g2.print("Time: ");
    u8g2.print(timeinfo->tm_hour);
    u8g2.print(":");
    if (timeinfo->tm_min < 10) u8g2.print("0");
    u8g2.print(timeinfo->tm_min);
    u8g2.print(":");
    if (timeinfo->tm_sec < 10) u8g2.print("0");
    u8g2.println(timeinfo->tm_sec);
    
    u8g2.setFont(u8g2_font_6x10_tf);
  } while (u8g2.nextPage());
}

void drawMessage(const char* msg) {
  u8g2.firstPage();
  do {
    u8g2.setCursor(0, 32);
    u8g2.print(msg);
  } while (u8g2.nextPage());
}