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
    #define LONG_PRESSION_DURATION  2 * COUNTS_1_SECOND // 2 seconds of pression
    #define SINGLE_CLICK            1
    #define DOUBLE_CLICK            2
    
    /* Defines related to the different channels of the MUX component in order to link the correct clock signal to the PWM
    components according to the device state */
    #define MUX_CHANNEL_COLOUR      0
    #define MUX_CHANNEL_BLINKING    1
    
    // Include related to all the variables implemented into the actual project
    #include <Global.h>
    
    // Variables related to the type of pressing that it is allowed for the PushButton component
    uint8_t start_press;
    uint8_t start_release;
    uint8_t flag_error;
    uint8_t flag_singleclick;
    uint8_t flag_doubleclick;
    uint8_t flag_longpression;
    
    /* Declaration of the function that identifies the different type of pressing of the
    PushButton component --> SINGLE CLICK, DOUBLE CLICK or LONG PRESSION, taking into account
    also of possible configuration of pressings that are not associated to any implemented
    functionality of the device (SINGLE CLICK + LONG PRESSION, idenfitied by the flag_error
    variable) */
    void Pressing_Identification();
    
    /* Declaration of the function that manages the different options related to the type of 
    pressing allowed on the PushButton component --> change the device state among the possibilities:
    RUN, WAIT or CONFIGURATION MODE */
    void Hardware_Menu();
    
    /* Declaration of the functions that manage the components inside the CONFIGURATION MODE step
    of the hardware menu */
    void HM_Configuration();
    void HM_Start();
    void HM_Stop();
#endif

/* [] END OF FILE */
