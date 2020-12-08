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

/*
    File InterruptRoutines_BUTTON.h
    Header file aimed to the declaration of the ISR related to the PushButton component
    Authors: Garofalo Daniela, Pedica Benedetta and Sala Davide
*/
    
#ifndef __INTERRUPTROUTINES_BUTTON_H__
    #define __INTERRUPTROUTINES_BUTTON_H__
    
    // Include related to all the variables implemented into the actual project
    #include <Global.h>
    
    // Include related to all the cytypes functions 
    #include <cytypes.h>
    
    // Declaration of the ISR
    CY_ISR_PROTO(custom_BUTTON_PRESS_ISR);
    CY_ISR_PROTO(custom_BUTTON_REL_ISR);
#endif

/* [] END OF FILE */