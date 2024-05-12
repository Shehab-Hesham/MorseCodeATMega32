/*
 * main.c
 *
 *  Created on: Oct 14, 2023
 *      Author: SHAHB
 */
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "util/delay.h"
#include "Drivers/STD.h"
#include "Drivers/BITMATH.h"
#include "Drivers/MCAL/DIO/DIO_AD.h"
#include "Drivers/MCAL/Interrupts/GLOBAL_INTERRUPT.h"
#include "Drivers/MCAL/Interrupts/EXT_INTERRUPT.h"
#include "Drivers/MCAL/ADC/ADC_INIT.h"
#include "Drivers/MCAL/Timer/TIMER0_INT.h"
#include "Drivers/MCAL/Timer/MCU_HW_Timer1.h"
#include "Drivers/MCAL/Timer/Timer2.h"
#include "Drivers/MCAL/UART/UART_CINFIG.h"
#include "Drivers/MCAL/UART/UART_PRIV.h"
#include "Drivers/MCAL/UART/PROTOTYPES.h"
#include "Drivers/MCAL/SPI/SPI_HW.h"
#include "Drivers/MCAL/I2C/I2C_HW.h"
#include "Drivers/HAL/EEPROM/PROTOTYPES.h"
#include "Drivers/HAL/Keypad/KEYPAD_INIT.h"
#include "Drivers/HAL/LCD/LCD_Init.h"

#define LED_1 DIO_B7

#define trialConst 500
#define dit 1 * trialConst
#define dah 3 * dit
#define interSpace dah
#define intraSpace dit
#define wordSpace (2 * dah) + dit

//int8 toLower(int8 letter){
//	if (letter >= 'A' && letter <= 'Z') {
//		// Convert uppercase letter to lowercase
//		return letter + ('a' - 'A');
//	}
//	return letter;
//}
int8* letterToMorse(int8 letter) {
    switch (tolower(letter)) {
//    switch (toLower(letter)) {
        case 'a': return ".-";
        case 'b': return "-...";
        case 'c': return "-.-.";
        case 'd': return "-..";
        case 'e': return ".";
        case 'f': return "..-.";
        case 'g': return "--.";
        case 'h': return "....";
        case 'i': return "..";
        case 'j': return ".---";
        case 'k': return "-.-";
        case 'l': return ".-..";
        case 'm': return "--";
        case 'n': return "-.";
        case 'o': return "---";
        case 'p': return ".--.";
        case 'q': return "--.-";
        case 'r': return ".-.";
        case 's': return "...";
        case 't': return "-";
        case 'u': return "..-";
        case 'v': return "...-";
        case 'w': return ".--";
        case 'x': return "-..-";
        case 'y': return "-.--";
        case 'z': return "--..";
        case ' ': return " ";  // Space
        default: return "";     // Unknown character
    }
}
void morseLetterDisplayer(int8* msg){ // in dits and dahs
	for (uint8 i = 0; i < strlen(msg); i++){
		DIO_set(LED_1, DIO_HIGH);
		if(msg[i] == '.'){
			_delay_ms(dit);
		} else if(msg[i] == '-'){
			_delay_ms(dah);
		}
		DIO_set(LED_1, DIO_LOW);
		if(i != strlen(msg)-1){ // last character in letter doesn't need space after
			_delay_ms(intraSpace);
		}
	}
}
void morseWordDisplayer(int8* word){ // in alphabetic
	int8* letter;
	for (uint8 i = 0; i < strlen(word); i++){
		letter = letterToMorse(word[i]);
		morseLetterDisplayer(letter);
		if(i != strlen(word)-1){ // last letter in word doesn't need space after
			_delay_ms(interSpace);
		}
	}
	_delay_ms(wordSpace);
}
void morseInit(){

	DIO_direction(LED_1, DIO_OUTPUT) ;
	DIO_set(LED_1, DIO_LOW);

	LCD_void_init() ;
	UART_init();

}
void main ()
{
	morseInit();
	uint8 letter;
	int8 msg[46] = "" ; //longest english word length: Pneumonoultramicroscopicsilicovolcanoconiosis
//	LCD_void_sendString(msg);
//	morseWordDisplayer(msg);
//	LCD_void_clear();
		while (1)
		{
			letter = UART_recieveData();
//			LCD_void_sendData(letter);
			if (letter != ']'){
//				LCD_void_sendData(letter);
//				dummy = (int8) letter;
				strncat(msg, &letter, 1);
				//LCD_void_sendData(letter);
				if(letter == ' '){
					LCD_void_clear();
					LCD_void_sendString(msg);
					morseWordDisplayer(msg);
					msg[0] = '\0';
					LCD_void_clear();
				}
			}
		}
}
