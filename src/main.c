/* Data de submissao: 11/08/20
   Nome: João Pedro Martins Ferreira
   RA: 218762*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <unistd.h>

#define N_PROCESSOS 4

/* Função para encontrar primo
   Retorna 1 se primo e 0 se nao for primo*/
int primo(int num) {
	int div=0;
	for (int i = 1; i <= num; i++) {
		if (num % i == 0) { 
			div++;
		}
	}	 
	if (div == 2) return 1;
	else return 0;
}


int main() {
	pid_t filho[N_PROCESSOS];
	int p=0, k=1, t_v, entrada;
			
	/* Definir flags de protecao e visibilidade de memoria */
	int protection = PROT_READ | PROT_WRITE;
	int visibility = MAP_SHARED | MAP_ANON;
	
	/* Criar area de memoria compartilhada b[0] sera utilizada para contar n primos,
	e o restante do vetor b para guardar os dados de entrada*/
	int *b;
	b = (int*) mmap(NULL, sizeof(int), protection, visibility, 0, 0);
	
	/*Receber os dados de entrada*/
	do {
		scanf("%d", &entrada);
		b[k] = entrada;
		k++;
	}
	while ((entrada=getchar()) != '\n');
	t_v = k-1;/*Tamanho do vetor*/
	
	/*Condicao quando a entrada nao ultrapassa o numero de processos*/
	if (t_v<=N_PROCESSOS){
	  	for (int i = 0; i < t_v; i++){
		  	filho[i] = fork();
		  	if (filho[i] == 0) {
		  			p = primo(b[i+1]);
		  			b[0] += p;
		  			exit(0);
		    	}
		}
	}
	
	/*Condicao quando a entrada ultrapassa o numero de processos*/
	else {
	  	int j = 0;
	  	while(j<t_v){
	  		for (int i = 0; i < N_PROCESSOS; i++){
	  			filho[i] = fork();
	  			if (filho[i] == 0) {
		  			p = primo(b[j+1]);
		      			b[0] += p;
		      			exit(0);
		      		}
		      		j++;
		      		if (j==t_v) break;
		   	}
		}
	 }
	 
	 /*Espera terminar todos os filhos*/
	 for (int i=0; i<N_PROCESSOS; i++) {
	 	waitpid(filho[i], NULL, 0);
	 }
	 
	 printf("%d\n",b[0]);/*Imprime a quantidade de primos da entrada*/
	 return 0;
}
