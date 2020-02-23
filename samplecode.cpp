#include <iostream>
#include <dirent.h>
#include <time.h>
#include <sys/stat.h>
#include <chrono>
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
	const unsigned MAX_GENS = 1000;	// run for 1000 gens
	const unsigned N_EXC = 5;
	
	const long unsigned rngSeed = time(NULL);	// seed to the random number generator
	MTRand rng(rngSeed);				// initialize the random number generator

	ofstream resultados ("resultados.txt");
	
	for (const string instancia: instancias) {

		cout << "Executando instância: " << instancia << endl;

		SampleDecoder decoder(instancia); // initialize the decoder
		const unsigned n = decoder.getM();		// size of chromosomes
		
		// initialize the BRKGA-based heuristic
		BRKGA< SampleDecoder, MTRand > algorithm(n, p, pe, pm, rhoe, decoder, rng, K, MAXT);

		double best = 0, time = 0, avg_cost = 0;

		resultados << "#######\t" << instancia << "#######\t" << endl;

		for (int i = 0; i < N_EXC; i++) {
			
			chrono::steady_clock::time_point start = chrono::steady_clock::now();
			algorithm.execute(X_INTVL, X_NUMBER, MAX_GENS);
			chrono::steady_clock::time_point end = chrono::steady_clock::now();

			auto elapsed_time = chrono::duration_cast<chrono::milliseconds>(end - start).count();
			time += elapsed_time;
			double best_i = algorithm.getBestFitness();
			avg_cost += best_i;
			if (best_i > best) {
				best = best_i;
			}
			resultados << '\t' << best_i << '\t' << elapsed_time << endl;
			algorithm.reset();
		}

		resultados << "#######\t" << best << '\t' << avg_cost / N_EXC << '\t' << time / N_EXC << "#######\t" << endl;
	}

	resultados.close();
	
	return 0;
}

std::vector<string> lerInstancias () {
	string path = "instances";
	DIR *dir = opendir(path.c_str());
	struct dirent *entry;
	struct stat filestat;
	std::vector<string> instancias;

	if (dir == NULL) {
		cout << "Erro ao abrir diretório de instâncias: " << path << endl;
		exit(1);
	} else {
		while ((entry = readdir(dir)) != NULL) {
			stat(entry->d_name, &filestat);
			if (!S_ISDIR(filestat.st_mode)) {
				instancias.push_back(path + '/' + entry->d_name);
			}
		}
	}

	return instancias;
}