/* ========================================
 *
 * Laboratorio di Tecnologie Elettroniche e Biosensori
 * Politecnico di Milano
 * AA 2020/2021 - I semestre
 *
 * Progetti finali:
 * - Progetto 2 -
 * Autori: Daniela Garofalo, Benedetta Pedica, Davide Sala
 * Data: 10/01/2021
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
    
    /* Increment the counters in order to detect the time intervals which correspond to the
    pressing of the PushButton component and the distance between 2 consecutive pressings */
    count_button_press++;
    count_button_rel++;
    
    // Controlling that the number of counts corresponds to 1 second
    if (count_global == COUNTS_1_SECOND)
    {
        // Increment the number of seconds
        seconds++;
        
        // Initialisation of the variable to the initial condition
        count_global = 0;
    }
    // Controlling that 60 seconds has been passed
    if (seconds == 60)
    {
        // Increment the number of minutes
        minutes++;
        
        // Initialisation of the variable to the initial condition
        seconds = 0;
    }
    
    // Controlling that 60 minutes has been passed
    if (minutes == 60)
    {
        // Increment the number of hours
        hours++;
        
        // Initialisation of the variable to the initial condition
        minutes = 0;
    }
    
    // Controlling that 24 hours has been passed to reset the absolute timer
    if (hours == 24)
    {
        // Initialisation of the vatiable to the initial condition
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
            // Switching ON the OnBoardLED component
            Pin_ONBOARD_LED_Write(ONBOARD_LED_ON);
        }
        else{
            // Switching OFF the OnBoardLED component
            Pin_ONBOARD_LED_Write(ONBOARD_LED_OFF);
        }
    }
    
    /* Count used for the correct timing in the waveform plot and in the comunication with the 
    Python code --> the overthreshold event is represented with samples with the same frequency
    of the acquisition (this depends on the ODR bits value) */
    if(flag_send_waveform | flag_export_file)
    {
        // Increment the variable
        count_waveform ++;
    }
}


/******************************************/
/*            BUTTON_PRESS ISR            */
/******************************************/

CY_ISR(custom_BUTTON_PRESS_ISR)
{
    // Initialisation of the variable to the initial condition
    count_button_press = 0;
    
    // Detection of a fast click, which is a necessary condition for the DOUBLE CLICK
    if (flag_fastclick == 1)
    {
        /* Controlling the time interval between 2 consecutive pressings of the PushButton
        component */
        if (count_button_rel <= COUNTS_1_SECOND/2)
        {
            /* Identification of the condition of 2 consecutive pressings with a short time
            distance one another */
            flag_shortdistance = 1;
            
            /* Initialisation of the variables to the initial condition in order to properly
            detect the DOUBLE CLICK condition, determined by two fast clicks with a short interval
            in between */
            flag_fastclick = 0;
        }
        else
        {
            // Initialisation of the variable to the initial condition
            flag_shortdistance = 0;
        }
    }
}


/******************************************/
/*            BUTTON_REL ISR              */
/******************************************/

CY_ISR(custom_BUTTON_REL_ISR)
{
    // Initialisation of the variable to the initial condition 
    count_button_rel = 0;
    
    // Identification of the occurrence of a LONG PRESSION (greater or equal than 2 seconds)
    if (count_button_press >= COUNTS_1_SECOND*2)
    {
        /****** LONG PRESSION ******/
        flag_longpression = 1;
        
        // Initialisation of the variables to the initial condition
        flag_shortdistance = 0;
        flag_fastclick = 0;
        flag_singleclick = 0;
        
    // Identification of the occurrence of a fast click (less or equal than 0.25 seconds)
    } else if (count_button_press <= COUNTS_1_SECOND/4)
    {
        // Fast click
        flag_fastclick = 1;
        
        // Initialisation of the variables to the initial condition
        flag_longpression = 0;
        flag_singleclick = 0;
        
    // Identification of the occurrence of a SINGLE CLICK (less or equal than 1 second)
    } else if (count_button_press <= COUNTS_1_SECOND)
    {
        /****** SINGLE CLICK ******/
        flag_singleclick = 1;
        
        // Initialisation of the variables to the initial condition
        flag_fastclick = 0;
        flag_longpression = 0;
        flag_shortdistance = 0;
    }
    
    /* Identification of the occurrence of 2 successive fast clicks separated by a short time 
    interval --> conditions for a DOUBLE CLICK */
    if(flag_shortdistance == 1 && flag_fastclick == 1)
    {
        /****** DOUBLE CLICK ******/
        flag_doubleclick = 1;
        
        // Initialisation of the variables to the initial condition
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
    // Controlling the state of the device
    if(device_state == WAIT)
    { 
        // Controlling that an unread data is available
        if (UART_ReadRxStatus() == UART_RX_STS_FIFO_NOTEMPTY)
        {
            // Saving the character received through the UART protocol
            ch_received = UART_GetChar();
            
            // Controlling the value of the character received
            switch(ch_received)
            {
                // Plotting the waveforms of the overthreshold events
                case 'B':
                case 'b':
                    flag_send_waveform = 1;
                    break;
                
                // Stopping the transmission of the information saved
                case 'S':
                case 's':
                    flag_send_waveform = 0;
                    flag_send_timestamps = 0;
                    break;
                
                // Sending the timestamps information
                case 'T':
                case 't':
                    flag_send_timestamps = 1;
                    break;
                
                /* Exporting the information about the overthreshold events (waveform and
                timestamps through UART in order to create a CSV file) */
                case 'W':
                case 'w':
                    flag_export_file = 1;
                    break;
                
                // Doing nothing when other characters are received
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
    // Sampling the accelerometer
    flag_ACC = 1; //Used in the main.c to sampling or for the overthreshold event detection
    
    // Restore the initial condition in order to allow the generation of other interrupts
    Pin_INT_AC_ClearInterrupt();
}

/* [] END OF FILE */
