#include "stdafx_u.h"
#include "UI_TotalEvaluation.h"
#include "FriendFacePart.h"
#include "Header/GameObjects/DefaultGameComponent/SpriteAnimationComponent.h"
#include "Header/GameObjects/DefaultGameComponent/PositionAnimationComponent.h"

void ButiEngine::UI_TotalEvaluation::OnUpdate()
{
	if (m_vlp_appearIntervalTimer->Update())
	{
		auto sound = gameObject.lock()->GetResourceContainer()->GetSound(SoundTag("Sound/LevelUp_Normal.wav"));
		GetManager().lock()->GetApplication().lock()->GetSoundManager()->PlaySE(sound, 0.5f);
		AddPositionAnimation(-890.0f, Easing::EasingType::EaseOutExpo, 10, m_appearIndex);

		m_appearIndex++;
		std::int32_t textCount = m_vec_vwp_texts.size();
		if (m_appearIndex == textCount)
		{
			m_vlp_appearIntervalTimer->Stop();
		}
	}

	if (m_vlp_waitDisappearTimer->Update())
	{
		m_vlp_waitDisappearTimer->Stop();

		std::int32_t textCount = m_vec_vwp_texts.size();
		for (std::int32_t i = 0; i < textCount; i++)
		{
			AddPositionAnimation(-1700.0f, Easing::EasingType::EaseInCirc, 5, i);
		}
	}
}

void ButiEngine::UI_TotalEvaluation::OnSet()
{
}

void ButiEngine::UI_TotalEvaluation::OnRemove()
{
}

void ButiEngine::UI_TotalEvaluation::OnShowUI()
{
}

void ButiEngine::UI_TotalEvaluation::Start()
{
	auto total = GetManager().lock()->GetGameObject("UI_TotalEvaluation_Total");
	auto increase = GetManager().lock()->GetGameObject("UI_TotalEvaluation_Increase");
	auto eye = GetManager().lock()->GetGameObject("UI_TotalEvaluation_Eye");
	auto nose = GetManager().lock()->GetGameObject("UI_TotalEvaluation_Nose");
	auto mouth = GetManager().lock()->GetGameObject("UI_TotalEvaluation_Mouth");

	m_vec_vwp_texts.push_back(total);
	m_vec_vwp_texts.push_back(increase);
	m_vec_vwp_texts.push_back(eye);
	m_vec_vwp_texts.push_back(nose);
	m_vec_vwp_texts.push_back(mouth);

	m_appearIndex = 0;
	m_vlp_appearIntervalTimer = ObjectFactory::Create<RelativeTimer>(4);

	m_vlp_waitDisappearTimer = ObjectFactory::Create<RelativeTimer>(60);
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::UI_TotalEvaluation::Clone()
{
	return ObjectFactory::Create<UI_TotalEvaluation>();
}

void ButiEngine::UI_TotalEvaluation::Appear()
{
	if (m_vlp_appearIntervalTimer->IsOn())
	{
		return;
	}

	m_appearIndex = 0;
	m_vlp_appearIntervalTimer->Start();
}

void ButiEngine::UI_TotalEvaluation::Disappear()
{
	m_vlp_appearIntervalTimer->Reset();
	m_vlp_appearIntervalTimer->Stop();

	m_vlp_waitDisappearTimer->Start();
}

const ButiEngine::Vector4 badColor = ButiEngine::Vector4(0.39f, 0.39f, 0.39f, 1.0f);
const ButiEngine::Vector4 normalColor = ButiEngine::Vector4(0.3f, 0.55f, 0.81f, 1.0f);
const ButiEngine::Vector4 goodColor = ButiEngine::Vector4(0.93f, 0.48f, 0.08f, 1.0f);

void ButiEngine::UI_TotalEvaluation::SetTotalRank(const Rank arg_rank)
{
	if (arg_rank == Rank::Bad)
	{
		m_vec_vwp_texts[0].lock()->GetGameComponent<SpriteAnimationComponent>()->SetHorizontalAnim(0);
		m_vec_vwp_texts[1].lock()->GetGameComponent<MeshDrawComponent>(0)->SetColor(badColor);
		m_vec_vwp_texts[1].lock()->GetGameComponent<SpriteAnimationComponent>()->SetHorizontalAnim(1);
	}
	else if (arg_rank == Rank::Normal)
	{
		m_vec_vwp_texts[0].lock()->GetGameComponent<SpriteAnimationComponent>()->SetHorizontalAnim(1);
		m_vec_vwp_texts[1].lock()->GetGameComponent<MeshDrawComponent>(0)->SetColor(normalColor);
		m_vec_vwp_texts[1].lock()->GetGameComponent<SpriteAnimationComponent>()->SetHorizontalAnim(2);
	}
	else if (arg_rank == Rank::Good)
	{
		m_vec_vwp_texts[0].lock()->GetGameComponent<SpriteAnimationComponent>()->SetHorizontalAnim(2);
		m_vec_vwp_texts[1].lock()->GetGameComponent<MeshDrawComponent>(0)->SetColor(goodColor);
		m_vec_vwp_texts[1].lock()->GetGameComponent<SpriteAnimationComponent>()->SetHorizontalAnim(3);
	}
}

void ButiEngine::UI_TotalEvaluation::SetPartRank(const Rank arg_eyeRank, const Rank arg_noseRank, const Rank arg_mouthRank)
{
	if (arg_eyeRank == Rank::Bad)
	{
		m_vec_vwp_texts[2].lock()->GetGameComponent<SpriteAnimationComponent>()->SetHorizontalAnim(0);
	}
	else if (arg_eyeRank == Rank::Normal)
	{
		m_vec_vwp_texts[2].lock()->GetGameComponent<SpriteAnimationComponent>()->SetHorizontalAnim(1);
	}
	else if (arg_eyeRank == Rank::Good)
	{
		m_vec_vwp_texts[2].lock()->GetGameComponent<SpriteAnimationComponent>()->SetHorizontalAnim(2);
	}

	if (arg_noseRank == Rank::Bad)
	{
		m_vec_vwp_texts[3].lock()->GetGameComponent<SpriteAnimationComponent>()->SetHorizontalAnim(0);
	}
	else if (arg_noseRank == Rank::Normal)
	{
		m_vec_vwp_texts[3].lock()->GetGameComponent<SpriteAnimationComponent>()->SetHorizontalAnim(1);
	}
	else if (arg_noseRank == Rank::Good)
	{
		m_vec_vwp_texts[3].lock()->GetGameComponent<SpriteAnimationComponent>()->SetHorizontalAnim(2);
	}

	if (arg_mouthRank == Rank::Bad)
	{
		m_vec_vwp_texts[4].lock()->GetGameComponent<SpriteAnimationComponent>()->SetHorizontalAnim(0);
	}
	else if (arg_mouthRank == Rank::Normal)
	{
		m_vec_vwp_texts[4].lock()->GetGameComponent<SpriteAnimationComponent>()->SetHorizontalAnim(1);
	}
	else if (arg_mouthRank == Rank::Good)
	{
		m_vec_vwp_texts[4].lock()->GetGameComponent<SpriteAnimationComponent>()->SetHorizontalAnim(2);
	}
}

void ButiEngine::UI_TotalEvaluation::AddPositionAnimation(const float arg_targetPosX, Easing::EasingType arg_easeType, const std::int32_t arg_animFrame, const std::int32_t arg_index)
{
	auto addAnimObject = m_vec_vwp_texts[arg_index];
	auto anim = addAnimObject.lock()->AddGameComponent<PositionAnimation>();

	Vector3 pos = addAnimObject.lock()->transform->GetLocalPosition();
	anim->SetInitPosition(pos);

	Vector3 targetPos = pos;
	targetPos.x = arg_targetPosX;
	anim->SetTargetPosition(targetPos);

	anim->SetSpeed(1.0f / arg_animFrame);
	anim->SetEaseType(arg_easeType);
}
