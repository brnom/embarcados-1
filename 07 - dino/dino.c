#include <reg51.h>
#include <stdio.h>
#include <string.h>

// Ports
sbit rs = P1^3;
sbit e = P1^2;
sbit botao = P1^1;

// Global Variables
char start_txt[7] = "Start!";
char lost_text[5] = "Lost!";
char record_txt[13] = "Uhul Record!";
char score_txt[8] = "Score: ";
char score_str[4];
int cacti_pos_1 = 0Xc4;
int cacti_pos_2 = 0xcc;
int trex_pos = 0xc0;
int should_continue = 1;
int score = 0;
int _delay;

void delay(unsigned int t) {
	unsigned int i,j;

	for(i=0;i<t;i++)
	   for(j=0;j<127;j++);
}

void cmd(unsigned char a) {
	unsigned char x;
	x=a&0xf0;

	P1=x;
	rs=0;
	//rw=0;
	e=1;
	delay(1);
	e=0;

	x=(a<<4)&0xf0;

	P1=x;
	rs=0;
	//rw=0;
	e=1;
	delay(1);
	e=0;
}

void dat(unsigned char a) {
	unsigned char x;
	x=a&0xf0;

	P1=x;
	rs=1;
	//rw=0;
	e=1;
	delay(1);
	e=0;

	x=(a<<4)&0xf0;

	P1=x;
	rs=1;
	//rw=0;
	e=1;
	delay(1);
	e=0;
}

void escreve(char* m, int position, int time) {
	unsigned int j;
	cmd(0x01);
	cmd(position);

	for(j = 0;m[j]!='\0';j++) {
		dat(m[j]);
	}

	delay(time);
}

void handle_button_press(void) {
	if(botao == 0) {
			trex_pos = 0xc0;
	} else {
			trex_pos = 0x80;
	}
}

void store() {
  // dino*
  cmd(64);
  dat(130);
  dat(10);
  dat(63);
  dat(58);
  dat(63);
  dat(60);
  dat(59);
  dat(59);

  // cacti*
  cmd(72);
  dat(150);
  dat(112);
  dat(63);
  dat(63);
  dat(63);
  dat(42);
  dat(42);
  dat(63);
}

void main(void) {
	cmd(0x28);
	cmd(0x01);
	cmd(0x0c);
	cmd(0x80);
	cmd(0x06);
	store();

	escreve(start_txt, 0x82, 1000);

	while(should_continue == 1) {
		handle_button_press();

		cmd(0x01);
		cmd(trex_pos);
		dat(0);
		cmd(cacti_pos_1);
		dat(1);
		cmd(cacti_pos_2);
		dat(1);

		cacti_pos_1--;
		cacti_pos_2--;

		if(cacti_pos_1 == trex_pos || cacti_pos_2 == trex_pos) {
			should_continue = 0;
		} else {
			if(cacti_pos_1 == 0xc0) {
				cacti_pos_1 = 0Xcf;
			}
			if(cacti_pos_2 == 0xc0) {
				cacti_pos_2 = 0Xcf;
			}

			if(score > 950) {
			  // MUDAR ESTRUTURA
				delay(50);
			} else {
			  // MUDAR ESTRUTURA
				_delay = 1000 - score;
				delay(_delay);
			}

      // MUDAR ESTRUTURA - usar switch
			if(score < 100) {
				score = score + 25;
			} else if((score >= 100) && (score < 400)) {
				score = score + 15;
			} else if(score >= 400 && score <800) {
				score = score + 10;
			} else if(score>=800) {
				score = score + 5;
			}

			if(score == 2500) {
				should_continue = 0;
			}
		}
	}

	sprintf(score_str,"%d",score);

	strcat(score_txt, score_str);

	if(score == 2500) {
		escreve(record_txt,0x80,5000);
  } else {
		escreve(lost_text,0x84,2500);
		escreve(score_txt,0x82,2500);
	}
}
