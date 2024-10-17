all: test_avltree_performance test_memory_diagnosis test_memory_diagnosis_cpp

test_avltree_performance: ../naughty_avltree.c ../naughty_avltree.h ../naughty_list.c ../naughty_list.h ./test_avltree_performance.cc 
	gcc ../naughty_avltree.c ../naughty_list.c -c -I.. -fPIC -g -std=gnu99
	g++ ./naughty_avltree.o ./naughty_list.o ./test_avltree_performance.cc -o avltree_performance_test -I.. -fPIC -g

test_memory_diagnosis: ../naughty_avltree.c ../naughty_memory_diagnosis.c ./test_memory_diagnosis.c
	gcc ../naughty_avltree.c ../naughty_memory_diagnosis.c ./test_memory_diagnosis.c -o memory_diagnosis_test_c -I.. -DUSE_BACKTRACE -g -std=gnu99

test_memory_diagnosis_cpp: ../naughty_avltree.c ../naughty_memory_diagnosis.c ./test_memory_diagnosis.cc ../naughty_memory_diagnosis_interface.cc
	gcc ../naughty_avltree.c ../naughty_memory_diagnosis.c -I.. -DUSE_BACKTRACE -c -g -std=gnu99
	g++ ./naughty_avltree.o ./naughty_memory_diagnosis.o ../naughty_memory_diagnosis_interface.cc ./test_memory_diagnosis.cc -I.. -o memory_diagnosis_test_cpp -g

test_list: ../naughty_list.c ../naughty_list.h
	gcc $^ -I.. -c -g -std=c99 -DARCH_BIT_WIDTH=64
	g++ ./naughty_list.o ./test_list.cc -I.. -o $@ -g

test_fifo: ../naughty_fifo.c ../naughty_fifo.h
	gcc $^ -I.. -c -g -std=c99 -DARCH_BIT_WIDTH=64
	g++ ./naughty_fifo.o ./test_fifo.cc -I.. -o $@ -g

test_heap: ../naughty_heap.c ../naughty_list.c 
	gcc $^ -I.. -c -g -std=c99 -DARCH_BIT_WIDTH=64
	g++ ./naughty_heap.o ./naughty_list.o ./test_heap.cc -I.. -DUSE_BACKTRACE -g -o test_heap

.PHONY: clean

clean:
	rm ./*.o
	rm ./a.out
	rm ./avltree_performance_test
	rm ./memory_diagnosis_test_c
	rm ./memory_diagnosis_test_cpp
