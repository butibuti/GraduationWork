#include "stdafx_u.h"
#include "Result_FriendFallPoint.h"

void ButiEngine::Result_FriendFallPoint::OnUpdate()
{
	if (!m_isStart)
	{
		return;
	}

	float progress = m_vlp_animTimer->GetPercent();
	Vector3 pos = MathHelper::LerpPosition(m_startPos, m_vec_checkPoints[0]->pos, Easing::GetEase(progress, m_vec_checkPoints[0]->easeType));
	gameObject.lock()->transform->SetLocalPosition(pos);

	if (m_vlp_animTimer->Update())
	{
		gameObject.lock()->transform->SetLocalPosition(m_vec_checkPoints[0]->pos);

		m_vec_checkPoints.erase(m_vec_checkPoints.begin());
		SetNewCheckPoint();
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
	if (GUI::Button("StartMove"))
	{
		StartMove();
	}

	if (GUI::Button("AddCheckPoint"))
	{
		m_vec_checkPoints.push_back(ObjectFactory::Create<CheckPoint>());
	}

	if (m_vec_checkPoints.size() >= 2)
	{
		GUI::SameLine();

		if (GUI::Button("RemoveCheckPoint"))
		{
			m_vec_checkPoints.pop_back();
		}
	}

	std::int32_t index = 0;
	auto end = m_vec_checkPoints.end();
	for (auto itr = m_vec_checkPoints.begin(); itr != end; ++itr)
	{
		if (GUI::TreeNode("CheckPoint" + std::to_string(index)))
		{
			GUI::BulletText("Pos");
			GUI::DragFloat3("##Pos" + std::to_string(index), &(*itr)->pos.x, 0.1f, -100.0f, 100.0f);

			GUI::BulletText("AnimFrame");
			GUI::DragInt("##AnimFrame" + std::to_string(index), (*itr)->animFrame, 1.0f, 1, 3000);


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
}

void ButiEngine::Result_FriendFallPoint::Start()
{
	m_isStart = false;

	m_vlp_animTimer = ObjectFactory::Create<RelativeTimer>(60);

	SetNewCheckPoint();
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::Result_FriendFallPoint::Clone()
{
	auto clone = ObjectFactory::Create<Result_FriendFallPoint>();
	clone->m_vec_checkPoints = m_vec_checkPoints;
	return clone;
}

void ButiEngine::Result_FriendFallPoint::StartMove()
{
	m_isStart = true;
}

void ButiEngine::Result_FriendFallPoint::SetNewCheckPoint()
{
	if (m_vec_checkPoints.size() == 0)
	{
		m_vlp_animTimer->Stop();
		SetIsRemove(true);
		return;
	}

	m_startPos = gameObject.lock()->transform->GetLocalPosition();
	m_vlp_animTimer->ChangeCountFrame(m_vec_checkPoints[0]->animFrame);
	m_vlp_animTimer->Reset();

	if (!m_vlp_animTimer->IsOn())
	{
		m_vlp_animTimer->Start();
	}
}
