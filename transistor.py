import time
import serial

from flask import Flask

app = Flask(__name__)
from waitress import serve

# Replace 'COM3' with your Arduino's actual port
arduino_port = 'COM5' 

def testLoop():
    try:
        ser = serial.Serial(arduino_port, 9600, timeout=1)
        time.sleep(2) # Wait for Arduino to reset
        while True:
            # 1. Turn the transistor ON
            print("Setting pin to HIGH...")
            ser.write(b'1')
            time.sleep(0.5)

            # 2. Ask the Arduino to read the pin state
            print("Requesting pin state...")
            ser.write(b'R') # Send the 'Read' command
            
            # 3. Read the response from the Arduino
            raw_response = ser.readline()                  # Read incoming line of data
            pin_state = raw_response.decode('utf-8').strip() # Clean up the bytes into text
            
            print(f"The current state of pin D2 is: {pin_state}") 
            # Will print '1' if the pin is high, or '0' if it is low
            
            time.sleep(3.0)
            
            # 1. Turn the transistor OFF
            print("Setting pin to LOW...")
            ser.write(b'0')
            time.sleep(0.5)

            # 2. Ask the Arduino to read the pin state
            print("Requesting pin state...")
            ser.write(b'R') # Send the 'Read' command
            
            # 3. Read the response from the Arduino
            raw_response = ser.readline()                  # Read incoming line of data
            pin_state = raw_response.decode('utf-8').strip() # Clean up the bytes into text
            
            print(f"The current state of pin D2 is: {pin_state}") 
            
            time.sleep(3.0)
        ser.close()

    except serial.SerialException as e:
        print(f"Could not open serial port: {e}")
def push_button():
    res=['Activating...']
    try:
        res.append("Initializing arduino...")
        ser = serial.Serial(arduino_port, 9600, timeout=1)
        time.sleep(2) # Wait for Arduino to reset
        res.append("Pushing button...")
        # 1. Turn the transistor ON
        print("Setting pin to HIGH...")
        ser.write(b'1')
        time.sleep(0.5)  
        #
        # 2. Ask the Arduino to read the pin state
        print("Requesting pin state...")
        ser.write(b'R') # Send the 'Read' command
        #
        # 3. Read the response from the Arduino
        raw_response = ser.readline()                  # Read incoming line of data
        pin_state = raw_response.decode('utf-8').strip() # Clean up the bytes into text
        #
        print(f"The current state of pin D2 is: {pin_state}") 
        # Will print '1' if the pin is high, or '0' if it is low
        # 1. Turn the transistor ON
        print("Setting pin to LOW...")
        ser.write(b'0')
        time.sleep(0.5)
        #
        # 2. Ask the Arduino to read the pin state
        print("Requesting pin state...")
        ser.write(b'R') # Send the 'Read' command
        #
        # 3. Read the response from the Arduino
        raw_response = ser.readline()                  # Read incoming line of data
        pin_state = raw_response.decode('utf-8').strip() # Clean up the bytes into text
        #
        print(f"The current state of pin D2 is: {pin_state}") 
        # Will print '1' if the pin is high, or '0' if it is low
        res.append("Successfully pushed the button!")        
    except serial.SerialException as e:
        res.append(f"Could not open serial port: {e}")
        print(res)
    return str(res)
def main():
  @app.route("/")
  def hello():
        return "Hello, World!"
  @app.route("/activate")
  def activate():
        return push_button()
    #app.run()
  serve(app, listen='*:8080')

if __name__ == "__main__":
  main()