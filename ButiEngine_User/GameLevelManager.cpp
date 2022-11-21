#include "stdafx_u.h"
#include "GameLevelManager.h"
#include "PauseManager.h"
#include "FriendManager.h"

void ButiEngine::GameLevelManager::OnUpdate()
{
	if (m_isLevelUp)
	{
		if (m_vlp_levelUpTimer->Update())
		{
			m_vlp_levelUpTimer->Stop();
			m_gameLevel++;
			m_vwp_friendManager.lock()->ResetCurrentLevelFriendCount();
		}

		if (m_vlp_pauseTimer->Update())
		{
			m_vlp_pauseTimer->Stop();
			m_vwp_pauseManager.lock()->SetIsPause(false);
			m_isLevelUp = false;
		}
	}
}

void ButiEngine::GameLevelManager::OnSet()
{
	if (m_vec_necessaryFriendCounts.size() == 0)
	{
		m_vec_necessaryFriendCounts.push_back(1);
	}
}

void ButiEngine::GameLevelManager::OnRemove()
{
}

void ButiEngine::GameLevelManager::OnShowUI()
{
	if (GUI::Button("AddLevel"))
	{
		m_vec_necessaryFriendCounts.push_back(10);
	}

	if (m_vec_necessaryFriendCounts.size() >= 2)
	{
		GUI::SameLine();

		if (GUI::Button("RemoveLevel"))
		{
			m_vec_necessaryFriendCounts.erase(m_vec_necessaryFriendCounts.end() - 1);
		}
	}

	GUI::BulletText("TargetFriendCount");
	std::int32_t index = 0;
	auto end = m_vec_necessaryFriendCounts.end();
	for (auto itr = m_vec_necessaryFriendCounts.begin(); itr != end; ++itr)
	{
		GUI::Text("Level:" + std::to_string(index));
		GUI::DragInt("##TargetFridndCount:" + std::to_string(index), *itr, 1.0f, 1, 100);

		index++;
	}
	m_vec_necessaryFriendCounts[0] = 1;
}

void ButiEngine::GameLevelManager::Start()
{
	m_gameLevel = 0;

	m_vwp_pauseManager = GetManager().lock()->GetGameObject("PauseManager").lock()->GetGameComponent<PauseManager>();
	m_vwp_friendManager = GetManager().lock()->GetGameObject("FriendManager").lock()->GetGameComponent<FriendManager>();

	m_vlp_levelUpTimer = ObjectFactory::Create<RelativeTimer>(60);
	m_vlp_pauseTimer = ObjectFactory::Create<RelativeTimer>(120);
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::GameLevelManager::Clone()
{
	auto clone = ObjectFactory::Create<GameLevelManager>();
	clone->m_vec_necessaryFriendCounts = m_vec_necessaryFriendCounts;
	return clone;
}

void ButiEngine::GameLevelManager::CheckLevelUp(const std::int32_t arg_currentLevelFriendCount)
{
	if (arg_currentLevelFriendCount >= m_vec_necessaryFriendCounts[m_gameLevel])
	{
		LevelUp();
	}
}

void ButiEngine::GameLevelManager::LevelUp()
{
	if (m_gameLevel == 0)
	{
		m_gameLevel++;
		m_vwp_friendManager.lock()->ResetCurrentLevelFriendCount();
		return;
	}

	m_vlp_levelUpTimer->Reset();
	m_vlp_pauseTimer->Reset();

	m_vwp_pauseManager.lock()->SetIsPause(true);

	m_vlp_levelUpTimer->Start();
	m_vlp_pauseTimer->Start();

	m_isLevelUp = true;
}
