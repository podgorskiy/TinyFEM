#pragma once
#include <boost/signals2.hpp>


class MainMenuRenderer
{
public:
	MainMenuRenderer();
	void RenderMainMenu();

	boost::signals2::signal<void()> m_newFileSignal;
	boost::signals2::signal<void()> m_openFileSignal;
	boost::signals2::signal<void()> m_saveFileSignal;
	boost::signals2::signal<void()> m_saveAsFileSignal;
	boost::signals2::signal<void()> m_closeSignal;
	boost::signals2::signal<void()> m_showMeshViewOptions;

private:
	void ShowMenuFile();
	void ShowMenuWindow();
};