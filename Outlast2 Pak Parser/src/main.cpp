#include <iostream>
#include "Package.h"

int main(int argc, char** argv) {


	if (argc > 2) {
		std::cout << "Too many arguments." << std::endl;
		return -1;
	}

	const char* file1 = "OLGame.upk";
	const char* file2 = "VSlice2-04_SE.upk";
	const char* file3 = "X_Checkpoints.upk";
	const char* file4 = "X_Checkpoints_OL1.upk";

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
