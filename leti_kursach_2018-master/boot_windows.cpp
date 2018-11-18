#include <iostream>
#include <array>
#include <windows.h>

#include "Sequence.cpp"

struct rand101{
	int operator() const{
		return rand()%101;
	}
};


int main()
{
	setlocale(LC_TYPE, "rus");
	FILE *out;
	fopen_s(&out, "in.txt", "w");
	if(out){
		double time;
		LARGE_INTEGER T1, T2;
		LARGE_INTEGER frequency;

		int cardinality = 5;
		int const MAX_CARDINALITY = 205;
		int const STEP = 5;

		int i, j, k;

		Sequence<int> A, B, C, D, E;
		system("cls");

		QueryPerfomanceFrequency (&frequency);

		fprintf(out, "%d\n", ((int)(MAX_CARDINALITY - cardinality))/STEP + 1));

		do{
			A.getRandomSequence(cardinality - 4, rand101();
			B.getRandomSequence(cardinality, rand101());
			C.getRandomSequence(cardinality + 4, rand101());

			if(!QueryPerfomanceCounter(&T1)){
				std::cout << "Houston, we have a problem!" << '\n';
				break;
			}

			auto B = Sequence<int>::exclude(C, A);
			auto C = Sequence<int>::concat(A, B);
			auto D = Sequence<int>::merge(B, C);

			E = (A / B) & (A) | (C ^ D);

			if(!QueryPerfomanceCounter(&T2)){
				std::cout << "Houston, we have a problem!" << '\n';
				break;
			}

			long long tt1 = T1.QuadPart, tt2 = T2.QuadPart;
			long long tt3 = frequency.QuadPart;

			time = ((double)(tt2 - tt1)/(double)tt3);

			printf("\np=%d T1=%le, T2=%le, Dt=%le\n", cardinality, T1.QuadPart, T2.QuadPart, time);
			fprintf(out, "%d %le\n", cardinality, time);

		}while((cadrinality += STEP) <= MAX_CARDINALITY);

		puts("\n\n=== THE END IS HERE ===");
		getchar();
	}
	else{
		std::cout << "Houston, we have a problem with .txt!" << '\n';
		getchar();
	}



	/*Sequence<int> seq;
	seq.insert(6);
	seq.insert(2);
	seq.insert(3);
	seq.insert(3);
	seq.insert(3);
	seq.insert(2);
	seq.insert(3);

	Sequence<int> seq2;
	seq2.insert(3);
	seq2.insert(3);


	auto res = Sequence<int>::exclude(seq, seq2);
    seq.getRandomSequence(10);
	for(auto& el : seq){
		std::cout << el << "\n";
	}
	std::cout << "!\n";
	seq.getRandomSequence(20);
	for(auto& el : seq){
		std::cout << el << "\n";
	}
	std::cout << std::endl;*/


	return 0;
}
