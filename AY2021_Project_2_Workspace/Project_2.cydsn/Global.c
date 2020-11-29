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
    File Global.c
    Source file aimed to the initialisation of all the variables needed for the programming
    Authors: Garofalo Daniela, Pedica Benedetta and Sala Davide
*/
    
#include <Global.h>

void Device_Initialisation()
{
    // Initialisation of the variables
    device_state = WAIT;
    count_global = 0;
    count_button = 0;
    minutes = 0;
    hours = 0;
    count_clicks = 0;
    parameter_selected = ODR_ACC;
    potentiometer_value = 0;
    parameter_value = 0;
    
    // Initialisation of the flags
    flag_isbuttonpressed = 0;
    flag_configurationmode = CM_EXIT;
    flag_sampling = 0;
    flag_blinking = 0;
    
    // Initialisation of the components
    ADC_DelSig_Init();
    EEPROM_INTERNAL_Start();
    I2C_Master_Init();
    Pin_ONBOARD_LED_Write(ONBOARD_LED_OFF);
    PWM_RG_Init();
    PWM_B_Init();
    Timer_TIMESTAMP_Init();
    UART_Init();
    
    // Declaration of the ISR functions
    isr_BUTTON_StartEx(custom_BUTTON_ISR);
    isr_TIMESTAMP_StartEx(custom_TIMER_ISR);
}

/* [] END OF FILE */
