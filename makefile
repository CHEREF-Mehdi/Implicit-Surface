
CC = g++
GFLAGS= -g -Wall -std=c++11

lIBOPENGL= -lGL -lGLU -lGLEW -lglut -lm 

EXEC= surfImp
SRC= surfImp.cpp 


$(EXEC): $(SRC)
	$(CC) $(GFLAGS) -o $(EXEC) $(SRC) $(lIBOPENGL)

clean:
	rm -rf *.o
	rm -rf $(EXEC)
