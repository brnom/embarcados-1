ORG 0000h
       
      RS      EQU     P1.3
      E       EQU     P1.2
       
Main:
        Clr RS
        Call FuncSet
        Call ConfigDisplay
        Call Leitura_Teclado
 
        MOV R2, #0   ; Initialize the counter
        MOV R3, #0   ; Initialize the index
 
        Mov DPTR, #LUT1
        SETB RS
 
incr:
        Clr A
        Movc A, @A+DPTR
        Jz carrega_senha
        Call EnviaCaractere
        INC DPTR
        JMP incr
 
carrega_senha:
        Call Home
        Mov DPTR, #LUT2
        SetB RS
Next:
        Call LeituraTeclado
        SetB RS
        Call verifica_senha
        Clr A
        Mov A, #'*'
        Call EnviaCaractere
        Cjne R7, #'#', Next
		JMP msg_erro
 
verifica_senha:
        CLR A
        Movc A, @A+DPTR   ; Load a character from the password
        Jz check_counter
        CJNE A, 07h, next_iteration   ; Compare the key with the password character
        INC DPTR   ; Increment the counter
        RET
 
next_iteration:
        INC R2   ; Move to the next character in the password
        RET   ; Repeat until all characters in the password are checked
 
check_counter:
        CJNE R2, #0, msg_erro   ; Compare if it is different from 4
        JMP msg_acerto
		RET
 
msg_erro:
        Mov DPTR, #LUT4
        SetB RS
        ; Call Home
incr_w:
        Clr A
        Movc A, @A+DPTR
        Jz $
        Call EnviaCaractere
        INC DPTR
        JMP incr_w
		RET
 
msg_acerto:
        Mov DPTR, #LUT3
        SetB RS
        ; Call Home
incr_r:
        Clr A
        Movc A, @A+DPTR
        Jz $
        Call EnviaCaractere
        INC DPTR
        JMP incr_r
		RET
 
FuncSet:
        Clr  P1.7
        Clr  P1.6
        SetB P1.5
        Clr  P1.4
        Call Pulso
        Call Delay
        Call Pulso
 
        SetB P1.7
        Clr  P1.6
        Clr  P1.5
        Clr  P1.4
        Call Pulso
        Call Delay
        Ret
 
ConfigDisplay:
        Clr P1.7
        Clr P1.6
        Clr P1.5
        Clr P1.4
        Call Pulso
 
        SetB P1.7
        SetB P1.6
        SetB P1.5
        SetB P1.4
        Call Pulso
        Call Delay
        Ret
 
Home:
        Clr RS
        SetB P1.7
        SetB P1.6
        Clr P1.5
        Clr P1.4
        Call Pulso
 
        Clr P1.7
        Clr P1.6
        Clr P1.5
        Clr P1.4
        Call Pulso
        Call Delay
        Ret
 
 
Leitura_Teclado:
        Clr P1.7
        Clr P1.6
        Clr P1.5
        Clr P1.4
        Call Pulso
 
        Clr  P1.7
        SetB P1.6
        SetB P1.5
        Clr  P1.4
 
        Call Pulso
        Call Delay
        Ret
 
Pulso:
        SetB E
        Clr  E
        Ret
 
 
EnviaCaractere:
        Mov C, ACC.7
        Mov P1.7, C
        Mov C, ACC.6
        Mov P1.6, C
        Mov C, ACC.5
        Mov P1.5, C
        Mov C, ACC.4
        Mov P1.4, C
 
        Call Pulso
 
        Mov C, ACC.3
        Mov P1.7, C
        Mov C, ACC.2
        Mov P1.6, C
        Mov C, ACC.1
        Mov P1.5, C
        Mov C, ACC.0
        Mov P1.4, C
 
        Call Pulso
        Call Delay
 
        Mov R1, #55h
        Ret
 
Delay:
        Mov R0, #50
        Djnz R0, $
        Ret
 
ESPSOL:
        Mov A, P0
        Anl A, #070h
        Cjne A, #070h, ESPSOL
 
        Mov TMOD, #01H
        Mov TH0, #08ah
        Mov TL0, #0cfh
        Setb TR0
        Jnb TF0, $
        Clr TR0
        Clr TF0
 
        Ret
 
LeituraTeclado:
        CLR P0.3
 
        CALL IDCode0
 
        SetB P0.3
        JB F0, Done
 
        CLR P0.2
 
        CALL IDCode1
 
        SetB P0.2
        JB F0, Done
 
        CLR P0.1
 
        CALL IDCode2
 
        SetB P0.1
        JB F0, Done
 
        CLR P0.0
 
        CALL IDCode3
 
        SetB P0.0
        JB F0, Done
 
        JMP LeituraTeclado
 
Done:
        Clr F0
        Ret
 
IDCode0:
        JNB P0.4, KeyCode03
        JNB P0.5, KeyCode13
        JNB P0.6, KeyCode23
        Ret
 
KeyCode03:
        SETB F0
        CALL ESPSOL
        Mov R7, #'3'
        Ret
 
KeyCode13:
        SETB F0
        CALL ESPSOL
        Mov R7, #'2'
        Ret
 
KeyCode23:
        SETB F0
        CALL ESPSOL
        Mov R7, #'1'
        Ret
 
IDCode1:
        JNB P0.4, KeyCode02
        JNB P0.5, KeyCode12
        JNB P0.6, KeyCode22
        Ret
 
KeyCode02:
        SETB F0
        CALL ESPSOL
        Mov R7, #'6'
        Ret
 
KeyCode12:
        SETB F0
        CALL ESPSOL
        Mov R7, #'5'
        Ret
 
KeyCode22:
        SETB F0
        CALL ESPSOL
        Mov R7, #'4'
        Ret
 
IDCode2:
        JNB P0.4, KeyCode01
        JNB P0.5, KeyCode11
        JNB P0.6, KeyCode21
        Ret
 
KeyCode01:
        SETB F0
        CALL ESPSOL
        Mov R7, #'9'
        Ret
 
KeyCode11:
        SETB F0
        CALL ESPSOL
        Mov R7, #'8'
        Ret
 
KeyCode21:
        SETB F0
        CALL ESPSOL
        Mov R7, #'7'
        Ret
 
IDCode3:
        JNB P0.4, KeyCode00
        JNB P0.5, KeyCode10
        JNB P0.6, KeyCode20
        Ret
 
KeyCode00:
        SETB F0
        CALL ESPSOL
        Mov R7, #'#'
        Ret
 
KeyCode10:
   SETB F0
   CALL ESPSOL
   Mov R7, #'0'
   Ret
 
KeyCode20:
   SETB F0
   CALL ESPSOL
   Mov R7, #'*'
   Ret
 
Stop:
   Jmp $
 
ORG 0200h
LUT1: DB 'E', 'N', 'T', 'R', 'E', ' ', 'C', 'O', 'M', ' ', 'A', ' ', 'S', 'E', 'N', 'H', 'A',':', 0
LUT2: DB '1', '2', '3', '4', 0
LUT3: DB ' ', 'O', 'K', 0
LUT4: DB ' ', 'A', 'C', 'E', 'S', 'S', 'O', ' ', 'N', 'E', 'G', 'A', 'D', 'O', 0
 
End
