/* ========================================
 *
 * Laboratorio di Tecnologie Elettroniche e Biosensori
 * Politecnico di Milano
 * AA 2020/2021 - I semestre
 *
 * Progetti finali:
 * - Progetto 2 -
 * Autori: Daniela Garofalo, Benedetta Pedica, Davide Sala
 * Data: 10/01/2021
 *
 * ========================================
*/

#include "Functions_EVENTS.h"

/*
Function used to write all the events information of the External EEPROM
The logic to write the waveforms (192 byte) is inverted according to the value of the current event:
    ODD event  --> 128+64 write operation
    EVEN event --> 64+128 write operation

All the operation are under a do-while cycle, this allows us to maximize the write speed,
the ACK polling strategy is used to send write commans until no error (ACK) is received
This means that the previous write operation is over
*/

void Write_EVENT_on_EXTERNAL_EEPROM(void)
{
    
    /******************************************/
    /*                WAVEFORM                */
    /******************************************/
    ErrorCode error;
    
    /****** ODD LOGIC ******/
    if ((count_overth_event % 2) != 0){
        // Write the first 128 bytes on the external EEPROM
        do {
            error = I2C_Peripheral_EXTERNAL_EEPROM_WriteRegisterMulti(EEPROM_EXTERNAL_ADDRESS,
                                                                      EEPROM_EXTERNAL_START_POINT_WAVEFORM + ((count_overth_event-1)*N_REG_WAVEFORM),
                                                                      N_REG_1PAGE,
                                                                      waveform_8bit);
        } while(error == ERROR); /* Controlling the output of the writing in order to detect
        when the operation is done --> from the datasheet, there is a maximum time needed to
        finish a multiwrite operation but in this way it is possible to optimise the timing */

        // Write the remaining 64 bytes on the external EEPROM
        do {
            error = I2C_Peripheral_EXTERNAL_EEPROM_WriteRegisterMulti(EEPROM_EXTERNAL_ADDRESS,
                                                                      EEPROM_EXTERNAL_START_POINT_WAVEFORM + ((count_overth_event-1)*N_REG_WAVEFORM) + N_REG_1PAGE,
                                                                      N_REG_HALFPAGE,
                                                                      &waveform_8bit[N_REG_1PAGE]);
        } while(error == ERROR);

    }
    
    /******* EVEN LOGIC ******/
    else{
        // Write the first 64 bytes on the external EEPROM
        do {
            error = I2C_Peripheral_EXTERNAL_EEPROM_WriteRegisterMulti(EEPROM_EXTERNAL_ADDRESS,
                                                                      EEPROM_EXTERNAL_START_POINT_WAVEFORM + ((count_overth_event-1)*N_REG_WAVEFORM),
                                                                      N_REG_HALFPAGE,
                                                                      waveform_8bit);
        } while(error == ERROR);

        // Write the remaining 128 bytes on the external EEPROM
        do {
            error = I2C_Peripheral_EXTERNAL_EEPROM_WriteRegisterMulti(EEPROM_EXTERNAL_ADDRESS,
                                                                      EEPROM_EXTERNAL_START_POINT_WAVEFORM + ((count_overth_event-1)*N_REG_WAVEFORM) + N_REG_HALFPAGE,
                                                                      N_REG_1PAGE,
                                                                      &waveform_8bit[N_REG_HALFPAGE]);
        } while(error == ERROR);

    }
    
    /******************************************/
    /*                TIMESTAMP               */
    /******************************************/
    
    // Hours
    do {
        error = I2C_Peripheral_EXTERNAL_EEPROM_WriteRegister(EEPROM_EXTERNAL_ADDRESS,
                                                             EEPROM_EXTERNAL_START_POINT_TIMESTAMP + 0 + ((count_overth_event-1)*N_REG_TIMESTAMP),
                                                             hours);
    } while(error == ERROR);
    
    // Minutes
    do {
        error = I2C_Peripheral_EXTERNAL_EEPROM_WriteRegister(EEPROM_EXTERNAL_ADDRESS,
                                                             EEPROM_EXTERNAL_START_POINT_TIMESTAMP + 1 + ((count_overth_event-1)*N_REG_TIMESTAMP),
                                                             minutes);
    } while(error == ERROR);
    
    // Seconds
    do {
        error = I2C_Peripheral_EXTERNAL_EEPROM_WriteRegister(EEPROM_EXTERNAL_ADDRESS,
                                                             EEPROM_EXTERNAL_START_POINT_TIMESTAMP + 2 + ((count_overth_event-1)*N_REG_TIMESTAMP),
                                                             seconds);
    } while(error == ERROR);
    
    /******************************************/
    /*              SENSITIVITY               */
    /******************************************/
    do {
        error = I2C_Peripheral_EXTERNAL_EEPROM_WriteRegister(EEPROM_EXTERNAL_ADDRESS,
                                                             EEPROM_EXTERNAL_START_POINT_SENSITIVITY + (count_overth_event-1),
                                                             Sensitivity);
    } while(error == ERROR);

    /******************************************/
    /*                DATARATE                */
    /******************************************/
    do {
        error = I2C_Peripheral_EXTERNAL_EEPROM_WriteRegister(EEPROM_EXTERNAL_ADDRESS,
                                                             EEPROM_EXTERNAL_START_POINT_DATARATE + (count_overth_event-1),
                                                             DataRate_reg);
    } while(error == ERROR);
}


/*
Function used to read the waveforms saved in the external eeprom,
convert them in int32 and send consequently by the UART
*/
void Read_Waveform_from_EXTERNAL_EEPROM(void)
{
    // Allocate the memory for the information referred to all the overthreshold events
    uint8_t all_waveforms[count_overth_event*N_REG_WAVEFORM];
    uint8_t all_sensitivity[count_overth_event];
    uint8_t all_datarate[count_overth_event];
    
    // Buffer used to write a pause of all zeros between successive waveforms
    uint8_t Pause[TRANSMIT_BUFFER_SIZE] = {0xA0,0,0,0,0,0,0,0,0,0,0,0,0,0xC0};
    
    // Waveform reading
    ErrorCode error = I2C_Peripheral_EXTERNAL_EEPROM_ReadRegisterMulti(EEPROM_EXTERNAL_ADDRESS,
                                                                       EEPROM_EXTERNAL_START_POINT_WAVEFORM,
                                                                       N_REG_WAVEFORM * count_overth_event,
                                                                       all_waveforms);
    // Sensitivity reading
    do {
        error = I2C_Peripheral_EXTERNAL_EEPROM_ReadRegisterMulti(EEPROM_EXTERNAL_ADDRESS,
                                                                 EEPROM_EXTERNAL_START_POINT_SENSITIVITY,
                                                                 count_overth_event,
                                                                 all_sensitivity);
    } while(error == ERROR);
    
    // Datarate reading
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
        the operation of reading the elements inside the FIFO register needs 2 clocks to be done */
        if (all_datarate[y] == MASK_DATARATE_50Hz){
            count_for_plotting = 4; // (200 Hz/4)/2 = 25 Hz
        }
        else if (all_datarate[y] == MASK_DATARATE_100Hz){
            count_for_plotting = 2; // (200 Hz/2)/2 = 50 Hz
        }
        else if (all_datarate[y] == MASK_DATARATE_200Hz){
            count_for_plotting = 1; // 200 Hz/2 = 100 Hz
        }
        
        // Initialisation of the variable to the initial condition
        count_waveform = 0;
        
        // Cycle among the values associated to each waveform (192 bytes)
        for(int16_t i = 0; i < N_REG_WAVEFORM - 5; i = i+6){
            
            /* Controlling that the transmission is happening with the same frequency with which
            the waveforms have been generated */
            if(count_waveform == count_for_plotting){
            
                // Index defining the first value to be sent each time
                uint32_t index = i + N_REG_WAVEFORM*y;
                
                // Conversion of the data to int16 type
                dataX = (int16)((all_waveforms[0+index] | (all_waveforms[1+index]<<8)))>>4;
                dataY = (int16)((all_waveforms[2+index] | (all_waveforms[3+index]<<8)))>>4;
                dataZ = (int16)((all_waveforms[4+index] | (all_waveforms[5+index]<<8)))>>4;
                
                // Conversion of data to m/s^2 (float32)
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
                
                // Transmission of the data 
                UART_PutArray(Buffer,TRANSMIT_BUFFER_SIZE);
                
                // Initialisation of the variable to the initial condition
                count_waveform = 0;
            }
            /* Return to the previous step (determined by the value of i) when the freqeuency of
            transmission is not the correct one */
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
                // Return to the previous step when the frequency is not equal to 100 Hz
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
}


/*
Function used to read the timestamps saved in the external eeprom
and send them through the UART
*/
void Read_Timestamp_from_EXTERNAL_EEPROM(void)
{
    // Allocate the memory for the timestamps referred to all the overthreshold events
    uint8_t all_timestamp[count_overth_event*N_REG_TIMESTAMP];
    char string[100];
    
    // Reading all the timestamps saved into the external EEPROM
    ErrorCode error = I2C_Peripheral_EXTERNAL_EEPROM_ReadRegisterMulti(EEPROM_EXTERNAL_ADDRESS,
                                                                       EEPROM_EXTERNAL_START_POINT_TIMESTAMP,
                                                                       N_REG_TIMESTAMP * count_overth_event,
                                                                       all_timestamp);
    if(error == ERROR){
        UART_PutString("Error occurred during I2C comm\r\n");  
    }

    // Sending a string containing the timestamp information for each of the event
    for (uint8_t i = 0; i < count_overth_event; i++)
    {
        // Transmission of the information through the UART communication protocol
        sprintf(string,"\n# event --> %d  Timestamp --> Hour: %d Minute: %d Second: %d\n\n", i+1, all_timestamp[0+i*N_REG_TIMESTAMP], all_timestamp[1+i*N_REG_TIMESTAMP], all_timestamp[2+i*N_REG_TIMESTAMP]);
        UART_PutString(string);
    }
}


/*
This function is used to read, interpret and send all the info
about the events to the serial port.
Then a Python file is used to re-interpret all the info and
save them in a CSV file (used then to plot all the events info)
*/

void Export_file_CSV(void)
{
    // Allocate the memory for the information referred to all the overthreshold events
    uint8_t all_waveforms[count_overth_event*N_REG_WAVEFORM];
    uint8_t all_sensitivity[count_overth_event];
    uint8_t all_datarate[count_overth_event];
    uint8_t all_timestamp[count_overth_event*N_REG_TIMESTAMP];
    
    // Definition of the buffer dimension, header and tail for the communication
    Buffer[0] = 0xA0;
    Buffer[TRANSMIT_BUFFER_SIZE-1] = 0xC0;
    
    // Waveforms
    ErrorCode error = I2C_Peripheral_EXTERNAL_EEPROM_ReadRegisterMulti(EEPROM_EXTERNAL_ADDRESS,
                                                                       EEPROM_EXTERNAL_START_POINT_WAVEFORM,
                                                                       (N_REG_WAVEFORM * count_overth_event),
                                                                       all_waveforms);
    
    // Sensitivity
    do {
        error = I2C_Peripheral_EXTERNAL_EEPROM_ReadRegisterMulti(EEPROM_EXTERNAL_ADDRESS,
                                                                 EEPROM_EXTERNAL_START_POINT_SENSITIVITY,
                                                                 count_overth_event,
                                                                 all_sensitivity);
    } while(error == ERROR);
    
    // Datarate
    do {
        error = I2C_Peripheral_EXTERNAL_EEPROM_ReadRegisterMulti(EEPROM_EXTERNAL_ADDRESS,
                                                                 EEPROM_EXTERNAL_START_POINT_DATARATE,
                                                                 count_overth_event,
                                                                 all_datarate);
    } while(error == ERROR);
    
    // Timestamp
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
    
    // Cycle among the events
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
            if(count_waveform == 1) // 200Hz
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
    Buffer[0] = 0xE0;
    UART_PutArray(Buffer,1);
}


/* [] END OF FILE */
