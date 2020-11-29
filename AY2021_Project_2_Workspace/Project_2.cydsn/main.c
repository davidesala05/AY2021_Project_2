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
uint8_t full_scale_range;
uint8_t threshold;

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */
  
    CyDelay(100);
    
    ErrorCode error;
    
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
    FS_range = 2;

    for(;;)
    {
        
         if (flag_ACC == 1){
            
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
            
            accX = (float32)(dataX)*mg_TO_g*G*Sensitivity;
            accY = (float32)(dataY)*mg_TO_g*G*Sensitivity;
            accZ = (float32)(dataZ)*mg_TO_g*G*Sensitivity;
            
            Set_RGB();
            
            
            
            /*
            WORKFLOW: control on Full Scale Range (changed through configuration menu)
                      control on time duration
                      if both conditions are true, log data in EEPROM
            */
            I2C_Peripheral_ReadRegister(LIS3DH_DEVICE_ADDRESS,
                                       LIS3DH_CTRL_REG4,
                                       &full_scale_range);
            switch (full_scale_range)
            {
                case LIS3DH_CTRL_REG4_FS_2G:
                threshold=LIS3DH_INT2_THS_INIT*COEFFICIENT_2G;
                break;
                case LIS3DH_CTRL_REG4_FS_4G:
                threshold=LIS3DH_INT2_THS_INIT*COEFFICIENT_4G;
                break;
                case LIS3DH_CTRL_REG4_FS_8G:
                threshold=LIS3DH_INT2_THS_INIT*COEFFICIENT_8G;
                break;
                case LIS3DH_CTRL_REG4_FS_16G:
                threshold=LIS3DH_INT2_THS_INIT*COEFFICIENT_16G;
                break;            
            }
            
            /*
            Check register for duration of event; duration time is: Content of duration register/ODR
            If ODR is 400 Hz we set the register to 120 so that duration is 0.3 seconds (max value we can set in register is 127)
            */
            I2C_Peripheral_WriteRegister(LIS3DH_DEVICE_ADDRESS,
                                        LIS3DH_INT2_DURATION,
                                        LIS3DH_INT2_DURATION_VALUE);
                 
    
         }
    }
}

/* [] END OF FILE */
