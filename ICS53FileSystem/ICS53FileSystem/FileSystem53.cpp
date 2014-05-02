//ICS 53 Project #1
//Gregory Bretall
//Braxton Han
//Jeremy Chao
//Warren Hsaio

#include "FileSystem53.h"

FileSystem53::FileSystem53()
	:B(64), K(7), FILE_SIZE_FIELD(1), ARRAY_SIZE(3), DESCR_SIZE(FILE_SIZE_FIELD+ARRAY_SIZE), MAX_FILE_NO(14),
	MAX_BLOCK_NO(64), MAX_FILE_NAME_LEN(10), MAX_OPEN_FILE(3), FILEIO_BUFFER_SIZE(64), _EOF(-1)
{
	ldisk = new char*[MAX_BLOCK_NO];
	for(unsigned int i = 0; i < MAX_BLOCK_NO; i++)
	{
		ldisk[i] = new char[B];
		for(unsigned int j = 0; j < B; j++)
		{
			ldisk[i][j] = 0;
		}
	}
	ldisk[0][0] = 1;
	ldisk[0][1] = 1;
	for(unsigned int i = 2; i < MAX_BLOCK_NO; i++)
	{
		ldisk[0][i] = 0;
	}
	desc_table = new descriptor [MAX_FILE_NO+2];
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

int FileSystem53::create(string symbolic_file_name)  //This function is now complete and working
{
	int fileStatus = -1;
	char* fileBuffer = new char[B];
	//Checks if there's space on disk
	for(unsigned int i = K; i < B; i++)
		if(desc_table[0].byteMap[i] == 0)
		{
			fileStatus = 0;
			break;
		}
	if(fileStatus == -1)
		return fileStatus;
	for(unsigned int i = 0; i < MAX_FILE_NO+1; i++)
	{
		if(!desc_table[i].isInitialized)
		{
			desc_table[i].isInitialized = true;
			//Checks for duplicates
			for(unsigned int j = 0; j < 3; j++)
			{
				if(desc_table[1].blockIndices[j] != 0)
				{
					read_block(desc_table[1].blockIndices[j], fileBuffer);
					bool isDuplicate = true;
					for(unsigned int k = 0; k < B-11; k+=11)
					{
						unsigned int strCount = 0;
						for(unsigned int m = k; m < 10; m++)
						{
							if(fileBuffer[m] != symbolic_file_name[strCount]) 
							{
								isDuplicate = false;
							}
							strCount++;
						}
						if(isDuplicate) return -2;
					}	
				}
			}
			//Writes into the directory file assuming no errors
			for(unsigned int arr = 0; arr < 3; arr++)
			{
				//If entry in bytemap is 0 (block is free for write)
				if(desc_table[0].byteMap[desc_table[1].blockIndices[arr]] == 0)
				{
					read_block(desc_table[1].blockIndices[arr], fileBuffer);
					for(unsigned int j = 0; j < B-11; j+=11)
					{
						if(fileBuffer[j] == 0)
						{
							int strCount = 0;
							for(unsigned int k = j; k < j+10; k++)
							{
								fileBuffer[k] = symbolic_file_name[strCount];
								strCount++;
							}
							fileBuffer[j+10] = i;
							if(j == 44)
								desc_table[0].byteMap[desc_table[1].blockIndices[arr]] = 1;
							write_block(desc_table[1].blockIndices[arr], fileBuffer);
							desc_table[1].fileLength += 11;
							return fileStatus;
						}
					}
				}
				//Checks if a new block must be allocated to the directory file
				else if(desc_table[1].blockIndices[arr] == 0)
				{
					for(unsigned int j = 0; j < K; j++)
						if(desc_table[0].byteMap[j] == 0)
						{
							for(unsigned int k = 0; k < MAX_FILE_NAME_LEN; k++)
								fileBuffer[k] = symbolic_file_name[k];
							fileBuffer[MAX_FILE_NAME_LEN] = i;
							for(unsigned int k = MAX_FILE_NAME_LEN+1; k < B; k++)
								fileBuffer[k] = 0;
							write_block(j, fileBuffer);
							desc_table[1].blockIndices[arr] = j;
							desc_table[1].fileLength += MAX_FILE_NAME_LEN+1;
							return fileStatus;
						}
				}
			}
		}
	}
}

void FileSystem53::format()
{
	char* fileBuffer = new char[B];
	for(unsigned int i = 0; i < B; i++)
		fileBuffer[i] = 0;
	for(unsigned int i = 1; i < K; i++)
	{
		read_block(i, fileBuffer);
		for(unsigned int j = 0; j < B; j++)
			fileBuffer[j] = 0;
		write_block(i, fileBuffer);
	}
	desc_table[0].byteMap = new char[B];
	for(unsigned int i = 0; i < B; i++)
		desc_table[0].byteMap[i] = ldisk[0][i];
	for(unsigned int i = 0; i < MAX_FILE_NO; i++)
	{
		desc_table[i].fileLength = 0;
		desc_table[i].blockIndices[0] = 0;
		desc_table[i].blockIndices[1] = 0;
		desc_table[i].blockIndices[2] = 0;
		if(i == 1) 
			desc_table[i].isInitialized = true;
		else
			desc_table[i].isInitialized = false;
	}
}

int FileSystem53::deleteFile(string fileName)
{
	return 0;
}

//Creates the open file table and initializes it to 0
void FileSystem53::OpenFileTable()
{
	open_file_table = new char[((B+2) * MAX_OPEN_FILE)];
	for(unsigned int i = 0; i < ((B+2) * MAX_OPEN_FILE); i++)
		open_file_table[i] = 0;
}

//Finds the first available entry in the open file table and returns its index
//Return -1 if open file table is full
int FileSystem53::find_oft()
{
	for(unsigned int i = 0; i < ((B+2) * MAX_OPEN_FILE); i+=(B+2))
	{
		if(open_file_table[i+B+1] == 0)
			return i;
	}
	return -1;
}

//Deallocates an entry from the open file table (resets it to 0)
void FileSystem53::deallocate_oft(int index)
{
	for(unsigned int i = index; i < B+2; i++)
		open_file_table[i] = 0;
}

//Returns a descriptor from the descriptor table
FileSystem53::descriptor FileSystem53::read_descriptor(int no)
{
	if(no > 0 && no < MAX_FILE_NO+2)
		return desc_table[no];
	else
	{
		FileSystem53::descriptor desc;
		return desc;
	}
}


void FileSystem53::write_descriptor(int no, FileSystem53::descriptor desc)
{
	if(no > 0 && no < MAX_FILE_NO+2)
	{
		desc_table[no] = desc;
	}
	char* fileBuffer = new char[B];
	read_block(1, fileBuffer);
	for(unsigned int i = 1; i < MAX_FILE_NO+1; i++)
	{
		for(unsigned int j = 0; j < DESCR_SIZE; j++)
		{
			if(j == 0)
				fileBuffer[((i-1)*DESCR_SIZE)+j] = desc_table[i].fileLength;
			else
				fileBuffer[((i-1)*DESCR_SIZE)+j] = desc_table[i].blockIndices[j];
		}
	}
	write_block(1, fileBuffer);
}

int FileSystem53::find_empty_descriptor()
{
	return 0;
}

int FileSystem53::find_empty_block()
{
	return 0;
}

int FileSystem53::fgetc(int index)
{
	return 0;
}

int FileSystem53::fputc(int c, int index)
{
	return 0;
}

bool FileSystem53::feof(int index)
{
	return 0;
}

int FileSystem53::search_dir(string symbolic_file_name) //This function is now complete and working
{
	char* fileBuffer = new char[B];
	for(unsigned int i = 0; i < ARRAY_SIZE; i++)
	{
		if(desc_table[1].blockIndices[i] != 0)
		{
			read_block(desc_table[1].blockIndices[i], fileBuffer);
			for(unsigned int j = 0; j < B-(MAX_FILE_NAME_LEN+1); j+=11)
			{
				int strCount = 0;
				int matchCount = 0;
				for(unsigned int k = j; k < MAX_FILE_NAME_LEN; k++)
				{
					if(fileBuffer[k] == symbolic_file_name[strCount])
					{
						matchCount++;
					}
					if(matchCount == 10)
					{
						return fileBuffer[j+10];
					}
					strCount++;
				}
			}		
		}
	}
	return -1;
}

void FileSystem53::delete_dir(int index, int start_pos, int len)
{
}

int FileSystem53::open_desc(int desc_no)
{
	return 0;
}

int FileSystem53::open(string symbolic_file_name)
{
	return 0;
}

int FileSystem53::read(int index, char* mem_area, int count)
{
	return 0;
}

int FileSystem53::write(int index, char value, int count)
{
	return 0;
}

int FileSystem53::lseek(int index, int pos)
{
	return 0;
}

void FileSystem53::close(int index)
{
}

void FileSystem53::directory()
{
}

void FileSystem53::diskdump(int start, int size)
{
}

void FileSystem53::tester()
{
	char* fileBuffer = new char[B];
	read_block(desc_table[1].blockIndices[0], fileBuffer);
	for(unsigned int i = 0; i < B-9; i++)
		cout << fileBuffer[i] << endl;
	cin.get();
}