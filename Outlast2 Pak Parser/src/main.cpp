#include <iostream>
#include "Package.h"
/*

	C1 83 2A 9E = signature
	72 03 = 882 = Version
	42 00 = 66 = Licensee version
	4A 90 35 00 = 3510346 = header size
	05 00 00 00 = Folder Name String Length. 5 Bytes including null terminating character
	4E 6F 6E 65 00 = Folder name ("None ") with null at end.
	09 00 A8 62 = Package Flags
	B4 51 00 00 = 20,916 = Name count
	F9 13 00 00 = Name offset 
	F0 98 00 00 = 39,152 = Export Count
	0E 8A 0A 00 = Export Offset
	50 03 00 00 = 848 = Import Count
	4E 2D 0A 00 = Import Offset
	8A 2C 33 00 = Depends offset
	4A 90 35 00 00 00 00 00 00 00 00 00 00 00 00 00 = Unknown 16 bytes
	F5 9A 1C 8E B8 B9 E1 4A B1 6E 75 8F 2B 62 68 42 = GUID {8E1C9AF5-B9B8-4AE1-B16E-758F2B626842}
	01 00 00 00 = Start of FArray saying that there is 1 element in it
	F0 98 00 00 B4 51 00 00 9C 3F 00 00 = FArray of GenerationInfoList
	6E 44 00 00 = 17518 = Engine Version
	8E 00 00 00 = 142 = Cooker Version

*/
int main(int argc, char** argv) {


	if (argc > 2) {
		std::cout << "Too many arguments." << std::endl;
		return -1;
	}

	const char* file1 = "OLGame.upk";
	const char* file2 = "VSlice2-04_SE.upk";
	const char* file3 = "X_Checkpoints.upk";

	const char* path = file3;


	if (argc == 2) 
		path = argv[1];


	Package* pak = new Package;

	if (!pak->LoadUPK(path)) {
		std::cout << "Failed to load file: " << path << std::endl;
		delete pak;
		return -1;
	}

	pak->PrintHeader();

	pak->SetYoungBlake();

	pak->PrintExport(0);

	std::cout << "Program by Hydle." << std::endl;

	delete pak;

}
