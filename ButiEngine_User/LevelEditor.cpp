#include "stdafx_u.h"
#include "LevelEditor.h"

void ButiEngine::LevelEditor::OnUpdate()
{
}

void ButiEngine::LevelEditor::OnSet()
{

	InputCereal(m_currentData, "Scene/" + GetManager().lock()->GetScene().lock()->GetSceneInformation()->GetSceneName() + "/levelData.lvd");
	if (m_currentData.list_data.GetSize() == 0) {
		m_currentData.list_data.Add(LevelData());
	}
	RecreatePreview();
}

void ButiEngine::LevelEditor::OnRemove()
{
	ReleasePreview();
}

template <typename T>
bool PartTypeButton(const std::string& arg_settingName, const T arg_type, T& arg_ref_drawInfo) {
	bool isSame = arg_ref_drawInfo == arg_type,ret=false;
	if (isSame) {
		ButiEngine::GUI::SetButtonColor(ButiEngine::Vector4(0.8f, 0.8f, 0.9f, 1.0f));
	}
	if (ret=ButiEngine::GUI::Button(arg_settingName)) {
		arg_ref_drawInfo = arg_type;
	}
	if (isSame) {

		ButiEngine::GUI::PopStyleColor(1);
	}
	return ret;
}
void ButiEngine::LevelEditor::OnShowUI()
{
	if (GUI::ArrowButton("##sub", GUI::GuiDir_Left)) {
		m_currentEditLevelIndex--;
		m_currentEditLevelIndex = max(m_currentEditLevelIndex, -1);
		RecreatePreview();
	}GUI::SameLine();
	GUI::Text(m_currentEditLevelIndex);
	GUI::SameLine();
	if (GUI::ArrowButton("##plus", GUI::GuiDir_Right)) {
		m_currentEditLevelIndex++;
		if (m_currentEditLevelIndex >= m_currentData.list_data.GetSize()) {
			m_currentData.list_data.Add(LevelData());
		}
		RecreatePreview();
	}
	if (m_currentEditLevelIndex < 0) { return; }
	std::int32_t l_index=0,l_removeIndex=-1;
	bool l_isEdited=false;
	l_isEdited|=GUI::Checkbox("IsContinue", m_currentData.list_data[m_currentEditLevelIndex].isContinue);
	for (auto& data : m_currentData.list_data[m_currentEditLevelIndex].list_data) {
		if (GUI::TreeNode(std::to_string(l_index))) {
			GUI::SameLine();
			if (GUI::Button("Remove##" + std::to_string(l_index))) {
				l_removeIndex = l_index;
			}
			l_isEdited |=GUI::Edit(*data.transform);
			l_isEdited |= data.faceParam.GUI_SetMoveParam();
			l_isEdited |= data.faceParam.GUI_SetLife();
			l_isEdited |= data.faceParam.GUI_SetGravityParam();
			l_isEdited |= data.faceParam.GUI_SetRotationParam();

			GUI::BulletText("Type");
			l_isEdited |= PartTypeButton("Eye", PartType::Eye, data.faceParam.type); GUI::SameLine();
			l_isEdited |= PartTypeButton("Nose", PartType::Nose, data.faceParam.type); GUI::SameLine();
			l_isEdited |= PartTypeButton("Mouth", PartType::Mouth, data.faceParam.type); GUI::SameLine();
			l_isEdited |= PartTypeButton("Dummy", PartType::Dummy, data.faceParam.type); 
			GUI::TreePop();
		}
		l_index++;
	}

	if (l_removeIndex >= 0) {
		auto remItr = m_currentData.list_data[m_currentEditLevelIndex].list_data.begin() + l_removeIndex;
		m_currentData.list_data[m_currentEditLevelIndex].list_data.erase(remItr);
		l_isEdited |= true;
	}

	if (GUI::Button("Add")) {
		m_currentData.list_data[m_currentEditLevelIndex].list_data.Add({ FacePartParameter(), ObjectFactory::Create<Transform>(Vector3(0,0,5))});
		l_isEdited |= true;
	}

	if (GUI::Button("Clear")) {
		m_currentData.list_data[m_currentEditLevelIndex].list_data.Clear();
		l_isEdited |= true;
	}

	static std::int32_t srcIndex = 0;
	if (GUI::ArrowButton("##sub_cpy", GUI::GuiDir_Left)) {
		srcIndex--;
		srcIndex = max(srcIndex, -1);
	}GUI::SameLine();
	GUI::Text(srcIndex);
	GUI::SameLine();
	if (GUI::ArrowButton("##plus_cpy", GUI::GuiDir_Right)) {
		srcIndex++;
		srcIndex = min(srcIndex, m_currentData.list_data.GetSize()-1);
	}


	if (GUI::Button("Copy")) {
		m_currentData.list_data[m_currentEditLevelIndex].list_data.Clear();
		for (auto data : m_currentData.list_data[srcIndex].list_data) {
			data.transform = data.transform->Clone();
			m_currentData.list_data[m_currentEditLevelIndex].list_data.Add(data);
		}
		l_isEdited |= true;
	}

	if (l_isEdited) {
		Save();
		RecreatePreview();
	}
	if (GUI::TreeNode("Gene")) {
		GUI::BulletText("LevelSize:" + std::to_string(m_currentData.list_data.GetSize()));
		if (GUI::Button("RemoveLast") && m_currentData.list_data.GetSize()) {
			m_currentData.list_data.RemoveLast();
		}
		GUI::TreePop();
	}

}

void ButiEngine::LevelEditor::Start()
{
	ReleasePreview();
	SetIsRemove(true);
}
void ButiEngine::LevelEditor::Save()
{
	OutputCereal(m_currentData, "Scene/" + GetManager().lock()->GetScene().lock()->GetSceneInformation()->GetSceneName() + "/levelData.lvd");
}

void ButiEngine::LevelEditor::CreatePreview()
{
	if (m_currentEditLevelIndex < 0) { return; }
	for (auto& data : m_currentData.list_data[m_currentEditLevelIndex].list_data) {
		std::string objectName = "PartsPreview_";
		switch (data.faceParam.type)
		{
		case PartType::Eye:
			objectName += "eye";
			break;
		case PartType::Nose:
			objectName += "nose";
			break;
		case PartType::Mouth:
			objectName += "mouth";
			break;
		case PartType::Dummy:
			objectName += "dummy";
			break;
		default:
			break;
		}

		auto gameobj=GetManager().lock()->AddObjectFromCereal(objectName, data.transform);
		data.transform=gameobj.lock()->transform  ;
	}
}

void ButiEngine::LevelEditor::ReleasePreview()
{
	auto parts= GetManager().lock()->GetGameObjects(GameObjectTag("FacePartPreview"));
	for (auto part : parts) {
		GetManager().lock()->RemoveObject(part);
		part->SetIsRemove(true);
	}
}

void ButiEngine::LevelEditor::RecreatePreview()
{
	ReleasePreview();
	CreatePreview();
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::LevelEditor::Clone()
{
	return ObjectFactory::Create<LevelEditor>();
}
