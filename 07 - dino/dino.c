#include <reg51.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Ports
sbit rs = P1^3;
sbit e = P1^2;
sbit botao = P1^1;

// Global
char txt_inicio[7] = "START ";
char txt_final[15] = "TENTE NOVAMENTE";
char txt_recorde[17] = "NOVO RECORDE ";
char txt_score[12] = "SCORE: ";
char score_str[4];
int pos_policia_1 = 0Xc4;
int pos_policia_2 = 0xcc;
int pos_nuvem = 0x88;
int pos_punk = 0xc0;
int valid = 1;
int score = 0;
int atraso;

// Functions
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
	e=1;
	delay(1);
	e=0;
		
	x=(a<<4)&0xf0;
		
	P1=x;
	rs=0;
	e=1;
	delay(1);
	e=0;
	
}

void dat(unsigned char a) {
	unsigned char x;
	x=a&0xf0;

	P1=x;
	rs=1;
	e=1;
	delay(1);
	e=0;
		
	x=(a<<4)&0xf0;
		
	P1=x;
	rs=1;
	e=1;
	delay(1);
	e=0;
}

void escreve(char* m, int pos, int time) {
	unsigned int j;
	cmd(0x01);
	cmd(pos);
	
	for(j = 0;m[j]!='\0';j++){
		dat(m[j]);
	}
	
	delay(time);
}

void buttonPush(void) {
	if(botao == 0){
			pos_punk = 0xc0;
	}
	else{
			pos_punk = 0x80;
	}
}

void store() {

	  // PUNK
    cmd(64);          
    dat(130);
    dat(10);
    dat(63);
    dat(58);
    dat(63);
    dat(60);
    dat(59);
    dat(59);
		
		// POLÍCIA
    cmd(72);
    dat(150);
    dat(112);
    dat(63);
    dat(63);
    dat(63);
    dat(42);
    dat(42);
    dat(63);
		
		// NUVEM
    cmd(80);
    dat(0);
    dat(4);
    dat(10);
    dat(18);
    dat(17);
    dat(17);
    dat(63);
    dat(0);
}

void main(void) {
	
	cmd(0x28);
	cmd(0x01);
	cmd(0x0c);
	cmd(0x80);
	cmd(0x06);
	store();
	
	escreve(txt_inicio,0x82,1000);
	
	while(valid == 1){
		buttonPush();
		
		cmd(0x01);
		cmd(pos_punk);
		dat(0);
		cmd(pos_policia_1);
		dat(1);
		cmd(pos_policia_2);
		dat(1);
		cmd(pos_nuvem);
		dat(2);
		
				
		pos_policia_1--;
		pos_policia_2--;
		pos_nuvem--;
		
		if(pos_policia_1 == pos_punk || pos_policia_2 == pos_punk){
			valid = 0;
		}
		
		else{
			
			if(pos_punk == 0x80) {
			score = score - 15;
			}
			
			if(pos_policia_1 == 0xc0) {
				delay(1);
				pos_policia_1 = 0xcf - rand() % 6;
			}
			
			if(pos_policia_2 == 0xc0) {
				delay(1);
				pos_policia_2 = 0xcf - rand() % 6;
			}
			
			if(pos_nuvem == 0x80) {
				delay(1);
				pos_nuvem = 0x8f - rand() % 6;
			}		
			
			if(score > 200 && score < 500) {
				delay(150);
				score = score + 15;
			}
			
			if(score <= 200) {
				score = score + 15;
				delay(300);
			}

			if(score >= 500 && score < 950) {
				delay(100);
				score = score + 15;
			}
			
			if(score >= 950){
				valid = 0;
			}
		}
	}
	
	
	sprintf(score_str,"%d",score);
	
	strcat(txt_score, score_str);
	
	if(score >= 950){
		escreve(txt_recorde,0x80,5000);
		escreve(txt_score,0x82,2500);
	}
	else{
		escreve(txt_final,0x81,2500);
		escreve(txt_score,0x82,2500);
	}
}