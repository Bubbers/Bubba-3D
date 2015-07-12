 all:
	cd build;cmake ..;make;

run:
	cd build;cmake -DCMAKE_BUILD_TYPE=Debug ..;make;./testz;
