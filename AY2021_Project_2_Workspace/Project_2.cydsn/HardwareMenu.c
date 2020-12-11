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

//****** HM_START FUNCTION ******// 
void HM_Start(void)
{
    /* Switching the MUX component to the channel 1 --> CLOCK_COLOUR_RGB: this clock is 
    set in order to allow the correct visualisation of the colour on the RGB LED when the
    accelerometer measuring the acceleration values */
    //Control_Reg_Write(MUX_CHANNEL_COLOUR);
    
    // Stopping the ADC_DelSig
    ADC_DelSig_Stop();

    // Stop blinking of the OnBoardLED component
    flag_blinking = 0;
    
    // Reset the flag value to the initial condition
    count_blinking = 0;

    // Conversion of the variables for the registers into values
    //Register_to_value();
    
    // Storage of the parameters on the internal EEPROM
    //Save_Parameters_on_INTERNAL_EEPROM();

    /* Restoring the actual state of the device before entering into the CONFIGURATION 
    MODE */
    if (device_state == RUN)
    {
        // Switching ON the OnBoardLED component when the device is set to START condition
        Pin_ONBOARD_LED_Write(ONBOARD_LED_ON);
    } else {
        // Swtitching OFF the OnBoardLED component when the device is set to STOP condition
        Pin_ONBOARD_LED_Write(ONBOARD_LED_OFF);
    }
    
//    reg = LIS3DH_CTRL_REG1_INIT | (MASK_DATARATE_50Hz << 4);
//   
//    ErrorCode error = I2C_Peripheral_WriteRegister(LIS3DH_DEVICE_ADDRESS,
//                                                   LIS3DH_CTRL_REG1,
//                                                   reg);
//    if(error == ERROR){
//        UART_PutString("Error occurred during I2C comm\r\n");  
//    }
//    Register_Initialization_after_Overth_Event();
//    
//    Initialize_Parameters();
//    
//    count_overth_event = 0;
    
    // Initialisation of the variables to their initial condition 
    hours = 0;
    minutes = 0;
    seconds = 0;
    count_global = 0;
    
    PWM_RG_WriteCounter(DC_100);
    PWM_B_WriteCounter(DC_100);
}


//****** HM_STOP FUNCTION ******//
void HM_Stop()
{
    /* Switching the MUX component to the channel 1 --> CLOCK_BLINKING_RGB: this clock is 
    set in order to allow the correct visualisation of the blinking on the RGB LED when 
    the parameters are changed as a feedback for the user */
    //Control_Reg_Write(MUX_CHANNEL_BLINKING);

    // Starting the ADC_DelSig component in order to sample the potentiometer values
    ADC_DelSig_Start();

    // Switching OFF the OnBoard LED component before blinking
    Pin_ONBOARD_LED_Write(ONBOARD_LED_OFF);

    // Start blinking of the OnBoardLED component
    flag_blinking = 1;
    
//    reg = LIS3DH_CTRL_REG1_INIT | MASK_DATARATE_0Hz;
//    
//    ErrorCode error = I2C_Peripheral_WriteRegister(LIS3DH_DEVICE_ADDRESS,
//                                                   LIS3DH_CTRL_REG1,
//                                                   reg);
//    if(error == ERROR)
//    {
//        UART_PutString("Error occurred during I2C comm\r\n");  
//    }
//    
    PWM_RG_WriteCounter(DC_0);
    PWM_B_WriteCounter(DC_0);
}


//****** HARDWARE MENU FUNCTION ******//
void Hardware_Menu()
{
    // Identification of the step of the CONFIGURATION MODE
    switch (flag_configurationmode)
    {
        // Sample the value of the potentiometer according to the selected parameter
        case CONFIGURATION_MODE:
            
            if(flag_sampling_pot)
            {
                // Reset the flag variable to the initial condition
                flag_sampling_pot = 0;
                
                // Sampling the potentiometer value 
                potentiometer_value = ADC_DelSig_Read16();
                
                // Control of the value assumed by the variable
                if (potentiometer_value > 255)  potentiometer_value = 255;
                if (potentiometer_value < 0)    potentiometer_value = 0;
            }
            
            // Modification of the parameters
            Potentiometer_to_Register(parameter_selected, potentiometer_value);

            /* Setting of the feedback on the RGB LED according to the chosen parameter and to the measured value from the
            potentiometer */
            Set_Feedback(parameter_selected);

            //****** SINGLE CLICK CONDITION ******//
            if (flag_singleclick)
            {
                // Reset the flag variable to the initial condition
                flag_singleclick = 0;
                
                /* Incrementing the value of the variable parameter_selected cycling among the 
                allowed possibilities */
                parameter_selected++;
                
                if (parameter_selected > VERBOSE_FLAG)
                {
                    parameter_selected = FS_RANGE;
                }
            }
            
            /****** LONG PRESSION CONDITION ******/
            if (flag_longpression)
            {
                // Reset the flag variable to the initial condition
                flag_longpression = 0;
                
                // Starting of the accelerometer when exiting from the CONFIGURATION MODE
                HM_Start();
                
                // Exiting from the CONFIGURATION MODE
                flag_configurationmode = IDLE;
            }
            
            //****** DOUBLE CLICK CONDITION *****//
            if (flag_doubleclick)
            {
                // Reset the flag variable to the initial condition
                flag_doubleclick = 0;
            }
            
            break;
        

        // Exiting from the CONFIGURATION MODE
        case IDLE:
            
            /****** LONG PRESSION CONDITION ******/
            if (flag_longpression)
            {
                // Reset the flag variable to the initial condition
                flag_longpression = 0;
                
                // Stopping the accelerometer when entering into the CONFIGURATION MODE
                HM_Stop();
                
                // Entering into the CONFIGURATION MODE
                flag_configurationmode = CONFIGURATION_MODE;
            }

            /****** DOUBLE CLICK CONDITION ******/
            if (flag_doubleclick) 
            {
                // Reset the flag variable to the initial condition
                flag_doubleclick = 0;
                
                // START and STOP values are defined as opposite values (+1 and -1 respectively)
                device_state = -device_state;

                // Control the value of the variable device_state
                if (device_state == RUN)
                {
                    // Switching ON the OnBoardLED component when the device is set to START condition
                    Pin_ONBOARD_LED_Write(ONBOARD_LED_ON);
                } else {
                    // Swtitching OFF the OnBoardLED component when the device is set to STOP condition
                    Pin_ONBOARD_LED_Write(ONBOARD_LED_OFF);
                }
            }
            
            /****** SINGLE CLICK CONDITION ******/
            if (flag_singleclick)
            {
                // Reset the flag variable to the initial condition
                flag_singleclick = 0;
            }
            
            break;
        

        /* Default condition --> the variable assumes a value which is not considered into the 
        switch case */
        default:
        
            /*??? Eventualmente si potrebbe segnalare una condizione di errore
            avvenuta all'interno degli step di entrata e di uscita del menù
            configurazione + reinizializzare il dispositivo alla condizione
            di prima accensione per resettarlo */
            break;
    }
}

/* [] END OF FILE */
