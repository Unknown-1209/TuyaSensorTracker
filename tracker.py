import LCD1602
import RPi.GPIO as GPIO
import time
import TuyaSensor
from datetime import datetime
import os

FILENAME = "DoorTrafficData.csv"

# Initialize global variables
door_count = 0
time_between_opens = 0.0
last_event_time = 0.0
current_status = "closed"
door_open_duration = 0.0

# GPIO Setup
GPIO.setmode(GPIO.BCM)

def initialize_system():
    """Initialize all components and load previous state"""
    global door_count, time_between_opens, last_event_time, current_status, door_open_duration

    # Initialize LCD
    LCD1602.init_lcd()

    # Initialize data file if needed
    if not os.path.exists(FILENAME):
        with open(FILENAME, 'w') as f:
            f.write("timestamp,event_type,count,time_between_opens,door_open_duration\n")
        door_count, time_between_opens, last_event_time, current_status, door_open_duration = 0, 0, time.time(), "closed", 0.0
        return

    # Load last state from file
    with open(FILENAME, 'r') as f:
        lines = f.readlines()
        if len(lines) <= 1:  # Only header exists
            door_count, time_between_opens, last_event_time, current_status, door_open_duration = 0, 0, time.time(), "closed", 0.0
            return

        # Find last closed event (most recent complete cycle)
        for line in reversed(lines[1:]):  # Skip header
            parts = line.strip().split(',')
            if parts[1] == "closed":
                door_count = int(parts[2])
                time_between_opens = float(parts[3])
                door_open_duration = float(parts[4])
                last_event_time = datetime.strptime(parts[0], "%Y-%m-%d %H:%M:%S").timestamp()
                current_status = "closed"
                return

        # If no closed events found, return defaults
        door_count, time_between_opens, last_event_time, current_status, door_open_duration = 0, 0, time.time(), "closed", 0.0
        print(f"Resumed with count: {door_count}, last event: {datetime.fromtimestamp(last_event_time)}")

def log_event(event_type):
    """Log door event with all relevant data"""
    global door_count, time_between_opens, last_event_time, current_status, door_open_duration

    timestamp = datetime.now().strftime("%Y-%m-%d %H:%M:%S")
    with open(FILENAME, 'a') as f:
        f.write(f"{timestamp},{event_type},{door_count},{time_between_opens:.2f},{door_open_duration:.2f}\n")
    print(f"[LOG] {timestamp} | {event_type.upper()} | Count: {door_count} | Between: {time_between_opens:.2f}s | Duration: {door_open_duration:.2f}s")

def update_display():
    """Update LCD with current metrics (optimized for 5-char time display)"""

    global door_count, time_between_opens, door_open_duration

    # Format time between opens for LCD display
    seconds = int(time_between_opens)

    # Determine the best format based on time duration
    if seconds < 60:  # Less than 1 minute
        time_str = f"{seconds}s"
    elif seconds < 3600:  # Less than 1 hour
        mins = seconds // 60
        secs = seconds % 60
        time_str = f"{mins}m {secs}s" if (mins < 10) else f"{mins}m"
    elif seconds < 86400:  # Less than 1 day
        hours = seconds // 3600
        mins = (seconds % 3600) // 60
        time_str = f"{hours}h {mins}m" if (hours < 10) else f"{hours}h"
    else:  # 1 day or more
        days = seconds // 86400
        hours = (seconds % 86400) // 3600
        time_str = f"{days}d {hours}h" if (days < 10) else f"{days}d"

    # Ensure we don't exceed 8 characters
    time_str = time_str[:8]

    line1 = f"Visitors: {door_count}"
    line2 = f"Intrvl: {time_str}"

    LCD1602.write_lcd(line1, line2)
    print(f"[DISPLAY] {line1} / {line2}")

def monitor():
    global door_count, time_between_opens, last_event_time, current_status, door_open_duration

    # Initialize system
    print("Initializing system...", end="")
    initialize_system()
    print("System initialized.\nStarting monitoring...")

    update_display()
    print(f"Resumed with count: {door_count}, last event: {datetime.fromtimestamp(last_event_time)}")

    try:
        while True:
            door_state = TuyaSensor.get_door_state()
            now = time.time()

            # Door just opened
            if door_state and current_status == "closed":
                current_status = "open"
                time_between_opens = now - last_event_time
                last_event_time = now
                door_open_duration = 0.0  # Reset duration at opening
                log_event("opened")
                update_display()

            # Door just closed
            elif not door_state and current_status == "open":
                current_status = "closed"
                door_open_duration = now - last_event_time
                door_count += 1
                last_event_time = now
                log_event("closed")

            time.sleep(0.2)

    except KeyboardInterrupt:
        LCD1602.send_command(0x01)
        print("\n[EXIT] Monitoring stopped")

if __name__ == "__main__":
    monitor()
