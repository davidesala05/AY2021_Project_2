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

#ifndef __INTERRUPTROUTINES_H__
    #define __INTERRUPTROUTINES_H__
    
    // Include related to all the variables implemented into the actual project
    #include "Global.h"
    #include "Functions_EVENTS.h"
    
    // Include related to all the cytypes functions 
    #include "cytypes.h"
    
    // Declaration of the ISRs
    
    CY_ISR_PROTO(custom_TIMER_ISR);
    
    CY_ISR_PROTO(custom_BUTTON_PRESS_ISR);
    
    CY_ISR_PROTO(custom_BUTTON_REL_ISR);
    
    CY_ISR_PROTO(Custom_UART_ISR);
    
    CY_ISR_PROTO(Custom_ACC_ISR);
    
#endif

/* [] END OF FILE */
