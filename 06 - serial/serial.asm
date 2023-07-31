org 0000h
	RS	equ P1.3
	E	equ P1.2

main:
	clr RS

         
	mov TMOD,#20H
	mov TH1,#0FCH  
	mov SCON,#50H 
	setb TR1  
	Mov DPTR,#LUT
	
	call funcSet
	call dispCon
	call entryMode
Back:		 
	clr A
    movc A,@A+DPTR
	call Transmit
	inc DPTR
	jnz Back

GetData:	 
	call GetInput
	mov P1,A
    
Transmit: 
	mov SBUF,A
Here:   	 
	jnb TI,Here  
	clr TI		
	Ret
          
GetInput: 
	jnb RI,GetInput
    mov A,SBUF
	call SendChar
    clr RI
    ret
;-----------lcd-------------
  FuncSet:
        Clr  P1.7
        Clr  P1.6
        SetB P1.5
        Clr  P1.4
        Call Pulse
        Call Delay
        Call Pulse

        SetB P1.7
        Clr  P1.6
        Clr  P1.5
        Clr  P1.4
        Call Pulse
        Call Delay
        Ret

DispCon:
        Clr P1.7
        Clr P1.6
        Clr P1.5
        Clr P1.4
        Call Pulse

        SetB P1.7
        SetB P1.6
        SetB P1.5
        SetB P1.4
        Call Pulse
        Call Delay
        Ret

Home:
        Clr RS
        SetB P1.7
        SetB P1.6
        Clr P1.5
        Clr P1.4
        Call Pulse

        Clr P1.7
        Clr P1.6
        Clr P1.5
        Clr P1.4
        Call Pulse
        Call Delay
        Ret


EntryMode:
        Clr P1.7
        Clr P1.6
        Clr P1.5
        Clr P1.4
        Call Pulse

        Clr  P1.7
        SetB P1.6
        SetB P1.5
        Clr  P1.4

        Call Pulse
        Call Delay
        Ret

Pulse:
        SetB E
        Clr  E
        Ret


SendChar:
        Mov C, ACC.7
        Mov P1.7, C
        Mov C, ACC.6
        Mov P1.6, C
        Mov C, ACC.5
        Mov P1.5, C
        Mov C, ACC.4
        Mov P1.4, C

        Call Pulse

        Mov C, ACC.3
        Mov P1.7, C
        Mov C, ACC.2
        Mov P1.6, C
        Mov C, ACC.1
        Mov P1.5, C
        Mov C, ACC.0
        Mov P1.4, C

        Call Pulse
        Call Delay

        Mov R1, #55h
        Ret

Delay:
        Mov R0, #50
        Djnz R0, $
        Ret
;------------------------


Stop:
        Jmp $
          Org 0200h
  
LUT:   	 DB 'D','i','g','i','t','e', 0
  end