#include "imgui_manager.h"

imgui_manager::imgui_manager()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
}

imgui_manager::~imgui_manager()
{
	ImGui::DestroyContext();
}
