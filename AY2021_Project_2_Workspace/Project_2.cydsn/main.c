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

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */
    
    I2C_Peripheral_Start();
    UART_Start();
    isr_ACC_StartEx(Custom_ACC_ISR);
    PWM_RG_Start();
    PWM_B_Start();
    
    CyDelay(100); //"The boot procedure is complete about 5 milliseconds after device power-up."
    
    Register_Initialization();
    
    CyDelay(100);
    

    ErrorCode error;
    
    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    
    Sensitivity = 1;
    FS_range = 2;

    for(;;)
    {
        /* Place your application code here. */
        
        
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
            
            UART_PutString("ciao");
            
            flag_ACC = 0;
            
            
            
        
        }
    }
}

/* [] END OF FILE */
