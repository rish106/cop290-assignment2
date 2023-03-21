p1:
	if [ ! -d part1/obj ]; then mkdir part1/obj; fi
	gcc part1/src/main.c -o part1/obj/main.o
	./part1/obj/main.o $(arg1) $(arg2) $(arg3) $(arg4) $(arg5)

p2-hashmap_test:
	if [ ! -d part2/obj ]; then mkdir part2/obj; fi
	gcc part2/test/hashmap_test.c -o part2/obj/hashmap_test.o
	./part2/obj/hashmap_test.o

p2-list_test:
	if [ ! -d part2/obj ]; then mkdir part2/obj; fi
	gcc part2/test/list_test.c -o part2/obj/list_test.o
	./part2/obj/list_test.o

p2-no_yield:
	if [ ! -d part2/obj ]; then mkdir part2/obj; fi
	gcc part2/test/main_no_yield.c -o part2/obj/main_no_yield.o
	./part2/obj/main_no_yield.o $(arg1) $(arg2) $(arg3) $(arg4)

p2-race:
	if [ ! -d part2/obj ]; then mkdir part2/obj; fi
	gcc part2/test/main_race.c -o part2/obj/main_race.o
	./part2/obj/main_race.o $(arg1) $(arg2) $(arg3) $(arg4)

p2-main:
	if [ ! -d part2/obj ]; then mkdir part2/obj; fi
	gcc part2/test/main.c -o part2/obj/main.o
	./part2/obj/main.o $(arg1) $(arg2) $(arg3) $(arg4)

p3:
	if [ ! -d part3/obj ]; then mkdir part3/obj; fi
	gcc -o part3/obj/main.o part3/test/main.c part3/src/hm.c part3/src/list.c part3/src/mythread.c -I part3/include/ -pthread
	./part3/obj/main.o $(arg1) $(arg2) $(arg3) $(arg4)

report:
	pdflatex part3/src/report.tex

clean:
	if [ -d part1/obj ]; then rm -f part1/obj/*; fi
	if [ -d part2/obj ]; then rm -f part2/obj/*; fi
	if [ -d part3/obj ]; then rm -f part3/obj/*; fi
