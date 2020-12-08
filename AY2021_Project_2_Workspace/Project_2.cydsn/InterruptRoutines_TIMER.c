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
    count_button_press++;
    count_button_rel++;
    
    // Setting the time variables according to their values
    if (count_global == COUNTS_1_SECOND)
    {
        seconds++;
        count_global = 0;
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
    
    /* Sampling the ADC_DelSig component when the device state is entered into the CONFIGURATION 
    MODE with a frequency equal to 100 Hz */
    if (flag_configurationmode == CM_SETPARAMETERS)
    {
        // Reading of the ADC_DelSig output value
        flag_sampling = 1;
    }
    
    // Blinking of the OnBoardLED component in the CONFIGURATION MODE
    if (flag_blinking)
    {
        // Increment of the count variable
        count_blinking++;
        
        /* The variable count_blinking is equal to the TIMER_FREQUENCY/2 so the blinking frequency
        is 2Hz --> count_blinking is compared with 50 counts so this accounts for half of the 
        entire 1 second period */
        if (count_blinking >= (COUNTS_1_SECOND/2))
        {
            // Blinkin of the OnBoardLED component
            Pin_ONBOARD_LED_Write(!Pin_ONBOARD_LED_Read());
            
            // Reset the flag value to the initial condition
            count_blinking = 0;
        }
    }
}

/* [] END OF FILE */
