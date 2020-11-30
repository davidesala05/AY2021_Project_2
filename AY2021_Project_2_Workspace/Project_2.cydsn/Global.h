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

    #define LIS3DH_TEMP_CFG_REG 0x1F
    #define LIS3DH_TEMP_CFG_REG_INIT 0b00000000

    #define LIS3DH_CTRL_REG1 0x20
    #define LIS3DH_CTRL_REG1_INIT 0b00000111

    #define LIS3DH_CTRL_REG3 0x22
    #define LIS3DH_CTRL_REG3_INIT 0b00010000

    #define LIS3DH_CTRL_REG4 0x23
    #define LIS3DH_CTRL_REG4_INIT 0b00001000

    #define LIS3DH_CTRL_REG5 0x24
    #define LIS3DH_CTRL_REG5_INIT 0b01001010

    #define LIS3DH_CTRL_REG6 0x25
    #define LIS3DH_CTRL_REG6_INIT 0b00101000

    #define LIS3DH_FIFO_CTRL_REG 0x2E
    #define LIS3DH_FIFO_CTRL_REG_INIT 0b11100000

    #define LIS3DH_INT2_CFG 0x34
    #define LIS3DH_INT2_CFG_INIT 0b00101010

    #define LIS3DH_INT2_THS 0x36
    #define LIS3DH_INT2_THS_INIT 0b00001111

    #define LIS3DH_INT2_DURATION 0x37
    #define LIS3DH_INT2_DURATION_INIT 0b00001111
    
    #define LIS3DH_INT2_SRC 0x35

    /******************************************/
    /*            OTHER ADDRESSES             */
    /******************************************/

    #define OUT_X_L 0x28
    #define EEPROM_INTERNAL_ADDRESS 0x0000
    #define MASK_OVERTH_EVENT 0b01000000

    /******************************************/
    /*              OTHER MACROS              */
    /******************************************/

    #define N_REG_ACC   6
    #define G           9.80665
    #define mg_TO_g     0.001
    #define DC_100      65535
    //Sizes of the BUFFER used to send the values
    #define BUFFER_SIZE          4*3 //4(the byte of a float32) * 3(the variables)
    #define TRANSMIT_BUFFER_SIZE 1+BUFFER_SIZE+1
    //TAIL and HEADER added to BUFFER
    #define HEADER 0xA0
    #define TAIL   0xC0
    //Used in the Switch_System_State function
    #define OFF 0
    #define ON 1
    
    #define INITIALIZATION 0
    #define UPDATING 1
    //Used in the Change_Parameters function
    #define SYSTEM_STATE 0
    #define FULL_SCALE 1
    #define DATA_RATE 2

    /******************************************/
    /*            GLOBAL VARIABLES            */
    /******************************************/

    extern uint8_t  reg;             //Used to save the registers content
    extern uint8_t  Register_Param;  //Used to save the 0x0000 register of the INTERNAL EEPROM
    extern uint8_t  flag_ACC;        //Used in the main to sampling
    extern uint8_t  reg_INT2_SRC;

    extern uint8_t  data[6];         //Used to save the acceleration values READ by the MULTIREAD
    extern int16    dataX;           //Used to store the X-axis acceleration in 16bit
    extern int16    dataY;           //Used to store the Y-axis acceleration in 16bit
    extern int16    dataZ;           //Used to store the Z-axis acceleration in 16bit
    extern float32  accX;            //Used to store the X-axis acceleration in float32
    extern float32  accY;            //Used to store the Y-axis acceleration in float32
    extern float32  accZ;            //Used to store the X-axis acceleration in float32

    extern uint8_t  FS_range;        //Used to save the FULL-SCALE range 
    extern uint8_t  Sensitivity;     //Used to save the sensitivity used for the conversion
    extern uint8_t  Verbose_flag;    //Used to the verbose flag option
    extern uint8_t  DataRate;        //Used to save the DATARATE range
    extern uint8_t  System_state;    //Used to save the system state option

    extern uint16_t DC_R;            //Used to save the duty cycle (CompareValue) of the PWM for RED channel
    extern uint16_t DC_G;            //Used to save the duty cycle (CompareValue) of the PWM for GREEN channel
    extern uint16_t DC_B;            //Used to save the duty cycle (CompareValue) of the PWM for BLUE channel

    extern uint8_t Buffer[TRANSMIT_BUFFER_SIZE]; //The BUFFER used to send the values by UART
    
    extern uint8_t flag_overth_event;
    
    extern uint8_t ch_received;
    
    extern uint8_t flag_send_timestamps;

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

    void Register_Initialization(void);

    void Set_RGB(void);

    void Start_Components(void);
    
    void Stop_Components(void);

    void Change_Parameters(uint8_t Parameter);

    void Switch_System_State(uint8_t State);

    
#endif

/* [] END OF FILE */
