all:
	mkdir -p build;cd build;cmake -DCMAKE_BUILD_TYPE:STRING=Debug ..;make;

run:
	mkdir -p build;cd build;cmake -DCMAKE_BUILD_TYPE:STRING=Debug ..;make;./testz;

clean:
	rm -rf build;

install:
	sudo apt-get install freeglut3-dev libdevil-dev libassimp-dev libglw1-mesa-dev libglew-dev libxmu-dev libxi-dev
