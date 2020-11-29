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
    File Global.h
    Header file aimed to the declaration all the variables needed for the programming
    Authors: Garofalo Daniela, Pedica Benedetta and Sala Davide
*/
    
#ifndef __GLOBAL_H__
    #define __GLOBAL_H__
    
    // Defines related to the different states of the model 
    #define RUN                 1
    #define WAIT                -1
    
    // Defines related to the different states of the PushButton component
    #define BUTTON_PRESSED      0
    
    // Defines related to the different states of the OnBoardLED component
    #define ONBOARD_LED_ON      1
    #define ONBOARD_LED_OFF     0
    
    // Defines related to the different parameters that can be selected from the hardware menu
    #define ODR_ACC             0
    #define FS_ACC              1
    #define FLAG_UART           2
    
    // Defines related to the different steps of the configuration mode
    #define CM_ENTRY            -1
    #define CM_SETPARAMETERS    0
    #define CM_EXIT             1
    
    // Include related to all the components implemented into the actual project
    #include <project.h>
    
    // Include related to the ISR functions defined into the actual project
    #include <InterruptRoutines_TIMER.h>
    #include <InterruptRoutines_BUTTON.h>
    
    // Variable related to the state of the device --> START or STOP conditions
    extern uint8_t device_state;
    
    // Variables related to the time measurement
    extern uint8_t count_global;
    extern uint8_t count_button;
    extern uint8_t minutes;
    extern uint8_t hours;
    
    // Variable related to the identification of the DOUBLE CLICK condition for the PushButton component
    extern uint8_t count_clicks;
    
    /* Variable related to the selection of the different parameters that is possible to change in the configuration mode
    of the hardware menu */
    extern uint8_t parameter_selected;
    
    // Variable related to the parameters according to the potentiometer value
    extern uint8_t potentiometer_value;
    extern uint8_t parameter_value;
    
    // Flag variables
    extern uint8_t flag_isbuttonpressed;
    extern int8_t flag_configurationmode;
    extern uint8_t flag_sampling;
    extern uint8_t flag_blinking;
    
    // Declaration of the function that initialises all the variables
    void Device_Initialisation();
#endif

/* [] END OF FILE */
