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
int rf_packet_len = 17;
char rf_base_addr = 0x11;

// Wireless packet types
char rf_packet_test = 0;
char rf_packet_IMU = 1;

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
    char data [1] = {0};
    send_packet(0, 0, data, 1);

    m_wait(100);

    char data_back [rf_packet_len];
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
bool send_packet(char packet_type, unsigned long time_stamp, char*data, unsigned int data_len) {
    char toSend [rf_packet_len];
    toSend[0] = packet_type;
    int ii;
    for (ii=0; ii<4; ii++) {
        toSend[ii+1] = ((char*)&time_stamp)[ii]; //assumes long is 4 bytes
    }
    if (data_len > (rf_packet_len - 5)) {
        data_len = rf_packet_len - 5; //TODO: FIX THIS SILENT FAILURE
    }
    for (ii=0;ii<data_len;ii++) {
        toSend[ii+5] = data[ii];
    }

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
//      data:   pointer to char[6] array for storing data
//
// RESPONSE:
//      true:   success
//      false:  failures
bool read_packet(char* data) {
    return (bool)m_rf_read(data, rf_packet_len);
}


ISR (INT2_vect) {
    // set flag for reading in the main loop?
}
