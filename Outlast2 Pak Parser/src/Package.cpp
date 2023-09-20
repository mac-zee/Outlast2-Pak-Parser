#include "Package.h"
#include "ExportObject.h"
#include <iostream>
#include <fstream>

Package::Package() 
{
	
}

Package::~Package()
{


}

void Package::PrintHeader()
{
	std::cout << "Package: " << m_path << std::endl;
	std::cout << "--------------------------" << std::endl;
	std::ios oldState(nullptr);
	oldState.copyfmt(std::cout);

	std::cout << "Signature: 0x" << std::uppercase << std::hex << signature << std::endl;
	std::cout.copyfmt(oldState);

	std::cout << "Version: " << version << std::endl;
	std::cout << "Licensee Version: " << licenseeVersion << std::endl;
	std::cout << "Folder Name: " << folderName << std::endl;

	std::cout << "Package Flags: 0x" << std::uppercase << std::hex << packageFlags << std::endl;
	std::cout.copyfmt(oldState);

	//PrintPkgFlags(packageFlags);

	std::cout << "Name Count: " << nameCount << std::endl;
	std::cout << "Name Offset: " << nameOffset << std::endl;

	std::cout << "Export Count: " << exportCount << std::endl;
	std::cout << "Export Offset: " << exportOffset << std::endl;

	std::cout << "Import Count: " << importCount << std::endl;
	std::cout << "Import Offset: " << importOffset << std::endl;

	std::cout << "Depends Offset: " << dependsOffset << std::endl;

	std::cout << "GUID: {" << std::uppercase << std::hex << guid.x[0] << "-" << guid.y[0] << "-" << guid.y[1] << "-" << guid.y[2] << "-" << guid.x[1] << guid.y[3] << "}" << std::endl;
	std::cout.copyfmt(oldState);

	std::cout << "Generation Count: " << generationCount << std::endl;

	std::cout << "Engine Version: " << engineVersion << std::endl;

	std::cout << "Cooker Version: " << cookerVersion << std::endl;

	std::cout << "Compression Flags: " << std::uppercase << std::hex << compressionFlags << std::endl;
	std::cout.copyfmt(oldState);

	std::cout << std::endl;
}

void Package::PrintPkgFlags(int flags)
{
	//unfinished function
	std::map<int, std::string> map;

	map[0] = "None";

	std::cout << "Enabled Flags: ";

	for (int i = 0; i < 23; i++) {

		int f = (int)std::pow(2, i);

		int test = flags & int(EPackageFlags::test);

		if ((flags & f) != 0) {
			std::cout << map[f] << " | ";
		}

	}
}

bool Package::LoadUPK(std::string path)
{

	m_path = path;

#if _DEBUG
	m_path.insert(0, "res/upks/");
#endif

	if (!LoadHeader()) {
		std::cout << "Failed to load header data!" << std::endl;
		return false;
	}

	if (!LoadNames()) {
		std::cout << "Failed to load name table!" << std::endl;
		return false;
	}

	if (!LoadExports()) {
	
		std::cout << "Failed to load export table!" << std::endl;
		return false;
	}

	//load imports
	
	m_nameTable.clear();

	return true;
}


bool Package::LoadHeader()
{

	std::ifstream file(m_path, std::ios::binary);
	if (!file.is_open()) {
		std::cout << "Failed to open file!" << std::endl;
		return false;
	}

	const int headerSize = 128;
	char buffer[headerSize] = { 0 };

	if (!file.read(buffer, headerSize)) {
		std::cout << "Failed to read! file" << std::endl;
		return false;
	}

	signature = *(int*)&(buffer[0]);

	if (signature != UPK_SIGNATURE) {
		std::cout << "Signature Mismatch! File at " << m_path << " is not a valid OL2 UPK file!" << std::endl;
		std::ios oldState(nullptr);
		oldState.copyfmt(std::cout);
		std::cout << "Expected: " << std::uppercase << std::hex << -1641380927 << " Got: " << signature << std::endl;
		std::cout.copyfmt(oldState);

		return false;
	}

	version = *(int*)&(buffer[4]);
	licenseeVersion = *(int*)&(buffer[6]);

	folderNameLength = *(int*)&(buffer[12]);

	for (int i = 0; i < folderNameLength - 1; i++) {
		folderName += (buffer[16 + i]);
	}

	int offset = 16 + folderNameLength;

	packageFlags = *(int*)&(buffer[offset]);

	nameCount = *(int*)&(buffer[offset + 4]);
	nameOffset = *(int*)&(buffer[offset + 8]);

	exportCount = *(int*)&(buffer[offset + 12]);
	exportOffset = *(int*)&(buffer[offset + 16]);

	importCount = *(int*)&(buffer[offset + 20]);
	importOffset = *(int*)&(buffer[offset + 24]);

	dependsOffset = *(int*)&(buffer[offset + 28]);

	guid.x[0] = *(int*)&(buffer[offset + 48]);

	guid.y[0] = *(int*)&(buffer[offset + 52]);
	guid.y[1] = *(int*)&(buffer[offset + 54]);
	guid.y[2] = *(int*)&(buffer[offset + 56]);

	guid.x[1] = *(int*)&(buffer[offset + 58]);
	guid.y[3] = *(int*)&(buffer[offset + 62]);

	generationCount = *(int*)&(buffer[offset + 64]);

	offset += 68 + ((sizeof(int) * 3) * generationCount);

	engineVersion = *(int*)&(buffer[offset]);

	cookerVersion = *(int*)&(buffer[offset + 4]);

	compressionFlags = *(int*)&(buffer[offset + 8]);


	file.close();
	return true;
}

bool Package::LoadExports()
{

	std::ifstream file(m_path, std::ios::binary);
	std::streampos offset = exportOffset;

	if (!file.is_open()) {
		std::cout << "Failed to open file!" << std::endl;
		return false;
	}

	const int ExportObjectSize = 68;
	char buffer[ExportObjectSize] = { 0 };


	for (int i = 0; i < exportCount; i++) {

		if (!file.seekg(offset)) {

			std::cout << "failed to seek" << std::endl;
			return false;
		}

		if (!file.read(buffer, ExportObjectSize)) {
			std::cout << "failed to read" << std::endl;
			return false;
		}

		ExportObject exportObj;

		exportObj.ObjTypeRef = *(int*)&(buffer);
		exportObj.ParentClassRef = *(int*)&(buffer[4]);
		exportObj.OwnerRef = *(int*)&(buffer[8]);
		int NameTableIdx = *(int*)&(buffer[12]);
		exportObj.name = m_nameTable[NameTableIdx];
		exportObj.NameCount = *(int*)&(buffer[16]);
		exportObj.Field6 = *(int*)&(buffer[20]);
		exportObj.ObjectFlagsH = *(int*)&(buffer[24]);
		exportObj.ObjectFlagsL = *(int*)&(buffer[28]);
		exportObj.ObjectFileSize = *(int*)&(buffer[32]);
		exportObj.DataOffset = *(int*)&(buffer[36]);
		exportObj.Field11 = *(int*)&(buffer[40]);
		exportObj.NumAdditionalFields = *(int*)&(buffer[44]);
		exportObj.Field13 = *(int*)&(buffer[48]);
		exportObj.Field14 = *(int*)&(buffer[52]);
		exportObj.Field15 = *(int*)&(buffer[56]);
		exportObj.Field16 = *(int*)&(buffer[60]);
		exportObj.Field17 = *(int*)&(buffer[64]);


		for (int i = 0; i < exportObj.NumAdditionalFields; i++) {
			int addfield = *(int*)&(buffer[68 + (i * sizeof(int))]);
			exportObj.AdditionalFields.push_back(addfield);
		}

		offset += ExportObjectSize + (exportObj.NumAdditionalFields * sizeof(int));

		m_exportTable.push_back(exportObj);

		
	}


	/*
	for (int i = 0; i < exportCount; i++) {

		std::cout << "Export #" << i << ": " << std::endl;
		PrintExport(m_exportTable[i]);
		
	}
	*/


	file.close();

	return true;
}

bool Package::LoadNames()
{

	std::ifstream file(m_path, std::ios::binary);
	std::streampos offset = nameOffset;

	const int NameObjectMaxSize = 76;
	char buffer[NameObjectMaxSize] = { 0 };

	int nameLength;
	
	for (int i = 0; i < nameCount; i++) {

		if (!file.seekg(offset)) {

			std::cout << "failed to seek" << std::endl;
		}
		
		if (!file.read(buffer, NameObjectMaxSize)) {
			
			std::cout << "failed to read" << std::endl;
		}

		nameLength = *(int*)&(buffer);

		std::string name(buffer + 4, nameLength);

		//std::cout << i << ": " << name << " Size: " << nameLength << " Offset: " << offset << std::endl;

		offset += (nameLength + 12);

		m_nameTable.push_back(name);
		
	}

	file.close();
	
	return true;
}

bool Package::SetYoungBlake()
{

	std::fstream file(m_path, std::ios::in | std::ios::out | std::ios::binary);
	std::streampos offset;

	char KidBlakeSpawnAOB[24] = {0x4D, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x24, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
	
	for (int i = 0; i < exportCount; i++) {

		const int bufferSize = m_exportTable[i].ObjectFileSize;
		char* buffer = (char*)alloca(bufferSize);
		

		offset = m_exportTable[i].DataOffset;
		file.seekg(offset);
		file.read(buffer, bufferSize);

		std::string aob(KidBlakeSpawnAOB, 24);
		std::string stringBuff(buffer, bufferSize);

		std::size_t n = stringBuff.find(aob);

		
		if (n == std::string::npos) {

			//std::cout << "Variable bYoungBlake not found" << std::endl;
		}
		else {

			std::cout << "Export #" << i << ' ' << m_exportTable[i].name << std::endl;
			std::cout << "--------------------------" << std::endl;
			std::cout << "bYoungBlake = " << ((int)buffer[n + 24] == 0 ? "False" : "True (SETTING TO FALSE)") << std::endl << std::endl;

			offset += (n + 24);

			const char boolBuff[1] = { 0 };

			file.seekg(offset);
			file.write(boolBuff, 1);

		}

	}


	file.close();
	std::cout << "Set all bYoungBlake variables to FALSE." << std::endl << std::endl;
	return true;
}


void Package::PrintExport(int index)
{
	if (index > m_exportTable.size()) {
		std::cout << "Cant print export object. Index out of range!" << std::endl;
		return;
	}

	std::cout << "Export #" << index << ":" << std::endl;





}


void Package::PrintExportOld(int index)
{

	if (index > m_exportTable.size()) {
		std::cout << "Cant print export object. Index out of range!" << std::endl;
		return;
	}
	// lots of printing values unfinished. index prints rather than actual value in table
	std::cout << "Export #" << index << ":" << std::endl;

	std::cout << "Object type reference: " << m_exportTable[index].ObjTypeRef << std::endl;
	std::cout << "Parent class reference: " << m_exportTable[index].ParentClassRef << std::endl;
	if (m_exportTable[index].OwnerRef) {
		std::cout << "Owner reference: " << m_exportTable[m_exportTable[index].OwnerRef - 1].name << std::endl;
	}
	std::cout << "Name: " << m_exportTable[index].name << std::endl;
	std::cout << "Name Count: " << m_exportTable[index].NameCount;
	if (!m_exportTable[index].NameCount)
		std::cout << " (Unique)" << std::endl;
	else
		std::cout << std::endl;
	std::cout << "Field 6: " << m_exportTable[index].Field6 << std::endl;
	std::cout << "Object Flags H: " << m_exportTable[index].ObjectFlagsH << std::endl;
	std::cout << "Object flags L (Internal): " << m_exportTable[index].ObjectFlagsL << std::endl;
	std::cout << "Object file Size: " << m_exportTable[index].ObjectFileSize << std::endl;
	std::cout << "Data Offset: " << m_exportTable[index].DataOffset << std::endl;
	std::cout << "Field 11: " << m_exportTable[index].Field11 << std::endl;
	std::cout << "Additional fields count: " << m_exportTable[index].NumAdditionalFields << std::endl;
	std::cout << "Field 13: " << m_exportTable[index].Field13 << std::endl;
	std::cout << "Field 14: " << m_exportTable[index].Field14 << std::endl;
	std::cout << "Field 15: " << m_exportTable[index].Field15 << std::endl;
	std::cout << "Field 16: " << m_exportTable[index].Field16 << std::endl;
	std::cout << "Field 17: " << m_exportTable[index].Field17 << std::endl;

	if (m_exportTable[index].NumAdditionalFields) {

		for (int i = 0; i < m_exportTable[index].NumAdditionalFields; i++) {
			std::cout << "Additonal Field " << i <<": " << m_exportTable[index].AdditionalFields[i] << std::endl;
		}

	}

	std::cout << std::endl;

}
