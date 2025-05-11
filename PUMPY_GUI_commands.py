# PUMPY GUI FOR COMMANDS

import tkinter as tk

import serial

import matplotlib
matplotlib.use('TkAgg')

# CHECK CORRECT PORT EACH TIME
ser = serial.Serial('/dev/cu.usbserial-110', 115200, timeout=1)

def send_command(command):
    ser.write(command.encode() + b'\n')

def update_sensors_and_plot():
    while ser.in_waiting > 0:
        line = ser.readline().decode('utf-8').strip()

        print(f"Received: {line}")

        if "TASK COMPLETED" in line.upper():
            reset()

    root.after(1000, update_sensors_and_plot)

def on_close():
    root.destroy()
    root.quit()

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
root.title("2.014 BUOYANCY ENGINE COMMANDS DASHBOARD")
root.geometry("1000x500")
root.configure(bg="white")

# Title label
title_label = tk.Label(root, text="2.014 BUOYANCY ENGINE COMMANDS DASHBOARD", font=("Arial", 26, "bold"))
title_label.pack(pady=10)

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

# Bind the close event
root.protocol("WM_DELETE_WINDOW", on_close)

root.after(1000, update_sensors_and_plot)

# Run the GUI
root.mainloop()