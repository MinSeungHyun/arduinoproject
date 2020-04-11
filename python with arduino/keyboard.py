from pynput import keyboard
import serial
print('serial ' + serial.__version__)

PORT = 'COM3'
BaudRate = 9600
ser = serial.Serial(PORT, BaudRate)

def on_press(key):
    try:
        key = (format(key.char))
        ser.write(key.encode('utf-8'))
        print(key)
    
    except AttributeError:
     key = (format(key))
     ser.write(key.encode('utf-8'))

def on_release(key):
    #print(format(key))
    if key == keyboard.Key.enter:
        # Stop listener
        return False

    
# Collect events until released
with keyboard.Listener(
        on_press=on_press,
        on_release=on_release) as listener:
    listener.join()