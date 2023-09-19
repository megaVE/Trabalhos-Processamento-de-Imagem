/*-------------------------------------------------------------
*          UNIFAL - Universidade Federal de Alfenas.
*            BACHARELADO EM CIENCIA DA COMPUTACAO.
* Trabalho..: Esteganografia
* Disciplina: Processamento de Imagens
* Professor.: Luiz Eduardo da Silva
* Aluno.....: Vinícius Eduardo de Souza Honório
* Data......: 22/06/2023
*-------------------------------------------------------------*/
/*---------------------------------------------------------
 * Image Processing
 * 
 * Steganography - decode
 *---------------------------------------------------------*/


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "imagelib.h"
#define DEBUG(x) x

/* Defines the current color */
void changeColor(int *color){
    switch(*color){
        case 0:
            *color = 1;
            break;
        case 1:
            *color = 2;
            break;
        case 2:
            *color = 0;
            break;
    }
    return;
}

/* Extracts the current color from the int value */
int extractColor(int color, int value){
    int r, g, b;
    r = value >> 16;
    g = (value >> 8) - (r << 8);
    b = value - (r << 16) - (g << 8);
    switch(color){
        case 0:
            value = r;
            break;
        case 1:
            value = g; 
            break;
        case 2:
            value = b;
            break;
    }
    return value;
}

/* Uncovers a hidden byte from the image file */
unsigned char decodeByte(image In, int index, int color){
    unsigned char byte;
    //printf("Calculando Byte...\n");
    for(int i = 0; i < 8; i++, index++){
        byte = byte << 1;
        if(extractColor(color, In->px[index]) % 2 == 1){
            byte++;
            //printf("Bit Obtido: 1\n");
        }
        else
            ;
            //printf("Bit Obtido: 0\n");
        changeColor(&color);
    }
    return byte;
}

/* Set the iterator values towards */
void proceed(int *index, int *color){
    changeColor(color);
    changeColor(color);
    *index+=8;
    //printf("\nColor: %d | Index: %d\n\n", *color, *index);
    return;
}

void decode(image In)
{
    FILE *file;
    char name[100];
    int fsize;
    char byte;

    int index = 0;
    int color = 0;

    /*
    printf("Decodificacao Iniciada!\n");
    printf("-----------------------\n");
    printf("Linhas: %d\n", In->nr);
    printf("Colunas: %d\n", In->nc);
    printf("Tonalidade: %d\n", In->ml);
    printf("Imagem:\n");
    for(int i = 0; i < In->nr; i++){
        for(int j = 0; j < In->nc; j++){
            r = In->px[i * In->nc + j] >> 16;
            g = (In->px[i * In->nc + j] >> 8) - (r << 8);
            b = In->px[i * In->nc + j] - (r << 16) - (g << 8);
            printf("%d %d %d ", r, g, b);
        }
        printf("\n");
    }
    printf("-----------------------\n");
    */

    // decode the name

    do{
        byte = decodeByte(In, index, color);
        proceed(&index, &color);
        //printf("Byte obtido (nome): %c [%d]\n", byte, byte);
        if(byte != 0)
            name[index/8-1] = byte;
    } while (byte != 0);
    name[index/8-1] = '\0';
    
    printf("\nFile name: %s\n\n", name);
    
    // decode file size

    fsize = 0;
    for(int i = 0; i < 4; i++){
        fsize = fsize << 8;
        byte = decodeByte(In, index, color);
        proceed(&index, &color);
        //printf("Byte obtido (tamanho): %c [%d]\n", byte, byte);
        fsize += byte;
    }
    
    printf("\nFile size: %d bytes\n\n", fsize);
    
    // decode file
    
    file = fopen(name, "wb");
    if (!file)
    {
        printf("Cannot create file %s\n", name);
        exit(10);
    }
    while (fsize)
    {
        // decode the bytes of the file
        byte = decodeByte(In, index, color);
        proceed(&index, &color);
        //printf("Byte obtido (arquivo): %c [%d]\n", byte, byte);
        fprintf(file, "%c", byte);
    
        fsize--;
    }
    fclose(file);
}

/* Mensagem Inicial */
void msg(char *s)
{
    printf("\nSteganography - decode");
    printf("\n-------------------------------");
    printf("\nUSE.:  %s  <IMG.PPM>", s);
    printf("\nWhere:\n");
    printf("    <IMG.PPM>  Image file in PPM ASCII format\n\n");
    exit(20);
}

/*-----------------------------------------
  Main program
------------------------------------------*/
int main(int argc, char *argv[])
{
    char name[100];
    image In;
    char *p;
    if (argc < 2)
        msg(argv[0]);
    strcpy(name, argv[1]);
    p = strstr(name, ".ppm");
    if (p)
        *p = 0;
    strcat(name, ".ppm");
    In = img_get(name, COLOR);
    decode(In);
    img_free(In);
    printf("Ok\n");
    return 0;
}
