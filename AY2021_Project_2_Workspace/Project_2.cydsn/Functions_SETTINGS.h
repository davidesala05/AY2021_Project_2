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
*  \brief header file for various Settings definition
* 
*  In this header file are declared all the functions used to 
*  define and change the setting of the device,
*  read them, and export them as CSV in order to be plot with
*  Matplotlib library in Python.
*   
*/

#ifndef _FUNCTIONS_SETTINGS_H_
    
    #define _FUNCTIONS_SETTINGS_H_
    
    // Include related to all the variables implemented into the actual project
    #include "Global.h"

    // Include related to all the components related to the actual project
    #include "project.h"
    
    // Include related to all the cytypes functions 
    #include "cytypes.h"
    
    
    /*
    Function used to initialize the register
    of the accelerometer when the device is started.
    
    This function is called at the power on the device in order to initialise the registers
    of the accelerometer component according to the settings stored inside the internal
    EEPROM memory--> it is necessary in order to set the options related to the
    accelerometer component to a given initial condition in the case the user will start
    the device (RUN mode) without changing the configurable options of the device
    */    
    void Register_Initialization(void);
    
    
    /*
    Function used to start the components when
    the device is powered ON
    */    
    void Start_Components_powerON(void);

    
    /*
    Function used to change the RGB color
    according to the raw acceleration value
    
    This function is called when the device is in the RUN mode in order to set the
    qualitative feedback related to the acceleration amplitude measured almost in
    real-time --> the values used to define the colour of the RGB LED are the ones
    exiting from the FIFO register and not directly the samples coming from the
    accelerometer because the requirements of the project (that specifies the usage
    of the FIFO register)
    */    
    void Set_RGB(void);
    
    
    /*
    Function used to let start the components
    after double-click (enter in configuration mode)
    
    This function is necessary to restore the functioning 
    of the device before entering in the configuration mode
    */
    void HM_Start(void);

    
    /*
    Function used to stop the components
    after double-click (exit from configuration mode)
    
    This function is necessary to stop the functioning
    of the device before entering in the configuration mode
    */
    void HM_Stop(void);

    
    /*
    Function used to initialize the three parameters
    (DataRate, Verbose flag and Full-scale Range)
    read from the internal EEPROM
    
    This function is called at the power on of the device
    to assign the last options set for the device and at
    the exiting from the configuration mode in order to
    update these parameters with the new values
    */    
    void Initialize_Parameters(void);

    
    /*
    Function used to convert the binary register value 
    used to change the parameter (for EEPROM saving)
    in the real value of the parameter (used for the conversions)
    
    This function is called when the device is exiting from
    the configuration mode in order to convert the values defined
    as registers (so they are masks to be superposed to the accelerometer
    registers in order to change its configuration) into the corresponding
    real values that can be used in the program for the correct conversion
    of the acceleration data expressed in m/s^2
    */
    void Register_to_value(void);


    /*
    Function used to convert the position of the potentiometer
    in the correspondent value of the current parameter to set
    
    This function is called when the device is in the configuration
    mode and it is used to associate a specific output of the potentiometer
    to an allowed value of the parameter that the user is modifying --> this
    association is managed in a discretised way: the permitted configuration
    are represented by equally spaced ranges of all the possible values that
    the potentiometer output can assume (considering the resolution of the
    ADC_DelSig component equal to 8 bits)
    */    
    void Potentiometer_to_Register(uint8_t parameter, int16_t value);

    
    /*
    Function used to convert the value read by the potentiometer in
    a determined parameter to a feedback.
    
    The feedback is different in blinking frequency and in colour of the RGB LED
    according to the below references, considering a duty cycle of 50%:
    
    - FS_RANGE --> RED
        +- 2G  -->  1 Hz
        +- 4G  -->  2 Hz
        +- 8G  -->  5 Hz
        +- 16G --> 10 Hz
    
    - DATARATE --> GREEN
        50Hz   --> 1 Hz
        100Hz  --> 2 Hz
        200Hz  --> 5 Hz
    
    - VERBOSE FLAG
        ON  --> 1 Hz
        OFF --> 5 Hz
    
    This function is called when the device is in configuration mode,
    immediately after the execution of the Register_to_value() function
    because it gives as output the real values assumed by the parameters
    */    
    void Set_Feedback(uint8_t parameter);
    
    
    /*
    Function used to save in the Internal EEPROM the
    register with the new parameters.
    
    This function is called at the exiting from the configuration mode
    in order to store the new parameters value inside the internal
    EEPROM memory, so they will be available at the next power on of 
    the device
    */
    void Save_Parameters_on_INTERNAL_EEPROM(void);

    
    /*
    Function used to reset the Stream_To_FIFO mode after
    an overthreshold event is occured
    
    This function is necessary to initialize the FIFO_CTRL register to the
    BYPASS MODE and then go back to the Stream_to_FIFO mode --> this is a
    mandatory passage in order to properly set the functioning of the FIFO
    register according to the datasheet of the accelerometer component
    */
    void Register_Initialization_after_Overth_Event(void);

    
    /*
    Function is used to force the counter of the PWMs to finish the period.
    
    This function is fundamental when the MUX switches between the clocks at
    4MHz and 100Hz --> otherwise the PWMs are blocked for a random number clocks 
    at 100Hz before being responsive to the new settings, introducing an unpredictable
    delay in the feedback from the RUN mode to the configuration one
    */
    void Reset_PWM_for_CONF_MODE(void);

#endif // _FUNCTIONS_SETTINGS_H_

/* [] END OF FILE */
