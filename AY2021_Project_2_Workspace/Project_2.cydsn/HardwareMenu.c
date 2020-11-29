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
        
        // Waiting until the release of the PushButton component in order to measure the time of the pressing
        while(Pin_BUTTON_Read() == BUTTON_PRESSED);
        
        // LONG PRESSION condition: 2 seconds of duration --> selection of the CONFIGURATION MODE
        if (count_button >= LONG_PRESSION_DURATION)
        {
            // ENTRY and EXIT states are defined as opposite values (-1 and +1 respectively)
            flag_configurationmode = -flag_configurationmode;
            
            // Reset the count variables to the initial condition
            count_button = 0;
            count_clicks = 0;
            
            // Entering into the CONFIGURATION MODE of the hardware menu
            HM_Configuration();
        }
        
        // DOUBLE CLICK condition: 2 clicks within 1 second of time --> selection of the START/STOP MODE
        if (count_button == 0 && count_clicks == DOUBLE_CLICK)
        {
            // START and STOP values are defined as opposite values (+1 and -1 respectively)
            device_state = -device_state; 
            
            // Reset the count variables to the initial condition
            count_button = 0;
            count_clicks = 0;
            
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
}

void HM_Stop()
{
    // Stopping the components
    I2C_Master_Stop();
    PWM_RG_Stop();
    PWM_B_Stop();
    Timer_TIMESTAMP_Stop();
    UART_Stop();
}

void HM_Configuration()
{
    // Identification of the step of the CONFIGURATION MODE
    switch (flag_configurationmode)
    {
        // Entering into the CONFIGURATION MODE
        case CM_ENTRY:
        {
            // Stop the components of the device
            HM_Stop();
            
            // Start the ADC_DelSig sampling
            ADC_DelSig_Start();
            
            // Start blinking of the OnBoardLED component
            flag_blinking = 1;
            
            // Next step of the CONFIGURATION MODE
            flag_configurationmode = CM_SETPARAMETERS;
        }
        
        // Sample the value of the potentiometer according to the selected parameter
        case CM_SETPARAMETERS:
        {
            // Sampling the potentiometer
            if (flag_sampling)
            {
                // Reset the value of the flag_sampling variable
                flag_sampling = 0;
                
                // Detect the parameter to be modified by the user
                switch (parameter_selected)
                {
                    case ODR_ACC:
                    {
                        
                    }
                    case FS_ACC:
                    {
                        
                    }
                    case FLAG_UART:
                    {
                        
                    }
                }
            }
            
            // Detecting the pressing of the PushButton component
            if (Pin_BUTTON_Read() == BUTTON_PRESSED)
            {
                // LONG PRESSION condition: 2 seconds of duration --> exiting from the CONFIGURATION MODE
                if (count_button >= LONG_PRESSION_DURATION)
                {
                    // Waiting until the release of the PushButton component in order to measure the time of the pressing
                    while(Pin_BUTTON_Read() == BUTTON_PRESSED);
                    
                    // Reset the count variables to the initial condition
                    count_button = 0;
                    count_clicks = 0;
                
                    // Reset the value of the flag_configurationmode 
                    flag_configurationmode = CM_EXIT;
                }
                
                // SINGLE CLICK condition: changing the parameter to be set by the user through the potentiometer
                if (count_button == 0 && count_clicks == SINGLE_CLICK)
                {
                    // Incrementing the value of the variable parameter_selected cycling among the allowed possibilities
                    parameter_selected++;
                    if (parameter_selected > FLAG_UART)
                    {
                        parameter_selected = ODR_ACC;
                    }
                }
            }
        }
        
        // Exiting from the CONFIGURATION MODE
        case CM_EXIT:
        {
            // Stop the ADC_DelSig sampling
            ADC_DelSig_Stop();
            
            // Stop blinking of the OnBoardLED component
            flag_blinking = 0;
            
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
        }
    }
}

/* [] END OF FILE */
