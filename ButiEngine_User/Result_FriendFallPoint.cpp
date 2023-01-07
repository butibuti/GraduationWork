#include "stdafx_u.h"
#include "Result_FriendFallPoint.h"
#include "Result_FriendSpawner.h"
#include "FriendManager.h"
#include "ResultManager.h"

void ButiEngine::Result_FriendFallPoint::OnUpdate()
{
	if (!m_isStart)
	{
		return;
	}

	if (m_vlp_changeSpeedTimer->IsOn())
	{
		float progress = m_vlp_changeSpeedTimer->GetPercent();
		m_moveSpeed = MathHelper::Lerp(m_startSpeed, m_vlp_passedCheckPoint->targetSpeed, Easing::GetEase(progress, m_vlp_passedCheckPoint->easeType));

		if (m_vlp_changeSpeedTimer->Update())
		{
			m_vlp_changeSpeedTimer->Stop();
			m_moveSpeed = m_vlp_passedCheckPoint->targetSpeed;
		}
	}

	if (m_vlp_passedCheckPoint && m_vlp_passedCheckPoint->hasLimit)
	{
		if (m_vlp_limitTimer->Update())
		{
			m_passedPointCount++;
			SetNextCheckPoint();
		}
		return;
	}

	float pos = gameObject.lock()->transform->TranslateX(-m_moveSpeed).x;
	if (pos <= m_vlp_nextCheckPoint->pos)
	{
		m_passedPointCount++;
		SetNextCheckPoint();
		return;
	}
}

void ButiEngine::Result_FriendFallPoint::OnSet()
{
	if (m_vec_checkPoints.size() == 0)
	{
		m_vec_checkPoints.push_back(ObjectFactory::Create<CheckPoint>());
	}
}

void ButiEngine::Result_FriendFallPoint::OnRemove()
{
}

void ButiEngine::Result_FriendFallPoint::OnShowUI()
{
	if (GUI::Button("Start"))
	{
		auto resultManager = GetManager().lock()->GetGameObject("ResultManager").lock()->GetGameComponent<ResultManager>();
		auto friendDatas = FriendManager::GetFriendDatas();
		if (friendDatas.size() < (resultManager->GetSuccessBorder() - 3))
		{
			auto friendSpawner = GetManager().lock()->GetGameObject("Result_FriendSpawner").lock()->GetGameComponent<Result_FriendSpawner>();
			std::int32_t startZoomInFriendNum = max(0, friendDatas.size() - 2);
			float diff = friendSpawner->GetFriendSpawnPos(startZoomInFriendNum).x - m_vec_checkPoints[0]->pos;

			m_vec_checkPoints[0]->changeSpeedFrame = 10;

			auto end = m_vec_checkPoints.end();
			std::int32_t index = 0;
			float diffCheckPoint0_1 = m_vec_checkPoints[0]->pos - m_vec_checkPoints[1]->pos;
			for (auto itr = m_vec_checkPoints.begin(); itr != end - 1; ++itr)
			{
				(*itr)->pos += diff;
				if (index >= 1)
				{
					(*itr)->pos += diffCheckPoint0_1 * 0.75f;
				}
				if ((*itr)->hasLimit)
				{
					(*itr)->limitFrame += 30;
				}
				index++;
			}
		}

		m_isStart = false;

		m_moveSpeed = m_initSpeed;

		m_vlp_changeSpeedTimer = ObjectFactory::Create<RelativeTimer>(60);
		m_vlp_limitTimer = ObjectFactory::Create<RelativeTimer>(60);

		m_passedPointCount = 0;

		SetNextCheckPoint();
	}
	GUI::Text("MoveSpeed:" + std::to_string(m_moveSpeed));

	if (GUI::Button("StartMove"))
	{
		StartMove();
	}

	GUI::BulletText("InitSpeed");
	GUI::DragFloat("##InitSpeed", m_initSpeed, 0.1f, 0.0f, 100.0f);

	if (GUI::TreeNode("CheckPoint"))
	{
		if (GUI::Button("AddCheckPoint"))
		{
			m_vec_checkPoints.push_back(ObjectFactory::Create<CheckPoint>());
		}

		GUI::BulletText("StartZoomPointNum");
		GUI::DragInt("##StartZoomPointNum", m_startZoomPointNum, 1.0f, 0, m_vec_checkPoints.size() - 1);

		std::int32_t index = 0;
		auto end = m_vec_checkPoints.end();
		for (auto itr = m_vec_checkPoints.begin(); itr != end; ++itr)
		{
			if (GUI::TreeNode("CheckPoint" + std::to_string(index)))
			{
				if (m_vec_checkPoints.size() >= 2)
				{
					if (GUI::Button("Insert"))
					{
						m_vec_checkPoints.insert(itr, ObjectFactory::Create<CheckPoint>());
						GUI::TreePop();
						break;
					}
					GUI::SameLine();
					if (GUI::Button("Remove"))
					{
						m_vec_checkPoints.erase(itr);
						GUI::TreePop();
						break;
					}
				}

				GUI::Checkbox("HasLimit", (*itr)->hasLimit);
				if ((*itr)->hasLimit)
				{
					GUI::BulletText("LimitFrame");
					GUI::DragInt("##LimitFrame", (*itr)->limitFrame, 1.0f, 0, 300);
				}

				GUI::BulletText("Pos");
				GUI::DragFloat("##Pos" + std::to_string(index), (*itr)->pos, 0.1f, -100.0f, 100.0f);

				GUI::BulletText("TargetSpeed");
				GUI::DragFloat("##TargetSpeed" + std::to_string(index), (*itr)->targetSpeed, 0.1f, 0.0f, 100.0f);

				GUI::BulletText("ChangeSpeedFrame");
				GUI::DragInt("##ChangeSpeedFrame" + std::to_string(index), (*itr)->changeSpeedFrame, 1.0f, 1, 3000);

				GUI::BulletText("EaseType");
				GUI::Text(Easing::EasingTypeNames[(std::int32_t)(*itr)->easeType]);

				if (GUI::ArrowButton("##EaseType_UP" + std::to_string(index), GUI::GuiDir_Up)) {
					(*itr)->easeType = ((Easing::EasingType)(((std::int32_t)(*itr)->easeType) + 1));
					if ((std::int32_t)(*itr)->easeType > (std::int32_t)Easing::EasingType::CubeRotate90) {
						(*itr)->easeType = Easing::EasingType::CubeRotate90;
					}
				}
				GUI::SameLine();
				if (GUI::ArrowButton("##EaseType_Down" + std::to_string(index), GUI::GuiDir_Down)) {

					(*itr)->easeType = ((Easing::EasingType)(((std::int32_t)(*itr)->easeType) - 1));
					if ((std::int32_t)(*itr)->easeType < 0) {
						(*itr)->easeType = Easing::EasingType::EaseIn;
					}
				}

				GUI::TreePop();
			}
			index++;
		}
		GUI::TreePop();
	}
}

void ButiEngine::Result_FriendFallPoint::Start()
{
	//auto resultManager = GetManager().lock()->GetGameObject("ResultManager").lock()->GetGameComponent<ResultManager>();
	//auto friendDatas = FriendManager::GetFriendDatas();
	//if (friendDatas.size() < (resultManager->GetSuccessBorder() - 3))
	//{
	//	m_vec_checkPoints.erase(m_vec_checkPoints.begin());
	//	auto friendSpawner = GetManager().lock()->GetGameObject("Result_FriendSpawner").lock()->GetGameComponent<Result_FriendSpawner>();
	//	float diff = friendSpawner->GetFriendSpawnPos(friendDatas.size() - 1).x - m_vec_checkPoints[0]->pos;

	//	auto end = m_vec_checkPoints.end();
	//	for (auto itr = m_vec_checkPoints.begin(); itr != end - 1; ++itr)
	//	{
	//		(*itr)->pos += diff;
	//	}
	//}

	//m_isStart = false;

	//m_moveSpeed = m_initSpeed;

	//m_vlp_changeSpeedTimer = ObjectFactory::Create<RelativeTimer>(60);
	//m_vlp_limitTimer = ObjectFactory::Create<RelativeTimer>(60);

	//m_passedPointCount = 0;

	//SetNextCheckPoint();
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::Result_FriendFallPoint::Clone()
{
	auto clone = ObjectFactory::Create<Result_FriendFallPoint>();
	clone->m_initSpeed = m_initSpeed;
	clone->m_vec_checkPoints = m_vec_checkPoints;
	clone->m_startZoomPointNum = m_startZoomPointNum;
	return clone;
}

void ButiEngine::Result_FriendFallPoint::StartMove()
{
	m_isStart = true;
}

void ButiEngine::Result_FriendFallPoint::StopMove()
{
	m_isStart = false;
}

void ButiEngine::Result_FriendFallPoint::SetNextCheckPoint()
{
	if (m_passedPointCount == m_vec_checkPoints.size())
	{
		m_vlp_changeSpeedTimer->Stop();
		SetIsRemove(true);
		return;
	}

	m_vlp_passedCheckPoint = m_vlp_nextCheckPoint;
	m_vlp_nextCheckPoint = m_vec_checkPoints[m_passedPointCount];

	if (!m_vlp_passedCheckPoint)
	{
		return;
	}

	if (m_vlp_passedCheckPoint->hasLimit)
	{
		m_vlp_limitTimer->ChangeCountFrame(m_vlp_passedCheckPoint->limitFrame);
		m_vlp_limitTimer->Reset();
		if (!m_vlp_limitTimer->IsOn())
		{
			m_vlp_limitTimer->Start();
		}
	}

	if (m_vlp_passedCheckPoint->changeSpeedFrame == 0)
	{
		m_moveSpeed = m_vlp_passedCheckPoint->targetSpeed;
		return;
	}

	m_startSpeed = m_moveSpeed;
	m_vlp_changeSpeedTimer->ChangeCountFrame(m_vlp_passedCheckPoint->changeSpeedFrame);
	m_vlp_changeSpeedTimer->Reset();

	if (!m_vlp_changeSpeedTimer->IsOn())
	{
		m_vlp_changeSpeedTimer->Start();
	}
}
