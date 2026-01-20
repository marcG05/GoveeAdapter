CC := g++
FLAGS :=  -g -O0 -Iinc

main: main.o GooveConnector.o GooveDevice.o
	$(CC) -o main main.o GooveConnector.o GooveDevice.o

main.o: ./src/main.cpp
	$(CC) $(FLAGS) -c ./src/main.cpp

GooveConnector.o: ./src/GooveConnector.cpp ./inc/GooveConnector.h
	$(CC) $(FLAGS) -c ./src/GooveConnector.cpp

GooveDevice.o: ./src/GooveDevice.cpp ./inc/GooveDevice.h
	$(CC) $(FLAGS) -c ./src/GooveDevice.cpp

clean:
	rm *.o