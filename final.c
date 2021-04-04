#include <REG51.H>


#include <string.h>

unsigned char roomTemp[] = "Temperature: "; //First line of the LCD
unsigned char roomTempS[] = "TemperaturS: "; //Second line of the LCD TEMP MENU
unsigned char roomHumidity[]="Humidity: "; //Second line of the LCD
unsigned char roomHumidityS[]="HumiditS: "; //Second line of the LCD HUM MENU
unsigned int index; //Index variable for going through the predefined strings

unsigned char tempRead;							//Store the value from the Temperature Sensor (ADC)
unsigned char tempReadL;						//LSB part
unsigned char tempReadH;						//MSB part

unsigned char tempSet=0x15; //Set the base temperature to 21 degree Celsius;		          
																//Store the base temperature
unsigned char humSet=0x2D; //Set the base humidity to 45 %								
									//Store the base humidity

unsigned char humRead;							//Store the value from the Humidity Sensor (ADC)
unsigned char humReadL;					   	//LSB part
unsigned char humReadH;							//MSB part

//Configuring the E, RW and RS pins of the LCD

sbit E = P3^0;
sbit RW = P3^1;
sbit RS = P3^4;
sbit relTemp = P1^7;
sbit relHum = P1^6;
sbit ADA = P1^0;

sbit menuTemp = P3^2;
sbit menuHum = P3^3;
sbit plusValue = P3^5;
sbit minusValue = P3^6;

void controlLCD(unsigned char value); //Prototype of the function to control the LCD
void dataLCD(unsigned char value); //Prototype of the function for data to be shown on the LCD
void delay(unsigned char TH, unsigned char TL); //Prototype of the function for the delay

void delay2(int cnt);

void initLCD(); //Prototype of the function to initialise the LCD


void main(void){
	
	P1 = 0xFF; //Set the PORT 1 as input port ADC
	P0 = 0xFF; //Set the PORT 0 as input port ADC
	RW = 0; //Set the RW pin on 0
	
	initLCD();
	
	
	while(1){
		 //initLCD();
		 ADA =0;
		 delay2(1000); //am folosit delay2 pentru a crea mai usor delayuri de secunde
		 tempRead = P0;											//Temperature value from ADC ( PORT1)
		 tempRead = tempRead/0x05;					//After testing, we should divide with 5 to obtain the correct temperature
		 if(tempRead >= 37)									//We have an error when the temperature is greater or equl to
			tempRead -= 1;										//37 degree so we will substract 1 
		 
		 tempReadH = (tempRead/10) + 0x30;	//First digit of the temperature in hexa 
		 tempReadL = (tempRead%10) + 0x30;	//Second digit of the temperature in hexa 
		 	
		 controlLCD(0x8C);									//Set the curson on line 1, pozition 12 (After the message "Temperature: "
		 delay(0xF8, 0xCD);
		 dataLCD(tempReadH);								//Display the first digit
		 delay(0xFF, 0xD2);
		 
		 dataLCD(tempReadL);								//Display the second digit
		 delay(0xFF, 0xD2);
		 
		 dataLCD(0XDF);										//Display degree symbol
		 delay(0xFF, 0xD2);
		 dataLCD('C');											//Display C from Celsius
		 delay(0xFF, 0xD2);
		if(tempRead < tempSet)
			relTemp = 0;
		else 
			relTemp = 1;
	  
	 
		 ADA=1;
		 //Humidity
			delay2(3500); //Delay of 5 ms to obtain the value for humidity. We need a longer delay because humidity
										//sensor takes more time to stabilize.
		 humRead = P0;											//Humidity value from ADC ( PORT0)
		
		 humRead = humRead/0x04;						//After testing, we should divide with 4 to obtain the correct humidity
			if(humRead >= 16)									//We have an error when the humidity is greater or equl to 16
			humRead -= 1;											//so we will substract 1
			if(humRead >= 31)									//We have an error when the humidity is greater or equl to 31
			humRead -= 1;											//so we will substract 1
			if(humRead >= 47)									//We have an error when the humidity is greater or equl to 47
			humRead -= 1;											//so we will substract 1
					
		 humReadH = (humRead/10) + 0x30;	//First digit of the temperature in hexa 
		 humReadL = (humRead%10) + 0x30;	//Second digit of the temperature in hexa 
		 
		 controlLCD(0xCC);								//Set the curson on line 2, pozition 13 (After the message "Humidity:   "
		 delay(0xF8, 0xCD);
		 dataLCD(humReadH);								//Display the first digit
		 delay(0xFF, 0xD2);
		 
		 dataLCD(humReadL);								//Display the second digit
		 delay(0xFF, 0xD2);
	
		 dataLCD(' ');											//Space
		 delay(0xFF, 0xD2);
		 dataLCD('%');											//%
		 delay(0xFF, 0xD2);	
		if(humRead < humSet)
			relHum = 0;
		else 
			relHum = 1;
		
		if(menuTemp == 0){
			controlLCD(0x01);	  // Clear LCD
			controlLCD(0x80);				//Setting the cursor on the first line, first position
	
	//Displaying predefined message "Temperature:" 
	index = 0;
	while(roomTemp[index] != NULL){		//going through the string till the NULL terminator

		dataLCD(roomTemp[index]);			//display each character of the string on the LCD
		index++; //increment the index 
	}
	
	controlLCD(0xC0);				//Setting the cursor on the second line, first position
	
	//Displaying predefined message "Temperature S:"
	index = 0;
	while(roomTempS[index] != NULL){		//going through the string till the NULL terminator
		dataLCD(roomTempS[index]);			//display each character of the string on the LCD
		index++;//increment the index 
	}
		 controlLCD(0x8C);									//Set the curson on line 1, pozition 12 (After the message "Temperature: "
		 delay(0xF8, 0xCD);
		 dataLCD(tempReadH);								//Display the first digit
		 delay(0xFF, 0xD2);
		 
		 dataLCD(tempReadL);								//Display the second digit
		 delay(0xFF, 0xD2);
		 
		 dataLCD(0XDF);										//Display degree symbol
		 delay(0xFF, 0xD2);
		 dataLCD('C');											//Display C from Celsius
		 delay(0xFF, 0xD2);
			delay2(3000);
	if(plusValue == 0)
			tempSet++;
		if(minusValue==0)
			tempSet--;
		 tempReadH = (tempSet/10) + 0x30;	//First digit of the temperature in hexa 
		 tempReadL = (tempSet%10) + 0x30;	//Second digit of the temperature in hexa 
	
	   controlLCD(0xCC);								//Set the curson on line 2, pozition 13 (After the message "Temperature:   "
		 delay(0xF8, 0xCD);
		 dataLCD(tempReadH);								//Display the first digit
		 delay(0xFF, 0xD2);
		 
		 dataLCD(tempReadL);								//Display the second digit
		 delay(0xFF, 0xD2);
	
		 dataLCD(0XDF);										//Display degree symbol
		 delay(0xFF, 0xD2);
		 dataLCD('C');											//Display C from Celsius
		 delay(0xFF, 0xD2);
		
		
	}	
		delay2(2000);
		
			if(menuHum == 0){
			controlLCD(0x01);	  // Clear LCD
			controlLCD(0x80);				//Setting the cursor on the first line, first position
	
	//Displaying predefined message "Temperature:" 
	index = 0;
	while(roomHumidity[index] != NULL){		//going through the string till the NULL terminator

		dataLCD(roomHumidity[index]);			//display each character of the string on the LCD
		index++; //increment the index 
	}
	
	controlLCD(0xC0);				//Setting the cursor on the second line, first position
	
	//Displaying predefined message "HumiditS:"
	index = 0;
	while(roomHumidityS[index] != NULL){		//going through the string till the NULL terminator
		dataLCD(roomHumidityS[index]);			//display each character of the string on the LCD
		index++;//increment the index 
	}
		 controlLCD(0x8C);									//Set the curson on line 1, pozition 12 (After the message "Temperature: "
		 delay(0xF8, 0xCD);
		 dataLCD(humReadH);								//Display the first digit
		 delay(0xFF, 0xD2);
		 
		 dataLCD(humReadL);								//Display the second digit
		 delay(0xFF, 0xD2);
		 
		 dataLCD(' ');											//Space
		 delay(0xFF, 0xD2);
		 dataLCD('%');											//%
		 delay(0xFF, 0xD2);	
			delay2(3000);
	if(plusValue == 0)
			humSet++;
		if(minusValue==0)
			humSet--;
		 humReadH = (humSet/10) + 0x30;	//First digit of the temperature in hexa 
		 humReadL = (humSet%10) + 0x30;	//Second digit of the temperature in hexa 
	
	   controlLCD(0xCC);								//Set the curson on line 2, pozition 13 (After the message "HumiditS:   "
		 delay(0xF8, 0xCD);
		 dataLCD(humReadH);								//Display the first digit
		 delay(0xFF, 0xD2);
		 
		 dataLCD(humReadL);								//Display the second digit
		 delay(0xFF, 0xD2);
	
		  dataLCD(' ');											//Space
		 delay(0xFF, 0xD2);
		 dataLCD('%');											//%
		 delay(0xFF, 0xD2);	
}
			delay2(1000);
			initLCD();

	}
}
void controlLCD(unsigned char value){
	 
	 P2 = value;							//Take the read value from port 2
	 RS = 0;									//Set the RS bit in 0 in order to slect the
														//mode of transmitting the command
	 E = 1;										//Enable
	 delay(0xF8, 0xCD);				//Delay
	 E = 0;										//Disable
 
	}

 void dataLCD(unsigned char value)
 {
	
	 P2 = value;							//Take the read value from port 2 (from the ADC)
	 RS = 1;									//Set the RS bit in 0 in order to select the
														//mode of transmitting the command.
	 E = 1;										//Enable
	 delay(0xFF, 0xD2);				//Delay
	 E = 0;										//Disable
	 
 }
 void delay(unsigned char TH, unsigned char TL){
	TMOD = 0x10;					//We use the timer 1 in mod 1 
	TH1 = TH;							//We introduce the high part of the start value 
	TL1 = TL;							//We introduce the low part of the start value
	TR1 = 1;							//Start the timer. 
	while(TF1 == 0){} 		//We wait untill it finish to count.
	TF1 = 0;							//Delete the overflow flag of te timer.
	TR1 = 0;							//Stop the timer.
}
 

// 1 ms delay function
void delay2(int cnt)
{
    unsigned int i, j;
		for(j=0; j<cnt; j++)
		for(i=0; i<112; i++);
} 

void initLCD(){
	//LCD initialization
	controlLCD(0x38);				//Using 2 lines (5x7)
	controlLCD(0x01);				//Clear LCD
	controlLCD(0x0C);				//Blink cursor - off
	
	//LCD LINE 1
	controlLCD(0x80);				//Setting the cursor on the first line, first position
	
	//Displaying predefined message "Temperature:" 
	index = 0;
	while(roomTemp[index] != NULL){		//going through the string till the NULL terminator

		dataLCD(roomTemp[index]);			//display each character of the string on the LCD
		index++; //increment the index 
	}
	
	//Displaying predefined message "Humidity:" 
	controlLCD(0xC0);				//Setting the cursor on the second line, first position
	
	//Displaying predefined message "Temperature:"
	index = 0;
	while(roomHumidity[index] != NULL){		//going through the string till the NULL terminator
		dataLCD(roomHumidity[index]);			//display each character of the string on the LCD
		index++;//increment the index 
	}
}
