from machine import Pin
import bluetooth
from ble_simple_peripheral import BLESimplePeripheral
import time  # Ensure time is imported

# Initialize Bluetooth Low Energy (BLE)
ble = bluetooth.BLE()
sp = BLESimplePeripheral(ble)

# Initialize the relay pin
relay_pour = Pin(0, Pin.OUT)
relay_pour.value(0)

relay_reveal = Pin(1, Pin.OUT)
relay_reveal.value(0)

def on_rx(data):
    # Decode and parse the incoming data
    data_str = data.decode('utf-8').strip()
    print(data_str)
    if data_str.startswith("pour"):
        key, value = data_str.split('=')
        pour_time = float(value)
        relay_pour.value(1)
        time.sleep(pour_time)
        relay_pour.value(0)
    elif data_str == "reveal_on":
        relay_reveal.value(1)
    elif data_str == "reveal_off":
        relay_reveal.value(0)
        

# Set the callback for received data
sp.on_write(on_rx)

while True:
    if sp.is_connected():
        pass  # Optionally, you can add code here to handle other tasks while connected

