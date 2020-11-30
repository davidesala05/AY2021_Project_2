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

#include "InterruptRoutines_UART.h"

CY_ISR(Custom_UART_ISR){
    
    if(system_status == 1){
        
        if (UART_ReadRxStatus() == UART_RX_STS_FIFO_NOTEMPTY) {
            
            ch_received = UART_GetChar(); //The character's value is saved
            
            switch(ch_received){
                
                case 'B':
                case 'b':
                    flag_send_timestamps = 1; //State used in the main to start all the components
                    break;      //and switch ON the LED status of the UART
                
                case 'S':
                case 's':
                    flag_send_timestamps = 0; //State used in the main to stop all the components,
                    break;       //initialize the UART and switch OFF the LED status of the UART
                
                default:
                    break;
            }
        } 
    }
}

/* [] END OF FILE */
