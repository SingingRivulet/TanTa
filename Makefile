blog:main.c
	cd markdown && make
	cd model && make
	gcc main.c model/model.a markdown/libsundown.so -llwan -lpthread -lm -ggdb -o blog -I./ -lleveldb -lstdc++
	-mkdir data
clean:
	-cd markdown && make clean
	-cd model && make clean
	-rm -rf data
	-rm blog
	-rm ALLOW_CREATE_USER

createUserOn:
	echo 1 > ALLOW_CREATE_USER

createUserOff:
	rm ALLOW_CREATE_USER
