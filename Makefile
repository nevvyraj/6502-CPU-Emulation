CXX=g++
APP_NAME=nes.out

$(APP_NAME): cpu.o main.o
	$(CXX) -o $(APP_NAME) cpu.o main.o

cpu.o: cpu.h cpu.cpp
	$(CXX) -c cpu.cpp

main.o: main.cpp
	$(CXX) -c main.cpp

clean:
	rm -f *.o $(APP_NAME)


