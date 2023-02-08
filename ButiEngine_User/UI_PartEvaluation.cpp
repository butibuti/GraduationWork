#include "stdafx_u.h"
#include "UI_PartEvaluation.h"
#include "NumberDraw.h"
#include "FriendFacePart.h"
#include "Header/GameObjects/DefaultGameComponent/SpriteAnimationComponent.h"

void ButiEngine::UI_PartEvaluation::OnUpdate()
{
	if (m_vlp_timer->IsOn())
	{
		float progress = m_vlp_timer->GetPercent();
		std::int32_t drawScore = MathHelper::Lerp(0, 100, Easing::GetEase(progress, Easing::EasingType::EaseOutExpo));
		drawScore = min(drawScore, m_score);

		CheckLevelUp(drawScore);

		m_vwp_numberDraw.lock()->SetNumber(drawScore);

		if (m_vlp_timer->Update())
		{
			m_vlp_timer->Stop();

			CheckLevelUp(m_score);
			m_vwp_numberDraw.lock()->SetNumber(m_score);
		}
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

	m_vlp_timer = ObjectFactory::Create<RelativeTimer>(60);
	m_vlp_timer->Start();
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::UI_PartEvaluation::Clone()
{
	return ObjectFactory::Create<UI_PartEvaluation>();
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
}

void ButiEngine::UI_PartEvaluation::LevelUpGood()
{
	m_isLevelUpGood = true;

	gameObject.lock()->GetGameComponent<SpriteAnimationComponent>(3)->SetHorizontalAnim(2);
	SetColor(goodColor);
}

void ButiEngine::UI_PartEvaluation::SetColor(const Vector4& arg_color)
{
	m_vwp_numberDraw.lock()->SetColor(arg_color);
	gameObject.lock()->GetGameComponent<MeshDrawComponent>(3)->SetColor(arg_color);
	gameObject.lock()->GetGameComponent<MeshDrawComponent>(4)->SetColor(arg_color);
}
