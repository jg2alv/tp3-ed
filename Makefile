GPP = g++ -I./include
RM = rm -rf

OBJ = ./obj/Passagem.o ./obj/Consulta.o ./obj/Celula.o ./obj/Lista.o ./obj/Arvore.o

all: ./src/main.cpp $(OBJ)
	$(GPP) ./src/main.cpp $(OBJ) -o ./bin/tp3.out

./obj/Passagem.o: ./src/Passagem.cpp
	$(GPP) -c ./src/Passagem.cpp -o ./obj/Passagem.o

./obj/Consulta.o: ./src/Consulta.cpp
	$(GPP) -c ./src/Consulta.cpp -o ./obj/Consulta.o

./obj/Celula.o: ./src/Celula.cpp
	$(GPP) -c ./src/Celula.cpp -o ./obj/Celula.o

./obj/Lista.o: ./src/Lista.cpp
	$(GPP) -c ./src/Lista.cpp -o ./obj/Lista.o

./obj/Arvore.o: ./src/Arvore.cpp
	$(GPP) -c ./src/Arvore.cpp -o ./obj/Arvore.o

clean:
	$(RM) ./obj/*.o ./bin/*
