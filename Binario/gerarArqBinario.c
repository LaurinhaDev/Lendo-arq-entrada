#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct {
    int NroUSP;
    int curso;
    int estado;
    int idade;
    bool valido;
} REGISTRO;

void gerarArquivo( char *entrada ) {

    FILE *arquivoEntrada = fopen(entrada, "wb"); //wb = write binary

    if ( !arquivoEntrada ){
        fprintf( stderr, "Nao foi possivel abrir arquivoEntrada" );
        exit(-1);
    }

    REGISTRO r1 = {
        12345678, 
        1, 
        25, 
        18, 
        true
    };

    REGISTRO r2 = {
        98765432, 
        3, 
        14, 
        25, 
        false
    };

    REGISTRO r3 = {
        11111111, 
        2, 
        8, 
        22, 
        true
    };

    fwrite(&r1, sizeof(REGISTRO), 1, arquivoEntrada);
    fwrite(&r2, sizeof(REGISTRO), 1, arquivoEntrada);
    fwrite(&r3, sizeof(REGISTRO), 1, arquivoEntrada);

    fclose(arquivoEntrada);
}

int main( int argc, char *argv[] ){

    if( argc != 2 ){
        fprintf( stderr, "O programa exige mais arquivos" );
        exit(-1);
    }

    char *arquivoEntrada = argv[1];

    gerarArquivo( arquivoEntrada );

    return 0;
}