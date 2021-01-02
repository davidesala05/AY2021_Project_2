/* ========================================
 *
 * Laboratorio di Tecnologie Elettroniche e Biosensori
 * Politecnico di Milano
 * AY 2020/2021 - I semester
 *
 * Final Projects:
 * - Project 2 - 
 * Authors: Daniela Garofalo, Benedetta Pedica, Davide Sala
 * Date: 10/01/2021
 *
 * ========================================
*/

// Include related to the header file related to the functions defined in this file
#include "Functions_EVENTS.h"

/*
Function used to write all the events information of the External EEPROM
The logic to write the waveforms (192 byte) is inverted according to the 
value of the current event:
    ODD event  --> 128+64 write operation
    EVEN event --> 64+128 write operation
    
In this way it is possible to put in place a specific space-saving strategy to store the
information inside the external EEPROM: it is necessary to take into consideration that a
multi-write operation can be done for a maximum of 128 bytes each time (that are considered
as a "page" inside the memory) --> according to this strategy, it is possible to write 2 
events inside 3 complete pages avoiding unused space in between.

All the operation are under a do-while cycle (so they are executed at least once), and this 
allows us to maximize the writing speed applying the ACK polling strategy to send write 
commands until no error (ACK) is received (meaning that the previous writing operation is
over).
*/

void Write_EVENT_on_EXTERNAL_EEPROM(void)
{
    
    /******************************************/
    /*                WAVEFORM                */
    /******************************************/
    ErrorCode error;
    
    /* This operation starts from the address 0 because the memory has been divided in sectors
    according to the type of data that will be stored --> the 192 bytes related to the same
    overthreshold event will be put sequentially inside the memory according to the number
    of the events detected by the device */
    
    /****** ODD LOGIC ******/
    if ((count_overth_event % 2) != 0){
        // Write the first 128 bytes on the external EEPROM through the I2C protocol
        do {
            error = I2C_Peripheral_EXTERNAL_EEPROM_WriteRegisterMulti(EEPROM_EXTERNAL_ADDRESS,
                                                                      EEPROM_EXTERNAL_START_POINT_WAVEFORM + ((count_overth_event-1)*N_REG_WAVEFORM),
                                                                      N_REG_1PAGE,
                                                                      waveform_8bit);
        } while(error == ERROR); /* Controlling the output of the writing in order to detect
        when the operation is done --> from the datasheet, there is a maximum time needed to
        finish a multiwrite operation but in this way it is possible to optimise the timing */

        // Write the remaining 64 bytes on the external EEPROM through the I2C protocol
        do {
            error = I2C_Peripheral_EXTERNAL_EEPROM_WriteRegisterMulti(EEPROM_EXTERNAL_ADDRESS,
                                                                      EEPROM_EXTERNAL_START_POINT_WAVEFORM + ((count_overth_event-1)*N_REG_WAVEFORM) + N_REG_1PAGE,
                                                                      N_REG_HALFPAGE,
                                                                      &waveform_8bit[N_REG_1PAGE]);
        } while(error == ERROR);

    } // Odd logic 
    
    /******* EVEN LOGIC ******/
    else {
        // Write the first 64 bytes on the external EEPROM through the I2C protocol
        do {
            error = I2C_Peripheral_EXTERNAL_EEPROM_WriteRegisterMulti(EEPROM_EXTERNAL_ADDRESS,
                                                                      EEPROM_EXTERNAL_START_POINT_WAVEFORM + ((count_overth_event-1)*N_REG_WAVEFORM),
                                                                      N_REG_HALFPAGE,
                                                                      waveform_8bit);
        } while(error == ERROR);

        // Write the remaining 128 bytes on the external EEPROM through the I2C protocol
        do {
            error = I2C_Peripheral_EXTERNAL_EEPROM_WriteRegisterMulti(EEPROM_EXTERNAL_ADDRESS,
                                                                      EEPROM_EXTERNAL_START_POINT_WAVEFORM + ((count_overth_event-1)*N_REG_WAVEFORM) + N_REG_HALFPAGE,
                                                                      N_REG_1PAGE,
                                                                      &waveform_8bit[N_REG_HALFPAGE]);
        } while(error == ERROR);

    } // Even logic
    
    
    /******************************************/
    /*                TIMESTAMP               */
    /******************************************/
    
    /* This operation starts from the address 60k because the memory has been divided in sectors
    according to the type of data that will be stored --> the 3 bytes related to the same
    overthreshold event will be put sequentially inside the memory according to the number
    of the events detected by the device */
    
    // Write the byte related to the hours
    do {
        error = I2C_Peripheral_EXTERNAL_EEPROM_WriteRegister(EEPROM_EXTERNAL_ADDRESS,
                                                             EEPROM_EXTERNAL_START_POINT_TIMESTAMP + 0 + ((count_overth_event-1)*N_REG_TIMESTAMP),
                                                             hours);
    } while(error == ERROR);
    
    // Write the byte related to the minutes 
    do {
        error = I2C_Peripheral_EXTERNAL_EEPROM_WriteRegister(EEPROM_EXTERNAL_ADDRESS,
                                                             EEPROM_EXTERNAL_START_POINT_TIMESTAMP + 1 + ((count_overth_event-1)*N_REG_TIMESTAMP),
                                                             minutes);
    } while(error == ERROR);
    
    // Write the byte related to the seconds
    do {
        error = I2C_Peripheral_EXTERNAL_EEPROM_WriteRegister(EEPROM_EXTERNAL_ADDRESS,
                                                             EEPROM_EXTERNAL_START_POINT_TIMESTAMP + 2 + ((count_overth_event-1)*N_REG_TIMESTAMP),
                                                             seconds);
    } while(error == ERROR);
    
    
    /******************************************/
    /*              SENSITIVITY               */
    /******************************************/
    
    /* This operation starts from the address 63k because the memory has been divided in sectors
    according to the type of data that will be stored --> the bytes will be put sequentially 
    inside the memory according to the number of the events detected by the device */
    
    // Write the byte related to the sensitivity
    do {
        error = I2C_Peripheral_EXTERNAL_EEPROM_WriteRegister(EEPROM_EXTERNAL_ADDRESS,
                                                             EEPROM_EXTERNAL_START_POINT_SENSITIVITY + (count_overth_event-1),
                                                             Sensitivity);
    } while(error == ERROR);

    /******************************************/
    /*                DATARATE                */
    /******************************************/
    
    /* This operation starts from the address 63.5k because the memory has been divided in sectors
    according to the type of data that will be stored --> the bytes will be put sequentially 
    inside the memory according to the number of the events detected by the device */
    
    do {
        error = I2C_Peripheral_EXTERNAL_EEPROM_WriteRegister(EEPROM_EXTERNAL_ADDRESS,
                                                             EEPROM_EXTERNAL_START_POINT_DATARATE + (count_overth_event-1),
                                                             DataRate_reg);
    } while(error == ERROR);
    
} // Write_EVENT_on_EXTERNAL_EEPROM()


/*
Function used to read the waveforms saved in the external EEPROM,
convert them in int32 and send consequently by the UART

This function can be called when the device is in WAIT modality and
the user send a 'B' or 'b' character --> this command will activate
the reading operation of the extenal EEPROM in order to obtain the
waveforms related to all the events detected during the normal
functioning of the device; this is done in order to allow the plotting
of the events through the Bridge Control Panel extension sendin the
information with the UART protocol
*/
void Read_Waveform_from_EXTERNAL_EEPROM(void)
{
    // Allocate the memory for the information referred to all the overthreshold events
    uint8_t all_waveforms[count_overth_event*N_REG_WAVEFORM];
    uint8_t all_sensitivity[count_overth_event];
    uint8_t all_datarate[count_overth_event];
    
    /* Buffer used to write a pause of all zeros between successive waveforms --> this
    void space is put between consecutive events in order to separate them making their
    visualisation more easy to be understood by the user (it has the same dimension of a 
    buffer containing the acceleration data on the 3 axes related to 1 sample with the
    resolution of 2 bytes = 6 bytes in total + header and tail) */
    uint8_t Pause[TRANSMIT_BUFFER_SIZE] = {0xA0,0,0,0,0,0,0,0,0,0,0,0,0,0xC0};
    
    // Waveform reading and saving the information inside the related variable
    ErrorCode error = I2C_Peripheral_EXTERNAL_EEPROM_ReadRegisterMulti(EEPROM_EXTERNAL_ADDRESS,
                                                                       EEPROM_EXTERNAL_START_POINT_WAVEFORM,
                                                                       N_REG_WAVEFORM * count_overth_event,
                                                                       all_waveforms);
    // Sensitivity reading and saving the information inside the related variable
    do {
        error = I2C_Peripheral_EXTERNAL_EEPROM_ReadRegisterMulti(EEPROM_EXTERNAL_ADDRESS,
                                                                 EEPROM_EXTERNAL_START_POINT_SENSITIVITY,
                                                                 count_overth_event,
                                                                 all_sensitivity);
    } while(error == ERROR);
    
    // Datarate reading and saving the information inside the related variable
    do {
        error = I2C_Peripheral_EXTERNAL_EEPROM_ReadRegisterMulti(EEPROM_EXTERNAL_ADDRESS,
                                                                 EEPROM_EXTERNAL_START_POINT_DATARATE,
                                                                 count_overth_event,
                                                                 all_datarate);
    } while(error == ERROR);
    
    // Cycle among all the overthreshold events to be stored
    for(uint8_t y = 0; y < count_overth_event; y++)
    {
        /* Sending the waveform with the same frequency to which they have been generated --> the
        real sampling frequency is half of the timer frequency of interrupt generation because
        the operation of reading the elements inside the FIFO register needs 2 clocks to be done
        
        Considering that the timer component is able to generate an interrupt with a frequency
        equal to 100 Hz, in order to obtain a submultiple of this frequency it is necessary to
        consider more than one interrupt generation */
        if (all_datarate[y] == MASK_DATARATE_50Hz){
            count_for_plotting = 4; // 100 Hz/4 = 25 Hz
        }
        else if (all_datarate[y] == MASK_DATARATE_100Hz){
            count_for_plotting = 2; // 100 Hz/2 = 50 Hz
        }
        else if (all_datarate[y] == MASK_DATARATE_200Hz){
            count_for_plotting = 1; // 100 Hz
        }
        
        /* Initialisation of the variable to the initial condition --> this counter will be
        compared with the count_for_plotting value in order to send the waveform with the
        proper frequency according to the datarate at which they were been acquired */
        count_waveform = 0;
        
        /* Cycle among the values associated to each waveform (192 bytes) --> they are analysed
        6-by-6 because one sample of the acceleration is composed by values with 2 bytes of
        resolution each for the 3 axes of the space */
        for(int16_t i = 0; i < N_REG_WAVEFORM - 5; i = i+6){
            
            /* Controlling that the transmission is happening with the same frequency with which
            the waveforms have been generated */
            if(count_waveform == count_for_plotting){
            
                /* Index defining the first value to be sent each time --> it identifies the 
                first byte of the accelerometer output that has to be send for each sample */
                uint32_t index = i + N_REG_WAVEFORM*y;
                
                /* Conversion of the data to int16 type --> this operation is necessary because
                the data have been saved as int16 variables */
                dataX = (int16)((all_waveforms[0+index] | (all_waveforms[1+index]<<8)))>>4;
                dataY = (int16)((all_waveforms[2+index] | (all_waveforms[3+index]<<8)))>>4;
                dataZ = (int16)((all_waveforms[4+index] | (all_waveforms[5+index]<<8)))>>4;
                
                // Conversion of data to m/s^2 (float32)
                accX = (float32)(dataX)*mg_TO_g*G*all_sensitivity[y];
                accY = (float32)(dataY)*mg_TO_g*G*all_sensitivity[y];
                accZ = (float32)(dataZ)*mg_TO_g*G*all_sensitivity[y];
                
                /* X-axis --> this initialisation is used to correct convert the float32 data
                of the acceleration in a format that is possible to be sent through the UART 
                communication protocol to the serial port */
                DataUnion.f = accX;
                
                /* The elements of the Buffer are assigned as the uint8 conversion of the float
                data of the acceleration --> it is important to notice that the bits will not be
                changed during this procedure, but they are interpret in a different way according
                to the variable type considered */
                Buffer[1] = (uint8_t)((DataUnion.l & 0xFF000000) >> 24);
                Buffer[2] = (uint8_t)((DataUnion.l & 0x00FF0000) >> 16);
                Buffer[3] = (uint8_t)((DataUnion.l & 0x0000FF00) >> 8);
                Buffer[4] = (uint8_t)((DataUnion.l & 0x000000FF) >> 0);
                
                /* Y-axis --> this initialisation is used to correct convert the float32 data
                of the acceleration in a format that is possible to be sent through the UART 
                communication protocol to the serial port */
                DataUnion.f = accY;
                
                /* The elements of the Buffer are assigned as the uint8 conversion of the float
                data of the acceleration --> it is important to notice that the bits will not be
                changed during this procedure, but they are interpret in a different way according
                to the variable type considered */
                Buffer[5] = (uint8_t)((DataUnion.l & 0xFF000000) >> 24);
                Buffer[6] = (uint8_t)((DataUnion.l & 0x00FF0000) >> 16);
                Buffer[7] = (uint8_t)((DataUnion.l & 0x0000FF00) >> 8);
                Buffer[8] = (uint8_t)((DataUnion.l & 0x000000FF) >> 0);
                
                /* Z-axis --> this initialisation is used to correct convert the float32 data
                of the acceleration in a format that is possible to be sent through the UART 
                communication protocol to the serial port */
                DataUnion.f = accZ;
                
                /* The elements of the Buffer are assigned as the uint8 conversion of the float
                data of the acceleration --> it is important to notice that the bits will not be
                changed during this procedure, but they are interpret in a different way according
                to the variable type considered */
                Buffer[9]  = (uint8_t)((DataUnion.l & 0xFF000000) >> 24);
                Buffer[10] = (uint8_t)((DataUnion.l & 0x00FF0000) >> 16);
                Buffer[11] = (uint8_t)((DataUnion.l & 0x0000FF00) >> 8);
                Buffer[12] = (uint8_t)((DataUnion.l & 0x000000FF) >> 0);
                
                // Transmission of the data 
                UART_PutArray(Buffer,TRANSMIT_BUFFER_SIZE);
                
                // Initialisation of the variable to the initial condition
                count_waveform = 0;
            }
            /* Return to the previous step (determined by the value of i) when the freqeuency of
            transmission is not the correct one --> if the number of interrupts generated does
            not correspond to the needed frequency, it is necessary to restore the value of the
            variable 'i' to the previous conditions */
            else
            {
                i = i-6;
            }
            
            // Controlling that the plot are not aborted through extenal commands ("s" by user)
            if(flag_send_waveform == 0){
                break;
            }
        }
        
        // Controlling that the plot are not aborted through extenal commands ("s" by user)
        if(flag_send_waveform == 0){
                break;
        }
        
        // Initialisation of the variable to the initial condition
        count_waveform = 0;
        
        /* Sending 100 values at 100 Hz in order to define the spatiature between 2 consecutive
        overthreshold events */
        for(int16_t i = 0; i < PAUSE_LENGHT; i++){
            
            /* Controlling the value of the counter according to the frequency with which the data
            will be sent through the serial port */
            if(count_waveform == 1)
            {
                // Transmission of the pause
                UART_PutArray(Pause,TRANSMIT_BUFFER_SIZE);
                
                // Initialisation of the variable to the initial condition
                count_waveform = 0;
            }
            else
            {
                /* Return to the previous step (determined by the value of i) when the freqeuency of
                transmission is not the correct one --> if the number of interrupts generated does
                not correspond to the needed frequency, it is necessary to restore the value of the
                variable 'i' to the previous conditions */
                i--;
            }
            
            // Controlling that the plot are not aborted through extenal commands ("s" by user)
            if(flag_send_waveform == 0){
                break;
            }
        }
        
        // Controlling that the plot are not aborted through extenal commands ("s" by user)
        if(flag_send_waveform == 0){
                break;
        }
    }
} // Read_Waveform_from_EXTERNAL_EEPROM()


/*
Function used to read the timestamps saved in the external eeprom
and send them through the UART

This function can be called when the device is in WAIT modality and
the user send a 'T' or 't' character --> this command will activate
the reading operation of the extenal EEPROM in order to obtain the
timestamrps related to all the events detected during the normal
functioning of the device; this is done in order to allow the user
to know when the events have been detected because the Bridge Control
Panel does not allow the representation of the timing information with
the waveforms plotting
*/
void Read_Timestamp_from_EXTERNAL_EEPROM(void)
{
    // Allocate the memory for the timestamps referred to all the overthreshold events
    uint8_t all_timestamp[count_overth_event*N_REG_TIMESTAMP];
    char string[100];
    
    // Timestamps reading and saving the information inside the related variable 
    ErrorCode error = I2C_Peripheral_EXTERNAL_EEPROM_ReadRegisterMulti(EEPROM_EXTERNAL_ADDRESS,
                                                                       EEPROM_EXTERNAL_START_POINT_TIMESTAMP,
                                                                       N_REG_TIMESTAMP * count_overth_event,
                                                                       all_timestamp);
    if(error == ERROR){
        UART_PutString("Error occurred during I2C comm\r\n");  
    }

    // // Cycle among all the overthreshold events
    for (uint8_t i = 0; i < count_overth_event; i++)
    {
        // Transmission of the information through the UART communication protocol
        sprintf(string,"\n# event --> %d  Timestamp --> Hour: %d Minute: %d Second: %d\n\n", i+1, all_timestamp[0+i*N_REG_TIMESTAMP], all_timestamp[1+i*N_REG_TIMESTAMP], all_timestamp[2+i*N_REG_TIMESTAMP]);
        UART_PutString(string);
    }
} // Read_Timestamp_from_EXTERNAL_EEPROM()


/*
This function is used to read, interpret and send all the info
about the events to the serial port: first of all, the device
transmits the total number of overthreshold events detected
during the normal functioning; secondly, if there is at least
one event stored in the external EEPROM, reading operations are
done to obtain all the information related to the events and
finally these data are sent through the serial port.
Then a Python file is used to re-interpret all the inforomation
and save them in a CSV file.

This function can be called when the device is in WAIT modality and
the user send a 'W' or 'w' character --> this command will activate
the reading operation of the extenal EEPROM in order to obtain the
all the data related to all the events detected during the normal
functioning of the device; this is done in order to allow the plotting
of the events with a Python-based GUI interface with the timing
information and other useful data */

void Export_file_CSV(void)
{
    // NO generation of an overthreshold event
    if(count_overth_event == 0){
    
        // Sending the number of the overthreshold events stored inside the external EEPROM
        Buffer_csv[0] = count_overth_event;
    
        // Transmission of the information
        UART_PutArray(Buffer_csv, 1);
    }
    // At least one overthreshold event has been generated
    else{
        // Allocate the memory for the information referred to all the overthreshold events
        uint8_t all_waveforms[count_overth_event*N_REG_WAVEFORM];
        uint8_t all_sensitivity[count_overth_event];
        uint8_t all_datarate[count_overth_event];
        uint8_t all_timestamp[count_overth_event*N_REG_TIMESTAMP];
        
        // Definition of the buffer dimension, header and tail for the communication
        Buffer[0] = 0xA0;
        Buffer[TRANSMIT_BUFFER_SIZE-1] = 0xC0;
        
        // Waveforms reading and saving the information inside the related variable 
        ErrorCode error = I2C_Peripheral_EXTERNAL_EEPROM_ReadRegisterMulti(EEPROM_EXTERNAL_ADDRESS,
                                                                           EEPROM_EXTERNAL_START_POINT_WAVEFORM,
                                                                           (N_REG_WAVEFORM * count_overth_event),
                                                                           all_waveforms);
        
        // Sensitivity reading and saving the information inside the related variable
        do {
            error = I2C_Peripheral_EXTERNAL_EEPROM_ReadRegisterMulti(EEPROM_EXTERNAL_ADDRESS,
                                                                     EEPROM_EXTERNAL_START_POINT_SENSITIVITY,
                                                                     count_overth_event,
                                                                     all_sensitivity);
        } while(error == ERROR);
        
        // Datarate reading and saving the information inside the related variable
        do {
            error = I2C_Peripheral_EXTERNAL_EEPROM_ReadRegisterMulti(EEPROM_EXTERNAL_ADDRESS,
                                                                     EEPROM_EXTERNAL_START_POINT_DATARATE,
                                                                     count_overth_event,
                                                                     all_datarate);
        } while(error == ERROR);
        
        // Timestamp reading and saving the information inside the related variable
        do {
        
            error = I2C_Peripheral_EXTERNAL_EEPROM_ReadRegisterMulti(EEPROM_EXTERNAL_ADDRESS,
                                                                     EEPROM_EXTERNAL_START_POINT_TIMESTAMP,
                                                                     N_REG_TIMESTAMP * count_overth_event,
                                                                     all_timestamp);
        } while(error == ERROR);
        
        // Sending the number of the overthreshold events stored inside the external EEPROM
        Buffer_csv[0] = count_overth_event;
        
        // Transmission of the information
        UART_PutArray(Buffer_csv, 1);
        
        // Delay to avoid conflicts during the communication
        CyDelay(100);
        
        // Initialisation of the variable to the initial condition
        count_waveform = 0;
        
        // Cycle among the overthreshol events
        for(uint8_t y = 0; y < count_overth_event; y++){
              
            // Sending the parameters associated to each event
            Buffer_csv[0] = all_sensitivity[y];
            Buffer_csv[1] = all_datarate[y];
            Buffer_csv[2] = all_timestamp[0+y*N_REG_TIMESTAMP];
            Buffer_csv[3] = all_timestamp[1+y*N_REG_TIMESTAMP];
            Buffer_csv[4] = all_timestamp[2+y*N_REG_TIMESTAMP];
            
            // Transmission of the information
            UART_PutArray(Buffer_csv, 5);
            
            // Initialisation of the variable to the initial condition
            count_waveform = 0;        
            
            // Delay to avoid conflicts during the communication
            CyDelay(10);
            
            // Waveforms
            /*
            Cycle between the 192 bytes
            The values are sent at a 200Hz frequency, same strategy of the plotting with Bridge 
            Control Panel
            */
            for(int16_t i = 0; i < N_REG_WAVEFORM - 5; i = i+6)
            {
                // Controlling the frequency of transmission
                if(count_waveform == 1) // 100Hz
                { 
                    // Index defining the first value to be sent each time
                    uint32_t index = i + N_REG_WAVEFORM*y;
                    
                    // Conversion of the data to int16
                    dataX = (int16)((all_waveforms[0+index] | (all_waveforms[1+index]<<8)))>>4;
                    dataY = (int16)((all_waveforms[2+index] | (all_waveforms[3+index]<<8)))>>4;
                    dataZ = (int16)((all_waveforms[4+index] | (all_waveforms[5+index]<<8)))>>4;
                    
                    // Conversion of the data to m/s^2 (float32)
                    accX = (float32)(dataX)*mg_TO_g*G*all_sensitivity[y];
                    accY = (float32)(dataY)*mg_TO_g*G*all_sensitivity[y];
                    accZ = (float32)(dataZ)*mg_TO_g*G*all_sensitivity[y];
                    
                    // X-axis
                    DataUnion.f = accX;
                            
                    Buffer[1] = (uint8_t)((DataUnion.l & 0xFF000000) >> 24);
                    Buffer[2] = (uint8_t)((DataUnion.l & 0x00FF0000) >> 16);
                    Buffer[3] = (uint8_t)((DataUnion.l & 0x0000FF00) >> 8);
                    Buffer[4] = (uint8_t)((DataUnion.l & 0x000000FF) >> 0);
                    
                    // Y-axis
                    DataUnion.f = accY;
                    
                    Buffer[5] = (uint8_t)((DataUnion.l & 0xFF000000) >> 24);
                    Buffer[6] = (uint8_t)((DataUnion.l & 0x00FF0000) >> 16);
                    Buffer[7] = (uint8_t)((DataUnion.l & 0x0000FF00) >> 8);
                    Buffer[8] = (uint8_t)((DataUnion.l & 0x000000FF) >> 0);
                    
                    // Z-axis
                    DataUnion.f = accZ;
                    
                    Buffer[9]  = (uint8_t)((DataUnion.l & 0xFF000000) >> 24);
                    Buffer[10] = (uint8_t)((DataUnion.l & 0x00FF0000) >> 16);
                    Buffer[11] = (uint8_t)((DataUnion.l & 0x0000FF00) >> 8);
                    Buffer[12] = (uint8_t)((DataUnion.l & 0x000000FF) >> 0);
                    
                    // Transmission of the information
                    UART_PutArray(Buffer,TRANSMIT_BUFFER_SIZE);
                    
                    // Initialisation of the variable to the initial condition
                    count_waveform = 0;
                }
                else
                {
                    // Return to the previous step when the frequency is not the correct one
                    i = i-6;
                }
            }
        }
    }
} // Export_file_CSV()


/* [] END OF FILE */
