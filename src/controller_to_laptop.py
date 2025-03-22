# all failed attempts at trying to read the logitech controller data from laptop
# seems like an issue with macOS compatibility

import hid
import pyautogui
import time

# Define the mapping between controller buttons and keyboard keys
button_to_key = {
    1: 'a',  # Button A on the controller maps to the 'a' key
    2: 'b',  # Button B on the controller maps to the 'b' key
    3: 'x',  # Button X on the controller maps to the 'x' key
    4: 'y',  # Button Y on the controller maps to the 'y' key
}

# List available HID devices
def list_hid_devices():
    devices = hid.enumerate()
    for device in devices:
        print(f"Device: {device}")

try:
    # List all available HID devices

    print("Available HID devices:")
    list_hid_devices()

    # Attempt to open the HID device by path
    device_path = b'DevSrvsID:4294970745'  # Path from device listing
    device = hid.device(path=device_path)
    print("Controller connected and ready!")

    # Listen for button presses
    print("Listening for button presses... Press Ctrl+C to exit.")
    while True:
        # Read the data from the device (controller)
        
        data = device.read(128)
        print("Raw data received:", data)

        if data:
            # Buttons are typically in the first byte (check your controller's report structure)
            buttons = data[1]  # Assuming button states are in the second byte

            # Check each button and simulate a key press
            for button_id, key in button_to_key.items():
                if buttons & (1 << (button_id - 1)):  # Check if the button is pressed
                    pyautogui.press(key)
                    print(f"Button {button_id} pressed, sending key '{key}'")

        time.sleep(0.5)  # Sleep to prevent high CPU usage

except ValueError as e:
    print(f"Error opening device: {e}")
except KeyboardInterrupt:
    print("\nExiting program.")
finally:
    if 'device' in locals() and device:
        device.close()
        print("Device closed.")


# Device: {'path': b'DevSrvsID:4294970745', 'vendor_id': 1133, 'product_id': 49689, 'serial_number': '', 'release_number': 773, 'manufacturer_string': 'Logitech', 'product_string': 'Logitech Cordless RumblePad 2', 'usage_page': 1, 'usage': 5, 'interface_number': 0, 'bus_type': 1}


# import evdev

# # Find the device path (e.g., /dev/input/event0)
# device = evdev.InputDevice("/dev/input/event0")
# print(device.path)  # Print the device path

# # Read events
# for event in device.read():
#     if event.type == evdev.EV_KEY and event.value == 1:  # Key press
#         print(event.code)
#     elif event.type == evdev.EV_ABS:  # Analog stick/trigger
#         print(event.code, event.value)




# import pygame
# import time

# pygame.init()

# # Find all joysticks
# pygame.joystick.init()

# # Open the first joystick
# joystick = pygame.joystick.Joystick(0)
# joystick.init()

# print("Joystick connected:", joystick.get_name())

# # Listen for events
# while True:
#     for event in pygame.event.get():
#         if event.type == pygame.JOYBUTTONDOWN:
#             print(f"Button {event.button} pressed")
#         elif event.type == pygame.JOYAXISMOTION:
#             print(f"Axis {event.axis} moved to {event.value}")
    
#     time.sleep(0.1)




# import pygame

# # Initialize pygame
# pygame.init()

# # Initialize the joystick system
# pygame.joystick.init()

# # Get the number of joysticks connected
# joystick_count = pygame.joystick.get_count()
# print(f"Number of joysticks connected: {joystick_count}")

# # Check if there is at least one joystick connected
# if joystick_count > 0:
#     joystick = pygame.joystick.Joystick(0)  # Open the first joystick (0-based index)
#     joystick.init()
#     print(f"Joystick '{joystick.get_name()}' initialized!")
# else:
#     print("No joystick found.")


# import hid

# for device in hid.enumerate():
#     print(f"0x{device['vendor_id']:04x}:0x{device['product_id']:04x} {device['product_string']}")




# import hid
# import pyautogui
# import time

# # Define the mapping between controller buttons and keyboard keys
# button_to_key = {
#     1: 'a',  # Button A on the controller maps to the 'a' key
#     2: 'b',  # Button B on the controller maps to the 'b' key
#     3: 'x',  # Button X on the controller maps to the 'x' key
#     4: 'y',  # Button Y on the controller maps to the 'y' key
# }

# # List available HID devices
# def list_hid_devices():
#     devices = hid.enumerate()
#     for device in devices:
#         print(f"Device: {device}")

# try:
#     # List all available HID devices

#     # print("Available HID devices:")
#     # list_hid_devices()

#     # Attempt to open the HID device by path
#     device_path = b'DevSrvsID:4294970745'  # Path from device listing
#     device = hid.device(path=device_path)
#     print("Controller connected and ready!")

#     # Listen for button presses
#     print("Listening for button presses... Press Ctrl+C to exit.")
#     while True:
#         # Read the data from the device (controller)
#         data = device.read(64)

#         if data:
#             # Buttons are typically in the first byte (check your controller's report structure)
#             buttons = data[1]  # Assuming button states are in the second byte

#             # Check each button and simulate a key press
#             for button_id, key in button_to_key.items():
#                 if buttons & (1 << (button_id - 1)):  # Check if the button is pressed
#                     pyautogui.press(key)
#                     print(f"Button {button_id} pressed, sending key '{key}'")

#         time.sleep(0.1)  # Sleep to prevent high CPU usage

# except ValueError as e:
#     print(f"Error opening device: {e}")
# except KeyboardInterrupt:
#     print("\nExiting program.")
# finally:
#     if 'device' in locals() and device:
#         device.close()
#         print("Device closed.")




# import hid

# devices = hid.enumerate()
# print(devices)