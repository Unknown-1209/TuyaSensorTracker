from tuya_connector import TuyaOpenAPI
# import time

ACCESS_ID = "your_access_id" # Can be found on the project's "Overview" section
ACCESS_KEY = "your_access_key" # Can be found on the project's "Overview" section
API_ENDPOINT = "https://openapi.tuyaus.com" # The region when you initially created the project
DEVICE_ID = "your_device_id" # Can be found on the project's "Devices" section


# Initialize Tuya OpenAPIa
print("Initializing API...", end="")
openapi = TuyaOpenAPI(API_ENDPOINT, ACCESS_ID, ACCESS_KEY)
openapi.connect()
print("Initialization is done!")

def get_door_state():
    try:
        response = openapi.get(
            f"/v1.0/iot-03/devices/{DEVICE_ID}/status"
        )

        # Parse through the response and only return the value of 'doorcontact_state'
        if response and response.get('success', False):
            for status in response['result']:
                if status['code'] == 'doorcontact_state':
                    return status['value']
        return None
    except Exception as e:
        print(f"Error getting door state: {e}")
        return None

def main():
    print("Tuya Door Monitor (Ctrl+C to exit)")
    print("---------------------------------")

    try:
        while True:
            state = get_door_state()
            if state is not None:
                status = "OPEN" if state else "CLOSED"
                timestamp = time.strftime("%Y-%m-%d %H:%M:%S")
                print(f"[{timestamp}] Door Status: {status}")

            time.sleep(1)  # Polling interval

    except KeyboardInterrupt:
        print("\nMonitoring stopped")

if __name__ == "__main__":
    main()
