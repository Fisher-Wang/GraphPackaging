CC = g++
FLAGS = -std=c++11 -Wall # -Wshadow

all : try_basic checker try_dfs_naive

%.o : %.cpp
	$(CC) -c $< $(FLAGS)

checker : checker.o 
	$(CC) $^ -o $@ $(FLAGS)

try_basic : try_basic.o graphio.o 
	$(CC) $^ -o $@ $(FLAGS)

try_dfs : try_dfs.o graphio.o 
	$(CC) $^ -o $@ $(FLAGS)

test:
	./try_dfs
	./checker data/Out_OutGraph_Basketball_480_Slice16_Gop8_10.log data/Out_SliceSize_Basketball_480_Slice16_Gop8_10.log result.txt