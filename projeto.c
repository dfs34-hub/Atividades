#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

#define MAX_ENTRADAS 100
#define MAX_USUARIOS 20

typedef struct{
    char nome[50];
    char lote[30];
    char codigo[30];
    int qtd_fardos;
    int qtd_por_fardos;
    char validade[11];
    float preco_fardo;
    float desconto;
    char data_limite_desconto[11];
} LoteProduto;

typedef struct{
    char chave[50];
    LoteProduto *lote_ref;
} EntradaDicionario;

typedef struct{
    EntradaDicionario entradas[MAX_ENTRADAS];
    int total;
} DicionarioEstoque;

typedef struct{
    int id;
    char usuario[30];
    char senha[30];
    int cargos;
} Usuario;

typedef struct{
    Usuario lista[MAX_USUARIOS];
    int total;
} SistemaUsuarios;

void salvar_usuarios(SistemaUsuarios *su);
void carregar_usuarios(SistemaUsuarios *su);
void salvar_estoque(DicionarioEstoque *dict);
void carregar_estoque(DicionarioEstoque *dict);
void formatar_sistema(SistemaUsuarios *su, DicionarioEstoque *dict);

void remover_quebra_linha(char *str){
    str[strcspn(str, "\n")] = 0;
}

void limpar_buffer(){
    int c;
    while((c = getchar()) != '\n' && c != EOF);
}

void inicializar_usuarios(SistemaUsuarios *su){
    su->total = 0;
}

void cadastrar_usuario(SistemaUsuarios *su, int id, const char *usr, const char *pwd, int cargo) {
    if (su->total < MAX_USUARIOS) {
        su->lista[su->total].id = id;
        strcpy(su->lista[su->total].usuario, usr);
        strcpy(su->lista[su->total].senha, pwd);
        su->lista[su->total].cargos = cargo;
        su->total++;
        printf("Usuario '%s' cadastrado com sucesso! Como '%s'\n", usr, (cargo == 1) ? "Gerente" : "Profissional Administrativo Autorizado");
    } else {
        printf("Limite de usuarios atingido!\n");
    }
}

void cadastrar_usuario_via_input(SistemaUsuarios *su){
    char usuario[30], senha[30];
    int id = 0, val;
    int cargo = 1;

    if(su->total > 0){
        printf("\n==================== SELECAO DE CARGO ====================\n");
        printf("\n1 - Gerente\n");
        printf("\n2 - Profissional Administrativo Autorizado\n");
        printf("\nEscolha um cargo: ");
        scanf("%i", &cargo);
        limpar_buffer();

        if(cargo != 1 && cargo != 2){
            printf("\nCargo Invalido! Definindo cargo para: Profissional Administrativo Autorizado por padrao\n");
            cargo = 2;
        }
    }
    else{
        printf("\nPrimeiro login do sistema.\n");
        printf("\nDefinindo cargo para: Gerente por padrao\n");
        cargo = 1;
    }

    do{
        val = 1;
        printf("\n================ CADASTRO DE NOVO USUARIO =================\n");
        
        printf("\nDigite seu nome de usuario: ");
        fgets(usuario, sizeof(usuario), stdin);
        remover_quebra_linha(usuario);
        
        if(strlen(usuario) < 5){
            printf("\nUsuario muito curto (MINIMO DE 5 CARACTERES)\n");
            val = 0;
            continue;
        }
        
        printf("\nDigite sua senha: ");
        fgets(senha, sizeof(senha), stdin);
        remover_quebra_linha(senha);
        
        if(strlen(senha) < 8){
            printf("\nSenha muito curta (MINIMO DE 8 CARACTERES)\n");
            val = 0;
            continue;
        }
        
        printf("\nDigite seu numero de indentificacao - ID (exatamente 9 digitos): ");
        scanf("%d", &id);
        limpar_buffer();
        
        int digitos = (id == 0) ? 1: (int)log10(abs(id)) + 1;

        if(digitos > 9 || digitos < 9){
            printf("\nID muito curto ou longo (TEM QUE TER EXATAMENTE DE 9 DIGITOS)\n");
            val = 0;
            continue;
        }
    } while(!val);
    cadastrar_usuario(su, id, usuario, senha, cargo);
    printf("\nUsuario cadastrado com sucesso!\n");
}

void listar_usuarios(SistemaUsuarios *su){
    printf("\n===========================================================\n");
    printf("\n                   USUARIOS CADASTRADOS                    \n");
    printf("\n===========================================================\n");

    if(su->total == 0){
        printf("\nNenhum usuario cadastrado.\n");
        return;
    }

    for(int i = 0; i < su->total; i++){
        printf("  ID: %-4d | Usuario: %s\n", su->lista[i].id, su->lista[i].usuario);
    }

    printf("\n=======================================\n");
    printf("\n         Total de Usuarios: %i         \n", su->total);
    printf("\n=======================================\n");
}

void alterar_cargo_via_input(SistemaUsuarios *su){
    int id_alvo;
    printf("\nDigite o ID do usuario para alteracao de cargo: ");
    scanf("%i", &id_alvo);
    limpar_buffer();

    int idx = -1;
    for(int i = 0; i < su->total; i++){
        if(su->lista[i].id == id_alvo){
            idx = i;
            break;
        }
    }

    if(idx == -1){
        printf("\nERROR: Usuario com ID '%i' nao encontrado.\n", id_alvo);
        return;
    }

    if(su->lista[idx].cargos == 1){
        int total_gerentes = 0;
        for(int i = 0; i < su->total; i++){
            if(su->lista[i].cargos == 1){
                total_gerentes++;
            }
        }
        if(total_gerentes <= 1){
            printf("\nNEGADO: Usuario %s e o unico gerente cadastrado", su->lista[idx].usuario);
            printf("\nCadastre um novo gerente ou mude o cargo de outro usuario para fazer a alteracao\n");
            return;
        }
    }

    printf("\nCargo atual de '%s': %s\n", su->lista[idx].usuario, (su->lista[idx].cargos == 1) ? "Gerente" : "Profissional Administrativo Autorizado");
    printf("Escolha o novo cargo:\n");
    printf("1 - Gerente\n");
    printf("2 - Profissional Administrativo Autorizado\n");
    printf("Opcao: ");
    
    int novo_cargo;
    scanf("%d", &novo_cargo);
    limpar_buffer();

    if (novo_cargo == 1 || novo_cargo == 2) {
        su->lista[idx].cargos = novo_cargo;
        printf("\n[SUCESSO] Cargo do usuario '%s' alterado para [%s]!\n", su->lista[idx].usuario, (novo_cargo == 1) ? "Gerente" : "Profissional Administrativo Autorizado");
        salvar_usuarios(su);
    } else {
        printf("\nERROR: Opcao de cargo invalida!\n");
    }
}

void excluir_usuario_via_input(SistemaUsuarios *su){
    if(su->total <= 1){
        printf("\nERROR: Nao e possivel excluir o unico usuario do sistema!\n");
        return;
    }

    int id_alvo;
    printf("\nDigite o ID do usuario a ser excluido: ");
    scanf("%i", &id_alvo);
    limpar_buffer();

    int indice_encontrado = -1;

    for(int i = 0; i < su->total; i++){
        if(su->lista[i].id == id_alvo){
            indice_encontrado = i;
            break;
        }
    }
    if(indice_encontrado == -1){
        printf("\nERROR: Usuario com ID %i nao encontrado.\n", id_alvo);
        return;
    }

    if (su->lista[indice_encontrado].cargos == 1) {
        int total_gerentes = 0;
        for (int i = 0; i < su->total; i++) {
            if (su->lista[i].cargos == 1) {
                total_gerentes++;
            }
        }
        if (total_gerentes <= 1) {
            printf("\nNEGADO: O usuario '%s' e o UNICO GERENTE do sistema!\n", su->lista[indice_encontrado].usuario);
            printf("Cadastre outro Gerente antes de excluir este usuario.\n");
            return;
        }
    }

    printf("\nSUCESSO: Usuario '%s' (ID: %i) removido!\n",su->lista[indice_encontrado].usuario, id_alvo);
    for(int i = indice_encontrado; i < su->total - 1; i++){
        su->lista[i] = su->lista[i + 1];
    }
    su->total--;
    salvar_usuarios(su);
}

int fazer_login(SistemaUsuarios *su){
    char login_input[30];
    char senha_input[30];
    int tentativas = 3;

    while (tentativas > 0){
        printf("\n===========================================================\n");
        printf("\n                 AUTENTICACAO DE USUARIO                \n");
        printf("\n===========================================================\n");
        printf("\nUsuario: ");
        fgets(login_input, sizeof(login_input), stdin);
        remover_quebra_linha(login_input);

        printf("\nSenha: ");
        fgets(senha_input, sizeof(senha_input), stdin);
        remover_quebra_linha(senha_input);

        for(int i = 0; i < su->total; i++){
            if (strcmp(su->lista[i].usuario, login_input)== 0 && strcmp(su->lista[i].senha, senha_input) == 0){

                printf("\n>>> Login efetuado com sucesso! Bem-vindo(a), %s! <<<\n\n", su->lista[i].usuario);
                return su->lista[i].cargos;
            }
        }
        tentativas--;
        printf("\nERROR: Usuario ou senha incorretos! Tentativas restantes %i\n", tentativas);
    }

    printf("\nACESSO NEGADO: Numero maximo de tentativas excedido.\n");
    return 0;
}

void inicializar(DicionarioEstoque *dict){
    dict->total = 0;
}

void vincular_chave(DicionarioEstoque *dict, const char *chave, LoteProduto *lote){
    if(dict->total < MAX_ENTRADAS){
        strcpy(dict->entradas[dict->total].chave, chave);
        dict->entradas[dict->total].lote_ref = lote;
        dict->total++;
    }
    else{
        printf("\nDicionario Cheio!");
    }
}

void cadastrar_lote(DicionarioEstoque *dict, const char *codigo, const char *nome, const char *lote, int fardos, int por_fardo, const char *validade, float preco_fardo){

    LoteProduto *Novo_Lote = (LoteProduto*) malloc(sizeof(LoteProduto));

    strcpy(Novo_Lote->nome, nome);
    strcpy(Novo_Lote->lote, lote);
    strcpy(Novo_Lote->validade, validade);
    strcpy(Novo_Lote->codigo, codigo);
    Novo_Lote->qtd_fardos = fardos;
    Novo_Lote->qtd_por_fardos = por_fardo;
    Novo_Lote->preco_fardo = preco_fardo;

    vincular_chave(dict, codigo, Novo_Lote);
    vincular_chave(dict, lote, Novo_Lote);
}

void consultar_estoque(DicionarioEstoque *dict, const char *chave){
    int encontrados = 0;
    int total_de_fardos_acumulados = 0;
    LoteProduto *lotes_exibidos[MAX_ENTRADAS];

    printf("\n=======================================================\n");
    printf("\n               CONSULTA DE ESTOQUE: '%s'               \n", chave);
    printf("\n=======================================================\n");

    for(int i = 0; i < dict->total; i++){
        if(strcmp(dict->entradas[i].chave, chave) == 0){
            LoteProduto *l = dict->entradas[i].lote_ref;

            int ja_exibido = 0;
            for(int j = 0; j<encontrados; j++){
                if(lotes_exibidos[j] == l){
                    ja_exibido = 1;
                    break;
                }
            }
            if (ja_exibido) continue;

            lotes_exibidos[encontrados] = l;
            encontrados++;
            total_de_fardos_acumulados += l->qtd_fardos;

            printf("\n[ LOTE %i]\n", encontrados);
            printf("  Produto:            %s\n", l->nome);
            printf("  Codigo:             %s\n", l->codigo);
            printf("  Numero do Lote:         %s\n", l->lote);
            printf("  Validade:           %s\n", l->validade);
            printf("  Fardos no Lote:     %d un\n", l->qtd_fardos);
            printf("  Unidades p/ Fardo:  %d un\n", l->qtd_por_fardos);
            printf("  Preco p/ Fardo:     R$ %.2f\n", l->preco_fardo);
            printf("--------------------------------------------------\n");
        }
    }
    if(encontrados == 0){
        printf("Nenhum produto encontrado com a chave '%s'.\n", chave);
    }
    else{
        printf("\n=>RESUMO: %i lote(s) encontrado(s) |  TOTAL EM ESTOQUE: %i fardos\n", encontrados, total_de_fardos_acumulados);
    }
    printf("\n=======================================================\n");
}

void cadastrar_lote_via_input(DicionarioEstoque *dict){
    char codigo[30], nome[50], lote[30], validade[11];
    int fardos, por_fardo;
    float preco;

    printf("\n================== CADASTRO DE NOVO LOTE ==================\n");

    printf("\nDigite o codigo do produto (SKU/EAN): ");
    fgets(codigo, sizeof(codigo), stdin);
    remover_quebra_linha(codigo);
    
    printf("\nDigite o nome do produto: ");
    fgets(nome, sizeof(nome), stdin);
    remover_quebra_linha(nome);
    
    printf("\nDigite o indentificador do lote: ");
    fgets(lote, sizeof(lote), stdin);
    remover_quebra_linha(lote);
    
    printf("\nDigite a validade do produto (ex: 31/12/2026): ");
    fgets(validade, sizeof(validade), stdin);
    remover_quebra_linha(validade);
    
    printf("\nDigite a quantidade de fardos: ");
    scanf("%i", &fardos);

    printf("\nDigite a quantidade de itens em 1 fardo: ");
    scanf("%i", &por_fardo);

    printf("\nDigite o preco de 1 fardo: R$ ");
    scanf("%f", &preco);

    limpar_buffer();
    
    cadastrar_lote(dict, codigo, nome, lote, fardos, por_fardo, validade, preco);

    printf("\nLote registrado com sucesso no sistema!\n");
}

void remover_lote(DicionarioEstoque *dict, LoteProduto *lote_para_remover){
    int i = 0;
    while(i<dict->total){
        if(dict->entradas[i].lote_ref == lote_para_remover){
            for(int j = i; j < dict->total - 1; j++){
                dict->entradas[j] = dict->entradas[j + 1];
            }
            dict->total--;
        }
        else{
            i++;
        }
    }
    free(lote_para_remover);

    printf("\nSUCESSO: Lote e suas chave foram removidas do sistema!\n");
}

LoteProduto* buscar_lote_exato(DicionarioEstoque *dict, const char *chave_lote){
    for(int i = 0; i < dict->total; i++){
        if(strcmp(dict->entradas[i].chave, chave_lote) == 0){
            return dict->entradas[i].lote_ref;
        }
    }
    return NULL;
}

void excluir_lote_manual(DicionarioEstoque *dict) {
    char id_lote[30];

    printf("\nDigite o NUMERO DO LOTE a ser excluido completamente: ");
    fgets(id_lote, sizeof(id_lote), stdin);
    remover_quebra_linha(id_lote);

    LoteProduto *lote = buscar_lote_exato(dict, id_lote);

    if (lote == NULL) {
        printf("\nERROR: Lote '%s' nao encontrado.\n", id_lote);
        return;
    }

    remover_lote(dict, lote);
    printf("\nSUCESSO: O lote '%s' foi excluido permanentemente do sistema!\n", id_lote);
}

void dar_baixa_fardo(DicionarioEstoque *dict){
    char id_lote[30];
    int qtd_saida;

    printf("\nDigite o NUMERO DO LOTE Especifico para dar baixa: ");
    fgets(id_lote, sizeof(id_lote), stdin);
    remover_quebra_linha(id_lote);

    LoteProduto *lote = buscar_lote_exato(dict, id_lote);

    if(lote == NULL){
        printf("\nERROR: Lote '%s' nao encontrado.\n", id_lote);
    }

    printf("\nQuantidade de fardos a retirar do lote %s (Disponivel %i)", lote->lote, lote->qtd_fardos);
    scanf("%i", &qtd_saida);
    limpar_buffer();

    if(qtd_saida <= 0 || qtd_saida > lote->qtd_fardos){
        printf("\nERROR: Quantidade invalida.\n");
        return;
    }

    lote->qtd_fardos -= qtd_saida;
    printf("\nBAIXA EFETUADA: retiramos %i fardos. Restam %i no lote %s.\n",qtd_saida, lote->qtd_fardos, lote->lote);

    if(lote->qtd_fardos == 0){
        printf("\nAVISO: Lote zerado! Removendo registro do sistema...\n");
        remover_lote(dict, lote);
    }
}



void verificar_alertas_de_validade(DicionarioEstoque *dict, int dias_limites){
    time_t agora = time(NULL);
    int alertas_encontrados = 0;
    LoteProduto *lotes_verificados[MAX_ENTRADAS];

    printf("\n==========================================================\n");
    printf("\n             ALERTAS DE VALIDADE / VENCIMENTO             \n");
    printf("\n==========================================================\n");

    for(int i = 0; i < dict->total; i++){
        LoteProduto *l = dict->entradas[i].lote_ref;

        int ja_verificado = 0;
        for(int k = 0; k < alertas_encontrados; k++){
            if(lotes_verificados[k] == l){
                ja_verificado = 1;
                break;
            }
        }
        if(ja_verificado) continue;

        int dia, mes, ano;
        if(sscanf(l->validade, "%d/%d/%d", &dia, &mes, &ano) == 3){
            struct tm tm_lote = {0};
            tm_lote.tm_mday = dia;
            tm_lote.tm_mon = mes - 1;
            tm_lote.tm_year = ano - 1900;
            tm_lote.tm_isdst = -1;

            time_t t_lote = mktime(&tm_lote);

            double segundos = difftime(t_lote, agora);
            int dias_restantes = (int)(segundos / (60*60*24));

            if(dias_restantes <= dias_limites){
                lotes_verificados[alertas_encontrados++] = l;

                if(dias_restantes < 0){
                    printf("\nAVISO CRITICO: Lote '%s' (%s) Vencido há %d dia(s)! VALIDADE: %s\n",l->lote, l->nome, abs(dias_restantes), l->validade);
                }
                else if(dias_restantes == 0){
                    printf("\nATENCAO: Lote '%s' (%s) VENCE HOJE! VALIDADE: %s\n", l->lote, l->nome, l->validade);
                }
                else{
                    printf("\nATENCAO: Lote '%s' (%s) VENCE EM %d DIAS! VALIDADE: %s\n", l->lote, l->nome, dias_restantes, l->validade);
                }
            }
        }
    }
    if(alertas_encontrados == 0){
        printf("\nNenhuma pendencia de validade nos proximos %d dias.\n", dias_limites);
    }
}

int desconto_esta_valido(const char *data_limite) {
    if (strlen(data_limite) == 0) return 1; // Sem limite de data cadastrado

    time_t agora = time(NULL);
    int dia, mes, ano;

    if (sscanf(data_limite, "%d/%d/%d", &dia, &mes, &ano) == 3) {
        struct tm tm_limite = {0};
        tm_limite.tm_mday = dia;
        tm_limite.tm_mon = mes - 1;
        tm_limite.tm_year = ano - 1900;
        tm_limite.tm_hour = 23; // Válido até 23:59:59 do dia limite
        tm_limite.tm_min = 59;
        tm_limite.tm_sec = 59;
        tm_limite.tm_isdst = -1;

        time_t t_limite = mktime(&tm_limite);
        return difftime(t_limite, agora) >= 0;
    }
    return 1;
}

void aplicar_desconto_via_input(DicionarioEstoque *dict){
    char chave[50], data_limite[11];
    float porcentagem;

    printf("\n==================== APLICAR DESCONTO ====================\n");
    printf("\nDgite o Codigo do produo ou numero do lote: ");
    fgets(chave, sizeof(chave), stdin);
    remover_quebra_linha(chave);

    int encontrados = 0;
    LoteProduto *lotes_modificados[MAX_ENTRADAS];

    for(int i = 0; i < dict->total; i++){
        if(strcmp(dict->entradas[i].chave, chave) == 0){
            LoteProduto *l = dict->entradas[i].lote_ref;

            int ja_adicionado = 0;
            for(int j = 0; j < ja_adicionado; j++){
                if(lotes_modificados[j] == l){
                    ja_adicionado = 1;
                    break;
                }
            }
            if(!ja_adicionado){
                lotes_modificados[encontrados++] = l;
            }
        }
    }
    if(encontrados==0){
        printf("\nERROR: nenhum lote encontrado com essa chave '%s'.\n", chave);
        return;
    }

    printf("\nDigite a porcentagem do desconto (ex.:10 para 10%%, 0 para remover desconto): ");
    scanf("%f", &porcentagem);
    limpar_buffer();

    if(porcentagem < 0.0f || porcentagem > 100.0F){
        printf("\nERROR: essa porcentagem e invalida.\n");
        return;
    }

    if (porcentagem > 0.0f) {
        printf("Digite a data limite do desconto (ex: 31/12/2026): ");
        fgets(data_limite, sizeof(data_limite), stdin);
        remover_quebra_linha(data_limite);
    } else {
        data_limite[0] = '\0';
    }

    for (int i = 0; i < encontrados; i++) {
        lotes_modificados[i]->desconto = porcentagem;
        strcpy(lotes_modificados[i]->data_limite_desconto, data_limite);
    }

    if (porcentagem > 0.0f) {
        printf("\n[SUCESSO] Desconto de %.1f%% aplicado até %s em %d lote(s)!\n", porcentagem, data_limite, encontrados);
    } else {
        printf("\n[SUCESSO] Desconto removido de %d lote(s)!\n", encontrados);
    }
}

void salvar_usuarios(SistemaUsuarios *su) {
    FILE *f = fopen("usuarios.txt", "w"); 
    if (f == NULL) return;

    for (int i = 0; i < su->total; i++) {
        fprintf(f, "%d;%s;%s;%d\n", su->lista[i].id, su->lista[i].usuario, su->lista[i].senha, su->lista[i].cargos);
    }
    fclose(f);
}

void carregar_usuarios(SistemaUsuarios *su) {
    FILE *f = fopen("usuarios.txt", "r");
    if (f == NULL) return;

    su->total = 0;
    char linha[200];

    while (fgets(linha, sizeof(linha), f) && su->total < MAX_USUARIOS) {
        remover_quebra_linha(linha);
        Usuario u;
        if (sscanf(linha, "%d;%29[^;];%29[^;];%d", &u.id, u.usuario, u.senha, &u.cargos) == 4) {
            su->lista[su->total++] = u;
        }
    }
    fclose(f);
}

void salvar_estoque(DicionarioEstoque *dict) {
    FILE *f = fopen("estoque.txt", "w");
    if (f == NULL){
        return;
    }

    LoteProduto *lotes_salvos[MAX_ENTRADAS];
    int total_salvos = 0;

    for (int i = 0; i < dict->total; i++) {
        LoteProduto *l = dict->entradas[i].lote_ref;

        int ja_salvo = 0;
        for (int k = 0; k < total_salvos; k++) {
            if (lotes_salvos[k] == l) {
                ja_salvo = 1;
                break;
            }
        }

        if (ja_salvo){
            continue;
        }

        lotes_salvos[total_salvos++] = l;

        char data_lim[15];
        if (strlen(l->data_limite_desconto) == 0) {
            strcpy(data_lim, "NENHUMA");
        } else {
            strcpy(data_lim, l->data_limite_desconto);
        }

        fprintf(f, "%s;%s;%s;%d;%d;%s;%.2f;%.2f;%s\n",
                l->codigo, l->nome, l->lote, l->qtd_fardos, l->qtd_por_fardos,
                l->validade, l->preco_fardo, l->desconto, data_lim);
    }
    fclose(f);
}

void carregar_estoque(DicionarioEstoque *dict) {
    FILE *f = fopen("estoque.txt", "r");
    if (f == NULL) return;

    dict->total = 0;
    char linha[256];

    while (fgets(linha, sizeof(linha), f)) {
        remover_quebra_linha(linha);
        char codigo[30], nome[50], lote[30], validade[11], data_lim[11];
        int fardos, por_fardo;
        float preco, desconto;

        if (sscanf(linha, "%29[^;];%49[^;];%29[^;];%d;%d;%10[^;];%f;%f;%10[^\n]",
                   codigo, nome, lote, &fardos, &por_fardo, validade, &preco, &desconto, data_lim) == 9) {

            LoteProduto *Novo_Lote = (LoteProduto*) malloc(sizeof(LoteProduto));
            strcpy(Novo_Lote->codigo, codigo);
            strcpy(Novo_Lote->nome, nome);
            strcpy(Novo_Lote->lote, lote);
            Novo_Lote->qtd_fardos = fardos;
            Novo_Lote->qtd_por_fardos = por_fardo;
            strcpy(Novo_Lote->validade, validade);
            Novo_Lote->preco_fardo = preco;
            Novo_Lote->desconto = desconto;

            if (strcmp(data_lim, "NENHUMA") == 0) {
                Novo_Lote->data_limite_desconto[0] = '\0';
            } else {
                strcpy(Novo_Lote->data_limite_desconto, data_lim);
            }

            vincular_chave(dict, codigo, Novo_Lote);
            vincular_chave(dict, lote, Novo_Lote);
        }
    }
    fclose(f);
}

void formatar_sistema(SistemaUsuarios *su, DicionarioEstoque *dict) {
    char confirmacao;

    printf("\n==========================================================\n");
    printf("\n  [!] ATENCAO: FORMATACAO E LIMPEZA TOTAL DO SISTEMA [!]  \n");
    printf("\n==========================================================\n");
    printf("\n  Esta operacao ira APAGAR PERMANENTEMENTE todos os dados: \n");
    printf("\n    - Todos os lotes e produtos do estoque (estoque.txt)    \n");
    printf("\n  - Todos os usuarios e senhas cadastradas (usuarios.txt)  \n");
    printf("\n-----------------------------------------------------------\n");
    printf("Deseja realmente prosseguir? (S/N): ");
    scanf(" %c", &confirmacao);
    limpar_buffer();

    if (confirmacao == 'S' || confirmacao == 's') {
        remove("usuarios.txt");
        remove("estoque.txt");

        LoteProduto *lotes_liberados[MAX_ENTRADAS];
        int liberados = 0;
        for (int i = 0; i < dict->total; i++) {
            LoteProduto *l = dict->entradas[i].lote_ref;
            int ja_liberado = 0;
            for (int k = 0; k < liberados; k++) {
                if (lotes_liberados[k] == l) {
                    ja_liberado = 1;
                    break;
                }
            }
            if (!ja_liberado) {
                lotes_liberados[liberados++] = l;
                free(l);
            }
        }

        dict->total = 0;
        su->total = 0;

        printf("\n[SUCESSO] Sistema formatado com sucesso!\n");
        printf("Todos os arquivos foram removidos e o sistema foi restaurado ao estado original.\n");
    } else {
        printf("\n[CANCELADO] Operacao de formatacao cancelada com seguranca.\n");
    }
}

void menu_estoque(DicionarioEstoque *estoque){
    int opcao;
    char chave_buscar[50];

    do{
        printf("\n--------------------- MENU DO ESTOQUE ---------------------\n");
        printf("1 - Cadastrar Novo Lote\n");
        printf("2 - Consultar estoque (por codigo ou lote)\n");
        printf("3 - Dar baixa em fardos\n");
        printf("4 - Excluir lote completamente\n");
        printf("5 - Aplicar descontos em produtos\n");
        printf("0 - Sair do submenu\n");
        printf("Escolha uma opcao: ");
        scanf("%i", &opcao);
        limpar_buffer();

        switch(opcao){
            case 1:
                cadastrar_lote_via_input(estoque);
                break;
            case 2:
                printf("\nDigite o Codigo do Produto ou o Numero do Lote: ");
                fgets(chave_buscar, sizeof(chave_buscar), stdin);
                remover_quebra_linha(chave_buscar);
                consultar_estoque(estoque, chave_buscar);
                break;
            case 3:
                dar_baixa_fardo(estoque);
                break;
            case 4:
                excluir_lote_manual(estoque);
                break;
            case 5:
                aplicar_desconto_via_input(estoque);
                break;
            case 0:
                printf("\nEncerrando Programa...");
                break;
            default:
                printf("\nopcao invalida\n");
        }
    } while(opcao != 0);
}

void menu_usuarios(SistemaUsuarios *usuario){
    int opcao;

    printf("\n--------------------- MENU DOS USUARIOS ---------------------\n");
    do{
        printf("1 - Cadastrar Novo Usuario\n");
        printf("2 - Alterar cargo\n");
        printf("3 - Listar usuarios cadastrados\n");
        printf("4 - Excluir usuario por ID\n");
        printf("0 - Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%i", &opcao);
        limpar_buffer();

        switch(opcao){
            case 1:
                cadastrar_usuario_via_input(usuario);
                break;
            case 2:
                alterar_cargo_via_input(usuario);
                break;
            case 3:
                listar_usuarios(usuario);
                break;
            case 4:
                excluir_usuario_via_input(usuario);
                break;
            case 0:
                printf("\nEncerrando Programa...");
                break;
            default:
                printf("\nopcao invalida!\n");
        }
    } while (opcao != 0);
}

int main(){
    SistemaUsuarios usuario;
    inicializar_usuarios(&usuario);
    carregar_usuarios(&usuario);

    DicionarioEstoque estoque;
    inicializar(&estoque);
    carregar_estoque(&estoque);

    if(usuario.total == 0){
        printf("\n===========================================================\n");
        printf("\n           NENHUM USUARIO CADASTRADO NO SISTEMA!           \n");
        printf("\n   Cadastre o primeiro usuario para habilitar o acesso.   \n");
        printf("\n===========================================================\n");
        cadastrar_usuario_via_input(&usuario);
    }

    int cargo_logado = fazer_login(&usuario);

    if(cargo_logado == 0){
        return 0;
    }

    verificar_alertas_de_validade(&estoque, 30);

    int opcao;

    do{
        printf("\n--------------------- MENU DO SISTEMA ---------------------\n");
        printf("1 - Menu do Estoque\n");
        if(cargo_logado == 1){
            printf("2 - Menu dos usuarios\n");
            printf("3 - Formatar sistema / Apagar arquivos\n");
        }
        printf("0 - Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%i", &opcao);
        limpar_buffer();

        switch(opcao){
            case 1:
                menu_estoque(&estoque);
                break;
            case 2:
                if(cargo_logado == 1){
                    menu_usuarios(&usuario);
                }
                else{
                    printf("\nNEGADO: Apenas usuarios do cargo gerente podem acessar esse menu!");
                }
                break;
            case 3:
                if (cargo_logado == 1) {
                    formatar_sistema(&usuario, &estoque);
                    if (usuario.total == 0) {
                        opcao = 0;
                    }
                } else {
                    printf("\n[ACESSO NEGADO] Apenas GERENTES podem formatar o sistema!\n");
                }
                break;
            case 0:
                salvar_usuarios(&usuario);
                salvar_estoque(&estoque);
                printf("\nEncerrando Programa...");
                break;
            default:
                printf("\nopcao invalida!\n");
        }
    }
    while (opcao != 0);
    return 0;
}