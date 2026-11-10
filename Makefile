CXX      = g++
CC       = cc
CXXFLAGS = -std=c++17 -Wall -Wextra -pedantic
CFLAGS   = -std=c11 -O2

MUNIT_DIR = .

GAME  = tictactwo
TESTS = run_tests

.PHONY: all test clean

all: $(GAME)

$(GAME): main.o game.o
	$(CXX) $(CXXFLAGS) $^ -o $@

$(TESTS): test_last.o game.o munit.o
	$(CXX) $(CXXFLAGS) $^ -o $@

main.o: main.cpp game.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

game.o: game.cpp game.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

test_last.o: test_last.cpp game.h $(MUNIT_DIR)/munit.h
	$(CXX) $(CXXFLAGS) -I$(MUNIT_DIR) -c $< -o $@

munit.o: $(MUNIT_DIR)/munit.c $(MUNIT_DIR)/munit.h
	$(CC) $(CFLAGS) -c $< -o $@

test: $(TESTS)
	./$(TESTS)

clean:
	rm -f *.o $(GAME) $(TESTS)
