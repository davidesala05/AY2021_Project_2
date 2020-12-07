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

//#include <HardwareMenu.h>
//#include <stdio.h>
//
////****** PRESSING IDENTIFICATION FUNCTION ******//
//void Pressing_Identification()
//{
//    // Detect the pressing of the PushButton component
//    if (flag_isbuttonpressed)
//    {
//        // Reset the flag variable to the initial condition
//        flag_isbuttonpressed = 0;
//        
//        /* Store the value of the count_global when the PushButton component is pressed in order to
//        evaluate the pressing time */
//        start_press = count_global;
//        
//        // Waiting until the release of the PushButton component
//        while (Pin_BUTTON_Read() == BUTTON_PRESSED);
//        
//        /* Store the value of the count_global when the PushButton components is released at the 
//        occurrence of the first click in order to evaluate the time interval between the two consecutive
//        clicks --> DOUBLE CLICK identification */
//        if (count_clicks == 0)
//        {
//            start_release = count_global;
//        }
//        
//        /* Verify that the pressing of the PushButton component is shorter or equal than 1 second of 
//        duration in order to idenitfy the occurrence of the SINGLE or DOUBLE CLICK conditions */
//        if (abs(count_global - start_press) <= COUNTS_1_SECOND/2)
//        {
//            /* Increment the variable only when the pressing of the PushButton component is identified
//            as a short pression */
//            count_clicks++;
//            
//            /* Verify that the second pressing of the PushButton component happens within a time interval
//            shorter or equal than 0.5 seconds in order to identify the possible occurrence of the DOUBLE
//            CLICK condition */
//            if ((abs(start_press - start_release) <= COUNTS_1_SECOND/2) && (count_clicks == DOUBLE_CLICK))
//            {
//                //****** DOUBLE CLICK CONDITION ******//
//                flag_doubleclick = 1;
//                count_clicks = 0;
//                
//                // Testing the function
//                UART_PutString("DOUBLE CLICK\r\n");
//            }
//        }
//        
//        // Single short pressing occurrence
//        if (count_clicks == SINGLE_CLICK)
//        {
//            /* Verify that the time interval from the release of the PushButton component is greater
//            than 0.5 seconds in order to identify the SINGLE CLICK occurrence */
//            if (abs(count_global - start_release) > COUNTS_1_SECOND/2)
//            {
//                //****** SINGLE CLICK CONDITION ******//
//                flag_singleclick = 1;
//                count_clicks = 0;
//                
//                // Testing the function
//                UART_PutString("SINGLE CLICK\r\n");
//            }
//            
//            /* Verify that the second pressing of the PushButton component is longer than 0.5 seconds
//            in order to identify a SINGLE CLICK + LONG PRESSION condition, which is meaningless in the
//            implementation of the hardware menu */
//            if (abs(count_global - start_press) > COUNTS_1_SECOND/2)
//            {
//                //****** ERROR CONDITION ******//
//                flag_error = 1;
//                count_clicks = 0;
//                
//                // Testing the function
//                UART_PutString("Not allowable pressing\r\n");
//            }
//        }
//        
//        /* Verify that the pressing of the PushButton component is longer or equal than 2 seconds 
//        of duration in order to identify the occurrence of the LONG PRESSION condition */
//        if (abs(count_global - start_press) >= LONG_PRESSION_DURATION && flag_doubleclick == 0 && 
//        flag_singleclick == 0 && flag_error == 0)
//        {
//            //****** LONG PRESSION CONDITION ******//
//            flag_longpression = 1;
//            count_clicks = 0;
//            
//            // Testing the function
//            UART_PutString("LONG PRESSION\r\n");
//        }
//        
//        /* Definition of a support variable used to identify the possibile occurrence of an ERROR
//        condition --> this flag is posed equal to 1 when two conditions are satisfied:
//        - The first click is a short pressing (duration shorter or equal than 1 second)
//        - The second click, which happens within the time interval of 0.5 seconds (in the other case,
//        this event will be classified as SINGLE CLICK condition), is a long pressing */
//        flag_error = 0;
//    }
//}
//
////****** HARDWARE MENU FUNCTION ******//
//void Hardware_Menu()
//{
//    // Manage the different types of pressing of the PushButton component
//    Pressing_Identification();
//        
//    /****** LONG PRESSION CONDITION ******/
//    if (flag_longpression)
//    {
//        // Reset the flag variable to the initial condition
//        flag_longpression = 0;
//        
//        // ENTRY and EXIT states are defined as opposite values (-1 and +1 respectively)
//        flag_configurationmode = -flag_configurationmode;
//        
//        // Testing the function
//        char message[20] = {'\0'};
//        if (flag_configurationmode == CM_ENTRY) sprintf(message, "Configuration mode ENTRY\r\n");
//        if (flag_configurationmode == CM_EXIT)  sprintf(message, "Configuration mode EXIT\r\n");
//        UART_PutString(message);
//        
//        // Entering into the CONFIGURATION MODE of the hardware menu
//        HM_Configuration();
//    }
//
//    /****** DOUBLE CLICK CONDITION ******/
//    if (flag_doubleclick) 
//    {
//        // Reset the flag variable to the initial condition
//        flag_doubleclick = 0;
//        
//        // START and STOP values are defined as opposite values (+1 and -1 respectively)
//        device_state = -device_state;
//
//        // Control the value of the variable device_state
//        if (device_state == RUN)
//        {
//            // Entering in the RUN condition of the device
//            HM_Start();
//            
//            // Switching ON the OnBoardLED component when the device is set to START condition
//            Pin_ONBOARD_LED_Write(ONBOARD_LED_ON);
//            
//            // Testing the function
//            char message[20] = {'\0'};
//            sprintf(message, "Device state RUN\r\n");
//            UART_PutString(message);
//        } else {
//            // Entering in the WAIT condition of the device
//            HM_Stop();
//
//            // Swtitching OFF the OnBoardLED component when the device is set to STOP condition
//            Pin_ONBOARD_LED_Write(ONBOARD_LED_OFF);
//            
//            // Testing the function
//            char message[20] = {'\0'};
//            sprintf(message, "Device state WAIT\r\n");
//            UART_PutString(message);
//        }
//    }
//}
//
////****** HM CONFIGURATION FUNCTION ******//
//void HM_Configuration()
//{
//    // Identification of the step of the CONFIGURATION MODE
//    switch (flag_configurationmode)
//    {
//        // Entering into the CONFIGURATION MODE
//        case CM_ENTRY:
//        {
//            /* Switching the MUX component to the channel 1 --> CLOCK_BLINKING_RGB: this clock is 
//            set in order to allow the correct visualisation of the blinking on the RGB LED when 
//            the parameters are changed as a feedback for the user */
//            //Control_Reg_Write(MUX_CHANNEL_BLINKING);
//
//            // Stop the components of the device
//            HM_Stop();
//
//            // Start the ADC_DelSig sampling
//            ADC_DelSig_Start();
//
//            // Start blinking of the OnBoardLED component
//            flag_blinking = 1;
//
//            // Next step of the CONFIGURATION MODE
//            flag_configurationmode = CM_SETPARAMETERS;
//
//            break;
//        }
//
//        // Sample the value of the potentiometer according to the selected parameter
//        case CM_SETPARAMETERS:
//        {
//            // Testing the function
//            char message[20] = {'\0'};
//            sprintf(message, "Configuration mode SETPARAMETERS\r\n");
//            UART_PutString(message);
//            
//            // Modification of the parameters
//            //Potentiometer_to_Register(parameter_selected, potentiometer_value);
//
//            /* Setting of the feedback on the RGB LED according to the chosen parameter and to the measured value from the
//            potentiometer */
//            //Set_Feedback(parameter_selected, potentiometer_value);
//            
//            /****** LONG PRESSION CONDITION ******/
//            if (flag_longpression)
//            {
//                // Reset the flag variable to the initial condition
//                flag_longpression = 0;
//         
//                // Next step of the CONFIGURATION MODE
//                flag_configurationmode = CM_EXIT;
//            }
//
//            //****** SINGLE CLICK CONDITION ******//
//            if (flag_singleclick)
//            {
//                // Reset the flag variable to the initial condition
//                flag_singleclick = 0;
//                
//                /* Incrementing the value of the variable parameter_selected cycling among the 
//                allowed possibilities */
//                parameter_selected++;
//                if (parameter_selected > VERBOSE_FLAG)
//                {
//                    parameter_selected = FS_RANGE;
//                }
//                
//                // Testing the function
//                char message[20] = {'\0'};
//                if (parameter_selected == FS_RANGE)     sprintf(message, "Parameter selected FS_RANGE\r\n");
//                if (parameter_selected == DATARATE)     sprintf(message, "Parameter selected DATARATE\r\n");
//                if (parameter_selected == VERBOSE_FLAG) sprintf(message, "Parameter selected VERBOSE_FLAG\r\n");
//                UART_PutString(message);
//            }
//            
//            break;
//        }
//
//        // Exiting from the CONFIGURATION MODE
//        case CM_EXIT:
//        {
//            /* Switching the MUX component to the channel 1 --> CLOCK_COLOUR_RGB: this clock is 
//            set in order to allow the correct visualisation of the colour on the RGB LED when the
//            accelerometer measuring the acceleration values */
//            //Control_Reg_Write(MUX_CHANNEL_COLOUR);
//
//            // Stop the ADC_DelSig sampling
//            ADC_DelSig_Stop();
//
//            // Stop blinking of the OnBoardLED component
//            flag_blinking = 0;
//
//            // Conversion of the variables for the registers into values
//            //Register_to_value();
//            
//            // Storage of the parameters on the internal EEPROM
//            //Save_Parameters_on_INTERNAL_EEPROM();
//
//            /* Restoring the actual state of the device before entering into the CONFIGURATION 
//            MODE */
//            if (device_state == RUN)
//            {
//                // RUN condition of the device
//                HM_Start();
//
//                // Switching ON the OnBoardLED component when the device is set to START condition
//                Pin_ONBOARD_LED_Write(ONBOARD_LED_ON);
//            } else {
//                /* Swtitching OFF the OnBoardLED component when the device is set to STOP
//                condition */
//                Pin_ONBOARD_LED_Write(ONBOARD_LED_OFF);
//            }
//        }
//        break;
//
//        /* Default condition --> the variable assumes a value which is not considered into the 
//        switch case */
//        default:
//        {
//            /* Eventualmente si potrebbe segnalare una condizione di errore
//            avvenuta all'interno degli step di entrata e di uscita del menù
//            configurazione + reinizializzare il dispositivo alla condizione
//            di prima accensione per resettarlo */
//            break;
//        }
//    }
//}
//
////****** HM START FUNCTION ******//
//void HM_Start()
//{
//    // Starting the components
//    I2C_Master_Start();
//    PWM_RG_Start();
//    PWM_B_Start();
//    Timer_TIMESTAMP_Start();
//    UART_Start();
//
//    /* Controllare che i bit ODR siano diversi da zero, altrimenti
//    l'accelerometro non inizierà in ogni caso a campionare i propri
//    valori */
//}
//
////****** HM STOP FUNCTION ******//
//void HM_Stop()
//{
//    // Stopping the components
//    I2C_Master_Stop();
//    PWM_RG_Stop();
//    PWM_B_Stop();
//    Timer_TIMESTAMP_Stop();
//    UART_Stop();
//
//    /* Settare i bit ODR nella confiurazione 'power down mode' per
//    interrompere il campionamento di nuovi dati da parte
//    dell'accelerometro */
//}
//
///* [] END OF FILE */
//