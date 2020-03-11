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

	vector<pair<int, int>> selecionados; // criando vetor para os m selecionados: <pos, elemento>


	for(unsigned i = 0; i < chromosome.size(); ++i) {
		int pos = floor(total.size() * chromosome[i]);
		selecionados.push_back(make_pair(pos, total[pos]));

		total.erase(total.begin() + pos);
	}

	float fitness = 0, 
		menorDiversidade = numeric_limits<float>::max(), 
		diversidadeAtual = 0,
		dist;
	int posElementoMenosDiverso = -1;

	for (int i = 0; i < selecionados.size(); i++) {
		for (int j = 0; j < selecionados.size(); j++) {
			dist = this->distancias[selecionados[i].second][selecionados[j].second];
			fitness += dist;
			diversidadeAtual += dist;
		}
		if (diversidadeAtual < menorDiversidade) {
			menorDiversidade = diversidadeAtual;
			posElementoMenosDiverso = i;
		}
		diversidadeAtual = 0;
	}
	fitness /= 2;

	return fitness;
}
