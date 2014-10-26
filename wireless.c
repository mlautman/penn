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
// Packet contents (15 bytes):
//      packet type (1 byte), [time stamp (2 bytes)], data (12-14 bytes)
//
////////////////////////////////////////////////

// Wireless packet length (refer to notes)
rf_packet_len = 15;

////////////////////////////////////////////////
// INIT_CONNECTION
//
// FUNCTIONALITY:
//      Initialize m_rf wireless board channel and address for communication
//
// PARAMETERS:
//      receiver_addr:  unsigned int 0x00 to 0xFF
//      receiver_chan:  unsigned int 1 to 32
//
// RESPONSE:
//      true:   success
//      false:  failure
bool init_wireless(unsigned char receiver_addr, unsigned char receiver_chan) {
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
    return false;
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
bool send_packet(unsigned char packet_type, unsigned long time_stamp, char*data) {
    return false;
}


////////////////////////////////////////////////
// READ_PACKET
//
// FUNCTIONALITY:
//      Read data from the base station
//
// PARAMETERS:
//      data:   pointer to char[6] array for storing data
//
// RESPONSE:
//      true:   success
//      false:  failure
bool read_packet(char* data) {
    return false;
}


ISR (INT2_vect) {

}
