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
#include "Functions.h"


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
        
        if(device_state == RUN){
            if (flag_ACC == 1){

                /*
                Read the register INT2_SRC where the pin AI is high if an interrupt
                on INT2(overthreshold event occur).
                if pin AI is HIGH --> isr caused by an overthreshold event
                if pin AI is LOW --> isr cause by the ZYXDA event (new data available for sampling)
                */
                error = I2C_Peripheral_ReadRegister(LIS3DH_DEVICE_ADDRESS,
                                                    LIS3DH_INT2_SRC, 
                                                    &reg_INT2_SRC);
                if(error == ERROR){
                    UART_PutString("Error occurred during I2C comm\r\n");  
                }
                
                if (reg_INT2_SRC & MASK_OVERTH_EVENT){
                    flag_overth_event = 1; //NO SAMPLING, OVERTHRESHOLD EVENT SAVE
                }
                else {
                    flag_overth_event = 0; //SAMPLING
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
                
                else if (flag_overth_event == 1){
                    
                    count_overth_event++;
                    
                    UART_PutString("OVERTHRESHOLD EVENT!!");
                    
                    
                    error = I2C_Peripheral_ReadRegisterMulti(LIS3DH_DEVICE_ADDRESS,
                                                             OUT_X_L,
                                                             N_REG_WAVEFORM,
                                                             waveform_8bit);
                    if(error == ERROR){
                        UART_PutString("Error occurred during I2C comm0\r\n");  
                    }
                    
                    //Function to write the EVENT in the EXTERNAL EEPROM
                    Write_EVENT_on_EXTERNAL_EEPROM();
                    
                    CyDelay(1000); //To not be sensible to consecutive events
                 
                    Register_Initialization_after_Overth_Event(); //The last thing to do!!
                }
            }
        }
        
        /******************************************/
        /*              PRINT EVENTS              */
        /******************************************/
        
        if(device_state == WAIT){
            if(flag_send_waveform == 1){
                
                Read_Waveform_from_EXTERNAL_EEPROM();
            }
            
            if(flag_send_timestamps == 1){
                
                Read_Timestamp_from_EXTERNAL_EEPROM();
                flag_send_timestamps = 0;
            }
            if (flag_export_file == 1){
            
                Export_file_CSV();
                flag_export_file = 0;
            }
        }
    }
}

/* [] END OF FILE */
