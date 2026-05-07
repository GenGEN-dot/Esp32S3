#include <Arduino.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <U8g2lib.h>
#include <Wire.h>

// 引脚定义
#define DHTPIN 6      // DHT11 数据引脚连接到 GPIO6
#define DHTTYPE DHT11 // 选择 DHT11 型号

#define I2C_SDA 4   // SDA：GPIO4
#define I2C_SCL 5   // SCL：GPIO5


// --- 硬件配置 ---
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

DHT dht(DHTPIN, DHTTYPE);

float humidity = 0;     // 湿度（百分比）
float temperature = 0;  // 温度（摄氏度）

void setup() {
    Serial.begin(115200);   // 初始化串口，用于输出数据
    dht.begin();          // 初始化 DHT11
      // I2C初始化
    Wire.begin(I2C_SDA, I2C_SCL);
    Wire.setClock(400000);  // 设置 I2C 时钟频率为 400 kHz
    // OLED 初始化
    u8g2.begin();
    u8g2.enableUTF8Print();
    u8g2.setFont(u8g2_font_wqy16_t_gb2312);
    u8g2.setFontPosTop();
    delay(5000);
    Serial.print("初始化完成\n");
}

void loop() {
    humidity = dht.readHumidity();
    temperature = dht.readTemperature();

    if (isnan(humidity) || isnan(temperature)) {
        Serial.println("Failed to read from DHT sensor!");
        return;
    }

    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.println(" °C");
    Serial.print("Humidity: ");
    Serial.print(humidity);
    Serial.println(" %");

    // ----- OLED 显示部分 -----
    u8g2.clearBuffer();                     // 清空缓冲区

    u8g2.setCursor(0, 0);                  // 第1行，列0
    u8g2.print("Temp: ");
    u8g2.print(temperature);
    u8g2.println(" C");

    u8g2.setCursor(0, 18);                 // 第2行，列0（中文字体高度约16像素）
    u8g2.print("Humidity: ");
    u8g2.print(humidity);
    u8g2.println(" %");

    u8g2.sendBuffer();                     // 将缓冲区内容发送到屏幕 ← 关键！

    delay(1000);
}