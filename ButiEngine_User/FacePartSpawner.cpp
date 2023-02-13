#include "stdafx_u.h"
#include "FacePartSpawner.h"
#include"FriendFacePart.h"
#include"PartRespawnPoint.h"
void ButiEngine::FacePartSpawner::OnUpdate()
{
	if (!m_remainPart&&(m_successPart!= m_currentData.list_data[m_currentEditLevelIndex].list_data.GetSize())
		&&( m_currentEditLevelIndex >= 0) && 
		m_currentData.list_data[m_currentEditLevelIndex].isContinue) {
		LevelIncrement();
	}
}

void ButiEngine::FacePartSpawner::OnSet()
{
}

void ButiEngine::FacePartSpawner::OnRemove()
{
}

void ButiEngine::FacePartSpawner::OnShowUI()
{

	GUI::BulletText("currentLevel:"+std::to_string(m_currentEditLevelIndex));

	if (GUI::TreeNode("Random")) {

		GUI::BulletText("Random_min");
		if (GUI::ArrowButton("##sub_min", GUI::GuiDir_Left)) {
			m_randomSelectLevelMin--;
			m_randomSelectLevelMin = max(m_randomSelectLevelMin, 0);
		}GUI::SameLine();
		GUI::Text(m_randomSelectLevelMin);
		GUI::SameLine();
		if (GUI::ArrowButton("##plus_min", GUI::GuiDir_Right)) {

			m_randomSelectLevelMin++;
		}

		GUI::BulletText("Random_max");

		if (GUI::ArrowButton("##sub_max", GUI::GuiDir_Left)) {
			m_randomSelectLevelMax--;
			m_randomSelectLevelMax = max(m_randomSelectLevelMax, 0);
		}GUI::SameLine();
		GUI::Text(m_randomSelectLevelMax);
		GUI::SameLine();
		if (GUI::ArrowButton("##plus_max", GUI::GuiDir_Right)) {

			m_randomSelectLevelMax++;
		}
		GUI::TreePop();
	}
}

void ButiEngine::FacePartSpawner::Start()
{
	InputCereal(m_currentData, "Scene/" + GetManager().lock()->GetScene().lock()->GetSceneInformation()->GetSceneName() + "/levelData.lvd");

	m_randomSelectLevelMin = min(m_randomSelectLevelMin, m_currentData.list_data.GetSize()-1);
	m_randomSelectLevelMax = min(m_randomSelectLevelMax, m_currentData.list_data.GetSize()-1);
	List<std::int32_t>l_list_emptyIndex;
	for (std::int32_t index = 0, size = m_currentData.list_data.GetSize(); index < size; index++) {
		if (!m_currentData.list_data[index].list_data.GetSize()) {
			l_list_emptyIndex.Add(index);
		}
	}
	std::int32_t remCount = 0;
	for (auto index : l_list_emptyIndex) {
		auto remItr = m_currentData.list_data.begin() + (index-remCount);
		m_currentData.list_data.erase(remItr);
		remCount++;
	}

	LevelIncrement();
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::FacePartSpawner::Clone()
{
	return ObjectFactory::Create<FacePartSpawner>();
}


void ButiEngine::FacePartSpawner::CreatePartArrangement()
{
	if (m_currentEditLevelIndex < 0) { return; }
	m_remainPart = 0; m_successPart = 0;
	for (auto& data : m_currentData.list_data[m_currentEditLevelIndex].list_data) {

		auto respawnPoint = GetManager().lock()->AddObjectFromCereal("RespawnPoint", data.transform);
		auto respawnPointComponent = respawnPoint.lock()->GetGameComponent<PartRespawnPoint>();
		respawnPointComponent->SetSpawner(GetThis<FacePartSpawner>());
		respawnPointComponent->SetParam(data.faceParam);
		respawnPointComponent->SetIsContinue(m_currentData.list_data[m_currentEditLevelIndex].isContinue);
		m_remainPart++;
	}
	
}

void ButiEngine::FacePartSpawner::Clear()
{
	auto spawnpoints = GetManager().lock()->GetGameObjects(GameObjectTag("RespawnPoint"));
	for (auto spawnPoint: spawnpoints) {
		spawnPoint->GetGameComponent<PartRespawnPoint>()->Clear();
		spawnPoint->SetIsRemove(true);
	}
}

void ButiEngine::FacePartSpawner::LevelIncrement()
{

	SetLevel(m_isRandomLevelSelect? ButiRandom::GetInt(m_randomSelectLevelMin, m_randomSelectLevelMax): m_currentEditLevelIndex + 1);
}

void ButiEngine::FacePartSpawner::SetLevel(const std::int32_t arg_level)
{
	Clear();
	m_currentEditLevelIndex =max(arg_level,0);
	if (m_currentEditLevelIndex >= m_currentData.list_data.GetSize()) {
		m_isRandomLevelSelect = true;
		m_currentEditLevelIndex = ButiRandom::GetInt(m_randomSelectLevelMin, m_randomSelectLevelMax);
	}
	CreatePartArrangement();
}
