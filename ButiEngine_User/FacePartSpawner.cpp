#include "stdafx_u.h"
#include "FacePartSpawner.h"
#include"FriendFacePart.h"
#include"PartRespawnPoint.h"
void ButiEngine::FacePartSpawner::OnUpdate()
{
	if (!m_remainPart && m_currentEditLevelIndex>=0&& m_currentData.list_data[m_currentEditLevelIndex].isContinue) {
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
}

void ButiEngine::FacePartSpawner::Start()
{
	InputCereal(m_currentData, "Scene/" + GetManager().lock()->GetScene().lock()->GetSceneInformation()->GetSceneName() + "/levelData.lvd");
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
		spawnPoint->SetIsRemove(true);
	}
}

void ButiEngine::FacePartSpawner::LevelIncrement()
{
	Clear();
	m_currentEditLevelIndex++;
	if (m_currentEditLevelIndex >= m_currentData.list_data.GetSize()) { m_currentEditLevelIndex = 0; }
	CreatePartArrangement();
}
