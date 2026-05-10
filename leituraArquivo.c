/* Como rodar:
1) gcc leituraArquivo.c -o leituraArquivo
2) ./leituraArquivo.exe "entrada.txt" "saida.txt"
*/

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

typedef struct
{
    int nroUsp; // chave primária
    int curso;
    int estado;
    int cidade;
    int valido; // para exclusão lógica (seria um "bool", 0 = false, 1 = true)
} REGISTRO;


//função para passar de um arq1.txt para um arq2.txt somente os registros válidos (sem salvar na memória)
void passarValidos(char *entrada, char *saida)
{

    FILE *arquivoEntrada = fopen(entrada, "r");
    FILE *arquivoSaida = fopen(saida, "w");

    if (!arquivoEntrada || !arquivoSaida)
    {
        fprintf(stderr, "Não foi possível abrir o arquivo de entrada/saida na função passarValidos");
        exit(-1);
    }

    int nroUsp, curso, estado, cidade, valido;
    while (fscanf(arquivoEntrada, "%d %d %d %d %d", &nroUsp, &curso, &estado, &cidade, &valido) == 5)
    {
        if (valido == 1) //ou seja, se é valido
        {
            fprintf(arquivoSaida, "%d %d %d %d true\n", nroUsp, curso, estado, cidade);
        }
    }
    fclose(arquivoEntrada);
    fclose(arquivoSaida);
}

// função para inserir os registrs num arq2 na odem inversa da do arq1
void inverterRegistros(char *entrada, char *saida)
{

    FILE *arquivoEntrada = fopen(entrada, "r");
    FILE *arquivoSaida = fopen(saida, "w");
    if (!arquivoEntrada || !arquivoSaida)
    {
        fprintf(stderr, "Não foi possível abrir o arquivo de entrada/saida na função passarValidos");
        exit(-1);
    }

    int capacidade = 20;
    int totalRegistros = 0; // nro atual de registros

    REGISTRO *registros = (REGISTRO *)malloc(sizeof(REGISTRO) * capacidade); // array para os registros

    int nroUsp, curso, estado, cidade, valido;

    while (fscanf(arquivoEntrada, "%d %d %d %d %d", &nroUsp, &curso, &estado, &cidade, &valido) == 5)
    {

        if (totalRegistros >= capacidade) //vou ter que aumentar o array
        {
            capacidade *= 2; // dobro a capacidade (mais seguro)
            REGISTRO *registrosAux = (REGISTRO *)realloc(registros, sizeof(REGISTRO) * capacidade);

            if (!registrosAux)
            {
                fprintf(stderr, "Não há espaço suficiente para alocar um vetor auxiliar ");
                fclose(arquivoEntrada);
                fclose(arquivoSaida);
                exit(-1);
            }
            registros = registrosAux;
        }
        registros[totalRegistros].nroUsp = nroUsp;
        registros[totalRegistros].curso = curso;
        registros[totalRegistros].estado = estado;
        registros[totalRegistros].cidade = cidade;
        registros[totalRegistros].valido = valido;

        totalRegistros++;
    }

    for (int i = totalRegistros - 1; i > -1; i--) //não vou até o final do array, só até onde tem algo
    {
        fprintf(arquivoSaida, "%d %d %d %d %d\n", registros[i].nroUsp, registros[i].curso, registros[i].estado, registros[i].cidade, registros[i].valido);
    }

    free(registros);
    fclose(arquivoEntrada);
    fclose(arquivoSaida);
}

// função para inserir um novo registro num arq2 (não pode ter 2 números usp iguais)
void inserirNovoRegistro(char *entrada, char *saida, int numUsp, int numCurso, int numEstado, int numCidade, int numValido)
{

    FILE *arquivoEntrada = fopen(entrada, "r");
    FILE *arquivoSaida = fopen(saida, "w");

    if (!arquivoEntrada || !arquivoSaida)
    {
        fprintf(stderr, "Houve um erro de alocação na função inserirNovoRegistro");
        exit(-1);
    }

    int nroUsp, curso, estado, cidade, valido;
    while (fscanf(arquivoEntrada, "%d %d %d %d %d", &nroUsp, &curso, &estado, &cidade, &valido) == 5)
    {
        if (nroUsp == numUsp)
        {
            fprintf(stderr, "O numero USP %d ja existe e nao pode ser inserido novamente", nroUsp);
            fclose(arquivoEntrada);
            fclose(arquivoSaida);
            exit(-1);
        }
        else
        {
            fprintf(arquivoSaida, "%d %d %d %d %d\n", nroUsp, curso, estado, cidade, valido);
        }
    }
    fprintf(arquivoSaida, "%d %d %d %d %d\n", numUsp, numCurso, numEstado, numCidade, numValido);

    fclose(arquivoEntrada);
    fclose(arquivoSaida);
}


//função auxiliar para a função retornaRegistro
void printarRegistro(char *saida, REGISTRO *registro){

    FILE* arquivoSaida = fopen( saida, "w" );

    fprintf( arquivoSaida, "%d %d %d %d %d", registro->nroUsp, registro->curso, registro->estado, registro->cidade, registro->valido );

    fclose(arquivoSaida);
    free(registro);
}

// função que, dada um nro usp, retorna o registro correspondente
REGISTRO *retornaRegistro(char *entrada, char *saida, int numUsp)
{

    FILE *arquivoEntrada = fopen(entrada, "r");
    if (!arquivoEntrada)
    {
        fprintf(stderr, "Houve um erro de alocação na função inserirNovoRegistro");
        exit(-1);
    }

    int nroUsp, curso, estado, cidade, valido;
    while (fscanf(arquivoEntrada, "%d %d %d %d %d", &nroUsp, &curso, &estado, &cidade, &valido) == 5)
    {
        if( nroUsp == numUsp ){
            REGISTRO *registros = (REGISTRO*)malloc(sizeof(REGISTRO)); //aloco só ela
            registros->cidade = cidade;
            registros->curso = curso;
            registros->estado = estado;
            registros->nroUsp = nroUsp;
            registros->valido = valido;
            
            return registros;
        }
    }

    fprintf(stderr, "Nao existe um registro com esse numero usp");
    fclose(arquivoEntrada);
    exit(-1);
}


void imprimirVetor( char *saida, REGISTRO *registros, int tamanho ){

    FILE *arquivoSaida = fopen( saida, "w" );

    for( int i = 0 ; i < tamanho ; i++ ){
        fprintf( arquivoSaida, "%d %d %d %d %d\n", registros[i].nroUsp, registros[i].curso, registros[i].estado, registros[i].cidade, registros[i].valido );
    }
}

// ordenação 
void insertionSort( char *saida, REGISTRO *registros, int tamanho ){
    int i, chave, j;

    for( i = 1 ; i < tamanho ; i++ ){
        chave = registros[i].nroUsp;
        j = i - 1;

        while( j>= 0 && registros[j].nroUsp > chave ){
            registros[j+1].nroUsp = registros[j].nroUsp;
            j--;
        }
        registros[j+1].nroUsp = chave;
    }

    imprimirVetor( saida, registros, tamanho );
}


//função que, dado um arq1.txt com registros, os ordena e coloca em um arq2.txt
void lerESalvar( char *entrada, char *saida ){

    FILE *arquivoEntrada = fopen( entrada, "r" );

    if( !arquivoEntrada ){
        fprintf( stderr, "Nao foi possivel abrir o arquivo de entrada" );
        exit(-1);
    }

    int quantidade = 0, capacidade = 20;
    REGISTRO *registros = (REGISTRO*)malloc(sizeof(REGISTRO) * capacidade );

    int nroUsp, curso, estado, cidade, valido; 
    while( fscanf( arquivoEntrada, "%d %d %d %d %d",  &nroUsp, &curso, &estado, &cidade, &valido ) == 5 ){

        if( quantidade == capacidade ){
            capacidade *= 2;

            REGISTRO *registrosAux = (REGISTRO*)realloc(registros, sizeof(REGISTRO) * capacidade );

            if( !registrosAux ){
                fprintf( stderr, "Nao foi possivel alocar registrosAux na funcao lerESalvar" );
                fclose(arquivoEntrada);
                exit(-1);
            }
            registros = registrosAux;
        }

        registros[quantidade].nroUsp = nroUsp;
        registros[quantidade].curso = curso;
        registros[quantidade].estado = estado;
        registros[quantidade].cidade = cidade;
        registros[quantidade].valido = valido;

        quantidade++;
    }

    insertionSort( saida, registros, quantidade );

    fclose(arquivoEntrada);
    free(registros);
}


int main(int argc, char *argv[]) //argc = quantos argumentos são passados, argv[] = array com argumentos
{

    if (argc < 3) //tem que ter 3 argumentos: o executável, entrada.txt, saida.txt
    {
        fprintf(stderr, "Este programa exige 3 arquivos e aqui só tem %d", argc);
        exit(-1);
    }

    //argv[0] é o executável
    char *entrada = argv[1];
    char *saida = argv[2];

    /* tire as funções da anotação e teste (uma a uma, de preferência)*/

    // passarValidos( entrada, saida ); função ok

    // inverterRegistros( entrada, saida ); função ok

    // inserirNovoRegistro( entrada, saida, 14879963, 3, 90, 76, 1 ); função ok
    // inserirNovoRegistro( entrada, saida, 45678994, 3, 90, 76, 1 ); função ok

    //printarRegistro(saida, retornaRegistro(entrada, saida, 14879963)); função ok

    //lerESalvar( entrada, saida ); função ok

    return 0;
}