;-----------------------------------------------
;   Title: Heating and Cooling System Controller
;-----------------------------------------------
;Purpose: This program controls a heating/cooling system
;Dependencies: NONE
;Compiler: MPLAB X IDE v6.20
;Author: Mathew Teague
;OUTPUTS:
;   PORTD.1 - Heating system
;   PORTD.2 - Cooling system
;   refD1   - registers refDx store refTemp in decimal
;   refD2
;   refD3
;   measuredD1	- registers measuredDx store measuredTemp in decimal
;   measuredD2
;   measuredD3
;INPUTS:
;   keypad -> refTemp
;   temperature sensor -> measuredTemp
;Versions:
;   V1.0: 2/29/2024 - First Version
;   V2.0: 3/4/2024  - I had to overhaul the code to match the updated 
;		      assignemnt requirements
;   V2.1: 3/5/2024  - Added a code block that accounts for negative inputs
;   V2.2: 3/6/2024  - Changed some of the register names so the code is
;		      is easier to understand
;		    - Added code for converting the inputs into decimal
;		    - Changed how PORTD is set
;		    - Finished comments
;---------------------
; Initialization
;---------------------
#include "AssemblyConfig.inc"
#include <xc.inc>

;---------------------
; Program Inputs
;---------------------
#define refTemp		15	;from keypad (in deicmal)
#define measuredTemp	5	;from sensor (in decimal)

;---------------------
; Registers
;---------------------
#define refTempReg	  0x20	;refTemp's register
#define measuredTempReg   0x21	;measuredTemp's register
#define contReg		  0x22	;contReg
    
;For hex to dec conversion:
NUME	EQU		  0x25	;numerator (register)
QU	EQU		  0x30	;quotient (register)
MYDEN	EQU		  0xA ;denominator = 0xA

;---------------------
; Program Outputs
;---------------------
;#define heater		PORTD,1
;#define cooler		PORTD,2
;heater	EQU		PORTD,1
;cooler  EQU		PORTD,2
#define refD1		0x62
#define refD2		0x61
#define refD3		0x60
#define measuredD1	0x72
#define measuredD2	0x71
#define measuredD3	0x70

;---------------------
; Main Program
;---------------------
    PSECT absdata,abs,ovrld
    org 0x20
   
    ;Put inputs into registers
    MOVLW   refTemp
    MOVWF   refTempReg	    ;REG20 = refTemp
    MOVLW   measuredTemp    
    MOVWF   measuredTempReg	    ;REG21 = measuredTemp
    
    ;Convert refTemp to deicmal and store in regs
    ;I repurposed an algorithim from the Mazidi textbook (pg 164)
    MOVLW   refTemp	    ;converting refTemp first
    MOVWF   NUME	    ;load numerator
    MOVLW   MYDEN
    CLRF    QU,1	    ;clear quotient reg
refD_1:
    INCF    QU,1	    ;increment QU for every subtraction
    SUBWF   NUME	    ;NUME - WREG, store in wreg
    BC	    refD_1		    ;if NUME > 0, repeat
    ADDWF   NUME	    ;put number back in NUME
    DECF    QU,1	    ;decrement QU since we reversed the subtraction
    MOVFF   NUME,refD1	    ;save the low digit
    MOVFF   QU,NUME	    ;repeat one more time
    CLRF    QU		    ;clear QU
refD_2:
    INCF    QU,1
    SUBWF   NUME	    ;sub WREG from NUME value
    BC	    refD_2	    ;(C = 1 for positive)
    ADDWF   NUME	    ;once too many
    DECF    QU,1
    MOVFF   NUME,refD2	    ;save middle digit
    MOVFF   QU,refD3	    ;save high digit, always 0 for our case
    
    
    
    ;Convert refTemp to deicmal and store in regs
    ;I repurposed an algorithim from the Mazidi textbook (pg 164)
    MOVLW   measuredTemp    ;converting refTemp first
    MOVWF   NUME	    ;load numerator
    MOVLW   MYDEN
    CLRF    QU,1	    ;clear quotient reg
measuredD_1:
    INCF    QU,1	    ;increment QU for every subtraction
    SUBWF   NUME	    ;NUME - WREG, store in wreg
    BC	    measuredD_1	    ;if NUME > 0, repeat
    ADDWF   NUME	    ;put number back in NUME
    DECF    QU,1	    ;decrement QU since we reversed the subtraction
    MOVFF   NUME,measuredD1	    ;save the low digit
    MOVFF   QU,NUME	    ;repeat one more time
    CLRF    QU		    ;clear QU
measuredD_2:
    INCF    QU,1
    SUBWF   NUME	    ;sub WREG from NUME value, store in WREG
    BC	    measuredD_2	    ;(C = 1 for positive)
    ADDWF   NUME	    ;once too many
    DECF    QU,1
    MOVFF   NUME,measuredD2 ;save middle digit
    MOVFF   QU,refD3	    ;save high digit, always 0 for our case
    
    ;configure port D
    ;CLRF   TRISD   ;set PORTC bits to all 0 (output)
    
    SUBWF   refTempReg, 0	;d bit = 0 so value in REG20 isn't overwritten
    BZ	    _EQUAL   ;branch if refTemp = measuredTemp
    
    ;check if measuredTemp is negative
    ;since the system requirements specify that refTemp cannot be negative,
    ;we can skip to turning on the heater if measuredTemp is negative, without
    ;comparing the values
    MOVLW   measuredTemp
    ADDLW   0x0
    BN	    _HEATING
    
    MOVLW   refTemp	      
    CPFSGT  measuredTempReg   ;Check measuredTemp > WREG(refTemp), skip if true
    GOTO    _HEATING	    ;measuredTemp < refTemp
    GOTO    _COOLING	    ;measuredTemp > refTemp
    
_EQUAL:
    MOVLW   0x0
    MOVWF   contReg	;contReg = 0
    GOTO    _END
    
_HEATING:	    ;measuredTemp < refTemp
    MOVLW   0x1
    MOVWF   contReg   ;contReg = 1
    BSF	    PORTD,1
    ;MOVLW   0x2
    ;MOVWF   PORTD  ;PORTD,1
    GOTO    _END
    
_COOLING:	    ;measuredTemp > refTemp
    MOVLW   0x2
    MOVWF   contReg   ;contReg = 2
    BSF	    PORTD,2
    ;MOVLW   0x4
    ;MOVWF   PORTD  ;PORTD,2
    GOTO    _END
    
_END:
    SLEEP
