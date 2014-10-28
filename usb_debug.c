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
        m_usb_init();
        bool isConn = m_usb_isconnected();
        return isConn;
    } else {
        return TRUE;
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
//   - char setup_mask  : mask for setup success/fail bits
//
// Returns
//   - success/failure of transmission

bool usb_debug_send_setupMask(void){
    unsigned char wireless_isConnected = (test_connection()?1:0);
    unsigned char imu_init = (imu_isInit()?1:0);
    unsigned char loop_timer_running = loop_isSet()?1:0;
    unsigned char stopWatch_running = stopwatch_isSet()?1:0;
    unsigned char setupMask = 0;
    setupMask |= wireless_isConnected;
    setupMask |= imu_init<<1;
    setupMask |= loop_timer_running<<2;
    setupMask |= stopWatch_running<<3;

    return m_usb_tx_char(setupMask);
}



////////////////////////////////////////////////
// USB_DEBUG_IMU_TX
//
// Functionality :
//   - Sends Imu values over usb instead of wireless
//
// Parameters
//   - int* imu_data
//   - char dataLen : lenght of rf_data
//
// Returns
//   - success/failure of transmission
bool usb_debug_imu_tx(int* imu_data, char dataLen){
    m_usb_tx_string("IMU data:");
    bool txSuccess = true;

    int i;
    for(i = 0; i < dataLen; i++){
        txSuccess &= m_usb_tx_char('\t');
        txSuccess &= m_usb_tx_char(imu_data[i]);
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
//   - char* rf_data : data that would normally be sent over rf
//   - char dataLen : lenght of rf_data
//
// Returns
//   - success/failure of transmission
bool usb_debug_rf_data(char* data, char dataLen){
    m_usb_tx_string("rf_data:");

    bool txSuccess = true;
    int i;
    for(i = 0; i < dataLen; i++){
        txSuccess &= m_usb_tx_char('\t');
        txSuccess &= m_usb_tx_char(data[i]);
    }
    m_usb_tx_string("\n\r");
    return txSuccess;
}



////////////////////////////////////////////////
// USB_DEBUG_RF_DROP_COUNT
//
// Functionality :
//   - Sends number of dropped packets
//
// Parameters
//   - unsigned int drop_cnt : number of dropped packets as cnted by wireless
//
// Returns
//   - success/failure of transmission
bool usb_debug_rf_drop_count(unsigned int drop_cnt){
    // TODO
    return false;
}


