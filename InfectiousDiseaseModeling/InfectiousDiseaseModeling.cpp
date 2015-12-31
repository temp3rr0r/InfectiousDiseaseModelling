#include <omp.h>
#include <iostream>

int main() {

	bool do_use_parallel = false;

	
	omp_set_num_threads(4);
//#pragma omp parallel if(do_use_parallel)
#pragma omp parallel
	{
		std::cout << "Hello from " << omp_get_thread_num() << std::endl;
	}



	system("pause");	
}

