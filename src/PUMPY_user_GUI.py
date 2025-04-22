# RUN ON LAPTOP
# run this script on laptop to connect to ESP32 port
# make sure that PUMPY.cpp has already been uploaded to the ESP32
# make sure that you have exited out of the arduino IDE

# if this script outputs a weird GUI thats basically a black screen
# copy the code into a new window of whatever software you're using and run it there
# platform.ini doesn't like tkinter

import tkinter as tk
import time

import serial

import matplotlib
matplotlib.use('TkAgg')

import matplotlib.pyplot as plt
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg
from collections import deque

# CHECK CORRECT PORT EACH TIME
ser = serial.Serial('/dev/cu.usbserial-10', 115200, timeout=1)

def send_command(command):
    ser.write(command.encode() + b'\n')

time_data = deque(maxlen=500)  # Store up to 500 data points
temp1_data = deque(maxlen=500)

def update_sensors_and_plot():
    while ser.in_waiting > 0:
        line = ser.readline().decode('utf-8').strip()

        print(f"Received: {line}")

        if "TASK COMPLETED" in line.upper():
            reset()
            continue  # Skip parsing this line

        try:
            # Parse format: TEMP1:x,TEMP2:y,PRESSURE:z,DEPTH:w
            parts = line.split(',')
            data_dict = {kv.split(':')[0].strip(): float(kv.split(':')[1].strip()) for kv in parts}

            if 'TEMP1' in data_dict:
                ext_temperature.set(f"{data_dict['TEMP1']:.2f} °C")

            if 'TEMP2' in data_dict:
                int_temperature.set(f"{data_dict['TEMP2']:.2f} °C")

                # now = time.time() - start_time
                # time_data.append(now)
                # temp_string = int_temperature.get()

                # try:
                #     temp_float = float(temp_string.replace(" °C", ""))  # Remove the " °C" and convert to float
                #     temp1_data.append(temp_float)  # Add the float value to your data
                # except ValueError:
                #     pass  

                now = time.time() - start_time
                time_data.append(now)
                temp1_data.append(data_dict['TEMP2'])

            if 'PRESSURE' in data_dict:
                pressure.set(f"{data_dict['PRESSURE']:.2f} hPa")

            if 'DEPTH' in data_dict:
                depth.set(f"{data_dict['DEPTH']:.2f} m")

        except Exception as e:
            print(f"Parse error: {e}")

    root.after(1000, update_sensors_and_plot)



def update_timer():
    elapsed_time = int(time.time() - start_time)
    minutes, seconds = divmod(elapsed_time, 60)
    timer.set(f"{minutes}m {seconds}s")
    root.after(1000, update_timer)

def on_close():
    root.destroy()

    new_window = tk.Tk()
    new_window.title("Temperature vs Time")

    fig, ax = plt.subplots(figsize=(5, 4), dpi=100)
    ax.plot(time_data, temp1_data)
    ax.set_title("Temperature vs Time")
    ax.set_xlabel("Time (s)")
    ax.set_ylabel("Temperature (°C)")

    canvas = FigureCanvasTkAgg(fig, master=new_window)
    canvas.draw()
    canvas.get_tk_widget().pack(fill=tk.BOTH, expand=True)

    new_window.mainloop()

def ascend():
    status_label.config(text="Status: Ascending", fg="blue")
    send_command("ASCEND")

def descend():
    status_label.config(text="Status: Descending", fg="blue")
    send_command("DESCEND")

def abort_ascent():
    status_label.config(text="Status: Ascent Aborted", fg="red")
    send_command("ABORT_ASCENT")

def abort_descent():
    status_label.config(text="Status: Descent Aborted", fg="red")
    send_command("ABORT_DESCENT")

def reset():
    status_label.config(text="Status: Idle", fg="green")
    send_command("RESET")

BUTTON_WIDTH = 20
BUTTON_HEIGHT = 2
FONT_BUTTON = ("Arial", 18, "bold")
FONT_LABEL = ("Arial", 18)
FONT_VALUE = ("Arial", 20, "bold")

# Create the main window
root = tk.Tk()
root.title("2.014 BUOYANCY ENGINE DASHBOARD")
root.geometry("1000x500")
root.configure(bg="white")

# Sensor values
pressure = tk.StringVar()
depth = tk.StringVar()
int_temperature = tk.StringVar()
ext_temperature = tk.StringVar()
console = tk.StringVar()
timer = tk.StringVar()
duty_cycle_value = tk.StringVar()

# Timer variable
timer = tk.StringVar()
timer.set("0m 0s")

# Start time for timer
start_time = time.time()
update_timer()

# Title label
title_label = tk.Label(root, text="2.014 BUOYANCY ENGINE DASHBOARD", font=("Arial", 26, "bold"))
title_label.pack(pady=10)

# Container for three frames
sensor_container = tk.Frame(root, bg="white")
sensor_container.pack(pady=10, padx=20)

# Pressure Frame
pressure_frame = tk.Frame(sensor_container, bg="lightgray", bd=2, relief="solid")
pressure_frame.pack(side="left", padx=10, ipadx=5, ipady=5)

pressure_label = tk.Label(pressure_frame, text="Pressure:", font=FONT_LABEL, bg="lightgray")
pressure_label.grid(row=0, column=0, padx=5, pady=5)
pressure_value = tk.Label(pressure_frame, textvariable=pressure, font=FONT_VALUE, bg="lightgray")
pressure_value.grid(row=0, column=1, padx=5, pady=5)

depth_label = tk.Label(pressure_frame, text="Depth (freshwater):", font=FONT_LABEL, bg="lightgray")
depth_label.grid(row=1, column=0, padx=5, pady=5)
depth_value = tk.Label(pressure_frame, textvariable=depth, font=FONT_VALUE, bg="lightgray")
depth_value.grid(row=1, column=1, padx=5, pady=5)

# Temperature Frame
temp_frame = tk.Frame(sensor_container, bg="lightgray", bd=2, relief="solid")
temp_frame.pack(side="left", padx=10, ipadx=5, ipady=5)

int_temperature_label = tk.Label(temp_frame, text="Internal temperature:", font=FONT_LABEL, bg="lightgray")
int_temperature_label.grid(row=0, column=0, padx=5, pady=5)
int_temperature_value = tk.Label(temp_frame, textvariable=int_temperature, font=FONT_VALUE, bg="lightgray")
int_temperature_value.grid(row=0, column=1, padx=5, pady=5)

ext_temperature_label = tk.Label(temp_frame, text="External temperature:", font=FONT_LABEL, bg="lightgray")
ext_temperature_label.grid(row=1, column=0, padx=5, pady=5)
ext_temperature_value = tk.Label(temp_frame, textvariable=ext_temperature, font=FONT_VALUE, bg="lightgray")
ext_temperature_value.grid(row=1, column=1, padx=5, pady=5)

# Timer frame
timer_frame = tk.Frame(sensor_container, bg="lightgray", bd=2, relief="solid")
timer_frame.pack(side="left", padx=10, ipadx=5, ipady=5)

timer_label = tk.Label(timer_frame, text="Time Elapsed:", font=FONT_LABEL, bg="lightgray")
timer_label.grid(row=0, column=0, padx=5, pady=5)
timer_value = tk.Label(timer_frame, textvariable=timer, font=FONT_VALUE, bg="lightgray")
timer_value.grid(row=0, column=1, padx=5, pady=5)

# Buttons
button_frame = tk.Frame(root, bg="white")
button_frame.pack(pady=10)

tk.Button(button_frame, text="ASCEND", command=ascend, width=BUTTON_WIDTH, height=BUTTON_HEIGHT, bg="white", fg="blue", font=FONT_BUTTON).grid(row=0, column=0, padx=10, pady=5)
tk.Button(button_frame, text="DESCEND", command=descend, width=BUTTON_WIDTH, height=BUTTON_HEIGHT, bg="white", fg="blue", font=FONT_BUTTON).grid(row=0, column=1, padx=10, pady=5)
tk.Button(button_frame, text="ABORT ASCENT", command=abort_ascent, width=BUTTON_WIDTH, height=BUTTON_HEIGHT, bg="white", fg="darkred", font=FONT_BUTTON).grid(row=1, column=0, padx=10, pady=5)
tk.Button(button_frame, text="ABORT DESCENT", command=abort_descent, width=BUTTON_WIDTH, height=BUTTON_HEIGHT, bg="white", fg="darkred", font=FONT_BUTTON).grid(row=1, column=1, padx=10, pady=5)
tk.Button(button_frame, text="RESET", command=reset, width=BUTTON_WIDTH*2+1, height=BUTTON_HEIGHT, bg="white", fg="green", font=FONT_BUTTON).grid(row=2, column=0, columnspan=2, padx=10, pady=5)

# Status label
status_label = tk.Label(root, text="Status: Idle", font=("Arial", 20, "bold"), bg="white")
status_label.pack(pady=10)

label = tk.Label(root, text="Close this window to see a plot of temperature vs time.", font=("Arial", 16))
label.pack(padx=20, pady=20)

# Bind the close event
root.protocol("WM_DELETE_WINDOW", on_close)

root.after(1000, update_sensors_and_plot)

# Run the GUI
root.mainloop()