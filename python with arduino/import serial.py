import serial

PORT = 'COM6'
BaudRate = 9600
arduino = serial.Serial(PORT, BaudRate)

print('serial ' + serial.__version__)

while 1:
    
    datafromUser = input()

    if datafromUser == '1':
        arduino.write(b'1')
        print("ONE")
        
    elif datafromUser == '0':   
        arduino.write(b'0')
        print("none")
