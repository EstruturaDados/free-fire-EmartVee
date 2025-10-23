#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_ITENS 10

typedef struct {
    char nome[30];
    char tipo[20];
    int  quantidade;
} Item;

void limparEntrada(void){
    int c; while((c=getchar())!='\n' && c!=EOF){}
}
void lerString(const char *prompt, char *dest, size_t tam){
    printf("%s", prompt);
    if(!fgets(dest, (int)tam, stdin)){ dest[0]='\0'; return; }
    size_t len=strlen(dest);
    if(len>0 && dest[len-1]=='\n') dest[len-1]='\0'; else limparEntrada();
}
int lerInteiroPositivo(const char *prompt){
    int v;
    for(;;){
        printf("%s", prompt);
        if(scanf("%d",&v)!=1){ printf("Entrada invalida. Digite um numero inteiro.\n"); limparEntrada(); continue; }
        limparEntrada();
        if(v<=0){ printf("A quantidade deve ser maior que zero.\n"); continue; }
        return v;
    }
}
int nomesIguaisIgnoreCase(const char *a,const char *b){
    while(*a && *b){ if(tolower((unsigned char)*a)!=tolower((unsigned char)*b)) return 0; a++; b++; }
    return *a=='\0' && *b=='\0';
}

int inserirItem(Item m[], int *q){
    if(*q>=MAX_ITENS){ printf("Mochila cheia! Capacidade maxima de %d itens.\n", MAX_ITENS); return 0; }
    Item n;
    lerString("Nome do item: ", n.nome, sizeof(n.nome));
    if(n.nome[0]=='\0'){ printf("Nome nao pode ser vazio.\n"); return 0; }
    for(int i=0;i<*q;i++) if(nomesIguaisIgnoreCase(m[i].nome,n.nome)){ printf("Ja existe um item com esse nome.\n"); return 0; }
    lerString("Tipo do item (arma/municao/cura/ferramenta): ", n.tipo, sizeof(n.tipo));
    if(n.tipo[0]=='\0'){ printf("Tipo nao pode ser vazio.\n"); return 0; }
    n.quantidade = lerInteiroPositivo("Quantidade: ");
    m[*q]=n; (*q)++; printf("Item cadastrado com sucesso!\n"); return 1;
}
int buscarItemSequencial(const Item m[], int q, const char *nome){
    for(int i=0;i<q;i++) if(nomesIguaisIgnoreCase(m[i].nome,nome)) return i;
    return -1;
}
int removerItem(Item m[], int *q){
    if(*q==0){ printf("Nao ha itens para remover.\n"); return 0; }
    char alvo[30]; lerString("Digite o nome do item a remover: ", alvo, sizeof(alvo));
    int idx = buscarItemSequencial(m, *q, alvo);
    if(idx==-1){ printf("Item '%s' nao encontrado.\n", alvo); return 0; }
    for(int i=idx;i<*q-1;i++) m[i]=m[i+1];
    (*q)--; printf("Item removido com sucesso.\n"); return 1;
}
void listarItens(const Item m[], int q){
    printf("\n================= INVENTARIO ATUAL (%d/%d) =================\n", q, MAX_ITENS);
    if(q==0){ printf("(vazio) - Dica: use a opcao 1 para cadastrar itens.\n"); printf("===========================================================\n\n"); return; }
    printf("%-3s | %-28s | %-18s | %-10s\n", "#", "Nome", "Tipo", "Quantidade");
    printf("----+------------------------------+--------------------+------------\n");
    for(int i=0;i<q;i++) printf("%-3d | %-28s | %-18s | %-10d\n", i+1, m[i].nome, m[i].tipo, m[i].quantidade);
    printf("===========================================================\n\n");
}

int main(void){
    Item mochila[MAX_ITENS]={0};
    int qtde=0, opcao;
    printf("=============================================\n");
    printf(" MOCHILA DE LOOT - Nivel Inicial do Jogador \n");
    printf("=============================================\n");
    listarItens(mochila, qtde);
    do{
        printf("Escolha uma acao:\n");
        printf("  1) Cadastrar item\n");
        printf("  2) Remover item\n");
        printf("  3) Buscar item por nome\n");
        printf("  4) Listar itens\n");
        printf("  0) Sair\n");
        printf("Opcao: ");
        if(scanf("%d",&opcao)!=1){ printf("Entrada invalida. Digite um numero.\n"); limparEntrada(); continue; }
        limparEntrada();
        switch(opcao){
            case 1: inserirItem(mochila,&qtde); listarItens(mochila,qtde); break;
            case 2: removerItem(mochila,&qtde); listarItens(mochila,qtde); break;
            case 3: {
                char alvo[30]; lerString("Nome do item a buscar: ", alvo, sizeof(alvo));
                int i=buscarItemSequencial(mochila,qtde,alvo);
                if(i==-1) printf("Item '%s' nao foi encontrado.\n", alvo);
                else { printf("Encontrado!\n  - Nome: %s\n  - Tipo: %s\n  - Quantidade: %d\n", mochila[i].nome, mochila[i].tipo, mochila[i].quantidade); }
                listarItens(mochila,qtde); break;
            }
            case 4: listarItens(mochila,qtde); break;
            case 0: printf("Saindo... Boa sorte!\n"); break;
            default: printf("Opcao invalida.\n");
        }
    }while(opcao!=0);
    return 0;
}