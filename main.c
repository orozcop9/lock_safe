 * ---------------------
 * Title: Lock Safe Sensor
 * ---------------------
 * Program Details:
 *  The goal is to create a safe that will open if the correct sequence is
 *  inputted. This input comes in the form of triggering photo resistors by
 *  covering them one at a time. Also a pushbutton must be pressed to confirm
 *  code. If correct code is inputted then a motor will turn.
 * Inputs: RD0, RD1 (photo resistors), RD2 (Push button)
 * Outputs: RC[0:5] (LED outputs), RC6 (Motor Output)
 * Setup: C- Simulator
 * Date: Apr 20, 2024
 * File Dependencies / Libraries: It is required to include the 
 * Configuration Header File 
 * Compiler: xc8, 2.4
 * Author: Pablo Orozco
 * Versions:
 *      V1.0: Original
 * Useful links:  
 *      Datasheet: https://ww1.microchip.com/downloads/en/DeviceDoc/PIC18(L)F26-27-45-46-47-55-56-57K42-Data-Sheet-40001919G.pdf 
 *      PIC18F Instruction Sets: https://onlinelibrary.wiley.com/doi/pdf/10.1002/9781119448457.app4 
 *      List of Instrcutions: http://143.110.227.210/faridfarahmand/sonoma/courses/es310/resources/20140217124422790.pdf 
 */


#include <stdio.h>
#include <stdlib.h>
#include <xc.h> // must have this
#include "../../../../../Applications/Microchip/xc8/v2.41/pic/include/proc/pic18f46k42.h"
#define _XTAL_FREQ 4000000                 // Fosc  frequency for _delayd()  library
#define FCY    _XTAL_FREQ/4

#define PhotoResistor1 PORTCbits.RC0 
#define PhotoResistor2 PORTCbits.RC1
#define Enter_Button   PORTCbits.RC2 
#define Motor          PORTAbits.RA0 
#define LED            PORTD 
#define LED_OUTPUT     PORTAbits.RA1



 char Input1 = 0; 
 char Input2 = 0;
// char BCDValue = 0; 
 char BCD_Val_Low = 0;
 char BCD_Val_High = 0; 
 char Button_Sequence = 0; 
 char Secret_Button = 0x23; 

// Prototype Functions 
 void Receive_Inputs(); 
 void CorrectSequence(); 
 void IncorrectSequence(); 
 void IncrementBCD();
 void IncrementBCD1();
 

void main() {
    PORTC = PORTD = PORTA = 0;
    LATC = LATD = LATA = 0;
    ANSELC = ANSELD = ANSELA = 0;

    TRISC = 0b00000111; // Setting TRISC to 3 inputs
    TRISD = 0b00000000; // setting all of TRISD to inputs 
    TRISA = 0b00000000; // Setting TRISA bits 0 and 1 to inputs
    LED_OUTPUT = 1; // Signals the user that system is on 


    while (1){

        if (Enter_Button == 1) {

            // format Inputs in form of correct sequence (Masking)
            Button_Sequence = (Input2 << 4) + Input1;

            if (Button_Sequence == Secret_Button) {
           CorrectSequence();
            } 
            else {
                IncorrectSequence();
            } 
        }
            else {
             
                Receive_Inputs();

            }
        }
    
    return;
}

void Receive_Inputs() {
    if (PhotoResistor1 == 0) { // Changed from 1 to 0
        Input1++;
        while (PhotoResistor1 == 0) { // Wait while the photoresistor is still blocked
        }

        LED_OUTPUT = 0; // Turn off the LED
        __delay_ms(1000); // 1 second delay 
        LED_OUTPUT = 1; // Turn on the LED
    }
    else if (PhotoResistor2 == 0) { // Changed from 1 to 0
        Input2++;
        while (PhotoResistor2 == 0) { // Wait while the photoresistor is still blocked
        }
        
        LED_OUTPUT = 0; // Turn off the LED
        __delay_ms(1000); // 1 second delay 
        LED_OUTPUT = 1; // Turn on the LED
    }
}


void CorrectSequence () {     
    Motor = 1;    // Motor turns on
                  //  LED_OUTPUT = !LED_OUTPUT; // Signals the user that system is on 

    __delay_ms(3000); // Motor is on for approx 3 seconds 
    
    Motor = 0; // Motor is off 
    
    IncrementBCD(); 
    
    
    // Clear Inputs of the variables 
    Input1 = 0; 
    Input2 = 0; 
    Button_Sequence = 0; 
    
    
}



void IncrementBCD() 
{   
    if (BCD_Val_Low == 9) 
    { 
        BCD_Val_High ++; 
        BCD_Val_Low = 0; 
     }
    
    if (BCD_Val_High == 4)
    { 
        // Reset Count 
        BCD_Val_High = 0; 
        BCD_Val_Low = 0; 
        
    }
    else 
    {
        BCD_Val_Low ++; 
      
    }
    
    LED = (BCD_Val_High << 4) + BCD_Val_Low; //0x39
}



void IncorrectSequence() 
{
    LED_OUTPUT = !LED_OUTPUT; // Signals the user that system is on 

    for(int i = 0; i < 5; i ++ )
    { 
        // Display an alert to the user that the password is wrong 

        LED_OUTPUT = 0; 
        __delay_ms(1000); 
        LED_OUTPUT = 1; 
    }
                                
    Input1 = 0;
    Input2 = 0;
    Button_Sequence = 0; 
}
