#pragma once

#include <entry/entry.h>
#include <string>

#include "Problem/Problem.h"
#include "OpenDialog/OpenFileState.h"
#include "MeshRender/Camera.h"

class MainMenuRenderer;
class MeshRenderer;

class Application: public  entry::AppI
{
public:
	virtual void init(int /*_argc*/, char** /*_argv*/);
	virtual bool update();
	virtual int shutdown();
	bool ProcessEvent();
	double GetFPS();

	void RequestClose();

	void OpenFile();
	void OpenExample(int i);
	void CloseFile();
	void SaveFile();
	void SaveFileAs();
private:
	void ShowJobView();
	void ResetJobViewWindowPosition(bool force);
	uint32_t m_width;
	uint32_t m_height;
	uint32_t m_debug;
	uint32_t m_reset;
	entry::MouseState m_mouseState;
	bool m_requestClose;
	OpenFileState m_openstate;
	MainMenuRenderer* m_guiRenderer;
	Problem m_problem;
	MeshRenderer* m_meshRenderer;
	Camera m_camera;

	bool m_showJobView;
};