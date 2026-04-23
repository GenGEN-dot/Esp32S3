#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

// 灯带配置
#define LED_PIN     14      // 数据引脚 D1 接 GPIO14
#define NUM_LEDS    12      // 灯珠数量
#define BRIGHTNESS  1      // 亮度 (0-255)，建议 50 以降低电流

Adafruit_NeoPixel strip(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);

// 颜色预览表 (R, G, B)
uint32_t colors[] = {
    strip.Color(255, 0, 0),     // 红
    strip.Color(0, 255, 0),     // 绿
    strip.Color(0, 0, 255),     // 蓝
    strip.Color(255, 255, 0),   // 黄
    strip.Color(255, 0, 255),   // 紫
    strip.Color(0, 255, 255),   // 青
    strip.Color(255, 255, 255)  // 白
};
const int numColors = sizeof(colors) / sizeof(colors[0]);

// 辅助函数：输入 0~255 色相，返回 RGB 颜色（提前定义，供 loop 调用）
uint32_t Wheel(byte wheelPos) {
    wheelPos = 255 - wheelPos;
    if (wheelPos < 85) {
        return strip.Color(255 - wheelPos * 3, 0, wheelPos * 3);
    }
    if (wheelPos < 170) {
        wheelPos -= 85;
        return strip.Color(0, wheelPos * 3, 255 - wheelPos * 3);
    }
    wheelPos -= 170;
    return strip.Color(wheelPos * 3, 255 - wheelPos * 3, 0);
}

void setup() {
    Serial.begin(115200);
    strip.begin();
    delay(5000);
    strip.setBrightness(BRIGHTNESS);
    strip.show();  // 初始化所有 LED 熄灭
    Serial.println("灯带测试开始");
}

void loop() {
    // 测试 1: 逐个点亮并熄灭，显示每个 LED 的编号
    for (int i = 0; i < NUM_LEDS; i++) {
        strip.setPixelColor(i, strip.Color(255, 100, 0));  // 橙色
        strip.show();
        delay(100);
    }
    delay(500);
    strip.clear();  // 全灭
    strip.show();
    delay(500);

    // 测试 2: 全彩呼吸效果（颜色依次渐变）
    for (int c = 0; c < numColors; c++) {
        for (int i = 0; i < NUM_LEDS; i++) {
            strip.setPixelColor(i, colors[c]);
        }
        strip.show();
        delay(1000);  // 每种颜色停留 1 秒
    }

    // 测试 3: 跑马灯（白色流动）
    for (int i = 0; i < NUM_LEDS + 8; i++) {
        strip.clear();
        // 点亮连续 3 颗灯珠
        for (int j = 0; j < 3; j++) {
            int pos = (i + j) % NUM_LEDS;
            strip.setPixelColor(pos, strip.Color(255, 255, 255));
        }
        strip.show();
        delay(100);
    }
    delay(500);

    // 测试 4: 彩虹渐变（所有灯珠按色相循环）
    for (int j = 0; j < 256; j++) {
        for (int i = 0; i < NUM_LEDS; i++) {
            // 将色相偏移，制造流动感
            int hue = (i * 256 / NUM_LEDS + j) % 256;
            strip.setPixelColor(i, Wheel(hue));
        }
        strip.show();
        delay(20);
    }
}