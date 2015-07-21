 all:
	cd build;cmake -DCMAKE_BUILD_TYPE:STRING=Debug ..;make;

run:
	cd build;cmake -DCMAKE_BUILD_TYPE:STRING=Debug ..;make;./testz;
