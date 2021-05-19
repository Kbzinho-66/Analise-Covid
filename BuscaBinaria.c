//Gabriel Vieira Alles - Algoritmos e Estruturas de Dados II

#include <stdio.h>
#include <stdlib.h>

struct registro {
    int codigo;
    char nome[15];
    int idade;
    int pontos; // Nao sei o que de fato eh esse numero
};
typedef struct registro Registro;

FILE *arquivo;

void gerarArquivoBinario();

int pesquisaBinaria(int codigo, int comeco, int fim);

int main(){

    arquivo = fopen("Registros.bin", "rb");
    size_t tamanho;
    int posicoes, meio;
    Registro atual;

    fseek(arquivo, 0L, SEEK_END);
    tamanho = ftell(arquivo);
    posicoes = tamanho / sizeof(Registro);
    rewind(arquivo);

    int codigo;
    char buffer[10];

    gerarArquivoBinario();

    printf("Insira o código a ser pesquisado:\n");
    fgets(buffer, 9, stdin);
    codigo = (int) strtod(buffer, NULL);
    printf("Procurando pelo código %d...\n", codigo);
    

    if (!pesquisaBinaria(codigo, 0, posicoes)) {
        printf("Não foi encontrado um registro com esse código.\n");
    }

    return 0;
}

void gerarArquivoBinario() {

    FILE *texto, *binario;

    texto = fopen("Registros.txt", "r");
    binario = fopen("Registros.bin", "wb");
    
    if (texto == NULL || binario == NULL) {
        printf("Erro ao abrir um dos arquivos. \n");
        exit(0);
    }

    Registro temp;

    while (fscanf(texto, "%d%s%d%d", &temp.codigo, temp.nome, &temp.idade, &temp.pontos) == 4) {
        fwrite(&temp, sizeof(Registro), 1, binario);
    }

    fclose(texto);
    fclose(binario);
}

int pesquisaBinaria(int codigo, int comeco, int fim) {

    Registro temp;

    int meio = (comeco + fim) / 2;
    fseek(arquivo, meio * sizeof(Registro), SEEK_SET);
    fread(&temp, sizeof(Registro), 1, arquivo);

    // printf("Meio = %d\n", meio);
    // printf("%d %s %d %d\n", temp.codigo, temp.nome, temp.idade, temp.pontos);

    if (codigo == temp.codigo) {
        printf("--------------------------------------------------------\n");
        printf("Encontrado:\nNome: %s\nIdade: %d\tPontos: %d\n", temp.nome, temp.idade, temp.pontos);
        printf("--------------------------------------------------------\n");
        return 1;
    } else if (codigo < temp.codigo && comeco != fim) {
        printf("Menor que %d\n", temp.codigo);
        return pesquisaBinaria(codigo, comeco, meio);
    } else if (comeco != fim) {
        printf("Maior que %d\n", temp.codigo);
        return pesquisaBinaria(codigo, meio+1, fim);
    }

    return 0;
}