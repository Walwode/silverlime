#ifndef CONFIGURATION_H
#define CONFIGURATION_H

// --- pins ---
#define SERIAL_BAUD 115200
#define IR_SND_PIN  16   // esp8862 pin D0 = GPIO16
#define IR_RCV_PIN  0    // esp8862 pin D3 = GPIO0
#define RC_SND_PIN  4    // esp8862 pin D2 = GPIO4
#define RC_RCV_PIN  5    // esp8862 pin D1 = GPIO5
#define RF_CE_PIN   2    // esp8862 pin D4 or D8
#define RF_CSN_PIN  15   // esp8862 pin D4 or D8

// --- wifi ---
#define WIFI_SSID "WIFI-NAME"
#define WIFI_PASS "WIFI-PASSWORD"

// --- InfraRed Signals ---
#define IR_SAMSUNG_POWER      0xABCDEF01
#define IR_SAMSUNG_SMARTHUB   0xABCDEF02
#define IR_SAMSUNG_OK         0xABCDEF03
#define IR_RECEIVER_POWER_1   0x12A
#define IR_RECEIVER_POWER_2   0x23B

// --- 433MHz Signals ---
#define RC_POWER_ON_CH1    "101010101010010101010101"
#define RC_POWER_OFF_CH1   "101010101010010101010111"
#define RC_POWER_ON_CH2    "101010101010010101011111"
#define RC_POWER_OFF_CH2   "101010101010010111111111"
#define RC_POWER_ON_CH3    "101010101010011111111111"
#define RC_POWER_OFF_CH3   "101010101010111111111111"
#define RC_POWER_ON_CH4    "101010101011111111111111"
#define RC_POWER_OFF_CH4   "101010101111111111111111"

// --- remote event enumeration ---
#define TV_POWER_ON        110
#define TV_POWER_OFF       111
#define PC_POWER_ON        120
#define PC_POWER_OFF       121
#define SAMSUNG_POWER      10
#define SAMSUNG_SMARTHUB   11
#define SAMSUNG_OK         15
#define RECEIVER_POWER     20

#endif
