#include <Arduino.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <U8g2lib.h>
#include <Wire.h>

// ========== 引脚定义 ==========
#define DHTPIN 6
#define DHTTYPE DHT11
#define I2C_SDA 4
#define I2C_SCL 5
#define LIGHT_ANALOG_PIN 18
#define LIGHT_DIGITAL_PIN 19

// ========== 滑动窗口参数 ==========
const int WINDOW_SIZE = 10;
float tempWindow[WINDOW_SIZE];
float humWindow[WINDOW_SIZE];
float lightWindow[WINDOW_SIZE];
int windowIndex = 0;
bool windowFilled = false;

// ========== 阈值范围 ==========
const float TEMP_MIN = 18.0;
const float TEMP_MAX = 30.0;
const float HUM_MIN = 40.0;
const float HUM_MAX = 80.0;
const int LIGHT_MIN = 500;
const int LIGHT_MAX = 3500;
const float STD_DEV_MULTIPLIER = 2.0;

// ========== 对象声明 ==========
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);
DHT dht(DHTPIN, DHTTYPE);

// ========== 全局变量 ==========
float humidity = 0.0;
float temperature = 0.0;
int lightAnalog = 0;
bool lightDigital = HIGH;
String plantStatus = "初始化";

// ========== 函数声明 ==========
void addToWindow(float temp, float hum, float light);
bool isStableValue(float newVal, float window[], float stdMult);
bool isInRange(float val, float minVal, float maxVal);
void updatePlantStatus();

// ========== 函数实现（与之前完全相同，省略节省篇幅）==========
void addToWindow(float temp, float hum, float light) {
    tempWindow[windowIndex] = temp;
    humWindow[windowIndex] = hum;
    lightWindow[windowIndex] = light;
    windowIndex++;
    if (windowIndex >= WINDOW_SIZE) {
        windowIndex = 0;
        windowFilled = true;
    }
}

bool isStableValue(float newVal, float window[], float stdMult) {
    if (!windowFilled) return true;
    float sum = 0, sumSq = 0;
    for (int i = 0; i < WINDOW_SIZE; i++) {
        sum += window[i];
        sumSq += window[i] * window[i];
    }
    float mean = sum / WINDOW_SIZE;
    float variance = (sumSq / WINDOW_SIZE) - (mean * mean);
    float stddev = sqrt(variance);
    float lowBound = mean - stdMult * stddev;
    float highBound = mean + stdMult * stddev;
    return (newVal >= lowBound && newVal <= highBound);
}

bool isInRange(float val, float minVal, float maxVal) {
    return (val >= minVal && val <= maxVal);
}

void updatePlantStatus() {
    bool tempOk = isInRange(temperature, TEMP_MIN, TEMP_MAX);
    bool humOk  = isInRange(humidity, HUM_MIN, HUM_MAX);
    bool lightOk = isInRange(lightAnalog, LIGHT_MIN, LIGHT_MAX);
    bool tempStable = isStableValue(temperature, tempWindow, STD_DEV_MULTIPLIER);
    bool humStable = isStableValue(humidity, humWindow, STD_DEV_MULTIPLIER);
    bool lightStable = isStableValue(lightAnalog, lightWindow, STD_DEV_MULTIPLIER);
    if (!tempStable || !humStable || !lightStable) {
        plantStatus = "环境波动大";
        return;
    }
    if (tempOk && humOk && lightOk) {
        plantStatus = "健康";
    } else {
        String issues = "";
        if (!tempOk) issues += "温度 ";
        if (!humOk)  issues += "湿度 ";
        if (!lightOk) issues += "光照";
        plantStatus = "注意: " + issues;
    }
}

void setup() {
    Serial.begin(115200);
    delay(1000);
    Serial.println("智能绿植系统启动...");
    dht.begin();
    Wire.begin(I2C_SDA, I2C_SCL);
    Wire.setClock(400000);
    u8g2.begin();
    u8g2.enableUTF8Print();
    // <<<<<<<< 关键修改1：换成 12px 中文字体 >>>>>>>>
    u8g2.setFont(u8g2_font_wqy12_t_gb2312);   // 更小，更紧凑
    u8g2.setFontPosTop();
    analogReadResolution(12);
    analogSetAttenuation(ADC_11db);
    pinMode(LIGHT_DIGITAL_PIN, INPUT_PULLUP);
    for (int i = 0; i < WINDOW_SIZE; i++) {
        tempWindow[i] = 0;
        humWindow[i] = 0;
        lightWindow[i] = 0;
    }
    delay(1000);
    Serial.println("初始化完成");
    u8g2.clearBuffer();
    u8g2.setCursor(0, 0);
    u8g2.print("系统就绪");
    u8g2.sendBuffer();
    delay(2000);
}

void loop() {
    float h = dht.readHumidity();
    float t = dht.readTemperature();
    bool dhtOk = !(isnan(h) || isnan(t));
    if (dhtOk) {
        humidity = h;
        temperature = t;
        Serial.print("温度: "); Serial.print(temperature); Serial.print(" °C    ");
        Serial.print("湿度: "); Serial.print(humidity); Serial.println(" %");
    } else {
        Serial.println("DHT11 读取失败");
    }

    long sum = 0;
    const int SAMPLES = 5;
    for (int i = 0; i < SAMPLES; i++) {
        sum += analogRead(LIGHT_ANALOG_PIN);
        delay(2);
    }
    lightAnalog = sum / SAMPLES;
    lightDigital = digitalRead(LIGHT_DIGITAL_PIN);
    Serial.print("光照模拟值: "); Serial.print(lightAnalog);
    Serial.print("  数字输出: "); Serial.println(lightDigital ? "暗区触发" : "亮区未触发");

    if (dhtOk) {
        addToWindow(temperature, humidity, lightAnalog);
    }

    if (windowFilled && dhtOk) {
        updatePlantStatus();
    } else if (!windowFilled) {
        plantStatus = "采集中...";
    } else if (!dhtOk) {
        plantStatus = "传感器错误";
    }

    // ========== OLED 显示（小字体，行间距 14px）==========
    u8g2.clearBuffer();

    // 第1行 (Y=0)
    u8g2.setCursor(0, 0);
    u8g2.print("温度: ");
    if (dhtOk) { u8g2.print(temperature, 1); u8g2.print(" C"); } else { u8g2.print("---"); }

    // 第2行 (Y=14)
    u8g2.setCursor(0, 14);
    u8g2.print("湿度: ");
    if (dhtOk) { u8g2.print(humidity, 0); u8g2.print(" %"); } else { u8g2.print("---"); }

    // 第3行 (Y=28)
    u8g2.setCursor(0, 28);
    u8g2.print("光照: ");
    u8g2.print(lightAnalog);
    u8g2.print(" (0-4095)");

    // 第4行 (Y=42)
    u8g2.setCursor(0, 42);
    u8g2.print("亮度: ");
    if (lightAnalog < 1500) u8g2.print("亮");
    else if (lightAnalog < 3000) u8g2.print("中");
    else u8g2.print("暗");
    u8g2.print("/");
    u8g2.print(lightDigital ? "触发" : "未触发");

    // 第5行 (Y=56) —— 状态行
    u8g2.setCursor(0, 56);
    u8g2.print("状态: ");
    u8g2.print(plantStatus);

    u8g2.sendBuffer();
    delay(500);
}