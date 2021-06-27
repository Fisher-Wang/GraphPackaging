CC = g++
FLAGS = -std=c++11 -Wall # -Wshadow
EXES = try_basic checker try_dfs_naive try_dfs2 try_dfs3 try_eval try_eval_p1 try_dfs_post try_dfs2_eval try_dfs2_backup random-algo checker_for_times

all : $(EXES)

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

try_dfs3 : try_dfs3.o graphio.o
	$(CC) $^ -o $@ $(FLAGS)

try_eval : try_eval.o graphio.o
	$(CC) $^ -o $@ $(FLAGS)

try_eval_p1 : try_eval_p1.o graphio.o
	$(CC) $^ -o $@ $(FLAGS)

try_dfs_post : try_dfs_post.o graphio.o
	$(CC) $^ -o $@ $(FLAGS)

try_dfs2_eval : try_dfs2_eval.o graphio.o
	$(CC) $^ -o $@ $(FLAGS)

try_dfs2_backup : try_dfs2_backup.o graphio.o
	$(CC) $^ -o $@ $(FLAGS)

limit : limit.o graphio.o
	$(CC) $^ -o $@ $(FLAGS)

try_analyze : try_analyze.o graphio.o
	$(CC) $^ -o $@ $(FLAGS)

random-algo : random-algo.o
	$(CC) $^ -o $@ $(FLAGS)

checker_for_times : checker_for_times.o
	$(CC) $^ -o $@ $(FLAGS)



test:
	python test.py

clean:
	rm *.o $(EXES)