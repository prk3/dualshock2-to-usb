#include <Arduino.h>

#include <PSX.h>
#include <Joystick.h>

const int DATA_PIN  = PIN2; // Brown wire
const int CMD_PIN   = PIN3; // Orange wire
const int ATT_PIN   = PIN5; // Yellow wire
const int CLOCK_PIN = PIN4; // Blue wire

// Playstation 2 controller
PSX ps2_controller;

// Playstation 2 controller data storage
PSX::PSXDATA ps2_controller_data;
int ps2_controller_error;

// USB gamepad
Joystick_ usb_gamepad;

void setup() {
  // Set up the Playstation 2 controller
  ps2_controller.setupPins(DATA_PIN, CMD_PIN, ATT_PIN, CLOCK_PIN, 10);
  ps2_controller.config(PSXMODE_ANALOG);

  // Set up the USB gamepad
  usb_gamepad.begin(false);
  usb_gamepad.setXAxisRange(0, 255);
  usb_gamepad.setYAxisRange(0, 255);
  usb_gamepad.setZAxisRange(0, 255);
  usb_gamepad.setRxAxisRange(0, 255);
  usb_gamepad.setRyAxisRange(0, 255);
  usb_gamepad.setRzAxisRange(0, 255);
}

void loop() {
  // Read PS2 controller state
  ps2_controller_error = ps2_controller.read(ps2_controller_data);

  // Check if the read was successful
  if (ps2_controller_error == PSXERROR_SUCCESS) {

    // Pass PS2 controller state to USB gamepad
    // Mapping compatible with PS3 and PS4 controllers
    // http://gamepadviewer.com/

    // Shapes
    usb_gamepad.setButton(0, ps2_controller_data.buttons & PSXBTN_CROSS    ? 1 : 0);
    usb_gamepad.setButton(1, ps2_controller_data.buttons & PSXBTN_CIRCLE   ? 1 : 0);
    usb_gamepad.setButton(2, ps2_controller_data.buttons & PSXBTN_SQUARE   ? 1 : 0);
    usb_gamepad.setButton(3, ps2_controller_data.buttons & PSXBTN_TRIANGLE ? 1 : 0);

    // Back buttons
    usb_gamepad.setButton(4, ps2_controller_data.buttons & PSXBTN_L1 ? 1 : 0);
    usb_gamepad.setButton(5, ps2_controller_data.buttons & PSXBTN_R1 ? 1 : 0);
    usb_gamepad.setButton(6, ps2_controller_data.buttons & PSXBTN_L2 ? 1 : 0);
    usb_gamepad.setButton(7, ps2_controller_data.buttons & PSXBTN_R2 ? 1 : 0);

    // Start and select
    usb_gamepad.setButton(8, ps2_controller_data.buttons & PSXBTN_SELECT ? 1 : 0);
    usb_gamepad.setButton(9, ps2_controller_data.buttons & PSXBTN_START  ? 1 : 0);

    // Left analog stick
    usb_gamepad.setXAxis(ps2_controller_data.JoyLeftX);
    usb_gamepad.setYAxis(ps2_controller_data.JoyLeftY);
    usb_gamepad.setButton(10, ps2_controller_data.buttons & 0x0200 ? 1 : 0);

    // Right analog stick
    usb_gamepad.setZAxis(ps2_controller_data.JoyRightX);
    usb_gamepad.setRxAxis(ps2_controller_data.JoyRightY);
    usb_gamepad.setButton(11, ps2_controller_data.buttons & 0x0400 ? 1 : 0);

    // D-pad
    usb_gamepad.setButton(12, ps2_controller_data.buttons & PSXBTN_UP    ? 1 : 0);
    usb_gamepad.setButton(13, ps2_controller_data.buttons & PSXBTN_DOWN  ? 1 : 0);
    usb_gamepad.setButton(14, ps2_controller_data.buttons & PSXBTN_LEFT  ? 1 : 0);
    usb_gamepad.setButton(15, ps2_controller_data.buttons & PSXBTN_RIGHT ? 1 : 0);
  }

  // Send buttons to the computer
  usb_gamepad.sendState();
}
