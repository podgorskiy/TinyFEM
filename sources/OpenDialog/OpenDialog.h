#include <string>

class OpenSaveFileDialog
{
public:
	OpenSaveFileDialog();
	void CreateOpenFileDialog(const std::string& options, bool save);
	bool OpenSucceeded();
	std::string GetFileName();

private:
	std::string m_filename;
	bool m_openSucceeded;
};