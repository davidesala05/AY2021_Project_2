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
    File HardwareMenu.c
    Source file aimed to manage the hardware menu implemented with the potentiometer
    Authors: Garofalo Daniela, Pedica Benedetta and Sala Davide
*/

#include <HardwareMenu.h>

void Hardware_Menu()
{
    // Detect the type of pressing of the PushButton component: DOUBLE CLICK or LONG PRESSION
    if (flag_isbuttonpressed)
    {
        // Reset the flag variable to the initial condition
        flag_isbuttonpressed = 0;

        // Store the value of the count_global when the PushButton component is present in order to evaluate the pressing time
        start_press = count_global;
        
        if (count_clicks == 1)
        {
            if (count_global - start_press_DB <= (TIMER_FREQUENCY/2)) 
            {
                count_clicks = 0;
                flag_second_click = 1;
                start_press = count_global;
            }
        }

        // Waiting until the release of the PushButton component
        while(Pin_BUTTON_Read() == BUTTON_PRESSED);
        
        if (flag_second_click == 1 && abs(count_global - start_press) <= LONG_PRESSION_DURATION)
        {
            flag_DB = 1;
        }
        
        if (count_clicks == 0)
        {
            // Istante in cui rilascio il bottone al primo click
            start_press_DB = count_global;
        }

        /* LONG PRESSION condition: 2 seconds of duration from the pressing of the PushButton component --> selection of the
        CONFIGURATION MODE */
        if (abs(count_global - start_press) >= LONG_PRESSION_DURATION)
        {
            // ENTRY and EXIT states are defined as opposite values (-1 and +1 respectively)
            flag_configurationmode = -flag_configurationmode;

            // Reset the count variables to the initial condition
            start_press = 0;
            count_clicks = 0;

            // Entering into the CONFIGURATION MODE of the hardware menu
            HM_Configuration();
        }

        // DOUBLE CLICK condition: 2 clicks within 1 second of time --> selection of the START/STOP MODE
        if (count_global - start_press <= (TIMER_FREQUENCY/2)) 
        {
            // Recognition of a click with a duration lower than 0.5 second
            count_clicks++;
        }
        
        if (flag_DB)
        {
            // Reset the count variables to the initial condition
            start_press = 0;
            count_clicks = 0;
            
            // START and STOP values are defined as opposite values (+1 and -1 respectively)
            device_state = -device_state;

            // Control the value of the variable device_state
            if (device_state == RUN)
            {
                // Entering in the RUN condition of the device
                HM_Start();

                // Switching ON the OnBoardLED component when the device is set to START condition
                Pin_ONBOARD_LED_Write(ONBOARD_LED_ON);
            } else {
                // Entering in the WAIT condition of the device
                HM_Stop();

                // Swtitching OFF the OnBoardLED component when the device is set to STOP condition
                Pin_ONBOARD_LED_Write(ONBOARD_LED_OFF);
            }
        }
    }
}

void HM_Start()
{
    // Starting the components
    I2C_Master_Start();
    PWM_RG_Start();
    PWM_B_Start();
    Timer_TIMESTAMP_Start();
    UART_Start();

    /* Controllare che i bit ODR siano diversi da zero, altrimenti
    l'accelerometro non inizierà in ogni caso a campionare i propri
    valori */
}

void HM_Stop()
{
    // Stopping the components
    I2C_Master_Stop();
    PWM_RG_Stop();
    PWM_B_Stop();
    Timer_TIMESTAMP_Stop();
    UART_Stop();

    /* Settare i bit ODR nella confiurazione 'power down mode' per
    interrompere il campionamento di nuovi dati da parte
    dell'accelerometro */
}

void HM_Configuration()
{
    // Identification of the step of the CONFIGURATION MODE
    switch (flag_configurationmode)
    {
        // Entering into the CONFIGURATION MODE
        case CM_ENTRY:
        {
            /* Switching the MUX component to the channel 1 --> CLOCK_BLINKING_RGB: this clock is set in order to allow the
            correct visualisation of the blinking on the RGB LED when the parameters are changed as a feedback for the user */
            Control_Reg_Write(MUX_CHANNEL_BLINKING);

            // Stop the components of the device
            HM_Stop();

            // Start the ADC_DelSig sampling
            ADC_DelSig_Start();

            // Start blinking of the OnBoardLED component
            flag_blinking = 1;

            // Next step of the CONFIGURATION MODE
            flag_configurationmode = CM_SETPARAMETERS;

            break;
        }

        // Sample the value of the potentiometer according to the selected parameter
        case CM_SETPARAMETERS:
        {
            // Modification of the parameters
            Potentiometer_to_Register(parameter_selected, potentiometer_value);

            /* Setting of the feedback on the RGB LED according to the chosen parameter and to the measured value from the
            potentiometer */
            Set_Feedback(parameter_selected, potentiometer_value);

            // Detect the type of pressing of the PushButton component: SINGLE CLICK or LONG PRESSION
            if (flag_isbuttonpressed)
            {
                // Reset the flag variable to the initial condition
                flag_isbuttonpressed = 0;

                // Store the value of the count_global when the PushButton component is present in order to evaluate the pressing time
                start_press = count_global;

                // Waiting until the release of the PushButton component
                while(Pin_BUTTON_Read() == BUTTON_PRESSED);

                /* LONG PRESSION condition: 2 seconds of duration from the pressing of the PushButton component --> selection of the
                CONFIGURATION MODE */
                if (abs(count_global - start_press) >= LONG_PRESSION_DURATION)
                {
                    // Reset the count variables to the initial condition
                    start_press = 0;
                    count_clicks = 0;

                    // Reset the value of the flag_configurationmode
                    flag_configurationmode = CM_EXIT;
                }

                // SINGLE CLICK condition: changing the parameter to be set by the user through the potentiometer
                if (abs(count_global - start_press) <= TIMER_FREQUENCY && count_clicks == SINGLE_CLICK)
                {
                    // Reset the count variables to the initial condition
                    start_press = 0;
                    count_clicks = 0;
                    
                    // Incrementing the value of the variable parameter_selected cycling among the allowed possibilities
                    parameter_selected++;
                    if (parameter_selected > VERBOSE_FLAG)
                    {
                        parameter_selected = FS_RANGE;
                    }
                }
            }
            break;
        }

        // Exiting from the CONFIGURATION MODE
        case CM_EXIT:
        {
            /* Switching the MUX component to the channel 1 --> CLOCK_COLOUR_RGB: this clock is set in order to allow the
            correct visualisation of the colour on the RGB LED when the parameters are changed as a feedback for the user */
            Control_Reg_Write(MUX_CHANNEL_COLOUR);

            // Stop the ADC_DelSig sampling
            ADC_DelSig_Stop();

            // Stop blinking of the OnBoardLED component
            flag_blinking = 0;

            // Conversion of the variables for the registers into values
            Register_to_value();
            
            // Storage of the parameters on the internal EEPROM
            ???

            // Restoring the actual state of the device before entering into the CONFIGURATION MODE
            if (device_state == RUN)
            {
                // RUN condition of the device
                HM_Start();

                // Switching ON the OnBoardLED component when the device is set to START condition
                Pin_ONBOARD_LED_Write(ONBOARD_LED_ON);
            } else {
                // Swtitching OFF the OnBoardLED component when the device is set to STOP condition
                Pin_ONBOARD_LED_Write(ONBOARD_LED_OFF);
            }
            
            // Reset the flag variable in order to enter again into the CONFIGURATION MODE 
            flag_configurationmode = CM_ENTRY;
        }
        break;

        // Default condition --> the variable assumes a value which is not considered into the switch case
        default:
        {
            /* Eventualmente si potrebbe segnalare una condizione di errore
            avvenuta all'interno degli step di entrata e di uscita del menù
            configurazione + reinizializzare il dispositivo alla condizione
            di prima accensione per resettarlo */
            break;
        }
    }
}

/* [] END OF FILE */
