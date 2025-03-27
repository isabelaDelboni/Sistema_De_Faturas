#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct Fatura {
    int numero_fatura;           // Número de id da fatura (gerado automaticamente)
    char data_vencimento[11];    // Formato: dd/mm/aaaa
    float valor_fatura;          // Valor fatura
    char status[10];             // Pendente ou paga
    struct Fatura *esquerda;     // Aponta para a fatura à esquerda
    struct Fatura *direita;      // Aponta para a fatura filho à direita
} Fatura;

static int gerador_id_fatura = 1;

// Cria um novo nó de fatura 
Fatura* criar_fatura(char* data_vencimento, float valor_fatura, char* status) {
    // Aloca um espaço na memória para o novo nó   
    Fatura* nova_fatura = (Fatura*) malloc(sizeof(Fatura));
    // Gera o número da fatura automaticamente e incrementa
    nova_fatura->numero_fatura = gerador_id_fatura++;  
    strcpy(nova_fatura->data_vencimento, data_vencimento);
    nova_fatura->valor_fatura = valor_fatura;
    strcpy(nova_fatura->status, status);
    // Inicializa os ponteiro esquerda e direita 
    nova_fatura->esquerda = NULL;
    nova_fatura->direita = NULL;
    return nova_fatura;
}

// Insere a fatura na árvore binária de busca
Fatura* inserir_fatura(Fatura* raiz, char* data_vencimento, float valor_fatura, char* status) {
    // Verifica se é vazia, se for cria uma nova fatura e a coloca     
    if (raiz == NULL) {
        return criar_fatura(data_vencimento, valor_fatura, status);
    }
    
    // Cria a nova fatura para comparar
    Fatura* nova_fatura = criar_fatura(data_vencimento, valor_fatura, status);

    // Verifica se o número da fatura da nova fatura é menor ou maior que o número da fatura atual
    if (nova_fatura->numero_fatura < raiz->numero_fatura) {
        raiz->esquerda = inserir_fatura(raiz->esquerda, data_vencimento, valor_fatura, status);
    } else if (nova_fatura->numero_fatura > raiz->numero_fatura) {
        raiz->direita = inserir_fatura(raiz->direita, data_vencimento, valor_fatura, status);
    }
    
    return raiz;
}

// Exibe as faturas em ordem crescente
void exibir_faturas(Fatura* raiz) {
    if (raiz != NULL) {
        // Exibe as faturas menores, ou seja, lado esquerdo     
        exibir_faturas(raiz->esquerda);
        printf("Numero da Fatura: %d\n", raiz->numero_fatura);
        printf("Data de Vencimento: %s\n", raiz->data_vencimento);
        printf("Valor: %.2f\n", raiz->valor_fatura);
        printf("Status: %s\n\n", raiz->status);
        // Exibe as faturas maiores, ou seja, lado direito
        exibir_faturas(raiz->direita);
    }
}

// Buscar a fatura pelo id
Fatura* buscar_fatura(Fatura* raiz, int numero_fatura) {
    // Se o nó for NULL ou o número da fatura for igual ao procurado, retorna o nó atual    
    if (raiz == NULL || raiz->numero_fatura == numero_fatura) {
        return raiz;
    }
    // Se for menor, buscamos à esquerda
    if (numero_fatura < raiz->numero_fatura) {
        return buscar_fatura(raiz->esquerda, numero_fatura);
    }
    // Se não, buscamos à direita
    return buscar_fatura(raiz->direita, numero_fatura);
}

// Altera o status de pagamento 
void atualizar_status(Fatura* raiz, int numero_fatura, char* novo_status) {
    Fatura* fatura = buscar_fatura(raiz, numero_fatura);
    if (fatura != NULL) {
        strcpy(fatura->status, novo_status);
        printf("Status da fatura %d atualizado para '%s'.\n", numero_fatura, novo_status);
    } else {
        printf("Fatura %d não encontrada!\n", numero_fatura);
    }
}

// Encontra o nó com o valor mínimo para remover
Fatura* minimo(Fatura* raiz) {
    while (raiz && raiz->esquerda != NULL) {
        raiz = raiz->esquerda;
    }
    return raiz;
}

// Remove uma fatura da árvore
Fatura* remover_fatura(Fatura* raiz, int numero_fatura) {
    if (raiz == NULL) {
        return raiz;
    }

    if (numero_fatura < raiz->numero_fatura) {
        // Se o número da fatura for menor, busca à esquerda
        raiz->esquerda = remover_fatura(raiz->esquerda, numero_fatura);
    } else if (numero_fatura > raiz->numero_fatura) {
        // Se o número da fatura for maior, busca à direita
        raiz->direita = remover_fatura(raiz->direita, numero_fatura);
    } else {
        // Fatura encontrada
        if (raiz->esquerda == NULL) {
            // Se não tem nada à esquerda, substitui o nó pelo da direita
            Fatura* temp = raiz->direita;
            free(raiz);
            return temp;
        } else if (raiz->direita == NULL) {
            // Se não tem nada à direita, substitui o nó pelo da esquerda
            Fatura* temp = raiz->esquerda;
            free(raiz);
            return temp;
        }

        // Substitui o nó com dois nós pelo nó mínimo da direita e remove o nó mínimo
        Fatura* temp = minimo(raiz->direita);
        raiz->numero_fatura = temp->numero_fatura;
        strcpy(raiz->data_vencimento, temp->data_vencimento);
        raiz->valor_fatura = temp->valor_fatura;
        strcpy(raiz->status, temp->status);

        raiz->direita = remover_fatura(raiz->direita, temp->numero_fatura);
    }
    return raiz;
}

int main() {
    Fatura* raiz = NULL;
    int opcao, numero_fatura;
    char data_vencimento[11], status[10];
    float valor_fatura;

    do {
        printf("\nSistema de Faturas \n");
        printf("1. Inserir Fatura \n");
        printf("2. Exibir Faturas \n");
        printf("3. Buscar Fatura \n");
        printf("4. Atualizar Status de Pagamento \n");
        printf("5. Remover Fatura \n");
        printf("6. Sair \n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);
        getchar(); 

        switch (opcao) {
            case 1:
                printf("\n"); 
                printf("Digite a data de vencimento (dd/mm/aaaa): ");
                fgets(data_vencimento, sizeof(data_vencimento), stdin);
                data_vencimento[strcspn(data_vencimento, "\n")] = '\0'; // Remove o \n da string
                printf("Digite o valor da fatura: ");
                scanf("%f", &valor_fatura);
                getchar();
                printf("Digite o status da fatura (pendente/paga): ");
                fgets(status, sizeof(status), stdin);
                status[strcspn(status, "\n")] = '\0';
                raiz = inserir_fatura(raiz, data_vencimento, valor_fatura, status);
                printf("Fatura cadastrada, número: %d\n", gerador_id_fatura - 1);
                break;

            case 2:
                printf("\n"); 
                printf("Faturas cadastradas: \n");
                exibir_faturas(raiz);
                break;

            case 3:
                printf("\n");  
                printf("Digite o número da fatura para fazer a busca: ");
                scanf("%d", &numero_fatura);
                Fatura* fatura = buscar_fatura(raiz, numero_fatura);
                if (fatura != NULL) {
                    printf("Fatura encontrada! \n");
                    printf("Numero da Fatura: %d\n", fatura->numero_fatura);
                    printf("Data de Vencimento: %s\n", fatura->data_vencimento);
                    printf("Valor: %.2f\n", fatura->valor_fatura);
                    printf("Status: %s\n", fatura->status);
                } else {
                    printf("Fatura não encontrada!\n");
                }
                break;

            case 4:
                printf("\n");  
                printf("Digite o número da fatura para atualizar o status: ");
                scanf("%d", &numero_fatura);
                printf("Digite o novo status (pendente/paga): ");
                getchar();
                fgets(status, sizeof(status), stdin);
                status[strcspn(status, "\n")] = '\0'; // Remove o \n da string
                atualizar_status(raiz, numero_fatura, status);
                break;

            case 5:
                printf("\n"); 
                printf("Digite o número da fatura para remover: ");
                scanf("%d", &numero_fatura);
                raiz = remover_fatura(raiz, numero_fatura);
                printf("Fatura %d removida.\n", numero_fatura);
                break;

            case 6:
                printf("\n");  
                printf("Encerrando o sistema! \n");
                break;

            default:
                printf("Opção inválida! \n");
        }

    } while (opcao != 6);

    return 0;
}
