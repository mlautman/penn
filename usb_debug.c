/*********************************************
 *********************************************
 *
 *  Project: Penn
 *  Script: usb_debug.c
 *  Authors: Mike Lautman
 *********************************************
 *********************************************
 */

#import "usb_debug.h"
#import "m_usb.h"
#import "m_general.h"
#include "wireless.h"

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
    } else{
        return True;
    }
}


////////////////////////////////////////////////
// USB_DEBUG_send_setupMask
//
// Functionality :
//   - send success or failure of setup ops muxed into single value
//  [
//      wireless_init,
//      wireless_isConnected,
//      imu_init,
//      loop_timer,
//      stopWatch_timer,
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
    // TODO need a wireless_isInit() method
    // TODO need a imu_isInit() method
    // TODO need a loop_isSet() method
    unsigned char wireless_init = (wireless_isInit()? 1 : 0);
    unsigned char wireless_isConnected = (test_connection()?1:0);
    unsigned char imu_init = (imu_isInit()?1:0);
    unsigned char loop_timer_running = loop_isSet()?1:0;

    unsigned char setupMask = 0;
    setupMask |= wireless_init;
    setupMask |= wireless_isConnected<<1;
    setupMask |= imu_init;
    setupMask |= loop_timer_running;
    setupMask |= stopWatch_timer_running;

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
    m_usb_tstring("\n\r");
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
    m_usb_tstring("\n\r");
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


