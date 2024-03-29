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
	GUI::BulletText("Digit");
	if (GUI::DragInt("##DigitCount", &m_digitCount, 1.0f, 1, 10))
	{
		m_digitCount = max(1, m_digitCount);
	}

	GUI::Checkbox("isFillZero", &m_isFillZero);
}

void ButiEngine::NumberDraw::Start()
{
	SetMeshDrawComponent();
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::NumberDraw::Clone()
{
	auto clone = ObjectFactory::Create<NumberDraw>();
	clone->m_digitCount = m_digitCount;
	clone->m_isFillZero = m_isFillZero;
	return clone;
}

void ButiEngine::NumberDraw::SetNumber(const std::int32_t arg_number)
{
	std::vector<std::int32_t> vec_digitNums = GetDigitNums(arg_number);

	if (!m_isFillZero)
	{
		SetHideDigit(arg_number, vec_digitNums);
	}

	for (std::int32_t i = 0; i < m_digitCount; i++)
	{
		m_vec_vwp_spriteAnimationComponents[i].lock()->SetHorizontalAnim(vec_digitNums[i]);
	}
}

void ButiEngine::NumberDraw::SetColor(const Vector4& arg_color)
{
	auto end = m_vec_vwp_meshDrawComponents.end();
	for (auto itr = m_vec_vwp_meshDrawComponents.begin(); itr != end; ++itr)
	{
		(*itr).lock()->SetColor(arg_color);
	}
}

void ButiEngine::NumberDraw::SetMeshDrawComponent()
{
	if (m_isInitialized)
	{
		return;
	}

	for (std::int32_t i = 0; i < m_digitCount; i++)
	{
		m_vec_vwp_meshDrawComponents.push_back(gameObject.lock()->GetGameComponent<MeshDrawComponent>(i));
		m_vec_vwp_spriteAnimationComponents.push_back(gameObject.lock()->GetGameComponent<SpriteAnimationComponent>(i));
	}

	m_defaultScale = m_vec_vwp_meshDrawComponents[0].lock()->GetTransform()->GetLocalScale();


	m_isInitialized = true;
}

void ButiEngine::NumberDraw::SetHideDigit(const std::int32_t arg_number, std::vector<std::int32_t> arg_vec_digitNums)
{
	//一桁目は0でも描画する
	if (m_digitCount <= 1)
	{
		return;
	}

	//一番大きい桁以外の処理
	for (std::int32_t i = 1; i < m_digitCount; i++)
	{
		std::int32_t digit = pow(10, i);
		if (arg_vec_digitNums[i] == 0 && arg_number < digit)
		{
			m_vec_vwp_meshDrawComponents[i].lock()->GetTransform()->SetLocalScale(0.0f);
		}
		else
		{
			m_vec_vwp_meshDrawComponents[i].lock()->GetTransform()->SetLocalScale(m_defaultScale);
		}
	}
}

std::vector<std::int32_t> ButiEngine::NumberDraw::GetDigitNums(const std::int32_t arg_number)
{
	std::vector<std::int32_t> vec_digitNums;
	for (std::int32_t i = 0; i < m_digitCount; i++)
	{
		std::int32_t digit = pow(10, i);
		std::int32_t digitNum = (arg_number % (digit * 10)) / digit;
		vec_digitNums.push_back(digitNum);
	}

	return vec_digitNums;
}
