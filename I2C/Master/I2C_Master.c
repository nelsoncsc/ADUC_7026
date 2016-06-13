/***************************************************************************

 Author        : Jonatas R. Kinas, Nelson, Nicolau and Rayan

 Date          : May. 2016

 File          : I2C_Master.c

 Hardware      : Applicable to ADuC702x rev H or I silicon
                 Currently targetting ADuC7028.

 Description   : I2C master	to demonstrate with I2C_Slave.c
				 
				 

		
***************************************************************************/

#include<ADuC7028.h>				   

void senddata(short);
void delay(int);
char hex2ascii(char);
void headerSlave(int);



#define count 0x0;				  // Number of bytes to be recieved - 1
int ID=1;			  
int data_slave1=0;
int data_slave2=0;

int main()
{
	GP4DAT = 0x04000000;		 //enable pin p4.2 as output and turn on the led;
	delay(400000);
	GP4DAT = 0x04040000;		 //turn off the led

	//////////////////////////////////////

	POWKEY1 = 0x01;
	POWCON = 0x00;		   		// 41.78MHz
	POWKEY2 = 0xF4;

   	GP0CON = 0x10000000; 		// enable ECLK output on P0.5

    // Start setting up UART at 9600bps

   	COMCON0 = 0x080;			// Setting DLAB
   	COMDIV0 = 0x088;			// Setting DIV0 and DIV1 to DL calculated
   	COMDIV1 = 0x000;
   	COMCON0 = 0x007;			// Clearing DLAB

	///////////////////////////////////////
	

 	GP1CON = 0x2211; 			// I2C1 on P1.2 (SCL) and P1.3 (SDA) 
		    					//for I2C we set GP1CON = 0x2200 and for UART GP1CON = 0x11
								// To achieve both conditions, we mask GP1CON with an OR operation between
								// 0x2200 and 0x11 in which results GP1CON = 0x2211
	
	I2C1CFG = 0x82;		  		// Master Enable & Enable Generation of Master Clock I2C1
	
	// I2C-Master setup
 	
	//I2C1DIV = 0x283C;			// 0x3232 = 400kHz

	I2C1DIV = 0xCFCF;		    // 0xCFCF = 100kHz
		
	I2C1MTX =   0x00;			
	 
	// Recieve
	I2C1CNT = 	count;			// Number of bytes to be read from slave
		
	
	while (1)
	{
		delay(400000);
		if(ID==1){
		I2C1ADR = 	0xA1;
		if((I2C1MSTA & 0x8) == 0x8)				    // Master Recieve IRQ
		{
			data_slave1 = I2C1MRX;
		}
		senddata(data_slave1);
		delay(40000);
		headerSlave(ID);
		delay(40000);
		ID=2;
		}
		if(ID==2){
		I2C1ADR = 	0xA3;
		if((I2C1MSTA & 0x8) == 0x8)				    // Master Recieve IRQ
		{
			data_slave2 = I2C1MRX;
		}
		senddata(data_slave2);
		delay(40000);
		headerSlave(ID);
		delay(40000);
		ID=1;
		}
	}

}

void delay (int length)
{
	while (length >0)
    	length--;
}

void senddata(short to_send)
{
	//while(!(0x020==(COMSTA0 & 0x020))){}
		//COMTX = 0x0A;						// output LF 
	//while(!(0x020==(COMSTA0 & 0x020))){}
		//COMTX = 0x0D;						// output CR 
	while(!(0x020==(COMSTA0 & 0x020))){}
		COMTX = hex2ascii ((to_send >> 8) & 0x0F);
	while(!(0x020==(COMSTA0 & 0x020))){}
		COMTX = hex2ascii ((to_send >> 4) & 0x0F);						
	while(!(0x020==(COMSTA0 & 0x020))){}
		COMTX = hex2ascii (to_send & 0x0F);
	while(!(0x020==(COMSTA0 & 0x020))){}
		COMTX = 0x0A;
	while(!(0x020==(COMSTA0 & 0x020))){}
		COMTX = 0x0D;
			
						
}

char hex2ascii(char toconv)
{
	if (toconv<0x0A) 
	{
		toconv += 0x30;
	}
	else 
	{
		toconv += 0x37;
	}
	
	return (toconv);
}

void headerSlave(int ID){
		
		//COMTX = 0x0A;
		while(!(0x020==(COMSTA0 & 0x020))){}
        COMTX = 0x53;
		while(!(0x020==(COMSTA0 & 0x020))){}
		COMTX = 0x4C;
		while(!(0x020==(COMSTA0 & 0x020))){}
		COMTX = 0x41;
		while(!(0x020==(COMSTA0 & 0x020))){}
		COMTX = 0x56;
		while(!(0x020==(COMSTA0 & 0x020))){}
		COMTX = 0x45;
		while(!(0x020==(COMSTA0 & 0x020))){}
		COMTX = 0x2D;
		while(!(0x020==(COMSTA0 & 0x020))){}
		if(ID == 1)
		  COMTX = 0x31;
		else if(ID == 2)
		  COMTX = 0x32;
		else
		  COMTX = 0x3F;
		while(!(0x020==(COMSTA0 & 0x020))){}
		COMTX = 0x20;
		while(!(0x020==(COMSTA0 & 0x020))){}
		COMTX = 0x2D;
		while(!(0x020==(COMSTA0 & 0x020))){}
		COMTX = 0x2D;
		while(!(0x020==(COMSTA0 & 0x020))){}
		COMTX = 0x3E;
		while(!(0x020==(COMSTA0 & 0x020))){}
		COMTX = 0x20;
		while(!(0x020==(COMSTA0 & 0x020))){}
}

