COMPILESTRING = mkdir -p build;cd build;cmake -DCMAKE_BUILD_TYPE:STRING=Debug ..;make -j16;

all:
	$(COMPILESTRING)

tests: $(all)
	$(COMPILESTRING) ctest;

testsX: $(all)
	$(COMPILESTRING) ctest -V;


clean:
	rm -rf build;

install:
	-sudo add-apt-repository ppa:ubuntu-toolchain-r/test -y
	-sudo add-apt-repository ppa:george-edison55/cmake-3.x -y
	-sudo apt-get update
	-sudo apt-get install gcc-4.9 g++-4.9 -y
	-sudo apt-get install freeglut3-dev libjpeg-dev libfreetype6-dev libxrandr-dev libglew-dev libsndfile1-dev libopenal-dev libudev-dev -y
	-sudo apt-get install software-properties-common libfreeimage3 libfreeimage-dev libassimp-dev libglw1-mesa-dev libglew-dev libxmu-dev libxi-dev -y
	-sudo apt-get install cmake -y
