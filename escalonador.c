#include<stdio.h>
#include<stdlib.h>

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
// inicia a fila como nula

componentesProcessos *criarElementoFila( int id1, int arrival_time1, int priority1, int processor_time1, int Mbytes1, int impressoras1, int scanners1, int modems1, int cds1 );
// cria variavel de variaveis para encaixar na fila

void inserirFila(fila *fila, componentesProcessos *novo);
// insere um elemento (componentesProcessos) na fila

componentesProcessos * retirarFila(fila * q);
// retira um elemento para a fila e envia ele para uma variavel

void trocarElementoFila(fila* a1, fila *a2);
// recebe 2 filas e retira o primeiro elemento da primeira fila e coloca no final da sugunda

void entradaDeProcesso();
// cria a FE (fila de entrada), le o txt e cria os processos aparti dele, coloca os processos criados na FE em ordem de chegada

void separacaoProcessos(fila *queueEntradaProcessos);
// recebe a FE,  cria duas filas uma para processos de prioridade 0 (FTR) e outra pro resto(FU), e separa os processos apartir ques estão na FE

void politicasProcessos(fila *ProcessosTempoReal, fila *ProcessosUsuario);
//recebe a as filas de processos FTR e FU, e as envia cada uma para a sua politica, FTR para politica First ComeFirst Served e FU politica Feedback

hardware * Sistema();
// simula as disponibilidades do sistema

hardware * restaurarSistema();
// restaura as disponibilidades do sistemas, uma ves que elas ja foram gastadas

hardware * utilizacaoSistema( hardware * sistema, componentesProcessos * aux, int filaId);
// recebe o "sistema", um processo que vai utilizar o sistema, e um id dizendo de qual fila o processo veio

void * politicaFirstComeFirstServed(fila *filaTempoReal, hardware * sistema);
//aplica a politica First Come First Served

void politicaFeedback(fila *feedbackQ0, hardware * sistema);
// aplica a prolitica de feedback, criando 2 filas de espera

//void exibirFila(fila *fila);
// recebe um fila e exibi ela (essa função é usada geralmente pra teste)

void printarProcesso(componentesProcessos * processo, int value, int localizacao);
//imprime na tela os estados do processos e se necessario suas expificações; recebe o processos, value e localização do processo; 
//os valores do value podem ser: 0 para processo criado. 1 para processo suspenso. 2 para processo executando. 3 para processo terminado. 4 para imprimir espicficações do processos. fora isso ele n interage com outros valores;
// localização pode ter 4 valores: 0 para a fila de feedback 0. 1 para a fila de feedback 1. 2 para a fila de feedback 2. 4 para a fila de tempo real. 


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


hardware * restaurarSistema(){
    hardware * a1 = (hardware*)malloc(sizeof(hardware));
    a1 = Sistema();
    return a1;
}

hardware * utilizacaoSistema( hardware * sistema, componentesProcessos * aux, int filaId){
    if( filaId == 4){
        printarProcesso(aux, 2, 4);
        printarProcesso(aux, 4, 4);
        
    }
    else if(filaId == 0) {
        printarProcesso(aux, 2, 0);
        printarProcesso(aux, 4, 0);
        
    }
    else if(filaId == 1) {
        printarProcesso(aux, 2, 1);
        printarProcesso(aux, 4, 1);
            
    }
    else if(filaId == 2) {
        printarProcesso(aux, 2, 2);
        printarProcesso(aux, 4, 2);
            
    }

    sistema->impressora = sistema->impressora - aux->impressoras;
    sistema->CD = sistema->CD - aux->cds;
    sistema->modem = sistema->modem - aux->modems;
    sistema->scanner = sistema->scanner - aux->scanners;
    
    if(aux->priority == 0){
        int i;
        for(i =0; i<=aux->processor_time; aux->processor_time-- ){
            sistema->memoriaPrincipal = sistema->memoriaPrincipal - aux->Mbytes;
        }
    }

    else{
        int i;
        for(i =0; i<=2 || aux->processor_time== 0; i++ ){
            sistema->memoriaPrincipal = sistema->memoriaPrincipal - aux->Mbytes;
            aux->processor_time--;
        }
        
        
    }

    return sistema;
}


void inicializarFila(fila *fila){
    fila->fim = NULL;
    fila->inicio = NULL;
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


void trocarElementoFila(fila* a1, fila *a2){
  componentesProcessos * elemento = retirarFila(a1);
  inserirFila(a2, elemento);
}


void entradaDeProcesso( ){
    fila queueEntradaProcessos;
    inicializarFila(&queueEntradaProcessos);

    FILE *processo;
    processo = fopen("lista-de-processos.txt", "r");
    

    if(processo == NULL )
        printf("erro no arquivo\n");

    else{
        char residuos[7];
        componentesProcessos a1;

        while(fscanf(processo, "%d %c  %d  %c %d %c %d %c %d %c %d %c %d %c %d",&a1.arrival_time, &residuos[0], &a1.priority, &residuos[1], &a1.processor_time, &residuos[2],&a1.Mbytes, &residuos[3], &a1.impressoras,  &residuos[4], &a1.scanners, &residuos[5], &a1.modems,  &residuos[6], &a1.cds)!=EOF){
            
            componentesProcessos *newProcesso;
            newProcesso = criarElementoFila(a, a1.arrival_time, a1.priority, a1.processor_time, a1.Mbytes, a1.impressoras, a1.scanners, a1.modems, a1.cds);
            printarProcesso(newProcesso, 0, 5);
            a++;
            inserirFila(&queueEntradaProcessos, newProcesso );
         }
        separacaoProcessos(&queueEntradaProcessos);  
    }
    
}


void separacaoProcessos(fila *queueEntradaProcessos){
    fila  ProcessosTempoReal;
    fila  ProcessosUsuario;
    
    inicializarFila(&ProcessosTempoReal);
    inicializarFila(&ProcessosUsuario);

    componentesProcessos *passador = (componentesProcessos*)malloc(sizeof(componentesProcessos));
    componentesProcessos *aux = (componentesProcessos*)malloc(sizeof(componentesProcessos));
    passador = queueEntradaProcessos->inicio;
    
    while(passador){
        
        if(passador->priority == 0){
            passador = passador->prox;
            aux= retirarFila(queueEntradaProcessos);
            printarProcesso(aux, 1, 5);
            inserirFila(&ProcessosTempoReal, aux);
        }
        else{
            passador = passador->prox;
            aux= retirarFila(queueEntradaProcessos);
            printarProcesso(aux, 1, 5);
            inserirFila(&ProcessosUsuario, aux);
        } 
    }
    politicasProcessos(&ProcessosTempoReal,&ProcessosUsuario);
}

void politicasProcessos(fila *ProcessosTempoReal, fila *ProcessosUsuario){
    
   hardware *sistema = Sistema();
   politicaFirstComeFirstServed(ProcessosTempoReal, sistema);
   politicaFeedback(ProcessosUsuario, sistema);
}

void * politicaFirstComeFirstServed(fila *filaTempoReal, hardware * sistema){
    
    componentesProcessos * processo;
    componentesProcessos * aux;
    aux = NULL;
    processo = filaTempoReal->inicio;

    while(processo!= NULL){
        aux = processo;

        if(aux->Mbytes <= sistema->memoriaPrincipal && aux->impressoras <= sistema->impressora && aux->modems <= sistema->modem && aux->scanners <= sistema->scanner && aux->cds <= sistema->CD){
            printf("processo de id: %d foi para a memoria principal\n", aux->id);
            utilizacaoSistema(sistema, aux, 4);
            printf("processo de id: %d foi encerrado\n", aux->id);
            sistema = restaurarSistema(sistema);
        }
        processo = processo->prox;
    }
}


void politicaFeedback(fila *feedbackQ0, hardware * sistema){
    
    fila feedbackQ1;
    fila feedbackQ2;

    inicializarFila(&feedbackQ1);
    inicializarFila(&feedbackQ2);

    componentesProcessos * processo;
    componentesProcessos * excluir;

    while( feedbackQ0->inicio != NULL ){
        
        processo = feedbackQ0->inicio;
        if(processo->Mbytes <= sistema->memoriaPrincipal && processo->impressoras <= sistema->impressora && processo->modems <= sistema->modem && processo->scanners <= sistema->scanner && processo->cds <= sistema->CD){

            
            sistema = utilizacaoSistema(sistema, processo, 0);
            if(processo->processor_time <= 0){
                printarProcesso(processo, 3, 0);
                excluir = retirarFila(feedbackQ0);
                free(excluir);
            }
            else{
                printarProcesso(processo, 1, 5);
                trocarElementoFila(feedbackQ0, &feedbackQ1); 
            }  
        }
        sistema = restaurarSistema(sistema);
    }

    excluir = NULL;
    processo = NULL;
    while( feedbackQ1.inicio != NULL ){

        processo = feedbackQ1.inicio;

        if(processo->Mbytes <= sistema->memoriaPrincipal && processo->impressoras <= sistema->impressora && processo->modems <= sistema->modem && processo->scanners <= sistema->scanner && processo->cds <= sistema->CD){

            sistema = utilizacaoSistema(sistema, processo, 1);

            if(processo->processor_time <= 0){
                printarProcesso(processo, 3, 5);
                excluir = retirarFila(&feedbackQ1);
                free(excluir);
            }
            else{
                printarProcesso(processo, 1, 5);
                trocarElementoFila(&feedbackQ1, &feedbackQ2);
            }
        }
        sistema = restaurarSistema(sistema);
    }

    excluir = NULL;
    processo = NULL;

    while( feedbackQ2.inicio != NULL ){
        
        processo = feedbackQ2.inicio;

        if(processo->Mbytes <= sistema->memoriaPrincipal && processo->impressoras <= sistema->impressora && processo->modems <= sistema->modem && processo->scanners <= sistema->scanner && processo->cds <= sistema->CD){

            sistema = utilizacaoSistema(sistema, processo, 2);

            if(processo->processor_time <= 0){
                printarProcesso(processo, 3, 5);
                excluir = retirarFila(&feedbackQ2);
                free(excluir);
            }
            else{
                printarProcesso(processo, 1, 5);
                trocarElementoFila(feedbackQ0, &feedbackQ1);
            }
        }
        sistema = restaurarSistema(sistema);
    }
}

int main(){
    
    entradaDeProcesso();


    return 0;
}

void printarProcesso(componentesProcessos * processo, int value, int localizacao){
    if(value == 0){
        printf("processo de id: %d foi criado\n\n", processo->id);
    }
    else if(value == 1){
        printf("processo de id: %d foi para o estado suspendo\n\n", processo->id);
    }
    else if(value == 2){
        printf("processo de id: %d foi para o estado execucao\n\n", processo->id);
    }
    else if(value == 3){
        printf("processo de id: %d foi terminado\n\n", processo->id);
    }
    else if(value == 4){
        printf("{\n");
        printf("processo de id: %d iniciado\n", processo->id);
        printf("Prioridade: %d\n", processo->priority);
        printf("tempo de processamento: %dseg\n", processo->processor_time);
        printf("tamanho: %dmb\n", processo->Mbytes);
        printf("modens necessarios: %d\n", processo->modems);
        printf("impressoras necessarios: %d\n", processo->impressoras);
        printf("cds necessarios: %d\n", processo->cds);
        printf("scanner necessarios: %d\n", processo->scanners);
        if(localizacao == 3){
            printf("localzacao: fila tempo real\n");
        }
        else if(localizacao < 3){
            printf("localzacao: fila fila feedback %d\n", localizacao);
        }
        printf("}\n\n");
    }

}

/*
void exibirFila(fila *fila){

    componentesProcessos *passador;
    passador = fila->inicio;
    
    while(passador){
        printf("o id do processo atual é: %d\n", passador->id);
        passador = passador->prox;
    }
}
*/

