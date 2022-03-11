all: run

run: main.o linal.o scene.o figures.o lights.o material.o 
	c++ main.o linal.o scene.o figures.o lights.o material.o  -o run


main.o: main.cpp
	c++ main.cpp -c

linal.o: render/linal/linal.cpp
	c++ render/linal/linal.cpp -c

material.o: render/material/material.cpp
	c++ render/material/material.cpp -c

scene.o: render/scene.cpp
	c++ render/scene.cpp -c

figures.o: render/figures.cpp
	c++ render/figures.cpp -c

lights.o: render/lights.cpp
	c++ render/lights.cpp -c

clean:
	rm -rf run *.o run
