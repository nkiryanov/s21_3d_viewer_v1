CC = gcc
FLAGS = -Wall  -Wextra 

all: clean install
	./build/front.app/Contents/MacOS/front
	
install:
	mkdir build
	cd ./build; cmake ../front/CMakeLists.txt; cmake --build .

uninstall:
	rm -rf build

clean: 
	rm -rf *.o *.a *.g* 3DViewer


dvi:
	open 1.html

dist: 
	cd ..; tar -cvf src.tar src

# for linux: -lrt -lm -lsubunit
test: clean 
	$(CC) $(FLAGS) model/3DViewer.c model/test.c -lcheck  -o test
	./test

gcov_report: clean
	$(CC) --coverage model/test.c model/3DViewer.c $(FLAGS) -lcheck -o test
	./test
	lcov -t "test" -o test.info -c -d .
	genhtml -o report test.info
#open report/index.html

rebuild: clean all


21: clean
	$(CC) $(FLAGS) model/3DViewer.c -o 3DViewer
	./3DViewer

