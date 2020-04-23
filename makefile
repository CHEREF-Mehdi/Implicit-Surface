
CC = g++
GFLAGS= -g -Wall -std=c++11

lIBOPENGL= -lGL -lGLU -lGLEW -lglut -lm 

EXEC= surfImp
SRC= surfImp.cpp 


$(EXEC): $(SRC)
	$(CC) -g -Wall -std=c++11 -o $(EXEC) $(SRC) $(lIBOPENGL)

all: 
	clean 
	$(EXEC)

clean:
	rm -rf *.o
	rm -rf $(EXEC)
