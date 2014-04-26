//ICS 53 Project #1
//Gregory Bretall
//Braxton Han
//Jeremy Chao
//Warren Hsaio

#include "FileSystem53.h"

FileSystem53::FileSystem53(int l, int b, string storage)
	:B(64), K(7), FILE_SIZE_FIELD(1), ARRAY_SIZE(3), DESCR_SIZE(FILE_SIZE_FIELD+ARRAY_SIZE), MAX_FILE_NO(14),
	MAX_BLOCK_NO(64), MAX_FILE_NAME_LEN(32), MAX_OPEN_FILE(3), FILEIO_BUFFER_SIZE(64), _EOF(-1)
{
	ldisk = new char*[MAX_BLOCK_NO];
	for(unsigned int i = 0; i < MAX_BLOCK_NO; i++)
		ldisk[i] = new char[B];
	ldisk[0][0] = 1;
	ldisk[0][1] = 1;
	for(unsigned int i = 2; i < MAX_BLOCK_NO; i++)
		ldisk[0][i] = 0;
	ldisk[1][0] = 0;
}

void FileSystem53::read_block(int i, char *p)
{
	for(unsigned int j = 0; j < B; j++)
		p[j] = ldisk[i][j];
}

void FileSystem53::write_block(int i, char *p)
{
	for(unsigned int j = 0; j < B; j++)
		ldisk[i][j] = p[j];
}

void FileSystem53::restore()
{
	ifstream saveFile;
  	saveFile.open("otherBackup.txt");
  	if(!saveFile.is_open())
  		cout << "File not open" << endl;
  	else
  	{
		for(unsigned int i = 0; i < MAX_BLOCK_NO; i++)
		{
			saveFile.read(ldisk[i], B);
		}
	  	saveFile.close();	
	}
}

void FileSystem53::save()
{
	ofstream saveFile;
  	saveFile.open("backup.txt");
  	if(!saveFile.is_open())
  		cout << "File not open" << endl;
  	else
  	{
		for(unsigned int i = 0; i < MAX_BLOCK_NO; i++)
		{
			saveFile.write(ldisk[i], B);
		}
	  	saveFile.close();	
	}
}

int FileSystem53::create(string symbolic_file_name)
{
	for(unsigned int i = 0; i < K; i++)
	{
		if(ldisk[0][i] == 0)
		{
			
		}
		else
			return -1;
	}
}