#include "stdafx_u.h"
#include "Result_FallFriend.h"
#include "Result_FriendFallPoint.h"
#include "ResultManager.h"
#include "Result_BackHuman.h"
#include "Header/GameObjects/DefaultGameComponent/ModelDrawComponent.h"
#include "CompleteFriend.h"

float ButiEngine::Result_FallFriend::g_groundedSoundVolume = 0.1f;

void ButiEngine::Result_FallFriend::OnUpdate()
{
	CheckFall();
	if (m_isFall)
	{
		Fall();
	}
}

void ButiEngine::Result_FallFriend::OnSet()
{
}

void ButiEngine::Result_FallFriend::OnRemove()
{
}

void ButiEngine::Result_FallFriend::OnShowUI()
{
}

void ButiEngine::Result_FallFriend::Start()
{
	m_vwp_fallPoint = GetManager().lock()->GetGameObject("FallPoint");

	m_isFallStarted = false;
	m_isFall = false;
	m_velocity = Vector3Const::Zero;

	std::int32_t successBorder = GetManager().lock()->GetGameObject("ResultManager").lock()->GetGameComponent<ResultManager>()->GetSuccessBorder();
	float minVolume = 0.1f;
	float maxVolume = 1.0f;
	m_addGroundedSoundVolume = (maxVolume - minVolume) / successBorder;

	g_groundedSoundVolume = minVolume;
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::Result_FallFriend::Clone()
{
	return ObjectFactory::Create<Result_FallFriend>();
}

void ButiEngine::Result_FallFriend::Fall()
{
	constexpr float gravity = 0.05f;

	m_velocity.y -= gravity;

	Vector3 pos = gameObject.lock()->transform->Translate(m_velocity);

	if (pos.y <= 0.0f)
	{
		auto sound = gameObject.lock()->GetResourceContainer()->GetSound(SoundTag("Sound/Grounded_Base.wav"));
		GetManager().lock()->GetApplication().lock()->GetSoundManager()->PlaySE(sound, 0.5f);

		sound = gameObject.lock()->GetResourceContainer()->GetSound(SoundTag("Sound/Grounded_High.wav"));
		GetManager().lock()->GetApplication().lock()->GetSoundManager()->PlaySE(sound, g_groundedSoundVolume);

		g_groundedSoundVolume += m_addGroundedSoundVolume;
		g_groundedSoundVolume = min(1.0f, g_groundedSoundVolume);

		gameObject.lock()->transform->SetLocalPositionY(0.0f);
		m_isFall = false;
	}
}

void ButiEngine::Result_FallFriend::CheckFall()
{
	if (m_isFallStarted)
	{
		return;
	}

	Vector3 pos = gameObject.lock()->transform->GetLocalPosition();
	Vector3 fallPointPos = m_vwp_fallPoint.lock()->transform->GetLocalPosition();

	if (pos.x >= fallPointPos.x)
	{
		GetManager().lock()->GetGameObject("BackHuman_Body").lock()->GetGameComponent<Result_BackHuman>()->AdvanceFacePartAnimation();

		gameObject.lock()->GetGameComponent<CompleteFriend>()->StartDance();

		m_isFallStarted = true;
		m_isFall = true;
	}
}
