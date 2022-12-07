#include "stdafx_u.h"
#include "FriendHead_Center.h"
#include "Header/GameObjects/DefaultGameComponent/RigidBodyComponent.h"
#include "Header/GameObjects/DefaultGameComponent/TriggerComponent.h"
#include "FriendBody_Neck.h"

void ButiEngine::FriendHead_Center::OnUpdate()
{
}

void ButiEngine::FriendHead_Center::OnSet()
{
	gameObject.lock()->AddCollisionEnterReaction(
		[this](ButiBullet::ContactData& arg_other)
		{
			if (arg_other.vwp_gameObject.lock())
			{
				//タグ判定
				if (arg_other.vwp_gameObject.lock()->HasGameObjectTag("FriendBody_Neck"))
				{
					auto neck = arg_other.vwp_gameObject.lock()->GetGameComponent<FriendBody_Neck>();
					m_vwp_collisionFriendBody = neck->GetParent();
				}
			}
		}
	);

	gameObject.lock()->AddCollisionLeaveReaction(
		[this](ButiBullet::ContactData& arg_other)
		{
			if (arg_other.vwp_gameObject.lock())
			{
				//タグ判定
				if (arg_other.vwp_gameObject.lock()->HasGameObjectTag("FriendBody_Neck"))
				{
					m_vwp_collisionFriendBody = Value_weak_ptr<GameObject>();
				}
			}
		}
	);
}

void ButiEngine::FriendHead_Center::OnRemove()
{
}

void ButiEngine::FriendHead_Center::OnShowUI()
{
}

void ButiEngine::FriendHead_Center::Start()
{
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::FriendHead_Center::Clone()
{
	return ObjectFactory::Create<FriendHead_Center>();
}
