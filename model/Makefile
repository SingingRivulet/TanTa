model.a:model.o cJSON.o sha256.o render.o
	ar rv model.a model.o sha256.o cJSON.o render.o

sha256.o:sha256.cpp
	g++ sha256.cpp -I../markdown/html -I../markdown/src -c -ggdb

model.o:model.cpp utils.h passage.h user.h model.h cJSON.o
	g++ model.cpp -I../markdown/html -I../markdown/src -c -ggdb

cJSON.o:cJSON.c
	gcc cJSON.c -c

render.o:render.cpp
	g++ render.cpp -I../markdown/html -I../markdown/src -c -ggdb
	
clean:
	-rm *.o
	-rm *.gch
	-rm *.a
