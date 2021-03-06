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

	instancia >> this->n >> this->m;

	this->distancias = vector<vector<float>>(this->n);
	for (int i = 0; i < this->n; i++) {
		this->distancias[i] = vector<float>(this->n);
	}

	int i, j;
	float distancia;

	while (instancia >> i >> j >> distancia) {
		this->distancias[i][j] = this->distancias[j][i] = distancia;
	}
}

// Runs in \Theta(n \log n):
double SampleDecoder::decode(const std::vector< double >& chromosome) const {

	vector<int> total(this->getN()); // alocando vetor com n elementos
	iota(total.begin(), total.end(), 0); // preenchendo vetor de 0 a n - 1

	vector<int> selecionados; // criando vetor para os m selecionados

	for(unsigned i = 0; i < chromosome.size(); ++i) {
		int pos = floor(total.size() * chromosome[i]);
		selecionados.push_back(total[pos]);

		total.erase(total.begin() + pos);
	}

	float fitness = 0;

	for (int i = 0; i < selecionados.size(); i++) {
		for (int j = i + 1; j < selecionados.size(); j++) {
			fitness += this->distancias[selecionados[i]][selecionados[j]];
		}
	}

	return fitness;
}
