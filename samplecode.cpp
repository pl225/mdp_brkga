#include <iostream>
#include <dirent.h>
#include <time.h>
#include <fstream>
#include "SampleDecoder.h"
#include "MTRand.h"
#include "BRKGA.h"

std::vector<string> lerInstancias();

int main(int argc, char* argv[]) {

	std::vector<string> instancias = lerInstancias();
	
	const unsigned p = 1000;	// size of population
	const double pe = 0.20;		// fraction of population to be the elite-set
	const double pm = 0.10;		// fraction of population to be replaced by mutants
	const double rhoe = 0.70;	// probability that offspring inherit an allele from elite parent
	const unsigned K = 5;		// number of independent populations
	const unsigned MAXT = 4;	// number of threads for parallel decoding
	const unsigned X_INTVL = 100;	// exchange best individuals at every 100 generations
	const unsigned X_NUMBER = 2;	// exchange top 2 best
	const unsigned N_EXC = 5;
	
	const long unsigned rngSeed = time(NULL);	// seed to the random number generator
	MTRand rng(rngSeed);				// initialize the random number generator

	ofstream resultados ("resultados_10_min.txt", ios::out | ios::app);
	
	for (const string instancia: instancias) {

		cout << "Executando instância: " << instancia << endl;

		SampleDecoder decoder(instancia); // initialize the decoder
		const unsigned n = decoder.getN();		// size of chromosomes
		
		// initialize the BRKGA-based heuristic
		BRKGA< SampleDecoder, MTRand > algorithm(n, p, pe, pm, rhoe, decoder, rng, K, MAXT);

		resultados << "#######\t" << instancia << "#######\t" << endl;

		int64_t tempoAchado = algorithm.execute(X_INTVL, X_NUMBER);

	}

	resultados.close();
	
	return 0;
}

std::vector<string> lerInstancias () {
	string path = "10_min/";
	DIR *dir = opendir(path.c_str());
	struct dirent *entry;
	std::vector<string> instancias;

	if (dir == NULL) {
		cout << "Erro ao abrir diretório de instâncias: " << path << endl;
		exit(1);
	} else {
		while ((entry = readdir(dir)) != NULL) {
			string filename = entry->d_name;
			if (filename.size() > 2) {
				instancias.push_back(path + entry->d_name);
			}
		}
	}

	return instancias;
}