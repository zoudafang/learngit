#include "Container.h"

int containers::keybit=128;
int containers::hammdist=8;
int containers::sub_index_num=4; 
int containers::test_size = 1000;
int containers::initialize_size = 200000;

int main() {
    containers example;
	example.prepare();
	example.initialize();
	example.test();
	printf("Successfully found similar photos! successful_num=%d\n", example.successful_num);
	return 0;
}