//como o tamanho de cada registro é fixo e conhecido (17 bytes), uso a função fread para ler exatamente sizeof(REGISTRO) bytes de uma vez. C já vai preencher a struct pra mim

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct {
    int NroUSP;   // 4 bytes (int padrão)
    int curso;    // 4 bytes
    int estado;   // 4 bytes
    int idade;    // 4 bytes
    bool valido;  // 1 byte
} REGISTRO;       // total: 17 bytes 


//função para eliminar os registros inválidos (valido == false) dado um vetor dinâmico de REGISTROS
REGISTRO *pegaValidos( REGISTRO *registros, int totalRegistros ){

    REGISTRO *registrosValidos = (REGISTRO*)malloc(sizeof(REGISTRO) * totalRegistros );

    if( !registrosValidos ){
        fprintf( stderr, "Nao ha espaco suficiente para alocar o vetor registrosValidos" );
        exit(-1);
    }

    for( int i = 0 ; i < totalRegistros ; i++ ){
        if( registros[i].valido == false ){ 
            registrosValidos[i] = registros[i];
        }
    }

    return registrosValidos; 
}


int main( int argc, char *argv[] ){

    if( argc != 3 ){
        fprintf( stderr, "O programa exige 3 argumentos. Por favor, tente novamente");
        exit(-1);
    }

    FILE *arquivoEntrada = fopen( argv[1] , "rb" ); //rb = read binary
    FILE *arquivoSaida = fopen( argv[2], "wb" ); //wb = write binary

    if( !arquivoEntrada || !arquivoSaida ){
        fprintf( stderr, "Nao foi possivel abrir um dos arquivos" );
        exit(-1);
    }

    /** descubro quantos REGISTROS tem no arquivo, movo o cursor pro final do arquivo
     * arquivoEntrada
     * 0 -> mova-se 0 bytes a partir de um ponto de referência
     * SEEK_END -> const que define o ponto de referência como final do arquivo
    **/
    fseek( arquivoEntrada, 0, SEEK_END );

    /** 
     * ftell -> me diz que posição (em bytes) do arquivo o cursor está agora. Ele foi pro final com o fseek() acima, então tem que dar o tamanho total do arquivo
     */
    int totalRegistros = ftell(arquivoEntrada) / sizeof(REGISTRO);

    //volta o cursor pro início do arquivo de novo
    rewind( arquivoEntrada ); 

    REGISTRO *registros = (REGISTRO*)malloc(sizeof(REGISTRO) * totalRegistros); 

    /** 
     * (ondeVouColocarOqLi, tamanhoDeCadaStructREGISTRO, quantasStructsREGISTRO, arquivo)
     * fread() lê totalRegistros blocos de sizeof(REGISTRO) bytes cada e joga no array registros
     */
    fread(registros, sizeof(REGISTRO), totalRegistros, arquivoEntrada);
    fclose(arquivoEntrada);

    //(origem, tamanhoDeCadaStructREGISTRO, quantasStructsREGISTRO, arquivo)
    //pego as infos da origem e escrevo no arquivoSaida
    fwrite( pegaValidos(registros, totalRegistros), sizeof(REGISTRO), totalRegistros, arquivoSaida);

    fclose(arquivoSaida);
    free(registros);

    return 0;
}