model.o:model.cpp utils.h passage.h user.h model.h cJSON.o
	g++ model.cpp -I../markdown/html -I../markdown/src -c -ggdb
cJSON.o:cJSON.c
	gcc cJSON.c -c
clean:
	rm *.o
	rm *.gch
