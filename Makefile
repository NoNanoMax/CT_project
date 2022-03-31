all: run

run: main.o linal.o scene.o figures.o lights.o material.o 
	c++ --std=c++11 main.o linal.o scene.o figures.o lights.o material.o  -o run


main.o: main.cpp
	c++ --std=c++11 main.cpp -c -O2

linal.o: render/linal/linal.cpp
	c++ --std=c++11 render/linal/linal.cpp -c -O2

material.o: render/material/material.cpp
	c++ --std=c++11 render/material/material.cpp -c -O2

scene.o: render/scene.cpp
	c++ --std=c++11 render/scene.cpp -c -O2

figures.o: render/figures.cpp
	c++ --std=c++11 render/figures.cpp -c -O2

lights.o: render/lights.cpp
	c++ --std=c++11 render/lights.cpp -c -O2

clean:
	rm -rf run *.o run
