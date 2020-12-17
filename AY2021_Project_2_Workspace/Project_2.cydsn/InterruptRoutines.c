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

#include "InterruptRoutines.h"

/******************************************/
/*               TIMER ISR                */
/******************************************/
    
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
    if (configurationmode_state == CM_SETPARAMETERS)
    {
        // Reading of the ADC_DelSig output value
        flag_sampling_pot = 1;
    }
    
    // Blinking of the OnBoardLED component in the CONFIGURATION MODE
    if (flag_blinking == 1)
    {
        /* The variable count_global is a multiple of the TIMER_FREQUENCY/2 so the blinking frequency
        is 2Hz --> count_global is compared with 50 counts so this accounts for half of the entire
        1 second period */
        if (count_global >= (COUNTS_1_SECOND/2))
        {
            // Blinkin of the OnBoardLED component
            Pin_ONBOARD_LED_Write(ONBOARD_LED_ON);
        }
        else{
            Pin_ONBOARD_LED_Write(ONBOARD_LED_OFF);
        }
    }
    
    if(flag_send_waveform){
        count_waveform ++;
    }
}

/******************************************/
/*            BUTTON_PRESS ISR            */
/******************************************/

CY_ISR(custom_BUTTON_PRESS_ISR)
{
    count_button_press = 0;
    if (flag_fastclick == 1)
    {
        if (count_button_rel <= COUNTS_1_SECOND/2)
        {
            flag_shortdistance = 1;
            flag_fastclick = 0;
        }
        else
        {
            flag_shortdistance = 0;
        }
    }
}

/******************************************/
/*            BUTTON_REL ISR              */
/******************************************/

CY_ISR(custom_BUTTON_REL_ISR)
{
    count_button_rel = 0;
    if (count_button_press >= COUNTS_1_SECOND*2)
    {
        flag_longpression = 1;
        flag_shortdistance = 0;
        flag_fastclick = 0;
        flag_singleclick = 0;
    } else if (count_button_press <= COUNTS_1_SECOND/4)
    {
        flag_fastclick = 1;
        flag_longpression = 0;
        flag_singleclick = 0;
        
    } else if (count_button_press <= COUNTS_1_SECOND)
    {
        flag_singleclick = 1;
        flag_fastclick = 0;
        flag_longpression = 0;
        flag_shortdistance = 0;
    }
    if(flag_shortdistance == 1 && flag_fastclick == 1)
    {
        flag_doubleclick = 1;
        flag_longpression = 0;
        flag_fastclick = 0;
        flag_shortdistance = 0;
    }
}

/******************************************/
/*                UART ISR                */
/******************************************/

CY_ISR(Custom_UART_ISR)
{
    
    if(device_state == WAIT){ //To be changed according to Daniela's code
        
        if (UART_ReadRxStatus() == UART_RX_STS_FIFO_NOTEMPTY) {
            
            ch_received = UART_GetChar(); //The character's value is saved
            
            switch(ch_received){
                
                case 'B':
                case 'b':
                    flag_send_waveform = 1;
                    break;
                
                case 'S':
                case 's':
                    flag_send_waveform = 0;
                    flag_send_timestamps = 0;
                    break;
                
                case 'T':
                case 't':
                    flag_send_timestamps = 1;
                    break;
                
                case 'W':
                case 'w':
                    flag_export_file = 1;
                    break;
                
                default:
                    break;
            }
        } 
    }
}

/******************************************/
/*                ACC ISR                 */
/******************************************/

CY_ISR(Custom_ACC_ISR)
{

    flag_ACC = 1; //Used in the main.c to sampling or for the overthreshold event detection
    
    Pin_INT_AC_ClearInterrupt(); //necessary for the correct use of isr linked to the pin
}


/* [] END OF FILE */
