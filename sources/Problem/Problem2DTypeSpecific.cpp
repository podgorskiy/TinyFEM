#include "Problem2DTypeSpecific.h"
#include "Problem.h"
#include "MeshRender/MeshRenderer.h"

#include <imgui/imgui.h>

int Problem2DTypeSpecific::GetProblemDimension(int nodesCount)
{
	return GetDOFPerNode() * nodesCount;
}

void Problem2DTypeSpecific::GrabConstraintIndices(int node, NodalConstraint::ConstraintType type, std::vector<int> & indices)
{
	int ix = 2 * node + 0;
	int iy = 2 * node + 1;
	if (type & NodalConstraint::UX)
	{
		indices.push_back(ix);
	}
	if (type & NodalConstraint::UY)
	{
		indices.push_back(iy);
	}
}

int Problem2DTypeSpecific::GetDOFPerNode()
{
	return 2;
}

void Problem2DTypeSpecific::SolveProblem(Problem& problem)
{
	problem.GenerateGlobalKMatrix();
	problem.ApplyConstrints();
	problem.CreateLoadVector();
	problem.SolveSLI();
}

void Problem2DTypeSpecific::UpdateNodes(const Problem& problem, MeshRenderer* meshRenderer) const
{
	std::vector<float> m_avarageValues;
	float max = FLT_MIN;
	float min = FLT_MAX;
	if (problem.GetDeforms().size() == problem.GetNodes().GetCount() * problem.GetNodes().GetDof())
	{
		m_avarageValues.resize(problem.GetNodes().GetCount(), 0.0f);
		std::vector<int> count;
		count.resize(problem.GetNodes().GetCount(), 0);
		std::vector<Eigen::Vector3f> valuesAtNodes;
		for (int i = 0; i < problem.GetElements().GetCount(); ++i)
		{
			valuesAtNodes = problem.GetElements()(i)->GetFunctionValuesAtNodes(problem.GetDeforms());
			std::vector<int> ind = problem.GetElements()(i)->GetIndices();
			int j = 0;
			for (std::vector<int>::iterator it = ind.begin(); it != ind.end(); ++it)
			{
				Eigen::Vector2f uv(problem.GetDeforms()[*it * 2 + 0], problem.GetDeforms()[*it * 2 + 1]);
				m_avarageValues[*it] += GetValue(valuesAtNodes[j++], problem.GetElements()(i), uv);
				count[*it]++;
			}
		}
		int j = 0;
		for (std::vector<float>::iterator it = m_avarageValues.begin(); it != m_avarageValues.end(); ++it)
		{
			(*it) /= static_cast<float>(count[j++]);
			if (*it > max){ max = *it; }
			if (*it < min){ min = *it; }
		}
		meshRenderer->SetRanges(min, max);

		for (std::vector<float>::iterator it = m_avarageValues.begin(); it != m_avarageValues.end(); ++it)
		{
			*it = (*it - min) / (max - min);
		}
	}

	meshRenderer->SetNodes(problem.GetNodes(), problem.GetDeforms(), m_avarageValues);
	LOGI("Updated nodes.");
}

void Problem2DTypeSpecific::SubmitMeshesToRender(const Problem& problem, MeshRenderer* meshRenderer) const
{
	meshRenderer->PushMesh(
		problem.GetElements(), 0, problem.GetElements().GetCount(), 
		problem.GetNodalForceList(),
		problem.GetNodalConstraintList());
}

float MaxPrincipal(float x, float y, float xy)
{
	return (x + y) / 2.0f + sqrt(pow((x - y) / 2.0f, 2.0f) + pow(xy / 2.0f, 2.0f));
}

float MinPrincipal(float x, float y, float xy)
{
	return (x + y) / 2.0f - sqrt(pow((x - y) / 2.0f, 2.0f) + pow(xy / 2.0f, 2.0f));
}

float Mises(float x, float y, float xy)
{
	return sqrt(x*x - x*y + y*y + 3*xy*xy);
}

float TransformTensor(float x, float y, float xy, Problem2DTypeSpecific::Component component)
{
	float value = 0.0f;
	switch (component)
	{
	case Problem2DTypeSpecific::C_11:
	{
		value = x;
		break;
	}
	case Problem2DTypeSpecific::C_22:
	{
		value = y;
		break;
	}
	case Problem2DTypeSpecific::C_12:
	{
		value = xy;
		break;
	}
	case Problem2DTypeSpecific::C_Max:
	{
		value = MaxPrincipal(x, y, xy);
		break;
	}
	case Problem2DTypeSpecific::C_Min:
	{
		value = MinPrincipal(x, y, xy);
		break;
	}
	case Problem2DTypeSpecific::C_Mises:
	{
		value = Mises(x, y, xy);
		break;
	}
	}
	return value;
}

float Problem2DTypeSpecific::GetValue(const Eigen::Vector3f& input, IElement* element, const Eigen::Vector2f& uv) const
{
	float value = 0.0f;
	switch (m_display)
	{
		case D_U:
		{
			switch (m_component)
			{
				case C_1:
				{
					value = uv.x();
					break;
				}
				case C_2:
				{
					value = uv.y();
					break;
				}
				case C_Magnitude:
				{
					value = sqrt(uv.x()*uv.x() + uv.y()*uv.y());
					break;
				}
			}
			break;
		}
		case D_E:
		{
			value = TransformTensor(input.x(), input.y(), input.z(), m_component);
			break;
		}
		case D_S:
		{
			tfem::Material* mat = element->GetMaterial();
			Eigen::Vector3f S =  mat->GetElasticityMatrix(fem::PT_FlatStress) * input;
			value = TransformTensor(S.x(), S.y(), S.z(), m_component);
			break;
		}
	}
	return value;
}

std::string plotOptionTitle = "Display options";

bool Problem2DTypeSpecific::UpdateView(MeshRenderer* meshRenderer)
{
	bool needUpdate = false;
	static bool m_showMeshViewOption = true;
	if (ImGui::Begin(plotOptionTitle.c_str()))// , &m_showMeshViewOption))
	{
		ImVec2 vpos;
		int width = 400;
		vpos.x = 0;
		vpos.y = 21 + 160;
		ImGuiSetCond_ cond = ImGuiSetCond_Appearing;
		ImGui::SetWindowPos(plotOptionTitle.c_str(), vpos, cond);
		ImVec2 vsize;
		vsize.x = width;
		vsize.y = 120;
		ImGui::SetWindowSize(plotOptionTitle.c_str(), vsize, cond);

		static int displayItem = 0;
		ImGui::PushItemWidth(300.0f);
		{
			const char* items[] = {
				"U", "E", "S"
			};
			needUpdate = needUpdate || ImGui::Combo("Plot", &displayItem, items, 3);
		}
		{
			const char* itemsU[] = {
				"U1", "U2", "Magnitude"
			};
			const char* itemsE[] = {
				"E11", "E22", "E12", "Max. Principal", "Min. Principal"
			};
			const char* itemsS[] = {
				"S11", "S22", "S12", "Max. Principal", "Min. Principal", "Mises"
			};
			static int componentitem = 2;
			if (needUpdate)
			{
				componentitem = displayItem == 0 ? 2 : displayItem == 1 ? 3 : 5;
			}

			const char** items = NULL;
			int count = 0;
			switch (displayItem)
			{
			case 0: items = itemsU; count = 3; break;
			case 1: items = itemsE; count = 5; break;
			case 2: items = itemsS; count = 6; break;
			}

			needUpdate = needUpdate || ImGui::Combo("Component", &componentitem, items, count);
			
			static int intervals = 10;
			ImGui::SliderInt("", &intervals, 2, 24, "Contour intervals %0.f");
			meshRenderer->SetIntervals(intervals);


			m_display = static_cast<Display>(displayItem);
			m_component = static_cast<Component>(componentitem);
			
		}
		ImGui::PopItemWidth();
	}
	ImGui::End();
	return needUpdate;
}