/*
 * SampleDecoder.cpp
 *
 *  Created on: Jan 14, 2011
 *      Author: rtoso
 */

#include "SampleDecoder.h"
#include <fstream>
#include <iostream>

SampleDecoder::SampleDecoder(string caminho) {
	ifstream instancia(caminho);

	instancia >> this->n >> this->m;

	this->distancias = new float *[this->n];
	for (int i = 0; i < this->n; i++) {
		this->distancias[i] = new float[this->n];
	}

	int i, j;
	float distancia;

	while (instancia >> i >> j >> distancia) {
		this->distancias[i][j] = this->distancias[j][i] = distancia;

		cout << distancias[i][j] << ' ' << distancias[j][i] << endl;
	}
}

// Runs in \Theta(n \log n):
double SampleDecoder::decode(const std::vector< double >& chromosome) const {
	std::vector< std::pair< double, unsigned > > ranking(chromosome.size());

	for(unsigned i = 0; i < chromosome.size(); ++i) {
		ranking[i] = std::pair< double, unsigned >(chromosome[i], i);
	}

	// Here we sort 'permutation', which will then produce a permutation of [n] in pair::second:
	std::sort(ranking.begin(), ranking.end());

	// permutation[i].second is in {0, ..., n - 1}; a permutation can be obtained as follows
	std::list< unsigned > permutation;
	for(std::vector< std::pair< double, unsigned > >::const_iterator i = ranking.begin();
			i != ranking.end(); ++i) {
		permutation.push_back(i->second);
	}

	// sample fitness is the first allele
	return chromosome.front();
}
