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

    // Define related to the frequency of interrupt generation related to the Timer component
    #define COUNTS_1_SECOND     100 
    
    // Defines related to the different states of the device
    #define RUN                 1
    #define WAIT                -1
    
    // Defines related to the different states of the PushButton component
    #define BUTTON_PRESSED      0
    #define BUTTON_RELEASED     1
    
    // Defines related to the different states of the OnBoardLED component
    #define ONBOARD_LED_ON      1
    #define ONBOARD_LED_OFF     0
    
    // Defines related to the different parameters that can be selected from the hardware menu
    #define FS_RANGE            0
    #define DATARATE            1
    #define VERBOSE_FLAG        2       
    
    // Defines related to the different steps of the configuration mode
    #define CONFIGURATION_MODE  0
    #define IDLE                1
    
    // Include related to all the components implemented into the actual project
    #include <project.h>
    #include <stdlib.h>
    
    // Include related to the ISR functions defined into the actual project
    #include <InterruptRoutines_TIMER.h>
    #include <InterruptRoutines_BUTTON.h>
    
    // Variable related to the state of the device --> START or STOP conditions
    extern int8_t device_state; /* Defined as integer (and not as uint) because it can assume both
    positive and negative values: RUN and WAIT are opposite values */
    
    /* Variables related to the time measurement --> InterruptRoutines_BUTTON and 
    InterruptRoutines_TIMER */
    extern uint8_t count_global;
    extern uint8_t seconds;
    extern uint8_t minutes;
    extern uint8_t hours;
    extern uint16_t count_button_press;
    extern uint16_t count_button_rel;
    extern uint8_t count_blinking;
    
    /* Variable related to the selection of the different parameters that is possible to change in the configuration mode
    of the hardware menu */
    extern uint8_t parameter_selected;
    
    // Variable related to the parameters according to the potentiometer value
    extern int16_t potentiometer_value;
    extern uint8_t FS_range_reg;
    extern uint8_t DataRate_reg;
    extern uint8_t Verbose_flag;
    
    // Flag variables 
    extern uint8_t flag_configurationmode; /* Defined as integer (and not as uint) because it can 
    assume both positive and negative values: CM_ENTRY and CM_EXIT are opposite values */
    extern uint8_t flag_sampling_pot;
    extern uint8_t flag_blinking;
    
    // Variables related to the type of pressing that it is allowed for the PushButton component
    extern uint8_t flag_singleclick;
    extern uint8_t flag_doubleclick;
    extern uint8_t flag_longpression;
    extern uint8_t flag_shortdistance;
    extern uint8_t flag_fastclick;
    
    // Declaration of the function that initialises all the variables
    void Device_Initialisation();
    
    
    
    //******* DAVIDE ******//
    //MAX and MIN possible duty cycle for the RGB LED
    #define DC_100      65535
    #define DC_0        0
    
    //DUTY CYCLES used in the FEEDBACK (clock = 100Hz)
    #define PERIOD_1Hz      99
    #define PERIOD_2Hz      49
    #define PERIOD_5Hz      19
    #define PERIOD_10Hz      9
    
    //MASKS used in the Register_to_value function
    //FS_RANGE conversion
    
    #define MASK_FS_RANGE_2G  0b00000000
    #define MASK_FS_RANGE_4G  0b00000001
    #define MASK_FS_RANGE_8G  0b00000010
    #define MASK_FS_RANGE_16G 0b00000011
    
    //DATARATE conversion
    #define MASK_DATARATE_0Hz   0b00000000
    #define MASK_DATARATE_50Hz  0b00000100
    #define MASK_DATARATE_100Hz 0b00000101
    #define MASK_DATARATE_200Hz 0b00000110
    
    void Potentiometer_to_Register(uint8_t parameter, int16_t value);
    void Set_Feedback(uint8_t parameter);
#endif

/* [] END OF FILE */
