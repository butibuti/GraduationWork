#include "stdafx_u.h"
#include "FriendBodySpawner.h"
#include "PauseManager.h"
#include "StageManager.h"
#include "GameLevelManager.h"
#include "FriendBody.h"
#include"FacePartSpawner.h"
#include "BombFriend.h"

void ButiEngine::FriendBodySpawner::OnUpdate()
{
	if (!CanUpdate())
	{
		return;
	}


	if (m_isPlayPattern == false)
	{
		StartNextSpawnPattern();
	}
	else
	{
		UpdateSpawnPattern();
	}

	/*if (m_vlp_spawnTimer->Update())
	{
		SpawnBody();
		SetSpawnInterval();
	}*/
}

void ButiEngine::FriendBodySpawner::OnSet()
{
}

void ButiEngine::FriendBodySpawner::OnRemove()
{
}

void ButiEngine::FriendBodySpawner::OnShowUI()
{
	if (!m_vwp_gameLevelManager.lock())
	{
		m_vwp_gameLevelManager = GetManager().lock()->GetGameObject("GameLevelManager").lock()->GetGameComponent<GameLevelManager>();
	}

	ResizeLevelParameter();

	std::int32_t maxLevel = m_vwp_gameLevelManager.lock()->GetMaxLevel();
	for (std::int32_t i = 1; i < maxLevel + 1; i++)
	{

		if (GUI::TreeNode("Level:" + std::to_string(i)))
		{
			GUI::BulletText(U8("生成する間隔"));
			GUI::DragInt("##SpawnIntervalFrame" + std::to_string(i), m_vec_spawnIntervalFrames[i], 1.0f, 0, 100);

			GUI::TreePop();
		}
	}
}

void ButiEngine::FriendBodySpawner::Start()
{
	InputCereal(m_currentData, "Scene/" + GetManager().lock()->GetScene().lock()->GetSceneInformation()->GetSceneName() + "/levelData.lvd");

	m_vwp_stageManager = GetManager().lock()->GetGameObject("StageManager").lock()->GetGameComponent<StageManager>();
	m_vwp_pauseManager = GetManager().lock()->GetGameObject("PauseManager").lock()->GetGameComponent<PauseManager>();
	m_vwp_gameLevelManager = GetManager().lock()->GetGameObject("GameLevelManager").lock()->GetGameComponent<GameLevelManager>();
	m_vwp_facePartSpawner = GetManager().lock()->GetGameObject("FacePartSpawner").lock()->GetGameComponent<FacePartSpawner>();
	ResizeLevelParameter();

	m_vlp_spawnTimer = ObjectFactory::Create<RelativeTimer>(120);

	m_vlp_spawnPatternTimer = ObjectFactory::Create<RelativeTimer>(60);
	m_spawnedBodiesNumber = 0;
	m_existingBodiesNumber = 0;
	m_spawnPatternOrderCounter = 0;
	m_isPlayPattern = false;
	UpdateSpawnPattern();
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::FriendBodySpawner::Clone()
{
	auto clone = ObjectFactory::Create<FriendBodySpawner>();
	clone->m_vec_spawnIntervalFrames = m_vec_spawnIntervalFrames;
	return clone;
}

void ButiEngine::FriendBodySpawner::DecreaceBodiesNumber()
{
	m_existingBodiesNumber--;
}

void ButiEngine::FriendBodySpawner::SpawnBody()
{
	auto body = GetManager().lock()->AddObjectFromCereal("FriendBody");
	body.lock()->transform->SetLocalPosition(gameObject.lock()->transform->GetLocalPosition());
	Value_weak_ptr<FriendBody> friendBody = body.lock()->GetGameComponent<FriendBody>();
	friendBody.lock()->m_vwp_friendBodySpawner = gameObject.lock()->GetGameComponent<FriendBodySpawner>();
	friendBody.lock()->SetParameter(0, 0);
	m_spawnedBodiesNumber++;
	m_existingBodiesNumber++;
}

void ButiEngine::FriendBodySpawner::SpawnBody(Vector3 spawnPosition, float moveSpeed, float rotateSpeed, float initRotationY)
{
	auto body = GetManager().lock()->AddObjectFromCereal("FriendBody");
	body.lock()->transform->SetLocalPosition(spawnPosition);
	body.lock()->transform->SetLocalRotationY_Degrees(initRotationY);
	Value_weak_ptr<FriendBody> friendBody = body.lock()->GetGameComponent<FriendBody>();
	friendBody.lock()->m_vwp_friendBodySpawner = gameObject.lock()->GetGameComponent<FriendBodySpawner>();
	friendBody.lock()->SetParameter(moveSpeed, rotateSpeed);
	m_spawnedBodiesNumber++;
	m_existingBodiesNumber++;
}

void ButiEngine::FriendBodySpawner::SpawnBombBody(Vector3 spawnPosition, float moveSpeed, float rotateSpeed, float initRotationY, std::int32_t arg_bombCount)
{
	auto body = GetManager().lock()->AddObjectFromCereal("BombFriend");
	body.lock()->transform->SetLocalPosition(spawnPosition);
	body.lock()->transform->SetLocalRotationY_Degrees(initRotationY);
	Value_weak_ptr<FriendBody> friendBody = body.lock()->GetGameComponent<FriendBody>();
	friendBody.lock()->m_vwp_friendBodySpawner = gameObject.lock()->GetGameComponent<FriendBodySpawner>();
	friendBody.lock()->SetParameter(moveSpeed, rotateSpeed);
	auto bombFriend = body.lock()->GetGameComponent<BombFriend>();
	bombFriend->SetFrameToExplode(arg_bombCount);
	m_spawnedBodiesNumber++;
	m_existingBodiesNumber++;
}

void ButiEngine::FriendBodySpawner::StartSpawnPattern()
{
	m_vlp_spawnPatternTimer->ChangeCountFrame(100000);
	m_vlp_spawnPatternTimer->Reset();
	m_vlp_spawnPatternTimer->Start();
	m_spawnedBodiesNumber = 0;
	m_existingBodiesNumber = 0;

	m_isPlayPattern = true;

	if (m_spawnPatternOrderCounter == 13)
	{
		m_vlp_spawnTimer->ChangeCountFrame(180);
		m_vlp_spawnTimer->Reset();
		m_vlp_spawnTimer->Start();
	}
	if (m_spawnPatternOrderCounter == 11)
	{
		m_vlp_spawnTimer->ChangeCountFrame(300);
		m_vlp_spawnTimer->Reset();
		m_vlp_spawnTimer->Start();
	}

}

void ButiEngine::FriendBodySpawner::UpdateSpawnPattern()
{
	if (m_existingBodiesNumber > 0) 
	{ 
	}
	else if(m_spawnedBodiesNumber==0) {
		if (m_currentData.list_data.GetSize() <= m_spawnPatternOrderCounter) {

			if (m_vlp_spawnTimer->Update())
			{
				m_vlp_spawnTimer->ChangeCountFrame(ButiRandom::GetInt(6, 10));
				SpawnBody(Vector3(5.5f, -4, 0), ButiRandom::GetRandom(-0.1f, -0.15f, 10), ButiRandom::GetRandom(20.0f, 30.0f, 10), ButiRandom::GetRandom(0.0f, 360.0f, 10));
			}
		}
		else {
			for (auto& bodyData : m_currentData.list_data[m_spawnPatternOrderCounter].list_bodyData) {
				if (bodyData.isBomb) {
					SpawnBombBody(Vector3(bodyData.initPosition.x, -4, bodyData.initPosition.z), bodyData.translateSpeed, bodyData.rotationSpeed, bodyData.initRotate,bodyData.bombCount);
				}
				else {
					SpawnBody(Vector3(bodyData.initPosition.x, -4, bodyData.initPosition.z), bodyData.translateSpeed, bodyData.rotationSpeed, bodyData.initRotate);
				}
				
			}
		}
	}

	if (m_existingBodiesNumber <= 0) m_isPlayPattern = false;

}

void ButiEngine::FriendBodySpawner::StartNextSpawnPattern()
{

	if (m_vwp_facePartSpawner.lock()) {
		m_vwp_facePartSpawner.lock()->LevelIncrement();
		m_spawnPatternOrderCounter = m_vwp_facePartSpawner.lock()->GetCurrentLevel();
	}
	StartSpawnPattern();

}

void ButiEngine::FriendBodySpawner::SetSpawnInterval()
{
	if (!m_vlp_spawnTimer)
	{
		return;
	}
	std::int32_t gameLevel = m_vwp_gameLevelManager.lock()->GetGameLevel();
	std::int32_t spawnIntervalFrame = ButiRandom::GetInt(m_vec_spawnIntervalFrames[gameLevel], m_vec_spawnIntervalFrames[gameLevel]);
	spawnIntervalFrame = max(spawnIntervalFrame, 0);

	m_vlp_spawnTimer->ChangeCountFrame(spawnIntervalFrame);
	m_vlp_spawnTimer->Reset();
}

bool ButiEngine::FriendBodySpawner::CanUpdate()
{
	if (!m_vwp_stageManager.lock()->IsGameStart())
	{
		return false;
	}

	if (m_vwp_pauseManager.lock()->IsPause())
	{
		return false;
	}

	return true;
}

void ButiEngine::FriendBodySpawner::ResizeLevelParameter()
{
	std::int32_t maxLevel = m_vwp_gameLevelManager.lock()->GetMaxLevel();
	if (m_vec_spawnIntervalFrames.size() != (maxLevel + 1))
	{
		m_vec_spawnIntervalFrames.resize(maxLevel + 1);
	}
}
