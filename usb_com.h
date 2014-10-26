bool usb_debug_init(bool activate);

bool usb_debug_setup(bool* setup_mask);

bool usb_debug_imu_tx(int* imu_data);

bool usb_debug_rf_data(char* data);

bool usb_debug_rf_drop_count(unsigned long count);

