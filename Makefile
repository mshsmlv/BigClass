%.o: %.cpp %.h
	${CXX} -c $< -o $@ 

all: main.cpp *.o
	${CXX} -g $^ -o main

claen:
	rm *.o main
	
