#include "stdafx_u.h"
#include "UI_PartEvaluation.h"
#include "NumberDraw.h"
#include "FriendFacePart.h"
#include "Header/GameObjects/DefaultGameComponent/SpriteAnimationComponent.h"
#include "Header/GameObjects/DefaultGameComponent/ScaleAnimationComponent.h"

void ButiEngine::UI_PartEvaluation::OnUpdate()
{
	if (m_vlp_timer->IsOn())
	{
		float progress = m_vlp_timer->GetPercent();
		std::int32_t drawScore = MathHelper::Lerp(0, 100, Easing::GetEase(progress, Easing::EasingType::Liner));
		drawScore = min(drawScore, m_score);

		CheckLevelUp(drawScore);

		m_vwp_numberDraw.lock()->SetNumber(drawScore);

		if (m_vlp_timer->Update())
		{
			m_vlp_timer->Stop();

			CheckLevelUp(m_score);
			m_vwp_numberDraw.lock()->SetNumber(m_score);

			PlayRankSound();

			m_vlp_waitDisappearTimer->Start();
		}
	}

	if (m_vlp_waitDisappearTimer->Update())
	{
		m_vlp_waitDisappearTimer->Stop();

		std::int32_t animFrame = 30;
		gameObject.lock()->AddGameComponent<SucideComponent>(animFrame);
		
		auto anim = gameObject.lock()->AddGameComponent<ScaleAnimation>();
		anim->SetInitScale(gameObject.lock()->transform->GetLocalScale());
		anim->SetTargetScale(0.0f);
		anim->SetSpeed(1.0f / animFrame);
		anim->SetEaseType(Easing::EasingType::EaseInBack);
	}
}

void ButiEngine::UI_PartEvaluation::OnSet()
{
}

void ButiEngine::UI_PartEvaluation::OnRemove()
{
}

void ButiEngine::UI_PartEvaluation::OnShowUI()
{
}

void ButiEngine::UI_PartEvaluation::Start()
{
	m_vwp_numberDraw = gameObject.lock()->GetGameComponent<NumberDraw>();

	m_vlp_timer = ObjectFactory::Create<RelativeTimer>(24);
	m_vlp_timer->Start();

	m_vlp_waitDisappearTimer = ObjectFactory::Create<RelativeTimer>(30);

	auto anim = gameObject.lock()->AddGameComponent<ScaleAnimation>();
	anim->SetInitScale(gameObject.lock()->transform->GetLocalScale());
	anim->SetTargetScale(gameObject.lock()->transform->GetLocalScale() * 1.1f);
	anim->SetSpeed(1.0f / 10);
	anim->SetEaseType(Easing::EasingType::Parabola);
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::UI_PartEvaluation::Clone()
{
	return ObjectFactory::Create<UI_PartEvaluation>();
}

void ButiEngine::UI_PartEvaluation::SetRank(const Rank arg_rank)
{
	m_rank = arg_rank;
	return;
	if (m_rank == Rank::Bad)
	{
		gameObject.lock()->GetGameComponent<SpriteAnimationComponent>(3)->SetHorizontalAnim(0);
	}
	else if (m_rank == Rank::Normal)
	{
		gameObject.lock()->GetGameComponent<SpriteAnimationComponent>(3)->SetHorizontalAnim(1);
	}
	else if (m_rank == Rank::Good)
	{
		gameObject.lock()->GetGameComponent<SpriteAnimationComponent>(3)->SetHorizontalAnim(2);
	}
}

void ButiEngine::UI_PartEvaluation::CheckLevelUp(const std::int32_t arg_score)
{
	if (!m_isLevelUpNormal && arg_score >= FriendFacePart::GetNormalScoreBorder())
	{
		LevelUpNormal();
	}
	else if (!m_isLevelUpGood && arg_score >= FriendFacePart::GetGoodScoreBorder())
	{
		LevelUpGood();
	}
}

const ButiEngine::Vector4 normalColor = ButiEngine::Vector4(0.3f, 0.55f, 0.81f, 1.0f);
const ButiEngine::Vector4 goodColor = ButiEngine::Vector4(0.93f, 0.48f, 0.08f, 1.0f);

void ButiEngine::UI_PartEvaluation::LevelUpNormal()
{
	m_isLevelUpNormal = true;

	gameObject.lock()->GetGameComponent<SpriteAnimationComponent>(3)->SetHorizontalAnim(1);
	SetColor(normalColor);

	auto sound = gameObject.lock()->GetResourceContainer()->GetSound(SoundTag("Sound/LevelUp_Normal.wav"));
	GetManager().lock()->GetApplication().lock()->GetSoundManager()->PlaySE(sound, 1.0f);

	auto anim = gameObject.lock()->AddGameComponent<ScaleAnimation>();
	anim->SetInitScale(gameObject.lock()->transform->GetLocalScale());
	anim->SetTargetScale(gameObject.lock()->transform->GetLocalScale() * 1.1f);
	anim->SetSpeed(1.0f / 5);
	anim->SetEaseType(Easing::EasingType::Parabola);
}

void ButiEngine::UI_PartEvaluation::LevelUpGood()
{
	m_isLevelUpGood = true;

	gameObject.lock()->GetGameComponent<SpriteAnimationComponent>(3)->SetHorizontalAnim(2);
	SetColor(goodColor);

	auto sound = gameObject.lock()->GetResourceContainer()->GetSound(SoundTag("Sound/LevelUp_Good.wav"));
	GetManager().lock()->GetApplication().lock()->GetSoundManager()->PlaySE(sound, 0.5f);

	auto anim = gameObject.lock()->AddGameComponent<ScaleAnimation>();
	anim->SetInitScale(gameObject.lock()->transform->GetLocalScale());
	anim->SetTargetScale(gameObject.lock()->transform->GetLocalScale() * 1.2f);
	anim->SetSpeed(1.0f / 10);
	anim->SetEaseType(Easing::EasingType::Parabola);
}

void ButiEngine::UI_PartEvaluation::SetColor(const Vector4& arg_color)
{
	m_vwp_numberDraw.lock()->SetColor(arg_color);
	gameObject.lock()->GetGameComponent<MeshDrawComponent>(4)->SetColor(arg_color);
}

void ButiEngine::UI_PartEvaluation::PlayRankSound()
{
	std::string soundName = "Sound/Normal.wav";
	if (m_rank == Rank::Bad)
	{
		soundName = "Sound/Bad.wav";
	}
	else if (m_rank == Rank::Normal)
	{
		soundName = "Sound/Normal.wav";
	}
	else if (m_rank == Rank::Good)
	{
		soundName = "Sound/Good.wav";
	}

	auto sound = gameObject.lock()->GetResourceContainer()->GetSound(SoundTag(soundName));
	GetManager().lock()->GetApplication().lock()->GetSoundManager()->PlaySE(sound, 0.5f);
}
