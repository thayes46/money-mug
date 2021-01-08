def sendToArduino(sendStr):
    serialConnection.write(sendStr.encode('utf-8'))


def recvFromArduino():
    global startMarker, endMarker
    
    ck = ""
    x = "z" # any value that is not an end- or startMarker
    byteCount = -1 # to allow for the fact that the last increment will be one too many
    
    # wait for the start character
    while  ord(x) != startMarker: 
        x = serialConnection.read()
    
    # save data until the end marker is found
    while ord(x) != endMarker:
        if ord(x) != startMarker:
            ck = ck + x.decode("utf-8") # change for Python3
            byteCount += 1
        x = serialConnection.read()
    
    return(ck)


def waitForArduino():
    # wait until the Arduino sends 'Arduino Ready' - allows time for Arduino reset
    
    global startMarker, endMarker
    
    msg = ""
    while msg.find("Arduino is ready") == -1:

        while serialConnection.inWaiting() == 0:
            pass
        
        msg = recvFromArduino()

        print(msg) 
        print()

def run(td):
    numLoops = len(td)
    waitingForReply = False

    n = 0
    while n < numLoops:
        teststr = td[n]

        if waitingForReply == False:
            sendToArduino(teststr)
            print("Sent from PC -- LOOP NUM " + str(n) + " STR Sent " + teststr)
            waitingForReply = True

        if waitingForReply == True:

            while serialConnection.inWaiting() == 0:
                pass
            
            dataRecvd = recvFromArduino()
            print("Reply Received  " + dataRecvd)
            n += 1
            waitingForReply = False

            print("===========")

        time.sleep(5)


import serial, time

print()
print()

# NOTE the user must ensure that the serial port and baudrate are correct
serPort = "/dev/ttyUSB1"
# serPort = input("List the absolute path to the port like /dev/ttyUSB0 or /dev/cu.usbserial-0001:\n")
baudRate = 115200
arduino = serial.Serial(serPort, baudRate)
print("Serial port " + serPort + " opened  Baudrate " + str(baudRate))
time.sleep(2)
arduino.write("x069")
while True:
    data = arduino.read(1)
    # if data:
    print data




# startMarker = 60
# endMarker = 62

# waitForArduino()


# testData = []
# concern = input("ConcernValue:\n")
# while (concern != 0) :
    # testdata will send a string followed by the RGB values or the concern level 
    # for the testData to be valid correct fields needs to be setup on the arduino code

    #  to send New RGB Colors
# testData.append("<update, 40>")
        #  to send ConcernValue
# testData.append("<update," + (str)(concern) + ">")
# run(testData)


arduino.close

