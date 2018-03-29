all: main.cpp big.cpp big_algorithm.cpp
	g++ -c big.cpp
	g++ -c big_algorithm.cpp
	g++ -o main main.cpp big.o big_algorithm.o
	${CXX} -g $^ -o main

clean:
	rm *.o main
	
