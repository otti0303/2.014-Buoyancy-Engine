// SENSORS CODE
// I2C / CAN BUS INIT & DATA HANDLING

#include "sensors.h"
#include <Arduino.h>
#include "config.h"

// connect to blue robotics pressure sensor inside flooded hull
// connect to temperature sensors?
// connect to computer through tether / i2c ?

// I2C CONNECTION OR CAN BUS CONNECTION

// void i2c_connect() { 
//     // temp sensors and pressure sensor
// }

void initCAN() {
    // Configure TWAI (CAN) driver
    twai_general_config_t g_config = TWAI_GENERAL_CONFIG_DEFAULT(GPIO21, GPIO22, TWAI_MODE_NORMAL);
    twai_timing_config_t t_config = TWAI_TIMING_CONFIG_500KBITS();  // 500 kbps speed
    twai_filter_config_t f_config = TWAI_FILTER_CONFIG_ACCEPT_ALL(); // Accept all messages

    // Install CAN driver
    if (twai_driver_install(&g_config, &t_config, &f_config) == ESP_OK) {
        Serial.println("CAN driver installed");
    } else {
        Serial.println("CAN driver install failed");
        return;
    }

    // Start CAN communication
    if (twai_start() == ESP_OK) {
        Serial.println("CAN Bus started");
    } else {
        Serial.println("Failed to start CAN Bus");
        return;
    }
}

void sendCANMessage(uint32_t sensorID) {
    twai_message_t message;
    message.identifier = sensorID; // Sensor ID
    message.extd = 0;  // Standard frame
    message.data_length_code = 1;  // Request message
    message.data[0] = 0x01;  // Command to request data

    if (twai_transmit(&message, pdMS_TO_TICKS(1000)) == ESP_OK) {
        Serial.printf("Sent CAN request to sensor 0x%X\n", sensorID);
    } else {
        Serial.printf("Failed to send CAN message to 0x%X\n", sensorID);
    }
}

float receiveCANMessage() { // may need to create a CANMessage struct, and return a CAN message rather than a float ??
    twai_message_t message;

    if (twai_receive(&message, pdMS_TO_TICKS(1000)) == ESP_OK) {
        Serial.printf("Received CAN message from 0x%X: ", message.identifier);
        for (int i = 0; i < message.data_length_code; i++) {
            Serial.printf("%02X ", message.data[i]);
        }
        Serial.println();

        // Convert received data to float (assuming it's a 4-byte float)
        if (message.data_length_code == 4) {
            float receivedValue;
            memcpy(&receivedValue, message.data, sizeof(float));
            return receivedValue;
        } else {
            Serial.println("Unexpected data length, returning 0.");
            return 0.0;
        }
    } else {
        Serial.println("No CAN response received");
        return 0.0;
    }
}

float getCurrentTemperature() {
    // read from temperature sensors
    // compute an average among temp sensors??
    sendCANMessage(TEMP_SENSOR_ID);
    float temp = receiveCANMessage(); // (check units and range)
    return temp
}

float getHullPressure() {
    // from blue robotics pressure sensor in flooded hull
    sendCANMessage(PRESSURE_SENSOR_ID);
    float press = receiveCANMessage(); // (check units (atm?) and range)
    return press
}