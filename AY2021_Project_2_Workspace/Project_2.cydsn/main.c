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
#include "InterruptRoutines_ACC.h"
#include "stdio.h"
#include "24LC512.h"

uint8 full_scale_range;
uint8 output_data_rate;

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */
  
    CyDelay(100);
    
    ErrorCode error;
    
    Register_to_value();
    
    Initialize_Parameters();
    
      char message[50] = {'\0'};  //buffer string to print out message in UART
    
    //check which devices are present on the I2C bus 
    for(uint8 i=0; i<128; i++)
    {
        if(I2C_IsDeviceConnected(i))
        {
            //print out the adress in Hex format
            sprintf(message, "Device 0x%02X is connected\r\n", i);
            UART_PutString(message); 
        }

    }
    
    Sensitivity = 1;
    FS_range_value = 2;

    for(;;)
   
    {
        
        
        /******************************************/
        /*            INTERRUPT BY ACC            */
        /******************************************/
        
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
            
            /*
            WORKFLOW: control on Full Scale Range (changed through configuration menu)
                      control on time duration
                      if both conditions are true, log data in EEPROM
            */
            
            /*We read the register on which is saved the value of FS, depending on it the value inside the
            THS register is changes accordingly to a conversion so that we have a fixed threshold of 2G 
            for all the different full scale range selected through the menu
            */
            

            switch (FS_range_value) //switch  FS_range_value 
            {
                case 2:
                I2C_Peripheral_WriteRegister(LIS3DH_DEVICE_ADDRESS,
                                             LIS3DH_INT2_THS,              
                                             LIS3DH_INT2_THS_2G);
                break;
                case 4:
                I2C_Peripheral_WriteRegister(LIS3DH_DEVICE_ADDRESS,
                                             LIS3DH_INT2_THS,              
                                             LIS3DH_INT2_THS_4G);
                break;
                case 8:
                I2C_Peripheral_WriteRegister(LIS3DH_DEVICE_ADDRESS,
                                             LIS3DH_INT2_THS,              
                                             LIS3DH_INT2_THS_8G);
                break;
                case 16:
                I2C_Peripheral_WriteRegister(LIS3DH_DEVICE_ADDRESS,
                                             LIS3DH_INT2_THS,              
                                             LIS3DH_INT2_THS_16G);
                break;            
            }
            
            /*
            Check register for duration of event; duration time is: Content of duration register/ODR
            We check on ODR value on the register and re-write it basing on the frequency we have
            */
            
             switch (DataRate_value)
            {
                case 50:
                I2C_Peripheral_WriteRegister(LIS3DH_DEVICE_ADDRESS,
                                             LIS3DH_INT2_DURATION,              
                                             LIS3DH_INT2_DURATION_50HZ);
                break;
                case 100:
                I2C_Peripheral_WriteRegister(LIS3DH_DEVICE_ADDRESS,
                                             LIS3DH_INT2_DURATION,              
                                             LIS3DH_INT2_DURATION_100HZ);
                break;
                case 200:
                I2C_Peripheral_WriteRegister(LIS3DH_DEVICE_ADDRESS,
                                             LIS3DH_INT2_DURATION,              
                                             LIS3DH_INT2_DURATION_200HZ);
                break;
    
            }
            
           /* I2C_Peripheral_ReadEEPROMRegisterMulti(EEPROM_EXTERNAL_ADDRESS,
                                             OUT_X_L,
                                             OUT_X_L,
                                             N_REG_ACC,
                                             data);
            */
        }
    }
  }
}
/* [] END OF FILE */
