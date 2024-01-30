# Define some Makefile variables for the compiler and compiler flags
CC = g++
CFLAGS = -g -Wall -Weffc++ -std=c++11
BIN = bin
SRC = src

objects = $(BIN)/Action.o $(BIN)/ActionFactory.o $(BIN)/Customer.o $(BIN)/main.o $(BIN)/Order.o $(BIN)/Volunteer.o $(BIN)/WareHouse.o

all: directory warehouse

directory:
	mkdir -p $(BIN)

warehouse: clean $(objects)
	@echo 'Building warehouse with all objects'
	$(CC) -o $(BIN)/warehouse $(objects)
	@echo 'Finished building warehouse'

$(BIN)/Action.o: $(SRC)/Action.cpp
	@echo 'Compiling Action'
	$(CC) $(CFLAGS) -c -Iinclude -o $(BIN)/Action.o $(SRC)/Action.cpp
	@echo 'Finished Compiling Action'

$(BIN)/ActionFactory.o: $(SRC)/ActionFactory.cpp
	@echo 'Compiling ActionFactory'
	$(CC) $(CFLAGS) -c -Iinclude -o $(BIN)/ActionFactory.o $(SRC)/ActionFactory.cpp
	@echo 'Finished Compiling ActionFactory'

$(BIN)/Customer.o: $(SRC)/Customer.cpp
	@echo 'Compiling Customer'
	$(CC) $(CFLAGS) -c -Iinclude -o $(BIN)/Customer.o $(SRC)/Customer.cpp
	@echo 'Finished Compiling Customer'

$(BIN)/main.o: $(SRC)/main.cpp
	@echo 'Compiling main'
	$(CC) $(CFLAGS) -c -Iinclude -o $(BIN)/main.o $(SRC)/main.cpp
	@echo 'Finished Compiling main'

$(BIN)/Order.o: $(SRC)/Order.cpp
	@echo 'Compiling Order'
	$(CC) $(CFLAGS) -c -Iinclude -o $(BIN)/Order.o $(SRC)/Order.cpp
	@echo 'Finished Compiling Order'

$(BIN)/Volunteer.o: $(SRC)/Volunteer.cpp
	@echo 'Compiling Volunteer'
	$(CC) $(CFLAGS) -c -Iinclude -o $(BIN)/Volunteer.o $(SRC)/Volunteer.cpp
	@echo 'Finished Compiling Volunteer'

$(BIN)/WareHouse.o: $(SRC)/WareHouse.cpp
	@echo 'Compiling WareHouse'
	$(CC) $(CFLAGS) -c -Iinclude -o $(BIN)/WareHouse.o $(SRC)/WareHouse.cpp
	@echo 'Finished Compiling WareHouse'

clean:
	@echo 'Cleaning up...'
	rm -f $(BIN)/*
