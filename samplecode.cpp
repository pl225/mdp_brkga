#include <iostream>
#include <dirent.h>
#include <time.h>
#include <sys/stat.h>
#include "SampleDecoder.h"
#include "MTRand.h"
#include "BRKGA.h"

int main(int argc, char* argv[]) {

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
	
	const unsigned p = 1000;	// size of population
	const double pe = 0.20;		// fraction of population to be the elite-set
	const double pm = 0.10;		// fraction of population to be replaced by mutants
	const double rhoe = 0.70;	// probability that offspring inherit an allele from elite parent
	const unsigned K = 5;		// number of independent populations
	const unsigned MAXT = 4;	// number of threads for parallel decoding
	const unsigned X_INTVL = 100;	// exchange best individuals at every 100 generations
	const unsigned X_NUMBER = 2;	// exchange top 2 best
	const unsigned MAX_GENS = 1000;	// run for 1000 gens
	
	const long unsigned rngSeed = time(NULL);	// seed to the random number generator
	MTRand rng(rngSeed);				// initialize the random number generator

	for (const string instancia: instancias) {

		cout << "Executando instância: " << instancia << endl;

		SampleDecoder decoder(instancia); // initialize the decoder
		const unsigned n = decoder.getM();		// size of chromosomes
		
		// initialize the BRKGA-based heuristic
		BRKGA< SampleDecoder, MTRand > algorithm(n, p, pe, pm, rhoe, decoder, rng, K, MAXT);
		
		unsigned generation = 0;		// current generation
		do {
			algorithm.evolve();	// evolve the population for one generation
			
			if((++generation) % X_INTVL == 0) {
				algorithm.exchangeElite(X_NUMBER);	// exchange top individuals
			}
		} while (generation < MAX_GENS);
		
		std::cout << "Best solution found has objective value = "
				<< algorithm.getBestFitness() << std::endl;

		exit(0);
	}
	
	return 0;
}