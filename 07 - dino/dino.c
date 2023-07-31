#include <reg51.h>
#include <stdio.h>
#include <string.h>

//Portas
sbit rs = P1^3;
sbit e = P1^2;
sbit botao = P1^1;

//Variáveis globais
char texto_inicio[7] = "INICIO";
char texto_perdeu[7] = "PERDEU";
char texto_recorde[17] = "RECORDE!!!! :)) ";
char texto_pontuacao[12] = "PONTUACAO: ";
char score_str[4];
int posicao_cacto_1 = 0Xc4;
int posicao_cacto_2 = 0xcc;
int posicao_dino = 0xc0;
int continuar = 1;
int score = 0;
int atraso;

//JOGAR FUNCOES PRA CIMA -- 
void delay(unsigned int);
void cmd(unsigned char);
void dat(unsigned char);
void buttonP(void);
void escreve(char*,int,int);
void store();

void main(void){
	
	cmd(0x28);
	cmd(0x01);
	cmd(0x0c);
	cmd(0x80);
	cmd(0x06);
	store();
	
	escreve(texto_inicio,0x82,1000);
	
	while(continuar == 1){
		buttonP();
		
		
		cmd(0x01);
		cmd(posicao_dino);
		dat(0);
		cmd(posicao_cacto_1);
		dat(1);
		cmd(posicao_cacto_2);
		dat(1);
		
		
		posicao_cacto_1--;
		posicao_cacto_2--;
		
		if(posicao_cacto_1 == posicao_dino || posicao_cacto_2 == posicao_dino){
			continuar = 0;
		}
		
		else{
			if(posicao_cacto_1 == 0xc0){
				posicao_cacto_1 = 0Xcf;
			}
			if(posicao_cacto_2 == 0xc0){
				posicao_cacto_2 = 0Xcf;
			}
			
			// MUDAR ESTRUTURA
			if(score > 950){
				delay(50);
			}
			
			// MUDAR ESTRUTURA
			else{
				atraso = 1000 - score;
				delay(atraso);
			}

			
			if(score < 100){
				score = score + 25;
			}
			
			else if((score >= 100) && (score < 400)){
				score = score + 15;
			}
			
			else if(score >= 400 && score <800){
				score = score + 10;
			}
			
			else if(score>=800){
				score = score + 5;
			}
			
			if(score == 2500){
				continuar = 0;
			}
		}
	}
	
	
	sprintf(score_str,"%d",score);
	
	strcat(texto_pontuacao, score_str);
	
	if(score == 2500){
		escreve(texto_recorde,0x80,5000);
	}
	else{
		escreve(texto_perdeu,0x84,2500);
		escreve(texto_pontuacao,0x82,2500);
	}
}
void delay(unsigned int t)
{
	unsigned int i,j;
	
	for(i=0;i<t;i++)
	   for(j=0;j<127;j++);
}

void cmd(unsigned char a)
{
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

void dat(unsigned char a)
{
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

void escreve(char* m, int position, int delai){
	unsigned int j;
	cmd(0x01);
	cmd(position);
	
	for(j = 0;m[j]!='\0';j++){
		dat(m[j]);
	}
	
	delay(delai);
}
void buttonP(void){
	if(botao == 0){
			posicao_dino = 0xc0;
	}
	else{
			posicao_dino = 0x80;
	}
}

void store()
{
	  // VAQUINHA -- MUDAR ESTRUTRURA
    cmd(64);          
    dat(1);
    dat(1);
    dat(1);
    dat(1);
    dat(1);
    dat(1);
    dat(1);
    dat(1);
	
		// CERCA -- MUDAR ESTRUTRURA
    cmd(72);
    dat(63);
    dat(63);
    dat(63);
    dat(63);
    dat(63);
    dat(63);
    dat(63);
    dat(63);
}