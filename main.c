/*
Reloj contador
1 al pulsar 2 seg entra al menu config
2 se pasa de digito al pulsar 2 seg
3 si pulsa menos cada x milisegundos suma una unidad
4 se confirma pulsando 5 seg
micro: pic 18f4620
*/


#include <stdio.h>
#include <stdlib.h>

/* Bits de configuracion del micro */
#pragma config OSC=XT
#pragma config WDT=OFF, LVP=OFF, MCLRE=ON

/* Includes */
#include "p18f4620.h"
#include "delays.h"
//Descomente el ejemplo a ejecutar

#define EJEMPLO3


/* EJEMPLO 3 */
#ifdef EJEMPLO3

// Rotacion de bits en el puerto B
// Configuracion de hardware:
	// Jumper LEDS = cortocircuito
/*Definiciones*/
#define Switch PORTAbits.RA2

/*Variables*/
#pragma udata
unsigned char UnidadSegundos;
unsigned char DecenaSegundos;
unsigned char UnidadMinutos;
unsigned char DecenaMinutos;
unsigned char UnidadHora;
unsigned char DecenaHora;
unsigned char UnidadDia;
unsigned char DecenaDia;
unsigned char UnidadMes;
unsigned char DecenaMes;
unsigned char UnidadAnio;
unsigned char DecenaAnio;

unsigned char A;
unsigned char B;
unsigned char C;
unsigned char ModoConfig;
unsigned char SwitchAux;

unsigned char PreEstado;

/*Prototipos*/
void Timer0_Init(void);
void Interrupcion(void);
void delayUnSegundo(void);
void sumarUnidadMinutos(void);
void sumarDecenaMinutos(void);
void sumarUnidadSegundos(void);
void sumarDecenaSegundos(void);
void sumarHora(void);
void sumarFecha(void);
void sumarMes(void);
void sumarAnio(void);
int dameDia(void);
int dameMes(void);
int dameMes(void);



//Vector de Interrupcion
#pragma code Interrupciones = 0x08
void Interrupciones(void)
{
_asm
	goto Interrupcion
_endasm
}


/*Declaraciones*/
#pragma romdata Contador_Tabla = 0x180
const rom unsigned char Display_Tabla[10]={0b00111111,0b00000110,0b01011011,0b01001111,0b01100110,0b01101101,0b01111101,0b00000111,0b01111111,0b01100111};
const rom unsigned char Dias_Mes[12]={31,28,31,30,31,30,31,31,30,31,30,31};

#pragma code

void main(void)
{
        //Falta hacer la logica del main, seteo del dispositivo inicial, configuracion manual
	UnidadSegundos=0;
	DecenaSegundos=0;
	PreEstado=0;
	TRISB=0b00000000; //configuro el puerto b como salida
	ADCON1=0x0F; //configuro los pines como digitales
	TRISA=0b00000100; //RA1 Y RA2 como entrada
	LATA=0;
	ModoConfig = 0;
	SwitchAux = 0;

	//Activamos Displays
	LATAbits.LATA0=1;
	LATAbits.LATA1=0; //
	LATAbits.LATA3=1;
	LATB = Display_Tabla[0];
	

	// LATAbits.LATA2=1;
	// LATC = Display_Tabla[0];


	Timer0_Init();
	INTCONbits.PEIE=1;
	INTCONbits.GIE=1;


	while(1){

		SwitchAux = 0;

		while(Switch!=1) {
			delayUnSegundo();
			sumarUnidadSegundos();
		}
		Switch = 0;
		

		A = 0;
		for(A;A <=20;A++) {
			//sumarDecenaSegundos(); //para probar si anda el boton
			Delay1KTCYx(100);
			if(Switch!=1){
				Switch = 0;
				break;
			}
		}


		if(A == 21){ //pasaron 2 segundos

			ModoConfig = 1;

			// entra a modo config
			while(ModoConfig == 1){

				while(Switch!=1); //ciclo infinito hasta que presione

				while(Switch==1) {

					B = 0;
					for(B;B <=200;B++) {
						Delay1KTCYx(20);
						if(Switch!=1){ //si suelta el boton antes de los 4 segundos
							break;
						}
					}

					if(B == 201){ //pasaron 4 segundos
						//salgo modo config
						ModoConfig = 0;
					}
					
					delayUnSegundo();
					sumarDecenaSegundos(); //esto debe variar depende la posicion

				}
				Delay1KTCYx(200); //no tocar
			}

		} 

	};

}

void delayUnSegundo(){
	int i;
	for(i=0; i<5; i++) {
		Delay1KTCYx(200); //no te deja poner mar de 200, x eso hay tres
	}
}

void sumarUnidadSegundos(){
	if ( UnidadSegundos == 9 ){
		UnidadSegundos=0;
		sumarDecenaSegundos();
	}else{
		UnidadSegundos++;
	}
}

void sumarDecenaSegundos(){
	if  (DecenaSegundos == 5 ){
            DecenaSegundos = 0;
            sumarUnidadMinutos();
        }else{
            DecenaSegundos++;
        }
}

void sumarDecenaMinutos(){
	if  (DecenaMinutos == 5 ){
            DecenaMinutos = 0;
            sumarHora();
        }else{
            DecenaMinutos++;
        }
}

void sumarUnidadMinutos(){
	if ( UnidadMinutos == 9 ){
		UnidadMinutos=0;
		sumarDecenaMinutos();
	}else{
		UnidadMinutos++;
	}
}

void sumarHora(){
        //Si son mas de las 23
	if  (DecenaHora == 2 ){

            if (UnidadHora == 3){
                UnidadHora = 0;
                DecenaHora = 0;
                //Cambiar dia
                sumarFecha();
            } else{
                UnidadHora++;
            }
        }else{
            if(UnidadHora == 9){
                UnidadHora = 0;
                DecenaHora++;
            }else{
                UnidadHora++;
            }
        }
}
void sumarFecha(){
    int diasMesActual = Dias_Mes[dameMes()];
    if( dameDia() == diasMesActual){
        UnidadDia= 1;
        DecenaDia= 0;
        sumarMes();
    }else{
        UnidadDia++;
    }

}

void sumarMes(){
    if( dameMes() == 12){
        UnidadMes==1;
        DecenaMes==0;
        sumarAnio();
    }else{
        UnidadMes++;
    }
}

void sumarAnio(){
    if( UnidadAnio == 9){
        UnidadAnio=0;
        DecenaAnio++;
    }else{
        UnidadAnio++;
    }
}

int dameDia(){
    int dia = DecenaDia*10 + UnidadDia;
    return  dia;
}

int dameMes(){
    int mes = DecenaMes*10 + UnidadMes;
    return  mes;
}

void Timer0_Init(void)
{
	INTCONbits.TMR0IF=0;
	INTCONbits.TMR0IE=1;
	T0CON = 0b01000111; // 8bit prescaler 256
	TMR0H=0;
	TMR0L=128;
	T0CONbits.TMR0ON=1;

}


#pragma interrupt Interrupcion
void Interrupcion(void)
{
	if(INTCONbits.TMR0IF)
	{
		INTCONbits.TMR0IF=0;
		if(LATAbits.LATA0==1)
			{
			LATAbits.LATA0=0;
			// LATB=Display_Tabla[9];
			LATB=Display_Tabla[DecenaSegundos];
			LATAbits.LATA1=1;
			}
		else
			{
			LATAbits.LATA1=0;
			// LATB=Display_Tabla[8];
			LATB=Display_Tabla[UnidadSegundos];
			LATAbits.LATA0=1;
			}
	TMR0H=0;
	TMR0L=225; // De 177 (20 ms) para abajo empieza a aparecer el efecto visual del parpadeo
	}
}
#endif
/*FIN EJEMPLO 3*/