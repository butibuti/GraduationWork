#include "stdafx_u.h"
#include "FriendAligner.h"

void ButiEngine::FriendAligner::OnUpdate()
{
}

void ButiEngine::FriendAligner::OnSet()
{
}

void ButiEngine::FriendAligner::OnRemove()
{
}

void ButiEngine::FriendAligner::OnShowUI()
{
	GUI::BulletText("MaxFriendCount");
	GUI::DragInt("##MaxFriendCount", m_maxFriendCount, 1.0f, 1, 100);
}

void ButiEngine::FriendAligner::Start()
{
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::FriendAligner::Clone()
{
	return ObjectFactory::Create<FriendAligner>();
}

ButiEngine::Vector3 ButiEngine::FriendAligner::GetCalcFriendPos(std::int32_t arg_friendNum)
{
	Vector3 pos = gameObject.lock()->transform->GetLocalPosition();
	Vector3 scale = gameObject.lock()->transform->GetLocalScale();

	float rightPos = (pos - scale * 0.5f).x;
	float leftPos = (pos + scale * 0.5f).x;

	float length = abs(rightPos - leftPos);
	float space = length / m_maxFriendCount;

	Vector3 friendPos = pos;
	friendPos.z -= space * (arg_friendNum / m_maxFriendCount);
	arg_friendNum %= m_maxFriendCount;
	friendPos.x = leftPos;
	friendPos.x -= space * arg_friendNum;

	return friendPos;
}
