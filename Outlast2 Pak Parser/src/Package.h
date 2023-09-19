#pragma once
#include <string>
#include <vector>
#include <map>

#define UPK_SIGNATURE -1641380927

static struct GUID {
	int x[2] = { 0 };
	short int y[4] = { 0 };
};

class Package
{

public:

	Package();
	~Package();

	bool LoadUPK(std::string path);

	bool SetYoungBlake();

	void PrintHeader();
	void PrintExport(int index);

private:

	bool LoadHeader();
	bool LoadExports();
	bool LoadNames();

	//doesnt work
	void PrintPkgFlags(int flags);

private:

	std::string m_path;

	enum EPackageFlags
	{
		test = 1
	};

	std::vector<std::string> m_nameTable;
	std::vector<class ExportObject> m_exportTable;

	std::map<const char*, class ExportObject> m_map;

public:

	int signature = 0;
	short int version = 0, licenseeVersion = 0;

	int folderNameLength = 0;
	std::string folderName = "";

	int packageFlags = 0;

	int nameCount = 0;
	int nameOffset = 0;

	int exportCount = 0;
	int exportOffset = 0;

	int importCount = 0;
	int importOffset = 0;

	int dependsOffset = 0;

	int generationCount = 0;

	int engineVersion = 0;

	int cookerVersion = 0;

	int compressionFlags = 0;

	GUID guid = { 0 };


};

