#include "stdafx_u.h"
#include "Text_CountDown.h"
#include "Header/GameObjects/DefaultGameComponent/SpriteAnimationComponent.h"
#include "Header/GameObjects/DefaultGameComponent/ScaleAnimationComponent.h"

void ButiEngine::Text_CountDown::OnUpdate()
{
	if (m_vlp_lifeTimer->Update())
	{
		m_vlp_lifeTimer->Stop();
		m_vlp_deadTimer->Start();
	}

	if (m_vlp_deadTimer->IsOn())
	{
		float progress = m_vlp_deadTimer->GetPercent();
		float alpha = MathHelper::Lerp(m_startAlpha, 0.0f, Easing::GetEase(progress, Easing::EasingType::EaseOutCirc));

		m_color.w = alpha;
		m_vwp_meshDraw.lock()->SetColor(m_color);

		if (m_vlp_deadTimer->Update())
		{
			m_vlp_deadTimer->Stop();

			m_color.w = 0.0f;
			m_vwp_meshDraw.lock()->SetColor(m_color);

			gameObject.lock()->SetIsRemove(true);
		}
	}
}

void ButiEngine::Text_CountDown::OnSet()
{
}

void ButiEngine::Text_CountDown::OnRemove()
{
}

void ButiEngine::Text_CountDown::OnShowUI()
{
}

void ButiEngine::Text_CountDown::Start()
{
	float rollAngle = ButiRandom::GetRandom(5.0f, 15.0f, 10);
	if (m_number % 2 == 0)
	{
		rollAngle *= -1.0f;
	}
	gameObject.lock()->transform->RollLocalRotationZ_Degrees(rollAngle);

	auto anim = gameObject.lock()->AddGameComponent<ScaleAnimation>();
	anim->SetInitScale(Vector3(0.0f, 0.0f, 1.0f));
	anim->SetTargetScale(Vector3(800.0f, 800.0f, 1.0f));
	anim->SetSpeed(1.0f / 10);
	anim->SetEaseType(Easing::EasingType::EaseOutQuad);

	m_vwp_meshDraw = gameObject.lock()->GetGameComponent<MeshDrawComponent>();
	m_color = m_vwp_meshDraw.lock()->GetColor();
	m_startAlpha = m_color.w;

	m_vlp_lifeTimer = ObjectFactory::Create<RelativeTimer>(40);
	m_vlp_deadTimer = ObjectFactory::Create<RelativeTimer>(20);

	m_vlp_lifeTimer->Start();
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::Text_CountDown::Clone()
{
	return ObjectFactory::Create<Text_CountDown>();
}

void ButiEngine::Text_CountDown::SetNumber(const std::int32_t arg_num)
{
	m_number = 5 - arg_num;
	gameObject.lock()->GetGameComponent<SpriteAnimationComponent>()->SetHorizontalAnim(m_number);
}
