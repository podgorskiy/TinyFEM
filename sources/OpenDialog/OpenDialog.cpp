#include "Common.h"
#include "OpenDialog.h"

#ifdef _WIN32
#include <Windows.h>
#endif

void OpenSaveFileDialog::CreateOpenFileDialog(const std::string& options, bool save)
{
#ifdef _WIN32
	char filename[2048] = { 0 };
	char filter[2048] = { 0 };
	OPENFILENAME openstruct;
	ZeroMemory(&openstruct, sizeof(openstruct)); 
	openstruct.lStructSize = sizeof(openstruct);
	openstruct.hwndOwner = NULL;
	openstruct.lpstrFile = filename;
	openstruct.lpstrFile[0] = '\0';
	openstruct.nMaxFile = sizeof(filename);
	
	strcpy(filter, options.c_str());
	for (int i = 0; i < sizeof(filter); ++i)
	{
		if (filter[i] == '|')
		{
			filter[i] = '\0';
		}
	}
	openstruct.lpstrFilter = filter;

	openstruct.nFilterIndex = 1;
	openstruct.lpstrFileTitle = NULL;
	openstruct.nMaxFileTitle = 0;
	openstruct.lpstrInitialDir = NULL;
	openstruct.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
	if (save)
	{
		openstruct.lpstrTitle = "Save dialog";
		m_openSucceeded = GetSaveFileName(&openstruct);
	}
	else
	{
		openstruct.lpstrTitle = "Open dialog";
		m_openSucceeded = GetOpenFileName(&openstruct);
	}
	m_filename = filename;
#else
	m_openSucceeded = false;
#endif
}

bool OpenSaveFileDialog::OpenSucceeded()
{
	return m_openSucceeded;
}

std::string OpenSaveFileDialog::GetFileName()
{
	return m_filename;
}

OpenSaveFileDialog::OpenSaveFileDialog() :m_openSucceeded(false)
{
}
