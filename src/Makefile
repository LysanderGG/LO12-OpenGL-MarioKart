PROJET = tp1
BIN  = $(PROJET)
OBJ  = $(PROJET).o affiche.o scene.o DatUtils.o SceneUtils.o

CPP  = g++
CC   = gcc
LD   = ld
RM = rm -f

LIBS = -lGLU -lGL -lglut 
INCS =
CXXINCS =
CXXFLAGS = $(CXXINCS) -g -Wall
CFLAGS = $(INCS) -g -Wall


.PHONY: all clean

all: $(BIN)

clean:
	${RM} $(OBJ) $(BIN)

$(BIN): $(OBJ)
	$(CC) $(OBJ) $(LIBS) -o $(BIN)

$(PROJET).o: $(PROJET).c SceneUtils.h affiche.h scene.h DatUtils.h
	$(CC) -c $< $(CFLAGS) -o $@
        
%.o: %.c %.h
	$(CC) -c $< $(CFLAGS) -o $@