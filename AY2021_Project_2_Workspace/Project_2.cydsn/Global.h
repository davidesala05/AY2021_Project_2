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

#ifndef __GLOBAL_H_
    
    #define __GLOBAL_H_
    
    #include "project.h"
    #include "cytypes.h"
    #include "ErrorCodes.h"
    #include "I2C_Interface.h"
    #include "stdlib.h"
    #include "InterruptRoutines_ACC.h"
    #include "InterruptRoutines_UART.h"

    /******************************************/
    /*        CONFIGURATION REGISTERS         */
    /******************************************/

    #define LIS3DH_DEVICE_ADDRESS    0x18

    #define LIS3DH_CTRL_REG0 0x1E
    #define LIS3DH_CTRL_REG0_INIT 0b00010000

    #define LIS3DH_TEMP_CFG_REG 0x1F
    #define LIS3DH_TEMP_CFG_REG_INIT 0b00000000

    #define LIS3DH_CTRL_REG1 0x20
    #define LIS3DH_CTRL_REG1_INIT 0b00000111

    #define LIS3DH_CTRL_REG3 0x22
    #define LIS3DH_CTRL_REG3_INIT 0b00010000

    #define LIS3DH_CTRL_REG4 0x23
    #define LIS3DH_CTRL_REG4_INIT 0b00001000

    #define LIS3DH_CTRL_REG5 0x24
    #define LIS3DH_CTRL_REG5_INIT 0b01000000

    #define LIS3DH_CTRL_REG6 0x25
    #define LIS3DH_CTRL_REG6_INIT 0b00101000

    #define LIS3DH_FIFO_CTRL_REG 0x2E
    #define LIS3DH_FIFO_CTRL_REG_INIT 0b11100000

    #define LIS3DH_INT2_CFG 0x34
    #define LIS3DH_INT2_CFG_INIT 0b00101010

    #define LIS3DH_INT2_THS 0x36
    #define LIS3DH_INT2_THS_INIT 0b00111111 //a caso

    #define LIS3DH_INT2_DURATION 0x37
    #define LIS3DH_INT2_DURATION_INIT 0b00000111 //a caso
    

    /******************************************/
    /*            OTHER ADDRESSES             */
    /******************************************/

    #define OUT_X_L 0x28
    
    #define LIS3DH_INT2_SRC 0x35
    
    #define LIS3DH_FIFO_CTRL_REG_BYPASS_MODE 0b00100000
    
    #define LIS3DH_FIFO_CTRL_REG_Stream_to_FIFO_MODE 0b11100000
    
    #define EEPROM_INTERNAL_ADDRESS 0x0000

    /******************************************/
    /*              OTHER MACROS              */
    /******************************************/
    
    //Number of register to read for acceleration values
    #define N_REG_ACC   6
    //Constants for the digit to m/s^2 conversion
    #define G           9.80665
    #define mg_TO_g     0.001
    //MAX and MIN possible duty cycle for the RGB LED
    #define DC_100      65535
    #define DC_0        0
    //DUTY CYCLES used in the FEEDBACK (clock = 100Hz)
    #define PERIOD_1Hz      99
    #define PERIOD_2Hz      49
    #define PERIOD_5Hz      19
    #define PERIOD_10Hz      9
    //Sizes of the BUFFER used to send the values
    #define BUFFER_SIZE          4*3 //4(the byte of a float32) * 3(the variables)
    #define TRANSMIT_BUFFER_SIZE 1+BUFFER_SIZE+1
    //TAIL and HEADER added to BUFFER
    #define HEADER 0xA0
    #define TAIL   0xC0
    //Macros used in the Set_Colour_parameter function
    #define RED 0
    #define GREEN 1
    #define BLUE 2
    //Macros used in the Potentiometer_to_value function
    #define FS_RANGE 0
    #define DATARATE 1
    #define VERBOSE_FLAG 2
    
    /******************************************/
    /*                 MASKS                  */
    /******************************************/
    //MASK used to detect if an overthreshold event is occur
    #define MASK_OVERTH_EVENT 0b01000000
    //MASKS used in the Register_to_value function
    //FS_RANGE conversion
    #define MASK_FS_RANGE_2G  0b00000000
    #define MASK_FS_RANGE_4G  0b00000001
    #define MASK_FS_RANGE_8G  0b00000010
    #define MASK_FS_RANGE_16G 0b00000011
    //DATARATE conversion
    #define MASK_DATARATE_50Hz  0b00000100
    #define MASK_DATARATE_100Hz 0b00000101
    #define MASK_DATARATE_200Hz 0b00000110
    
    /******************************************/
    /*            GLOBAL VARIABLES            */
    /******************************************/

    extern uint8_t  reg;             //Used to save the registers content
    extern uint8_t  Register_Param;  //Used to save the 0x0000 register of the INTERNAL EEPROM
    extern uint8_t  flag_ACC;        //Used in the main to sampling
    extern uint8_t  reg_INT2_SRC;    //Used to save the INT2_SRC register content

    extern uint8_t  data[6];         //Used to save the acceleration values READ by the MULTIREAD
    extern int16    dataX;           //Used to store the X-axis acceleration in 16bit
    extern int16    dataY;           //Used to store the Y-axis acceleration in 16bit
    extern int16    dataZ;           //Used to store the Z-axis acceleration in 16bit
    extern float32  accX;            //Used to store the X-axis acceleration in float32
    extern float32  accY;            //Used to store the Y-axis acceleration in float32
    extern float32  accZ;            //Used to store the X-axis acceleration in float32

    extern uint8_t  FS_range_reg;    //Used to save the FULL-SCALE range REGISTER
    extern uint8_t  DataRate_reg;    //Used to save the DATARATE range REGISTER
    extern uint8_t  FS_range_value;  //Used to save the FULL-SCALE range VALUE
    extern uint8_t  DataRate_value;  //Used to save the DATARATE range VALUE
    extern uint8_t  Sensitivity;     //Used to save the sensitivity used for the conversion
    extern uint8_t  Verbose_flag;    //Used to the verbose flag option

    extern uint16_t DC_R;            //Used to save the duty cycle (CompareValue) of the PWM for RED channel
    extern uint16_t DC_G;            //Used to save the duty cycle (CompareValue) of the PWM for GREEN channel
    extern uint16_t DC_B;            //Used to save the duty cycle (CompareValue) of the PWM for BLUE channel

    extern uint8_t Buffer[TRANSMIT_BUFFER_SIZE]; //The BUFFER used to send the values by UART
    
    extern uint8_t flag_overth_event;    //Used to save if an isr called by the accelerometer is cause by an overthreshold event
    extern uint8_t ch_received;          //Variable used to save the character received by the UART
    extern uint8_t flag_send_timestamps; //flag used to send the timestamp (if ch_received is "B" or "b")

    /*
    Below the UNION used to store the values after the conversion in 32bit is declared
    - DataUnion.f is used to stored the float32 value
    - DataUnion.l is used to extract the byte to saved in Buffer[i]
    */

    union FloatUnion {
        float32 f;
        uint32_t l;
    }DataUnion;


    /******************************************/
    /*                FUNCTIONS               */
    /******************************************/
    
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
    void Start_Components(void);
    
    /*
    Function used to stop the components
    after double-click
    */
    void Stop_Components(void);
    
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
    Function used to change the colour of the RGB led
    according to the parameter that we are changing in the
    CONFIGURATION MODE
    */
    void Set_Colour_Parameter(uint8_t parameter);
    
    /*
    Function used to convert the position of the potentiometer
    in the correspondent value of the current parameter to set
    */
    void Potentiometer_to_Register(uint8_t parameter, uint8_t value);
    
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
    void Set_Feedback(uint8_t parameter, uint8_t value);
    
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

    
#endif

/* [] END OF FILE */
