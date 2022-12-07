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
			m_isLevelUp = false;

			ChangeLevelUpInterval(m_vec_levelUpIntervalFrames[m_gameLevel]);
		}

		//if (m_vlp_pauseTimer->Update())
		//{
		//	m_vlp_pauseTimer->Stop();
		//	m_vwp_pauseManager.lock()->SetIsPause(false);
		//	m_isLevelUp = false;
		//}
	}

	if (m_vwp_pauseManager.lock()->IsPause() || m_isLevelLock)
	{
		return;
	}

	if (m_vlp_levelUpIntervalTimer->Update())
	{
		m_vlp_levelUpIntervalTimer->Stop();
		LevelUp();
	}
}

void ButiEngine::GameLevelManager::OnSet()
{
	if (m_maxLevel == 0)
	{
		m_maxLevel = 1;
	}
}

void ButiEngine::GameLevelManager::OnRemove()
{
}

void ButiEngine::GameLevelManager::OnShowUI()
{
	GUI::Text("CurrentLevel:" + std::to_string(m_gameLevel));

	GUI::Checkbox("LevelLock", m_isLevelLock);

	static std::int32_t nextLevel;
	if (GUI::DragInt("NextLevel", &nextLevel, 1.0f, 1, m_maxLevel))
	{
		nextLevel = min(nextLevel, m_maxLevel);
		nextLevel = max(0, nextLevel);
	}
	if (GUI::Button("ChangeLevel"))
	{
		ChangeLevel(nextLevel);
	}

	if (GUI::Button("AddLevel"))
	{
		m_maxLevel++;
	}

	if ((m_maxLevel + 1) >= 2)
	{
		GUI::SameLine();

		if (GUI::Button("RemoveLevel"))
		{
			m_maxLevel--;
		}
	}

	ResizeLevelParameter();

	for (std::int32_t i = 1; i < m_maxLevel + 1; i++)
	{
		std::string label = "Level:" + std::to_string(i);
		if (i == m_maxLevel)
		{
			label = "Level:Max";
		}

		if (GUI::TreeNode(label))
		{
			GUI::BulletText(U8("レベルアップに必要なフレンドの数"));
			GUI::DragInt("##NecessaryFridndCount:" + std::to_string(i), m_vec_necessaryFriendCounts[i], 1.0f, 1, 100000);

			GUI::BulletText(U8("強制的にレベルアップするまでの時間(Frame)"));
			if (GUI::DragInt("##LevelUpIntervalFrame:" + std::to_string(i), m_vec_levelUpIntervalFrames[i], 1.0f, 1, 100000))
			{
				ChangeLevelUpInterval(m_vec_levelUpIntervalFrames[i]);
			}

			GUI::TreePop();
		}
	}

	m_vec_necessaryFriendCounts[m_maxLevel] = 100000;
}

void ButiEngine::GameLevelManager::Start()
{
	m_gameLevel = 0;

	m_vwp_pauseManager = GetManager().lock()->GetGameObject("PauseManager").lock()->GetGameComponent<PauseManager>();
	m_vwp_friendManager = GetManager().lock()->GetGameObject("FriendManager").lock()->GetGameComponent<FriendManager>();

	m_vlp_levelUpIntervalTimer = ObjectFactory::Create<RelativeTimer>(60);

	m_vlp_levelUpTimer = ObjectFactory::Create<RelativeTimer>(1);
	m_vlp_pauseTimer = ObjectFactory::Create<RelativeTimer>(120);

	m_isLevelLock = false;
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::GameLevelManager::Clone()
{
	auto clone = ObjectFactory::Create<GameLevelManager>();
	clone->m_maxLevel = m_maxLevel;
	clone->m_vec_necessaryFriendCounts = m_vec_necessaryFriendCounts;
	clone->m_vec_levelUpIntervalFrames = m_vec_levelUpIntervalFrames;
	return clone;
}

void ButiEngine::GameLevelManager::ChangeLevelUpInterval(const std::int32_t arg_levelUpIntervalFrame)
{
	if (!m_vlp_levelUpIntervalTimer)
	{
		return;
	}

	m_vlp_levelUpIntervalTimer->ChangeCountFrame(arg_levelUpIntervalFrame);
	m_vlp_levelUpIntervalTimer->Reset();
	m_vlp_levelUpIntervalTimer->Start();
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
	if (m_isLevelLock)
	{
		return;
	}

	if (m_gameLevel >= m_maxLevel)
	{
		return;
	}

	if (m_gameLevel == 0)
	{
		m_gameLevel = 2;
		m_vwp_friendManager.lock()->ResetCurrentLevelFriendCount();
		ChangeLevelUpInterval(m_vec_levelUpIntervalFrames[m_gameLevel]);
		return;
	}

	m_vlp_levelUpTimer->Reset();
	//m_vlp_pauseTimer->Reset();

	//m_vwp_pauseManager.lock()->SetIsPause(true);

	m_vlp_levelUpTimer->Start();
	//m_vlp_pauseTimer->Start();

	m_isLevelUp = true;
}

void ButiEngine::GameLevelManager::ChangeLevel(const std::int32_t arg_nextLevel)
{
	m_gameLevel = arg_nextLevel;

	m_vlp_levelUpTimer->Reset();
	//m_vlp_pauseTimer->Reset();

	//m_vwp_pauseManager.lock()->SetIsPause(true);

	m_vlp_levelUpTimer->Start();
	//m_vlp_pauseTimer->Start();

	m_isLevelUp = true;
}

void ButiEngine::GameLevelManager::ResizeLevelParameter()
{
	if (m_vec_necessaryFriendCounts.size() != (m_maxLevel + 1))
	{
		m_vec_necessaryFriendCounts.resize(m_maxLevel + 1);
	}

	if (m_vec_levelUpIntervalFrames.size() != (m_maxLevel + 1))
	{
		m_vec_levelUpIntervalFrames.resize(m_maxLevel + 1);
	}
}
