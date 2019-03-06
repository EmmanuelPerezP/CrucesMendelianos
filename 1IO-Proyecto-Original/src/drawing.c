#define _GNU_SOURCE
#include <cairo.h>
#include <gtk/gtk.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stddef.h>
#include <assert.h>

FILE *infoFile;

int numberNodes = 0;
int mtrx[27][27];
char  * smtrx[27][27];
char  * names[27];


int cantidad = 5;

int matriz[10][10] = { {0,-1,1,3,4},
                  {1,0,5,6,7},
                  {2,4,0,2,3},
                  {3,6,1,0,33},
                  {4,8,1,2,0}};

int nodePos[27][2] = {};


void setPositions(){
  double tetha = 2*M_PI / numberNodes;

  for(int i = 0; i < numberNodes; i++){
    nodePos[i][0] = (int)(cos(tetha*i)*(200));
    nodePos[i][1] = (int)(sin(tetha*i)*(200));
  }
}

void nodeCircles(cairo_t *cr, GtkWidget *widget){
  cairo_stroke_preserve(cr);
  for (int i = 0; i < numberNodes; i++)
  {

    cairo_set_source_rgb(cr, 0, 0, 0);
    cairo_set_line_width(cr, 2);  
    cairo_arc(cr, nodePos[i][0], nodePos[i][1], 30*(1-(numberNodes/27))+10, 0, 2*M_PI);
    cairo_stroke_preserve(cr);
    cairo_set_source_rgb(cr, 0.3, 0.4, 0.6);
    cairo_fill(cr);  
  }
}

void nodeName(cairo_t *cr, GtkWidget *widget){
  char buff[100];
  for (int i = 0; i < numberNodes; i++)
  {
    strcpy(buff, names[i]);
    cairo_select_font_face(cr, "Serif", CAIRO_FONT_SLANT_NORMAL,
    CAIRO_FONT_WEIGHT_BOLD);
    cairo_set_font_size(cr, 15);

    cairo_set_source_rgb(cr, 0, 0, 0);
    cairo_move_to(cr, nodePos[i][0], nodePos[i][0]);  
    cairo_show_text(cr, buff);  
  
    cairo_set_source_rgb(cr, 0.5, 0.5, 0.5);
    cairo_move_to(cr, nodePos[i][0]+3, nodePos[i][0]+3);  
    cairo_show_text(cr, buff); 
  }
}

void drawShort(cairo_t *cr, GtkWidget *widget){
  for(int i = 0; i < numberNodes; i++){
    for(int j = 0; j < numberNodes; j++){
      if(mtrx[i][j] > 0){
        if(mtrx[i][j] < mtrx[j][i] || mtrx[j][i] == -1){
          cairo_set_source_rgb(cr, 0.7, 0.4, 0.7); 
          cairo_set_line_width(cr, 4);
          cairo_move_to(cr, nodePos[i][0], nodePos[i][1]);
          cairo_line_to(cr, nodePos[j][0], nodePos[j][1]);
        } else {
          int cx = (nodePos[i][0] + nodePos[j][0])/2;
          int cy = (nodePos[i][1] + nodePos[j][1])/2;
          cairo_move_to(cr, nodePos[i][0], nodePos[i][1]);
          cairo_curve_to(cr, cx+50, cy+50, cx+50, cy+50, nodePos[j][0], nodePos[j][1]);
        }
      }
    }
  }
}

static void do_drawing(cairo_t *, GtkWidget *);

static gboolean on_draw_event(GtkWidget *widget, cairo_t *cr, 
    gpointer user_data)
{  
  do_drawing(cr, widget);  

  return FALSE;
}



static void do_drawing(cairo_t *cr, GtkWidget *widget)
{
  GtkWidget *win = gtk_widget_get_toplevel(widget);

  int width, height;
  gtk_window_get_size(GTK_WINDOW(win), &width, &height);
   
  cairo_set_source_rgb(cr, 0.69, 0, 0);
  
  cairo_translate(cr, width/2, height/2);

  setPositions();
  nodeCircles(cr, win);
  drawShort(cr, win);
  //nodeName(cr,win);

  cairo_stroke_preserve(cr);


  //cairo_arc(cr, 0, 0, 50, 0, 0.5 * M_PI);
  cairo_stroke_preserve(cr);
}


int main (int argc, char *argv[])
{
  GtkWidget *window;
  GtkWidget *darea;

  loadData();
  readLines();

  gtk_init(&argc, &argv);

  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

  darea = gtk_drawing_area_new();
  gtk_container_add(GTK_CONTAINER(window), darea);

  g_signal_connect(G_OBJECT(darea), "draw", 
      G_CALLBACK(on_draw_event), NULL);
  g_signal_connect(G_OBJECT(window), "destroy",
      G_CALLBACK(gtk_main_quit), NULL);

  gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
  gtk_window_set_default_size(GTK_WINDOW(window), 800, 600); 
  gtk_window_set_title(GTK_WINDOW(window), "Fill & stroke");

  gtk_widget_show_all(window);

  gtk_main();

  return 0;
}

int loadData(){
  infoFile = fopen("temp/temp.txt", "r");
  int ch;
  if(infoFile != NULL){
    while(feof(infoFile) == 0) {  
      ch = fgetc(infoFile);
      if (ch == '\n'){
        break;
      }
      if (ch ==';'){
        numberNodes ++;
      }
    }
    numberNodes--;
    fclose(infoFile);
    return 1;
  }
  return 0;
}

void readLines(){

    FILE * fp = fopen("temp/temp.txt", "r");
    const char delimiters[] = ";";
    char * line= NULL;
    size_t len = 0;
    ssize_t read;

    if (fp == NULL) exit(EXIT_FAILURE);

    int i = 0;

    if((read = getline(&line, &len, fp)) != -1){
      char * running2 = strdupa(line);
      strsep(&running2, delimiters);
      for(int j = 0; j < numberNodes;j++){
          names[j] = strsep(&running2, delimiters);
      }
      while((read = getline(&line, &len, fp)) != -1){
        char * running = strdupa(line);
        strsep(&running, delimiters);
        for(int j = 0; j < numberNodes;j++){
          smtrx[i][j] = strsep(&running, delimiters);
        }
        i++;
      }
    }

    for (i = 0; i < numberNodes; i++){
      printf("%s ", names[i]);
    }
    printf("\n");

    for (i = 0; i < numberNodes; i++){
      for (int j = 0; j < numberNodes; j++){
        if(strcmp(smtrx[i][j],"INF") == 0){
          mtrx[i][j] = -1;
        } else {
          mtrx[i][j] = atoi(smtrx[i][j]);
        }
        printf("%i ", mtrx[i][j]);
      }
      printf("\n");
    }

    for (i = 0; i < numberNodes; i++){
      for (int j = 0; j < numberNodes; j++){
        printf("%s ", smtrx[i][j]);
      }
      printf("\n");
    }

    fclose(fp);
    if (line)
        free(line);
}

char** string_split(char* a_str, const char a_delim)
{
    char** result    = 0;
    size_t count     = 0;
    char* tmp        = a_str;
    char* last_comma = 0;
    char delim[2];
    delim[0] = a_delim;
    delim[1] = 0;

    /* Count how many elements will be extracted. */
    while (*tmp)
    {
        if (a_delim == *tmp)
        {
            count++;
            last_comma = tmp;
        }
        tmp++;
    }

    /* Add space for trailing token. */
    count += last_comma < (a_str + strlen(a_str) - 1);

    /* Add space for terminating null string so caller
       knows where the list of returned strings ends. */
    count++;

    result = malloc(sizeof(char*) * count);

    if (result)
    {
        size_t idx  = 0;
        char* token = strtok(a_str, delim);

        while (token)
        {
            assert(idx < count);
            *(result + idx++) = strdup(token);
            token = strtok(0, delim);
        }
        assert(idx == count - 1);
        *(result + idx) = 0;
    }

    return result;
}
