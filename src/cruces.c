#include <gtk/gtk.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "utility/utilidades.h"


#define gtk_spin_button_get_value_as_float gtk_spin_button_get_value
#define MINAUX(a, b) ((a) < (b) ? a : b) // return min a , b
#define path(a, b, c, d) ((a) != (b) ? d : c) 


GtkBuilder    *myBuilder; 
GtkWidget     *windowInitial;
GtkWidget     *windowCreateData;
GtkWidget     *windowGenotipos;
GtkWidget     *windowFinal;
GtkWidget     *chooseFileButton;
GtkWidget     *windowSelectSize;
GtkWidget     *windowSelectFile;
GtkWidget     *windowError;
GtkWidget     *window_ingresar_info;	//detalle de las caracteristicas 

GtkWidget     *spinButtonNode;
GtkWidget     *graph;
//CreateData 
GtkWidget     ***tableD0;
GtkWidget 		*scrolledTable;
GtkWidget 		*columnD0;

GtkWidget     ***tableP;
GtkWidget 		*columnP;
GtkWidget 		*genotypeTable;
GtkWidget 		*genotypeTable1;

GtkWidget     *label_table_DNumber;
GtkWidget     *label_betterPath;
GtkWidget     *label_Table_P;
GtkWidget     *label_peso;
GtkWidget     *graph_darea;

const char **header;
char bufferForFile[9];
int loadFileFlag = 0;
char *filename;

int begin = -1;
int end = -1;


//final
GtkWidget       *combobox_from;
GtkWidget       *combobox_to;
GtkWidget       *container_for_combobox_from;
GtkWidget       *container_for_combobox_to;
GtkWidget       *filenameEntry;
GtkWidget	*ent_letra;
GtkWidget	*ent_descripcion;
GtkWidget       *filenameLabel;
GtkWidget       *subtitleLabel;
GtkWidget       *saveButton;
GtkWidget       *windowSave;
GtkWidget       *drawButton;


//Guardar Archivo
FILE *infoFile;
FILE *infoFileAux;

const char *alphabetNodes[27]={"A","B","C","D","E","F","G","H","I","J",
"K","L","M","N","Ñ","O","P","Q","R","S","T","U","V","W","X","Y","Z"};

struct caracteristica {
	char letra[1];
	char descripcion[20];	
	};

//struct caracteristica *caracteristicas[20]={NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL}; 

int inputNumberNodes;
int numberNodes = 0;
int totalNodes  = 0;
int totalCycles = 0;




void on_window_main_destroy(){gtk_main_quit();}

void myCSS(void){
	GtkCssProvider *provider;
	GdkDisplay *display;
	GdkScreen *screen;

	provider = gtk_css_provider_new ();
	display = gdk_display_get_default ();
	screen = gdk_display_get_default_screen (display);
	gtk_style_context_add_provider_for_screen (screen, GTK_STYLE_PROVIDER (provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

	const gchar *myCssFile = "myStyle.css";
	GError *error = 0;

	gtk_css_provider_load_from_file(provider, g_file_new_for_path(myCssFile), &error);
	g_object_unref (provider);
}



void createGenotipos() {
    GtkWidget *entry, *radio1, *radio2;
    tableP = calloc(numberNodes,sizeof(GtkWidget**));
    // https://developer.gnome.org/gtk3/stable/GtkGrid.html
		printf("%d\n", numberNodes);
    columnP = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(genotypeTable), columnP);
    for(int j = 0; j < numberNodes; j++) {
        tableP[j] = calloc(numberNodes, sizeof(GtkWidget*));
    }



    // -------------------------------------------------------
    // father genotypes

    for(int j = 0; j < numberNodes; j++) 
    {

        if (j == 0){
            // Create the first entry
            // Create a radio button with a GtkEntry widget
            radio1 = gtk_radio_button_new_with_label(NULL, "AABBaaB");
            gtk_grid_attach (GTK_GRID(columnP), radio1, 0, 0, 1, 1);
        }
        if (j != 0){
            // Create a radio button with a label
            radio2 = gtk_radio_button_new_with_label_from_widget(GTK_RADIO_BUTTON(radio1),
                                                                "BABbbAA");
            tableP[j][0] = radio2;
            gtk_grid_attach (GTK_GRID(columnP), tableP[j][0], 0, j, 1, 1);
        }
    }

    // -------------------------------------------------------
    // mother genotypes

    GtkWidget *radioM1, *radioM2;
    GtkWidget     ***tableP2;
    GtkWidget 		*columnP2;
    columnP2 = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(genotypeTable1), columnP2);
    for(int j = 0; j < numberNodes; j++) {
        tableP2[j] = calloc(numberNodes, sizeof(GtkWidget*));
    }

    for(int j = 0; j < numberNodes; j++) 
    {

        if (j == 0){
            // Create the first entry
            // Create a radio button with a GtkEntry widget
            radioM1 = gtk_radio_button_new_with_label(NULL, "DDBBaaB");
            gtk_grid_attach (GTK_GRID(columnP2), radioM1, 0, 0, 1, 1);
        }
        if (j != 0){
            // Create a radio button with a label
            radioM2 = gtk_radio_button_new_with_label_from_widget(GTK_RADIO_BUTTON(radioM1),
                                                                "CCCbbAA");
            tableP2[j][0] = radioM2;
            gtk_grid_attach (GTK_GRID(columnP2), tableP2[j][0], 0, j, 1, 1);
        }
    }



    gtk_widget_show_all(windowGenotipos);
}


void createTableD0 (int nodes)
{
	numberNodes = nodes;
	printf("%d\n", numberNodes);
	tableD0 = calloc(nodes,sizeof(GtkWidget**));
	columnD0 = gtk_grid_new ();


	for(int j = 0; j < nodes; j++) {
		tableD0[j] = calloc(nodes,sizeof(GtkWidget*));
	}

	gtk_container_add(GTK_CONTAINER(scrolledTable), columnD0);

	for(int i = 0; i < nodes; i++)
	{
		for(int j = 0; j < 4; j++)
		{	
			tableD0[i][j] = gtk_entry_new();
			gtk_entry_set_width_chars(GTK_ENTRY(tableD0[i][j]),8);
			gtk_grid_attach (GTK_GRID(columnD0),tableD0[i][j], j, i, 1, 1);

			if(i == 0 && j == 0){
				gtk_entry_set_text (GTK_ENTRY(tableD0[i][j]),"CD");
				gtk_widget_set_sensitive(tableD0[i][j],FALSE);
			}

			if(i == 0 && j == 1){
				// Descripción caracteristicas dominantes -> table[1...n][1]
				gtk_entry_set_text (GTK_ENTRY(tableD0[i][j]),"descrip.");
				gtk_widget_set_sensitive(tableD0[i][j],FALSE);
			}

			if(i == 0 && j == 2){
				gtk_entry_set_text (GTK_ENTRY(tableD0[i][j]),"CR");
				gtk_widget_set_sensitive(tableD0[i][j],FALSE);
			}

			if(i == 0 && j == 3){
				// Descripción caracteristicas recesivas -> table[1...n][3]

				gtk_entry_set_text (GTK_ENTRY(tableD0[i][j]),"descrip.");
				gtk_widget_set_sensitive(tableD0[i][j],FALSE);
			}

			if(i == 0 && j != 0){
				//gtk_entry_set_text (GTK_ENTRY(tableD0[i][j]),"0");
				gtk_widget_set_sensitive(tableD0[i][j],FALSE);
			}
			if (j == 2 && i != 0){
				//Caracteristicas recesivas -> table[1...n][2]

				gtk_entry_set_text (GTK_ENTRY(tableD0[i][j]),"c");
				gtk_widget_set_sensitive(tableD0[i][j],FALSE);

				//gtk_widget_hide(window_ingresar_info); 
			}
			if (j ==0 && i!=0){
				//Caracteristicas dominantes -> table[1...n][2]
				gtk_entry_set_text (GTK_ENTRY(tableD0[i][j]),"C");
				
			}
			
		}
	}

	gtk_widget_show_all(windowCreateData); 
}

void guardarInfo(){

	// Para guardar los datos ingresados por el usuario
	char caracteristicas[numberNodes][20];
	char feno_dominantes[numberNodes][20];
	char feno_recesivos[numberNodes][20];

	printf("%d\n", numberNodes);

	for(int i = 0; i < numberNodes; i++)
	{
		for(int j = 0; j < 4; j++)
		{
			//Caracteristicas dominantes -> table[1...n][2]
			if (j == 0 && i!=0)
			{
				strcpy(caracteristicas[i-1], gtk_entry_get_text(GTK_ENTRY(tableD0[i][j])));
				//caracteristicas[i] = gtk_entry_get_text(GTK_ENTRY(tableD0[i][j]));
			}
			
			// Descripción caracteristicas dominantes -> table[1...n][1]
			if(j == 1 && i != 0)
			{
				strcpy(feno_dominantes[i-1], gtk_entry_get_text(GTK_ENTRY(tableD0[i][j])));
				//feno_dominantes[i] = gtk_entry_get_text(GTK_ENTRY(tableD0[i][j]));
			}
			
			//Caracteristicas recesivas -> table[1...n][2]
			if (j == 2 && i != 0)
			{
				//gtk_entry_get_text(GTK_ENTRY(tableD0[i][j]));	
			}
			
			// Descripción caracteristicas recesivas -> table[1...n][3]
			if (j == 3 && i != 0) 
			{
				strcpy(feno_recesivos[i-1], gtk_entry_get_text(GTK_ENTRY(tableD0[i][j])));
				//feno_recesivos[i] = gtk_entry_get_text(GTK_ENTRY(tableD0[i][j]));
			
			}


		}
	}

	//Imprimir datos para verificar 
   int loop;
   printf("Caracteristicas: \n");
   for(loop = 0; loop < numberNodes; loop++)
      printf("%s\n", caracteristicas[loop]);

   int loop1;
   printf("Fenotipos Dominantes: \n");
   for(loop1 = 0; loop1 < numberNodes; loop1++)
      printf("%s\n", feno_dominantes[loop1]);

   int loop2;
   printf("Fenotipos Recesivos: \n");
   for(loop2 = 0; loop2 < numberNodes; loop2++)
      printf("%s\n", feno_recesivos[loop2]);


}

void setTableD(int MatrixD[][numberNodes-1]){
	int nodes = numberNodes;
	char labelInstruction[100] = "Características(";
	char tableNumber[4];
	sprintf(tableNumber, "%d", totalCycles);
	strcat(labelInstruction, tableNumber);
	strcat(labelInstruction, ")");

	gtk_label_set_text(GTK_LABEL(label_table_DNumber), labelInstruction);

	for(int i =0; i < nodes; i++)
	{ 
		for(int j=0; j < nodes; j++) 
		{
			char str[10];
			gtk_widget_set_sensitive(tableD0[i][j],FALSE);

			if (i == 0 && j != 0){
				gtk_entry_set_text (GTK_ENTRY(tableD0[i][j]), header[j]);
			}
			if (j ==0 && i!=0){
				gtk_entry_set_text (GTK_ENTRY(tableD0[i][j]),header[i]);
			}

			if (j !=0 && i!=0){
				sprintf(str, "%d", MatrixD[i-1][j-1]);

				 if (strcmp("1000000",str) == 0){
				 	// Los valores que en la tabla actual son infinitos
				 	gtk_widget_set_name (tableD0[i][j],"oldValue");
				 	gtk_entry_set_text (GTK_ENTRY(tableD0[i][j]),"INF");

				 } else if (strcmp(str,gtk_entry_get_text(GTK_ENTRY(tableD0[i][j]))) == 0) {
					gtk_widget_set_name (tableD0[i][j],"oldValue");
					gtk_entry_set_text (GTK_ENTRY(tableD0[i][j]),str);
				} else {
					gtk_widget_set_name (tableD0[i][j],"newValue");
					gtk_entry_set_text (GTK_ENTRY(tableD0[i][j]),str);
				}
			}
		}
	}

	gtk_widget_show_all(scrolledTable); 
}

void setTableP(int matrixP[][numberNodes-1]){
	int nodes = numberNodes;

	for(int i =0; i < nodes; i++) 
	{
		for(int j=0; j < nodes; j++) 
		{
			char str[10];

			if (i == 0 && j != 0){
				gtk_entry_set_text (GTK_ENTRY(tableP[i][j]),header[j]);
			}
			if (j ==0 && i!=0){
				gtk_entry_set_text (GTK_ENTRY(tableP[i][j]),header[i]);
			}
			if (j !=0 && i!=0){
				sprintf(str, "%d", matrixP[i-1][j-1]);
				if (strcmp(str,gtk_entry_get_text(GTK_ENTRY(tableP[i][j])))==0) {
					gtk_widget_set_name (tableP[i][j],"oldValue");
				} else {
					gtk_widget_set_name (tableP[i][j],"newValueP");
				}
				gtk_entry_set_text (GTK_ENTRY(tableP[i][j]),str);
			}
		}
	}
	gtk_widget_show_all(genotypeTable); 
}

void setTableFile(int Matriz[][numberNodes-1]){
	int nodes=numberNodes;
	tableD0 = calloc(nodes,sizeof(GtkWidget**));


	columnD0 = gtk_grid_new ();
	gtk_container_add (GTK_CONTAINER (scrolledTable), columnD0);

	for(int j = 0; j < nodes; j++) {
		tableD0[j] = calloc(nodes,sizeof(GtkWidget*));
	}

	for(int i =0; i < nodes; i++) 
	{
		for(int j=0; j < nodes; j++) 
		{
			char str[10];
			tableD0[i][j] = gtk_entry_new();
			gtk_entry_set_width_chars(GTK_ENTRY(tableD0[i][j]),8);
			gtk_grid_attach (GTK_GRID (columnD0),tableD0[i][j] , j, i, 1, 1);

			if(i == 0 && j == 0){
				gtk_widget_set_sensitive(tableD0[i][j],FALSE);
			}

			if(i==j && i != 0 && j != 0){
				gtk_entry_set_text (GTK_ENTRY(tableD0[i][j]),"0");
				gtk_widget_set_sensitive(tableD0[i][j],FALSE);
			}
			if (i == 0 && j != 0){
				gtk_entry_set_text (GTK_ENTRY(tableD0[i][j]),header[j]);
			}
			if (j ==0 && i!=0){
				gtk_entry_set_text (GTK_ENTRY(tableD0[i][j]),header[i]);
				gtk_widget_set_sensitive(tableD0[i][j],FALSE);
			}

			if (j !=0 && i!=0 && i!=j) {
				if ( Matriz[i-1][j-1] == 1000000){
					gtk_entry_set_text (GTK_ENTRY(tableD0[i][j]),"INF");
				}else{
					sprintf(str, "%d", Matriz[i-1][j-1]);
					gtk_entry_set_text (GTK_ENTRY(tableD0[i][j]),str);
				}

			}

		}
	}

	gtk_widget_show_all(windowCreateData); 
}

void appendChar(int _val) {
	if (strlen(bufferForFile) == 0) {
		char dato = (char)_val;
		char auxiliar[] = {dato,'\0'};
		strcat(bufferForFile,auxiliar);
	} else{
		char dato = (char)_val;
		char auxiliar[]={dato,'\0'};
		strcat(bufferForFile,auxiliar);
	}
}

void cleanBuffer(){
	memset(bufferForFile,'\0',strlen(bufferForFile));
}

int getNext() {
	cleanBuffer();

	int ch;

	while (feof(infoFileAux)==0) {   
		ch = fgetc(infoFileAux);
		if (ch=='\n') {
			return 0;
		}
		if (ch==';') {
			return 1;
		}
		appendChar(ch);
	}
	return 0;
}

int loadData(char *filename)
{
	infoFile = fopen(filename, "r");
	int ch;
	if(infoFile != NULL)
	{
		while(feof(infoFile) == 0) {  
			ch = fgetc(infoFile);
			if (ch == '\n'){
				break;
			}
			if (ch ==';'){
				numberNodes ++;
			}
		}

		fclose(infoFile);
		return 1;
	}
	return 0;
}

void createTablesFromFile(){
	int matrizAux[numberNodes-1][numberNodes-1];
	header = malloc(numberNodes * sizeof(char*));
	infoFileAux = fopen(filename, "r");

	int i = 0;
	int j = 0;
	int flag = 0;
	int accion = getNext();
	int var = 0;

	while (var < numberNodes) {
		
		if (accion == 0){
			break;
		}
		char * data = malloc(1000);
		strcpy(data,bufferForFile);

		header[var] = data;

		var ++;
		accion = getNext();
	}

	while (i < numberNodes-1) {
		while (j < numberNodes-1) {
			accion = getNext();
			if (flag == 0) {
				flag = 1;
				accion = getNext();
			}
			if (accion == 1) {
				if(strcmp("INF",bufferForFile) == 0){
					// Traduce el infinito como un millon.
					matrizAux[i][j] = 1000000;
					j ++;
				}else{
					int valor = atoi(bufferForFile);

					if(valor >= 1000000){
						matrizAux[i][j] = 1000000;
						j ++;
					}else{
						matrizAux[i][j] = valor;
						j ++;
					}
				}

			}	
		}
		j = 0;
		i ++;
		flag = 0;
		accion = getNext();
	}

	createGenotipos();
	setTableFile(matrizAux);
}
void createInfoFile(char *filename) {
	infoFile = fopen(filename,"w+");
	for(int i =0; i < numberNodes; i++) 
	{
	    for(int j=0; j < numberNodes; j++) 
	    {
	      
	      fprintf(infoFile,"%s;",(gtk_entry_get_text(GTK_ENTRY(tableD0[i][j]))));
	    }
	     fprintf(infoFile,"\n");
	}
	fclose(infoFile);
}

void saveFile()
{
    char filename[1000] = "test/floyd/";

    int len = gtk_entry_get_text_length (GTK_ENTRY(filenameEntry));
    if(len != 0 )
    {
    	strcat(filename,gtk_entry_get_text (GTK_ENTRY(filenameEntry)));
    	strcat(filename,".txt");
    	
    	createInfoFile(filename);
    	gtk_entry_set_text(GTK_ENTRY(filenameEntry),"");
    	gtk_widget_show_all(windowSave);
    }
}

void saveTemp()
{
    char filename[1000] = "temp/";
    strcat(filename,"temp");
    strcat(filename,".txt");
    createInfoFile(filename);
}


void destroy()
{
	gtk_widget_hide(windowSave);
}

void destroyGetPath()
{
	gtk_widget_hide(windowFinal);
}

int loadFile()
{
	filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(chooseFileButton));
	int flag = loadData(filename);
	return flag;
}

void createMatrixFile()
{
	if(loadFile() == 1)
	{
		gtk_widget_hide(windowSelectFile);
		loadFileFlag = 1;
		createTablesFromFile();
		gtk_widget_show_all(windowCreateData);
	} 
}

void printMatrix(int matrix[numberNodes-1][numberNodes-1], int totalNode)
{
	for (int i = 0; i < totalNode; i++)
	{
		for (int j = 0; j < totalNode; j++)
		{
			printf("%d\t", matrix[i][j]);
		}
		printf("\n");
	}
	printf("\n\n\n");
}

void floydAux(int matrix[numberNodes-1][numberNodes-1], int matrixAux[numberNodes-1][numberNodes-1], int matrixP[numberNodes-1][numberNodes-1], int totalNode, int node)
{
	for (int i = 0; i < totalNode; i++)
    { 
        int p;
        for (int j = 0; j < totalNode; j++)
        { 
            p = MINAUX(matrix[i][j], matrix[i][node - 1] + matrix[node - 1][j]);
            matrixAux[i][j] = p;
            matrixP[i][j] = path(p, matrix[i][j], matrixP[i][j], node);
        }
    }
    for (int i = 0; i < totalNode; i++)
    { 
        for (int j = 0; j < totalNode; j++)
        { 
            matrix[i][j] = matrixAux[i][j];     
        }
    }
}


void createGraph()
{
	gtk_widget_hide(windowInitial); 
	gtk_widget_show_now(windowSelectSize);
}

void loadGraph()
{
	gtk_widget_hide(windowInitial); 
	gtk_widget_show_now(windowSelectFile);
}

//Se devuelve a la pantalla inicial
void goBackFile()
{
	gtk_widget_hide(windowSelectFile); 
	gtk_widget_show_now(windowInitial);
}

//Se devuelve a la pantalla inicial
void goBackSize()
{
	gtk_widget_hide(windowSelectSize); 
	gtk_widget_show_now(windowInitial);
}

void createMatrix()
{
	int i = 0;
	gtk_widget_hide(windowSelectSize); 
	inputNumberNodes = gtk_spin_button_get_value_as_int (GTK_SPIN_BUTTON(spinButtonNode));	
	inputNumberNodes++; 
	createTableD0(inputNumberNodes);
	gtk_widget_show_now(windowCreateData);
}

void createHeader() {
	header = malloc(numberNodes * sizeof(char*));
	for(int i =0; i < numberNodes; i++) 
	{
		for(int j=0; j < numberNodes; j++) 
		{
			if (i == 0){
				header[j] = gtk_entry_get_text(GTK_ENTRY(tableD0[i][j]));
			}
		}
	}
}

void applyFloyd() {
	int MatrizD[numberNodes-1][numberNodes-1];
	int matrixP[numberNodes-1][numberNodes-1];
	for(int i =0; i < numberNodes; i++) 
	{
		for(int j=0; j < numberNodes; j++) 
		{
			if (i != 0 && j != 0)
            {
				if (strcmp(gtk_entry_get_text(GTK_ENTRY(tableD0[i][j])), "INF") == 0)
                {
					// Si en la tabla actual hay un infinitos se traduce a un millon.
					MatrizD[i-1][j-1] = 1000000;
					matrixP[i-1][j-1] = atoi(gtk_entry_get_text(GTK_ENTRY(tableP[i][j])));

				}
				else
                {
					// Si en la tabla actual hay un numero más grande que millon se traduce a infinito.
					int auxiliar = atoi(gtk_entry_get_text(GTK_ENTRY(tableD0[i][j])));
					if (auxiliar > 1000000)
                    {
						MatrizD[i-1][j-1] = 1000000;
						matrixP[i-1][j-1] = atoi(gtk_entry_get_text(GTK_ENTRY(tableP[i][j])));
					}
                    else
                    {
						MatrizD[i-1][j-1] = auxiliar;
						matrixP[i-1][j-1] = atoi(gtk_entry_get_text(GTK_ENTRY(tableP[i][j])));
					}
				}
			}
		}
	}

	int matrixAux[numberNodes-1][numberNodes-1];;
	floydAux(MatrizD, matrixAux, matrixP,numberNodes-1, totalCycles);

	//printf("%s\n","matriz post" );
	//printMatrix(MatrizD, numberNodes-1);
	setTableD(MatrizD);
	setTableP(matrixP);
	totalCycles++;
}

void fillCombobox()
{
	combobox_from = gtk_combo_box_text_new();
	combobox_to = gtk_combo_box_text_new();

	for (int i = 0; i < numberNodes; ++i)
	{
		gtk_combo_box_text_append (GTK_COMBO_BOX_TEXT(combobox_from),NULL,header[i]);
		gtk_combo_box_text_append (GTK_COMBO_BOX_TEXT(combobox_to),NULL,header[i]);
	}
	gtk_container_add(GTK_CONTAINER(container_for_combobox_from), combobox_from);
	gtk_container_add(GTK_CONTAINER(container_for_combobox_to), combobox_to);
}

void execFloyd()
{
	gtk_widget_hide(filenameEntry);
	gtk_widget_hide(filenameLabel);
	gtk_widget_hide(subtitleLabel);
	gtk_widget_hide(saveButton);
	gtk_label_set_text(GTK_LABEL(label_Table_P), " Table P");

	if (totalCycles == 0) {
		totalCycles++;
		createHeader();
		applyFloyd();
	}
    else
    {
        if (totalCycles < numberNodes) {
            applyFloyd();
        }
        if (totalCycles == numberNodes) {
            fillCombobox();
            gtk_widget_show_all(windowFinal);
            totalCycles++;
        }
        if (totalCycles>numberNodes)
        {
            gtk_widget_show_all(windowFinal);
        }
	}
}

void getOptimalPath(int begin,int end, char label []){
	char arrow[7] = " --> ";
	//printf("%s\n", gtk_entry_get_text(GTK_ENTRY(tableP[begin][end])));
	int medium = atoi(gtk_entry_get_text(GTK_ENTRY(tableP[begin][end])));
	

	if (medium == 0){
		//printf("Ruta directa de %d a %d \n",begin,end);
		strcat(label,arrow);
		strcat(label, header[end]);
	}
	else{
		strcat(label,arrow);
		strcat(label, header[medium]);
		//printf("Tome %d y pase por %d \n",begin,medium);
		getOptimalPath(medium,end, label);
	}
	gtk_label_set_text (GTK_LABEL(label_betterPath),label);
}


void getOptimalPath_button() {
	char resultPre [10000] = "";
	char resultDistance [10000] = "Total distance: ";
	
	//trcpy(resultPreOptimal,"");
	for (int i = 0; i < numberNodes; ++i)
	{
		if(strcmp(header[i],gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(combobox_from))) == 0) {
			begin = i;
		}
		if(strcmp(header[i],gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(combobox_to))) == 0) {
			end = i;
		}
	}

	//printf("%d, %d\n",begin, end );
	if((begin != -1)  && (end != -1)){
		strcat(resultPre,header[begin]);
		//resultPreOptimal = header[begin];

		//print 
		getOptimalPath(begin,end, resultPre);
		int peso = atoi(gtk_entry_get_text(GTK_ENTRY(tableD0[begin][end])));
		char peso_str[80];
		sprintf(peso_str, "%d", peso);

		if(strcmp(gtk_entry_get_text(GTK_ENTRY(tableD0[begin][end])), "INF") != 0){
			strcat(resultDistance, peso_str);
			gtk_label_set_text (GTK_LABEL(label_peso), resultDistance);
		}else{
			strcat(resultDistance, " there is no route");
			gtk_label_set_text (GTK_LABEL(label_peso), resultDistance);
		}
		

	}
}

void closeSave()
{
	gtk_widget_hide(windowSave); 
}

void drawGraph()
{
	saveTemp();
	system("./drawing &");
}

void closeError()
{
	gtk_widget_hide(windowError); 
}

//------------------------------------------------------------------------------------------------------------------------------------
// Funciones para cruces mendelianos

void openGenotipos()
{
	createGenotipos();
	return;
}



//------------------------------------------------------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------------------------------------------------------
/*FUNCION PARA PERMITIR SOLO NUMEROS EN ENTRADAS*/

gboolean on_key_press (GtkWidget *widget, GdkEventKey *evento, gpointer user_data)
//Permite solo numeros en entradas, restringiendo los keypress
{
    if ((evento->keyval >= GDK_KEY_A) && (evento->keyval <= GDK_KEY_Z))
        return FALSE;	

    if ((evento->keyval >= GDK_KEY_a) && (evento->keyval <= GDK_KEY_z))
        return FALSE;

    if (evento->keyval == GDK_KEY_Right)
        return FALSE;
    
    if (evento->keyval == GDK_KEY_Left)
        return FALSE;
    
    if (evento->keyval == GDK_KEY_BackSpace) 
        return FALSE;
    
    if (evento->keyval == GDK_KEY_space) 
        return FALSE;
    else
            return TRUE; 
}

int main(int argc, char *argv[])
{
    gtk_init(&argc, &argv);
    myCSS();

    myBuilder = gtk_builder_new();
    gtk_builder_add_from_file (myBuilder, "glade/window_floyd.glade", NULL);
    //g_signal_connect (G_OBJECT (window), "key_press_event", G_CALLBACK (on_key_press), NULL);   
    windowInitial = GTK_WIDGET(gtk_builder_get_object(myBuilder, "window_floyd_start"));
    windowCreateData = GTK_WIDGET(gtk_builder_get_object(myBuilder, "window_floyd"));
    windowGenotipos = GTK_WIDGET(gtk_builder_get_object(myBuilder, "window_genotipos"));
    windowFinal  = GTK_WIDGET(gtk_builder_get_object(myBuilder, "window_floyd_results"));

    g_signal_connect (G_OBJECT (windowCreateData), "key_press_event", G_CALLBACK (on_key_press), NULL);   
    window_ingresar_info = GTK_WIDGET(gtk_builder_get_object(myBuilder, "window_ingresar_info"));  

    g_signal_connect (G_OBJECT (window_ingresar_info), "key_press_event", G_CALLBACK (on_key_press), NULL); 
    windowSelectSize = GTK_WIDGET(gtk_builder_get_object(myBuilder, "window_floyd_selectSize"));
    windowSelectFile = GTK_WIDGET(gtk_builder_get_object(myBuilder, "window_floyd_selectFile"));
    windowError = GTK_WIDGET(gtk_builder_get_object(myBuilder, "window_floyd_error"));
    windowSave = GTK_WIDGET(gtk_builder_get_object(myBuilder, "window_floyd_saved"));
    spinButtonNode = GTK_WIDGET(gtk_builder_get_object(myBuilder, "ent_size"));

    gtk_spin_button_set_range (GTK_SPIN_BUTTON(spinButtonNode),1,27);
    gtk_spin_button_set_increments (GTK_SPIN_BUTTON(spinButtonNode),1,3);
    label_table_DNumber = GTK_WIDGET(gtk_builder_get_object(myBuilder, "lbl_table"));
    scrolledTable = GTK_WIDGET(gtk_builder_get_object(myBuilder, "scrolled_table"));
    genotypeTable = GTK_WIDGET(gtk_builder_get_object(myBuilder, "genotype_table"));
    genotypeTable1 = GTK_WIDGET(gtk_builder_get_object(myBuilder, "genotype_table1"));
    combobox_from = GTK_WIDGET(gtk_builder_get_object(myBuilder, "combobox_from"));
    combobox_to = GTK_WIDGET(gtk_builder_get_object(myBuilder, "combobox_to"));
    container_for_combobox_from = GTK_WIDGET(gtk_builder_get_object(myBuilder,"container_from"));
    container_for_combobox_to = GTK_WIDGET(gtk_builder_get_object(myBuilder,"container_to"));
    label_betterPath = GTK_WIDGET(gtk_builder_get_object(myBuilder, "lbl_result"));
    label_peso = GTK_WIDGET(gtk_builder_get_object(myBuilder, "lbl_peso"));
    chooseFileButton = GTK_WIDGET(gtk_builder_get_object(myBuilder, "btn_fileChooser"));
    filenameEntry = GTK_WIDGET(gtk_builder_get_object(myBuilder, "ent_fileName"));
    filenameLabel = GTK_WIDGET(gtk_builder_get_object(myBuilder, "lbl_fileName"));
    subtitleLabel = GTK_WIDGET(gtk_builder_get_object(myBuilder, "lbl_subtitle3"));
    label_Table_P = GTK_WIDGET(gtk_builder_get_object(myBuilder, "lbl_graph"));
    saveButton = GTK_WIDGET(gtk_builder_get_object(myBuilder, "btn_save")); 
    drawButton = GTK_WIDGET(gtk_builder_get_object(myBuilder, "btn_dibujar"));
    graph_darea = GTK_WIDGET(gtk_builder_get_object(myBuilder, "graph_area"));
    graph = GTK_WIDGET(gtk_builder_get_object(myBuilder, "window_floyd_graph"));

    gtk_builder_connect_signals(myBuilder, NULL);
    g_object_unref(myBuilder);
    gtk_widget_show_all(windowInitial);                
    gtk_main();
    return 0;
}

