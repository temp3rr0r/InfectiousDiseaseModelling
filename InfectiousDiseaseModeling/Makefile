all:
	$(CXX) InfectiousDiseaseModelingLinux.cpp -O3 -o diseasemodeling -std=c++11 -fopenmp
run:
	./diseasemodeling
clean:
	find . -name "diseasemodeling" -exec rm -rf {} \;
	find . -name "*.dot" -exec rm -rf {} \;
	find . -name "*.csv" -exec rm -rf {} \;