/***************************************************************************

 Author        : Jonatas R. Kinas, Nelson, Nicolau and Rayan

 Date          : May. 2016

 File          : I2C_Slave.c

 Hardware      : Applicable to ADuC702x rev H or I silicon
                 Currently targetting ADuC7028.

 Description   : I2C Slave to demonstrate with I2C_Master.
				 
				 

		
***************************************************************************/

#include<ADuC7028.h>

void delay(int);
void My_IRQ_Handler(void);
void ADCpoweron(int);

int i = 0, dat[9],data_adc;


int main(){
	GP4DAT = 0x04000000;		//enable pin p4.2 as output and turn on the led;
	delay(400000);
	GP4DAT = 0x04040000;	    //turn off the led
	
	////////////////////////
	
	// ADC and DAC configuration
	ADCpoweron(20000);			// power on ADC	
	ADCCP = 0x00;				// conversion on ADC0
	DAC1CON = 0x13;				// AGND-AVDD range
	REFCON = 0x00;				// internal 2.5V reference
	
	///////////////////////	


	// I2C1 on P1.2 (scl) and P1.3 (sda)
 	GP1CON = 0x2200;
	I2C1CFG = 0x01;		  		// Slave Enable

	ADCCON = 0x7E3;				// ADC config: fADC/2, acq. time = 16 clocks => ADC Speed = 1MSPS
	while (!ADCSTA){}			// wait for end of conversion
	data_adc=ADCDAT >> 16;

	I2C1ID0 = 0xA2; 			//(LSB = 0 means that the slave is writing on the master)	// Slave ID
	I2C1STX = data_adc; 		// the first value the slave is writing into the master
		

	while (1)
	{
		ADCCON = 0x7E3;			// ADC config: fADC/2, acq. time = 16 clocks => ADC Speed = 1MSPS
		while (!ADCSTA){}		// wait for end of conversion
		data_adc=ADCDAT >> 16;
		I2C1STX = data_adc;
	}; 

}

 void delay (int length){
	while (length >0)
    	length--;
}

void ADCpoweron(int time)
{
	ADCCON = 0x620;	 					// power-on the ADC
	while (time >=0)	  				// wait for ADC to be fully powered on
    time--;
}
