#include "stdafx_u.h"
#include "UI_FriendCount.h"
#include "FriendManager.h"
#include "NumberDraw.h"
#include "ResultManager.h"

void ButiEngine::UI_FriendCount::OnUpdate()
{
	Animation();
	CheckNextAnimation();
}

void ButiEngine::UI_FriendCount::OnSet()
{
}

void ButiEngine::UI_FriendCount::OnRemove()
{
}

void ButiEngine::UI_FriendCount::OnShowUI()
{
}

void ButiEngine::UI_FriendCount::Start()
{
	m_startNumberPos = 0.0f;
	m_targetNumberPos = 0.25f;

	for (std::int32_t i = 0; i < 4; i++)
	{
		m_vec_vwp_numbers.push_back(gameObject.lock()->GetGameComponent<MeshDrawComponent>(i));
	}

	m_defaultNumberScale = m_vec_vwp_numbers[0].lock()->GetTransform()->GetLocalScale();
	m_startNumberScale = m_defaultNumberScale;
	m_targetNumberScale = m_defaultNumberScale * 1.1f;

	m_animFrame = 6;
	m_vlp_animTimer = ObjectFactory::Create<RelativeTimer>(m_animFrame);

	m_isAddCountAnimation = false;
	m_isRemoveCountAnimation = false;
	m_isReturnAnimation = false;

	m_isFirstAnimation = true;
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::UI_FriendCount::Clone()
{
	return ObjectFactory::Create<UI_FriendCount>();
}

void ButiEngine::UI_FriendCount::AddCount()
{
	m_targetCount++;
	m_targetCount = min(m_targetCount, 99);
}

void ButiEngine::UI_FriendCount::RemoveCount()
{
	m_targetCount--;
	m_targetCount = max(m_targetCount, 0);
}

void ButiEngine::UI_FriendCount::CheckNextAnimation()
{
	if (m_vlp_animTimer->IsOn())
	{
		return;
	}

	if (m_isAddCountAnimation || m_isRemoveCountAnimation)
	{
		StartReturnAnimation();
		return;
	}

	if (m_drawCount == m_targetCount)
	{
		return;
	}

	m_vlp_animTimer->ChangeCountFrame(m_animFrame);
	if(m_drawCount < m_targetCount)
	{
		StartAddCountAnimation();
	}
	else if (m_drawCount > m_targetCount)
	{
		StartRemoveCountAnimation();
	}
}

void ButiEngine::UI_FriendCount::CheckRestartAnimation()
{
	if (m_drawCount == m_targetCount)
	{
		return;
	}

	if (m_isAddCountAnimation && m_drawCount < m_targetCount)
	{
		m_isReturnAnimation = false;
		m_vlp_animTimer->ChangeCountFrame(m_animFrame * 0.5f);
		StartAddCountAnimation();
	}
	else if (m_isRemoveCountAnimation && m_drawCount > m_targetCount)
	{
		m_isReturnAnimation = false;
		m_vlp_animTimer->ChangeCountFrame(m_animFrame * 0.5f);
		StartRemoveCountAnimation();
	}
}

void ButiEngine::UI_FriendCount::StartAddCountAnimation()
{
	m_vlp_animTimer->Reset();

	m_drawCount++;
	gameObject.lock()->GetGameComponent<NumberDraw>()->SetNumber(m_drawCount + m_drawCount * 100);

	m_startNumberPos = m_vec_vwp_numbers[0].lock()->GetTransform()->GetLocalPosition().y;
	m_targetNumberPos = 0.35f;

	m_startNumberScale = m_vec_vwp_numbers[0].lock()->GetTransform()->GetLocalScale();
	m_targetNumberScale = m_defaultNumberScale * 1.2f;

	m_vlp_animTimer->Start();

	m_isAddCountAnimation = true;
}

void ButiEngine::UI_FriendCount::StartRemoveCountAnimation()
{
	m_vlp_animTimer->Reset();

	m_drawCount--;
	gameObject.lock()->GetGameComponent<NumberDraw>()->SetNumber(m_drawCount + m_drawCount * 100);

	m_startNumberPos = m_vec_vwp_numbers[0].lock()->GetTransform()->GetLocalPosition().y;
	m_targetNumberPos = -0.35f;

	m_startNumberScale = m_vec_vwp_numbers[0].lock()->GetTransform()->GetLocalScale();
	m_targetNumberScale = m_defaultNumberScale * 0.8f;

	m_vlp_animTimer->Start();

	m_isRemoveCountAnimation = true;
}

void ButiEngine::UI_FriendCount::StartReturnAnimation()
{
	m_vlp_animTimer->Reset();

	m_startNumberPos = m_vec_vwp_numbers[0].lock()->GetTransform()->GetLocalPosition().y;
	m_targetNumberPos = 0.0f;

	m_startNumberScale = m_vec_vwp_numbers[0].lock()->GetTransform()->GetLocalScale();
	m_targetNumberScale = m_defaultNumberScale;

	m_vlp_animTimer->Start();

	m_isReturnAnimation = true;
}

void ButiEngine::UI_FriendCount::Animation()
{
	if (!m_vlp_animTimer->IsOn())
	{
		return;
	}

	if (m_vlp_animTimer->Update())
	{
		m_vlp_animTimer->Stop();

		SetNumberPos(m_targetNumberPos);
		SetNumberScale(m_targetNumberScale);

		if (m_isReturnAnimation)
		{
			m_isAddCountAnimation = false;
			m_isRemoveCountAnimation = false;
			m_isReturnAnimation = false;
		}

		PlaySE();
		CheckColor();

		return;
	}

	float progress = m_vlp_animTimer->GetPercent();
	float newPos = MathHelper::Lerp(m_startNumberPos, m_targetNumberPos, progress);
	SetNumberPos(newPos);

	Vector3 newScale = MathHelper::LerpPosition(m_startNumberScale, m_targetNumberScale, progress);
	SetNumberScale(newScale);

	if (m_isReturnAnimation && progress >= 0.5f)
	{
		CheckRestartAnimation();
	}
}

void ButiEngine::UI_FriendCount::SetNumberPos(const float arg_pos)
{
	auto end = m_vec_vwp_numbers.end();
	for (auto itr = m_vec_vwp_numbers.begin(); itr != end; ++itr)
	{
		(*itr).lock()->GetTransform()->SetLocalPositionY(arg_pos);
	}
}

void ButiEngine::UI_FriendCount::SetNumberScale(const Vector3& arg_scale)
{
	auto end = m_vec_vwp_numbers.end();
	for (auto itr = m_vec_vwp_numbers.begin(); itr != end; ++itr)
	{
		(*itr).lock()->GetTransform()->SetLocalScale(arg_scale);
	}
}

void ButiEngine::UI_FriendCount::SetNumberColor(const Vector4& arg_color)
{
	m_vec_vwp_numbers[0].lock()->SetColor(arg_color);
	m_vec_vwp_numbers[1].lock()->SetColor(arg_color);
}

void ButiEngine::UI_FriendCount::SetUnitColor(const Vector4& arg_color)
{
	gameObject.lock()->GetGameComponent<MeshDrawComponent>(4)->SetColor(arg_color);
}

void ButiEngine::UI_FriendCount::PlaySE()
{
	if (m_isFirstAnimation)
	{
		return;
	}
	
	if (m_isAddCountAnimation)
	{
		auto sound = gameObject.lock()->GetResourceContainer()->GetSound(SoundTag("Sound/Grounded_High.wav"));
		GetManager().lock()->GetApplication().lock()->GetSoundManager()->PlaySE(sound, 0.5f);
	}
	else if (m_isRemoveCountAnimation)
	{
		auto sound = gameObject.lock()->GetResourceContainer()->GetSound(SoundTag("Sound/Grounded_Base.wav"));
		GetManager().lock()->GetApplication().lock()->GetSoundManager()->PlaySE(sound, 0.5f);
	}

	m_isFirstAnimation = false;
}

void ButiEngine::UI_FriendCount::CheckColor()
{
	if (m_drawCount >= ResultManager::GetSuccessBorder())
	{
		SetNumberColor(m_successColor);
		SetUnitColor(m_successColor);
	}
	else
	{
		SetNumberColor(m_defaultColor);
		SetUnitColor(m_defaultColor);
	}
}
