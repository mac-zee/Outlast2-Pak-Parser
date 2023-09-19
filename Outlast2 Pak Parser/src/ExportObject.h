#pragma once
#include <vector>

class ExportObject
{


public:


	ExportObject();
	~ExportObject();

	int ObjTypeRef = 0;
	int ParentClassRef = 0;
	int OwnerRef = 0;
	const char* name;
	int NameCount = 0;
	int Field6 = 0;
	int ObjectFlagsH = 0;
	int ObjectFlagsL = 0;
	int ObjectFileSize = 0;
	int DataOffset = 0;
	int Field11 = 0;
	int NumAdditionalFields = 0;
	int Field13 = 0;
	int Field14 = 0;
	int Field15 = 0;
	int Field16 = 0;
	int Field17 = 0;
	std::vector<int> AdditionalFields;
	
};

