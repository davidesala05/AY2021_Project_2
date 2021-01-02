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

// Include related to the header file related to the functions defined in this file
#include "Functions_SETTINGS.h"

/*
Function used to initialize the register
of the accelerometer when the device is started.

This function is called at the power on the device in order to initialise the registers
of the accelerometer component according to the settings stored inside the internal
EEPROM memory --> it is necessary in order to set the options related to the
accelerometer component to a given initial condition in the case the user will start
the device (RUN mode) without changing the configurable options of the device
*/

void Register_Initialization(void)
{
    // Initialization of the LIS3DH_CTRL_REG0 register
    reg = LIS3DH_CTRL_REG0_INIT;
    ErrorCode error = I2C_Peripheral_WriteRegister(LIS3DH_DEVICE_ADDRESS,
                                                   LIS3DH_CTRL_REG0,
                                                   reg);
    if(error == ERROR){
        UART_PutString("Error occurred during I2C comm\r\n");  
    }
    
    // Initialization of the LIS3DH_TEMP_CFG_REG register
    reg = LIS3DH_TEMP_CFG_REG_INIT;
    error = I2C_Peripheral_WriteRegister(LIS3DH_DEVICE_ADDRESS,
                                         LIS3DH_TEMP_CFG_REG,
                                         reg);
    if(error == ERROR){
        UART_PutString("Error occurred during I2C comm\r\n");  
    }
    
    // Initialization of the LIS3DH_CTRL_REG1 register
    reg = LIS3DH_CTRL_REG1_INIT;
    error = I2C_Peripheral_WriteRegister(LIS3DH_DEVICE_ADDRESS,
                                         LIS3DH_CTRL_REG1,
                                         reg);
    if(error == ERROR){
        UART_PutString("Error occurred during I2C comm\r\n");  
    }
    
    // Initialization of the LIS3DH_CTRL_REG3 register
    reg = LIS3DH_CTRL_REG3_INIT;
    error = I2C_Peripheral_WriteRegister(LIS3DH_DEVICE_ADDRESS,
                                         LIS3DH_CTRL_REG3,
                                         reg);
    if(error == ERROR){
        UART_PutString("Error occurred during I2C comm\r\n");  
    }
    
    // Initialization of the LIS3DH_CTRL_REG4 register
    reg = LIS3DH_CTRL_REG4_INIT;
    error = I2C_Peripheral_WriteRegister(LIS3DH_DEVICE_ADDRESS,
                                         LIS3DH_CTRL_REG4,
                                         reg);
    if(error == ERROR){
        UART_PutString("Error occurred during I2C comm\r\n");  
    }
    
    // Initialization of the LIS3DH_CTRL_REG5 register
    reg = LIS3DH_CTRL_REG5_INIT;
    error = I2C_Peripheral_WriteRegister(LIS3DH_DEVICE_ADDRESS,
                                         LIS3DH_CTRL_REG5,
                                         reg);
    if(error == ERROR){
        UART_PutString("Error occurred during I2C comm\r\n");  
    }
    
    // Initialization of the LIS3DH_CTRL_REG6 register
    reg = LIS3DH_CTRL_REG6_INIT;
    error = I2C_Peripheral_WriteRegister(LIS3DH_DEVICE_ADDRESS,
                                         LIS3DH_CTRL_REG6,
                                         reg);
    if(error == ERROR){
        UART_PutString("Error occurred during I2C comm\r\n");  
    }
    
    // Initialization of the LIS3DH_FIFO_CTRL_REG register
    reg = LIS3DH_FIFO_CTRL_REG_INIT;
    error = I2C_Peripheral_WriteRegister(LIS3DH_DEVICE_ADDRESS,
                                         LIS3DH_FIFO_CTRL_REG,
                                         reg);
    if(error == ERROR){
        UART_PutString("Error occurred during I2C comm\r\n");  
    }
    
    // Initialization of the LIS3DH_INT2_CFG register
    reg = LIS3DH_INT2_CFG_INIT;
    error = I2C_Peripheral_WriteRegister(LIS3DH_DEVICE_ADDRESS,
                                         LIS3DH_INT2_CFG,
                                         reg);
    if(error == ERROR){
        UART_PutString("Error occurred during I2C comm\r\n");  
    }
} // Register_Initialisation()


/*
Function used to let start the components when
the device is powered ON.
*/

void Start_Components_powerON(void)
{
    /* Starting the I2C component allowing the communication with the accelerometer and the 
    external EEPROM */
    I2C_Peripheral_Start();
    
    // Starting the UART component allowing the communication through the serial port
    UART_Start();
    
    // Strarting the execution of the ISR called by the accelerometer
    isr_ACC_StartEx(Custom_ACC_ISR);
    
    // Strarting the execution of the ISR called by the UART
    isr_UART_StartEx(Custom_UART_ISR);
    
    // Strarting the execution of the ISR called by the timer
    isr_TIMESTAMP_StartEx(custom_TIMER_ISR);
    
    // Strarting the execution of the ISR called by the pressure of the PushButton
    isr_BUTTON_PRESS_StartEx(custom_BUTTON_PRESS_ISR);
    
    // Strarting the execution of the ISR called by the release of the PushButton
    isr_BUTTON_REL_StartEx(custom_BUTTON_REL_ISR);
    
    // Starting the internal EEPROM
    EEPROM_INTERNAL_Start();    
    
    // Starting the timer
    Timer_TIMESTAMP_Start();                            
} // Start_Components_powerON()


/*
Function used to change the RGB color
according to the acceleration value

This function is called when the device is in the RUN mode in order to set the
qualitative feedback related to the acceleration amplitude measured almost in
real-time --> the values used to define the colour of the RGB LED are the ones
exiting from the FIFO register and not directly the samples coming from the
accelerometer because the requirements of the project (that specifies the usage
of the FIFO register)
*/

void Set_RGB(void)
{
    /* Conversion of the acceleration data into duty cycles related to the PWM components -->
    this operation is done as a sort of calibration process from the acceleration inputs to the
    DC outputs: it has been considered the entire full scale range of the accelerometer as the
    input range while the output one is comprised between 0 and 100; finally, the absolute value
    is taken into consideration in order to increase the RGB LED luminosity when the amplitude
    of the acceleration increases both towards positive and negative values */
    DC_R = abs((int16)((DC_100/(FS_range_value*G))*accX));
    DC_G = abs((int16)((DC_100/(FS_range_value*G))*accY));
    DC_B = abs((int16)((DC_100/(FS_range_value*G))*accZ));
    
    // Writing the periods of the PWM components
    PWM_RG_WritePeriod(DC_100);
    PWM_B_WritePeriod(DC_100);
    
    // Writing the duty cycle values of the PWM components
    PWM_RG_WriteCompare1(DC_R);
    PWM_RG_WriteCompare2(DC_G);
    PWM_B_WriteCompare(DC_B);
} // Set_RGB()


/*
Function used to let start the components
after double-click

This function is necessary to restore the functioning 
of the device before entering in the configuration mode
*/

void HM_Start(void)
{
    /* Setting the stream-to-FIFO mode: this modality is appropriate according to the
    requirements of the project because the register acts like a normal FIFO during
    the functioning of the device and then it is freezed at the occurrende on an INT2
    interrupt allowing the reading of the data stored inside the FIFO register */
    Register_Initialization_after_Overth_Event();
    
    // Saving the parameters inside the internal EEPROM
    Initialize_Parameters();
    
    /* Converting the registers values into real data according to the meaning of the bits
    with respect to the registers considered */
    Register_to_value();
    
    // Starting the PWM components 
    PWM_RG_Start();
    PWM_B_Start();
} // HM_Start()


/*
Function used to stop the components
after double-click

This function is necessary to stop the functioning
of the device before entering in the configuration mode
*/

void HM_Stop(void)
{
    // Stopping the sampling of the accelerometer forcing the value of the bits 
    reg = LIS3DH_CTRL_REG1_INIT | MASK_DATARATE_0Hz;
    
    // Writing the register
    ErrorCode error = I2C_Peripheral_WriteRegister(LIS3DH_DEVICE_ADDRESS,
                                                   LIS3DH_CTRL_REG1,
                                                   reg);
    if(error == ERROR){
        UART_PutString("Error occurred during I2C comm\r\n");  
    }
    
    // Stopping the PWM components
    PWM_RG_Stop();
    PWM_B_Stop();
} // HM_Stop()


/*
Function used to initialize the parameters
(DataRate, Verbose flag and Full-scale Range)
read from the internal EEPROM, then these values
are used to set the internal registers of the
accelerometer component

This function is called at the power on of the device
to assign the last options set for the device and at
the exiting from the configuration mode in order to
update these parameters with the new values
*/

void Initialize_Parameters(void)
{
    // Reading the internal EEPROM address 0x0000
    reg = EEPROM_INTERNAL_ReadByte(EEPROM_INTERNAL_ADDRESS);
    
    // Saving the EEPROM information inside different variables
    DataRate_reg = 0b00001111 & (reg >> 4);
    FS_range_reg = 0b00000011 & (reg >> 2);
    Verbose_flag = 0b00000001 & (reg >> 1);

    // Datarate information writing in the accelerometer register
    reg = LIS3DH_CTRL_REG1_INIT | (DataRate_reg << 4);
    
    ErrorCode error = I2C_Peripheral_WriteRegister(LIS3DH_DEVICE_ADDRESS,
                                                   LIS3DH_CTRL_REG1,
                                                   reg);
    if(error == ERROR){
        UART_PutString("Error occurred during I2C comm\r\n");  
    }

    // Full scale range information writing in the accelerometer register
    reg = LIS3DH_CTRL_REG4_INIT | (FS_range_reg << 4);
    
    error = I2C_Peripheral_WriteRegister(LIS3DH_DEVICE_ADDRESS,
                                         LIS3DH_CTRL_REG4,
                                         reg);
    if(error == ERROR){
        UART_PutString("Error occurred during I2C comm\r\n");  
    }
    
    /*
    We read the register on which is saved the value of FS, depending on it the value inside the
    THS register is changed accordingly to a conversion so that we have a fixed threshold of 2G 
    for all the different full scale range selected through the menu --> this operation is
    necessary because the threshold value is set dependently on the full scale range value and
    it is not a fixed limit
    */
    
    /****** FULL SCALE RANGE ******/
    // Setting the options of the accelerometer component according to the parameters value
    switch (FS_range_reg) 
    {
        case MASK_FS_RANGE_2G: // +-2G
            I2C_Peripheral_WriteRegister(LIS3DH_DEVICE_ADDRESS,
                                         LIS3DH_INT2_THS,              
                                         LIS3DH_INT2_THS_2G);
            break;
        
        case MASK_FS_RANGE_4G: // +-4G
            I2C_Peripheral_WriteRegister(LIS3DH_DEVICE_ADDRESS,
                                         LIS3DH_INT2_THS,              
                                         LIS3DH_INT2_THS_4G);
            break;
        
        case MASK_FS_RANGE_8G: // +-8G
            I2C_Peripheral_WriteRegister(LIS3DH_DEVICE_ADDRESS,
                                         LIS3DH_INT2_THS,              
                                         LIS3DH_INT2_THS_8G);
            break;
        
        case MASK_FS_RANGE_16G: // +-16G
            I2C_Peripheral_WriteRegister(LIS3DH_DEVICE_ADDRESS,
                                         LIS3DH_INT2_THS,              
                                         LIS3DH_INT2_THS_16G);
            break;
        
        default:
            break;
    }
    
    /*
    We read the register on which is saved the value of ODR, depending on it the value inside the
    DURATION register is changed accordingly to a conversion so that we have a fixed duration of 
    0.2 seconds for all the different datarate selected through the menu --> this operation is
    necessary because the duration time is set dependently on the datarate value and it is not a 
    fixed limit
    */
    
    /****** DATARATE ******/
    // Setting the options of the accelerometer component according to the parameters value
    switch (DataRate_reg)
    {
        case MASK_DATARATE_50Hz: // 50 Hz
            I2C_Peripheral_WriteRegister(LIS3DH_DEVICE_ADDRESS,
                                         LIS3DH_INT2_DURATION,              
                                         LIS3DH_INT2_DURATION_50HZ);
            break;
        
        case MASK_DATARATE_100Hz: // 100 Hz
            I2C_Peripheral_WriteRegister(LIS3DH_DEVICE_ADDRESS,
                                         LIS3DH_INT2_DURATION,              
                                         LIS3DH_INT2_DURATION_100HZ);
            break;
        
        case MASK_DATARATE_200Hz: // 200 Hz
            I2C_Peripheral_WriteRegister(LIS3DH_DEVICE_ADDRESS,
                                         LIS3DH_INT2_DURATION,              
                                         LIS3DH_INT2_DURATION_200HZ);
            break;
        
        default:
            break;

    }
} // Initialize_Parameters()


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

void Register_to_value(void)
{
    /****** FULL SCALE RANGE ******/
    if (FS_range_reg == MASK_FS_RANGE_2G) // +-2G
    {
        // Initialisation of the variable according to the register value
        FS_range_value = 2;
        
        /* Initialisation of the sensitivity according to the full scale range chosen --> the
        sensitivity value is chosen with respect to the indications contained inside the
        datasheet of the accelerometer component */
        Sensitivity = 1;
    }
    else if (FS_range_reg == MASK_FS_RANGE_4G) // +-4G
    {
        // Initialisation of the variable according to the register value
        FS_range_value = 4;
        
        /* Initialisation of the sensitivity according to the full scale range chosen --> the
        sensitivity value is chosen with respect to the indications contained inside the
        datasheet of the accelerometer component */
        Sensitivity = 2;
    }
    else if (FS_range_reg == MASK_FS_RANGE_8G) // +-8G
    {
        // Initialisation of the variable according to the register value
        FS_range_value = 8;
        
        /* Initialisation of the sensitivity according to the full scale range chosen --> the
        sensitivity value is chosen with respect to the indications contained inside the
        datasheet of the accelerometer component */
        Sensitivity = 4;
    }
    else if (FS_range_reg == MASK_FS_RANGE_16G) // +-16G
    {
        // Initialisation of the variable according to the register value
        FS_range_value = 16;
        
        /* Initialisation of the sensitivity according to the full scale range chosen --> the
        sensitivity value is chosen with respect to the indications contained inside the
        datasheet of the accelerometer component */
        Sensitivity = 12;
    }
    
    /******* DATARATE *******/
    if (DataRate_reg == MASK_DATARATE_50Hz) // 50 Hz
    {
        // Initialisation of the variable according to the register value
        DataRate_value = 50;
    }
    else if (DataRate_reg == MASK_DATARATE_100Hz) // 100 Hz
    {
        // Initialisation of the variable according to the register value
        DataRate_value = 100;
    }
    else if (DataRate_reg == MASK_DATARATE_200Hz) // 200 Hz
    {
        // Initialisation of the variable according to the register value
        DataRate_value = 200;
    }
} // Register_to_value()


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

The feedback of the RGB LED is set with a arbitrary blinking frequencies
that increase with respect to the value assumed by the parameter
*/

void Potentiometer_to_Register(uint8_t parameter, int16_t value)
{
    // Controlling the value of the potentiometer
    switch (parameter)
    {
        /******* FULL SCALE RANGE ******/
        case FS_RANGE : /* Division of the entire range of allowed values into 4 equally spaced
        sectors according to the number of possible values that the variable can assume */
            if (value <= 64) // First sector
            {
                FS_range_reg = MASK_FS_RANGE_2G; // +-2G
            }
            else if (value > 64 && value <= 128) // Second sector
            {
                FS_range_reg = MASK_FS_RANGE_4G; // +-4G
            }
            else if (value > 128 && value <= 192) // Third sector 
            {
                FS_range_reg = MASK_FS_RANGE_8G; // +-8G
            }
            else if (value > 192) // Fourth sector
            { 
                FS_range_reg = MASK_FS_RANGE_16G; // +-16G
            }
        break;
        
        /******* DATARATE *******/
        case DATARATE : /* Division of the entire range of allowed values into 3 equally spaced
        sectors according to the number of possible values that the variable can assume */
            if (value <= 85) // First sector
            {
                DataRate_reg = MASK_DATARATE_50Hz; // 50 Hz
            }
            else if (value > 85 && value <= 170) // Second sector 
            {
                DataRate_reg = MASK_DATARATE_100Hz; // 100 Hz
            }
            else if (value > 170) // Third sector
            {
                DataRate_reg = MASK_DATARATE_200Hz; // 200 Hz
            }
        break;
        
        /****** VERBOSE FLAG ******/
        case VERBOSE_FLAG : /* Division of the entire range of allowed values into 2 equally spaced
        sectors according to the number of possible values that the variable can assume */
            if(value <= 128) // First half
            {
                Verbose_flag = 0;
            }
            else // Second half
            {
                Verbose_flag = 1;
            }
        break;
            
        default :
            break;
    }
} // Potentiometer_to_Register()


/*
Function used to convert the value read by the potentiometer in
a determined parameter to a feedback.
The feedback is different in blicking frequency and in colour of the RGB LED
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

void Set_Feedback(uint8_t parameter)
{
    /* Initialisation of the periods of the PWM: this is necessary for the correct usage of the 
    components with the CLOCKs connected through a MULTIPLEXER --> when a source clock is not
    defined univocally for a PWM component, it is necessary to reset the period register and
    re-write it in order to allow the proper functioning of the components */
    PWM_RG_WritePeriod(DC_0);
    PWM_B_WritePeriod(DC_0);
    
    // Controlling the value of the parameter
    switch (parameter)
    {
        /****** FULL SCALE RANGE ******/
        case FS_RANGE : // RED feedback
        
            // Switch OFF GREEN and BLUE LED
            PWM_RG_WriteCompare2(DC_0);
            PWM_B_WriteCompare(DC_0);
            
            if (FS_range_reg == MASK_FS_RANGE_2G) // Blinking at 1 Hz
            {
                // Setting the period
                PWM_RG_WritePeriod(PERIOD_1Hz);
                
                // Setting the compare value with half of the period (duty cycle = 50%)
                PWM_RG_WriteCompare1(PERIOD_1Hz/2);
            }
            else if (FS_range_reg == MASK_FS_RANGE_4G) // Blinking at 4 Hz
            {
                // Setting the period
                PWM_RG_WritePeriod(PERIOD_2Hz);
                
                // Setting the compare value with half of the period (duty cycle = 50%)
                PWM_RG_WriteCompare1(PERIOD_2Hz/2);
            }
            else if (FS_range_reg == MASK_FS_RANGE_8G) // Blinking at 5 Hz
            { 
                // Setting the period
                PWM_RG_WritePeriod(PERIOD_5Hz);
                
                // Setting the compare value with half of the period (duty cycle = 50%)
                PWM_RG_WriteCompare1(PERIOD_5Hz/2);
            }
            else if (FS_range_reg == MASK_FS_RANGE_16G) // Blinking at 10 Hz
            {
                // Setting the period
                PWM_RG_WritePeriod(PERIOD_10Hz);
                
                // Setting the compare value with half of the period (duty cycle = 50%)
                PWM_RG_WriteCompare1(PERIOD_10Hz/2);
            }
            break;
        
        /****** DATARATE ******/
        case DATARATE : // GREEN feedback
            
            // Switch OFF RED and BLUE LED
            PWM_RG_WriteCompare1(DC_0);
            PWM_B_WriteCompare(DC_0);
            
            if (DataRate_reg == MASK_DATARATE_50Hz) // Blinking at 1 Hz
            { 
                // Setting the period
                PWM_RG_WritePeriod(PERIOD_1Hz);
                
                // Setting the compare value with half of the period (duty cycle = 50%)
                PWM_RG_WriteCompare2(PERIOD_1Hz/2);
            }
            else if (DataRate_reg == MASK_DATARATE_100Hz) // Blinking at 2 Hz
            { 
                // Setting the period
                PWM_RG_WritePeriod(PERIOD_2Hz);
                
                // Setting the compare value with half of the period (duty cycle = 50%)
                PWM_RG_WriteCompare2(PERIOD_2Hz/2);
            }
            else if (DataRate_reg == MASK_DATARATE_200Hz) // Blinking at 5 Hz
            { 
                // Setting the period
                PWM_RG_WritePeriod(PERIOD_5Hz);
                
                // Setting the compare value with half of the period (duty cycle = 50%)
                PWM_RG_WriteCompare2(PERIOD_5Hz/2);
            }
            break;
            
        /****** VERBOSE FLAG ******/
        case VERBOSE_FLAG : // BLUE feedback
            
            // Switch OFF RED and GREEN LED
            PWM_RG_WriteCompare1(DC_0);
            PWM_RG_WriteCompare2(DC_0);
            
            if(Verbose_flag == 0) // Blinking at 1 Hz
            {
                // Setting the period
                PWM_B_WritePeriod(PERIOD_1Hz);
                
                // Setting the compare value with half of the period (duty cycle = 50%)
                PWM_B_WriteCompare(PERIOD_1Hz/2);
            }
            else // Blinking at 5 Hz
            { 
                // Setting the period
                PWM_B_WritePeriod(PERIOD_5Hz);
                
                // Setting the compare value with half of the period (duty cycle = 50%)
                PWM_B_WriteCompare(PERIOD_5Hz/2);
            }
            break;
        
        // Doing nothing in all the other conditions
        default :
            break;
    }
} // Set_Feedback()


/*
Function used to save in the Internal EEPROM the
register with the new parameters.

This function is called at the exiting from the configuration mode
in order to store the new parameters value inside the internal
EEPROM memory, so they will be available at the next power on of 
the device
*/

void Save_Parameters_on_INTERNAL_EEPROM(void)
{
    // Creating the variable in order to write the new parameters
    reg = (DataRate_reg << 4) | (FS_range_reg << 2) | (Verbose_flag << 1);
    
    // Update the temperature value
    EEPROM_INTERNAL_UpdateTemperature();
    
    // Writing the variable in the internal EEPROM at the 0x0000 address
    EEPROM_INTERNAL_WriteByte(reg, EEPROM_INTERNAL_ADDRESS);
} // Save_Parameters_on_INTERNAL_EEPROM()


/*
Function used to reset the Stream_To_FIFO mode after
an overthreshold event is occured

This function is necessary to initialize the FIFO_CTRL register to the
BYPASS MODE and then go back to the Stream_to_FIFO mode --> this is a
mandatory passage in order to properly set the functioning of the FIFO
register according to the datasheet of the accelerometer component
*/

void Register_Initialization_after_Overth_Event(void)
{
    /* Setting bypass mode --> this passage is mandatory to allow the restoration of the normal
    functioning of the FIFO register */
    reg = LIS3DH_FIFO_CTRL_REG_BYPASS_MODE;
    ErrorCode error = I2C_Peripheral_WriteRegister(LIS3DH_DEVICE_ADDRESS,
                                                   LIS3DH_FIFO_CTRL_REG,
                                                   reg);
    if(error == ERROR){
        UART_PutString("Error occurred during I2C comm\r\n");  
    }
    
    // Restore the stream-to-FIFO mode
    reg = LIS3DH_FIFO_CTRL_REG_Stream_to_FIFO_MODE;
    error = I2C_Peripheral_WriteRegister(LIS3DH_DEVICE_ADDRESS,
                                         LIS3DH_FIFO_CTRL_REG,
                                         reg);
    if(error == ERROR){
        UART_PutString("Error occurred during I2C comm\r\n");  
    }
} // Register_Initialization_after_Overth_Event()


/*
Function is used to force the counter of the PWMs to finish the period.
    
This function is fundamental when the MUX switches between the clocks at
4MHz and 100Hz --> otherwise the PWMs are blocked for a random number clocks 
at 100Hz before being responsive to the new settings, introducing an unpredictable
delay in the feedback from the RUN mode to the configuration one
*/

void Reset_PWM_for_CONF_MODE(void)
{
    // Initialisation of the counter register to zero
    PWM_RG_WriteCounter(DC_0);
    PWM_B_WriteCounter(DC_0);

    // Starting the PWM components
    PWM_RG_Start();
    PWM_B_Start();
} // Reset_PWM_for_CONF_MODE()

/* [] END OF FILE */
