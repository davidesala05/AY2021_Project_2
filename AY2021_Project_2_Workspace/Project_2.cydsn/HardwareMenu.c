/* ========================================
 *
 * Laboratorio di Tecnologie Elettroniche e Biosensori
 * Politecnico di Milano
 * AY 2020/2021 - I semester
 *
 * Final Projects:
 * - Project 2 - 
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

// Include related to the header file related to the functions defined in this file
#include "HardwareMenu.h"

//****** HM CONFIGURATION FUNCTION ******//
void Hardware_Menu(void)
{
    // Identification of the step of the CONFIGURATION MODE
    switch (configurationmode_state)
    {
        /* The device is outside from the configuration mode, so it is sensible to a LONG
        PRESSURE condition that allows the entering into this modality and to a DOUBLE CLICK
        condition which permits to change the device state from RUN to WAIT and vice versa */
        case IDLE:
            // Identification of a LONG PRESSION 
            if (flag_longpression == 1)
            {  
                // Entering into the configuration mode of the device
                configurationmode_state = CM_ENTRY;
                
                // Reset the flag variable to the initial condition
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

                    /* Switching ON the OnBoardLED component when the device is set to START 
                    condition */
                    Pin_ONBOARD_LED_Write(ONBOARD_LED_ON);
                }
                else
                {
                    // Entering in the WAIT condition of the device
                    HM_Stop();

                    /* Swtitching OFF the OnBoardLED component when the device is set to STOP
                    condition */
                    Pin_ONBOARD_LED_Write(ONBOARD_LED_OFF);
                }
            }
            break;
        
        /* The device is entering in the configuration mode, so it is necessary to do some
        operations before giving the user the possibility to modify the parameters value in
        order to guarantee the proper functioning of the device 
        First of all, it is needed to stop all the components which are not required in the
        configuration mode; secondly the correct clock source must be connected to the PWM
        components for the blinking of the RGB LED; suddenly the PWM components are reset
        to their initial condition so they will be able to generate the proper feedback to the
        user; finally, the ADC_DelSig component is activated in order to sample the potentiometer
        outputs defining the paramters value */
        case CM_ENTRY:
            // Stopping the unused components of the device
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
            
            // Initialisation of the parameter selected
            parameter_selected = FS_RANGE;

            // Next step of the CONFIGURATION MODE
            configurationmode_state = CM_SETPARAMETERS;
            break;

        /* The device is effectively in the configuration mode, so it is necessary to sample 
        the value of the potentiometer according to the selected parameter; in this case, the
        device is sensible to the occurrence of a LONG PRESSION, which is necessary to exit
        from the configuration mode and returning to the current state of the device (RUN or
        WAIT after having updated the parameters both in terms of variable and in terms of 
        stored information inside the internal EEPROM) and to a SINGLE CLICK in order to change
        the parameter considered */
        case CM_SETPARAMETERS:
            /* Identification of the correct timing to sample the potentiometer with the
            given frequency equal to 100 Hz */
            if(flag_sampling_pot == 1)
            {
                // Reading the output of the ADC_DelSig and saving it inside a variable
                potentiometer_value = ADC_DelSig_Read16();
                
                // Control of the value assumed by the variable to rebound it if needed
                if (potentiometer_value > 255)  potentiometer_value = 255;
                if (potentiometer_value < 0)    potentiometer_value = 0;
           
                // Modification of the parameters according to the potentiometer value
                Potentiometer_to_Register(parameter_selected, potentiometer_value);

                /* Setting of the feedback on the RGB LED according to the chosen parameter and
                to the measured value from the potentiometer */
                Set_Feedback(parameter_selected);

                //****** SINGLE CLICK CONDITION ******//
                if (flag_singleclick == 1)
                {
                    /* Incrementing the value of the variable parameter_selected cycling among 
                    the allowed possibilities */
                    parameter_selected++;
                    
                    // Cycling among the allowed possibilities
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
                // Doing nothing when a double click condition is identified
                if (flag_doubleclick)
                {
                    // Reset the flag variable to the initial condition
                    flag_doubleclick = 0;
                }
                
                // Reset the flag variable to the initial condition
                flag_sampling_pot = 0;  
            }
            break;
       
        /* The device is exiting from the configuration mode, so it is necessary to do some
        operations before restoring the current state of the device (RUN or WAIT mode,
        depending on in which modality were the device before entering in the configuration
        mode)
        First of all, it is needed to start again all the components which are not required in 
        the configuration mode if the device were in the RUN mode before entering in the
        configuration mode; secondly the correct clock source must be connected to the PWM
        components for controlling the intensity of the RGB LED; suddenly the PWM components 
        are reset to their initial condition so they will be able to generate the proper feedback
        to the user; finally, the ADC_DelSig component is stopped */
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

                /* Switching ON the OnBoardLED component when the device is set to 
                RUN condition */
                Pin_ONBOARD_LED_Write(ONBOARD_LED_ON);
            }
            else
            {
                // WAIT condition of the device
                HM_Stop();
                
                /* Swtitching OFF the OnBoardLED component when the device is set to
                WAIT condition */
                Pin_ONBOARD_LED_Write(ONBOARD_LED_OFF);
            }
            
            // Reset the state variable to the initial condition 
            configurationmode_state = IDLE;
            
            break;
        
        default:
            break;
        
    }
} // Hardware_Menu()

/* [] END OF FILE */
