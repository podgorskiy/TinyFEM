#include "Common.h"

#include "Application.h"
#include "OpenDialog/OpenDialog.h"
#include "MeshRender/MeshRenderer.h"

#include <bx/timer.h>
#include <bx/fpumath.h>

#include <entry/entry.h>
#include <bgfx_utils.h>

#include <bgfx.h>
#include <bx/timer.h>
#include <bx/fpumath.h>
#include <imgui/imgui.h>

#include <font/font_manager.h>
#include <font/text_metrics.h>
#include <font/text_buffer_manager.h>

#include <ocornut-imgui/imgui.h>
#include "GUI/MainMenuRenderer.h"

#include <boost/bind.hpp>

#include "../embeddedExamples/embeddedExample1.h"
#include "../embeddedExamples/embeddedExample2.h"

void Application::OpenFile()
{
	if (m_openstate.OpenFile())
	{
		m_problem.OpenFromFile(m_openstate.GetFileName());
		m_meshRenderer->Clear();
		m_problem.UpdateNodes(m_meshRenderer);
		m_problem.SubmitMeshesToRender(m_meshRenderer);
	}
}

void Application::OpenExample(int i)
{
	std::vector<char> file;

	if (i == 1)
	{
		file.resize(sizeof(embeddedExample1));
		memcpy(&file[0], embeddedExample1, sizeof(embeddedExample1));
	}
	if (i == 2)
	{
		file.resize(sizeof(embeddedExample2));
		memcpy(&file[0], embeddedExample2, sizeof(embeddedExample2));
	}

	file.push_back(0);
	m_problem.OpenFromMemory(&file[0]);
	m_meshRenderer->Clear();
	m_problem.UpdateNodes(m_meshRenderer);
	m_problem.SubmitMeshesToRender(m_meshRenderer);
}

void Application::SaveFile()
{
	m_openstate.SaveFile(false);
}

void Application::SaveFileAs()
{
	m_openstate.SaveFile(true);
}

void Application::CloseFile()
{
	m_openstate.CloseFile();
}


void Application::init(int /*_argc*/, char** /*_argv*/)
{
	m_requestClose = false;
	m_width = 1280;
	m_height = 720;
	m_debug = BGFX_DEBUG_TEXT;
	m_reset = BGFX_RESET_VSYNC;// | BGFX_RESET_MSAA_X16;

	bgfx::init(bgfx::RendererType::OpenGL);

	bgfx::reset(m_width, m_height, m_reset);

	// Enable debug text.
	bgfx::setDebug(m_debug);

	// Set view 0 clear state.
	bgfx::setViewClear(0
		, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH
		, 0x303030ff
		, 1.0f
		, 0
		);

	// Imgui.
	imguiCreate();

	m_guiRenderer = new MainMenuRenderer;
	m_meshRenderer = new MeshRenderer;
	m_meshRenderer->Init();
	m_camera.Reset();

	m_showJobView = true;

	m_guiRenderer->m_closeSignal.connect(boost::bind(&Application::RequestClose, this));
	m_guiRenderer->m_newFileSignal.connect(boost::bind(&Application::CloseFile, this));
	m_guiRenderer->m_saveFileSignal.connect(boost::bind(&Application::SaveFile, this));
	m_guiRenderer->m_saveAsFileSignal.connect(boost::bind(&Application::SaveFileAs, this));
	m_guiRenderer->m_openFileSignal.connect(boost::bind(&Application::OpenFile, this));
	m_guiRenderer->m_showMeshViewOptions.connect(boost::bind(&MeshRenderer::ShowMeshViewOptions, m_meshRenderer));
	m_guiRenderer->m_showJobView.connect(boost::bind(&Application::ShowJobView, this));
	m_guiRenderer->m_openExampleSignal.connect(boost::bind(&Application::OpenExample, this, _1));
}

void Application::RequestClose()
{
	m_requestClose = true;
}

bool Application::ProcessEvent()
{
	return entry::processEvents(m_width, m_height, m_debug, m_reset, &m_mouseState);
}

double Application::GetFPS()
{
	int64_t now = bx::getHPCounter();
	static int64_t last = now;
	const int64_t frameTime = now - last;
	last = now;
	const double freq = double(bx::getHPFrequency());
	const double toMs = 1000.0 / freq;
	static double frameTimeInMs = 0;
	frameTimeInMs += frameTime * toMs;
	static int framesCount = 0;
	framesCount++;
	static double fps = 0;
	if (frameTimeInMs> 500)
	{
		fps = framesCount / frameTimeInMs * 1000;
		frameTimeInMs = 0;
		framesCount = 0;
	}
	return fps;
}

std::string jonViewTitle = "Job view";

bool Application::update()
{
	if (ProcessEvent() )
	{
		return false;
	}

	m_camera.SetWidthHeight(m_width, m_height);
	m_camera.Update(m_mouseState);

	int32_t scrollArea = 0;
	const int32_t guiPanelWidth = 250;
	const int32_t guiPanelHeight = 200;
	float textScroll = 0.0f;
	float textRotation = 0.0f;
	float textScale = 1.0f;
	float textSize = 14.0f;
	float visibleLineCount = 20.0f;


	imguiBeginFrame(m_mouseState.m_mx
		, m_mouseState.m_my
		, (m_mouseState.m_buttons[entry::MouseButton::Left] ? IMGUI_MBUT_LEFT : 0)
		| (m_mouseState.m_buttons[entry::MouseButton::Right] ? IMGUI_MBUT_RIGHT : 0)
		| (m_mouseState.m_buttons[entry::MouseButton::Middle] ? IMGUI_MBUT_MIDDLE : 0)
		, m_mouseState.m_mz
		, m_width
		, m_height
		);

	m_guiRenderer->RenderMainMenu();
	
	//ImGui::ShowTestWindow();
	
	if (m_showJobView)
	{
		if (ImGui::Begin(jonViewTitle.c_str(), &m_showJobView))
		{
			const char* items[] = {
				"Direct sparse LDLT Cholesky factorizations",
				"Direct sparse LLT Cholesky factorizations",
				"Iterative conjugate gradient solver"
			};
			static int item = 0;
			static int maxIterationCount = 100;
			static float tolerance = 1e-5;
			ImGui::PushItemWidth(300.0f);
			ImGui::Combo("Solver type", &item, items, 3);
			ImGui::PopItemWidth();

			if (item == 2)
			{
				ImGui::PushItemWidth(100.0f);
				ImGui::InputInt("Max number of iterations", &maxIterationCount, 1, 10);
				ImGui::InputFloat("Tolerance", &tolerance, 0.0f, 0.0f, -1);
				ImGui::PopItemWidth();
			}

			if (ImGui::Button("Solve", ImVec2(100, 40)))
			{
				m_problem.SetSolverOptions(static_cast<Problem::SolverType>(item), maxIterationCount, tolerance);
				m_problem.SolveProblem();
				m_meshRenderer->Clear();
				m_problem.UpdateNodes(m_meshRenderer);
				m_problem.SubmitMeshesToRender(m_meshRenderer);
			}
			ResetJobViewWindowPosition(false);
		}
		ImGui::End();
	}

	bool needUpdate = m_problem.UpdateViewOption(m_meshRenderer);
	if (needUpdate)
	{
		m_problem.UpdateNodes(m_meshRenderer);
	}

	// Set view 0 default viewport.
	bgfx::setViewRect(0, 0, 0, m_width, m_height);

	m_camera.UpplyTransform();

	m_meshRenderer->Render(m_width, m_height, m_camera);

	imguiEndFrame();


	bgfx::touch(0);


	//bgfx::dbgTextClear();
	//bgfx::dbgTextPrintf(0, 1, 0x0f, "bgfx/examples/11-fontsdf");
	//bgfx::dbgTextPrintf(0, 2, 0x0f, "Frame: % 7.3f[fps]", GetFPS());

	bgfx::frame();
	
	if (m_requestClose)
	{
		return false;
	}

	return true;
}

void Application::ShowJobView()
{
	if (!m_showJobView)
	{
		m_showJobView = true;
		ImGui::SetWindowFocus(jonViewTitle.c_str());
		ResetJobViewWindowPosition(true);
	}
}

void Application::ResetJobViewWindowPosition(bool force)
{
	ImVec2 vpos;
	int width = 400;
	vpos.x = 0;
	vpos.y = 21;
	ImGuiSetCond_ cond = force ? ImGuiSetCond_Always : ImGuiSetCond_Appearing;
	ImGui::SetWindowPos(jonViewTitle.c_str(), vpos, cond);
	ImVec2 vsize;
	vsize.x = width;
	vsize.y = 160;
	ImGui::SetWindowSize(jonViewTitle.c_str(), vsize, cond);
}

int Application::shutdown()
{
	delete m_guiRenderer;
	delete m_meshRenderer;
	imguiDestroy();
	// Shutdown bgfx.
	bgfx::shutdown();
	return 0;
}

ENTRY_IMPLEMENT_MAIN(Application);