list:
	gcc -o obj/list_test test/list_test.c src/list.c
	./obj/list_test  -I include/

doc:
	doxygen Doxyfile

clean:
	rm -f obj/*
	rm -fr doc/*

test:
	gcc -g3 -o obj/main.o test/main.c src/list.c src/hm.c src/mythread.c -I include/ -pthread
	./obj/main.o data/big_random1.txt data/big_random2.txt data/big_random3.txt

report:
	pdflatex texample.tex

.PHONY: all test clean
