/*********************************************
 *********************************************
 *
 *  Project: Penn
 *  Script: usb_debug.c
 *  Authors: Mike Lautman
 *********************************************
 *********************************************
 */

#include "usb_debug.h"

////////////////////////////////////////////////
// USB_DEBUG
//
// Functionality :
//   - set whether usb debugger is used
//
// Parameters
//   - bool activate : whether or not to use usb debugger
//
// Returns
//   - usb_debug state
bool usb_debug(bool activate){
    if (activate){
        m_red(ON);
        m_usb_init();
        while(!m_usb_isconnected());
        bool isConn = m_usb_isconnected();
        return isConn;
        m_red(OFF);
    } else {
        return FALSE;
    }
}


////////////////////////////////////////////////
// USB_DEBUG_send_setupMask
//
// Functionality :
//   - send success or failure of setup ops muxed into single value
//  [
//      wireless_isConnected,
//      imu_init,
//      loop_timer,
//      stopWatch_timer,
//      N/A,
//      N/A,
//      N/A,
//      N/A,
//  ]
//
// Parameters
//   - int8_t setup_mask  : mask for setup success/fail bits
//
// Returns
//   - success/failure of transmission

bool usb_debug_send_setupMask(void){
    uint8_t wireless_isConnected = (test_connection()?1:0);
    uint8_t imu_init = (imu_isInit()?1:0);
    uint8_t loop_timer_running = loop_isSet()?1:0;
    uint8_t stopWatch_running = stopwatch_isSet()?1:0;

    uint8_t setupMask = 0;
    setupMask += (wireless_isConnected ? 1:0);
    setupMask += (imu_init ? 1:0)<<1;
    setupMask += (loop_timer_running ? 1:0)<<2;
    setupMask += (stopWatch_running ? 1:0)<<3;
    m_usb_tx_int(setupMask);
    return setupMask;
}



////////////////////////////////////////////////
// USB_DEBUG_IMU_TX
//
// Functionality :
//   - Sends Imu values over usb instead of wireless
//
// Parameters
//   - int16_t* imu_data
//   - int8_t dataLen : lenght of rf_data
//
// Returns
//   - success/failure of transmission
bool usb_debug_imu_tx(int16_t* imu_data, int8_t dataLen){
    m_usb_tx_string("IMU data:\t");
    bool txSuccess = true;

    int16_t i;
    for(i = 0; i < dataLen; i++){
        txSuccess &= m_usb_tx_char('\t');
        m_usb_tx_int(imu_data[i]);
    }
    m_usb_tx_string("\n\r");
    return txSuccess;
}


////////////////////////////////////////////////
// USB_DEBUG_RF_TX
//
// Functionality :
//   - Sends rf packets values over usb instead of wireless
//
// Parameters
//   - int8_t* rf_data : data that would normally be sent over rf
//   - int8_t dataLen : lenght of rf_data
//
// Returns
//   - success/failure of transmission
int16_t rf_packet_len_debug = 17;
bool usb_debug_rf_data(int8_t packet_type, uint32_t time_stamp, int8_t* data, uint32_t data_len){
    m_usb_tx_string("rf_data:\t");

    uint8_t toSend [17] = {0};
    // toSend[0] = (uint8_t)packet_type;
    // int16_t ii;
    // for (ii=0; ii<2; ii++) {
    //     toSend[ii+1] = ((uint8_t*)&time_stamp)[ii]; //assumes long is 4 bytes
    // }
    // if (data_len > (rf_packet_len_debug - 3)) {
    //     data_len = rf_packet_len_debug - 3; //TODO: FIX THIS SILENT FAILURE
    // }
    // for (ii=0;ii<data_len;ii++) {
    //     toSend[ii+3] = data[ii];
    // }

    // packet type
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

    int16_t i;
    m_usb_tx_int((int16_t)toSend[0]);
    m_usb_tx_string("\t");
    uint32_t now = 0;
    now += (uint32_t)toSend[1] << 24;
    now += (uint32_t)toSend[2] << 16;
    now += (uint32_t)toSend[3] << 8;
    now += (uint32_t)toSend[4];

    m_usb_tx_long((long)now);

    for(i = 5; i < rf_packet_len_debug; i+=2){
        m_usb_tx_char('\t');
        m_usb_tx_int(*(int16_t*)&toSend[i]);
    }
    m_usb_tx_string("\n\r");

    bool txSuccess = m_usb_tx_char('\t');

    return txSuccess != -1;
}


////////////////////////////////////////////////
// USB_DEBUG_RF_DROP_COUNT
//
// Functionality :
//   - Sends number of dropped packets
//
// Parameters
//   - uint16_t drop_cnt : number of dropped packets as cnted by wireless
//
// Returns
//   - success/failure of transmission
bool usb_debug_rf_drop_count(uint16_t drop_cnt){
    // TODO
    return false;
}


///////////////////////////////////////////////
// USB_DEBUG_STOPWATCH
//
// Functionality :
//   - Sends stopWatch value
//
// Parameters
//   - uint16_t now : stopWatch cnt value
//
// Returns
//   - success/failure of transmission
bool usb_debug_stopWatch(uint32_t now){
    m_usb_tx_string("stopwatch_now:\t");
    m_usb_tx_ulong(now);
    m_usb_tx_string("\n\r");
    return true;
}


