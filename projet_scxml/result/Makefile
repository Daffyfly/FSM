main: main.o machineState.o
	g++ -std=c++11 -o main main.o machineState.o

main.o: main.cpp machineState.h
	g++ -std=c++11 -Wall -c main.cpp

machineState.o: machineState.cpp machineState.h
	g++ -std=c++11 -Wall -c machineState.cpp

clean:
	rm *.o main
