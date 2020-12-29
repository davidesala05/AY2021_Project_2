/* ========================================
 *
 * Laboratorio di Tecnologie Elettroniche e Biosensori
 * Politecnico di Milano
 * AA 2020-2021 - I semester
 *
 * Final Projects:
 * Project 2 
 * Authors: Daniela Garofalo, Benedetta Pedica, Davide Sala
 * Date: 10/01/2021
 *
 * ========================================
*/

/*
*
*  \brief Hardware Menu source file
*
*  This file is aimed to manage the Hardware menu implemented with the potentiometer
*  
*/

#include "HardwareMenu.h"

//****** HM CONFIGURATION FUNCTION ******//
void Hardware_Menu(void)
{
    // Identification of the step of the CONFIGURATION MODE
    switch (configurationmode_state)
    {
        // Entering into the CONFIGURATION MODE
        case IDLE:
        
            if (flag_longpression == 1)
            {  
                // ENTRY and EXIT states are defined as opposite values (-1 and +1 respectively)
                configurationmode_state = CM_ENTRY;
                
                flag_longpression = 0;
            }

            /****** DOUBLE CLICK CONDITION ******/
            if (flag_doubleclick == 1) 
            {
                // Reset the flag variable to the initial condition
                flag_doubleclick = 0;
                
                // START and STOP values are defined as opposite values (+1 and -1 respectively)
                device_state = -device_state;

                // Control the value of the variable device_state
                if (device_state == RUN)
                {
                    // Entering in the RUN condition of the device
                    HM_Start();

                    // Switching ON the OnBoardLED component when the device is set to START condition
                    Pin_ONBOARD_LED_Write(ONBOARD_LED_ON);
                }
                else
                {
                    // Entering in the WAIT condition of the device
                    HM_Stop();

                    // Swtitching OFF the OnBoardLED component when the device is set to STOP condition
                    Pin_ONBOARD_LED_Write(ONBOARD_LED_OFF);
                }
            }
            break;
        
        
        case CM_ENTRY:
        
            HM_Stop();
            
            /* Switching the MUX component to the channel 1 --> CLOCK_BLINKING_RGB: this clock is 
            set in order to allow the correct visualisation of the blinking on the RGB LED when 
            the parameters are changed as a feedback for the user */

            Control_Reg_Write(MUX_CHANNEL_BLINKING);
            
            // Function used to reset the counter of the PWM to deal with the new clock
            Reset_PWM_for_CONF_MODE();

            // Start the ADC_DelSig sampling
            ADC_DelSig_Start();

            // Start blinking of the OnBoardLED component
            flag_blinking = 1;
            
            parameter_selected = FS_RANGE;

            // Next step of the CONFIGURATION MODE
            configurationmode_state = CM_SETPARAMETERS;
            break;
        

        // Sample the value of the potentiometer according to the selected parameter
        case CM_SETPARAMETERS:
        
            if(flag_sampling_pot == 1)
            {
            
                potentiometer_value = ADC_DelSig_Read16();
                
                // Control of the value assumed by the variable
                if (potentiometer_value > 255)  potentiometer_value = 255;
                if (potentiometer_value < 0)    potentiometer_value = 0;
            
            
                // Modification of the parameters
                Potentiometer_to_Register(parameter_selected, potentiometer_value);

                /* Setting of the feedback on the RGB LED according to the chosen parameter and to the measured value from the
                potentiometer */
                Set_Feedback(parameter_selected);

                //****** SINGLE CLICK CONDITION ******//
                if (flag_singleclick == 1)
                {
                    /* Incrementing the value of the variable parameter_selected cycling among the 
                    allowed possibilities */
                    parameter_selected++;
                    if (parameter_selected > VERBOSE_FLAG)
                    {
                        parameter_selected = FS_RANGE;
                    }
                    
                    // Reset the flag variable to the initial condition
                    flag_singleclick = 0;
                }
                
                //****** LONG PRESSION CONDITION *****//
                if (flag_longpression)
                {
                    // Reset the flag variable to the initial condition
                    flag_longpression = 0;
                    
                    // Next step of the the CONFIGURATION MODE
                    configurationmode_state = CM_EXIT;
                }
            
                //****** DOUBLE CLICK CONDITION *****//
                if (flag_doubleclick)
                {
                    // Reset the flag variable to the initial condition
                    flag_doubleclick = 0;
                }
                
                flag_sampling_pot = 0;  
            }
            break;
        

        // Exiting from the CONFIGURATION MODE
        case CM_EXIT:
        
            /* Switching the MUX component to the channel 1 --> CLOCK_COLOUR_RGB: this clock is 
            set in order to allow the correct visualisation of the colour on the RGB LED when the
            accelerometer measuring the acceleration values */
            Control_Reg_Write(MUX_CHANNEL_COLOUR);

            // Stop the ADC_DelSig sampling
            ADC_DelSig_Stop();

            // Stop blinking of the OnBoardLED component
            flag_blinking = 0;

            // Conversion of the variables for the registers into values
            Register_to_value();
            
            // Storage of the parameters on the internal EEPROM
            Save_Parameters_on_INTERNAL_EEPROM();
            
            /* Restoring the actual state of the device before entering into the CONFIGURATION 
            MODE */
            if (device_state == RUN)
            {
                // RUN condition of the device
                HM_Start();

                // Switching ON the OnBoardLED component when the device is set to START condition
                Pin_ONBOARD_LED_Write(ONBOARD_LED_ON);
            }
            else
            {
                
                HM_Stop();
                
                /* Swtitching OFF the OnBoardLED component when the device is set to STOP
                condition */
                Pin_ONBOARD_LED_Write(ONBOARD_LED_OFF);
            }
            
            configurationmode_state = IDLE;
            
            break;
        
        default:
            break;
        
    }
}



/* [] END OF FILE */
