# A Makefile to build the project

PROJECT =  air-brakes
HEADERS =  $(PROJECT).h
OBJ     =  air-brakes.o PID.o BMX055.o bma2x2.o bmg160.o bmm150.o


C99     =  cc -std=c99
CFLAGS  =  -Wall -pedantic -Werror


$(PROJECT) : $(OBJ)
	$(C99) $(CFLAGS) -o $(PROJECT) $(OBJ) -lm


%.o : %.c $(HEADERS)
	$(C99) $(CFLAGS) -c $<

clean:
	rm -f $(PROJECT) $(OBJ)
