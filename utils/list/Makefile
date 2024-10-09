run_container:
	docker run -v "${PWD}:/root" --privileged -ti agodio/itba-so-multi-platform:3.0

compile:
	gcc -Wall list.c test_list.c -o test_list.o

clean:
	rm test_list.o