#include "white_list.h"



white_list::white_list():_file_path("whiteList.txt")
{
}

const char* white_list::get_file_path()
{
	return _file_path;
}

void white_list::add_device_to_white_list(device* dev)
{
	_file = fopen(_file_path, "a+");

	int i = 1;
	fwrite(&i, sizeof(int), 1, _file);
	fwrite(dev->letter, sizeof(char), 1, _file);
	i= wcslen(dev->manufacture)+2;
	fwrite(&i, sizeof(int), 1, _file);
	fwrite(dev->manufacture, sizeof(WCHAR), i, _file);
	i = wcslen(dev->name_of_product)+2;
	fwrite(&i, sizeof(int), 1, _file);
	fwrite(dev->name_of_product, sizeof(WCHAR), i, _file);
	i = wcslen(dev->serial_number)+2;
	fwrite(&i, sizeof(int), 1, _file);
	fwrite(dev->serial_number, sizeof(WCHAR), i, _file);
	
	fclose(_file);
}

bool white_list::is_device_trusted(device* dev)
{
	_file = fopen(_file_path, "r");
	bool result = false;

	device * dev1 = new device;
	int i = 0;
	while(!feof(_file))
	{
		
		fread(&i, sizeof(int), 1, _file);
		if (feof(_file)) { break; }
		fread(dev1->letter, sizeof(char), 1, _file);
		fread(&i, sizeof(int), 1, _file);
		dev1->manufacture = (WCHAR*)malloc(i * sizeof(WCHAR));
		fread(dev1->manufacture, sizeof(WCHAR), i, _file);

		fread(&i, sizeof(int), 1, _file);
		dev1->name_of_product = (WCHAR*)malloc(i * sizeof(WCHAR));
		fread(dev1->name_of_product, sizeof(WCHAR), i, _file);

		fread(&i, sizeof(int), 1, _file);
		dev1->serial_number = (WCHAR*)malloc(i * sizeof(WCHAR));
		fread(dev1->serial_number, sizeof(WCHAR), i, _file);

		if( !wcscmp(dev->serial_number,dev1->serial_number))
		{
			result = true;
			break;
		}
	}

	fclose(_file);
	return result;
}

void white_list::delete_device_from_white_list(evice* dev)
{
	_file = fopen(_file_path ,"r+");

	//

	fclose(_file);
}


white_list::~white_list()
{
}
