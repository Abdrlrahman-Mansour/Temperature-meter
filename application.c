/* 
 * File:   application.c
 * Author: Abdelrahman Mansour
 *
 * Created on August 13, 2024, 5:13 AM
 */
#include "application.h"

Std_ReturnType ret = E_NOT_OK;
uint16 adc_temp =0;
uint16 lcd_temp1 =0;      
uint16 lcd_temp2 =0;      
        
        
lcd_8bit_t lcd_obj = {
  .lcd_rs.port = PORTA_INDEX,
  .lcd_rs.pin = PIN5,
  .lcd_rs.direction = GPIO_OUTPUT ,
  .lcd_rs.direction = LOW ,
  .lcd_en.port = PORTA_INDEX,
  .lcd_en.pin = PIN6,
  .lcd_en.direction = GPIO_OUTPUT ,
  .lcd_en.direction = LOW ,
  .lcd_data[0].port = PORTC_INDEX,
  .lcd_data[0].pin = PIN0,
  .lcd_data[0].direction = GPIO_OUTPUT,
  .lcd_data[0].logic = LOW,
  .lcd_data[1].port = PORTC_INDEX,
  .lcd_data[1].pin = PIN1,
  .lcd_data[1].direction = GPIO_OUTPUT,
  .lcd_data[1].logic = LOW,
  .lcd_data[2].port = PORTC_INDEX,
  .lcd_data[2].pin = PIN2,
  .lcd_data[2].direction = GPIO_OUTPUT,
  .lcd_data[2].logic = LOW,
  .lcd_data[3].port = PORTC_INDEX,
  .lcd_data[3].pin = PIN3,
  .lcd_data[3].direction = GPIO_OUTPUT,
  .lcd_data[3].logic = LOW,
  .lcd_data[4].port = PORTC_INDEX,
  .lcd_data[4].pin = PIN4,
  .lcd_data[4].direction = GPIO_OUTPUT,
  .lcd_data[4].logic = LOW,
  .lcd_data[5].port = PORTC_INDEX,
  .lcd_data[5].pin = PIN5,
  .lcd_data[5].direction = GPIO_OUTPUT,
  .lcd_data[5].logic = LOW,
  .lcd_data[6].port = PORTC_INDEX,
  .lcd_data[6].pin = PIN6,
  .lcd_data[6].direction = GPIO_OUTPUT,
  .lcd_data[6].logic = LOW,
  .lcd_data[7].port = PORTC_INDEX,
  .lcd_data[7].pin = PIN7,
  .lcd_data[7].direction = GPIO_OUTPUT,
  .lcd_data[7].logic = LOW,
};

adc_config_t adc_1 = {
    .ADC_InterruptHandler = NULL,
    .acquisition_time = ADC_12_TAD,
    .channel_select = ADC_CHANNEL_AN0,
    .convertion_clock = ADC_FOSC_DIV_16,
    .result_format = ADC_RESULT_RIGHT,
    .volt_ref = ADC_VOLT_REF_DESABLE
};

motor_t HEATER = {
    .motor[0].motor_port = PORTD_INDEX,
    .motor[0].motor_pin = PIN0,
    .motor[0].motor_status = MOTOR_OFF_STATUS,
    
    .motor[1].motor_port = PORTD_INDEX,
    .motor[1].motor_pin = PIN1,
    .motor[1].motor_status = MOTOR_OFF_STATUS,
    
};

motor_t AIR_CONDITIONER = {
    .motor[0].motor_port = PORTD_INDEX,
    .motor[0].motor_pin = PIN2,
    .motor[0].motor_status = MOTOR_OFF_STATUS,
    
    .motor[1].motor_port = PORTD_INDEX,
    .motor[1].motor_pin = PIN3,
    .motor[1].motor_status = MOTOR_OFF_STATUS,
    
};



int main() {
    ret = lcd_8bit_init(&lcd_obj);
    ret = ADC_Init(&adc_1);
    ret = lcd_8bit_send_string_pos(&lcd_obj,1,1,"Temperature:");
    ret = motor_init(&HEATER);
    ret = motor_init(&AIR_CONDITIONER);
    
        
    while(1){
        ret = ADC_GetConversion_Blocking(&adc_1, ADC_CHANNEL_AN0, &adc_temp);
        adc_temp = adc_temp * 0.4878048780487805;
        lcd_temp1 = (adc_temp/10) + 48;
        lcd_temp2 = (adc_temp%10) + 48;
        lcd_8bit_send_char_data_pos(&lcd_obj,1,13,lcd_temp1);
        lcd_8bit_send_char_data_pos(&lcd_obj,1,14,lcd_temp2);
        lcd_8bit_send_char_data_pos(&lcd_obj,1,15,'C');
        
        if(adc_temp < 20){
            motor_move_right(&HEATER);
        }
        else if(adc_temp > 30){
             motor_move_right(&AIR_CONDITIONER);
        }
        else{
            motor_stop(&AIR_CONDITIONER);
            motor_stop(&HEATER);
        }
    
    }
    
    return (EXIT_SUCCESS);
}

