#include <omp.h>
#include <iostream>

#include <boost/lambda/lambda.hpp>
#include <iterator>
#include <algorithm>

void main2()
{
	using namespace boost::lambda;
	typedef std::istream_iterator<int> in;

	std::for_each(
		in(std::cin), in(), std::cout << (_1 * 3) << " ");
}

int main() {

	bool do_use_parallel = false;

	
	omp_set_num_threads(4);
//#pragma omp parallel if(do_use_parallel)
#pragma omp parallel
	{
		std::cout << "Hello from " << omp_get_thread_num() << std::endl;
	}
	
	main2();

	system("pause");	
}

