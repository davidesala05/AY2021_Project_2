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
    File InterruptRoutines_TIMER.c
    Source file aimed to update the flag variable related to the Timer component
    Authors: Garofalo Daniela, Pedica Benedetta and Sala Davide
*/
    
#include <InterruptRoutines_TIMER.h>
    
CY_ISR(custom_TIMER_ISR)
{
    // Allowing the generation of other interrupts by reading the Status Register of the Timer component
    Timer_TIMESTAMP_ReadStatusRegister();
    
    // Increment count_global variable in order to detect the passing of time 
    count_global++;
    
    // Setting the time variables according to their values
    if (count_global == TIMER_FREQUENCY)
    {
        seconds++;
    }
    if (seconds == 60)
    {
        seconds = 0;
        minutes++;
    }
    if (minutes == 60)
    {
        minutes = 0;
        hours++;
    }
    if (hours == 24)
    {
        hours = 0;
    }
    
    // Sampling the ADC_DelSig component when the device state is entered into the CONFIGURATION MODE
    if (flag_configurationmode == CM_SETPARAMETERS && flag_sampling == 0)
    {
        // The variable count_global is a multiple of the TIMER_FREQUENCY/2 so the sampling frequency is 2Hz
        if (count_global % (TIMER_FREQUENCY/2) == 0) 
        {
            // Reading of the ADC_DelSig output value
            potentiometer_value = ADC_DelSig_Read8();
            
            // Control of the value assumed by the variable
            if (potentiometer_value > 255)  potentiometer_value = 255;
            if (potentiometer_value < 0)    potentiometer_value = 0;
        
            // Sampling completed
            flag_sampling = 1;
        }
    }
    
    // Blinking of the OnBoardLED component in the CONFIGURATION MODE
    if (flag_blinking)
    {
        // The variable count_global is a multiple of the TIMER_FREQUENCY/2 so the blnking frequency is 2Hz
        if (count_global % (TIMER_FREQUENCY/2) == 0)
        {
            // Blinkin of the OnBoardLED component
            Pin_ONBOARD_LED_Write(!Pin_ONBOARD_LED_Read());
        }
    }
}

/* [] END OF FILE */
