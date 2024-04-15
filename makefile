# $ make
# $ ./boids-sim

CC = g++
CFLAGS = -c
DEBUG = -g
SFML = -lsfml-graphics -lsfml-window -lsfml-system

all: model

model: Simulation.o Agent.o main.o
	$(CC) $(DEBUG) Simulation.o Agent.o main.o -o boids-sim $(SFML)

Simulation.o : Simulation.cpp
	$(CC) $(CFLAGS) $(DEBUG) Simulation.cpp

Agent.o : Agent.cpp
	$(CC) $(CFLAGS) $(DEBUG) Agent.cpp

clean:
	\rm *.o