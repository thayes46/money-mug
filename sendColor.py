
#=====================================

#  Function Definitions

#=====================================

def sendToArduino(sendStr):
    serialConnection.write(sendStr.encode('utf-8'))


#======================================

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


#============================

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
        
#======================================

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


#======================================

# The Program Starts Here

#======================================

import serial
import time

print()
print()

# NOTE the user must ensure that the serial port and baudrate are correct
# serPort = "/dev/ttyS80 or /dev/cu.usbserial-0001"
serPort = "/dev/cu.usbserial-0001"
baudRate = 9600
serialConnection = serial.Serial(serPort, baudRate)
print("Serial port " + serPort + " opened  Baudrate " + str(baudRate))


startMarker = 60
endMarker = 62


waitForArduino()


testData = []
testData.append("<update,200,0,0>")


run(testData)


serialConnection.close

