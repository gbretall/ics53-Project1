#include "FileSystem53.h"

int main()
{
	FileSystem53 fs53;
	fs53.format();
	cout << fs53.create("hhhhhhhhhh") << endl;
	cout << fs53.create("hhhhhhhhhh") << endl;
	cout << fs53.create("jjjjjjjjjj") << endl;
	cout << fs53.create("ijijijijij") << endl;
	cout << fs53.create("ijijijijik") << endl;
	cout << fs53.create("ijijijijir") << endl;
	cout << fs53.create("ijijijijig") << endl;
	cout << fs53.search_dir("hhhhhhhhhh") << endl;
	cout << fs53.search_dir("mynamegood") << endl;
	//fs53.tester();
	cin.get();
	return 0;
}