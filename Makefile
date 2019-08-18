blog:main.c
	gcc main.c model/model.o model/cJSON.o markdown/libsundown.so -llwan -lpthread -lm -ggdb -o blog -I./ -lleveldb -lstdc++
