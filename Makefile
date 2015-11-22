all:
	mkdir -p build;cd build;cmake -DCMAKE_BUILD_TYPE:STRING=Debug ..;make;

tests:
	mkdir -p build;cd build;cmake -DCMAKE_BUILD_TYPE:STRING=Debug ..;make; make test;

testsX:
	mkdir -p build;cd build;cmake -DCMAKE_BUILD_TYPE:STRING=Debug ..;make; ctest -V;


run:
	mkdir -p build;cd build;cmake -DCMAKE_BUILD_TYPE:STRING=Debug ..;make;./Bubba3D;

clean:
	rm -rf build;

install:
	sudo apt-get install g++ software-properties-common freeglut3-dev libfreeimage3 libfreeimage-dev libassimp-dev libglw1-mesa-dev libglew-dev libxmu-dev libxi-dev; sudo add-apt-repository ppa:george-edison55/cmake-3.x; sudo apt-get update; sudo apt-get install cmake;