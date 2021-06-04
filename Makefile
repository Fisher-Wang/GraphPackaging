CC = g++
FLAGS = -std=c++11 -Wall # -Wshadow

all : try_basic checker try_dfs_naive try_dfs2

%.o : %.cpp
	$(CC) -c $< $(FLAGS)

checker : checker.o 
	$(CC) $^ -o $@ $(FLAGS)

try_dfs_naive : try_dfs_naive.o graphio.o 
	$(CC) $^ -o $@ $(FLAGS)

try_basic : try_basic.o graphio.o 
	$(CC) $^ -o $@ $(FLAGS)

try_dfs2 : try_dfs2.o graphio.o
	$(CC) $^ -o $@ $(FLAGS)


test:
	python test.py