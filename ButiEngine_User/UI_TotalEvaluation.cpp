#include "stdafx_u.h"
#include "UI_TotalEvaluation.h"
#include "FriendFacePart.h"
#include "Header/GameObjects/DefaultGameComponent/SpriteAnimationComponent.h"

void ButiEngine::UI_TotalEvaluation::OnUpdate()
{
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
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::UI_TotalEvaluation::Clone()
{
	return ObjectFactory::Create<UI_TotalEvaluation>();
}

void ButiEngine::UI_TotalEvaluation::Appear()
{
}

void ButiEngine::UI_TotalEvaluation::Disappear()
{
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
}

void ButiEngine::UI_TotalEvaluation::AddPositionAnimation(const Vector3& arg_targetPos, Easing::EasingType arg_easeType, const std::int32_t arg_animFrame)
{
}
