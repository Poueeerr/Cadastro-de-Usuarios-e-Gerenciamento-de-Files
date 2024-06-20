#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct CADASTRO {
    char nome[100];
    int idade;
    float saldo;
    int id;
};

struct TRANSFERENCIA {
    int id_origem;
    int id_destino;
    float quantia;
};

int adduser(int qtdusuarios, struct CADASTRO **cad) {
    *cad = realloc(*cad, (qtdusuarios + 1) * sizeof(struct CADASTRO));
    if (*cad == NULL) {
        printf("FALHA AO ALOCAR MEMORIA\n");
        return qtdusuarios;
    }

    do {
        printf("\nDigite o nome completo do titular da nova conta: ");
        fgets((*cad)[qtdusuarios].nome, 100, stdin);
        (*cad)[qtdusuarios].nome[strcspn((*cad)[qtdusuarios].nome, "\n")] = 0;
        if (strlen((*cad)[qtdusuarios].nome) >= 100) {
            printf("ERRO: NOME COM TAMANHO MAIOR QUE 100 CARACTERES\n");
        }
    } while (strlen((*cad)[qtdusuarios].nome) >= 100);

    printf("\nDigite a idade do titular: ");
    scanf("%d", &(*cad)[qtdusuarios].idade);
    getchar();

    printf("\nDigite o saldo do titular: ");
    scanf("%f", &(*cad)[qtdusuarios].saldo);
    getchar();

    (*cad)[qtdusuarios].id = qtdusuarios + 1;

    return qtdusuarios + 1;
}

int addmultusers(int qtdusuarios, struct CADASTRO **cad) {
    int qtdusuariosolicitados;
    printf("\nDigite o numero de usuarios que deseja adicionar: ");
    scanf("%d", &qtdusuariosolicitados);
    getchar();

    for (int i = 0; i < qtdusuariosolicitados; i++) {
        qtdusuarios = adduser(qtdusuarios, cad);
    }

    return qtdusuarios;
}

void saveUsersToFile(int qtdusuarios, struct CADASTRO *cad, const char *filename) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo para escrita!\n");
        return;
    }

    for (int i = 0; i < qtdusuarios; i++) {
        fprintf(file, "%s, %d, %.2f, %d\n", cad[i].nome, cad[i].idade, cad[i].saldo, cad[i].id);
    }

    fclose(file);
}

void TransacaoArquivo(int qtdTransf, struct TRANSFERENCIA *transf, const char *filename) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo para escrita!\n");
        return;
    }
    for (int i = 0; i < qtdTransf; i++) {
        fprintf(file, "%d, %d, %.2f\n", transf[i].id_origem, transf[i].id_destino, transf[i].quantia);
    }
    fclose(file);
}

void loadUsersFromFile(struct CADASTRO **cad, int *qtdusuarios, const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Arquivo nao pode ser aberto!\n");
        return;
    }

    *qtdusuarios = 0;
    struct CADASTRO temp;
    while (fscanf(file, "%99[^,], %d, %f, %d\n", temp.nome, &temp.idade, &temp.saldo, &temp.id) == 4) {
        *cad = realloc(*cad, (*qtdusuarios + 1) * sizeof(struct CADASTRO));
        if (*cad == NULL) {
            printf("FALHA AO ALOCAR MEMORIA\n");
            fclose(file);
            return;
        }
        (*cad)[*qtdusuarios] = temp;
        (*qtdusuarios)++;
    }

    fclose(file);
}

void Atualizar(struct TRANSFERENCIA **transf, int *qtdTransf, const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Arquivo nao pode ser aberto!\n");
        return;
    }

    *qtdTransf = 0;
    struct TRANSFERENCIA temp;
    while (fscanf(file, "%d, %d, %f\n", &temp.id_origem, &temp.id_destino, &temp.quantia) == 3) {
        *transf = realloc(*transf, (*qtdTransf + 1) * sizeof(struct TRANSFERENCIA));
        if (*transf == NULL) {
            printf("FALHA AO ALOCAR MEMORIA\n");
            fclose(file);
            return;
        }
        (*transf)[*qtdTransf] = temp;
        (*qtdTransf)++;
    }

    fclose(file);
}

void mostrarUsuario(const char *filename) {
    int id;
    struct CADASTRO user;
    printf("Digite o id do usuario que deseja buscar: ");
    scanf("%d", &id);
    getchar();

    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Arquivo nao pode ser aberto!\n");
        return;
    }

    while (fscanf(file, "%99[^,], %d, %f, %d\n", user.nome, &user.idade, &user.saldo, &user.id) != EOF) {
        if (user.id == id) {
            printf("\nNome: %s\nIdade: %d\nSaldo: %.2f\nID: %d\n\n", user.nome, user.idade, user.saldo, user.id);
            fclose(file);
            return;
        }
    }

    printf("\nErro: Usuario %d nao encontrado.\n\n", id);
    fclose(file);
}

int existeId(int id, const char *filename) {
    struct CADASTRO user;
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Arquivo nao pode ser aberto!\n");
        return 0;
    }

    while (fscanf(file, "%99[^,], %d, %f, %d\n", user.nome, &user.idade, &user.saldo, &user.id) != EOF) {
        if (user.id == id) {
            fclose(file);
            return 1;
        }
    }

    fclose(file);
    return 0;
}

float saldoUsuario(int id, const char *filename) {
    struct CADASTRO user;
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Arquivo nao pode ser aberto!\n");
        return 0;
    }

    while (fscanf(file, "%99[^,], %d, %f, %d\n", user.nome, &user.idade, &user.saldo, &user.id) != EOF) {
        if (user.id == id) {
            fclose(file);
            return user.saldo;
        }
    }

    fclose(file);
    return 0;
}

void atualizarSaldoUsuario(int id, float novoSaldo, const char *filename) {
    struct CADASTRO *usuarios = NULL;
    int qtdusuarios = 0;
    loadUsersFromFile(&usuarios, &qtdusuarios, filename);

    for (int i = 0; i < qtdusuarios; i++) {
        if (usuarios[i].id == id) {
            usuarios[i].saldo = novoSaldo;
        }
    }

    saveUsersToFile(qtdusuarios, usuarios, filename);
    free(usuarios);
}

void transferencia(struct TRANSFERENCIA **transf, int *qtdTransf, const char *filename1, const char *filename) {
    int id_origem, id_destino;
    struct TRANSFERENCIA transacao;
    float quantia, saldoOrigem, saldoDestino, novoSaldoOrigem, novoSaldoDestino;

    printf("Digite o id do usuario de origem: ");
    scanf("%d", &id_origem);

    printf("Digite o id do usuario de destino: ");
    scanf("%d", &id_destino);

    if ((existeId(id_origem, filename) && existeId(id_destino, filename)) && (id_origem != id_destino)) {
        printf("Digite o valor da quantia: ");
        scanf("%f", &quantia);

        saldoOrigem = saldoUsuario(id_origem, filename);
        saldoDestino = saldoUsuario(id_destino, filename);

        if (quantia <= saldoOrigem) {
            *transf = realloc(*transf, (*qtdTransf + 1) * sizeof(struct TRANSFERENCIA));
            if (*transf == NULL) {
                printf("FALHA AO ALOCAR MEMORIA\n");
                return;
            }

            transacao.id_origem = id_origem;
            transacao.id_destino = id_destino;
            transacao.quantia = quantia;
            (*transf)[*qtdTransf] = transacao;
            novoSaldoOrigem = saldoOrigem - quantia;
            novoSaldoDestino = saldoDestino + quantia;

            atualizarSaldoUsuario(id_origem, novoSaldoOrigem, filename);
            atualizarSaldoUsuario(id_destino, novoSaldoDestino, filename);

            (*qtdTransf)++;
            TransacaoArquivo(*qtdTransf, *transf, filename1);

            printf("Transferencia realizada com sucesso!\n");
        } else {
            printf("Saldo insuficiente para realizar a transferencia.\n");
        }
    } else {
        printf("Usuario de origem ou destino nao existe ou sao iguais.\n");
    }
}

int main() {
    struct CADASTRO *cad = NULL;
    struct TRANSFERENCIA *transf = NULL;
    int qtdusuarios = 0;
    int qtdTransf = 0;
    int opcao;
    const char *filename = "CLIENT_DATA.txt";
    const char *filename1 = "transferencias.txt";

    // Carregar usuários automaticamente ao iniciar o programa
    loadUsersFromFile(&cad, &qtdusuarios, filename);

    do {
        printf("Menu:\n");
        printf("1. Adicionar usuario\n");
        printf("2. Adicionar multiplos usuarios\n");
        printf("3. Mostrar usuario\n");
        printf("4. Realizar transferencia\n");
        printf("0. Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);
        getchar(); // Limpa o buffer do teclado

        switch (opcao) {
            case 1:
                qtdusuarios = adduser(qtdusuarios, &cad);
                saveUsersToFile(qtdusuarios, cad, filename);
                break;
            case 2:
                qtdusuarios = addmultusers(qtdusuarios, &cad);
                saveUsersToFile(qtdusuarios, cad, filename);
                break;
            case 3:
                mostrarUsuario(filename);
                break;
            case 4:
                transferencia(&transf, &qtdTransf, filename1, filename);
                break;
            case 0:
                printf("Saindo...\n");
                break;
            default:
                printf("Opcao invalida!\n");
                break;
        }
    } while (opcao != 0);

    free(cad);
    free(transf);
    return 0;
}
