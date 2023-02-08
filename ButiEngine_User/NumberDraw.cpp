#include "NumberDraw.h"
#include "Header/GameObjects/DefaultGameComponent/SpriteAnimationComponent.h"

void ButiEngine::NumberDraw::OnUpdate()
{
}

void ButiEngine::NumberDraw::OnSet()
{
}

void ButiEngine::NumberDraw::OnRemove()
{
}

void ButiEngine::NumberDraw::OnShowUI()
{
}

void ButiEngine::NumberDraw::Start()
{
	for (std::int32_t i = 0; i < 3; i++)
	{
		m_vec_vwp_meshDrawComponents.push_back(gameObject.lock()->GetGameComponent<MeshDrawComponent>(i));
		m_vec_vwp_spriteAnimationComponents.push_back(gameObject.lock()->GetGameComponent<SpriteAnimationComponent>(i));
	}

	m_defaultScale = m_vec_vwp_meshDrawComponents[0].lock()->GetTransform()->GetLocalScale();
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::NumberDraw::Clone()
{
	return ObjectFactory::Create<NumberDraw>();
}

void ButiEngine::NumberDraw::SetNumber(const std::int32_t arg_number)
{
	std::int32_t firstDigit = arg_number % 10;
	std::int32_t secondDigit = arg_number % 100 / 10;
	std::int32_t thirdDigit = arg_number / 100;

	if (thirdDigit == 0)
	{
		m_vec_vwp_meshDrawComponents[2].lock()->GetTransform()->SetLocalScale(0.0f);
		if (secondDigit == 0)
		{
			m_vec_vwp_meshDrawComponents[1].lock()->GetTransform()->SetLocalScale(0.0f);
		}
		else
		{
			m_vec_vwp_meshDrawComponents[1].lock()->GetTransform()->SetLocalScale(m_defaultScale);
		}
	}
	else
	{
		m_vec_vwp_meshDrawComponents[2].lock()->GetTransform()->SetLocalScale(m_defaultScale);
	}

	m_vec_vwp_spriteAnimationComponents[0].lock()->SetHorizontalAnim(firstDigit);
	m_vec_vwp_spriteAnimationComponents[1].lock()->SetHorizontalAnim(secondDigit);
	m_vec_vwp_spriteAnimationComponents[2].lock()->SetHorizontalAnim(thirdDigit);
}

void ButiEngine::NumberDraw::SetColor(const Vector4& arg_color)
{
	auto end = m_vec_vwp_meshDrawComponents.end();
	for (auto itr = m_vec_vwp_meshDrawComponents.begin(); itr != end; ++itr)
	{
		(*itr).lock()->SetColor(arg_color);
	}
}
