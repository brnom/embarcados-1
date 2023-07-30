#include <reg51.h>

sfr LCD_Port=0x90;
sbit rs=P1^0;
sbit rw=P1^1;
sbit en=P1^2;

void delay(unsigned int count) {
	int i,j;
	for(i=0;i<count;i++);
	for(j=0;j<112;j++);
}

void LCD_Command (char cmnd) {
	LCD_Port =(LCD_Port & 0x0F) | (cmnd & 0xF0);
	rs=0;
	rw=0;
	en=1;
	delay(1);
	en=0;
	delay(2);
	
	LCD_Port = ((LCD_Port & 0x0F) | (cmnd << 4));
	en=1;
	delay(1);
	en=0;
	delay(5);
}

void LCD_Char (char char_data) {
	LCD_Port = (LCD_Port & 0x0F) | (char_data & 0XF0);
	rs=1;
	rw=0;
	en=1;
	delay(1);
	en=0;
	delay(2);
	
	LCD_Port = (LCD_Port & 0x0F) | (char_data << 4);
	en=1;
	delay(1);
	en=0;
	delay(5);
}

void LCD_String (char *str) {
	int i;
	for(i=0;str[i]!=0;i++) {
		LCD_Char (str[i]);
	}
}

void LCD_String_xy(char row, char pos, char *str) {
	if (row == 0)
	LCD_Command((pos & 0x0F)|0x80);
	else if (row == 1)
	LCD_Command((pos & 0x0F)|0xC0);
	LCD_String(str);
}

void LCD_Init (void) {
	delay(20);
	LCD_Command (0x02);
	LCD_Command (0x28);
	LCD_Command (0x0C);
	LCD_Command (0x06);
	LCD_Command (0x01);
	LCD_Command (0x80);
}


void main() {
	int shift, i;

	LCD_Init();
	LCD_String("mensagem teste 1");
	LCD_Command(0xC0); // vai p segunda linha
	LCD_String_xy(1,0,"msg teste 2");

	shift = 5;

	while(1){
		for (i=0; i<shift; i++) {
			LCD_Command(0x18);
			// LCD_Command(0x1C);
			delay(3);
		}
	}
}