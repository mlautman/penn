bool init_wireless();
bool init_connection(unsigned char receiver_addr, unsigned char receiver_chan);
bool test_connection();
bool send_imu_data(char*data, unsigned long time_delta);