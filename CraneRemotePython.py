# Importing Libraries
import cv2 as cv
import serial
import numpy as np

def readInput():
    data = arduino.readline()
    arduino.reset_input_buffer()
    if len(data) > 15:
        mpu6050 = np.frombuffer(data, dtype = np.int16, count = 6, offset = 0)
        joystick = np.frombuffer(data, dtype = np.uint8, count = 3, offset = 12)
        return mpu6050, joystick
    else:
        return [-1],[-1]

def getHookPos(center, cap, template):
    ret, frame = cap.read()
    frame = cv.cvtColor(frame, cv.COLOR_BGR2GRAY)

    frame.astype(np.uint8)
    t_height, t_width = template.shape

    result = cv.matchTemplate(frame,template, cv.TM_CCOEFF)
    min_val, max_val, min_loc, max_loc = cv.minMaxLoc(result)

    location = np.zeros(2).astype(int)
    location[0] = max_loc[0] + round(t_width/2)
    location[1] = max_loc[1] + round(t_height/2)

    center = center + 0.002 * (location - center)
    
    int_center = center.astype(int)
    frame[int_center[1] - 2 : int_center[1] + 2 , :] = 255
    frame[: , int_center[0] - 2 : int_center[0] + 2] = 255
    frame[location[1] - 8 : location[1] + 8, location[0] - 8 : location[0] + 8] = 0
    cv.imshow('Match',frame)

    return center, location

arduino = serial.Serial(port='COM6', baudrate=115200, timeout=.1)

template = cv.imread(r"C:\Users\jonas\Documents\Python\LearnOpenCV\assets\Template.jpg",0)

cap = cv.VideoCapture(0)
print("Webcam geöffnet")
center = np.array([320,240])
integral = np.zeros(2)

modus = 0 # 0:Katze  1:Haken 
s1 = 1

Aktoren = bytearray([127,127,127]) #Katze Haken Drehkranz

while 1:
    mpu6050, joystick = readInput()
    
    center, HookPos = getHookPos(center, cap, template)

    # Proportionalregler
    deltax = HookPos[0] - center[0]
    deltay = - (HookPos[1] - center[1])

    Aktoren[0] = int(min(max(127 + deltax, 0), 255))
    Aktoren[2] = int(min(max(127 - deltay, 0), 255))
    for i in range(0,3):
        if abs(Aktoren[i] - 127) < 20:
            Aktoren[i] = 127

    tol = 10
    if joystick[0] != -1: #Überprüfen auf korrekte Übertragung
        if abs(joystick[1] - 127) > tol:
            Aktoren[2] = joystick[1] #Der Drehkranz wird direkt gesetzt

        if joystick[2] == 0 and s1 == 1:
            if modus == 0:
                modus = 1
            elif modus == 1:
                modus = 0
            s1 = 0
        elif joystick[2] == 1:
            s1 = 1

        if abs(joystick[0] - 127) > tol:
            Aktoren[modus] = joystick[0]
            if modus == 0:
                Aktoren[1] = 127
            elif modus == 1:
                Aktoren[0] = 127
            
    # print(Aktoren[0],Aktoren[1],Aktoren[2])
    print(joystick,Aktoren[0],Aktoren[1],Aktoren[2])

    arduino.write(Aktoren)

    if cv.waitKey(5) == ord('q'):
        break