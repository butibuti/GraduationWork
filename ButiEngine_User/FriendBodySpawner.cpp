#include "stdafx_u.h"
#include "FriendBodySpawner.h"
#include "PauseManager.h"
#include "StageManager.h"
#include "GameLevelManager.h"

void ButiEngine::FriendBodySpawner::OnUpdate()
{
	if (!CanUpdate())
	{
		return;
	}

	if (m_vlp_spawnTimer->Update())
	{
		SpawnBody();
		SetSpawnInterval();
	}
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
			GUI::BulletText(U8("¶¬‚·‚éŠÔŠu"));
			GUI::DragInt("##SpawnIntervalFrame" + std::to_string(i), m_vec_spawnIntervalFrames[i], 1.0f, 0, 100);

			GUI::TreePop();
		}
	}
}

void ButiEngine::FriendBodySpawner::Start()
{
	m_vwp_stageManager = GetManager().lock()->GetGameObject("StageManager").lock()->GetGameComponent<StageManager>();
	m_vwp_pauseManager = GetManager().lock()->GetGameObject("PauseManager").lock()->GetGameComponent<PauseManager>();
	m_vwp_gameLevelManager = GetManager().lock()->GetGameObject("GameLevelManager").lock()->GetGameComponent<GameLevelManager>();

	ResizeLevelParameter();

	m_vlp_spawnTimer = ObjectFactory::Create<RelativeTimer>(60);
	SetSpawnInterval();
	m_vlp_spawnTimer->Start();
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::FriendBodySpawner::Clone()
{
	auto clone = ObjectFactory::Create<FriendBodySpawner>();
	clone->m_vec_spawnIntervalFrames = m_vec_spawnIntervalFrames;
	return clone;
}

void ButiEngine::FriendBodySpawner::SpawnBody()
{
	auto body = GetManager().lock()->AddObjectFromCereal("FriendBody");
	body.lock()->transform->SetLocalPosition(gameObject.lock()->transform->GetLocalPosition());
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
