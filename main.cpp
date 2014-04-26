#include "FileSystem53.h"

int main()
{
	char mainMemory[5] = {'a', 'b', 'c', 'd', 'e'};
	char* p = mainMemory;
	FileSystem53 fs53(5, 5, "filler");
	fs53.read_block(2,mainMemory);
	char mainMemory2[5] = {'a', 'b', 'c', 'd', 'e'};
	fs53.write_block(1, mainMemory2);
	//fs53.save();
	return 0;
}