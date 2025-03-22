// failed attempt at connecting the logitech controller to send cmds directly to the esp32

#include <Bluepad32.h>

GamepadPtr myGamepad = nullptr;

void onGamepadConnected(GamepadPtr gp) {
    Serial.println("Gamepad connected");
    myGamepad = gp;
}

void onGamepadDisconnected(GamepadPtr gp) {
    Serial.println("Gamepad disconnected");
    if (myGamepad == gp) {
        myGamepad = nullptr;
    }
}

void setup() {
    Serial.begin(115200);
    BP32.setup(&onGamepadConnected, &onGamepadDisconnected);
    BP32.forgetBluetoothKeys(); // Optional: Clears stored Bluetooth connections
}

void loop() {
    BP32.update(); // Updates gamepad state

    if (myGamepad && myGamepad->isConnected()) {
        if (myGamepad->a()) {
            Serial.println("A button pressed");
        }
        if (myGamepad->b()) {
            Serial.println("B button pressed");
        }
        if (myGamepad->x()) {
            Serial.println("X button pressed");
        }
        if (myGamepad->y()) {
            Serial.println("Y button pressed");
        }

        int lx = myGamepad->axisX(); // Left joystick X-axis
        int ly = myGamepad->axisY(); // Left joystick Y-axis
        int rx = myGamepad->axisRX(); // Right joystick X-axis
        int ry = myGamepad->axisRY(); // Right joystick Y-axis

        Serial.printf("Joystick: LX=%d LY=%d RX=%d RY=%d\n", lx, ly, rx, ry);
    }
    delay(100);
}