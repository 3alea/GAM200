#include "src\Engine\Composition\AEXSerialization.h"
#include "../Composition/AEXComponent.h"
#include "../Composition/AEXGameObject.h"
#include "AEXCollider.h"
#include "AEXComponents.h"
#include "../Imgui/imgui.h"

namespace AEX
{
	void Collider::Update()
	{
		if (mCollisionShape == CSHAPE_AABB && (int)(GetTransformComp->mLocal.mOrientation) % 90)
			mCollisionShape = CSHAPE_OBB;
		else if (mCollisionShape == CSHAPE_OBB && (int)(GetTransformComp->mLocal.mOrientation) % 90 == 0)
			mCollisionShape = CSHAPE_AABB;
	}

	json & Collider::operator<<(json & j) const
	{
		j["isGhost"] << IsGhost;
		j["collisionShape"] << mCollisionShape;
		j["dynamicState"] << DynamicState;
		return j;
	}

	void Collider::operator>>(json & j)
	{
		if (j.find("isGhost") != j.end())
			j["isGhost"] >> IsGhost;
		if (j.find("collisionShape") != j.end())
			j["collisionShape"] >> mCollisionShape;
		if (j.find("dynamicState") != j.end())
			j["dynamicState"] >> DynamicState;
	}

	std::ostream & Collider::operator<<(std::ostream & o) const
	{
		json j;
		this->operator<<(j);
		return o << j;
	}

	void Collider::OnGui()
	{
		if (ImGui::CollapsingHeader("Collider"))
		{
			//ImGui::Text("IsGhost"); ImGui::SameLine();
			ImGui::Checkbox("IsGhost", &IsGhost);

			ImGui::Text("Collision Shape");
			static int cShape = CSHAPE_AABB;
			// @WARNING : might set directly to AABB or OBB, just in case I added OBB
			ImGui::RadioButton("AABB", &cShape, CSHAPE_AABB); ImGui::SameLine();
			ImGui::RadioButton("OBB", &cShape, CSHAPE_OBB); ImGui::SameLine();
			ImGui::RadioButton("Circle", &cShape, CSHAPE_CIRCLE); 
			mCollisionShape = static_cast<ECollisionShape>(cShape);

			ImGui::Text("Dynamic State");
			static int dState = Dynamic;
			ImGui::RadioButton("Dynamic", &dState, Dynamic); ImGui::SameLine();
			ImGui::RadioButton("Static", &dState, Static); 
			DynamicState = static_cast<EDynamicState>(dState);

			/*static ImGuiComboFlags cShape_flags = 0;
			const char* cShape_items[] = { "Box", "Circle" };
			static const char* cShape_item_current = cShape_items[0];            // Here our selection is a single pointer stored outside the object.
			if (ImGui::BeginCombo("Collision shape", cShape_item_current, cShape_flags)) // The second parameter is the label previewed before opening the combo.
			{
				int n;
				for (n = 0; n < IM_ARRAYSIZE(cShape_items); n++)
				{
					bool is_selected = (cShape_item_current == cShape_items[n]);
					if (ImGui::Selectable(cShape_items[n], is_selected))
					{
						cShape_item_current = cShape_items[n];

						switch (n)
						{
						case 0:
							mCollisionShape = CSHAPE_AABB;
							break;
						case 1:
							mCollisionShape = CSHAPE_CIRCLE;
							break;
						}
					}
					if (is_selected)
						ImGui::SetItemDefaultFocus();   // Set the initial focus when opening the combo (scrolling + for keyboard navigation support in the upcoming navigation branch)
				}

				ImGui::EndCombo();
			}

			static ImGuiComboFlags dState_flags = 0;
			const char* dState_items[] = { "Dynamic", "Static" };
			static const char* dState_item_current = dState_items[0];            // Here our selection is a single pointer stored outside the object.
			if (ImGui::BeginCombo("Dynamic state", dState_item_current, dState_flags)) // The second parameter is the label previewed before opening the combo.
			{
				int n;
				for (n = 0; n < IM_ARRAYSIZE(dState_items); n++)
				{
					bool is_selected = (dState_item_current == dState_items[n]);
					if (ImGui::Selectable(dState_items[n], is_selected))
					{
						dState_item_current = dState_items[n];

						switch (n)
						{
						case 0:
							DynamicState = Dynamic;
							break;
						case 1:
							DynamicState = Static;
							break;
						}
					}
					if (is_selected)
						ImGui::SetItemDefaultFocus();   // Set the initial focus when opening the combo (scrolling + for keyboard navigation support in the upcoming navigation branch)
				}

				ImGui::EndCombo();
			}*/
		}
	}
}