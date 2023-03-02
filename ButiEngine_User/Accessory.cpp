#include "stdafx_u.h"
#include "Accessory.h"
#include "SeparateDrawObject.h"
#include "FriendHead.h"
#include "FriendHead_Center.h"
#include "Header/GameObjects/DefaultGameComponent/TriggerComponent.h"
#include "Header/GameObjects/DefaultGameComponent/ScaleAnimationComponent.h"

std::int32_t ButiEngine::Accessory::g_accessoryCount;

void ButiEngine::Accessory::OnUpdate()
{
	if (m_isAppear)
	{
		OnAppear();
	}

	m_vwp_drawObject.lock()->transform->RollLocalRotationY_Degrees(1.0f);
}

void ButiEngine::Accessory::OnSet()
{
	gameObject.lock()->AddCollisionEnterReaction(
		[this](ButiBullet::ContactData& arg_other)
		{
			if (arg_other.vwp_gameObject.lock())
			{
				//ƒ^ƒO”»’è
				if (arg_other.vwp_gameObject.lock()->HasGameObjectTag("HeadCenter"))
				{
					OnCollisionHeadCenter(arg_other.vwp_gameObject);
				}
			}
		}
	);
}

void ButiEngine::Accessory::OnRemove()
{
}

void ButiEngine::Accessory::OnShowUI()
{
}

void ButiEngine::Accessory::Start()
{
	m_vwp_drawObject = gameObject.lock()->GetGameComponent<SeparateDrawObject>()->GetDrawObject();

	m_isAppear = false;
	Appear();
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::Accessory::Clone()
{
	auto clone = ObjectFactory::Create<Accessory>();
	return clone;
}

void ButiEngine::Accessory::Dead()
{
	if (m_vwp_drawObject.lock())
	{
		m_vwp_drawObject.lock()->SetIsRemove(true);
	}
}

void ButiEngine::Accessory::OnCollisionHeadCenter(Value_weak_ptr<GameObject> arg_vwp_partHitArea)
{
	gameObject.lock()->GetGameComponent<SeparateDrawObject>()->ReturnDrawObject();
	gameObject.lock()->GetGameComponent<MeshDrawComponent>()->GetTransform()->SetLocalPosition(Vector3Const::Zero);

	auto head = arg_vwp_partHitArea.lock()->GetGameComponent<FriendHead_Center>()->GetParent();
	gameObject.lock()->transform->SetLocalPosition(Vector3Const::Zero);
	head.lock()->GetGameComponent<FriendHead>()->SetHelmet(gameObject);

	Accessory::RemoveAccessoryCount();
	gameObject.lock()->GetGameComponent<TriggerComponent>()->SetIsRemove(true);
	SetIsRemove(true);
}

void ButiEngine::Accessory::Appear()
{
	m_isAppear = true;
	gameObject.lock()->GetGameComponent<TriggerComponent>()->UnRegist();
	
	auto anim = gameObject.lock()->AddGameComponent<ScaleAnimation>();
	anim->SetInitScale(gameObject.lock()->transform->GetLocalScale());
	anim->SetTargetScale(1.0f);
	anim->SetSpeed(1.0f / 60);
	anim->SetEaseType(Easing::EasingType::EaseOutQuad);
}

void ButiEngine::Accessory::OnAppear()
{
	auto anim = gameObject.lock()->GetGameComponent<ScaleAnimation>();
	if (!anim)
	{
		m_isAppear = false;
		gameObject.lock()->GetGameComponent<TriggerComponent>()->Regist();
	}
}
