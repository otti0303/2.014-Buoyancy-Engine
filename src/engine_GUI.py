# if this script outputs a weird GUI thats basically a black screen
# copy the code into a new window of whatever software you're using and run it there
# seems like platform.ini doesn't like tkinter

import tkinter as tk
import random  # Simulating sensor data (replace with real sensor readings)
import time

def update_sensors():
    """Simulate sensor updates (replace with actual sensor read function)."""
    pressure.set(f"{random.uniform(900, 1100):.2f} hPa")  # Example: Atmospheric pressure in hPa
    temperature.set(f"{random.uniform(0, 40):.2f} °C")   # Example: Temperature in Celsius
    console.set("placeholder for esp32 readout")
    root.after(1000, update_sensors)  # Refresh every second

def update_timer():
    elapsed_time = int(time.time() - start_time)
    minutes, seconds = divmod(elapsed_time, 60)
    timer.set(f"{minutes}m {seconds}s")
    root.after(1000, update_timer)

def ascend():
    status_label.config(text="Status: Ascending", fg="blue")
    # ideally will update status to idle once action is finished

def descend():
    status_label.config(text="Status: Descending", fg="blue")
    # ideally will update status to idle once action is finished

def abort_ascent():
    status_label.config(text="Status: Ascent Aborted", fg="red")
    # ideally will update status to idle once action is finished

def abort_descent():
    status_label.config(text="Status: Descent Aborted", fg="red")
    # ideally will update status to idle once action is finished

def reset():
    status_label.config(text="Status: Idle", fg="green")

def power_on():
    status_label.config(text="Status: Power ON", fg="black")

def power_off():
    status_label.config(text="Status: Power OFF", fg="black")


# Create the main window
root = tk.Tk()
root.title("2.014 BUOYANCY ENGINE DASHBOARD")
root.geometry("1400x950")
root.configure(bg="white")

# Sensor values (simulated for now)
pressure = tk.StringVar()
temperature = tk.StringVar()
console = tk.StringVar()
timer = tk.StringVar()

# Timer variable
timer = tk.StringVar()
timer.set("0m 0s")  # Initialize timer with a default value

# Start time for timer
start_time = time.time()
update_timer()  # Start the timer loop

# Title label
title_label = tk.Label(root, text="2.014 BUOYANCY ENGINE DASHBOARD", font=("Arial", 30, "bold"))
title_label.pack(pady=10)

# Container for three frames (pressure sensor, temp sensor, time elapsed)
sensor_container = tk.Frame(root, bg="white")
sensor_container.pack(pady=20, padx=40)

# Pressure Frame
pressure_frame = tk.Frame(sensor_container, bg="lightgray", bd=2, relief="solid")
pressure_frame.pack(side="left", padx=20, ipadx=15, ipady=10)

pressure_label = tk.Label(pressure_frame, text="Pressure:", font=("Arial", 30), bg="lightgray")
pressure_label.grid(row=0, column=0, padx=10, pady=10)
pressure_value = tk.Label(pressure_frame, textvariable=pressure, font=("Arial", 30, "bold"), bg="lightgray")
pressure_value.grid(row=0, column=1, padx=10, pady=10)

# Temperature Frame
temp_frame = tk.Frame(sensor_container, bg="lightgray", bd=2, relief="solid")
temp_frame.pack(side="left", padx=20, ipadx=15, ipady=10)

temperature_label = tk.Label(temp_frame, text="Temperature:", font=("Arial", 30), bg="lightgray")
temperature_label.grid(row=0, column=0, padx=10, pady=10)
temperature_value = tk.Label(temp_frame, textvariable=temperature, font=("Arial", 30, "bold"), bg="lightgray")
temperature_value.grid(row=0, column=1, padx=10, pady=10)

# Timer frame
timer_frame = tk.Frame(sensor_container, bg="lightgray", bd=2, relief="solid")
timer_frame.pack(pady=20, padx=40, ipadx=15, ipady=10)

timer_label = tk.Label(timer_frame, text="Time Elapsed:", font=("Arial", 30), bg="lightgray")
timer_label.grid(row=0, column=0, padx=10, pady=10)
timer_value = tk.Label(timer_frame, textvariable=timer, font=("Arial", 30, "bold"), bg="lightgray")
timer_value.grid(row=0, column=1, padx=10, pady=10)

# Console Readout frame
console_frame = tk.Frame(root, bg="lightgray", bd=2, relief="solid")
console_frame.pack(pady=20, padx=40, ipadx=15, ipady=10)

console_label = tk.Label(console_frame, text="ESP32 Console Readout:", font=("Arial", 30), bg="lightgray")
console_label.grid(row=0, column=0, padx=10, pady=10)
console_value = tk.Label(console_frame, textvariable=console, font=("Arial", 30, "bold"), bg="lightgray")
console_value.grid(row=0, column=1, padx=10, pady=10)

# Buttons
button_frame = tk.Frame(root, bg="white")
button_frame.pack(pady=20)

tk.Button(button_frame, text="POWER ON", command=power_on, width=30, height=3, bg="white", fg="black", font=("Arial", 25, "bold")).grid(row=0, column=0, padx=20, pady=10)
tk.Button(button_frame, text="POWER OFF", command=power_off, width=30, height=3, bg="white", fg="black", font=("Arial", 25, "bold")).grid(row=0, column=1, padx=20, pady=10)
tk.Button(button_frame, text="ASCEND", command=ascend, width=30, height=3, bg="white", fg="blue", font=("Arial", 25, "bold")).grid(row=1, column=0, padx=20, pady=10)
tk.Button(button_frame, text="DESCEND", command=descend, width=30, height=3, bg="white", fg="blue", font=("Arial", 25, "bold")).grid(row=1, column=1, padx=20, pady=10)
tk.Button(button_frame, text="ABORT ASCENT", command=abort_ascent, width=30, height=3, bg="white", fg="darkred", font=("Arial", 25, "bold")).grid(row=2, column=0, padx=20, pady=10)
tk.Button(button_frame, text="ABORT DESCENT", command=abort_descent, width=30, height=3, bg="white", fg="darkred", font=("Arial", 25, "bold")).grid(row=2, column=1, padx=20, pady=10)
tk.Button(button_frame, text="RESET", command=reset, width=30, height=3, bg="white", fg="green", font=("Arial", 25, "bold")).grid(row=3, column=0, columnspan=2, padx=20, pady=10)
# Status label
status_label = tk.Label(root, text="Status: Power OFF", font=("Arial", 25, "bold"), bg="white")
status_label.pack(pady=20)

# Start sensor update loop
update_sensors()

# Run the GUI
root.mainloop()