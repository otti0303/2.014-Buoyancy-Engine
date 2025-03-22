# upload LED_testing.cpp or main2.cpp (depending on testing) using arduino ide
# then run this script on laptop terminal which will connect to esp32 port using python
# will let you send commands with laptop keys

import serial
from pynput import keyboard  # Use pynput for key press listening

# Update with the correct serial port
ser = serial.Serial('/dev/cu.usbserial-110', 115200, timeout=1)

def send_command(command):
    ser.write(command.encode() + b'\n')

# Define key press actions
def on_press(key):
    try:

        if key.char == 'a':
            send_command("ASCEND")
        elif key.char == 'd': 
            send_command("DESCEND")

        elif key.char == 'p': 
            send_command("ABORT_ASCENT")
        elif key.char == 's':
            send_command("ABORT_DESCENT")

        elif key.char == 'o':
            send_command("ALL_OFF") # 'reset'

        elif key.char == 't':
            send_command('TURN_ON')

    except AttributeError:
        pass 

# Set up listener for key presses
with keyboard.Listener(on_press=on_press) as listener:
    listener.join()