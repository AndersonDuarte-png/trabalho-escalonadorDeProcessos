#include<stdio.h>
#include<stdlib.h>
#include<time.h>

typedef struct aux {
    char id;
    int arrival_time;
    int priority;
    int processor_time;
    int Mbytes;
    int impressoras;
    int scanners;
    int modems;
    int cds;
    struct aux * prox;
} componentesProcessos;

typedef struct {
    componentesProcessos* inicio;
    componentesProcessos* fim;
} fila;


// variaveis globais

int a =0;

int memoriaPrincipal = 1024;

int quantum = 128;

//


// funções e explicações

void inicializarFila(fila *fila);

componentesProcessos *criarElementoFila( int id1, int arrival_time1, int priority1, int processor_time1, int Mbytes1, int impressoras1, int scanners1, int modems1, int cds1 );

void inserirFila(fila *fila, componentesProcessos *novo);

void entradaDeProcesso();

void separacaoProcessos(fila *queueEntradaProcessos, fila *ProcessosTempoReal, fila *ProcessosUsuario);

void memoria_principal(fila *ProcessosTempoReal, fila *ProcessosUsuario);

void politicafeedback(fila *feedbackQ0);

void exibirFila(fila *fila);

componentesProcessos * retirarFila(fila * q);
// fim das funções e suas explicações



void inicializarFila(fila *fila){
    fila->fim = NULL;
    fila->inicio = NULL;
}


componentesProcessos *criarElementoFila( int id1, int arrival_time1, int priority1, int processor_time1, int Mbytes1, int impressoras1, int scanners1, int modems1, int cds1 ) {

    componentesProcessos *novo = (componentesProcessos*) malloc(sizeof(componentesProcessos));
    novo->id = id1;
    novo->arrival_time = arrival_time1;
    novo->priority = priority1;
    novo->processor_time = processor_time1;
    novo->Mbytes = Mbytes1;
    novo->impressoras= impressoras1;
    novo->scanners = scanners1;
    novo->modems = modems1;
    novo->cds = cds1;
    novo->prox = NULL;
    return novo;
}


void inserirFila(fila *fila, componentesProcessos *novo) {
    if(fila->fim == NULL) {
        fila->inicio = novo;
        fila->fim = novo;
    }
    else {
        fila->fim->prox = novo;
        fila->fim = novo;
    }
}

componentesProcessos * retirarFila(fila * q){
    
    if(q->inicio == NULL)
      return NULL;

    componentesProcessos * p = q->inicio;

    if(q->inicio == q->fim)
      q->fim = NULL;

    q->inicio = p->prox;

    p->prox = NULL;

    return p;

}


void entradaDeProcesso(fila *queueEntradaProcessos ){



    FILE *processo;
    processo = fopen("lista-de-processos.txt", "r");
    

    if(processo == NULL )
        printf("erro\n");

    else{
        char residuos[7];
        componentesProcessos a1;
        while(fscanf(processo, "%d %c  %d  %c %d %c %d %c %d %c %d %c %d %c %d",&a1.arrival_time, &residuos[0], &a1.priority, &residuos[1], &a1.processor_time, &residuos[2],&a1.Mbytes, &residuos[3], &a1.impressoras,  &residuos[4], &a1.scanners, &residuos[5], &a1.modems,  &residuos[6], &a1.cds)!=EOF){

            componentesProcessos *newProcesso;
            newProcesso = criarElementoFila(a, a1.arrival_time, a1.priority, a1.processor_time, a1.Mbytes, a1.impressoras, a1.scanners, a1.modems, a1.cds);
            a++;
            inserirFila(queueEntradaProcessos, newProcesso );
        }   
    }
    fclose(processo);
}


void separacaoProcessos(fila *queueEntradaProcessos, fila *ProcessosTempoReal, fila *ProcessosUsuario){
    componentesProcessos *passador = (componentesProcessos*)malloc(sizeof(componentesProcessos));
    componentesProcessos *aux = (componentesProcessos*)malloc(sizeof(componentesProcessos));
    
    passador = queueEntradaProcessos->inicio;
    while(passador){
        
        if(passador->priority == 0){
            passador = passador->prox;
            aux= retirarFila(queueEntradaProcessos);
            inserirFila(ProcessosTempoReal, aux);
        }
        else{
            passador = passador->prox;
            aux= retirarFila(queueEntradaProcessos);
            inserirFila(ProcessosUsuario, aux);
        } 
    }

    memoria_principal(ProcessosTempoReal,ProcessosUsuario);
}

void memoria_principal(fila *ProcessosTempoReal, fila *ProcessosUsuario){
    
   politicafeedback(ProcessosUsuario);
}



void politicafeedback(fila *feedbackQ0){
    fila feedbackQ1;
    fila feedbackQ2;

    inicializarFila(&feedbackQ1);
    inicializarFila(&feedbackQ2);

    componentesProcessos * processo = (componentesProcessos*)malloc(sizeof(componentesProcessos));
    componentesProcessos *aux1 = (componentesProcessos*)malloc(sizeof(componentesProcessos));
    

    int aux;
    int i;

    exibirFila(feedbackQ0);
    processo = feedbackQ0->inicio;

    while( feedbackQ0->inicio != NULL ){
        
        printf("#####\n");

        if(processo->Mbytes != 0){
            aux = processo->Mbytes*2;
            for(i =1;i<2;i++){
                memoriaPrincipal -=processo->Mbytes;
                processo->Mbytes -= quantum;
            }
        }
        else{
            aux1 = processo;
            processo = processo->prox;
            free(aux1);
        }
        memoriaPrincipal+=aux;
        processo = processo->prox; 
    }

    exibirFila(&feedbackQ1);

    /*while( feedbackQ1.inicio != NULL ){
        processo = feedbackQ1.inicio;


        if(processo->Mbytes != 0){
            aux = processo->Mbytes*2;
            for(i =1;i<2;i++){
                memoriaPrincipal -=processo->Mbytes;
                processo->Mbytes -= quantum;
            }
        }
        else{
            aux1 = processo;
            processo = processo->prox;
            free(aux1);
        }
        memoriaPrincipal+=aux;
        processo = processo->prox;
        aux1 = retirarFila(&feedbackQ1);
        inserirFila(&feedbackQ2,aux1);
    }*/

    

   /* while( feedbackQ2.inicio != NULL ){ arrumar essa parte
        processo = feedbackQ1.inicio;


        if(processo->Mbytes != 0){
            aux = processo->Mbytes*2;
            for(i =1;i<2;i++){
                memoriaPrincipal -=processo->Mbytes;
                processo->Mbytes -= quantum;
            }
        }
        else{
            aux1 = processo;
            processo = processo->prox;
            free(aux1);
        }
        memoriaPrincipal+=aux;
        processo = processo->prox;
        aux1 = retirarFila(&feedbackQ2);
        inserirFila(&feedbackQ2,aux1);
    }*/

    printf("%d\n", memoriaPrincipal);
}


int main(){
    fila queueEntradaProcessos;
    inicializarFila(&queueEntradaProcessos);
    entradaDeProcesso(&queueEntradaProcessos);

    // filas com niveis de prioridade
    fila ProcessosTempoReal;
    fila ProcessosUsuario;
    inicializarFila(&ProcessosTempoReal);
    inicializarFila(&ProcessosUsuario);

    separacaoProcessos(&queueEntradaProcessos,&ProcessosTempoReal,&ProcessosUsuario);
    return 0;
}


void exibirFila(fila *fila){

    componentesProcessos *passador;
    passador = fila->inicio;
    
    while(passador){
        printf("o id do processo atual é: %d\n", passador->id);
        passador = passador->prox;
    }
}

