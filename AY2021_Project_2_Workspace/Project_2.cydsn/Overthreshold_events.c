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
#include "Global.h"
#include "Overthreshold_events.h"


void Set_FS_Registers (void){
    
            /*We read the register on which is saved the value of FS, depending on it the value inside the
            THS register is changes accordingly to a conversion so that we have a fixed threshold of 2G 
            for all the different full scale range selected through the menu
            */
            
            switch (FS_range_reg) //switch  FS_range_value 
            {
                case MASK_FS_RANGE_2G:
                I2C_Peripheral_WriteRegister(LIS3DH_DEVICE_ADDRESS,
                                             LIS3DH_INT2_THS,              
                                             LIS3DH_INT2_THS_2G);
                break;
                case MASK_FS_RANGE_4G:
                I2C_Peripheral_WriteRegister(LIS3DH_DEVICE_ADDRESS,
                                             LIS3DH_INT2_THS,              
                                             LIS3DH_INT2_THS_4G);
                break;
                case MASK_FS_RANGE_8G:
                I2C_Peripheral_WriteRegister(LIS3DH_DEVICE_ADDRESS,
                                             LIS3DH_INT2_THS,              
                                             LIS3DH_INT2_THS_8G);
                break;
                case MASK_FS_RANGE_16G:
                I2C_Peripheral_WriteRegister(LIS3DH_DEVICE_ADDRESS,
                                             LIS3DH_INT2_THS,              
                                             LIS3DH_INT2_THS_16G);
                break;
                default:
                break;
            }
}

void Set_Duration_Registers (void){
    
            /*
            Check register for duration of event; duration time is: Content of duration register/ODR
            We check on ODR value on the register and re-write it basing on the frequency we have
            */
            
             switch (DataRate_reg)
            {
                case MASK_DATARATE_50Hz:
                I2C_Peripheral_WriteRegister(LIS3DH_DEVICE_ADDRESS,
                                             LIS3DH_INT2_DURATION,              
                                             LIS3DH_INT2_DURATION_50HZ);
                break;
                case MASK_DATARATE_100Hz:
                I2C_Peripheral_WriteRegister(LIS3DH_DEVICE_ADDRESS,
                                             LIS3DH_INT2_DURATION,              
                                             LIS3DH_INT2_DURATION_100HZ);
                break;
                case MASK_DATARATE_200Hz:
                I2C_Peripheral_WriteRegister(LIS3DH_DEVICE_ADDRESS,
                                             LIS3DH_INT2_DURATION,              
                                             LIS3DH_INT2_DURATION_200HZ);
                break;
                default:
                break;
    
            }



}
/* [] END OF FILE */
