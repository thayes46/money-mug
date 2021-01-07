import serial


# ask for the arduino port
arduinoPort = input("Enter your board serial port (e.g. COM1 or /dev/cu.usbserial-0001)")

serialConnection = serial.Serial(arduinoPort, 9600)

while True:
    inputValue = "" # this needs to be an api call, change later when we have an actual working api
    serialConnection.write(inputValue.encode())