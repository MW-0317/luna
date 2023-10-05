#pragma once

#include "thirdparty/imgui/imgui.h"
#include <map>
#include <vector>

struct DefaultLog
{
	ImGuiTextBuffer Buf;
	ImGuiTextFilter Filter;
	ImVector<int> LineOffsets;
	bool AutoScroll;

	DefaultLog()
	{
		AutoScroll = true;
		clear();
	}

	void clear()
	{
		Buf.clear();
		LineOffsets.clear();
		LineOffsets.push_back(0);
	}

	void log(const char* fmt, ...) IM_FMTARGS(2)
	{
		int oldSize = Buf.size();
		va_list args;
		va_start(args, fmt);
		Buf.appendfv(fmt, args);
		va_end(args);
		for (int newSize = Buf.size(); oldSize < newSize; oldSize++)
			if (Buf[oldSize] == '\n')
				LineOffsets.push_back(oldSize + 1);
	}

	void draw(const char* title, bool* p_open = NULL)
	{
		if (!ImGui::Begin(title, p_open))
		{
			ImGui::End();
			return;
		}

		if (ImGui::BeginPopup("Options"))
		{
			ImGui::Checkbox("Auto-scroll", &AutoScroll);
			ImGui::EndPopup();
		}

		if (ImGui::Button("Options"))
			ImGui::OpenPopup("Options");
		ImGui::SameLine();
		bool clear = ImGui::Button("Clear");
		ImGui::SameLine();
		bool copy = ImGui::Button("Copy");
		ImGui::SameLine();
		Filter.Draw("Filter", -100.0f);

		ImGui::Separator();

		if (ImGui::BeginChild("scrolling", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar))
		{
			if (clear) this->clear();
			if (copy) ImGui::LogToClipboard();

			ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
			const char* buf = Buf.begin();
			const char* bufEnd = Buf.end();
			if (Filter.IsActive())
			{
				for (int i = 0; i < LineOffsets.Size; i++)
				{
					const char* lineStart = buf + LineOffsets[i];
					const char* lineEnd = (i + 1 < LineOffsets.Size) ? (buf + LineOffsets[i + 1] - 1) : bufEnd;
					if (Filter.PassFilter(lineStart, lineEnd))
						ImGui::TextUnformatted(lineStart, lineEnd);
				}
			}
			else
			{
				ImGuiListClipper clipper;
				clipper.Begin(LineOffsets.Size);
				while (clipper.Step())
				{
					for (int i = clipper.DisplayStart; i < clipper.DisplayEnd; i++)
					{
						const char* lineStart = buf + LineOffsets[i];
						const char* lineEnd = (i + 1 < LineOffsets.Size) ? (buf + LineOffsets[i + 1] - 1) : bufEnd;
						ImGui::TextUnformatted(lineStart, lineEnd);
					}
				}
				clipper.End();
			}
			ImGui::PopStyleVar();

			if (AutoScroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
				ImGui::SetScrollHereY(1.0f);
		}

		ImGui::EndChild();
		ImGui::End();
	}
};

struct Overlay
{
	std::map<std::string, float> overlay;
	std::vector<std::string> keys;

	Overlay()
	{

	}

	void update(std::string text, float value)
	{
		overlay[text] = value;
		for (int i = 0; i < keys.size(); i++)
		{
			if (keys[i] == text)
				return;
		}
		keys.push_back(text);
	}

	void draw(const char* title, bool* p_open = NULL)
	{
		const float PAD = 10.f;
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration 
			| ImGuiWindowFlags_AlwaysAutoResize 
			| ImGuiWindowFlags_NoSavedSettings 
			| ImGuiWindowFlags_NoFocusOnAppearing 
			| ImGuiWindowFlags_NoNav;

		const ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImVec2 workPos = viewport->WorkPos;
		ImVec2 workSize = viewport->WorkSize;
		ImGui::SetNextWindowPos(ImVec2(workPos.x + PAD, workPos.y + PAD), 
			ImGuiCond_Always, 
			ImVec2(0.0f, 0.0f));
		window_flags |= ImGuiWindowFlags_NoMove;

		ImGui::SetNextWindowBgAlpha(0.35f);
		if (ImGui::Begin("Overlay", p_open, window_flags))
		{
			for (int i = 0; i < keys.size(); i++)
			{
				ImGui::Text("%s: %f", keys[i].c_str(), overlay[keys[i]]);
			}
		}
		ImGui::End();
	}
};