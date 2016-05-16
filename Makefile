
all: main

clean:
	rm *.o
	rm main

main: main.cpp interfaz.cpp campo.o drone.o sistema.o
	g++ -o main $^

%.o: %.cpp
	g++ -c $<
