#ifndef _LED_H
#define _LED_H
#include <WS2812FX.h>

// =========================
// RGB 配置
// =========================
#define LED_COUNT 1
#define LED_DIN 48 // 按你硬件的实际 GPIO 修改

// 颜色定义
#define RGB_BLUE 0x0000FF
#define RGB_RED 0xFF0000

// 全局对象
WS2812FX ws2812fx = WS2812FX(LED_COUNT, LED_DIN, NEO_GRB + NEO_KHZ800);

enum RGB_STATE
{
    RGB_BLE_START,     // BLE 服务启动（蓝色闪烁）
    RGB_BLE_CONNECTED, // BLE 已连接（蓝色常亮）
};

RGB_STATE rgb_state = RGB_BLE_START;

// =========================
// BLE 服务启动：蓝色闪烁
// =========================
void rgb_show_ble_start()
{
    rgb_state = RGB_BLE_START;

    ws2812fx.setMode(FX_MODE_BLINK); // 闪烁
    ws2812fx.setSpeed(1000);
    ws2812fx.setColor(RGB_BLUE);
}

// =========================
// BLE 已连接：七彩渐变灯
// =========================
void rgb_show_ble_connected()
{
    rgb_state = RGB_BLE_CONNECTED;

    ws2812fx.setMode(FX_MODE_RAINBOW_CYCLE); // 七彩渐变灯
    ws2812fx.setSpeed(3000);                 // 效果速度（可调）
    // RAINBOW 模式不需要 setColor()
}

// =========================
// RGB Task
// =========================
void rgb_loop_task(void *pvParameters)
{
    Serial.println("[RGB] Task Started");
    ws2812fx.init();
    ws2812fx.setBrightness(80);
    ws2812fx.start();

    for (;;)
    {
        ws2812fx.service();
        vTaskDelay(1 / portTICK_PERIOD_MS);
    }
}

#endif // __LED_H__
