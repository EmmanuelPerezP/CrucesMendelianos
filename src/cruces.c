#include <gtk/gtk.h>
#include "utility/aBiologia.c"

#define gtk_spin_button_get_value_as_float gtk_spin_button_get_value
#define MINAUX(a, b) ((a) < (b) ? a : b) // return min a , b
#define path(a, b, c, d) ((a) != (b) ? d : c) 


GtkBuilder    *myBuilder; 
GtkWidget     *windowInitial;
GtkWidget     *windowCreateData;
GtkWidget     *windowGenotipos;
GtkWidget     *windowPercentage;
GtkWidget     *windowDescendencia;
GtkWidget     *chooseFileButton;
GtkWidget     *windowSelectSize;
GtkWidget     *windowSelectFile;

GtkWidget     *spinButtonNode;
//CreateData 
GtkWidget     ***tableD0;
GtkWidget 		*scrolledTable;
GtkGrid 		*columnD0;

GtkWidget  	  *windowInputError;
GtkWidget     *windowRepeatError;

GtkWidget     ***tableP;
GtkWidget 		*columnP;
GtkWidget 		*genotypeTable;
GtkWidget 		*genotypeTable1;
GtkWidget 		*offspringTable;

GtkWidget *labelGenotipoPadre;
GtkWidget *labelGenotipoMadre;
GtkWidget *labelFenotipoPadre;
GtkWidget *labelFenotipoMadre;


const char **header;
char bufferForFile[9];
int loadFileFlag = 0;
char *filename;

int begin = -1;
int end = -1;

GtkWidget       *filenameEntry;
GtkWidget       *windowSave;

//Guardar Archivo
FILE *infoFile;
FILE *infoFileAux;


struct caracteristica {
    char letra[1];
    char descripcion[20];	
};


int inputNumberNodes = 0;
int numberNodes = 0;
int totalNodes  = 0;
int totalCycles = 0;

int cantidadCaracteristicas;

// quit app
void on_window_main_destroy(){
    gtk_main_quit();
}

// BIOLO CODE --------------------------------------------------------------------------------------

// this is the callback that is called everytime the father genotype changes
void fatherCallback(GtkToggleButton *togglebutton, gpointer user_data) {
    // get the label from the widget toggled
    gchar *label;
    label = gtk_button_get_label(togglebutton);
    if (gtk_toggle_button_get_active(togglebutton)){
        printf("father: %s\n", label);
        // get the father group
        GSList *group = gtk_radio_button_get_group (GTK_RADIO_BUTTON (togglebutton));
        // get the index of the radio button toggled
        // https://people.gnome.org/~desrt/glib-docs/glib-Singly-Linked-Lists.html#g-slist-index
        int radioIndex = g_slist_index (group, togglebutton);
        // index real ya que el index es el opuesto en el array
        int indexReal = elevarApotencia(3,cantidadCaracteristicas)-1 - radioIndex;
        gtk_label_set_text (labelGenotipoPadre, listaPadres[indexReal]);
        gtk_label_set_text (labelFenotipoPadre, listaFenotipos[indexReal]);
    }
}

// this is the callback that is called everytime the mother genotype changes
void motherCallback(GtkToggleButton *togglebutton, gpointer user_data) {
    // get the label from the widget toggled
    gchar *label;
    label = gtk_button_get_label(togglebutton);
    if (gtk_toggle_button_get_active(togglebutton)){
        printf("mother: %s\n", label);
        // get the mother group
        GSList *group = gtk_radio_button_get_group (GTK_RADIO_BUTTON (togglebutton));
        // get the index of the radio button toggled
        // https://people.gnome.org/~desrt/glib-docs/glib-Singly-Linked-Lists.html#g-slist-index
        int radioIndex = g_slist_index (group, togglebutton);
        int indexReal = elevarApotencia(3,cantidadCaracteristicas)-1 - radioIndex;
        // set the label
        gtk_label_set_text (labelGenotipoMadre, listaPadres[indexReal]);
        gtk_label_set_text (labelFenotipoMadre, listaFenotipos[indexReal]);
    }
}


void createGenotipos(int n) {
    int number = elevarApotencia(3,n);
    int numbre2 = elevarApotencia(2,n);
    GtkWidget *radio1, *radio2;
    // https://developer.gnome.org/gtk3/stable/GtkGrid.html
    //printf("%d\n", numberNodes);
    columnP = gtk_grid_new();

    gtk_container_add(GTK_CONTAINER(genotypeTable), columnP);

    // memory for tableP
    tableP = calloc(number,sizeof(GtkWidget*));
    for(int j = 0; j < number; j++) {
        tableP[j] = calloc(numbre2, sizeof(GtkWidget));
    }
    // -------------------------------------------------------
    // father genotypes
    for(int j = 0; j < number; j++) 
    {

        if (j == 0){
            // Create the first entry
            // Create a radio button with a GtkEntry widget
            radio1 = gtk_radio_button_new_with_label(NULL, listaPadres[j]);
            gtk_widget_set_tooltip_text(radio1, listaFenotipos[j]);

            g_signal_connect (G_OBJECT (radio1), "clicked", G_CALLBACK (fatherCallback), NULL); 
            gtk_grid_attach (GTK_GRID(columnP), radio1, 0, 0, 1, 1);
        }
        if (j != 0){
            // Create a radio button with a label
            radio2 = gtk_radio_button_new_with_label_from_widget(GTK_RADIO_BUTTON(radio1), listaPadres[j]);
            gtk_widget_set_tooltip_text(radio2, listaFenotipos[j]);
            g_signal_connect (G_OBJECT (radio2), "clicked", G_CALLBACK (fatherCallback), NULL); 
            tableP[j][0] = radio2;
            gtk_grid_attach (GTK_GRID(columnP), tableP[j][0], 0, j, 1, 1);
        }
    }


    // -------------------------------------------------------
    // mother genotypes

    // temp pointers for the radios
    // the first radio that creates the group, the rest of the radios
    GtkWidget *radioM1, *radioM2;
    // the table containing the radios
    GtkWidget     ***tableP2;
    // the grid
    GtkWidget 		*columnP2;
    columnP2 = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(genotypeTable1), columnP2);
    // memory
    tableP2 = calloc(number,sizeof(GtkWidget*));
    for(int j = 0; j < number; j++) {
        tableP2[j] = calloc(numbre2, sizeof(GtkWidget));
    }

    for(int j = 0; j < number; j++) 
    {

        if (j == 0){
            // Create the first entry
            // Create a radio button with a GtkEntry widget
            radioM1 = gtk_radio_button_new_with_label(NULL, listaPadres[j]);
            gtk_widget_set_tooltip_text(radioM1, listaFenotipos[j]);
            gtk_grid_attach (GTK_GRID(columnP2), radioM1, 0, 0, 1, 1);
            g_signal_connect (G_OBJECT (radioM1), "clicked", G_CALLBACK (motherCallback), NULL); 
        }
        if (j != 0){
            // Create a radio button with a label
            //radioM2 = gtk_radio_button_new_with_label_from_widget(GTK_RADIO_BUTTON(radioM1), "CCCbbAA");
            radioM2 = gtk_radio_button_new_with_label(gtk_radio_button_get_group(radioM1), listaPadres[j]);
            gtk_widget_set_tooltip_text(radioM2, listaFenotipos[j]);
            g_signal_connect (G_OBJECT (radioM2), "clicked", G_CALLBACK (motherCallback), NULL); 
            tableP2[j][0] = radioM2;
            gtk_grid_attach (GTK_GRID(columnP2), tableP2[j][0], 0, j, 1, 1);
        }
    }
    gtk_widget_show_all(windowGenotipos);
}

void createDescendencia()
{

    GdkRGBA background;
    GtkWidget 		*columnDes;
    GtkWidget     ***tableDes;
    int tableSize = 10;
	// numberNodes = nodes;
	//printf("%d\n", tableSize);
	columnDes = gtk_grid_new ();


	tableDes = calloc(tableSize,sizeof(GtkWidget*));
	for(int j = 0; j < tableSize; j++) {
		tableDes[j] = calloc(tableSize,sizeof(GtkWidget));
	}

	gtk_container_add(GTK_CONTAINER(offspringTable), columnDes);

	for(int i = 0; i < tableSize; i++)
	{
		for(int j = 0; j < tableSize; j++)
		{	
			tableDes[i][j] = gtk_entry_new();
			gtk_entry_set_width_chars(GTK_ENTRY(tableDes[i][j]),8);
			gtk_grid_attach (GTK_GRID(columnDes),tableDes[i][j], j, i, 1, 1);

            // cambiar color de input
            // https://developer.gnome.org/gtk3/stable/GtkWidget.html#gtk-widget-override-background-color
            // https://developer.gnome.org/gtk3/stable/GtkWidget.html#GtkStateType
            // https://developer.gnome.org/gdk3/stable/gdk3-RGBA-Colors.html#GdkRGBA
            gdk_rgba_parse (&background, "#93CCC6");
            gtk_widget_override_background_color(tableDes[i][j], GTK_STATE_NORMAL, &background);

			if(i == 0 || j == 0){
				gtk_entry_set_text (GTK_ENTRY(tableDes[i][j]),"AB");
			}

			if(i != 0 && j != 0){
				// Descripción caracteristicas dominantes -> table[1...n][1]
				gtk_entry_set_text (GTK_ENTRY(tableDes[i][j]),"AABb");
			}
		}
	}

	gtk_widget_show_all(windowDescendencia); 
}



// this is the callback that is called everytime dominant entries changes

// static void enter_callback( GtkWidget *widget, GtkWidget *entry )
// {
//   const gchar *entry_text;
//   entry_text = gtk_entry_get_text (GTK_ENTRY (entry));
//   printf ("Entry contents: %s\n", entry_text);
	
// }

//solo permitir mayusculas 
gboolean on_key_press_m(GtkWidget *widget, GdkEventKey *evento, gpointer user_data)
{

	if ((evento->keyval >= GDK_KEY_A) && (evento->keyval <= GDK_KEY_Z)){
        return FALSE;	
    }
    if (evento->keyval == GDK_KEY_Right){
        return FALSE;
    }
    if (evento->keyval == GDK_KEY_Left){
        return FALSE;
    }
    if (evento->keyval == GDK_KEY_BackSpace) {
        return FALSE;
    }
    if (evento->keyval == GDK_KEY_space) {
        return FALSE;
    }
    else{
        return TRUE; 
    }
}

void createTableD0 (int nodes)
{
	numberNodes = nodes;
	//printf("%d\n", numberNodes);
	columnD0 = gtk_grid_new();

    // we get memory for the tableD0
	tableD0 = calloc(nodes, sizeof(GtkEntry*));
	for(int j = 0; j < nodes; j++) {
		tableD0[j] = calloc(nodes,sizeof(GtkEntry));
	}

	gtk_container_add(GTK_CONTAINER(scrolledTable), columnD0);
	for(int i = 0; i < nodes; i++)
	{
		for(int j = 0; j < 4; j++)
		{	

			tableD0[i][j] = gtk_entry_new();
			gtk_entry_set_width_chars(GTK_ENTRY(tableD0[i][j]),8);
			gtk_grid_attach (GTK_GRID(columnD0),tableD0[i][j], j, i, 1, 1);

			if (i != 0 && j == 0){
				//Caracteristicas dominantes -> table[1...n][2]
				gtk_entry_set_text (GTK_ENTRY(tableD0[i][j]),"C");
				g_signal_connect (G_OBJECT (tableD0[i][j]), "key_press_event", G_CALLBACK (on_key_press_m), NULL);
				//g_signal_connect (GTK_ENTRY(tableD0[i][j]), "activate", G_CALLBACK (enter_callback), GTK_ENTRY(tableD0[i][j]));
				gtk_entry_set_max_length (GTK_ENTRY(tableD0[i][j]), 1);
			}

			if(i == 0 && j == 0){
				gtk_entry_set_text (GTK_ENTRY(tableD0[i][j]),"C");
				gtk_widget_set_sensitive(tableD0[i][j],FALSE);
			}

			if(i == 0 && j == 1){
				// Descripción caracteristicas dominantes -> table[1...n][1]
				gtk_entry_set_text (GTK_ENTRY(tableD0[i][j]),"descrip.");
				gtk_widget_set_sensitive(tableD0[i][j],FALSE);
			}

			if(i == 0 && j == 2){
				gtk_entry_set_text (GTK_ENTRY(tableD0[i][j]),"C");
				gtk_widget_set_sensitive(tableD0[i][j],FALSE);
			}

			if(i == 0 && j == 3){
				// Descripción caracteristicas recesivas -> table[1...n][3]

				gtk_entry_set_text (GTK_ENTRY(tableD0[i][j]),"descrip.");
				gtk_widget_set_sensitive(tableD0[i][j],FALSE);
			}

			// if(i == 0 && j != 0){
			// 	//gtk_entry_set_text (GTK_ENTRY(tableD0[i][j]),"0");
			// 	gtk_widget_set_sensitive(tableD0[i][j],FALSE);
			// }
			if (i != 0 && j == 2){
				//Caracteristicas recesivas -> table[1...n][2]
				gtk_entry_set_text (GTK_ENTRY(tableD0[i][j]),"-");
				gtk_widget_set_sensitive(tableD0[i][j],FALSE);
			}

		}
	}

    printf("crash? %d\n", nodes);
	gtk_widget_show_all(windowCreateData); 
	// gtk_widget_show_now(windowCreateData);
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

//Funciones para validar que no hayan casillas vacias ni casillas repetidas - Falta terminar

int nhay_vacia(){
/*Verificar que hay posiciones vacias*/
	for (int row=0; row<numberNodes; row++){
   	 	for (int colum=0; colum<4; colum++){
        
            if (*gtk_entry_get_text(GTK_ENTRY(tableD0[row][colum]))=='\0')
            {
               return 0;
            }
        }
    }    
    return 1;
} 


int repeated_in_colum(const gchar* n)
/*Determina si n se repite en la columna*/
{
int count = 0;
const gchar* num = NULL;
    for (int row=1; row<numberNodes; row++){
        num = gtk_entry_get_text(GTK_ENTRY(tableD0[row][0]));
        
        if (*num==*n){
           count = count + 1;
           //printf("Repetidos: %s == %s \n",num,n);
           if (count>1){
              return 1;
           }
        }
    }
    return 0;
}

int valid_caract()
//Verifica si la columna es valida
{
    for (int cont=1; cont<numberNodes; cont++){
      if (repeated_in_colum(gtk_entry_get_text(GTK_ENTRY(tableD0[cont][0])))){
          return 0;
      }
    }
    return 1;
}

//-------------------------------------------------------------------------




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
    // free(tableD0);
    gtk_widget_hide(windowSave);
}

// ---------------------------
// Carga archivos de txt


int loadData(string nombreArchivo)
{
    contenidoArchivo archivoContenido;
    archivoContenido = leerArchivo(nombreArchivo);
    printf("%s\n",archivoContenido.caracteristicas[0]);
    printf("%s\n",archivoContenido.caracteristicas[1]);

}

void loadFile()
{
    char *nombreArchivo;
    nombreArchivo = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(chooseFileButton));
    printf("%s\n", nombreArchivo);
    loadData(nombreArchivo);
}
// ---------------------------
//Se devuelve a la pantalla inicial
void goBackSize()
{
    gtk_widget_hide(windowSelectSize); 
    gtk_widget_show_now(windowInitial);
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

void createMatrixFile()
{
    loadFile();
}

//Se devuelve a la pantalla inicial
void goBackFile()
{
    gtk_widget_hide(windowSelectFile); 
    gtk_widget_show_now(windowInitial);
}

// se usa para cargar la primera caja de caracteristicas
void createMatrix()
{
	gtk_widget_hide(windowSelectSize); 
	inputNumberNodes = gtk_spin_button_get_value_as_int (GTK_SPIN_BUTTON(spinButtonNode));	
	inputNumberNodes += 1; 
	createTableD0(inputNumberNodes);
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


void closeSave()
{
    gtk_widget_hide(windowSave); 
}

void drawGraph()
{
    saveTemp();
    system("./drawing &");
}

void closeInputError()
{
	gtk_widget_hide(windowInputError); 	
}

void closeRepeatError()
{
	gtk_widget_hide(windowRepeatError); 	
}

//------------------------------------------------------------------------------------------------------------------------------------
// Funciones para cruces mendelianos

contenidoArchivo guardarInfo(int n){

    // Para guardar los datos ingresados por el usuario
    contenidoArchivo contenido;

    printf("%d\n", n);

    if(nhay_vacia())
	{
		printf("Ya no hay casillas vacias.\n");

		if(valid_caract())
		{
			printf("Ya no hay casillas repetidas.\n");
            for(int i = 0; i <= n; i++)
            {
                for(int j = 0; j < 4; j++)
                {
                    //Caracteristicas dominantes -> table[1...n][2]
                    if (j == 0 && i!=0)
                    {
                        strcpy(contenido.caracteristicas[i-1], gtk_entry_get_text(GTK_ENTRY(tableD0[i][j])));
                        strcpy(contenido.caracteristicas[i],gtk_entry_get_text(GTK_ENTRY(tableD0[i][j])));
                    }
                    
                    // Descripción caracteristicas dominantes -> table[1...n][1]
                    if(j == 1 && i != 0)
                    {
                        strcpy(contenido.feno_dominantes[i-1], gtk_entry_get_text(GTK_ENTRY(tableD0[i][j])));
                        strcpy(contenido.feno_dominantes[i],gtk_entry_get_text(GTK_ENTRY(tableD0[i][j])));
                    }
                    
                    //Caracteristicas recesivas -> table[1...n][2]
                    if (j == 2 && i != 0)
                    {
                        //gtk_entry_get_text(GTK_ENTRY(tableD0[i][j])); 
                    }
                    
                    // Descripción caracteristicas recesivas -> table[1...n][3]
                    if (j == 3 && i != 0) 
                    {
                        strcpy(contenido.feno_recesivos[i-1], gtk_entry_get_text(GTK_ENTRY(tableD0[i][j])));
                        strcpy(contenido.feno_recesivos[i],gtk_entry_get_text(GTK_ENTRY(tableD0[i][j])));
                    
                    }
                }
            }

			int loop;
   			printf("Caracteristicas: \n");
   			for(loop = 0; loop < numberNodes; loop++)
  			{
   				gchar *c;	
   				int x = 0;

      			// printf("%s\n", caracteristicas[loop]);

      			c = gtk_entry_get_text(GTK_ENTRY(tableD0[loop][0]));
      			x = strlen(c);

      			c = g_utf8_strdown(c,x); 
      			printf("Minuscula: %s\n", c);

      			gtk_entry_set_text(GTK_ENTRY(tableD0[loop][2]), c);

   			}
		}
		else
		{
			printf("Error: hay caracteristicas repetidas.\n");
			gtk_widget_show_all(windowRepeatError); 
		}
	}
	else
	{
		printf("Error: hay casillas vacias.\n");
		gtk_widget_show_all(windowInputError); 
	}
			

//     //Imprimir datos para verificar 
//    int loop;
//    printf("Caracteristicas: \n");
//    for(loop = 0; loop < n; loop++)
//       printf("%s\n", contenido.caracteristicas[loop]);

//    int loop1;
//    printf("Fenotipos Dominantes: \n");
//    for(loop1 = 0; loop1 < n; loop1++)
//       printf("%s\n", contenido.feno_dominantes[loop1]);

//    int loop2;
//    printf("Fenotipos Recesivos: \n");
//    for(loop2 = 0; loop2 < n; loop2++)
//       printf("%s\n", contenido.feno_recesivos[loop2]);

  return contenido;

}

void openPercentage()
{
    gtk_widget_show_all(windowPercentage);
}



void openGenotipos()
{
    // padresMain();
    // contenidoArchivo temp;
    // temp = leerArchivo("../aTemp.txt");
    // printf("Caracteristica: %s",temp.caracteristicas[0]);
    cantidadCaracteristicas = gtk_spin_button_get_value_as_int (GTK_SPIN_BUTTON(spinButtonNode)); 
    printf("%d\n",cantidadCaracteristicas);
    contenidoArchivo info = guardarInfo(cantidadCaracteristicas);
    // printf("Padres: %s|", info.caracteristicas[0]);
    // printf("Padres: %s|", info.caracteristicas[2]);
    // printf("Padres: %s|", info.caracteristicas[1]);
    creaPadres(info.caracteristicas,cantidadCaracteristicas);
    creaFenotipos(listaPadres,cantidadCaracteristicas,info.caracteristicas,info.feno_dominantes,info.feno_recesivos);
    createGenotipos(cantidadCaracteristicas);
}

void openDescendencia(){
    createDescendencia();
    return;
}



//------------------------------------------------------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------------------------------------------------------
/*FUNCION PARA PERMITIR SOLO LETRAS EN ENTRADAS*/

gboolean on_key_press (GtkWidget *widget, GdkEventKey *evento, gpointer user_data)
//Permite solo numeros en entradas, restringiendo los keypress
{
    if ((evento->keyval >= GDK_KEY_A) && (evento->keyval <= GDK_KEY_Z)){
        return FALSE;	
    }

    if ((evento->keyval >= GDK_KEY_a) && (evento->keyval <= GDK_KEY_z)){
        return FALSE;
    }
    if (evento->keyval == GDK_KEY_Right){
        return FALSE;
    }
    
    if (evento->keyval == GDK_KEY_Left){
        return FALSE;
    }
    
    if (evento->keyval == GDK_KEY_BackSpace) {
        return FALSE;
    }
    if (evento->keyval == GDK_KEY_space) {
        return FALSE;
    }
    else{
        return TRUE; 
    }
}



int main(int argc, char *argv[])
{
    gtk_init(&argc, &argv);

    myBuilder = gtk_builder_new();
    gtk_builder_add_from_file (myBuilder, "glade/window_floyd.glade", NULL);
    //g_signal_connect (G_OBJECT (window), "key_press_event", G_CALLBACK (on_key_press), NULL);   
    windowInitial = GTK_WIDGET(gtk_builder_get_object(myBuilder, "window_floyd_start"));
    windowCreateData = GTK_WIDGET(gtk_builder_get_object(myBuilder, "window_floyd"));
    g_signal_connect (G_OBJECT (windowCreateData), "key_press_event", G_CALLBACK (on_key_press), NULL);   
    windowInputError = GTK_WIDGET(gtk_builder_get_object(myBuilder, "window_input_error"));
    windowRepeatError = GTK_WIDGET(gtk_builder_get_object(myBuilder, "window_repeat_error"));


    // ---------------------
    // windows for biologia
    windowGenotipos = GTK_WIDGET(gtk_builder_get_object(myBuilder, "window_genotipos"));
    windowDescendencia = GTK_WIDGET(gtk_builder_get_object(myBuilder, "window_descendencia"));
    // ---------------------

    windowSelectSize = GTK_WIDGET(gtk_builder_get_object(myBuilder, "window_floyd_selectSize"));
    windowSelectFile = GTK_WIDGET(gtk_builder_get_object(myBuilder, "window_floyd_selectFile"));

    windowSave = GTK_WIDGET(gtk_builder_get_object(myBuilder, "window_floyd_saved"));
    spinButtonNode = GTK_WIDGET(gtk_builder_get_object(myBuilder, "ent_size"));

    gtk_spin_button_set_range (GTK_SPIN_BUTTON(spinButtonNode),1,6);
    gtk_spin_button_set_increments (GTK_SPIN_BUTTON(spinButtonNode),1,3);
    scrolledTable = GTK_WIDGET(gtk_builder_get_object(myBuilder, "scrolled_table"));
    // ----------------------
    // tables for biologia
    genotypeTable = GTK_WIDGET(gtk_builder_get_object(myBuilder, "genotype_table"));
    genotypeTable1 = GTK_WIDGET(gtk_builder_get_object(myBuilder, "genotype_table1"));
    // table for descendencia
    offspringTable = GTK_WIDGET(gtk_builder_get_object(myBuilder, "offspring_table"));
    // ----------------------
    chooseFileButton = GTK_WIDGET(gtk_builder_get_object(myBuilder, "btn_fileChooser"));
    filenameEntry = GTK_WIDGET(gtk_builder_get_object(myBuilder, "ent_fileName"));

    // Window de porcentajes
    windowPercentage = GTK_WIDGET(gtk_builder_get_object(myBuilder, "window_percentage"));


    // label para el padre genotipo
	labelGenotipoPadre = GTK_WIDGET(gtk_builder_get_object(myBuilder, "lbl_genotipo_padre"));
	labelGenotipoMadre = GTK_WIDGET(gtk_builder_get_object(myBuilder, "lbl_genotipo_madre"));
	labelFenotipoPadre = GTK_WIDGET(gtk_builder_get_object(myBuilder, "lbl_fenotipo_padre"));
	labelFenotipoMadre = GTK_WIDGET(gtk_builder_get_object(myBuilder, "lbl_fenotipo_madre"));

    gtk_builder_connect_signals(myBuilder, NULL);
    g_object_unref(myBuilder);
    gtk_widget_show_all(windowInitial);                
    gtk_main();
    return 0;
}

