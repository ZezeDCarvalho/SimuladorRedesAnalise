/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   fila.h
 * Author: flavio
 *
 * Created on 9 de Maio de 2019, 16:35
 */

#ifndef FILA_H
#define FILA_H

#ifdef __cplusplus
extern "C" {
#endif




#ifdef __cplusplus
}
#endif

typedef struct pacote_{
	double tamanho;
	struct pacote_ * prox;
}pacote;

int inserir(pacote ** inicio, pacote ** fim, double tamanho);
double remover(pacote ** inicio);
pacote * aloca_pct();


#endif /* FILA_H */

