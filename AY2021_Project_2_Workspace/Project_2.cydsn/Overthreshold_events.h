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
#ifndef __OVERTHRESHOLD_EVENTS_H_
    
    #define __OVERTHRESHOLD_EVENTS_H_
             
    #include "project.h"
    #include "cytypes.h"
    
    #define LIS3DH_INT2_THS_2G 0b01111101
    #define LIS3DH_INT2_THS_4G 0b00111110
    #define LIS3DH_INT2_THS_8G 0b00100000
    #define LIS3DH_INT2_THS_16G 0b00001011
    
    #define LIS3DH_INT2_DURATION_50HZ 0b00001010
    #define LIS3DH_INT2_DURATION_100HZ 0b00010100
    #define LIS3DH_INT2_DURATION_200HZ 0b00101000
    #define LIS3DH_INT2_DURATION_400HZ 0b01010000
   
    void Set_FS_Registers (void);
    
    void Set_Duration_Registers (void);

#endif
/* [] END OF FILE */
