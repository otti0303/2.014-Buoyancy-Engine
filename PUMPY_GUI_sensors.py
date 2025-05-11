# PUMPY GUI FOR SENSORS

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

import csv
from pathlib import Path

import matplotlib
matplotlib.use('TkAgg')

import matplotlib.pyplot as plt
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg
# from collections import deque

# CHECK CORRECT PORT EACH TIME
ser = serial.Serial('/dev/cu.usbmodem1101', 9600, timeout=1)

# filename_var = tk.StringVar(value="data_log.csv")
filename = "test_1.csv"

# time_data = deque(maxlen=500)  # Store up to 500 data points
# temp1_data = deque(maxlen=500)

# === DATA STORAGE ===
time_data1 = []
time_data2 = []
TEMP1_data = []
TEMP2_data = []
start_time = time.time()

PRESSURE_data = []
DEPTH_data = []

# ext_temp_val = None
# int_temp_val = None
# press_val = None
# depth_val = None

def update_sensors_and_plot():

    ext_temp_val = None
    int_temp_val = None
    press_val = None
    depth_val = None

    elapsed = time.time() - start_time

    while ser.in_waiting > 0:
        line = ser.readline().decode('utf-8').strip()

        print(f"Received: {line}")

        try:
            # Parse format: TEMP1:x,TEMP2:y,PRESSURE:z,DEPTH:w
            parts = line.split(',')
            data_dict = {kv.split(':')[0].strip(): float(kv.split(':')[1].strip()) for kv in parts}

            if 'TEMP1' in data_dict:
                ext_temp_val = data_dict['TEMP1']
                ext_temperature.set(f"{data_dict['TEMP1']:.2f} °C")

            if 'TEMP2' in data_dict:
                int_temp_val = data_dict['TEMP2']
                int_temperature.set(f"{data_dict['TEMP2']:.2f} °C")

                # now = time.time() - start_time
                # time_data.append(now)
                # temp1_data.append(data_dict['TEMP2'])

            if 'PRESSURE' in data_dict:
                press_val = data_dict['PRESSURE']
                pressure.set(f"{data_dict['PRESSURE']:.2f} hPa")

            if 'DEPTH' in data_dict:
                depth_val = data_dict['DEPTH']
                depth.set(f"{data_dict['DEPTH']:.2f} m")

        except Exception as e:
            print(f"Parse error: {e}")

    # TEMP1_data.append(ext_temp_val if ext_temp_val is not None else 0.0)
    # TEMP2_data.append(int_temp_val if int_temp_val is not None else 0.0)
    # PRESSURE_data.append(press_val if press_val is not None else 0.0)
    # DEPTH_data.append(depth_val if depth_val is not None else 0.0)

    if ext_temp_val is not None:
        TEMP1_data.append(ext_temp_val)
        time_data1.append(elapsed)

    if int_temp_val is not None:
        TEMP2_data.append(int_temp_val)
        time_data2.append(elapsed)

    if press_val is not None:
        PRESSURE_data.append(press_val)

    if depth_val is not None:
        DEPTH_data.append(depth_val)

    # Plot temp data
    press_line.set_data(time_data1, PRESSURE_data)
    temp2_line.set_data(time_data2, TEMP2_data)

    # Rescale x-axis and y-axes
    ax1.set_xlim(0, elapsed)
    ax2.set_xlim(0, elapsed)

    max_press = max(PRESSURE_data, default=0)
    max_temp2 = max(TEMP2_data, default=0)

    ax1.set_ylim(950, max(0.1, max_press * 1.2))
    ax2.set_ylim(0, max(0.1, max_temp2 * 1.2))

    canvas.draw()
    root.after(500, update_sensors_and_plot)

def update_timer():
    elapsed_time = int(time.time() - start_time)
    minutes, seconds = divmod(elapsed_time, 60)
    timer.set(f"{minutes}m {seconds}s")
    root.after(1000, update_timer)

def on_close():
    with open(Path(filename), mode='w', newline='') as f:
        writer = csv.writer(f)
        writer.writerow(["Time 1 (s)", "Temp 1 (ext) (ºC)", "Time 2 (s)", "Temp 2 (int) (ºC)", "Pressure (hPa)", "Depth (m)"])
        for i in range(len(time_data1)):
            writer.writerow([
                f"{time_data1[i]:.2f}"if i < len(time_data1) else "None",
                f"{TEMP1_data[i]:.2f}" if i < len(TEMP1_data) else "None",
                f"{time_data2[i]:.2f}" if i < len(time_data2) else "None",
                f"{TEMP2_data[i]:.2f}" if i < len(TEMP2_data) else "None",
                f"{PRESSURE_data[i]:.2f}" if i < len(PRESSURE_data) else "None",
                f"{DEPTH_data[i]:.2f}" if i < len(DEPTH_data) else "None"
                ])
            
    root.destroy()
    root.quit()

BUTTON_WIDTH = 20
BUTTON_HEIGHT = 2
FONT_BUTTON = ("Arial", 18, "bold")
FONT_LABEL = ("Arial", 18)
FONT_VALUE = ("Arial", 20, "bold")

# Create the main window
root = tk.Tk()
root.title("2.014 BUOYANCY ENGINE SENSORS DASHBOARD")
root.geometry("1000x750")
root.configure(bg="white")

# Sensor values
pressure = tk.StringVar()
depth = tk.StringVar()
int_temperature = tk.StringVar()
ext_temperature = tk.StringVar()
console = tk.StringVar()
timer = tk.StringVar()

# Timer variable
timer = tk.StringVar()
timer.set("0m 0s")

# Start time for timer
# start_time = time.time()
update_timer()

# Title label
title_label = tk.Label(root, text="2.014 BUOYANCY ENGINE SENSORS DASHBOARD", font=("Arial", 26, "bold"))
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

# === RESET BUTTON ===
def reset_plot():
    global time_data1, time_data2, TEMP1_data, TEMP2_data, start_time
    time_data1.clear()
    time_data2.clear()
    TEMP1_data.clear()
    TEMP2_data.clear()
    start_time = time.time()
    ax1.set_xlim(0, 10)
    ax2.set_xlim(0, 10)
    canvas.draw()

# === PLOT SETUP ===
fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(8.5, 3), sharex=True)
fig.tight_layout(pad=3.0)  # smaller padding between plots

# ext temp plot
# temp1_line, = ax1.plot([], [], label="External Temperature", color='green')
# ax1.set_title("External Temperature")
# ax1.set_xlabel("Time (s)")
# ax1.set_ylabel("Temperature (ºC)")
# ax1.set_ylim(0, 1)
# ax1.legend()

# pressure temp plot
press_line, = ax1.plot([], [], label="Hull Pressure", color='green')
ax1.set_title("Hull Pressure")
ax1.set_xlabel("Time (s)")
ax1.set_ylabel("Pressure (hPa)")
ax1.set_ylim(0, 1)
ax1.legend()

# int temp plot
temp2_line, = ax2.plot([], [], label="Internal Temperature", color='blue')
ax2.set_title("Internal Temperature")
ax2.set_xlabel("Time (s)")
ax2.set_ylabel("Temperature (ºC)")
ax2.set_ylim(0, 1)
ax2.legend()

# canvas = FigureCanvasTkAgg(fig, master=root)
# canvas.get_tk_widget().grid(row=7, column=0, columnspan=2)

canvas = FigureCanvasTkAgg(fig, master=root)
canvas.get_tk_widget().pack(side="top", fill="both", expand=True)

# reset_btn = tk.Button(root, text="Reset Graphs", command=reset_plot, bg="white", fg="red", font=("Helvetica", 20, "bold"))
# reset_btn.grid(row=6, column=0)

reset_btn = tk.Button(root, text="Reset Graphs", command=reset_plot, bg="white", fg="red", font=("Helvetica", 20, "bold"))
reset_btn.pack(pady=10)

# Bind the close event
root.protocol("WM_DELETE_WINDOW", on_close)

root.after(1000, update_sensors_and_plot)

# Run the GUI
root.mainloop()