import serial, time, datetime, os

if __name__ == "__main__":
    # Parameters
    wait_time = 0.1 #time out [seconds]
    directory = 'log'

    # Connect to M2
    m2 = serial.Serial(port='/dev/ttyACM0', baudrate=9600)

    # Character to be written
    char_name = ''
    while char_name == '':
        char_name = raw_input('character to write: ')

    # Log characters to files
    while True:
        # Prepare text file
        nowtime = datetime.datetime.now().isoformat()
        nowtime = nowtime[0:21]
        filename = char_name + '_' + nowtime
        logs = os.listdir('./' + directory)

        text_out = open(directory + '/' + filename,'w')

        # Read measurements
        while m2.inWaiting() == 0:
            pass
        last_time = time.time();
        while True:
            available = m2.inWaiting()
            if available > 0:
                toprint = m2.read(available)
                last_time = time.time();
                print toprint
                text_out.write(toprint)
            if time.time() - last_time > wait_time:
                break
        text_out.close()

        # Get next aciton
        char_in = raw_input('next action: ')
        if len(char_in) != 1:
            if char_in == 'exit':
                print 'Exiting'
                break
            elif char_in == '':
                pass
            elif char_in[0] == '-' & len(char_in == 2):
                os.rename(filename, char_in[1] + filename[1:])
            else:
                print 'Unknown input. Exiting'
                break
        else:
            char_name = char_in


    

# Justin's notes to himself
# use rawinput to get which character was drawn by the user
# argv sysarg
# 



