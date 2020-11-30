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
    
    Start_Components();
    
    CyDelay(100); //"The boot procedure is complete about 5 milliseconds after device power-up."
    
    Register_Initialization();
    
    CyDelay(100);
    

    ErrorCode error;
    
    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    
    Sensitivity = 1;
    FS_range = 2;
    
    Buffer[0] = HEADER;
    Buffer[TRANSMIT_BUFFER_SIZE-1] = TAIL;

    for(;;)
    {
        
        
        /******************************************/
        /*                SAMPLING                */
        /******************************************/
        
        if (flag_ACC == 1){
            
            /*
            The new data are read by the contiguous
            registers and saved in the variable
            */
            error = I2C_Peripheral_ReadRegisterMulti(LIS3DH_DEVICE_ADDRESS,
                                                     OUT_X_L,//0x0000+32*count_isr_over_event
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
            
            /******************************************/
            /*              VERBOSE FLAG              */
            /******************************************/
            
            if(Verbose_flag == 1){
                
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
            
            flag_ACC = 0;
        
        }
    }
}

/* [] END OF FILE */
