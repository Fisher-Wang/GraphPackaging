CC = g++
FLAGS = -std=c++11 -Wall # -Wshadow

all : try_basic checker

%.o : %.cpp
	$(CC) -c $< $(FLAGS)

try_basic : try_basic.o graphio.o 
	$(CC) $^ -o $@ $(FLAGS)

checker : checker.o 
	$(CC) $^ -o $@ $(FLAGS)

test:
	./try_basic
	./checker data/Out_OutGraph_Basketball_480_Slice16_Gop8_10.log data/Out_SliceSize_Basketball_480_Slice16_Gop8_10.log result.txt