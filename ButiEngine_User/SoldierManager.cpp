#include "stdafx_u.h"
#include "SoldierManager.h"

void ButiEngine::SoldierManager::OnUpdate()
{
}

void ButiEngine::SoldierManager::OnSet()
{
}

void ButiEngine::SoldierManager::OnRemove()
{
}

void ButiEngine::SoldierManager::OnShowUI()
{
	GUI::BulletText(U8("ÉzÅ[ÉÄÇ…Ç¢ÇÈï∫émÇÃêî :" + std::to_string(m_vec_vwp_homeSoldiers.size())));
}

void ButiEngine::SoldierManager::Start()
{
	m_vec_vwp_homeSoldiers.clear();
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::SoldierManager::Clone()
{
	return ObjectFactory::Create<SoldierManager>();
}

ButiEngine::Value_weak_ptr<ButiEngine::GameObject> ButiEngine::SoldierManager::GetHomeSoldierRandom()
{
	std::int32_t homeSoldierCount = GetHomeSoldierCount();
	if (homeSoldierCount == 0)
	{
		return Value_weak_ptr<GameObject>();
	}

	std::int32_t index = ButiRandom::GetInt(0, homeSoldierCount - 1);
	return m_vec_vwp_homeSoldiers[index];
}

void ButiEngine::SoldierManager::AddHomeSoldier(Value_weak_ptr<GameObject> arg_soldier)
{
	auto find = std::find(m_vec_vwp_homeSoldiers.begin(), m_vec_vwp_homeSoldiers.end(), arg_soldier);
	if (find != m_vec_vwp_homeSoldiers.end())
	{
		return;
	}

	m_vec_vwp_homeSoldiers.push_back(arg_soldier);
}

void ButiEngine::SoldierManager::RemoveHomeSoldier(Value_weak_ptr<GameObject> arg_soldier)
{
	auto find = std::find(m_vec_vwp_homeSoldiers.begin(), m_vec_vwp_homeSoldiers.end(), arg_soldier);
	if (find == m_vec_vwp_homeSoldiers.end())
	{
		return;
	}

	m_vec_vwp_homeSoldiers.erase(find);
}
