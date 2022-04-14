
CC=g++

CFLAGS= -O0 -Wpedantic -Wextra -Wall -mavx -mavx2 -c

SRC=AlphaBlending.cpp BlendingFunctions.cpp
SRC_O=AlphaBlending.o BlendingFunctions.o
SRC_SFML=-lsfml-graphics -lsfml-window -lsfml-system

alpha_o=alpha
Alpha=a

all: $(Alpha) $(SRC) $(alpha_o) $(SRC_O)

$(Alpha): $(SRC)
	$(CC) $(CFLAGS) $(SRC)


$(alpha_o): $(SRC_O)
	$(CC) $(SRC_O) -o $(alpha_o) $(SRC_SFML)
	
clean:
	rm *.o
