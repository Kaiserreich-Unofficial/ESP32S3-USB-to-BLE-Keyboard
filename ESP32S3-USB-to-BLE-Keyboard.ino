#include <BleKeyboard.h>
#include <USBKeyboardHostLib.h>
#include "led.h"

USBKeyboardHostLib usbKeyboardHostLib;
BleKeyboard bleKeyboard;

void setup()
{
  Serial.begin(115200);
  xTaskCreatePinnedToCore(rgb_loop_task, "rgb_task", 4096, NULL, 1, NULL, 1);

  Serial.println("Starting usb host!");
  usbKeyboardHostLib.begin();

  // BLE 初始化前 → 蓝色闪烁
  rgb_show_ble_start();
  Serial.println("Starting BLE service!");
  bleKeyboard.begin();
}

void loop()
{
  // BLE 已连接后 → 七彩渐变灯
  if (bleKeyboard.isConnected() && rgb_state != RGB_BLE_CONNECTED)
  {
    rgb_show_ble_connected();
  }

  // Get USB data
  usbKeyboardHostLib.pollingData();

  if (usbKeyboardHostLib.usb_data_buffer != NULL && usbKeyboardHostLib.isKeyboardPollingPub)
  {

    // Clear the polling flag
    usbKeyboardHostLib.isKeyboardPollingPub = 0;
    // Send USB Keyboard to BLE
    bleKeyboard.sendUSBReport(usbKeyboardHostLib.usb_data_buffer);
  }
}
