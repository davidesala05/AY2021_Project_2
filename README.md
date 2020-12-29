# **ELECTRONIC TECHNOLOGIES AND BIOSENSORS LABORATORY - AA 2020/2021**
#
## **Project 2**: _Development of a system that controls the blinking frequency and the colour intensity of an RGB LED based on accelerometer data._
#
## Authors: Daniela Garofalo, Benedetta Pedica, Davide Sala
#
### Date: 10/01/2021
#
#
### HARDWARE COMPONENTS:
- LIS3DH Accelerometer
- 24LC512 EEPROM external memory
- LED RGB
- POTENTIOMETER
#
#
### MAIN FUNCTIONALITIES:
- Acquisition of acceleration data using the FIFO register
- Processing the raw information to control the blinking frequency of the LED
- Possibility to use a user interface in order to configure some options of the device
#
#
### OBJECTIVES: 
- Identification and storage of the overthreshold events​
- Implementation of a hardware menu based on the potentiometer values​
- Communication and representation of the information to the user
#
#
### DEVELOPMENT OF THE PROJECT: 

First of all, we started working with the accelerometer LIS3DH: values in the three axes are sampled and are able to trigger an interrupt whenever the FIFO collects a new set of data. This is done inside the for loop where, if the device is in RUN state and no overthreshold event is detected, acceleration data from the three axes (casted as float 32) are detected and used to tune the blinking frequency of the LED with the function Set_RGB. It's important to underline that in the conversion we took account of the fact that sensitivity is not constant because it changes with the Full Scale Range that is one of the parameters that can be set through the custom Menu we created.

Then we started managing the overthreshold event detection: first of all, we configured the Threshold and the Duration register (respectively LIS3DH_INT2_THS and LIS3DH_INT2_DURATION) so that the value inside the register is changed according to the Full scale range and to the Data Rate in order to keep threshold and duration of the event costant, respectively at ±2g and 0.2s. 

Then we implemented the functions that allow to write the waveform and the timestamp of the event in the External EEPROM: being 192 byte the dimension of our data and 128 Byte the maximal dimension allowed in a single write operation, we used an alternate writing technique so that for the ODD events we write 128+64 Byte while for the EVEN events we write 64+128 Byte ensuring that data are not overwritten.

We decided to use memory adresses from 0 to 60K to save the waveform of the events, from 60K to 63K to save the time stamp in hour,minute,second,from 63K to 63.5K to save the sensitivity and from 63.5K to the end to save the Datarate. After an overthreshold event has occurred, the Stream_to_FIFO mode must be reset, and this is done through the Register_Initialization_after_Overth_Event Function.

If the device is in WAIT state, there is the possibility to read the events from the external EEPROM and plot them: first data and the relative parameters (Data rate and Full scale range) are read, then the relative timestamp, and then with the function Export_File_CSV the information sent to the serial port are saved and exported as a CSV file that can be easily interpreted by a Phyton code. In particular, we used Matplotlib library in order to plot each waveform with an associated table specifying the Datarate, the Full scale range and the time stamp.

For what regards the Hardware Menu implemented with the OnBoard push button, we used a switch case in order to manage the different states in which the user can find himself:

- **IDLE**: if there is a double click the device is switched ON/OFF (and correspondingly the Blue Led OnBoard); if there is a long pression, we go to the entry state

- **CM_ENTRY**: we switch the channel of the MUX to 100Hz to guarantee a correct blinking of the RGB, PWMs are reset and ADC is started, then we directly go to configuration mode;

- **CM_SETPARAMETERS**: we sample the value of the potentiometer and set the Feedback, through a single click we switch sequentially between Full Scale Range, Data rate and Verbose Flag to send data with the UART, while through a long pression we end up in exit state;

- **CM_EXIT**: we switch back the channel of MUX to 4MHz, we stop the ADC and we convert parameters chosen in value for the register, and we also save the parameters set in the External EEPROM. Then we go back to IDLE state.

Eventually, we implemented an efficient communication via UART serial port and through Coolterm we can receive different characters:
- **b or B** --> plotting waveforms of overthreshold events;
- **s or S** --> stop plotting data;
- **t or T** --> send timestamp informations;
- **w or W** --> exporting waveform, settings and timestamp in order to create the CSV file;

We developed our project thinking as a possible concrete application a device for orientation feedback and detection of falls, with a possible application in the shipping field for example. 
