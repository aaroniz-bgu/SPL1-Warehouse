# Define some Makefile variables for the compiler and compiler flags
CC = g++
CFLAGS = -g -Wall -Weffc++ -std=c++11
BIN = ../bin
SRC = ../src

objects = $(BIN)/Action.o $(BIN)/ActionFactory.o $(BIN)/Customer.o $(BIN)/main.o $(BIN)/Order.o $(BIN)/Volunteer.o $(BIN)/WareHouse.o

all: directory SPLWarehouse

directory:
	mkdir -p $(BIN)

SPLWarehouse: clean $(objects)
	@echo '1: Building SPLWarehouse with all objects'
	$(CC) -o SPLWarehouse $(objects)
	@echo '2: Finished building SPLWarehouse'

$(BIN)/Action.o: $(SRC)/Action.cpp
	@echo '3: Compiling Action'
	$(CC) $(CFLAGS) -c -Iinclude -o $(BIN)/Action.o $(SRC)/Action.cpp
	@echo '4: Finished Compiling Action'

$(BIN)/ActionFactory.o: $(SRC)/ActionFactory.cpp
	@echo '5: Compiling ActionFactory'
	$(CC) $(CFLAGS) -c -Iinclude -o $(BIN)/ActionFactory.o $(SRC)/ActionFactory.cpp
	@echo '6: Finished Compiling ActionFactory'

$(BIN)/Customer.o: $(SRC)/Customer.cpp
	@echo '7: Compiling Customer'
	$(CC) $(CFLAGS) -c -Iinclude -o $(BIN)/Customer.o $(SRC)/Customer.cpp
	@echo '8: Finished Compiling Customer'

$(BIN)/main.o: $(SRC)/main.cpp
	@echo '9: Compiling main'
	$(CC) $(CFLAGS) -c -Iinclude -o $(BIN)/main.o $(SRC)/main.cpp
	@echo '10: Finished Compiling main'

$(BIN)/Order.o: $(SRC)/Order.cpp
	@echo '11: Compiling Order'
	$(CC) $(CFLAGS) -c -Iinclude -o $(BIN)/Order.o $(SRC)/Order.cpp
	@echo '12: Finished Compiling Order'

$(BIN)/Volunteer.o: $(SRC)/Volunteer.cpp
	@echo '13: Compiling Volunteer'
	$(CC) $(CFLAGS) -c -Iinclude -o $(BIN)/Volunteer.o $(SRC)/Volunteer.cpp
	@echo '14: Finished Compiling Volunteer'

$(BIN)/WareHouse.o: $(SRC)/WareHouse.cpp
	@echo '15: Compiling WareHouse'
	$(CC) $(CFLAGS) -c -Iinclude -o $(BIN)/WareHouse.o $(SRC)/WareHouse.cpp
	@echo '16: Finished Compiling WareHouse'

clean:
	@echo 'Cleaning up...'
	rm -f $(BIN)/*.o
