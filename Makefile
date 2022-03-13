all: run

run: main.o linal.o scene.o figures.o lights.o
	c++ main.o linal.o scene.o figures.o lights.o -o run


main.o: main.cpp
	c++ main.cpp -c

linal.o: render/scene/linal/linal.cpp
	c++ render/scene/linal/linal.cpp -c

scene.o: render/scene/scene.cpp
	c++ render/scene/scene.cpp -c

figures.o: render/scene/figures.cpp
	c++ render/scene/figures.cpp -c

lights.o: render/scene/lights.cpp
	c++ render/scene/lights.cpp -c

clean:
	rm -rf run *.o run
