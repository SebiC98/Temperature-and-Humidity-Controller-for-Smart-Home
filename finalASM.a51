org 0000h
	
mov P0, #0FFh ; input
mov P1, #0FFh ; input
mov R1, #15h; 21 degree base temp set
mov R2, #02Dh; 45 % humidity base set
clr P3.1

LCD: 
	mov a, #38h ;5x7 dots format
	acall commandLCD  ;call init function
	mov a, #01h ;display ON, cursor ON, not blinking
	acall commandLCD
	mov a, #0Ch ;clear display
	acall commandLCD
	mov a, #80h ;cursor at the begining of line 1
	acall commandLCD
	
;display the messages using predefined strings
	mov dptr, #100h ;mov dptr at the address of the first string
textTemperature:	
	mov a, #00h
	movc a, @a+dptr 
	acall dispOnLCD ;call the display function
	inc dptr   ;go to addres of the next character
	cjne a, #00h, textTemperature
	
	acall delay
	
	mov a, #0C0h ;cursor on line 2, at the begining
	acall commandLCD
	mov dptr, #200h ;mov dptr at the addres of the second string
textHumidity: 
	mov a, #00h
	movc a, @a+dptr
	acall dispOnLCD
	inc dptr
	cjne a, #00h, textHumidity


readValues:

readTemperature:
clr P1.0 ; take IN0
acall delay;

;cursor on line 1, column 12 (after the message "Temperature:" )
	mov a, #8Ch
	acall commandLCD
	acall delay
;the value from P1 will be put in the accumulator
	mov a, P0
	cjne a,01h,checkCT
	sjmp esteBineAici
	checkCT:
	jc smaller2
	sjmp esteBineAici
	smaller2:
	setb P1.7
	esteBineAici:
acall convertTemperature
	;the value from P1 will be put in the accumulator
	
;call the function that converts and displays temperature

	
readHumidity:
setb P1.0; ;take IN1
;curson on line 2, pozition 13 (After the message "Humidity:   ")
	mov a, #0CCh
	acall commandLCD
	acall delay
;the value from P0 will be put in the accumulator
	mov a, P0
	
	cjne a,02h,checkHumid //Aici daca ma uit in debug mi se suprascrie un reigstru si imi da eroarea
	//sjmp esteBineAici2
	checkHumid:
	//jc smaller3
	///sjmp esteBineAici2
	//smaller3:
	//setb P1.6
	//esteBineAici2:
	
	acall convertHumidity
;start reading again in an infinite loop
	jmp readValues
	
	/*
    jnb P3.2, readValues
	tempMenu:
	mov a, #38h ;5x7 dots format
	acall commandLCD  ;call init function
	mov a, #01h ;display ON, cursor ON, not blinking
	acall commandLCD
	mov a, #0Ch ;clear display
	acall commandLCD
	mov a, #80h ;cursor at the begining of line 1
	acall commandLCD
	/*mov dptr, #100h ;mov dptr at the address of the first string
	textTemperature2:	
	mov a, #00h
	movc a, @a+dptr 
	acall dispOnLCD ;call the display function
	inc dptr   ;go to addres of the next character
	cjne a, #00h, textTemperature2
	acall delay
	
	mov a, #0C0h ;cursor on line 2, at the begining
	acall commandLCD
	mov dptr, #200h ;mov dptr at the addres of the second string
textTemp2: 
	mov a, #00h
	movc a, @a+dptr
	acall dispOnLCD
	inc dptr
	cjne a, #00h, textTemp2

	here:sjmp here 

	*/
	
	
	
	
	
	
convertTemperature:	
mov b,#05h
;We need to divide by 5 because at the output of the ADC we will have 
;256 possible values. Our range of temperature in from 0 to 50 degrees, 
;meaning 51 values
	div ab
	;do the adjustments
	acall adjustTemperatureValue
	CJNE A, 1, NOT_EQUAL ;CHECK A WITH R1 (1 is the adress of bank0 register 1)
	
	NOT_EQUAL:
	 jc smallerTemp ;leave Realy off
	 setb P1.7
	 sjmp goo
	 smallerTemp:    ;turn ON Relay
	 clr P1.7
goo:
	;convert the first digit of the value into ASCII and display it
	mov b, #0Ah
	div ab
	add a, #30h
	acall dispOnLCD
	acall delay
	
;convert the second digit of the value into ASCII and display it
	mov a, b
	add a, #30h
	acall dispOnLCD
	mov a, #0DFh
	acall dispOnLCD
	mov a, #043h
	acall dispOnLCD
	acall delay
	
ret

;function to ajust the Temperature value
adjustTemperatureValue:
cjne a,#25h,checkCarry
checkCarry:
	jc smaller
	subb a,#01h
	smaller:
ret


convertHumidity:	
mov b,#04h
;We need to divide by 4 because at the output of the ADC we will have 
;256 possible values. Our range of relative humidity in from 0 to 65 %RH, 
;meaning 66 values; 256/66 is approx 4
	div ab
	;do the adjustments
	acall adjustHumidityValue
	CJNE A, 2, NOT_EQUAL2 ;CHECK A WITH R2 (1 is the adress of bank0 register 2)
	
	NOT_EQUAL2:
	 jc smallerHum ;leave Realy off
	 setb P1.6
	 sjmp gooo
	 smallerHum:    ;turn ON Relay
	 clr P1.6
gooo:
	;convert the first digit of the value into ASCII and display it
	mov b, #0Ah
	div ab
	add a, #30h
	acall dispOnLCD
	acall delay
	
;convert the second digit of the value into ASCII and display it
	mov a, b
	add a, #30h
	acall dispOnLCD
	mov a, #00h
	acall dispOnLCD
	mov a, #025h
	acall dispOnLCD
	acall delay
	
ret

;function to ajust the Temperature value
adjustHumidityValue:
cjne a,#2Fh,checkCarryHum
checkCarryHum:
	jc smaller47
	subb a,#03h
	sjmp adjusmentEnd
	smaller47:
	cjne a,#1Fh,checkCarryHum2
checkCarryHum2:
	jc smaller31
	subb a,#02h
	sjmp adjusmentEnd
	smaller31:
	cjne a,#10h,checkCarryHum3
checkCarryHum3:
	jc smaller16
	subb a,#02h
	sjmp adjusmentEnd
	smaller16:
	adjusmentEnd:
ret

;function to initialize the LCD
commandLCD:
	mov P2, a
	clr P3.4  ;RS=O
	setb P3.0 ;EN=1
	acall delay
	clr P3.0  ;EN=O
	ret
	
;function to display on the LCD
dispOnLCD:
	mov P2, a
	setb P3.4 ;RS=1
	setb P3.0 ;EN=1
	acall delay
	clr P3.0  ;EN=0
	ret
	
delay: 
	mov r4, #0Ah
delay1:
	mov r0, #98h 
	delay2:
		nop
		nop
		nop
		nop
		djnz r0, delay2 
		djnz r4, delay1
		ret
	
;the predefined strings at adresses 100h and 200h in code memory
org 100h	
temperature:
	db "Temperature:"

org 200h
humidity: 
	db "Humidity:"
		
end