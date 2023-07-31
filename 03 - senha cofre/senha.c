#include <reg51.h>

sfr LCD_Port = 0x90;
sbit rs = P1 ^ 3;
sbit rw = P1 ^ 0;
sbit en = P1 ^ 2;
sbit R1 = P0 ^ 3;
sbit R2 = P0 ^ 2;
sbit R3 = P0 ^ 1;
sbit R4 = P0 ^ 0;
sbit C1 = P0 ^ 6;
sbit C2 = P0 ^ 5;
sbit C3 = P0 ^ 4;

void delay(unsigned int count) {
  int i, j;
  for (i = 0; i < count; i++);
    for (j = 0; j < 112; j++);
}

void LCD_Command(char cmnd) {
  LCD_Port = (LCD_Port & 0x0F) | (cmnd & 0xF0);
  rs = 0;
  rw = 0;
  en = 1;
  delay(1);
  en = 0;
  delay(2);

  LCD_Port = ((LCD_Port & 0x0F) | (cmnd << 4));
  en = 1;
  delay(1);
  en = 0;
  delay(5);
}

void LCD_Char(char char_data) {
  LCD_Port = (LCD_Port & 0x0F) | (char_data & 0XF0);
  rs = 1;
  rw = 0;
  en = 1;
  delay(1);
  en = 0;
  delay(2);

  LCD_Port = (LCD_Port & 0x0F) | (char_data << 4);
  en = 1;
  delay(1);
  en = 0;
  delay(5);
}

void LCD_String(char *str) {
  int i;

  for (i = 0; str[i] != 0; i++) {
    LCD_Char(str[i]);
  }
}

void LCD_String_xy(char row, char pos, char *str) {
  if (row == 0)
    LCD_Command((pos & 0x0F) | 0x80);
  else if (row == 1)
    LCD_Command((pos & 0x0F) | 0xC0);

  LCD_String(str);
}

void LCD_Init(void) {
  delay(20);
  LCD_Command(0x02);
  LCD_Command(0x28);
  LCD_Command(0x0C);
  LCD_Command(0x06);
  LCD_Command(0x01);
  LCD_Command(0x80);
}

unsigned int password = 0114166; // Password in octal (9876 on keypad)

char get_input() {
  int i;
  char keypad[4][3] = {{'1', '2', '3'},
                       {'4', '5', '6'},
                       {'7', '8', '9'},
                       {'*', '0', '#'}};

  while (1) {
    for (i = 0; i < 4; i++) {
      switch (i){
        case 0:
          R1 = 0;
          R2 = R3 = R4 = 1;
          break;
        case 1:
          R2 = 0;
          R1 = R3 = R4 = 1;
          break;
        case 2:
          R3 = 0;
          R1 = R2 = R4 = 1;
          break;
        case 3:
          R4 = 0;
          R1 = R2 = R3 = 1;
          break;
      }

      if (C1 == 0) {
        while (C1 == 0);
          return keypad[i][0];
      }
      if (C2 == 0) {
        while (C2 == 0);
          return keypad[i][1];
      }
      if (C3 == 0) {
        while (C3 == 0);
          return keypad[i][2];
      }
    }
  }
}

void main() {
  int i = 0;
  char input = 0;
  unsigned int typed_password = 0;

  LCD_Init();
  LCD_String("Type digits:");

  // Espera o caracter # para começar a digitar a senha
  while (input != '*') {
    input = get_input();
  }

  for (i = 0; i < 4; i++) {
    input = get_input();

    // Se o caracter for #, sai do loop
    if (input == '#') {
      break;
    }

    // Desloca os bits para a esquerda e faz um OR com o novo valor
    typed_password <<= 4;
    // Subtrai '0' para converter ASCII para inteiro, depois faz um OR com typed_password
    typed_password |= (input - '0');
    // Mostra um asterisco no LCD no lugar da tecla digitada
    LCD_Char('*');
  }

  LCD_Command(0xC0);

  if (typed_password == password) {
    LCD_String_xy(1, 0, "Access Granted");
  } else {
    LCD_String_xy(1, 0, "Access Denied");
  }

  while (1);
}
