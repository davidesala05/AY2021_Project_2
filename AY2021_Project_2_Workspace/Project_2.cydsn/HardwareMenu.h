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
    File HardwareMenu.h
    Header file aimed to the declaration of the functions needed to manage the hardware menu build with the potentiometer
    Authors: Garofalo Daniela, Pedica Benedetta and Sala Davide
*/
    
#ifndef __HARDWAREMENU_H__
    #define __HARDWAREMENU_H__
    
    // Defines related to the different conditions of the PushButton component when it is pressed
    #define LONG_PRESSION_DURATION  2 * TIMER_FREQUENCY // 2 seconds of pression
    #define SINGLE_CLICK            1
    #define DOUBLE_CLICK            2
    
    /* Defines related to the different channels of the MUX component in order to link the correct clock signal to the PWM
    components according to the device state */
    #define MUX_CHANNEL_COLOUR      0
    #define MUX_CHANNEL_BLINKING    1
    
    // Include related to all the variables implemented into the actual project
    #include <Global.h>
    
    /* Declaration of the function that enters in a specific branch of the menu according to the type of pressing of the
    PushButton component: LONG PRESSION or DOUBLE CLICK conditions */
    void Hardware_Menu();
    
    // Declaration of the functions that manage the components according to the device state
    void HM_Start();
    void HM_Stop();
    void HM_Configuration();
#endif

/* [] END OF FILE */
