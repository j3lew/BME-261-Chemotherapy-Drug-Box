import serial
import time

# create variables
patient = False
morning = []
after = []
eve = []
bed = []
arr = []
x = 0

arduino = serial.Serial('/dev/cu.usbserial-1410', 9600, timeout=0) # connect to arduino --> /dev/cu.usbmodem14201
time.sleep(2)

path = '/Users/JinHwanCharlieLew/Desktop/bme261_pillbox/Drug_Regimen.txt' # open file
file = open(path,'r')

for line in file:   # read file and split into categories
    if "Name" in line:
        name = (line[6:])
        patient = True
    elif "Age" in line:
        age = (line[5:])
        patient = True
    elif "Treatment" in line:
        treatment = (line[11:])
        patient = True
    elif "Morn" in line:
        patient = True
        morning.append(line)
    elif "Aft" in line:
        patient = True
        after.append(line)
    elif "Eve" in line:
        patient = True
        eve.append(line)
    elif "Bed" in line:
        patient = True
        bed.append(line)

while True:
    if(arduino.in_waiting>0): # checking for the incoming byte
        if patient is True: # make sure that all the variables are set
            if True:    # make sure that arduino is connected
                word = arduino.readline().decode("utf-8").strip(' \n').strip('\r') # read what day the arduino wants 
                day = int(word) - 1 # set what index it is in each array 
                arduino.write(str.encode(name))
                arduino.write(str.encode(age))
                arduino.write(str.encode(treatment))
                arduino.write(str.encode(morning[day]))
                arduino.write(str.encode(after[day]))
                arduino.write(str.encode(eve[day]))
                arduino.write(str.encode(bed[day]))
                while(x<7): # store in an array
                    arr.append(arduino.readline().decode("utf-8").strip('\n').strip('\r'))
                    x=x+1
                x = 0
                while(x<len(arr)): # send the array to arduino
                    arduino.write(str.encode(arr[x]))
                    x = x+1
                # print(arduino.readline().decode("utf-8").strip('\n').strip('\r'))
    time.sleep(2) # after each loop, allow next serial communication from arduino to python


