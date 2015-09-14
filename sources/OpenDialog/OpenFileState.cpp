#include "OpenFileState.h"
#include "OpenDialog.h"

OpenFileState::OpenFileState(){};

void OpenFileState::SaveFile(bool as)
{
	if (IsSomeFileOpened() && !as)
	{
		//save file
	}
	else
	{
		OpenSaveFileDialog od;
		od.CreateOpenFileDialog("Input file (*.inp)|*.inp||", true);
		if (od.OpenSucceeded())
		{
			m_openedFile = od.GetFileName();
			printf("Saving file: %s\n", od.GetFileName().c_str());
		}
	}
}

bool OpenFileState::OpenFile()
{
	OpenSaveFileDialog od;
	od.CreateOpenFileDialog("Input file (*.inp)|*.inp||", false);
	if (od.OpenSucceeded())
	{
		m_openedFile = od.GetFileName();
		printf("Opening file: %s\n", od.GetFileName().c_str());
		return true;
	}
	return false;
}

void OpenFileState::CloseFile()
{
	//close
	m_openedFile = "";
}

std::string OpenFileState::GetFileName()
{
	return m_openedFile;
}


bool OpenFileState::IsSomeFileOpened()
{
	return !(m_openedFile == "");
}
