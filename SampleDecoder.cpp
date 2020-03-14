/*
 * SampleDecoder.cpp
 *
 *  Created on: Jan 14, 2011
 *      Author: rtoso
 */

#include "SampleDecoder.h"
#include <fstream>
#include <iostream>
#include <numeric>
#include <math.h>

SampleDecoder::SampleDecoder(string caminho) {
	ifstream instancia(caminho);

	instancia >> this->n >> this->m >> this->custo;

	this->distancias = vector<vector<float>>(this->n);
	for (int i = 0; i < this->n; i++) {
		this->distancias[i] = vector<float>(this->n);
		this->distancias[i][i] = 0;
	}

	int i, j;
	float distancia;

	while (instancia >> i >> j >> distancia) {
		this->distancias[i][j] = this->distancias[j][i] = distancia;
	}
}

// Runs in \Theta(n \log n):
double SampleDecoder::decode(std::vector< double >& chromosome) const {

	vector<int> total(this->getN()); // alocando vetor com n elementos
	iota(total.begin(), total.end(), 0); // preenchendo vetor de 0 a n - 1

	vector<pair<int, int>> selecionados; // criando vetor para os m selecionados: <pos, elemento>

	for(unsigned i = 0; i < chromosome.size(); ++i) { // decodificando vetor de rand
		int pos = floor(total.size() * chromosome[i]); // pos do elemento selecionado
		selecionados.push_back(make_pair(pos, total[pos])); // guardando o elemento e sua posicao

		total.erase(total.begin() + pos); // apagando elemento de pos
	}

	float fitness = 0, 
		menorDiversidade = numeric_limits<float>::max(), // iniciando com maior valor possível
		diversidadeAtual = 0,
		dist;
	int posElementoMenosDiverso = -1;

	for (int i = 0; i < selecionados.size(); i++) { // calculando diversidade total e elemento menos diverso
		for (int j = 0; j < selecionados.size(); j++) {
			dist = this->distancias[selecionados[i].second][selecionados[j].second];
			fitness += dist; // diversidade total
			diversidadeAtual += dist; // diversidade de i
		}
		if (diversidadeAtual < menorDiversidade) { // diversidade de i menor que todas as diversidades
			menorDiversidade = diversidadeAtual;
			posElementoMenosDiverso = i;
		}
		diversidadeAtual = 0;
	}

	fitness /= 2; // removendo repetição de diversidades.

	int u = -1; // substituirá o elemento menos diverso
	
	for (const int v : total) { // procurando primeiro elemento não escolhido v mais diverso que o menor diverso encontrado
		for (int i = 0; i < selecionados.size(); i++) {
			if (i == posElementoMenosDiverso) continue;
			diversidadeAtual = diversidadeAtual + // calculando diferença de diversidade de acordo com Martí
				(this->distancias[selecionados[i].second][v] -
				this->distancias[selecionados[i].second][selecionados[posElementoMenosDiverso].second]);
		}
		if (fitness < fitness + diversidadeAtual) { // averiguando se nova configuração é mais diversa
			u = v;
			break;
		} else {
			diversidadeAtual = 0;
		}
	}

	if (u != -1) {
		for (int i = 0; i < posElementoMenosDiverso; i++) { 
			if (selecionados[i].first < u) { // calculando a posição do elemento u após remoções de elementos no fase de seleção
				u--;
			}
		}
		chromosome[posElementoMenosDiverso] = (double) u / ((double)(this->getN() - posElementoMenosDiverso));
		return fitness + diversidadeAtual;
	}

	return fitness;
}
