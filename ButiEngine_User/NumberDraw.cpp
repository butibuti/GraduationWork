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
		m_vec_vwp_meshDrawComponents[2].lock()->UnRegist();
		if (secondDigit == 0)
		{
			m_vec_vwp_meshDrawComponents[1].lock()->UnRegist();
		}
		else
		{
			m_vec_vwp_meshDrawComponents[1].lock()->Regist();
		}
	}
	else
	{
		m_vec_vwp_meshDrawComponents[1].lock()->Regist();
	}
}
