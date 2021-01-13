#include<mpi.h>
#include<iostream>
#include <cmath>
#include<stdlib.h>

static constexpr auto points = 200000;

void gen_rand(unsigned& num_points) {
	auto x = static_cast<double>(rand()) / static_cast<double>(RAND_MAX);
	auto y = static_cast<double>(rand()) / static_cast<double>(RAND_MAX);

	auto dist = sqrt(x*x + y*y);
	if(dist <= 1) {
		num_points++;
	}
}
int main (int argc, char** argv) {
	int myid;
	int numprocs;

	srand(static_cast<unsigned>(myid));

	MPI_Init(&argc, &argv);

	MPI_Comm_rank(MPI_COMM_WORLD, &myid);

	MPI_Comm_size(MPI_COMM_WORLD, &numprocs);

	const auto s = points / numprocs;
	const auto s0 = s + points % numprocs;

	const auto startIndex = s0 + (myid-1)*s;
	const auto endIndex = startIndex + s;

	auto sum = 0;
	auto num_points = 0u;


	if(myid == 0) {
		for(auto i = 0; i < s0; i++) {
			gen_rand(num_points);
		}	
		std::cout << "master start: 0 end: " << s0 <<" part sum: " << num_points << std::endl;
	}
	else {
		for(auto i = startIndex  + 1; i < endIndex; i++) {
			gen_rand(num_points);
		}
	
		std::cout <<"slave: " << myid << "start: " << startIndex + 1 << " end: " << endIndex << " part sum: " << num_points << std::endl;
	}

	MPI_Reduce(&num_points, &sum, 1, MPI_UNSIGNED, MPI_SUM, 0, MPI_COMM_WORLD);
	
	MPI_Barrier(MPI_COMM_WORLD);

	if(myid == 0) {
		auto ratio = static_cast<double>(sum)/static_cast<double>(points);
		auto pi = 4*ratio;
		std::cout << "Sum: " << sum << " Ratio: " << ratio << std::endl;
		std::cout <<"PI: " << pi << std::endl;
	}	
	
	MPI_Finalize();
}
