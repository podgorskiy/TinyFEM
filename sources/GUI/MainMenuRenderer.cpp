#include "MainMenuRenderer.h"
#include <ocornut-imgui/imgui.h>

MainMenuRenderer::MainMenuRenderer()
{
}

void MainMenuRenderer::RenderMainMenu()
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			ShowMenuFile();
			ImGui::EndMenu();
		}
		/*
		if (ImGui::BeginMenu("Edit"))
		{
			if (ImGui::MenuItem("Undo", "CTRL+Z")) {}
			if (ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {}  // Disabled item
			ImGui::Separator();
			if (ImGui::MenuItem("Cut", "CTRL+X")) {}
			if (ImGui::MenuItem("Copy", "CTRL+C")) {}
			if (ImGui::MenuItem("Paste", "CTRL+V")) {}
			ImGui::EndMenu();
		}*/
		if (ImGui::BeginMenu("Window"))
		{
			ShowMenuWindow();
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}

}

void MainMenuRenderer::ShowMenuFile()
{
#ifndef __EMSCRIPTEN__
	if (ImGui::MenuItem("New")) {
		m_newFileSignal();
	}
	if (ImGui::MenuItem("Open", "Ctrl+O"))
	{
		m_openFileSignal();
	}
	if (ImGui::MenuItem("Save", "Ctrl+S"))
	{
		m_saveFileSignal();
	}
	if (ImGui::MenuItem("Save As.."))
	{
		m_saveAsFileSignal();
	}
	ImGui::Separator();
	if (ImGui::BeginMenu("Options"))
	{
		ImGui::EndMenu();
	}

	if (ImGui::MenuItem("Quit", "Alt+F4"))
	{
		m_closeSignal();
	}
#else
    ImGui::MenuItem("Web version cannot open user files.", NULL, false, false);
    ImGui::MenuItem("You may open only embedded examples,", NULL, false, false);
    ImGui::MenuItem("that are listed below", NULL, false, false);
	if (ImGui::MenuItem("Open example PlateWithHole_CPS3")) {
		m_openExampleSignal(1);
	}
	if (ImGui::MenuItem("Open example PlateWithHole_CPS4")) {
		m_openExampleSignal(2);
	}
#endif
}

void MainMenuRenderer::ShowMenuWindow()
{
	if (ImGui::MenuItem("Mesh view options", NULL)) {
		m_showMeshViewOptions();
	}
	if (ImGui::MenuItem("Job options", NULL)) {
		m_showJobView();
	}
}

