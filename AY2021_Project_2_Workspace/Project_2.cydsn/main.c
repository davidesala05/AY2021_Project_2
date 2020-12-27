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
#include "project.h"
#include "Global.h"
#include "stdio.h"
#include "Functions_SETTINGS.h"
#include "Functions_EVENTS.h"


int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */
    
    Start_Components_powerON(); //To start the components at the powerON of the device
    
    CyDelay(100);
    
    Register_Initialization(); //To initialize all the registers of the accelerometer
    
    Register_Initialization_after_Overth_Event();
    
    CyDelay(100);
    
    Initialize_Parameters(); //To initialize the parameter (DataRate, FS range and verbose flag) based on the values saved in the EEPROM
    
    Register_to_value(); //To convert the register of a parameter to its real value used in the code
    
    HM_Stop();
    
    CyDelay(100);
    
    Control_Reg_Write(MUX_CHANNEL_COLOUR);

    ErrorCode error;
    
    Buffer[0] = HEADER;
    Buffer[TRANSMIT_BUFFER_SIZE-1] = TAIL;

    for(;;)
    {
        
        /******************************************/
        /*              CONFIGURATION             */
        /******************************************/
        
        Hardware_Menu();
        
        /******************************************/
        /*            INTERRUPT BY ACC            */
        /******************************************/
        
        if(device_state == RUN){ //If the device in RUN mode
            
            if (flag_ACC == 1){ //If an ISR occurs by the accelerometer

                /*
                Read the register INT2_SRC where the pin AI is high if an interrupt
                on INT2(overthreshold event occur).
                if pin AI is HIGH --> isr caused by an overthreshold event
                if pin AI is LOW --> isr cause by the OVERRUN event
                */
                error = I2C_Peripheral_ReadRegister(LIS3DH_DEVICE_ADDRESS,
                                                    LIS3DH_INT2_SRC, 
                                                    &reg_INT2_SRC);
                if(error == ERROR){
                    UART_PutString("Error occurred during I2C comm\r\n");  
                }
                //NO SAMPLING, OVERTHRESHOLD EVENT SAVE
                if (reg_INT2_SRC & MASK_OVERTH_EVENT){
                    flag_overth_event = 1;
                    current_timestamp = hours*60*60 + minutes*60 + seconds + count_global/f_timer; //resolution of milliseconds
                }
                //SAMPLING
                else {
                    flag_overth_event = 0;
                }
                
                /******************************************/
                /*                SAMPLING                */
                /******************************************/
                
                if (flag_overth_event == 0){
                    /*
                    The new data are read by the contiguous
                    registers and saved in the variable
                    */
                    error = I2C_Peripheral_ReadRegisterMulti(LIS3DH_DEVICE_ADDRESS,
                                                             OUT_X_L,
                                                             N_REG_ACC,
                                                             data);
                    if(error == ERROR){
                        UART_PutString("Error occurred during I2C comm\r\n");  
                    }

                    /*
                    Data are transformed in int16 and divided by the corresponding axis
                    Each of the three data is expressed as two’s complement left-justified in 12 bit,
                    so a shift by 4 bits is required
                    */
                    dataX = (int16)((data[0] | (data[1]<<8)))>>4;
                    dataY = (int16)((data[2] | (data[3]<<8)))>>4;
                    dataZ = (int16)((data[4] | (data[5]<<8)))>>4;
                    
                    /*
                    Below the data from the accelerometer are converted in m/s^2
                    and casted as float32, the sensitivity is not constant since it change
                    with the full-scale range that is currently set.
                    */
                    accX = (float32)(dataX)*mg_TO_g*G*Sensitivity;
                    accY = (float32)(dataY)*mg_TO_g*G*Sensitivity;
                    accZ = (float32)(dataZ)*mg_TO_g*G*Sensitivity;
                    
                    /*
                    Function Set_RGB() is used to convert the acceleration data
                    in colour of the RGB LED.
                    */
                    Set_RGB();
                    
                    /******************************************/
                    /*              VERBOSE FLAG              */
                    /******************************************/
                    
                    /*
                    If the VERBOSE_FLAG is HIGH, thus the data coming from the accelerometer
                    are sent raw to the serial port by the UART.
                    */
                    if(Verbose_flag == 1){
                        
                        /*
                        Below the data are splitted in single byte in order to be sent by the UART
                        and correctly interpreted as float32 by the bridge control panel.
                        */
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
                    }
                    
                    /*END OF THE SAMPLING*/
                    flag_ACC = 0;
                }
                
                /******************************************/
                /*         OVERTHRESHOLD EVENT            */
                /******************************************/
                /*
                If the ISR is cause by an overthreshold event
                AND the current event is not in the same timestamp as the previous
                (The timestamp has a millisecond resolution for this purpose only)
                */
                else if ((flag_overth_event == 1) && (current_timestamp >= (old_timestamp + one_SECOND))){
                    
                    count_overth_event++; //New overthreshold event
                    
                    UART_PutString("OVERTHRESHOLD EVENT!!"); //String used to get know of the event by coolterm (in case of debugging)
                    
                    /*
                    Below all the 192 register are read, in order to get the 32 values for each of the axis
                    that correspond to the historical waveform that causes the event
                    */
                    error = I2C_Peripheral_ReadRegisterMulti(LIS3DH_DEVICE_ADDRESS,
                                                             OUT_X_L,
                                                             N_REG_WAVEFORM,
                                                             waveform_8bit);
                    if(error == ERROR){
                        UART_PutString("Error occurred during I2C comm0\r\n");  
                    }
                    
                    //Function to write the EVENT in the EXTERNAL EEPROM
                    Write_EVENT_on_EXTERNAL_EEPROM();
                    //This function is used to reset the FIFO register to be in stream-to-FIFO mode
                    Register_Initialization_after_Overth_Event();
                    
                    old_timestamp = current_timestamp; //the old timestamp is updated
                    
                    flag_ACC = 0;
                }
            }
        }
        
        /******************************************/
        /*              PRINT EVENTS              */
        /******************************************/
        //If the device is in WAIT mode only
        if(device_state == WAIT){
            //send the waveforms of the events in loop to the Bridge Control Panel
            if(flag_send_waveform == 1){
                //Function that reads the External EEPROM and sends the values to the serial port
                Read_Waveform_from_EXTERNAL_EEPROM();
            }
            //Send the timestamps of the events to Coolterm (textual information)
            if(flag_send_timestamps == 1){
                //Function that reads the External EEPROM and sends the values to the serial port
                Read_Timestamp_from_EXTERNAL_EEPROM();
                flag_send_timestamps = 0;
            }
            /*Send all the information of the events (waveforms, parameters and timestamps)
            to the serial port in order to be interpreted and save in a python program
            The aim is to export a CSV file and plot the figure with the events and all their information
            */
            if (flag_export_file == 1){
                //Function to read, elaborate and send all the info
                Export_file_CSV();//Function to read, elaborate and send all the info
                flag_export_file = 0;
            }
        }
    }
}

/* [] END OF FILE */
