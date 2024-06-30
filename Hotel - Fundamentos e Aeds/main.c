#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <assert.h>

typedef struct {
    int codigo;
    char nome[100];
    char endereco[200];
    int telefone;
} cliente;

typedef struct {
    int codigo;
    char nome[100];
    int telefone;
    char cargo[100];
    float salario;
} funcionario;

typedef struct {
    int numero;
    int numero_hospedes;
    float valor_diario;
    char status[100]; // "ocupado" ou "desocupado"
} quarto;

typedef struct {
    int codigo;
    char data_entrada[100];
    char data_saida[200];
    int codigo_cliente;
    int numero_quarto;
} estadia;

cliente Clientes[100];
funcionario Funcionarios[100];
quarto Quartos[100];
estadia Estadias[100];

int numero_cliente = 0;
int numero_funcionario = 0;
int numero_quarto = 0;
int numero_estadia = 0;

int cadastro_de_cliente(int codigo, char nome[], char endereco[], int telefone) {
    int i;
    // Verifica se já existe
    for (i = 0; i < numero_cliente; i++) {
        if (Clientes[i].codigo == codigo) {
            printf("Cliente com código %d já existe.\n", codigo);
            return 0;
        }
    }
    cliente c;
    c.codigo = codigo;
    strcpy(c.nome, nome);
    strcpy(c.endereco, endereco);
    c.telefone = telefone;
    Clientes[numero_cliente++] = c;
    printf("Cliente %s cadastrado com sucesso.\n", nome);
    return 1;
}

int cadastro_de_funcionario(int codigo, char nome[], int telefone, char cargo[], float salario) {
    int i;
    for (i = 0; i < numero_funcionario; i++) {
        if (Funcionarios[i].codigo == codigo) {
            printf("Funcionário com código %d já existe.\n", codigo);
            return 0;
        }
    }
    funcionario f;
    f.codigo = codigo;
    strcpy(f.nome, nome);
    f.telefone = telefone;
    strcpy(f.cargo, cargo);
    f.salario = salario;
    Funcionarios[numero_funcionario++] = f;
    printf("Funcionário %s cadastrado com sucesso.\n", nome);
    return 1;
}

int cadastrar_quarto(int numero, int numero_hospedes, float valor_diario) {
    int i;

    for (i = 0; i < numero_quarto; i++) {
        if (Quartos[i].numero == numero) {
            printf("Quarto com número %d já existe.\n", numero);
            return 0;
        }
    }
    // Cria o quarto e adiciona ao array
    quarto q;
    q.numero = numero;
    q.numero_hospedes = numero_hospedes;
    q.valor_diario = valor_diario;
    strcpy(q.status, "desocupado");
    Quartos[numero_quarto++] = q;
    printf("Quarto %d cadastrado com sucesso.\n", numero);
    return 1;
}

int calcular_diarias(char data_entrada[], char data_saida[]) {
    int diae, mese, anoe; // entrada
    int diass, mess, anos; // saída

    // Extrai dia, mês e ano da data de entrada
    sscanf(data_entrada, "%d/%d/%d", &diae, &mese, &anoe);

    // Extrai dia, mês e ano da data de saída
    sscanf(data_saida, "%d/%d/%d", &diass, &mess, &anos);

    // Converte as datas para o formato de dias desde uma data base (simples, mas impreciso)
    int diae_em_dias = anoe * 365 + mese * 30 + diae;
    int diass_em_dias = anos * 365 + mess * 30 + diass;

    // Calcula o número de dias entre as duas datas
    return diass_em_dias - diae_em_dias;
}

int verificar_disponibilidade_quarto(int numero_quarto, char data_entrada[], char data_saida[]) {
    int i;
    for (i = 0; i < numero_estadia; i++) {
        if (Estadias[i].numero_quarto == numero_quarto) {
            int dia_entrada_estadia, mes_entrada_estadia, ano_entrada_estadia;
            int dia_saida_estadia, mes_saida_estadia, ano_saida_estadia;
            int dia_entrada_nova, mes_entrada_nova, ano_entrada_nova;
            int dia_saida_nova, mes_saida_nova, ano_saida_nova;

            sscanf(Estadias[i].data_entrada, "%d/%d/%d", &dia_entrada_estadia, &mes_entrada_estadia, &ano_entrada_estadia);
            sscanf(Estadias[i].data_saida, "%d/%d/%d", &dia_saida_estadia, &mes_saida_estadia, &ano_saida_estadia);
            sscanf(data_entrada, "%d/%d/%d", &dia_entrada_nova, &mes_entrada_nova, &ano_entrada_nova);
            sscanf(data_saida, "%d/%d/%d", &dia_saida_nova, &mes_saida_nova, &ano_saida_nova);

            int dias_entrada_estadia = ano_entrada_estadia * 365 + mes_entrada_estadia * 30 + dia_entrada_estadia;
            int dias_saida_estadia = ano_saida_estadia * 365 + mes_saida_estadia * 30 + dia_saida_estadia;
            int dias_entrada_nova = ano_entrada_nova * 365 + mes_entrada_nova * 30 + dia_entrada_nova;
            int dias_saida_nova = ano_saida_nova * 365 + mes_saida_nova * 30 + dia_saida_nova;

            if (!(dias_saida_nova <= dias_entrada_estadia || dias_entrada_nova >= dias_saida_estadia)) {
                return 0;
            }
        }
    }
    return 1;
}

int cadastrar_estadia(int codigo_cliente, int numero_hospedes, char data_entrada[], char data_saida[]) {
    if (numero_cliente == 0) {
        printf("Erro: Nenhum cliente registrado. Redirecionando para o menu...\n");
        return 0;
    }

    if (numero_quarto == 0) {
        printf("Erro: Nenhum quarto registrado. Redirecionando para o menu...\n");
        return 0;
    }

    int i, j;
    for (i = 0; i < numero_cliente; i++) {
        if (Clientes[i].codigo == codigo_cliente) {
            printf("Quartos disponíveis:\n");
            int encontrou_quarto = 0;
            for (j = 0; j < numero_quarto; j++) {
                if (Quartos[j].status[0] == 'd' && Quartos[j].numero_hospedes >= numero_hospedes && verificar_disponibilidade_quarto(Quartos[j].numero, data_entrada, data_saida)) {
                    printf("Número do Quarto: %d, Capacidade: %d, Valor da Diária: %.2f\n", Quartos[j].numero, Quartos[j].numero_hospedes, Quartos[j].valor_diario);
                    encontrou_quarto = 1;
                }
            }

            if (!encontrou_quarto) {
                printf("Nenhum quarto disponível para as datas e quantidade de hóspedes solicitados.\n");
                return 0;
            }

            int numero_quarto_escolhido;
            printf("Escolha o número do quarto disponível: ");
            scanf("%d", &numero_quarto_escolhido);

            for (j = 0; j < numero_quarto; j++) {
                if (Quartos[j].numero == numero_quarto_escolhido) {
                    if (verificar_disponibilidade_quarto(Quartos[j].numero, data_entrada, data_saida)) {
                        int qtd_diarias = calcular_diarias(data_entrada, data_saida);
                        estadia e;
                        e.codigo = numero_estadia + 1;
                        strcpy(e.data_entrada, data_entrada);
                        strcpy(e.data_saida, data_saida);
                        e.codigo_cliente = codigo_cliente;
                        e.numero_quarto = Quartos[j].numero;
                        Estadias[numero_estadia++] = e;
                        strcpy(Quartos[j].status, "ocupado");
                        printf("Estadia cadastrada com sucesso no quarto %d.\n", Quartos[j].numero);
                        printf("Número da estadia: %d\n", e.codigo);
                        return 1;
                    }
                }
            }

            printf("Nenhum quarto disponível para as datas e quantidade de hóspedes solicitados.\n");
            return 0;
        }
    }

    printf("Cliente com código %d não encontrado.\n", codigo_cliente);
    return 0;
}

int dar_baixa(int numero_estadia) {
    int i;
    for (i = 0; i < numero_estadia; i++) {
        if (Estadias[i].codigo == numero_estadia) {
            int numero_quarto = Estadias[i].numero_quarto;
            int j;
            for (j = 0; j < numero_quarto; j++) {
                if (Quartos[j].numero == numero_quarto) {
                    strcpy(Quartos[j].status, "desocupado");
                    printf("Estadia número %d finalizada e quarto número %d desocupado.\n", numero_estadia, numero_quarto);
                    return 1;
                }
            }
        }
    }
    printf("Estadia número %d não encontrada.\n", numero_estadia);
    return 0;
}

void relatorio_quartos_disponiveis() {
    int i;
    printf("Relatório de quartos disponíveis:\n");
    for (i = 0; i < numero_quarto; i++) {
        if (Quartos[i].status[0] == 'd') {
            printf("Quarto %d, Capacidade: %d, Valor da Diária: %.2f\n", Quartos[i].numero, Quartos[i].numero_hospedes, Quartos[i].valor_diario);
        }
    }
}

void relatorio_clientes_cadastrados() {
    int i;
    printf("Relatório de clientes cadastrados:\n");
    for (i = 0; i < numero_cliente; i++) {
        printf("Código: %d, Nome: %s, Endereço: %s, Telefone: %d\n", Clientes[i].codigo, Clientes[i].nome, Clientes[i].endereco, Clientes[i].telefone);
    }
}

void relatorio_funcionarios_cadastrados() {
    int i;
    printf("Relatório de funcionários cadastrados:\n");
    for (i = 0; i < numero_funcionario; i++) {
        printf("Código: %d, Nome: %s, Telefone: %d, Cargo: %s, Salário: %.2f\n", Funcionarios[i].codigo, Funcionarios[i].nome, Funcionarios[i].telefone, Funcionarios[i].cargo, Funcionarios[i].salario);
    }
}

void relatorio_estadias_cadastradas() {
    int i;
    printf("Relatório de estadias cadastradas:\n");
    for (i = 0; i < numero_estadia; i++) {
        printf("Código: %d, Data de Entrada: %s, Data de Saída: %s, Código do Cliente: %d, Número do Quarto: %d\n", Estadias[i].codigo, Estadias[i].data_entrada, Estadias[i].data_saida, Estadias[i].codigo_cliente, Estadias[i].numero_quarto);
    }
}

int main() {
    setlocale(LC_ALL, "Portuguese");

    int opcao;
    int codigo, telefone, numero, numero_hospedes;
    char nome[100], endereco[200], cargo[100];
    float salario, valor_diario;
    char data_entrada[100], data_saida[100];

    do {
        printf("\n1 - Cadastrar Cliente\n");
        printf("2 - Cadastrar Funcionário\n");
        printf("3 - Cadastrar Quarto\n");
        printf("4 - Cadastrar Estadia\n");
        printf("5 - Dar Baixa em Estadia\n");
        printf("6 - Relatório de Quartos Disponíveis\n");
        printf("7 - Relatório de Clientes Cadastrados\n");
        printf("8 - Relatório de Funcionários Cadastrados\n");
        printf("9 - Relatório de Estadias Cadastradas\n");
        printf("10 - Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                printf("Código: ");
                scanf("%d", &codigo);
                printf("Nome: ");
                scanf(" %[^\n]s", nome);
                printf("Endereço: ");
                scanf(" %[^\n]s", endereco);
                printf("Telefone: ");
                scanf("%d", &telefone);
                cadastro_de_cliente(codigo, nome, endereco, telefone);
                break;
            case 2:
                printf("Código: ");
                scanf("%d", &codigo);
                printf("Nome: ");
                scanf(" %[^\n]s", nome);
                printf("Telefone: ");
                scanf("%d", &telefone);
                printf("Cargo: ");
                scanf(" %[^\n]s", cargo);
                printf("Salário: ");
                scanf("%f", &salario);
                cadastro_de_funcionario(codigo, nome, telefone, cargo, salario);
                break;
            case 3:
                printf("Número do Quarto: ");
                scanf("%d", &numero);
                printf("Capacidade de Hóspedes: ");
                scanf("%d", &numero_hospedes);
                printf("Valor da Diária: ");
                scanf("%f", &valor_diario);
                cadastrar_quarto(numero, numero_hospedes, valor_diario);
                break;
            case 4:
                printf("Código do Cliente: ");
                scanf("%d", &codigo);
                printf("Número de Hóspedes: ");
                scanf("%d", &numero_hospedes);
                printf("Data de Entrada (dd/mm/yyyy): ");
                scanf("%s", data_entrada);
                printf("Data de Saída (dd/mm/yyyy): ");
                scanf("%s", data_saida);
                cadastrar_estadia(codigo, numero_hospedes, data_entrada, data_saida);
                break;
            case 5:
                printf("Número da Estadia: ");
                scanf("%d", &numero);
                dar_baixa(numero);
                break;
            case 6:
                relatorio_quartos_disponiveis();
                break;
            case 7:
                relatorio_clientes_cadastrados();
                break;
            case 8:
                relatorio_funcionarios_cadastrados();
                break;
            case 9:
                relatorio_estadias_cadastradas();
                break;
            case 10:
                printf("Saindo...\n");
                break;
            default:
                printf("Opção inválida!\n");
        }
    } while (opcao != 10);

    return 0;
}
