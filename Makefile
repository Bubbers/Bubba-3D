all:
	mkdir -p build;cd build;cmake -DCMAKE_BUILD_TYPE:STRING=Debug ..;make;

run:
	mkdir -p build;cd build;cmake -DCMAKE_BUILD_TYPE:STRING=Debug ..;make;./testz;

clean:
	rm -rf build;

install:
	sudo apt-get install g++ software-properties-common freeglut3-dev libdevil-dev libassimp-dev libglw1-mesa-dev libglew-dev libxmu-dev libxi-dev; sudo add-apt-repository ppa:george-edison55/cmake-3.x; sudo apt-get update; sudo apt-get install cmake;
