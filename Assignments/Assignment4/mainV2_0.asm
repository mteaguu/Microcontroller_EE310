;-----------------------------------------------
;   Title: Heating and Cooling System Controller
;-----------------------------------------------
;Purpose: This program controls a heating/cooling system
;Dependencies: NONE
;Compiler: MPLAB X IDE v6.20
;Author: Mathew Teague
;OUTPUTS:
;   PORTD.1 -> Heating system
;   PORTD.2 -> Cooling system
;INPUTS:
;   keypad -> refTemp
;   temperature sensor -> measuredTemp
;Versions:
;   V1.0: 2/29/2024 - First Version
;   V2.0: 3/4/2024  - I had to overhaul the code to match the updated 
;		      assignemnt requirements
;
;
;---------------------
; Initialization
;---------------------
#include "AssemblyConfig.inc"
#include <xc.inc>

;---------------------
; Program Inputs
;---------------------
#define refTemp		15	;from keypad
#define measuredTemp	5	;from sensor


 
;---------------------
; Registers
;---------------------
#define refTempReg	  0x20	;refTemp
#define measuredTempReg   0x21	;measuredTemp
#define contReg		  0x22	;contReg

;---------------------
; Program Outputs
;---------------------
;#define

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
    
    ;Convert inputs to deicmal and store in regs
    
    
    ;configure port D
    MOVLW   0x0
    MOVWF   TRISD   ;set PORTC bits to all output
    
    SUBWF   refTempReg, 0	;d bit = 0 so value in REG20 isn't overwritten
    BZ	    _EQUAL   ;branch if refTemp = measuredTemp
    
    MOVLW   refTemp	      
    CPFSGT  measuredTempReg   ;Check REG21 > WREG(REG20), skip if true
    GOTO    _HEATING
    GOTO    _COOLING
    
_EQUAL:
    MOVLW   0x1
    MOVWF   contReg
    GOTO    _END
    
_HEATING:	    ;measuredTemp < refTemp
    MOVLW   0x2
    MOVWF   contReg   ;contReg = 1
    
    BSF	    PORTD,1  
    GOTO    _END
    
_COOLING:	    ;Cooling system activation
    MOVLW   0x2
    MOVWF   contReg   ;contReg = 2
    
    BSF	    PORTD,2
    GOTO    _END
    
_END:
    END