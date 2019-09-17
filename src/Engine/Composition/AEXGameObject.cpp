#pragma once
// ----------------------------------------------------------------------------
// Project: GAM300 - Sample Engine
// File:	IComp.h
// Purpose:	Header file for the IComp API
// Author:	Thomas Komair
//
// Copyright DigiPen Institute of Technology
// ----------------------------------------------------------------------------
#include "AEXComposition.h"
#include "AEXFactory.h"
#include "AEXObjectManager.h"
#include "../Imgui/imgui.h"
#include "../Graphics/GfxMgr.h"
#include "../Core/AEXGlobalVariables.h"

#include <./extern/glad/glad.h>
#include <./extern/glfw/glfw3.h>

#include <iostream>

namespace AEX
{
	int GameObject::objectID = 0;
	// ----------------------------------------------------------------------------
	// ----------------------------------------------------------------------------
	// AEXOBJECT

	GameObject::GameObject() : IBase()
	{
		char * id_str;
		std::string ObjectName = GOdefaultName + std::to_string(objectID);
		ObjectManager::Instance()->AddObject(this, ObjectName.c_str());
		objectID++;

		AddComp(GetCompByName("TransformComp"));
	}
	GameObject::~GameObject()
	{}

	// ----------------------------------------------------------------------------
	#pragma region// STATE METHODS
	
	void GameObject::SetEnabled(bool enabled) // Call Set Enabled on all components
	{
		// avoid redundant work
		if (mbEnabled == enabled)
			return;

		// call base method
		mbEnabled = enabled;

		// delegate to components
		FOR_EACH(it, mComps)
			(*it)->SetEnabled(enabled);
	}
	void GameObject::Initialize()
	{
		// Initialize all comps 
		FOR_EACH(it, mComps)
			(*it)->Initialize();

		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		// Initialize Render data
		float vertices[] = {
			// positions          // colors           // texture coords
			0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
			0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
			-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
			-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
		};
		unsigned int indices[] = {  // note that we start from 0!
			0, 1, 3,   // first triangle
			1, 2, 3    // second triangle
		};

		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);

		// ..:: Initialization code :: ..
		// 1. bind Vertex Array Object
		glBindVertexArray(VAO);
		// 2. copy our vertices array in a vertex buffer for OpenGL to use
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		// 3. copy our index array in a element buffer for OpenGL to use
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
		// 4. then set the vertex attributes pointers
		// position attribute
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		// color attribute
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
		// texture coord attribute
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
		glEnableVertexAttribArray(2);
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
		// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
		glBindVertexArray(0);

		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		// Create a texture
		mTex = new Texture("data/Images/container.jpg");
	}

	void GameObject::Render()
	{
		glBindTexture(GL_TEXTURE_2D, mTex->GetID());
		
		// create transformations
		AEMtx44 mtx = AEMtx44::Scale(0.1f, 1.0f, 1.0f) * AEMtx44::Rotate(0.0f, 0.0f, tan((float)glfwGetTime())) * AEMtx44::Translate(1.0f, 0.0f, 0.0f);  // make sure to initialize matrix to identity matrix first

		// get matrix's uniform location and set matrix
		GfxMgr->mCurrentShader->use();
		GfxMgr->mCurrentShader->setMat4("transform", mtx);

		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	}

	void GameObject::Shutdown()
	{
		// shutdown all comps 
		FOR_EACH(it, mComps)
			(*it)->Shutdown();

		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);
		glDeleteBuffers(1, &EBO);

		delete mTex;
	}

	#pragma endregion

	// ----------------------------------------------------------------------------
	#pragma region// COMPONENT MANAGEMENT

	// Find Component
	u32 GameObject::GetCompCount() const
	{
		return mComps.size();
	}
	IComp* GameObject::GetComp(u32 index) const
	{
		if (index < GetCompCount())
			return mComps[index];
		return NULL;
	}
	IComp* GameObject::GetComp(const char * type) const
	{
		// go throught the components and look for the same type
		for (auto it = mComps.begin(); it != mComps.end(); ++it)
		{
			std::string typeName = (*it)->GetType().GetName();
			if (typeName == type)
				return (*it);
		}
		return NULL;
	}
	IComp* GameObject::GetComp(const Rtti & type) const
	{
		// go throught the components and look for the same type
		for (auto it = mComps.begin(); it != mComps.end(); ++it)
		{
			if ((*it)->GetType().IsExactly(type))
				return (*it);
		}
		return NULL;
	}
	IComp* GameObject::GetCompName(const char * compName, const char *compType) const
	{
		for (auto it = mComps.begin(); it != mComps.end(); ++it)
		{
			// found a match with the name
			if (strcmp((*it)->GetName(), compName) == 0)
			{
				// not same type -> continue
				if (compType && strcmp(compType, (*it)->GetType().GetName()) != 0)
					continue;
				// same type or don't care about type -> return
				return (*it);
			}
		}
		return NULL;
	}

	// Add/Remove by address
	IComp * GameObject::AddComp(IComp* pComp)
	{
		if (pComp) {
			pComp->mOwner = this;
			mComps.push_back(pComp);
		}
		return pComp;
	}
	void GameObject::RemoveComp(IComp* pComp)
	{
		if (!pComp)
			return;
		//pComp->Shutdown(); //TODO: make sure that we indeed don't need that
		// NOTE this will create memory leaks.
		for (auto it = mComps.begin(); it != mComps.end(); ++it)
		{
			if ((*it) == pComp)
			{
				pComp->mOwner = NULL;
				mComps.erase(it);
				return;
			}
		}
	}

	// Removes first component encoutered that match the search criteria
	void GameObject::RemoveCompType(const char * compType)
	{
		RemoveComp(GetComp(compType));
	}
	void GameObject::RemoveCompType(const Rtti & compType)
	{
		RemoveComp(GetComp(compType));
	}
	void GameObject::RemoveCompName(const char * compName, const char * compType)
	{
		RemoveComp(GetCompName(compName, compType));
	}

	// Removes all components encoutered that match the search criteria
	void GameObject::RemoveAllCompType(const char * compType)
	{
		IComp* pComp = GetComp(compType);
		while (pComp)
		{
			RemoveComp(pComp);
			pComp = GetComp(compType);
		}
	}
	void GameObject::RemoveAllCompType(const Rtti & compType)
	{
		IComp* pComp = GetComp(compType);
		while (pComp)
		{
			RemoveComp(pComp);
			pComp = GetComp(compType);
		}
	}
	void GameObject::RemoveAllCompName(const char * compName, const char * compType)
	{
		IComp* pComp = GetCompName(compName, compType);
		while (pComp)
		{
			RemoveComp(pComp);
			pComp = GetCompName(compName, compType);
		}
	}

	void GameObject::RemoveAllComp()
	{
		while (mComps.size())
		{
			mComps.back()->Shutdown();
			delete mComps.back();
			mComps.pop_back();
		}
	}

	json & GameObject::operator<<(json & j) const
	{
		// serialize name (todo: implement this as a Property). 
		j["Name"] = mName;

		// serialize components
		json & comps = j["comps"];
		for (u32 i = 0; i < this->GetComps().size(); ++i)
		{
			json compJson;
			IComp * comp = GetComp(i);
			compJson["__type"] = comp->GetType().GetName();

			// write the component  using stream operators

			// note only IComp3 implement it
			if (auto comp2 = dynamic_cast<IComp*>(comp))
			{
				comp2->operator<<(compJson);
			}

			// add to json array
			comps.push_back(compJson);
		}

		return j;
	}

	void GameObject::operator >> (json & j)
	{
		// get components
		Shutdown(); // clear everything before loading. 

					// read name (this should really be another property).
		SetName(j["Name"].get<std::string>().c_str());

		// read comps and allocate them
		json & comps = *j.find("comps");
		for (auto it = comps.begin(); it != comps.end(); ++it)
		{
			// get json object for that component
			json & compJson = *it;

			// read type and create with factory
			std::string typeName = compJson["__type"].get<std::string>();
			IBase * newComp = aexFactory->Create(typeName.c_str());

			// error check - Factory couldn't allocate memory
			if (newComp == nullptr)
				continue;

			// only stream components deriving from IComp2
			if (auto comp2 = dynamic_cast<IComp*>(newComp))
				comp2->operator >> (compJson);

			// add new comp object
			AddComp((IComp*)newComp);
		}
	}

	std::ostream & GameObject::operator<<(std::ostream & o) const
	{
		json j;
		this->operator<<(j);
		return o << j;
	}

	void GameObject::AddIfNotRepeatedComp(const std::vector<IComp *> & compList, const std::string & namespaceString, const std::string & compToCheck)
	{
		for (unsigned i = 0; i < compList.size(); i++)
		{
			std::string compName = compList[i]->GetType().GetName();
			if (compName == (namespaceString + compToCheck))
				return;
		}

		AddComp(GetCompByName(compToCheck.c_str()));
	}

	void GameObject::OnGui()
	{
		if (ImGui::Begin("Object Info"))
		{
			if (ImGui::TreeNode(mName))
			{
				for (u32 i = 0; i < mComps.size(); ++i)
				{
					mComps[i]->OnGui();
					ImGui::Separator();
				}

				ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0.0f, 0.6f, 0.6f));
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(0.0f, 0.7f, 0.7f));
				ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(0.0f, 0.8f, 0.8f));

				static bool addCompClicked = false;
				if (ImGui::Button("                    Add Component                    "))
					addCompClicked = !addCompClicked;

				if (addCompClicked)
				{
					static ImGuiComboFlags flags = 0;
					std::string aexNamespace = "AEX::";
					const char* items[] = { "TransformComp", "RigidBody", "Collider" };
					static const char* item_current = items[0];            // Here our selection is a single pointer stored outside the object.
					if (ImGui::BeginCombo("Component list", item_current, flags)) // The second parameter is the label previewed before opening the combo.
					{
						int n;
						for (n = 0; n < IM_ARRAYSIZE(items); n++)
						{
							bool is_selected = (item_current == items[n]);
							if (ImGui::Selectable(items[n], is_selected))
							{
								item_current = items[n];
								std::string itemName_str = item_current;
								AddIfNotRepeatedComp(mComps, aexNamespace, itemName_str);
								addCompClicked = false;
							}
						}

						ImGui::EndCombo();
					}
				}

				ImGui::PopStyleColor(3);
				ImGui::TreePop();
			}
			if (ImGui::BeginPopupContextItem("item context menu"))
			{
				ImGui::Text("Edit name:");
				ImGui::InputText("##edit", mName, IM_ARRAYSIZE(mName));
				if (ImGui::Button("Close"))
					ImGui::CloseCurrentPopup();
				ImGui::EndPopup();
			}
		}
		ImGui::End();
	}

	#pragma endregion

	#pragma region // SERIALIZATION
	nlohmann::json & operator<<(nlohmann::json & j, const GameObject & obj)
	{
		obj.operator<<(j);
		return j;
	}
	GameObject & operator >> (nlohmann::json & j, GameObject & obj)
	{
		obj.operator >> (j);
		return obj;
	}
	std::ostream & operator<<(std::ostream & o, const GameObject & obj)
	{
		json j;
		obj.operator<<(j);
		return o << j;
	}
	#pragma endregion
}