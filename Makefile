all: run

run: main.o linal.o scene.o figures.o lights.o
	g++ --std=c++17 main.o linal.o scene.o figures.o lights.o -o run


main.o: main.cpp
	g++ --std=c++17 main.cpp -c

linal.o: render/scene/linal/linal.cpp
	g++ --std=c++17 render/scene/linal/linal.cpp -c

scene.o: render/scene/scene.cpp
	g++ --std=c++17 render/scene/scene.cpp -c

figures.o: render/scene/figures.cpp
	g++ --std=c++17 render/scene/figures.cpp -c

lights.o: render/scene/lights.cpp
	g++ --std=c++17 render/scene/lights.cpp -c

clean:
	rm -rf run *.o run
