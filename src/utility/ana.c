#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef char string[500];
string listaAlelos[3];
string listaPadres[730];
string listaFenotipos[730];

typedef struct archivo{
    int n;
 	string caracteristicas[10];
 	string feno_dominantes[10];
 	string feno_recesivos[10];
 } contenidoArchivo;

string *creaCombinacion(string caracteristica){
    //Recibe un string con la letra que representa
    // a la caracteristica y devuelve el string
    // en minuscula, como homocigoto recesivo.
	int i;
	string result = ""; 
	string *resultado = (string *) malloc(48 + 2);
	for(i = 0; i < strlen(caracteristica); i++){
		result[i] = tolower(caracteristica[i]);
	}
	strcat((*resultado),result);
  return resultado;
}

void clear_buffer(string *buffer){
	/* Este proceso limpia el string
	   dejandolo libre para recibir valor. 
	*/
	memset((*buffer), 0, 50);
       
}

void generaFenotipo(string padre, string listaCaracteristicas[], int n,string domimantes[], string recesivos[], int indice){
    string descripcion = "";
    int i;
    for(i = 0; i < n; i++){
        char *p =  strstr(padre,listaCaracteristicas[i]);
        if(p){
            strcat(descripcion, domimantes[i]);
            strcat(descripcion, " ");
        }else{
            strcat(descripcion, recesivos[i]);
            strcat(descripcion, " ");
        }
    }
    strcat(listaFenotipos[indice],descripcion);
}

int elevarApotencia(int base, int n){
    int i;
    int numero = 1;
    for(i = 0; i < n; i++){
        numero = numero * base;
    }
    return numero;
}

void creaFenotipos(string listaPadres[],int n, string caracteristicas[], string domimantes[], string recesivos[]){
    int i;
    int numero = elevarApotencia(3,n);
    for(i = 0; i < numero; i++){
        printf("%d ->  %s\n",i,listaPadres[i]);
        generaFenotipo( listaPadres[i], caracteristicas, n, domimantes, recesivos, i);
        printf("%d -> %s\n",i,listaFenotipos[i]);
    }
}

void creaAlelos(string caracteristicas){
    /* Esta funcion recibe un string con la carcteristica
       a combinar, y retorna la lista "listaAlelos" con
       las conbinaciones HOMOCIGOTO DOMINANTE, HETEROCIGOTO
       recesivo, homocigoto recesivo.
    */ 
    string temporal = "";
    strcpy(listaAlelos[0],"");
    strcpy(listaAlelos[1],"");
    strcpy(listaAlelos[2],"");
    strcat(temporal,caracteristicas);
    strcat(temporal,caracteristicas);
    strcat(listaAlelos[0],temporal);
    clear_buffer(&temporal);
    strcat(temporal,caracteristicas);
    strcat(temporal,(*creaCombinacion(caracteristicas)));
    strcat(listaAlelos[1],temporal);
    clear_buffer(&temporal);
    strcat(temporal,(*creaCombinacion(caracteristicas)));
    strcat(temporal,temporal);
    strcat(listaAlelos[2],temporal);
    clear_buffer(&temporal);
}

void copiarLista(string lista[], int n){
    //Esta funcion recibe una lista de combinaciones de alelos
    // pasa cada elemento de la lista de entrada a la lista
    // llamada listaPadres, que es una vairable global.
    int i;
    for(i = 0; i < n; i++){
        strcpy(listaPadres[i],lista[i]);
    }
}

void creaPadres(string listaCaracteristicas[], int n){
    /* Esta funcion recibe una lista con las caracteristicas
       domimantes y el numero de caracteristicas. Crea todas
       las combinaciones posibles entre esas caracteristicas
       ,retornando una lista con cada una de ellas.
    */
    int cont = 3;
    string listaPadres2[730];
    int cantidadPadres; 
    int i;
    
    //Este código permite empezar las combinaciones de 
    // caracteristicas con el primer elemento, apartir de 
    // esto se realizan las combinaciones con las otras 
    // caracteristicas.
    creaAlelos(listaCaracteristicas[0]);
    strcat(listaPadres[0],listaAlelos[0]);
    strcat(listaPadres[1],listaAlelos[1]);
    strcat(listaPadres[2],listaAlelos[2]);
    
    for(i = 1; i < n; i++){
        creaAlelos(listaCaracteristicas[i]);
        cantidadPadres = 0;
        int k;
        for(k = 0; k < cont; k++){
            int j;
            for(j = 0; j < 3; j++){
                string temporal = "";
                strcat(temporal,listaPadres[k]);
                strcat(temporal,listaAlelos[j]);
                strcpy(listaPadres2[cantidadPadres],temporal);
                cantidadPadres ++;
            }
        }
        cont = cantidadPadres;
        copiarLista(listaPadres2,cont);
    }
}

void escribirArchivo(string caracteristicas[],string fenoDominantes[], string fenoRecesivos[],int n, string nombreArchivo){
    FILE *archivo;
    int i;
    string extension = "";
    strcat(extension,nombreArchivo);
    strcat(extension,".txt");
    archivo = fopen(extension, "wt");
    fprintf(archivo,"%d\n",n);
    for(i = 0; i < n; i++){
         fprintf(archivo,"%s\n",caracteristicas[i]);
         fprintf(archivo,"%s\n",fenoDominantes[i]);
         fprintf(archivo,"%s\n",fenoRecesivos[i]);
    }
    fclose(archivo);
    printf("Proceso completado");
}

contenidoArchivo leerArchivo(string nombreArchivo){
    FILE *archivo;
 	char caracter;
 	contenidoArchivo contenido;
 	string extension = "";
    strcat(extension,nombreArchivo);
    strcat(extension,".txt");
 	archivo = fopen(extension,"rt");
 	string buffer;
 	fgets(buffer,50,archivo);
 	contenido.n = atoi(buffer);
    int i = 0;
    while (feof(archivo) == 0){
        string tempBuffer = "";
        fgets(buffer,50,archivo);
        strncpy(tempBuffer,buffer,1);
        strcat(contenido.caracteristicas[i],tempBuffer);
        fgets(buffer,50,archivo);
        strcat(contenido.feno_dominantes[i],buffer);
        fgets(buffer,50,archivo);
        strcat(contenido.feno_recesivos[i],buffer);
 		//printf("%s",contenido.caracteristicas[i]);
 		i++;
        
    }
 	    
    fclose(archivo);
    return contenido;
}


void padresGen(){
    string listaCaracteristicas[10] = {"A","B","C","D","E","F"};
    string fenotiposDominantes[10]  = {"Semilla lisa","Tallo alto","Liga","Calvicie","Ojos cafe","Esto"};
    string fenotiposRecesivos[10]   = {"Semilla arrugada","Tallo bajo","Saprissa","Sin calvicie","Ojos claros","Eso"};
    int n = 6;
    creaPadres(listaCaracteristicas,n);
    creaFenotipos(listaPadres,n,listaCaracteristicas,fenotiposDominantes,fenotiposRecesivos);
    return 0;
}

// int main(int argc, char *argv[])
void padresMain()
{
    string listaCaracteristicas[10] = {"A","B","C","D","E","F"};
    string fenotiposDominantes[10]  = {"Semilla lisa","Tallo alto","Liga","Calvicie","Ojos cafe","Esto"};
    string fenotiposRecesivos[10]   = {"Semilla arrugada","Tallo bajo","Saprissa","Sin calvicie","Ojos claros","Eso"};
    int n = 6;
    creaPadres(listaCaracteristicas,n);
    creaFenotipos(listaPadres,n,listaCaracteristicas,fenotiposDominantes,fenotiposRecesivos);


    escribirArchivo(listaCaracteristicas,fenotiposDominantes,fenotiposRecesivos,n,"aTemp");
    contenidoArchivo temp;
    temp = leerArchivo("aTemp");
    printf("Caracteristica: %s",temp.caracteristicas[0]);
    printf("Caracteristica: %s",temp.caracteristicas[1]);
    printf("Caracteristica: %s",temp.caracteristicas[2]);
    printf("Caracteristica: %s",temp.caracteristicas[3]);
    printf("Caracteristica: %s",temp.caracteristicas[4]);
    printf("Caracteristica: %s",temp.caracteristicas[5]);
    //leerArchivo("aTemp");
    return 0;
}

