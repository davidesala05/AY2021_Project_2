/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/

#include "Functions_EVENTS.h"

/*
Function used to write all the events information o fthe External EEPROM
The logic to write the waveforms (192 byte) is inverted according to the value of the current event:
    ODD event  --> 128+64 write operation
    EVEN event --> 64+128 write operation

All the operation are under a do-while cycle, this allows us to maximize the write speed,
the ACK polling strategy is used to send write commans until no error (ACK) is received
This means that the previous write operation is over
*/
void Write_EVENT_on_EXTERNAL_EEPROM(void){

    /******************************************/
    /*                WAVEFORM                */
    /******************************************/
    ErrorCode error;
    
    //ODD logic
    if ((count_overth_event % 2) != 0){
        //FIRST 128 registers
        do {
            error = I2C_Peripheral_EXTERNAL_EEPROM_WriteRegisterMulti(EEPROM_EXTERNAL_ADDRESS,
                                                                      EEPROM_EXTERNAL_START_POINT_WAVEFORM + ((count_overth_event-1)*N_REG_WAVEFORM),
                                                                      N_REG_1PAGE,
                                                                      waveform_8bit);
        } while(error == ERROR);

        //REMAIN 64 registers
        do {
            error = I2C_Peripheral_EXTERNAL_EEPROM_WriteRegisterMulti(EEPROM_EXTERNAL_ADDRESS,
                                                                      EEPROM_EXTERNAL_START_POINT_WAVEFORM + ((count_overth_event-1)*N_REG_WAVEFORM) + N_REG_1PAGE,
                                                                      N_REG_HALFPAGE,
                                                                      &waveform_8bit[N_REG_1PAGE]);
        } while(error == ERROR);

    }
    //EVEN logic
    else{
        //FIRST 64 registers
        do {
            error = I2C_Peripheral_EXTERNAL_EEPROM_WriteRegisterMulti(EEPROM_EXTERNAL_ADDRESS,
                                                                      EEPROM_EXTERNAL_START_POINT_WAVEFORM + ((count_overth_event-1)*N_REG_WAVEFORM),
                                                                      N_REG_HALFPAGE,
                                                                      waveform_8bit);
        } while(error == ERROR);

        //REMAIN 128 registers
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
    //HOURS
    do {
        error = I2C_Peripheral_EXTERNAL_EEPROM_WriteRegister(EEPROM_EXTERNAL_ADDRESS,
                                                             EEPROM_EXTERNAL_START_POINT_TIMESTAMP + 0 + ((count_overth_event-1)*N_REG_TIMESTAMP),
                                                             hours);
    } while(error == ERROR);
    //MINUTES
    do {
        error = I2C_Peripheral_EXTERNAL_EEPROM_WriteRegister(EEPROM_EXTERNAL_ADDRESS,
                                                             EEPROM_EXTERNAL_START_POINT_TIMESTAMP + 1 + ((count_overth_event-1)*N_REG_TIMESTAMP),
                                                             minutes);
    } while(error == ERROR);
    //SECONDS
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
void Read_Waveform_from_EXTERNAL_EEPROM(void){
    
    //In this case are not used global variables beacuse the memory must be allocated according to the number of events that are been generated
    uint8_t all_waveforms[count_overth_event*N_REG_WAVEFORM];
    uint8_t all_sensitivity[count_overth_event];
    uint8_t all_datarate[count_overth_event];
    //buffer used to write a pause of all zeros between successive waveforms
    uint8_t Pause[TRANSMIT_BUFFER_SIZE] = {0xA0,0,0,0,0,0,0,0,0,0,0,0,0,0xC0};
    //WAVEFORM READING
    ErrorCode error = I2C_Peripheral_EXTERNAL_EEPROM_ReadRegisterMulti(EEPROM_EXTERNAL_ADDRESS,
                                                                       EEPROM_EXTERNAL_START_POINT_WAVEFORM,
                                                                       N_REG_WAVEFORM * count_overth_event,
                                                                       all_waveforms);
    //SENSITIVITY READING
    do {
        error = I2C_Peripheral_EXTERNAL_EEPROM_ReadRegisterMulti(EEPROM_EXTERNAL_ADDRESS,
                                                                 EEPROM_EXTERNAL_START_POINT_SENSITIVITY,
                                                                 count_overth_event,
                                                                 all_sensitivity);
    } while(error == ERROR);
    //DATARATE READING
    do {
        error = I2C_Peripheral_EXTERNAL_EEPROM_ReadRegisterMulti(EEPROM_EXTERNAL_ADDRESS,
                                                                 EEPROM_EXTERNAL_START_POINT_DATARATE,
                                                                 count_overth_event,
                                                                 all_datarate);
    } while(error == ERROR);
    
    //Cycle between the events
    for(uint8_t y = 0; y < count_overth_event; y++){
        //The waveforms must be send with the same frequency with which are been generated
        if (all_datarate[y] == MASK_DATARATE_50Hz){
            count_for_plotting = 4; //25 Hz
        }
        else if (all_datarate[y] == MASK_DATARATE_100Hz){
            count_for_plotting = 2; //50 Hz
        }
        else if (all_datarate[y] == MASK_DATARATE_200Hz){
            count_for_plotting = 1; //100 Hz
        }
        
        count_waveform = 0;
        
        //Cycle between the values of each waveform (192 bytes)
        for(int16_t i = 0; i < N_REG_WAVEFORM - 5; i = i+6){
            
            //If is the right moment to send the buffer
            if(count_waveform == count_for_plotting){
            
                //index of the current value
                uint32_t index = i + N_REG_WAVEFORM*y;
                //Data are converted into int16
                dataX = (int16)((all_waveforms[0+index] | (all_waveforms[1+index]<<8)))>>4;
                dataY = (int16)((all_waveforms[2+index] | (all_waveforms[3+index]<<8)))>>4;
                dataZ = (int16)((all_waveforms[4+index] | (all_waveforms[5+index]<<8)))>>4;
                //Data are converted into m/s^2 (float32)
                accX = (float32)(dataX)*mg_TO_g*G*all_sensitivity[y];
                accY = (float32)(dataY)*mg_TO_g*G*all_sensitivity[y];
                accZ = (float32)(dataZ)*mg_TO_g*G*all_sensitivity[y];
                
                //X-axis
                DataUnion.f = accX;
                        
                Buffer[1] = (uint8_t)((DataUnion.l & 0xFF000000) >> 24);
                Buffer[2] = (uint8_t)((DataUnion.l & 0x00FF0000) >> 16);
                Buffer[3] = (uint8_t)((DataUnion.l & 0x0000FF00) >> 8);
                Buffer[4] = (uint8_t)((DataUnion.l & 0x000000FF) >> 0);
                
                //Y-axis
                DataUnion.f = accY;
                
                Buffer[5] = (uint8_t)((DataUnion.l & 0xFF000000) >> 24);
                Buffer[6] = (uint8_t)((DataUnion.l & 0x00FF0000) >> 16);
                Buffer[7] = (uint8_t)((DataUnion.l & 0x0000FF00) >> 8);
                Buffer[8] = (uint8_t)((DataUnion.l & 0x000000FF) >> 0);
                
                //Z-axis
                DataUnion.f = accZ;
                
                Buffer[9]  = (uint8_t)((DataUnion.l & 0xFF000000) >> 24);
                Buffer[10] = (uint8_t)((DataUnion.l & 0x00FF0000) >> 16);
                Buffer[11] = (uint8_t)((DataUnion.l & 0x0000FF00) >> 8);
                Buffer[12] = (uint8_t)((DataUnion.l & 0x000000FF) >> 0);
                
                /*The BUFFER is sent by the UART*/
                UART_PutArray(Buffer,TRANSMIT_BUFFER_SIZE);
                    
                count_waveform = 0;
            }
            //Is is not the right moment to send the data, the for cycle must be taken back to the previous step
            else{
                i = i-6;
            }
            //If is not necessary anymore to plot the waveforms ("s" by user)
            if(flag_send_waveform == 0){
                break;
            }
        }
        //If is not necessary anymore to plot the waveforms ("s" by user)
        if(flag_send_waveform == 0){
                break;
        }
        
        count_waveform = 0;
        //Write 100 zero values at 100Hz as pause
        for(int16_t i = 0; i < PAUSE_LENGHT; i++){
            
            if(count_waveform == 1){
            
                UART_PutArray(Pause,TRANSMIT_BUFFER_SIZE);
                count_waveform = 0;
            }
            else{
                i--;
            }
            if(flag_send_waveform == 0){
                break;
            }
        }
        
        if(flag_send_waveform == 0){
                break;
        }
    }
}

/*
Function used to read the timestamps saved in the external eeprom
and send them through the UART
*/
void Read_Timestamp_from_EXTERNAL_EEPROM(void){
    
    //Allocate the memory for the timestamps of all the events
    uint8_t all_timestamp[count_overth_event*N_REG_TIMESTAMP];
    char string[100];
    
    ErrorCode error = I2C_Peripheral_EXTERNAL_EEPROM_ReadRegisterMulti(EEPROM_EXTERNAL_ADDRESS,
                                                                       EEPROM_EXTERNAL_START_POINT_TIMESTAMP,
                                                                       N_REG_TIMESTAMP * count_overth_event,
                                                                       all_timestamp);
    if(error == ERROR){
        UART_PutString("Error occurred during I2C comm\r\n");  
    }

    //Send the string with the timestamp information for each of the event
    for (uint8_t i = 0; i < count_overth_event; i++){
        
        sprintf(string,"\n# event --> %d  Timestamp --> Hour: %d Minute: %d Second: %d\n\n", i+1, all_timestamp[0+i*N_REG_TIMESTAMP], all_timestamp[1+i*N_REG_TIMESTAMP], all_timestamp[2+i*N_REG_TIMESTAMP]);
        UART_PutString(string);
    }
}



/*
This function is used to read, interpret and send all the info
about the events to the serial port.
Then a python file is used to re-interpret all the info and
save them in a CSV file (used then to plot all the events info)
*/

void Export_file_CSV(void){

    //Allocate all the necessary memory for the events' info
    uint8_t all_waveforms[count_overth_event*N_REG_WAVEFORM];
    uint8_t all_sensitivity[count_overth_event];
    uint8_t all_datarate[count_overth_event];
    uint8_t all_timestamp[count_overth_event*N_REG_TIMESTAMP];
    
    Buffer[0] = 0xA0;
    Buffer[TRANSMIT_BUFFER_SIZE-1] = 0xC0;
    
    //READ DATA FROM  EXTERNAL EEPROM
    
    ErrorCode error = I2C_Peripheral_EXTERNAL_EEPROM_ReadRegisterMulti(EEPROM_EXTERNAL_ADDRESS,
                                                                       EEPROM_EXTERNAL_START_POINT_WAVEFORM,
                                                                       (N_REG_WAVEFORM * count_overth_event),
                                                                       all_waveforms);
    do {
        error = I2C_Peripheral_EXTERNAL_EEPROM_ReadRegisterMulti(EEPROM_EXTERNAL_ADDRESS,
                                                                 EEPROM_EXTERNAL_START_POINT_SENSITIVITY,
                                                                 count_overth_event,
                                                                 all_sensitivity);
    } while(error == ERROR);
    
    do {
        error = I2C_Peripheral_EXTERNAL_EEPROM_ReadRegisterMulti(EEPROM_EXTERNAL_ADDRESS,
                                                                 EEPROM_EXTERNAL_START_POINT_DATARATE,
                                                                 count_overth_event,
                                                                 all_datarate);
    } while(error == ERROR);
    
    do {
    
        error = I2C_Peripheral_EXTERNAL_EEPROM_ReadRegisterMulti(EEPROM_EXTERNAL_ADDRESS,
                                                                 EEPROM_EXTERNAL_START_POINT_TIMESTAMP,
                                                                 N_REG_TIMESTAMP * count_overth_event,
                                                                 all_timestamp);
    } while(error == ERROR);
    
    
    //SEND DATA TO PYTHON
    
    //SEND COUNT OVERTH EVENTS
    Buffer_csv[0] = count_overth_event;
    UART_PutArray(Buffer_csv, 1);
    
    CyDelay(100);
    
    count_waveform = 0;
    
    //Cycle between the events
    for(uint8_t y = 0; y < count_overth_event; y++){
          
        //SEND PARAMETERS EVENTS
        Buffer_csv[0] = all_sensitivity[y];
        Buffer_csv[1] = all_datarate[y];
        Buffer_csv[2] = all_timestamp[0+y*N_REG_TIMESTAMP];
        Buffer_csv[3] = all_timestamp[1+y*N_REG_TIMESTAMP];
        Buffer_csv[4] = all_timestamp[2+y*N_REG_TIMESTAMP];
        
        UART_PutArray(Buffer_csv, 5);

        count_waveform = 0;        
        
        CyDelay(10);
        
        //SEND WAVEFORMS
        /*
        Cycle between the 192 bytes
        The values are sent at a 200Hz frequency, same strategy of the plotting with Bridge Control Panel
        */
        for(int16_t i = 0; i < N_REG_WAVEFORM - 5; i = i+6){
            
            if(count_waveform == 1){ //200Hz
                
                uint32_t index = i + N_REG_WAVEFORM*y;
                
                dataX = (int16)((all_waveforms[0+index] | (all_waveforms[1+index]<<8)))>>4;
                dataY = (int16)((all_waveforms[2+index] | (all_waveforms[3+index]<<8)))>>4;
                dataZ = (int16)((all_waveforms[4+index] | (all_waveforms[5+index]<<8)))>>4;
                
                accX = (float32)(dataX)*mg_TO_g*G*all_sensitivity[y];
                accY = (float32)(dataY)*mg_TO_g*G*all_sensitivity[y];
                accZ = (float32)(dataZ)*mg_TO_g*G*all_sensitivity[y];
                
                //X-axis
                DataUnion.f = accX;
                        
                Buffer[1] = (uint8_t)((DataUnion.l & 0xFF000000) >> 24);
                Buffer[2] = (uint8_t)((DataUnion.l & 0x00FF0000) >> 16);
                Buffer[3] = (uint8_t)((DataUnion.l & 0x0000FF00) >> 8);
                Buffer[4] = (uint8_t)((DataUnion.l & 0x000000FF) >> 0);
                
                //Y-axis
                DataUnion.f = accY;
                
                Buffer[5] = (uint8_t)((DataUnion.l & 0xFF000000) >> 24);
                Buffer[6] = (uint8_t)((DataUnion.l & 0x00FF0000) >> 16);
                Buffer[7] = (uint8_t)((DataUnion.l & 0x0000FF00) >> 8);
                Buffer[8] = (uint8_t)((DataUnion.l & 0x000000FF) >> 0);
                
                //Z-axis
                DataUnion.f = accZ;
                
                Buffer[9]  = (uint8_t)((DataUnion.l & 0xFF000000) >> 24);
                Buffer[10] = (uint8_t)((DataUnion.l & 0x00FF0000) >> 16);
                Buffer[11] = (uint8_t)((DataUnion.l & 0x0000FF00) >> 8);
                Buffer[12] = (uint8_t)((DataUnion.l & 0x000000FF) >> 0);
                
                /*The BUFFER is sent by the UART*/
                UART_PutArray(Buffer,TRANSMIT_BUFFER_SIZE);
                    
                count_waveform = 0;
            }
            else{
                i = i-6;
            }
        }
    }
}


/* [] END OF FILE */
