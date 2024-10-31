compile_bitmap: 
	gcc -Wall -std=c99 ./bitmapManager/bitmap_memory_manager.c ./test/test_mm.c ./test/test_util.c -o ./memory_manager_test.o

compile_buddy:
	gcc -Wall -g -std=c99 ./buddyManager/buddy_memory_manager.c ./test/test_mm.c ./test/test_util.c -o ./memory_manager_test.o -lm

clean: 
	rm ./memory_manager_test.o

run_bitmap:
	./memory_manager_test.o 10240

run_buddy:
	./memory_manager_test.o 16384