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


    /******************************************/
    /*        CONFIGURATION REGISTERS         */
    /******************************************/

    #define LIS3DH_DEVICE_ADDRESS    0x18

    #define LIS3DH_CTRL_REG0 0x1E
    #define LIS3DH_CTRL_REG0_INIT 0b00010000

    #define LIS3DH_CTRL_REG1 0x20
    #define LIS3DH_CTRL_REG1_INIT 0b00000111

    #define LIS3DH_CTRL_REG3 0x22
    #define LIS3DH_CTRL_REG3_INIT 0b00010000

    #define LIS3DH_CTRL_REG4 0x23
    #define LIS3DH_CTRL_REG4_INIT 0b00001000

    #define LIS3DH_CTRL_REG5 0x24
    #define LIS3DH_CTRL_REG5_INIT 0b01000010

    #define LIS3DH_CTRL_REG6 0x25
    #define LIS3DH_CTRL_REG6_INIT 0b00101000

    #define LIS3DH_FIFO_CTRL_REG 0x2E
    #define LIS3DH_FIFO_CTRL_REG_INIT 0b10000000

    #define LIS3DH_INT2_CFG 0x34
    #define LIS3DH_INT2_CFG_INIT 0b00101010

    #define LIS3DH_INT2_THS 0x36
    #define LIS3DH_INT2_THS_INIT 0b00111111 
    #define LIS3DH_INT2_THS_2G 0b01111101
    #define LIS3DH_INT2_THS_4G 0b00111110
    #define LIS3DH_INT2_THS_8G 0b00100000
    #define LIS3DH_INT2_THS_16G 0b00001011

    #define LIS3DH_INT2_DURATION 0x37
    #define LIS3DH_INT2_DURATION_INIT 0b00000111 //a caso
    #define LIS3DH_INT2_DURATION_50HZ 0b00001010
    #define LIS3DH_INT2_DURATION_100HZ 0b00010100
    #define LIS3DH_INT2_DURATION_200HZ 0b00101000
    

    /******************************************/
    /*            OTHER ADDRESSES             */
    /******************************************/

    #define OUT_X_L 0x28
    
    #define LIS3DH_INT2_SRC 0x35
    
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
    #define DC_50       32767
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
    extern uint8_t Register_Param;   //Used to save the 0x0000 register of the INTERNAL EEPROM
    extern uint8_t  flag_ACC;        //Used in the main to sampling
    extern uint8_t  reg_INT2_SRC;
    
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
    
    extern uint16_t DC_R;            //Used to save the duty cycle (CompareValue) of the PWM for RED channel
    extern uint16_t DC_G;            //Used to save the duty cycle (CompareValue) of the PWM for GREEN channel
    extern uint16_t DC_B;            //Used to save the duty cycle (CompareValue) of the PWM for BLUE channel

    extern uint8_t Buffer[TRANSMIT_BUFFER_SIZE]; //The BUFFER used to send the values by UART
    
    extern uint8_t flag_overth_event;    //Used to save if an isr called by the accelerometer is cause by an overthreshold event
    extern uint8_t ch_received;          //Variable used to save the character received by the UART
    extern uint8_t flag_send_timestamps; //flag used to send the timestamp (if ch_received is "B" or "b")

    
    extern uint8_t flag_send_timestamps;
    
    void Register_Initialization(void);

    void Initialize_Parameters(void);
        
    /*
    Function used to convert the register used to
    change the parameter (for the EEPROM saving)
    in the real value of the parameter (used for the conversions)
    */
    void Register_to_value(void);
    
    void Set_FS_Registers (void);
    
    void Set_Duration_Registers (void);

#endif


/* [] END OF FILE */
