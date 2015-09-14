#pragma once
#include <string>

class OpenFileState
{
public:
	OpenFileState();
	bool OpenFile();
	void CloseFile();
	void SaveFile(bool as);
	std::string GetFileName();
private:
	bool IsSomeFileOpened();
	std::string m_openedFile;
};
