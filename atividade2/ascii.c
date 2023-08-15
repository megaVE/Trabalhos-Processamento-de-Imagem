/*-------------------------------------------------------------
 *          UNIFAL - Universidade Federal de Alfenas.
 *           BACHARELADO EM CIENCIA DA COMPUTACAO.
 *  Trabalho: Imagem ASCII
 *  Disciplina: Processamento de Imagens
 *  Professor: Luiz Eduardo da Silva
 *  Aluno: Vinicius Eduardo de Souza Honorio
 *  Data: 01/05/2023
*-------------------------------------------------------------*/

/*-------------------------------------------------------------------------
 * Image Processing using C-Ansi
 *   Base Program: Negative from grayscale image
 * By Luiz Eduardo da Silva.
 *   Modified Program: Ascii from .pgm image
 * By Vinicius Eduardo de Souza Honorio
 *-------------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "imagelib.h"

/* Horizontal Resizing Funcion */
/* - reduces the width of the Input Image to the desired value - */
void resize_x(image In, image Out, int nl, int nc, int max_x)
{
    int dsc = nc / max_x;
    int dsp = nc % max_x;
    for(int i = 0; i < nl; i++){
        int j = 0;
        int k = 0;
        while(j < nc){
            int aux = 0;
            int pixel = 0;
            int cdsp = 0;
            while(aux < dsc){
                pixel = pixel + In[i * nc + j];
                j++;
                aux++;
            }
            if(cdsp < dsp){
                pixel = pixel + In[i * nc + j];
                j++;
                aux++;
                cdsp++;
            }
            Out[i * max_x + k] = pixel / aux;
            k++;
        }
    }
}

/* Vertical Resizing Function */
/* - reduces the height of the Input Image to the desired value - */
void resize_y(image In, image Out, int nl, int nc, int max_y)
{
    int dsc = nl / max_y;
    int dsp = nl % max_y;
    for(int i = 0; i < nc; i++){
        int j = 0;
        int k = 0;
        while(j < nl){
            int aux = 0;
            int pixel = 0;
            int cdsp = 0;
            while(aux < dsc){
                pixel = pixel + In[j * nc + i];
                j++;
                aux++;
            }
            if(cdsp < dsp){
                pixel = pixel + In[j * nc + i];
                j++;
                aux++;
                cdsp++;
            }
            Out[k * nc + i] = pixel / aux;
            k++;
        }
    }
}

/* Explanatory Message */
/* - appears when the program is called with more/less than the required arguments - */
void msg(char *s)
{
    printf("\nAscii image");
    printf("\n-------------------------------");
    printf("\nUsage:  %s  image-name[.pgm] nc nr \"string\"\n\n", s);
    printf("    image-name[.pnm] is image file name \n");
    printf("    nc = number of columns\n");
    printf("    nr = number of rows\n");
    printf("    string = character list from which the image will be built\n\n");
    exit(1);
}

/*-------------------------------------------------------------------------
 * main function
 *-------------------------------------------------------------------------*/
int main(int argc, char *argv[])
{
    FILE *OutFile;
    int nc, nr, ml, max_x, max_y, ns;
    float ps;
    char *p, nameIn[100], nameOut[100], cmd[110];
    image In, Mid, Out;

    if (argc != 5)
        msg(argv[0]);

    max_x = atoi(argv[2]);
    max_y = atoi(argv[3]);

    //-- define input/output file name
    img_name(argv[1], nameIn, nameOut, 2);

    //-- read image
    In = img_get(nameIn, &nr, &nc, &ml, 2);
    
    ns = strlen(argv[4]);
    ps = (float) ml/ns;

    //-- returns error if the output required is larger than the inserted
    if(max_x > nc || max_y > nr){
        printf("Output Image Size Invalid!\n");
        printf("Must be equal or lower to the Input Image\n");
        exit(2);
    }

    //-- prints input info
    printf("Output Image X-size: %d [from %d]\n", max_x, nc);
    printf("Output Image Y-size: %d [from %d]\n", max_y, nr);
    printf("Char List Length: %d\n", ns);
    printf("Char List: %s\n", argv[4]);
    printf("Space Proportion: %f\n\n", ps);

    //-- execute resizing operations (if necessary)
    if(max_x < nc && max_y < nr){
        printf("Executing resize_x and resize_y...\n");
        Mid = img_alloc(nr, max_x);
        resize_x(In, Mid, nr, nc, max_x);
        Out = img_alloc(max_y, max_x);
        resize_y(Mid, Out, nr, max_x, max_y);
        img_free(Mid);
    }
    else{
        if(max_x < nc){
            printf("Executing resize_x...\n");
            Out = img_alloc(nr, max_x);
            resize_x(In, Out, nr, nc, max_x);
        }
        if(max_y < nr){
            printf("Executing resize_y...\n");
            Out = img_alloc(max_y, nc);
            resize_y(In, Out, nr, nc, max_y);
        }
        if(max_x == nc && max_y == nr){
            printf("No need of resizing...\n");
            Out = img_alloc(nr, nc);
            for(int i = 0; i < nr * nc; i++)
                Out[i] = In[i];
        }
    }
    printf("Output Image Complete!\n\n");

    //-- creates the output .txt file
    OutFile = fopen("result.txt", "w");
    //-- prints the ascii image to the file
    for(int i = 0; i < max_y; i++){
        for(int j = 0; j < max_x; j++){
            int aux = 0;
            while(aux < ns && aux != -1){
                if(Out[i * max_x + j] >= aux * ps && Out[i * max_x + j] <= (aux+1) * ps){
                    fprintf(OutFile, "%c", argv[4][aux/*ns - aux - 1*/]);
                    aux = -1;
                }
                else
                    aux++;
            }
            if(aux != -1)
                fprintf(OutFile, "%c", argv[4][ns - 1/*0*/]);
        }
        fprintf(OutFile, "\n");
    }

    //-- closes the output file
    fclose(OutFile);
    //-- frees the allocated memory space
    img_free(In);
    img_free(Out);
    printf("Everything OK!\n");
    return 0;
}
