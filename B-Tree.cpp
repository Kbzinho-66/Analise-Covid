#include <iostream>
#include <array>
#include <limits>
#include <algorithm>
#include <vector>
#include <unistd.h>

#define ORDEM 1
#define LIMITE_CHAVES ORDEM * 2
#define LIMITE_FILHOS ORDEM*2 + 1

class NodoBTree {

    public:
        int nChaves = 0;
        std::array<int, LIMITE_CHAVES> chaves;
        std::array<struct NodoBTree* , LIMITE_FILHOS> filhos;
        struct NodoBTree *pai = NULL;
        bool folha;

    NodoBTree(int n, std::array<int, ORDEM> valores, NodoBTree *pai) {
        this->nChaves = n;
        this->pai = pai;
        
        for (int i=0; i<ORDEM; i++){
            this->chaves.at(i) = valores.at(i);
        }

        for (int i=0; i<LIMITE_FILHOS; i++) {
            this->filhos[i] = NULL;
        }

        if (pai == NULL){
            folha = false;
        } else {
            folha = true;
        }
    }

    NodoBTree(int valor, NodoBTree *pai) {
        this->nChaves = 1;
        this->pai = pai;
        this->chaves[0] = valor;

        for (int i=0; i<LIMITE_FILHOS; i++) {
            this->filhos[i] = NULL;
        }

        if (pai == NULL){
            folha = false;
        } else {
            folha = true;
        }
    }
};

int insercaoOrdenada(int valor, NodoBTree *nodo);

NodoBTree *insercaoEmFolha(NodoBTree *raiz, int valor, NodoBTree *pai);

NodoBTree *insercaoEmNodo(int valor, NodoBTree *atual, std::array<int, ORDEM> menores, std::array<int, ORDEM> maiores);

bool consultaValor(NodoBTree *raiz, int valor, int nivel, bool echo);

void mostraArvore(NodoBTree *raiz , bool crescente);

void remocaoNodo();

int main() {
    
    NodoBTree *raiz = NULL;

    int resposta, valor;
    std::cout << "----- B-Tree -----" << std::endl;

    do {


        std::cout << std::endl << "Escolha uma opção: " << std::endl;
        std::cout << "1: Inserção de um novo valor." << std::endl;
        std::cout << "2: Consulta de um valor." << std::endl;
        std::cout << "3: Exibição de todos os valores." << std::endl;
        std::cout << "4: Remoção de um valor." << std::endl;
        // std::cout << "5: Inserção de vários valores." << std::endl;
        std::cout << "0: Sair." << std::endl;
        
        // Laço pra leitura de uma resposta válida
        while ((std::cout << "Escolha uma opção:" << std::endl ) 
                && !(std::cin >> resposta) || resposta < 0 || resposta > 4) {
            std::cout << "Opção Inválida!" << std::endl;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }

        switch (resposta) {
            case 1:

                while ((std::cout << "Digite o valor a ser armazenado:" << std::endl ) 
                    && !(std::cin >> valor)) {
                std::cout << "O valor precisa ser um Inteiro!" << std::endl;
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                }

                if (consultaValor(raiz, valor, 0, false)){
                    std::cout << "Esse número já foi inserido!" << std::endl;
                } else {

                    raiz = insercaoEmFolha(raiz, valor, NULL);
                    std::cout << "Valor inserido!" << std::endl;

                }
                
                break;

            case 2:

                while ((std::cout << "Digite o valor a ser consultado:" << std::endl ) 
                    && !(std::cin >> valor)) {
                std::cout << "O valor precisa ser um Inteiro!" << std::endl;
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                }

                std::cout << "Procurando..." << std::endl;
                if (!consultaValor(raiz, valor, 0, true)){
                    std::cout << "Valor não encontrado." << std::endl;
                }
                break;

            case 3:
                
                // char forma;
                // while ((std::cout << "Mostrar de forma [C]rescente ou [D]ecrescente?:" << std::endl ) 
                //     && !(std::cin >> forma) || toupper(forma) < 'C' || toupper(forma) > 'D') {
                // std::cout << "Você precisa digitar C/c ou D/d!" << std::endl;
                // std::cin.clear();
                // std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                // }

                // if (toupper(forma) == 'C') {
                    mostraArvore(raiz, true);
                // } else {
                //     mostraArvore(raiz, false);
                // }

                break;
            case 4:
                // TODO remocaoNodo();
                break;
        }
  
    } while (resposta);
    
    return 0;
}

int insercaoOrdenada(int valor, NodoBTree *nodo) {

    int troca;
    int posicao;
    NodoBTree *esq;
    NodoBTree *dir;

        for (int i=0; i<=nodo->nChaves-1; i++) {
            if (i == nodo->nChaves-1 && valor < nodo->chaves.at(i)) { // É menor que o último elemento

                nodo->chaves.at(i+1) = nodo->chaves.at(i);
                nodo->filhos[i+2] = nodo->filhos[i+1];

                nodo->chaves.at(i) = valor;
                nodo->nChaves++;
                return i; // Retornar a posição em que o novo elemento foi colocado

            } else if (i == nodo->nChaves-1 && valor > nodo->chaves.at(i)) { // Maior que o último elemento

                nodo->chaves.at(i+1) = valor;
                nodo->nChaves++;
                return i+1;

            } else if (valor < nodo->chaves.at(i)) {

                troca = nodo->chaves.at(i);
                esq = nodo->filhos[i];
                dir = nodo->filhos[i+1];

                nodo->chaves.at(i) = valor;
                nodo->nChaves++;
                posicao = i;

                for (int j=i+1; j<nodo->nChaves-1; j++){
                    std::swap(troca, nodo->chaves.at(j));
                    std::swap(esq, nodo->filhos[j]);
                    std::swap(dir, nodo->filhos[j+1]);
                }

                return posicao;
            }
        }

    return posicao;
}

NodoBTree *insercaoEmFolha(NodoBTree *raiz, int valor, NodoBTree *pai) {

    NodoBTree *atual = raiz;
    if (raiz == NULL){ // Se a árvore estiver vazia, vira a raiz

        NodoBTree *novo = new NodoBTree(valor, NULL);
        novo->folha = true;
        return(novo);

    } else if (atual->folha) { // Se chegou numa folha, tentar colocar o valor
        
        if (atual->nChaves + 1 <= LIMITE_CHAVES) { // O valor cabe na folha

            int posicao = insercaoOrdenada(valor, atual);

        } else { 

            // Achar a mediana dos valores
            std::array<int, ORDEM*2 + 1> overflow; 

            
            for (int i=0; i<atual->nChaves; i++){
                overflow.at(i) = atual->chaves.at(i);
            }
            overflow[atual->nChaves] = valor;
            std::sort(overflow.begin(), overflow.end());

            int valorMedio = overflow[ORDEM];

            // Subir o valorMedio e criar duas folhas com metade dos valores em cada uma
            std::array<int, ORDEM> valoresMenores;
            std::array<int, ORDEM> valoresMaiores;

            for (int i=0; i< ORDEM*2 + 1; i++){

                if (i < ORDEM) {
                    valoresMenores.at(i) = overflow[i];
                } else if (i > ORDEM) {
                    valoresMaiores.at(2*ORDEM - i) = overflow[i];
                }

            }

            if (pai == NULL){

                pai = new NodoBTree(valorMedio, NULL);
                pai->filhos[0] = new NodoBTree(ORDEM, valoresMenores, pai);
                pai->filhos[1] = new NodoBTree(ORDEM, valoresMaiores, pai);

                free(atual); // O nodo com os valores antigos deixa de existir
                return pai; // O novo pai passa a ser a raiz

            } else {

                if (pai->nChaves + 1 <= LIMITE_CHAVES) { // Se o valorMedio cabe no pai, inserir

                    int posicao = insercaoOrdenada(valorMedio, pai);
                    pai->filhos[posicao] = new NodoBTree(ORDEM, valoresMenores, pai);
                    pai->filhos[posicao+1] = new NodoBTree(ORDEM, valoresMaiores, pai);

                    free(atual);
                    return pai;

                } else { // O valorMedio não cabe no pai, reorganizar a árvore

                    return insercaoEmNodo(valorMedio, pai, valoresMenores, valoresMaiores);

                }
            }
        }

    } else { // Se não, passa por todos elementos da raiz pra encontrar o filho que vai seguir

        for (int i=0; i<atual->nChaves; i++){

            if (valor < atual->chaves[i]) { // Se o valor é menor que a chave atual, vai pro filho menor

                if (atual->filhos[i] != NULL && (atual->filhos[i])->nChaves+1 <= LIMITE_CHAVES) { //Folha existente e o valor cabe

                    atual->filhos[i] = insercaoEmFolha(atual->filhos[i], valor, atual);
                    return raiz;

                } else if (atual->filhos[i] != NULL && (atual->filhos[i])->nChaves+1 > LIMITE_CHAVES) { // Folha existe, mas o valor não cabe

                    return insercaoEmFolha(atual->filhos[i], valor, atual);

                } else { //Criar uma folha 

                    atual->filhos[i] = insercaoEmFolha(NULL, valor, atual);
                    atual->folha = false;
                    return raiz;

                }

            } else if (i == atual->nChaves - 1) { // Só acessar o filho maior se for o último valor

                if (atual->filhos[i+1] != NULL && (atual->filhos[i+1])->nChaves+1 <= LIMITE_CHAVES) {

                    atual->filhos[i+1] = insercaoEmFolha(atual->filhos[i+1], valor, atual);
                    return raiz;

                } else if (atual->filhos[i+1] != NULL && (atual->filhos[i+1])->nChaves+1 > LIMITE_CHAVES) {

                    return insercaoEmFolha(atual->filhos[i+1], valor, atual);

                } else {

                    atual->filhos[i+1] = insercaoEmFolha(NULL, valor, atual);
                    atual->folha = false;
                    return raiz;

                }
                
            }
        }

    }

    return raiz;
}

NodoBTree *insercaoEmNodo(int valor, NodoBTree *atual, std::array<int, ORDEM> menores, std::array<int, ORDEM> maiores) {

    /*  Função usada somente quando um nodo pai não suporta mais um valor.
        Sobe recursivamente a árvore até achar um nodo que suporte,
        dividindo quaisquer nodos que ficarem lotados. */

    if (atual == NULL) {

        NodoBTree *novoPai = new NodoBTree(valor, NULL);

        novoPai->filhos[0] = new NodoBTree(ORDEM, menores, novoPai);
        novoPai->filhos[0]->folha = false;

        novoPai->filhos[1] = new NodoBTree(ORDEM, maiores, novoPai);
        novoPai->filhos[1]->folha = false;
        
        return novoPai;

    } else if (atual->nChaves + 1 <= LIMITE_CHAVES) { // Se chegar num nodo onde cabe o valor, inserir

        insercaoOrdenada(valor, atual);
        atual->filhos[atual->nChaves++] = new NodoBTree(ORDEM, menores, atual);
        atual->filhos[atual->nChaves] = new NodoBTree(ORDEM, maiores, atual);
        return atual;

    } else { // Até isso acontecer, subir recursivamente a árvore

        std::array<int, ORDEM*2 + 1> overflow; 

        for (int i=0; i<atual->nChaves; i++){
            overflow.at(i) = atual->chaves.at(i);
        }
        overflow[atual->nChaves] = valor;
        std::sort(overflow.begin(), overflow.end());

        int valorMedio = overflow[ORDEM];

        std::array<int, ORDEM> valoresMenores;
        std::array<int, ORDEM> valoresMaiores;

        for (int i=0; i<ORDEM*2 + 1; i++){

            if (i < ORDEM) {
                valoresMenores.at(i) = overflow[i];
            } else if (i > ORDEM) {
                valoresMaiores.at(2*ORDEM - i) = overflow[i];
            }
            
        }

        NodoBTree *pai = insercaoEmNodo(valorMedio, atual->pai, valoresMenores, valoresMaiores);
        // O valor foi inserido em um nodo pai, transferir os filhos agora
        NodoBTree *esq = pai->filhos[pai->nChaves-1];
        NodoBTree *dir = pai->filhos[pai->nChaves];

        esq->filhos[0] = new NodoBTree(ORDEM, menores, esq);
        esq->filhos[1] = new NodoBTree(ORDEM, maiores, esq);

        dir->filhos[0] = atual->filhos[ORDEM];
        dir->filhos[1] = atual->filhos[ORDEM+1];

        return pai;
    }

}

bool consultaValor(NodoBTree *raiz, int valor, int nivel, bool echo) {
    
    NodoBTree *aux = raiz;
    if (raiz == NULL) {
        return false;
    }

    for (int i=0; i<=raiz->nChaves; i++) {

        if (i == raiz->nChaves) { // Chegou no fim das chaves, logo não encontrou

            return false;

        } else if (valor < raiz->chaves.at(i)) { // Quando for menor que qualquer elemento, vai pro filho menor

            if (echo) std::cout << "Menor que " << raiz->chaves.at(i) << " no nível " << nivel << std::endl;
            if (consultaValor(raiz->filhos[i], valor, ++nivel, echo)) {
                return true;
            }

        } else if (valor == raiz->chaves.at(i)) { // Valor encontrado

            if (echo) {
                std::cout << "Valor encontrado no nível " << nivel << ", no nodo a seguir:" << std::endl;
                for (int i=0; i<raiz->nChaves; i++) {
                    std::cout << " | " << raiz->chaves.at(i);
                }
                std::cout << " |" << std::endl;
            } 
            return true;

        } else if (valor > raiz->chaves.at(i) && i == raiz->nChaves - 1) { // Só acessar o filho maior se for o último elemento

            if (echo) std::cout << "Maior que " << raiz->chaves.at(i) << " no nível " << nivel << std::endl;
            if (consultaValor(raiz->filhos[i+1], valor, ++nivel, echo)) {
                return true;
            }

        }

    }

    return false;

}

void mostraArvore(NodoBTree *raiz, bool crescente) {

    if (raiz == NULL) {
        return;
    } else {

        if (crescente) {

            for (int i=0; i<raiz->nChaves; i++) {

                if (i < raiz->nChaves - 1) {
                    mostraArvore(raiz->filhos[i], crescente);

                    std::cout << raiz->chaves.at(i) << " ";

                } else {

                    std::cout << raiz->chaves.at(i) << " ";

                    mostraArvore(raiz->filhos[raiz->nChaves], crescente);
                }
                

            }

        } else {
            
            // TODO Fazer decrescente

        }
        
    }

}