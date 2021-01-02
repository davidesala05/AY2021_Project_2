# **ELECTRONIC TECHNOLOGIES AND BIOSENSORS LABORATORY - AA 2020/2021**
# Project 2
### _Development of a system that controls the blinking frequency and the colour intensity of an RGB LED based on accelerometer data and is able to detect and save overthreshold events and their time stamp. The system is configured through an Hardware Menu implemented with the OnBoard Push button and with a potentiometer that allows to tune correctly the parameters. Eventually, overthreshold events and all the associated parameters can be displayed through an user-friendly Pyhton GUI._ 
#
## Authors: Daniela Garofalo, Benedetta Pedica, Davide Sala
### Date: 10/01/2021
#
### CY8CKIT-059 Development Kit by Cypress    
### Integrated Design Environment: PSoC Creator 4.4
#
### HARDWARE COMPONENTS and PINs SETUP:
                                    
- **LIS3DH Accelerometer**
  * SCL: 12.0
  * SDA: 12.1
  * INT: 12.4
- **24LC512 EEPROM external memory**
  * SCL: 12.0
  * SDA: 12.1
  * A0: GND
  * A1: GND
  * A2: GND
- **LED RGB**
  * RED: 0.0
  * GREEN: 0.1
  * BLUE:0.2
- **POTENTIOMETER**
  * Vout: 3.3
- **PUSH BUTTON**
  * 2.2
- **LED OnBoard**
  * 2.1
- **HC-05 BLUETOOTH MODULE**
  * RXD: 12.7
  * TXD: 12.6
### MAIN FUNCTIONALITIES AND OBJECTIVES:
- Acquisition of acceleration data using the FIFO register
- Processing the raw information to control the blinking frequency of the LED
- Possibility to use a user interface in order to configure some options of the device
- Identification and storage of the overthreshold events​
- Implementation of a hardware menu based on the potentiometer values​
- Communication and representation of the information to the user
### CODE ORGANIZATION: 
- **Global**  --> Declaration and initialization of all the variables and the masks used for registers
- **Functions_EVENTS**  --> Function used to manage overthreshold events: save them in EEPROM, read them, export them as CSV for the GUI
- **Functions_SETTINGS** --> Function used to define and change various settings of the device
- **ErrorCodes** --> Error function used mainly for debugging
- **HardwareMenu** --> Implementation of all the function used in the user menu
- **I2C_Interface_ACC** --> Functions used to interface the Accelerometer with the I2C Peripheral
- **I2C_Interface_EXTERNAL_EEPROM** --> Functions used to interface the External EEPROM with the I2C Peripheral
- **InterruptRoutines** --> Functions used to manage all the interrupt used (Timer,Button,UART,Accelerometer)
### FUNCTIONING OF THE SYSTEM:

<p align="center">
<img width="250" alt="Schermata 2021-01-02 alle 12 10 53" src="https://user-images.githubusercontent.com/71715516/103456140-b4dbce00-4cf3-11eb-8f04-b2d8dadf5bb0.png">
</p>

After the device is succesfully initialized and programmed, the user through a double push of the OnBoard Push button each one for equal or less than 0.25s
can switch between RUN state during which the device is running and accelerometer is sampling and WAIT state during which the device is basically stopped.

<p align="center">
<img width="373" alt="Schermata 2021-01-02 alle 12 11 18" src="https://user-images.githubusercontent.com/71715516/103456144-c624da80-4cf3-11eb-9b18-a586f0533925.png">
</p>

If the user push the OnBoard Push button for equal or more than 2 seconds, this is identified as a "long pression" and the device enters in Configuration Mode: in this case, the user with a single click can sequentially switch between three parameters: **Full Scale Range, Data Rate and Verbose flag**. The first one refers to the range of measurement of the Accelerometer **(± 2,4,8,16 g)**, the second one to the Output DataRate **(25,50,100 Hz)** and the latter to a flag used to send raw acceleration data via UART to the Bridge Control Panel. These parameters can be opportunely tuned through the potentiometer and each of them has a visual feedback with one of the RGB channels: Red for Full Scale Range, Green for DataRate and Blue for Verbose flag. Also the frequency of the blinking changes according to the value sampled from the potentiometer. 

<p align="center">
<img width="506" alt="Schermata 2021-01-02 alle 12 13 41" src="https://user-images.githubusercontent.com/71715516/103456172-f8363c80-4cf3-11eb-8cd6-2cbd79b09dd6.png">
</p>

If the device is in WAIT state, the user can communicate via UART with the device for various purposes and this is done by opening CoolTerm and choosing amongst these options:
- writing **'b' or 'B'** --> start plotting waveforms of overthreshold events in the Bridge Control Panel 
- writing **'s' or 'S'** --> stop plotting data
- writing **'t' or 'T'** --> print timestamp information (hour,minute,second)
- writing **'w' or 'W'** --> export waveform, settings and timestamp in order to create the CSV file
### DEVELOPMENT OF THE PROJECT: 

First of all, we started working with the accelerometer LIS3DH: values in the three axes are sampled and are able to trigger an interrupt whenever the FIFO collects a new set of data. This is done inside the for loop where, if the device is in RUN state and no overthreshold event is detected, acceleration data from the three axes (casted as float 32) are detected and used to tune the blinking frequency of the LED with the function Set_RGB. It's important to underline that in the conversion we took account of the fact that sensitivity is not constant because it changes with the Full Scale Range that is one of the parameters that can be set through the custom Menu we created.

Then we started managing the overthreshold event detection: first of all, we configured the Threshold and the Duration register (respectively LIS3DH_INT2_THS and LIS3DH_INT2_DURATION) so that the value inside the register is changed according to the Full scale range and to the Data Rate in order to keep threshold and duration of the event costant, respectively at ±2g and 0.2s. 
Then we implemented the functions that allow to write the waveform and the timestamp of the event in the External EEPROM: being 192 byte the dimension of our data and 128 Byte the maximal dimension allowed in a single write operation, we used an alternate writing technique so that for the ODD events we write 128+64 Byte while for the EVEN events we write 64+128 Byte ensuring that data are not overwritten.
We decided to use memory adresses from 0 to 60K to save the waveform of the events, from 60K to 63K to save the time stamp in hour,minute,second,from 63K to 63.5K to save the sensitivity and from 63.5K to the end to save the Datarate. After an overthreshold event has occurred, the Stream_to_FIFO mode must be reset, and this is done through the Register_Initialization_after_Overth_Event Function.
If the device is in WAIT state, there is the possibility to read the events from the external EEPROM and plot them: first data and the relative parameters (Data rate and Full scale range) are read, then the relative timestamp, and then with the function Export_File_CSV the information sent to the serial port are saved and exported as a CSV file that can be easily interpreted by a Phyton code. In particular, we used Matplotlib library in order to plot each waveform with an associated table specifying the Datarate, the Full scale range and the time stamp.

For what regards the Hardware Menu implemented with the OnBoard push button, we used a switch case in order to manage the different states in which the user can find himself: 
- **IDLE**: if there is a double click the device is switched ON/OFF (and correspondingly the Blue Led OnBoard); if there is a long pression, we go to the entry state; 
- **CM_ENTRY**: we switch the channel of the MUX to 100Hz to guarantee a correct blinking of the RGB, PWMs are reset and ADC is started, then we directly go to configuration mode;
- **CM_SETPARAMETERS**: we sample the value of the potentiometer and set the Feedback, through a single click we switch sequentially between Full Scale Range, Data rate and Verbose Flag to send data with the UART, while through a long pression we end up in exit state;
- **CM_EXIT**: we switch back the channel of MUX to 4MHz, we stop the ADC and we convert parameters chosen in value for the register, and we also save the parameters set in the External EEPROM. Then we go back to IDLE state.
### EXAMPLE OF OVERTHRESHOLD EVENT:

<p align="center">
<img width="1623" alt="Schermata 2020-12-19 alle 10 08 57" src="https://user-images.githubusercontent.com/71715516/103455745-cae78f80-4cef-11eb-93aa-4129908e3ed6.png">
</p>

This is an example of the output of the Bridge Control Panel when we have three different overthreshold events detected generated with almost the same movement: we set the threshold to ±2g and the minimum duration for an event do be detected to 0.2 seconds. These three waveforms were detected with different Data Rates and different Full Scale Ranges and at different time instants. A big limitation of the BCP is that it just plots data and so is not possible to associate the waveform with all the corresponding parameters.
### PHYTON GUI:

It's a specific user-friendly GUI implemented with Python thanks to which it's possible to plot separately each overthreshold event's waveform with a table expliciting all the corresponding parameters: Full Scale Range, Data Rate, Time stamp, number of events generated. 
It's also possible to save these plots as images (.png).

<p align="center">
<img width="500" alt="Schermata 2020-12-31 alle 16 57 33" src="https://user-images.githubusercontent.com/71715516/103455771-2285fb00-4cf0-11eb-94c9-afc6afd7a5c9.png">
</p>

<p float="left">
<p align="center">
<img width="470" alt="Schermata 2021-01-02 alle 12 32 38" src="https://user-images.githubusercontent.com/71715516/103456441-b35fd500-4cf6-11eb-8b6b-a1fca2fbe235.png">
<img width="470" alt="Schermata 2021-01-02 alle 12 32 56" src="https://user-images.githubusercontent.com/71715516/103456442-b5299880-4cf6-11eb-9977-93730a137bf8.png">

</p>
