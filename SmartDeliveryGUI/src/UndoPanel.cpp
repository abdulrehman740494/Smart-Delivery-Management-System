#include "Panels.h"
#include "UndoStack.h"
#include <imgui.h>


void renderUndoPanel() {
    static std::string lastResult;

    ImGui::Text("Undoes the most recent package or driver deletion.");
    ImGui::Spacing();

    ImGui::BeginDisabled(!canUndo());
    if (ImGui::Button("Undo Last Delete", ImVec2(220, 40))) {
        lastResult = performUndo();
        if (lastResult.empty()) lastResult = "Nothing was undone.";
    }
    ImGui::EndDisabled();

    if (!canUndo()) {
        ImGui::SameLine();
        ImGui::TextDisabled("(nothing to undo)");
    }

    if (!lastResult.empty()) {
        ImGui::Spacing();
        ImGui::TextColored(ImVec4(0.5f, 0.9f, 0.5f, 1.0f), "%s", lastResult.c_str());
    }
}
