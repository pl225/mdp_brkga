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

	std::vector< std::pair< double, unsigned > > ranking(chromosome.size());

	for(unsigned i = 0; i < chromosome.size(); ++i) {
		ranking[i] = std::pair< double, unsigned >(chromosome[i], i);
	}

	// Here we sort 'permutation', which will then produce a permutation of [n] in pair::second:
	std::sort(ranking.begin(), ranking.end());

	// permutation[i].second is in {0, ..., n - 1}; a permutation can be obtained as follows
	std::vector< unsigned > selecionados;
	for(int i = 0; i < this->getM(); i++) {
		selecionados.push_back(ranking[i].second);
	}

	float fitness = 0, 
		menorDiversidade = numeric_limits<float>::max(), // iniciando com maior valor possível
		diversidadeAtual = 0,
		dist;
	int indElementoMenosDiverso = -1;

	for (int i = 0; i < selecionados.size(); i++) { // calculando diversidade total e elemento menos diverso
		for (int j = 0; j < selecionados.size(); j++) {
			dist = this->distancias[selecionados[i]][selecionados[j]];
			fitness += dist; // diversidade total
			diversidadeAtual += dist; // diversidade de i
		}
		if (diversidadeAtual < menorDiversidade) { // diversidade de i menor que todas as diversidades
			menorDiversidade = diversidadeAtual;
			indElementoMenosDiverso = i;
		}
		diversidadeAtual = 0;
	}

	fitness /= 2; // removendo repetição de diversidades.
	int indElementoMaisDiverso = -1;

	for (int i = this->getM(); i < this->getN(); i++) {
		int v = ranking[i].second;
		for (int i = 0; i < selecionados.size(); i++) {
			if (i == indElementoMenosDiverso) {
				continue;
			}
			diversidadeAtual = diversidadeAtual + // calculando diferença de diversidade de acordo com Martí
				(this->distancias[selecionados[i]][v] -
				this->distancias[selecionados[i]][selecionados[indElementoMenosDiverso]]);
		}

		if (fitness < fitness + diversidadeAtual) { // averiguando se nova configuração é mais diversa
			indElementoMaisDiverso = i;
			break;
		} else {
			diversidadeAtual = 0;
		}
	}

	if (indElementoMaisDiverso != -1) {

		chromosome[ranking[indElementoMenosDiverso].second] = ranking[indElementoMaisDiverso].first;
		chromosome[ranking[indElementoMaisDiverso].second] = ranking[indElementoMenosDiverso].first;

		return fitness + diversidadeAtual;
	}

	return fitness;
}
