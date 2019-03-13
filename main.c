#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <math.h>

typedef char string [100];






// Alias para lista

int main()
{



char  padre1[12] = "AaBB";
char  padre2[12] = "Aabb";
//----------------------------------------------------------------------------------------------------------------------------------------
typedef struct nodo
{
	char info[12];
	int bander;
} nodo ;
int largo_entrada = strlen(padre1); // es la cantidad de elementos que debemos generar
int posicion = 0; // a ala posicion de la letra a la que nos referimos
nodo arriba[120]; // son los alelos del padre 1
nodo izquierda[120];// son los alelos del padre 2
int cont_While = 0; //elemento que estamos indexando en este momento
int cont_figuritas = 0; // cantidad de veces que hemos ingresado determinado simbolo
int cont_largo = 1; // es la cantidad de veces que debemos repetir un elemento para formar el alelo
int vueltas = 0; //la posicion donde debe estar el caracter ingresado
nodo valor; // nodo utilizado para inicializar 
while(largo_entrada > 1){
	
	cont_largo = cont_largo * 2;
	largo_entrada -=2;
	
}

largo_entrada = cont_largo;
cont_largo= cont_largo/2;

int movimiento_sensual = 0;
while(posicion < largo_entrada){
	//printf("%d - posicion  \n", posicion);
	//printf("%d - cont_largo  \n", cont_largo);
	 cont_While = 0; 
	 cont_figuritas = 0;
	while((cont_While < largo_entrada) && (cont_largo != 0)){
		//printf("%d entre\n", cont_While );
		if((posicion < 2) && (cont_figuritas < cont_largo)){
			//printf("%dasigne\n", cont_While );
			valor.info[vueltas] = padre1[posicion];
			arriba[cont_While] = valor;
			valor.info[vueltas] = padre2[posicion];
			izquierda[cont_While]= valor;
			cont_While += 1;
			cont_figuritas+=1;

		}
		else if((posicion >= 2) && (cont_figuritas < cont_largo)){
			//printf("%dasigne\n", cont_While );
			arriba[cont_While].info[vueltas] = padre1[posicion];
			izquierda[cont_While].info[vueltas]= padre2[posicion];
			cont_While += 1;
			cont_figuritas+=1;
		}else{
			if(movimiento_sensual == 0){
				posicion += 1;
				cont_figuritas = 0;
				movimiento_sensual = 1;
			}else{
				posicion -= 1;
				cont_figuritas = 0;
				movimiento_sensual = 0;

			}
			



		}


	}
	cont_largo = cont_largo/2;
	posicion += 2;
	vueltas+=1;

}




/*largo_entrada = strlen(padre1)/2;
cont_largo = 2;

while(largo_entrada > 0){
	cont_largo =  cont_largo * 2;
	largo_entrada-=1;
}*/

largo_entrada = strlen(padre1);
largo_entrada = largo_entrada/2;
cont_largo = largo_entrada;
largo_entrada = 1;
while(cont_largo > 0){
	largo_entrada = largo_entrada * 2;
	cont_largo -= 1;

}
largo_entrada -=1;
cont_largo = vueltas = largo_entrada; // bucle interno / resetear cont_largo / bucle externo
posicion =  0; // cantidad de elementos totales
nodo hijos[1200]; // array solucion
while(largo_entrada >= 0){
	
	while(cont_largo >= 0){
		char temporal[120];
		//strcpy(temporal, izquierda[largo_entrada].info);
		//strcpy(valor.info, strcat(temporal, arriba[cont_largo].info));
		int ordenar = 0;
		while(ordenar < (strlen(padre1))){
			if(ordenar == 0){
				if(arriba[cont_largo].info[ordenar] > izquierda[largo_entrada].info[ordenar]){
				temporal[ordenar+1] = arriba[cont_largo].info[ordenar];
				temporal[ordenar] = izquierda[largo_entrada].info[ordenar];
				}else{
					temporal[ordenar] = arriba[cont_largo].info[ordenar];
					temporal[ordenar+1] = izquierda[largo_entrada].info[ordenar];
				}
				ordenar+= 1;


			}else{
				if(arriba[cont_largo].info[ordenar] > izquierda[largo_entrada].info[ordenar]){
				temporal[ordenar*2+1] = arriba[cont_largo].info[ordenar];
				temporal[ordenar*2] = izquierda[largo_entrada].info[ordenar];
				}else{
					temporal[ordenar*2] = arriba[cont_largo].info[ordenar];
					temporal[ordenar*2+1] = izquierda[largo_entrada].info[ordenar];
				}
				ordenar+= 1;
			}
		}
		strcpy(valor.info, temporal);
		hijos[posicion] = valor;
		cont_largo -=1;
		posicion +=1;
	}
	largo_entrada -=1;
	cont_largo = vueltas;

}


//-----------------------------------------------------------------------------------------------------------------------------------
//genotipos
cont_While = 0;
float cantidad[1000];
nodo palabras[1000];
int genotipos = 0;
while(cont_While < posicion){
	if(cont_While == 0){
		strcpy(palabras[genotipos].info,hijos[cont_While].info);
		cantidad[genotipos] =1;
		genotipos += 1;
	}else {
		
		int cont_largo = 0;
		while(cont_largo < genotipos){
			if(strcmp(hijos[cont_While].info, palabras[cont_largo].info) == 0){
				
				cantidad[cont_largo]+=1;
				break;
			}else if((strcmp(hijos[cont_While].info, palabras[cont_largo].info) != 0) && (cont_largo+1 == genotipos) ){
				strcpy(palabras[genotipos].info,hijos[cont_While].info);
				
				
				cantidad[genotipos] =1;
				
				genotipos += 1;
				
				break;
				

			}
			cont_largo+=1;
		}

	}

	cont_While+=1;
}


while(genotipos > 0){

	cantidad[genotipos-1] = cantidad[genotipos-1]/posicion * 100;

	printf("El genotipo %s tiene una probabilidad de - ->  %.0f %%\n", palabras[genotipos-1].info, cantidad[genotipos-1]);
	genotipos-= 1;
}
//--------------------------------------------------------------------------------------------------------------------------------------------

cont_While = 0;
char Fenotipos[1000];
float cantidad_fenotipos[1000];
int Fenotipos_diferentes = 0;
cont_largo = strlen(padre1)/2;
while(cont_While < cont_largo){

	if((padre1[cont_While*2] == padre1[cont_While*2+1]) && (padre1[cont_While*2] == padre2[cont_While*2]) && (padre2[cont_While*2] == padre2[cont_While*2+1])){
		Fenotipos[Fenotipos_diferentes] = padre1[cont_While*2];
		cantidad_fenotipos[Fenotipos_diferentes] = posicion;
		Fenotipos_diferentes+=1;
		printf("me cago en todo\n");
	}else {
		
		if(padre1[cont_While*2] != padre1[cont_While*2+1]){
			Fenotipos[Fenotipos_diferentes] = padre1[cont_While*2];
			cantidad_fenotipos[Fenotipos_diferentes] = 0;
			Fenotipos_diferentes+=1;
			Fenotipos[Fenotipos_diferentes] = padre1[cont_While*2+1];
			cantidad_fenotipos[Fenotipos_diferentes] = 0;
			Fenotipos_diferentes+=1;

		}else if (padre1[cont_While*2] != padre2[cont_While*2]){
			Fenotipos[Fenotipos_diferentes] = padre1[cont_While*2];
			cantidad_fenotipos[Fenotipos_diferentes] = 0;
			Fenotipos_diferentes+=1;
			Fenotipos[Fenotipos_diferentes] = padre2[cont_While*2];
			cantidad_fenotipos[Fenotipos_diferentes] = 0;
			Fenotipos_diferentes+=1;

		}else if (padre1[cont_While*2] != padre2[cont_While*2+1]){
			Fenotipos[Fenotipos_diferentes] = padre1[cont_While*2];
			cantidad_fenotipos[Fenotipos_diferentes] = 0;
			Fenotipos_diferentes+=1;
			Fenotipos[Fenotipos_diferentes] = padre2[cont_While*2+1];
			cantidad_fenotipos[Fenotipos_diferentes] = 0;
			Fenotipos_diferentes+=1;
			
		}else if (padre1[cont_While*2+1] != padre2[cont_While*2]){
			Fenotipos[Fenotipos_diferentes] = padre1[cont_While*2+1];
			cantidad_fenotipos[Fenotipos_diferentes] = 0;
			Fenotipos_diferentes+=1;
			Fenotipos[Fenotipos_diferentes] = padre2[cont_While*2];
			cantidad_fenotipos[Fenotipos_diferentes] = 0;
			Fenotipos_diferentes+=1;
			
		}else if (padre1[cont_While*2+1] != padre2[cont_While*2+1]){
			Fenotipos[Fenotipos_diferentes] = padre1[cont_While*2+1];
			cantidad_fenotipos[Fenotipos_diferentes] = 0;
			Fenotipos_diferentes+=1;
			Fenotipos[Fenotipos_diferentes] = padre2[cont_While*2+1];
			cantidad_fenotipos[Fenotipos_diferentes] = 0;
			Fenotipos_diferentes+=1;
			
		}else if (padre2[cont_While*2] != padre2[cont_While*2+1]){
			Fenotipos[Fenotipos_diferentes] = padre2[cont_While*2];
			cantidad_fenotipos[Fenotipos_diferentes] = 0;
			Fenotipos_diferentes+=1;
			Fenotipos[Fenotipos_diferentes] = padre2[cont_While*2+1];
			cantidad_fenotipos[Fenotipos_diferentes] = 0;
			Fenotipos_diferentes+=1;
			
		}
		cont_figuritas = 0;
		while(cont_figuritas < posicion){
			if(Fenotipos[Fenotipos_diferentes - 2] == hijos[cont_figuritas].info[cont_While*2]){
				cantidad_fenotipos[Fenotipos_diferentes-2]+=1;
			}else{
				cantidad_fenotipos[Fenotipos_diferentes-1]+=1;
			}
			cont_figuritas+=1;

		}

	}

	cont_While+=1;
}

while(Fenotipos_diferentes > 0){

	cantidad_fenotipos[Fenotipos_diferentes-1] = cantidad_fenotipos[Fenotipos_diferentes-1]/posicion * 100;

	printf("El Fenotipo %c tiene una probabilidad de - ->  %.0f %%\n", Fenotipos[Fenotipos_diferentes-1], cantidad_fenotipos[Fenotipos_diferentes-1]);
	Fenotipos_diferentes-= 1;
}





while(posicion > 0){
	printf("%s -- elemento %d\n", hijos[posicion-1].info, posicion );
	posicion -= 1;
}





}

