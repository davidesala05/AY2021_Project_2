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
    #include "I2C_Interface_ACC.h"
    #include "stdlib.h"
    #include "stdio.h"
    #include "InterruptRoutines.h"
    #include "I2C_Interface_EXTERNAL_EEPROM.h"
    #include "HardwareMenu.h"

    /******************************************/
    /*        CONFIGURATION REGISTERS         */
    /******************************************/

    #define LIS3DH_CTRL_REG0 0x1E
    #define LIS3DH_CTRL_REG0_INIT 0b00010000

    #define LIS3DH_TEMP_CFG_REG 0x1F
    #define LIS3DH_TEMP_CFG_REG_INIT 0b00000000

    #define LIS3DH_CTRL_REG1 0x20
    #define LIS3DH_CTRL_REG1_INIT 0b00000111

    #define LIS3DH_CTRL_REG3 0x22
    #define LIS3DH_CTRL_REG3_INIT 0b00000010 //ISR is generated on the FIFO OVERRUN, in this way the sampling is stopped after an event!!!

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
    
    #define LIS3DH_FIFO_CTRL_REG_BYPASS_MODE 0b00100000
    
    #define LIS3DH_FIFO_CTRL_REG_Stream_to_FIFO_MODE 0b11100000
    

    /******************************************/
    /*            OTHER ADDRESSES             */
    /******************************************/

    #define LIS3DH_DEVICE_ADDRESS    0x18
    
    #define OUT_X_L 0x28
    
    #define LIS3DH_INT2_SRC 0x35
    
    #define EEPROM_INTERNAL_ADDRESS 0x0000
    
    #define EEPROM_EXTERNAL_ADDRESS 0b01010000 //7-bit right justified
    
    #define EEPROM_EXTERNAL_START_POINT_WAVEFORM 0x0000
    
    #define EEPROM_EXTERNAL_START_POINT_TIMESTAMP 0xEA60 //60K
    
    #define EEPROM_EXTERNAL_START_POINT_SENSITIVITY 0xF618 //63K
    
    #define EEPROM_EXTERNAL_START_POINT_DATARATE 0xF80C //63.5K

    /******************************************/
    /*              OTHER MACROS              */
    /******************************************/
    
    //Number of register to read for acceleration values
    #define N_REG_ACC   6
    //Number of register to read all the FIFO register after an overthreshold event
    #define N_REG_WAVEFORM_8bit 192
    //Number of register to write all the TIMESTAMP in the EXTERNAL EEPROM
    #define N_REG_TIMESTAMP 3
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
    // Defines related to the different conditions of the PushButton component when it is pressed
    #define LONG_PRESSION_DURATION  2 * COUNTS_1_SECOND // 2 seconds of pression
    #define SINGLE_CLICK            1
    #define DOUBLE_CLICK            2
    
    /* Defines related to the different channels of the MUX component in order to link the correct clock signal to the PWM
    components according to the device state */
    #define MUX_CHANNEL_COLOUR      0
    #define MUX_CHANNEL_BLINKING    1
    //Masks used for the control register
    #define MUX_CHANNEL_COLOUR 0
    #define MUX_CHANNEL_BLINKING 1
    // Define related to the frequency of interrupt generation related to the Timer component
    #define COUNTS_1_SECOND     200
    // Defines related to the different states of the model 
    #define RUN                 1
    #define WAIT                -1
    // Defines related to the different states of the PushButton component
    #define BUTTON_PRESSED      0
    // Defines related to the different states of the OnBoardLED component
    #define ONBOARD_LED_ON      1
    #define ONBOARD_LED_OFF     0
    // Defines related to the different steps of the configuration mode
    #define CM_ENTRY            0
    #define CM_SETPARAMETERS    1
    #define CM_EXIT             2
    #define IDLE                3
    
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
    #define MASK_DATARATE_0Hz   0b00000000
    #define MASK_DATARATE_50Hz  0b00000100
    #define MASK_DATARATE_100Hz 0b00000101
    #define MASK_DATARATE_200Hz 0b00000110
    
    
    #define LIS3DH_INT2_THS_2G  0b01111101
    #define LIS3DH_INT2_THS_4G  0b00111110
    #define LIS3DH_INT2_THS_8G  0b00100000
    #define LIS3DH_INT2_THS_16G 0b00001011
    
    #define LIS3DH_INT2_DURATION_50HZ  0b00001010
    #define LIS3DH_INT2_DURATION_100HZ 0b00010100
    #define LIS3DH_INT2_DURATION_200HZ 0b00101000
    
    /******************************************/
    /*            GLOBAL VARIABLES            */
    /******************************************/

    extern uint8_t  reg;             //Used to save the registers content
    extern uint8_t  Register_Param;  //Used to save the 0x0000 register of the INTERNAL EEPROM
    extern uint8_t  flag_ACC;        //Used in the main to sampling
    extern uint8_t  reg_INT2_SRC;    //Used to save the INT2_SRC register content

    extern uint8_t  data[N_REG_ACC];         //Used to save the acceleration values READ by the MULTIREAD
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
    extern uint8_t flag_send_timestamps; //flag used to send the timestamp (if ch_received is "T" or "t")
    extern uint8_t flag_send_waveform;   //flag used to send the waveforms (if ch_received is "B" or "b")

    extern uint8_t waveform_8bit[N_REG_WAVEFORM_8bit];
    
    extern uint8_t waveform_8bit_to_write[N_REG_WAVEFORM_8bit];
    
    extern uint8_t count_overth_event;
    
    extern uint8_t timestamp_to_write[3];
    
    extern uint8_t flag_not_change;
    
    extern uint8_t old_value;
    // Variable related to the state of the device --> START or STOP conditions
    extern int8_t device_state; /* Defined as integer (and not as uint) because it can assume both
    positive and negative values: RUN and WAIT are opposite values */
    
    // Variables related to the time measurement
    extern uint8_t count_global;
    extern uint8_t seconds;
    extern uint8_t minutes;
    extern uint8_t hours;
    
    /* Variable related to the selection of the different parameters that is possible to change in the configuration mode
    of the hardware menu */
    extern uint8_t parameter_selected;
    
    // Variable related to the parameters according to the potentiometer value
    extern int16_t potentiometer_value;
    
    // Flag variables
    extern uint8_t flag_isbuttonpressed;
    extern uint8_t flag_configurationmode;
    //extern uint8_t flag_sampling;
    extern uint8_t flag_blinking;
    extern uint16_t count_button_press;
    extern uint16_t count_button_rel;
    extern uint8_t flag_doubleclick;
    extern uint8_t flag_singleclick;
    extern uint8_t flag_longpression;
    extern uint8_t flag_shortdistance;
    extern uint8_t flag_fastclick;
    extern uint8_t flag_sampling_pot;
    
    extern uint8_t count_waveform;
    extern uint8_t count_for_plotting;
    /*
    Below the UNION used to store the values after the conversion in 32bit is declared
    - DataUnion.f is used to stored the float32 value
    - DataUnion.l is used to extract the byte to saved in Buffer[i]
    */

    union FloatUnion {
        float32 f;
        uint32_t l;
    }DataUnion;

    
#endif

/* [] END OF FILE */
