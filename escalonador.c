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

typedef struct aux1 {
    int impressora;
    int scanner;
    int modem;
    int CD;
    int memoriaPrincipal;
    int quantum;
} hardware;


// variaveis globais
int a =0;
//


// funções e explicações

void inicializarFila(fila *fila);

componentesProcessos *criarElementoFila( int id1, int arrival_time1, int priority1, int processor_time1, int Mbytes1, int impressoras1, int scanners1, int modems1, int cds1 );

void inserirFila(fila *fila, componentesProcessos *novo);

componentesProcessos * retirarFila(fila * q);

void trocarElementoFila(fila* a1, fila *a2);

void entradaDeProcesso();

void separacaoProcessos(fila *queueEntradaProcessos, fila *ProcessosTempoReal, fila *ProcessosUsuario);

void politicasProcessos(fila *ProcessosTempoReal, fila *ProcessosUsuario);

hardware * Sistema();

void politicafeedback(fila *feedbackQ0, hardware * sistema);

void exibirFila(fila *fila);




// fim das funções e suas explicações

hardware * Sistema(){
    hardware * a1 = (hardware*)malloc(sizeof(hardware));
    a1->CD =2;
    a1->impressora =2;
    a1->memoriaPrincipal = 1024;
    a1->modem =1;
    a1->scanner =1;
    a1->quantum =2;

    return a1;
}

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

void trocarElementoFila(fila* a1, fila *a2){
  componentesProcessos * elemento = retirarFila(a1);
  inserirFila(a2, elemento);
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
    
    politicasProcessos(ProcessosTempoReal,ProcessosUsuario);
}

void politicasProcessos(fila *ProcessosTempoReal, fila *ProcessosUsuario){
    
   hardware *sistema = Sistema();
   politicafeedback(ProcessosUsuario, sistema);
}



void politicafeedback(fila *feedbackQ0, hardware * sistema){
    
    fila feedbackQ1;
    fila feedbackQ2;

    inicializarFila(&feedbackQ1);
    inicializarFila(&feedbackQ2);

    componentesProcessos * processo;

    int i =0;

    while( feedbackQ0->inicio != NULL || feedbackQ1.inicio != NULL || feedbackQ2.inicio != NULL ){
        if(feedbackQ0->inicio!= NULL ){
            trocarElementoFila(feedbackQ0,&feedbackQ1);
            printf("trocou da fila 0 para fila 1\n");
        }

        if(feedbackQ1.inicio!= NULL ){
            trocarElementoFila(&feedbackQ1,&feedbackQ2);
            printf("trocou da fila 1 para fila 2\n");
        }

        if(feedbackQ2.inicio!= NULL ){
            trocarElementoFila(&feedbackQ2,&feedbackQ2);
            printf("trocou da fila 2 para fila 2\n");
        } 
    }
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

