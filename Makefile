CXX=clang++
CXXFLAGS=-std=c++11
APP_NAME=nes.out

$(APP_NAME): cpu.o main.o
	$(CXX) $(CXXFLAGS) -o $(APP_NAME) cpu.o main.o

cpu.o: cpu.h cpu.cpp
	$(CXX) $(CXXFLAGS) -c cpu.cpp

main.o: main.cpp
	$(CXX) $(CXXFLAGS) -c main.cpp

.PHONY: clean
clean:
	rm -f *.o $(APP_NAME)






