/*********************************************
 *********************************************
 *
 *	Project: Penn
 * 	Script: wireless.c
 *  Author: Justin Yim
 *********************************************
 *********************************************
 */

#include "wireless.h"

////////////////////////////////////////////////
// NOTES:
//
// Packet contents (17 bytes):
//      packet type (1 byte), [time stamp (4 bytes)], data (12-14 bytes)
//
////////////////////////////////////////////////


// Wireless packet length (refer to notes)
int16_t rf_packet_len = 17;
int8_t rf_base_addr = 0x11;

// Wireless packet types
int8_t rf_packet_test = 0;
int8_t rf_packet_IMU = 1;

////////////////////////////////////////////////
// INIT_CONNECTION
//
// FUNCTIONALITY:
//      Initialize m_rf wireless board channel and address for communication
//
// PARAMETERS:
//      receiver_addr:  uint16_t 0x00 to 0xFF
//      receiver_chan:  uint16_t 1 to 32
//
// RESPONSE:
//      true:   success
//      false:  failure
bool init_wireless(uint8_t receiver_addr, uint8_t receiver_chan) {
    return (bool)m_rf_open(receiver_chan, receiver_addr, rf_packet_len);
}


////////////////////////////////////////////////
// TEST_CONNECTION
//
// FUNCTIONALILTY:
//      Ping the base station to check connection
//
// PARAMETERS:
//      none
//
// RESPONSE:
//      true:   connection is good
//      false:  connection is bad
bool test_connection() {
    int8_t data [1] = {0};
    send_packet(0, 0, data, 1);

    m_wait(100);

    int8_t data_back [rf_packet_len];
    bool heard = read_packet(data_back);
    return heard && data_back[0] == 13; //this is a magic number right now
}


////////////////////////////////////////////////
// SEND_PACKET
//
// FUNCTIONALITY:
//      Transmit data to the base station for logging on the computer
//
// PARAMETERS:
//      packet_type:    meaning of contents of "data" (IMU data, etc.)
//      time_stamp:     time of packet transmission from the M2
//      data:           packet contents
//
// RESPONSE:
//      true:   success
//      false:  failure
bool send_packet(int8_t packet_type, uint32_t time_stamp, int8_t* data, uint16_t data_len) {

    toSend[0] = (uint8_t)packet_type;

    // time_stamp
    toSend[1] = (uint8_t)((time_stamp & 0xff000000) >> 24);
    toSend[2] = (uint8_t)((time_stamp & 0x00ff0000) >> 16);
    toSend[3] = (uint8_t)((time_stamp & 0x0000ff00) >> 8);
    toSend[4] = (uint8_t)((time_stamp & 0x000000ff));

    // Accel
    toSend[5] =  (uint8_t)data[0];
    toSend[6] =  (uint8_t)data[1];
    toSend[7] =  (uint8_t)data[2];
    toSend[8] =  (uint8_t)data[3];
    toSend[9] =  (uint8_t)data[4];
    toSend[10] = (uint8_t)data[5];

    // Gyro
    toSend[11] =  (uint8_t)data[6];
    toSend[12] =  (uint8_t)data[7];
    toSend[13] =  (uint8_t)data[8];
    toSend[14] =  (uint8_t)data[9];
    toSend[15] =  (uint8_t)data[10];
    toSend[16] =  (uint8_t)data[11];

    m_rf_send(rf_base_addr, toSend, rf_packet_len);
    return false;
}


////////////////////////////////////////////////
// READ_PACKET
//
// FUNCTIONALITY:
//      Read data from the base station
//
// PARAMETERS:
//      data:   pointer to int8_t[6] array for storing data
//
// RESPONSE:
//      true:   success
//      false:  failures
bool read_packet(int8_t* data) {
    return (bool)m_rf_read(data, rf_packet_len);
}


ISR (INT2_vect) {
    // set flag for reading in the main loop?
}
