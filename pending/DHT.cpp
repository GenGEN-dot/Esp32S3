#include <Arduino.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>

// 引脚定义
#define DHTPIN 6      // DHT11 数据引脚连接到 GPIO6
#define DHTTYPE DHT11 // 选择 DHT11 型号

DHT dht(DHTPIN, DHTTYPE);

float humidity = 0;     // 湿度（百分比）
float temperature = 0;  // 温度（摄氏度）

void setup() {
  Serial.begin(115200);   // 初始化串口，用于输出数据
  dht.begin();          // 初始化 DHT11
}

void loop() {
  // 读取湿度值
  humidity = dht.readHumidity();
  // 读取温度值（摄氏度）
  temperature = dht.readTemperature();

  // 检查读取是否成功
  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // 输出读取到的数据
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println(" °C");
  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.println(" %");

  delay(1000); // 读取间隔为1秒
}