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

    #define LIS3DH_CTRL_REG0                         0x1E
    #define LIS3DH_CTRL_REG0_INIT                    0b00010000

    #define LIS3DH_TEMP_CFG_REG                      0x1F
    #define LIS3DH_TEMP_CFG_REG_INIT                 0b00000000

    #define LIS3DH_CTRL_REG1                         0x20
    #define LIS3DH_CTRL_REG1_INIT                    0b00000111

    #define LIS3DH_CTRL_REG3                         0x22
    #define LIS3DH_CTRL_REG3_INIT                    0b00000010

    #define LIS3DH_CTRL_REG4                         0x23
    #define LIS3DH_CTRL_REG4_INIT                    0b00001000

    #define LIS3DH_CTRL_REG5                         0x24
    #define LIS3DH_CTRL_REG5_INIT                    0b01000000

    #define LIS3DH_CTRL_REG6                         0x25
    #define LIS3DH_CTRL_REG6_INIT                    0b00101000

    #define LIS3DH_FIFO_CTRL_REG                     0x2E
    #define LIS3DH_FIFO_CTRL_REG_INIT                0b11100000

    #define LIS3DH_INT2_CFG                          0x34
    #define LIS3DH_INT2_CFG_INIT                     0b00101010

    #define LIS3DH_INT2_THS                          0x36
    #define LIS3DH_INT2_THS_INIT                     0b00111111 //a caso

    #define LIS3DH_INT2_DURATION                     0x37
    #define LIS3DH_INT2_DURATION_INIT                0b00000111 //a caso
    
    #define LIS3DH_FIFO_CTRL_REG_BYPASS_MODE         0b00100000
    
    #define LIS3DH_FIFO_CTRL_REG_Stream_to_FIFO_MODE 0b11100000
    

    /******************************************/
    /*            OTHER ADDRESSES             */
    /******************************************/

    #define LIS3DH_DEVICE_ADDRESS                   0x18
    #define OUT_X_L                                 0x28
    #define LIS3DH_INT2_SRC                         0x35
    #define EEPROM_INTERNAL_ADDRESS                 0x0000
    #define EEPROM_EXTERNAL_ADDRESS                 0b01010000
    #define EEPROM_EXTERNAL_START_POINT_WAVEFORM    0x0000
    #define EEPROM_EXTERNAL_START_POINT_TIMESTAMP   0xEA60 //60K
    #define EEPROM_EXTERNAL_START_POINT_SENSITIVITY 0xF618 //63K
    #define EEPROM_EXTERNAL_START_POINT_DATARATE    0xF80C //63.5K

    /******************************************/
    /*                 MACROS                 */
    /******************************************/
    
    #define N_REG_ACC               6
    #define N_REG_WAVEFORM_8bit     192
    #define N_REG_TIMESTAMP         3
    #define G                       9.80665
    #define mg_TO_g                 0.001
    #define DC_100                  65535
    #define DC_0                    0
    #define PERIOD_1Hz              99
    #define PERIOD_2Hz              49
    #define PERIOD_5Hz              19
    #define PERIOD_10Hz             9
    #define BUFFER_SIZE             4*3
    #define TRANSMIT_BUFFER_SIZE    1+BUFFER_SIZE+1
    #define HEADER                  0xA0
    #define TAIL                    0xC0
    #define RED                     0
    #define GREEN                   1
    #define BLUE                    2
    #define FS_RANGE                0
    #define DATARATE                1
    #define VERBOSE_FLAG            2
    #define LONG_PRESSION_DURATION  2 * COUNTS_1_SECOND // 2 seconds of pression
    #define SINGLE_CLICK            1
    #define DOUBLE_CLICK            2
    #define MUX_CHANNEL_COLOUR      0
    #define MUX_CHANNEL_BLINKING    1
    #define MUX_CHANNEL_COLOUR      0
    #define MUX_CHANNEL_BLINKING    1
    #define COUNTS_1_SECOND         200
    #define RUN                     1
    #define WAIT                    -1
    #define BUTTON_PRESSED          0
    #define ONBOARD_LED_ON          1
    #define ONBOARD_LED_OFF         0
    #define IDLE                    0
    #define CM_ENTRY                1
    #define CM_SETPARAMETERS        2
    #define CM_EXIT                 3
    
    /******************************************/
    /*                 MASKS                  */
    /******************************************/

    #define MASK_OVERTH_EVENT           0b01000000
    #define MASK_FS_RANGE_2G            0b00000000
    #define MASK_FS_RANGE_4G            0b00000001
    #define MASK_FS_RANGE_8G            0b00000010
    #define MASK_FS_RANGE_16G           0b00000011
    #define MASK_DATARATE_0Hz           0b00000000
    #define MASK_DATARATE_50Hz          0b00000100
    #define MASK_DATARATE_100Hz         0b00000101
    #define MASK_DATARATE_200Hz         0b00000110
    #define LIS3DH_INT2_THS_2G          0b01111101
    #define LIS3DH_INT2_THS_4G          0b00111110
    #define LIS3DH_INT2_THS_8G          0b00100000
    #define LIS3DH_INT2_THS_16G         0b00001011
    #define LIS3DH_INT2_DURATION_50HZ   0b00001010
    #define LIS3DH_INT2_DURATION_100HZ  0b00010100
    #define LIS3DH_INT2_DURATION_200HZ  0b00101000
    
    /******************************************/
    /*            GLOBAL VARIABLES            */
    /******************************************/

    //FLAGS
    extern uint8_t flag_ACC;        //Used in the main to sampling
    extern uint8_t flag_overth_event;    //Used to save if an isr called by the accelerometer is cause by an overthreshold event
    extern uint8_t flag_send_timestamps; //flag used to send the timestamp (if ch_received is "T" or "t")
    extern uint8_t flag_send_waveform;   //flag used to send the waveforms (if ch_received is "B" or "b")
    extern uint8_t flag_blinking;
    extern uint8_t flag_doubleclick;
    extern uint8_t flag_singleclick;
    extern uint8_t flag_longpression;
    extern uint8_t flag_shortdistance;
    extern uint8_t flag_fastclick;
    extern uint8_t flag_sampling_pot;
    
    //COUNTERS
    extern uint16_t count_button_press;
    extern uint16_t count_button_rel;
    extern uint8_t  count_overth_event;
    extern uint8_t  count_global;
    extern uint8_t  count_waveform;
    extern uint8_t  count_for_plotting;
    
    //REGISTERS
    extern uint8_t  reg;             //Used to save the registers content
    extern uint8_t  reg_INT2_SRC;    //Used to save the INT2_SRC register content
    extern uint8_t  FS_range_reg;    //Used to save the FULL-SCALE range REGISTER
    extern uint8_t  DataRate_reg;    //Used to save the DATARATE range REGISTER
    
    //ACCELERATION
    extern uint8_t  data[N_REG_ACC];         //Used to save the acceleration values READ by the MULTIREAD
    extern int16    dataX;           //Used to store the X-axis acceleration in 16bit
    extern int16    dataY;           //Used to store the Y-axis acceleration in 16bit
    extern int16    dataZ;           //Used to store the Z-axis acceleration in 16bit
    extern float32  accX;            //Used to store the X-axis acceleration in float32
    extern float32  accY;            //Used to store the Y-axis acceleration in float32
    extern float32  accZ;            //Used to store the X-axis acceleration in float32
    extern uint8_t  Buffer[TRANSMIT_BUFFER_SIZE]; //The BUFFER used to send the values by UART
    
    //PARAMETERS
    extern uint8_t  FS_range_value;  //Used to save the FULL-SCALE range VALUE
    extern uint8_t  DataRate_value;  //Used to save the DATARATE range VALUE
    extern uint8_t  Sensitivity;     //Used to save the sensitivity used for the conversion
    extern uint8_t  Verbose_flag;    //Used to the verbose flag option
    extern int16_t  potentiometer_value;
    extern uint8_t  parameter_selected;
    
    //COLOUR
    extern uint16_t DC_R;            //Used to save the duty cycle (CompareValue) of the PWM for RED channel
    extern uint16_t DC_G;            //Used to save the duty cycle (CompareValue) of the PWM for GREEN channel
    extern uint16_t DC_B;            //Used to save the duty cycle (CompareValue) of the PWM for BLUE channel

    //WAVEFORM
    extern uint8_t waveform_8bit[N_REG_WAVEFORM_8bit];
    extern uint8_t waveform_8bit_to_write[N_REG_WAVEFORM_8bit];
    
    //TIMESTAMP
    extern uint8_t seconds;
    extern uint8_t minutes;
    extern uint8_t hours;
    extern uint8_t timestamp_to_write[3];
    
    //OTHERS
    extern uint8_t ch_received;          //Variable used to save the character received by the UART
    extern int8_t  device_state;
    extern uint8_t configurationmode_state;
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
