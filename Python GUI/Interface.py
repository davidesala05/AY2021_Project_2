#IMPORT THE REQUIRED LIBRARIES
import csv
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import itertools
import PySimpleGUI as sg
import matplotlib
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg
import ctypes
import serial
import csv
import struct
import os

#Get the current working directory
path = os.getcwd()
print("Current working directory: %s" %path)

#Create a new directory to store the saved figures
try:
    os.mkdir(path + "/Event Images")
except OSError:
    print ("Creation of the directory failed")
else:
    print ("Successfully created the directory")
#Create a new directory to store the CSV file
try:
    os.mkdir(path + "/CSV file")
except OSError:
    print ("Creation of the directory failed")
else:
    print ("Successfully created the directory")

#Maximize the resolution of the PySimpleGUI
ctypes.windll.shcore.SetProcessDpiAwareness(True)
#Link matplotlib and PySimpleGUI
matplotlib.use('TkAgg')
# Initialization of the index through which change switch between events
index = -1
# Definition of the window menu
menu = [
    ['File', ['Export CSV file','Update', 'Open', 'Save', 'Exit']],
]   

# Define the window layout
layout = [
    [sg.Text("Overthreshold events plot\n", font=('Helvetica', 20,'bold'))],
    [sg.Canvas(key="-CANVAS-")],
    [sg.Text('', size=(50,8), font=('Helvetica', 15), key='Text1', justification='left', text_color='white')],
    [sg.Button("Back"), sg.Button("Next")],
    [sg.Menu(menu, font="Helvetica 9")],
]

# Create the form and show it without the plot
window = sg.Window(
    "Matplotlib graphs",
    layout,
    location=(10, 10),
    finalize=True,
    element_justification="center",
    font="Helvetica 18",
)
    

'''
The followind function is used to plot in the windows the event's
accelerations and a text box with all the info about the event
(timestamp, datarate, FS Range and the events number)
'''
def plot_event(i):
    #Close the previous plot
    plt.close()
    #Create a new Figure with high resolution
    fig = matplotlib.pyplot.figure(dpi=300)
    #Create the widget with the plot
    canvas = FigureCanvasTkAgg(fig, window['-CANVAS-'].Widget)
    plot_widget = canvas.get_tk_widget()
    plot_widget.grid(row=0, column=0)
    #Create the X axis according to the current datarate
    t = np.arange(0, 31/df['Datarate'].iloc[i*32], 1/df['Datarate'].iloc[i*32])

    #PLOT THE ACCELERATION and customize the plot
    plt.title("EVENT: %i" %(i + 1))
    plt.plot(t, df["X"].iloc[i*32:i*32+31], color = 'r', label = 'X axis')
    plt.plot(t, df["Y"].iloc[i*32:i*32+31], color = 'b', label = 'Y axis')
    plt.plot(t, df["Z"].iloc[i*32:i*32+31], color = 'g', label = 'Z axis')
    plt.legend()
    plt.xlabel('Time[s]')
    plt.ylabel('Acceleration[m/s^2]')
    plt.xlim((-0.1,1.3))
    plt.ylim((-100,100))
    plt.margins(0.1)

    #Create the string with the event info to be displayed under the plot
    textstr = '\n\n'.join((
        "\n\u25CF Full-Scale Range = \u00B1 %s g" %df['FS range'].iloc[i*32], #da cambiare in FS
        "\u25CF Data rate = %s Hz" %df['Datarate'].iloc[i*32],
        "\u25CF Timestamp = %s:%s:%s" % (df['Hour'].iloc[i*32], df['Minute'].iloc[i*32], df['Second'].iloc[i*32]),
        "\u25CF Total events = %s" %count_overth_event
    ))
    #Create the text box with the string previously created
    window['Text1'].Update(textstr)
    #Draw the plot into the window
    fig.canvas.draw()
    return

'''
The following function is used to update the
current dataframe with the new CSV file
'''
def Update_DataFrame():
    # Importing the CSV file and transforming it into a dataframe through Pandas library
    df = pd.read_csv('CSV file/Events.csv', header = None, names = ['Index','Event', 'FS range', 'Datarate', 'X', 'Y', 'Z', 'Hour', 'Minute', 'Second'])

    # Definition of the number of overthreshold events contained inside the file
    count_overth_event = df["Event"].iloc[-1]
    return df, count_overth_event

'''
The following function is used to create and save the CSV file
containing all the information about the events that have been generated
It requires a comunication with the microcontroller throught the serial port
'''
def Export_CSV_file():

    #Open the CSV file if it exists
    f = open("CSV file/Events.csv", "w")
    #If an old file exists, it is initialize to be rewritten with the new data
    f.truncate()
    #Close the file
    f.close()
    #Open the communication with the serial port
    s = serial.Serial('COM3', baudrate=57600, timeout=1)
    #if the communication is opened
    if s.is_open:
        print("Connected to COM3")

    s.flush()
    #Write the "W" character, this let the microcontroller start to send the data of the events
    s.write('W'.encode('utf-8'))
    #Initialization of some variables  
    count_read = 0
    len_waveform = 32
    start = 0
    #Fisrt data is the number of the events that have been generated
    if count_read == 0:
        count_overth_event = s.read(1)
        count_overth_event = struct.unpack('B',count_overth_event)[0]
        print("number of events",count_overth_event)
        count_read += 1
    
    #Cycle through the events
    for y in range(count_overth_event):
        #Firstly the information of the current event's parameters is readen
        parameters = s.read(5)
        parameters = struct.unpack('5B', parameters)
        sensitivity = parameters[0]
        datarate = parameters[1]
        #Conversion between sensitivity and FS range (the latter is more meaningful)
        if sensitivity == 1:
            FS_range = 2
        if sensitivity == 2:
            FS_range = 4
        if sensitivity == 4:
            FS_range = 8
        if sensitivity == 12:
            FS_range = 16
        #Conversion from the datarate reguster to the datarate value
        if datarate == 4:
            datarate = 25
        elif datarate == 5:
            datarate = 50
        elif datarate == 6:
            datarate = 100
        
        #TIMESTAMP is saved
        hour = parameters[2]
        minute = parameters[3]
        second = parameters[4]

        print("parameters",parameters)

        start = 0

        for i in range(len_waveform):
            #wait that the string with the acceleration data starts with the correct HEADER
            while start != 0xA0:
                start = s.read(1)
                if len(start) > 0:
                    start = struct.unpack('B', start)[0]
                #If all the events' data are sent
                if start == 0xE0:
                    print("End")
                    break
            #X axis
            accX = s.read(4)
            accX = struct.unpack('>f', accX)[0]
            #Y axis
            accY = s.read(4)
            accY = struct.unpack('>f', accY)[0]
            #Z axis
            accZ = s.read(4)
            accZ = struct.unpack('>f', accZ)[0]
            #Read the TAIL of the string
            end = s.read(1)
            end = struct.unpack('B', end)[0]

            #If the TAIL is the correct one, communication has not been corrupted
            if end == 0xC0:
                print("values", accX,accY,accZ)
                #write the row with all the data and parameters in the CSV file
                with open("CSV file/Events.csv","a") as f:
                    writer = csv.writer(f,delimiter=",")
                    writer.writerow([i, y+1, FS_range, datarate, accX, accY, accZ, hour, minute, second])
                    #if the 32 values of acceleration have not been read yet
                    if i != 31:
                        start = s.read(1)
                        start = struct.unpack('B', start)[0]
            #If the communication has been corrupted
            else:
                print("Invalid data.")
    #close the file
    f.close()
    return
  

#Create the CSV file and the DataFrame
Export_CSV_file()
df, count_overth_event = Update_DataFrame()

# Infinite loop
while (True):
    #Read the window       
    event, values = window.read()
    if event == 'Next':
        #Change the next event if it is not the last
        if index != count_overth_event-1:
            index = index +1
        #Plot the event
        plot_event(index)
        
    
    if event == 'Back':
        #Change the previous event if it is not the first
        if index != 0:
            index = index -1
        #Plot the event
        plot_event(index)

    if event == 'Exit':
        #Close the window
        window.close()
        break

    if event == 'Save':
        #Save the current figure in a PNG file
        plt.savefig(f"Event Images/Event_{index+1}.png")

    if event == 'Open':
        #Open the plot in a matplotlib figure
        plt.show()

    if event == 'Update':
        #Update the dataframe with the new CSV file
        df, count_overth_event = Update_DataFrame()

    if event == 'Export CSV file':
        #Create and save the new CSV file
        Export_CSV_file()

    if event == sg.WIN_CLOSED:
        window.close()
        break

    
