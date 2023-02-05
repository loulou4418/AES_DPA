# Makefile by @freelsn on gitHub

# - inc/
#     - *.h
# - src/
#     - *.c
# - obj/
#     - *.o
# - main

TARGET := main
SOURCES := $(wildcard src/*.c src/*.cpp)
OBJECTS := $(patsubst src%,obj%, $(patsubst %.c,%.o, $(patsubst %.cpp,%.o,$(SOURCES))))

INCLUDE := -I. -Iinc
LIBPATH := -pthread -lrt
LIBS :=

FLAGS := -Wall 
CCFLAGS := $(FLAGS) -std=c99 -g3 -O3

CC := gcc

all: $(OBJECTS)
	$(CC) $(CCFLAGS) $(INCLUDE) $(OBJECTS) -o $(TARGET) $(LIBPATH) $(LIBS)

%.o: ../src/%.c
	$(CC) $(CCFLAGS) $(INCLUDE) -c $< -o $@

.PHONY: clean

clean:
	rm -rf obj/*
	rm -f data/*.csv
	rm -f $(TARGET)
	
extract-numpy:
	cd data && python3 extract_data_in_csv.py

DPA:
	make extract-numpy
	make
	./main