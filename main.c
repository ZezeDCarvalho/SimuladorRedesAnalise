/* 
 * File:   main.c
 * Author: flavio
 *
 * Created on 26 de Abril de 2019, 16:28
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "fila.h"

typedef struct little{
    double tempo_anterior;
    double soma_areas;
    double qtd_pacotes;
}little;
/**
 * 
 * @return numero aleatorio entre (0,1]
 */
double aleatorio() {
	double u;
	u = rand() % RAND_MAX;
	u = u / RAND_MAX;
	u = 1.0 - u;

	return (u);
}

/**
 * 
 * @param l parametro da exponencial
 * @return intervalo de tempo, com media tendendo ao intervalo
 * informado pelo usuario.
 */
double chegada_pct(double l) {
	return ((-1.0 / l) * log(aleatorio()));
}

/**
 * 
 * @return tamanho do pacote que acabou de chegar,
 * seguindo a proporcao aproximada de 50% = 550 Bytes,
 * 40% = 40 Bytes e 10% = 1500 Bytes.
 */
double gera_tam_pct() {
	double a = aleatorio();
	//tamanhos convertidos para Mb
	if (a <= 0.5) {
		return ((550.0 * 8.0) / (1000000.0));
	} else if (a <= 0.9) {
		return ((40.0 * 8.0) / (1000000.0));
	}
	return ((1500.0 * 8.0) / (1000000.0));
}

/**
 * 
 * @param a valor
 * @param b valor
 * @return menor dentre os valosres
 */
double minimo(double a, double b) {
	if (a <= b)
		return a;
	return b;
}

/**
 * 
 * @param l iniciar variaveis de little
 */
void iniciaLittle(little *l){
    l->qtd_pacotes = 0.0;
    l->soma_areas = 0.0;
    l->tempo_anterior = 0.0;
}

/*
 * 
 */
int main() {
	pacote ** inicio = malloc(sizeof (pacote *));
	pacote ** fim = malloc(sizeof (pacote *));
	*inicio = NULL;
	*fim = NULL;

        little en;
        little ewEntrada;
        little ewSaida;

        // iniciando Little
        iniciaLittle(&en);
        iniciaLittle(&ewEntrada);
        iniciaLittle(&ewSaida);
        
	//iniciando a semente 
	//para a geracao dos numeros
	//pseudoaleatorios

	//int semente = time(NULL);
	int semente = 1556915527;
	printf("Semente: %d\n", semente);
	srand(semente);
	//tempo atual
	double tempo = 0.0;
	//tempo total
	double tempo_total;
	printf("Informe o tempo total de simulacao: ");
	scanf("%lF", &tempo_total);

	//intervalo medio entre chegadas
	double intervalo;
	printf("Informe o intervalo médio de tempo (em segundos) entre pacotes: ");
	scanf("%lF", &intervalo);
	//ajustando parametro para a exponencial
	intervalo = 1.0 / intervalo;

	//contador de pacotes
	//double cont_pcts = 0.0;

	//tamanho do pacote gerado
	double tam_pct;

	//tamanho do link de saida do roteador
	double link;
	printf("Informe o tamanho do link (Mbps): ");
	scanf("%lF", &link);

	//fila, onde fila == 0 indica
	//roteador vazio; fila == 1
	//indica 1 pacote, ja em transmissao;
	//fila > 1 indica 1 pacote em transmissao,
	//e demais em espera
	//double fila = 0.0;

	//tempo de chegada do proximo pacote
	//ao sistema
	double chegada_proximo_pct = chegada_pct(intervalo);
	//printf("Chegada do primeiro pacote: %lF\n", chegada_proximo_pct);

	//tempo de chegada do proximo pacote
	//cbr ao sistema, com intervalo
	//de 20 ms entre pacotes
	double chegada_proximo_pct_cbr = 0.02;


	//tempo de saida do pacote que esta
	//sendo atendido atualmente
	double saida_pct_atendimento;

	//variavel para o calculo da ocupacao
	//do roteador
	double ocupacao = 0.0;


	while (tempo <= tempo_total) {
		//roteador vazio. Logo, avanco
		//no tempo para a chegada do
		//proximo pacote.
		if (*inicio == NULL)
			tempo = minimo(chegada_proximo_pct_cbr, chegada_proximo_pct);
		else {
			//ha fila!
			tempo = minimo(minimo(chegada_proximo_pct_cbr, chegada_proximo_pct), saida_pct_atendimento);
		}

		//chegada de pacote
		if (tempo == chegada_proximo_pct) {
			//gero o tamanho do pacote
			tam_pct = gera_tam_pct();

			if (*inicio == NULL) {
				//gerando o tempo de atendimento
				saida_pct_atendimento = tempo + tam_pct / link;
				//calculo da ocupacao
				ocupacao += saida_pct_atendimento - tempo;
			}
			//pacote colocado na fila
			inserir(inicio, fim, tam_pct);

			//gerar o tempo de chegada do proximo
			chegada_proximo_pct = tempo + chegada_pct(intervalo);
                        
                        //calculo EN
                        en.soma_areas += en.qtd_pacotes * (tempo - en.tempo_anterior);
                        en.qtd_pacotes++;
                        en.tempo_anterior = tempo;
                        
                        //calculo EW Entrada
                        ewEntrada.soma_areas += ewEntrada.qtd_pacotes * (tempo - ewEntrada.tempo_anterior);
                        ewEntrada.qtd_pacotes++;
                        ewEntrada.tempo_anterior = tempo;
		} else if (tempo == chegada_proximo_pct_cbr) {
			//printf("CBR chegou!\n");
			
			//chega pct cbr 1200 Bytes
			tam_pct = ((1200.0 * 8.0) / (1000000.0));

			if (*inicio == NULL) {
				//gerando o tempo de atendimento
				saida_pct_atendimento = tempo + tam_pct / link;
				//calculo da ocupacao
				ocupacao += saida_pct_atendimento - tempo;
			}
			//pacote colocado na fila
			inserir(inicio, fim, tam_pct);

			//gerar o tempo de chegada do proximo
			chegada_proximo_pct_cbr += 0.02;
                        
                        //calculo EN
                        en.soma_areas += en.qtd_pacotes * (tempo - en.tempo_anterior);
                        en.qtd_pacotes++;
                        en.tempo_anterior = tempo;
                        
                        //calculo EW Entrada
                        ewEntrada.soma_areas += ewEntrada.qtd_pacotes * (tempo - ewEntrada.tempo_anterior);
                        ewEntrada.qtd_pacotes++;
                        ewEntrada.tempo_anterior = tempo;
		} else {//saida de pacote
			//printf("Saida de pacote no tempo: %lF\n", tempo);
			remover(inicio);
			//printf("Fila: %lF\n", fila);

			if (*inicio != NULL) {
				//obtem o tamanho do pacote
				tam_pct = (*inicio)->tamanho;
				//gerando o tempo em que o pacote
				//atual saira do sistema
				saida_pct_atendimento = tempo + tam_pct / link;

				ocupacao += saida_pct_atendimento - tempo;
			}
                        //calculo EN
                        en.soma_areas += en.qtd_pacotes * (tempo - en.tempo_anterior);
                        en.qtd_pacotes--;
                        en.tempo_anterior = tempo;
                        
                        //calculo EW Saida
                        ewSaida.soma_areas += ewSaida.qtd_pacotes * (tempo - ewSaida.tempo_anterior);
                        ewSaida.qtd_pacotes++;
                        ewSaida.tempo_anterior = tempo;
		}
		//printf("==========================\n");
		//getchar();
	}
        ewEntrada.soma_areas += ewEntrada.qtd_pacotes * (tempo - ewEntrada.tempo_anterior);
        ewSaida.soma_areas += ewSaida.qtd_pacotes * (tempo - ewSaida.tempo_anterior);
        
        double enLittle = en.soma_areas/tempo;
        double ew = ewEntrada.soma_areas - ewSaida.soma_areas;
        ew = ew/ewEntrada.qtd_pacotes;
        
        double lambda = ewEntrada.qtd_pacotes/tempo;
        
	printf("Ocupacao: %lF\n", ocupacao / tempo);
        printf("\n=====Little=====\n");
        printf("E[N] = %lF",enLittle);
        printf("\nE[W] = %lF",ew);
        printf("\nLambda = %lF",lambda);
        printf("\n\n===== Validação Little :  %.20lF =====\n", (lambda*ew) - enLittle);
        
	//	printf("Pacotes gerados: %lF\n", cont_pcts);
	//	printf("Media do intervalo: %lF\n", tempo/cont_pcts);	

	return (EXIT_SUCCESS);
}

