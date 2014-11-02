/*********************************************
 *********************************************
 *
 *	Project: Penn
 * 	Script: wireless.h
 *  Author: Mike Lautman, Justin Yim
 *********************************************
 *********************************************
 */

#ifndef __WIRELESS__PENN__
#define __WIRELESS__PENN__

#include <avr/io.h>
#include <stdlib.h>
#include "m_general.h"
#include "m_rf.h"
// includes stdbool


////////////////////////////////////////////////
// NOTES:
//
// Packet contents (17 bytes):
//      packet type (1 byte), [time stamp (4 bytes)], data (12-14 bytes)
//
////////////////////////////////////////////////


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
bool init_wireless(uint8_t receiver_addr, uint8_t receiver_chan);


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
bool test_connection();


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
bool send_packet(int8_t packet_type, uint32_t time_stamp, int8_t* data, uint16_t data_len, uint8_t button_);


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
//      false:  failure
bool read_packet(int8_t* data);


////////////////////////////////////////////////
// Note:
//      Interrupt for data reading implemented in wireless.c


#endif
