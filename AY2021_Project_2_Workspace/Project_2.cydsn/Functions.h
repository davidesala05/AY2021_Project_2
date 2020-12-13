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

#ifndef _FUNCTIONS_H_
    
    #define _FUNCTIONS_H_
    
    #include "project.h"
    #include "cytypes.h"
    #include "Global.h"
    
    
    /*
    Function used to initialize the register
    of the accelerometer when the device is started.
    */
    void Register_Initialization(void);
    
    /*
    Function used to let start the components when
    the device is powered ON.
    */
    void Start_Components_powerON(void);
    /*
    Function used to change the RGB color
    according to the acceleration value
    */
    void Set_RGB(void);
    
    /*
    Function used to let start the components
    after double-click
    */
    void HM_Start(void);
    
    /*
    Function used to stop the components
    after double-click
    */
    void HM_Stop(void);
    
    /*
    Function used to initialize the parameters
    (DataRate, Verbose flag and Full-scale Range)
    when the device is started.
    */
    void Initialize_Parameters(void);
    
    /*
    Function used to convert the register used to
    change the parameter (for the EEPROM saving)
    in the real value of the parameter (used for the conversions)
    */
    void Register_to_value(void);
    
    /*
    Function used to convert the position of the potentiometer
    in the correspondent value of the current parameter to set
    */
    void Potentiometer_to_Register(uint8_t parameter, int16_t value);
    
    /*
    Function used to convert the value read by the potentiometer in
    a determined parameter to a feedback.
    The feedback is different in blicking frequency and in colour of the RGB LED
    according to the below references:
    
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
    */
    void Set_Feedback(uint8_t parameter);
    
    /*
    Function used to save in the INTERNAL EEPROM the
    register with the new parameters.
    To be called at the exit of the CONFIGURATION MODE
    */
    void Save_Parameters_on_INTERNAL_EEPROM(void);
    
    /*
    Function used to reset the stream_to_FIFO MODE after
    that an overthreshold event is occured.
    Is necessary to initialize the FIFO_CTRL register to the
    BYPASS MODE and then go back to the Stream_to_FIFO MODE.
    */
    void Register_Initialization_after_Overth_Event(void);
    
    /*
    Function used to write all the waveform values with a multiread
    in the external eeprom.
    */
    void Write_Waveform_on_EXTERNAL_EEPROM(void);
    
    /*
    Function used to write the current sensitivity on the external
    eeprom. This is necessary since the sensitivity change with
    the full-scale range (parameter that could change in configuration
    mode during the run time).
    So, diffrent overtheshold waveform can be generated under different conditions
    and must be interpreted using different sensitivy.
    */
    void Write_Sensitivity_on_EXTERNAL_EEPROM(void);
    
    /*
    Function used to write the timestamp's three values (hours, minutes, seconds
    with a multiread in the external eeprom.
    */
    void Write_Timestamp_on_EXTERNAL_EEPROM(void);
    
    void Write_Datarate_on_EXTERNAL_EEPROM(void);
    
    /*
    Function used to read the waveforms saved in the external eeprom,
    convert them in int32 and send consequently by the UART
    */
    void Read_Waveform_from_EXTERNAL_EEPROM(void);
    
    /*
    Function used to read the timestamps saved in the external eeprom
    and send them through the UART
    */
    void Read_Timestamp_from_EXTERNAL_EEPROM(void);
    
    void Reset_PWM_for_CONF_MODE(void);
    
    void Reset_PWM_for_RUN_MODE(void);
    
    void Set_FS_Registers (void);
    
    void Set_Duration_Registers (void);

#endif

/* [] END OF FILE */
