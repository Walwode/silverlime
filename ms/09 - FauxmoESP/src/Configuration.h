#ifndef CONFIGURATION_H
#define CONFIGURATION_H

// --- pins ---
#define SERIAL_BAUD 115200
#define IR_SND_PIN  5   // esp8862 pin D0 = 16
#define IR_RCV_PIN  0    // esp8862 pin D3 = 0
#define RC_SND_PIN  4    // esp8862 pin D2 = 4
#define RC_RCV_PIN  15    // esp8862 pin D1 = 5
#define RF_CE_PIN   2    // esp8862 pin D4 or D8
#define RF_CSN_PIN  15   // esp8862 pin D4 or D8

// --- wifi ---
#define WIFI_SSID "WLAN-9PGVPK"
#define WIFI_PASS "6763142139087153"

// --- remote event enumeration ---
#define TV_POWER_ON        110
#define TV_POWER_OFF       111
#define PC_POWER_ON        120
#define PC_POWER_OFF       121
#define SAMSUNG_POWER      10
#define SAMSUNG_SMARTHUB   11
#define SAMSUNG_OK         15
#define RECEIVER_POWER     20

// --- InfraRed Signals ---
#define IR_SAMSUNG_POWER      0xE0E040BF
#define IR_SAMSUNG_SMARTHUB   0xE0E09E61
#define IR_SAMSUNG_OK         0xE0E016E9
#define IR_RECEIVER_POWER_1   0x20C
#define IR_RECEIVER_POWER_2   0xA0C

// uint16_t samsungVolumeDown[135] = {4584, 4426,  630, 1612,  626, 1612,  624, 1612,  628, 494,  624, 492,  628, 490,  630, 492,  626, 492,  628, 1612,  628, 1612,  628, 1614,  624, 492,  628, 492,  626, 490,  630, 488,  628, 494,  626, 1612,  626, 1610,  628, 492,  626, 1614,  626, 490,  628, 492,  628, 490,  628, 492,  626, 494,  626, 490,  628, 1614,  626, 488,  632, 1612,  624, 1612,  626, 1614,  624, 1612,  628, 46738,  4578, 4434,  626, 1616,  624, 1614,  624, 1612,  628, 492,  628, 492,  626, 492,  626, 492,  628, 492,  626, 1638,  602, 1610,  626, 1612,  626, 494,  624, 492,  626, 492,  628, 518,  602, 492,  624, 1614,  628, 1610,  628, 492,  626, 1610,  628, 492,  628, 490,  628, 492,  624, 492,  628, 492,  624, 492,  628, 1612,  626, 492,  626, 1614,  622, 1614,  628, 1610,  628, 1614,  626};  // SAMSUNG E0E0D02F
// uint16_t samsungVolumeUp[135] = {4582, 4428,  630, 1612,  624, 1614,  624, 1612,  628, 492,  628, 490,  630, 490,  628, 494,  626, 492,  628, 1614,  624, 1610,  628, 1612,  628, 490,  630, 490,  628, 488,  628, 494,  626, 492,  626, 1614,  624, 1612,  626, 1610,  630, 494,  626, 490,  630, 492,  626, 490,  630, 488,  632, 516,  602, 490,  630, 488,  632, 1608,  630, 1606,  632, 1614,  624, 1614,  626, 1612,  626, 46742,  4580, 4430,  626, 1612,  626, 1614,  626, 1610,  628, 492,  628, 492,  626, 490,  630, 490,  628, 494,  626, 1606,  632, 1612,  628, 1610,  626, 492,  628, 492,  626, 492,  626, 496,  624, 496,  624, 1610,  628, 1610,  628, 1612,  626, 494,  626, 492,  628, 490,  628, 490,  628, 494,  624, 494,  626, 492,  626, 492,  628, 1612,  626, 1616,  622, 1612,  626, 1614,  624, 1612,  628};  // SAMSUNG E0E0E01F
// uint16_t samsungArrowLeft[135] = {4580, 4430,  628, 1614,  626, 1638,  600, 1612,  628, 492,  626, 494,  626, 494,  628, 492,  628, 490,  628, 1610,  628, 1612,  626, 1612,  628, 490,  628, 490,  630, 492,  626, 494,  628, 492,  626, 1614,  626, 490,  628, 1612,  630, 490,  628, 490,  630, 1608,  630, 1612,  628, 490,  630, 490,  628, 1614,  624, 492,  628, 1614,  626, 1640,  600, 492,  626, 492,  628, 1612,  628, 46742,  4582, 4430,  628, 1612,  628, 1610,  628, 1610,  628, 490,  628, 490,  630, 490,  628, 492,  630, 490,  626, 1612,  626, 1612,  628, 1614,  626, 496,  622, 494,  626, 492,  628, 490,  628, 490,  630, 1610,  630, 490,  628, 1612,  626, 490,  628, 494,  628, 1610,  626, 1612,  626, 492,  628, 492,  628, 1614,  626, 492,  626, 1612,  628, 1610,  630, 490,  628, 490,  630, 1612,  626};  // SAMSUNG E0E0A659
// uint16_t samsungArrowRight[135] = {4586, 4430,  630, 1610,  628, 1608,  630, 1612,  628, 490,  630, 490,  628, 492,  628, 490,  630, 492,  628, 1614,  626, 1612,  628, 1638,  602, 490,  630, 488,  630, 490,  632, 488,  630, 490,  630, 490,  630, 1610,  630, 488,  628, 494,  626, 490,  630, 1610,  630, 1610,  628, 492,  626, 1614,  626, 488,  632, 1610,  628, 1608,  630, 1610,  630, 490,  630, 486,  630, 1612,  628, 46740,  4584, 4430,  628, 1610,  628, 1614,  624, 1614,  624, 494,  626, 490,  630, 490,  628, 492,  628, 490,  630, 1608,  630, 1636,  602, 1610,  630, 486,  632, 488,  630, 490,  628, 492,  628, 492,  626, 490,  630, 1612,  628, 492,  626, 492,  628, 492,  630, 1610,  630, 1610,  628, 492,  628, 1608,  630, 490,  628, 1612,  626, 1610,  628, 1612,  628, 492,  628, 492,  626, 1614,  626};  // SAMSUNG E0E046B9
// uint16_t samsungSource[135] = {4584, 4430,  630, 1610,  630, 1608,  632, 1606,  632, 490,  628, 488,  630, 486,  632, 514,  604, 490,  630, 1610,  630, 1608,  630, 1608,  630, 492,  628, 490,  630, 486,  632, 486,  634, 488,  630, 1608,  630, 488,  632, 488,  632, 488,  628, 488,  632, 490,  628, 488,  630, 488,  630, 490,  628, 1610,  630, 1610,  630, 1608,  632, 1606,  634, 1606,  632, 1610,  626, 1610,  630, 46740,  4586, 4424,  632, 1608,  628, 1612,  626, 1608,  630, 516,  604, 490,  632, 488,  630, 486,  632, 488,  630, 1612,  628, 1608,  630, 1608,  632, 490,  628, 488,  632, 486,  630, 488,  630, 490,  630, 1610,  628, 488,  630, 488,  630, 492,  628, 490,  630, 490,  632, 488,  630, 490,  630, 488,  630, 1608,  630, 1610,  628, 1610,  628, 1610,  630, 1608,  630, 1610,  630, 1612,  628};  // SAMSUNG E0E0807F

// --- 433MHz Signals ---
#define RC_POWER_ON_CH1    "010001000001010100110011"
#define RC_POWER_OFF_CH1   "010001000001010100111100"
#define RC_POWER_ON_CH2    "010001000001010111000011"
#define RC_POWER_OFF_CH2   "010001000001010111001100"
#define RC_POWER_ON_CH3    "010001000001011100000011"
#define RC_POWER_OFF_CH3   "010001000001011100001100"
#define RC_POWER_ON_CH4    "010001000001110100000011"
#define RC_POWER_OFF_CH4   "010001000001110100001100"

#endif