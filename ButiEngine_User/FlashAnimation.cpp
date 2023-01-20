#include "stdafx_u.h"
#include "FlashAnimation.h"
#include "Header/GameObjects/DefaultGameComponent/ModelDrawComponent.h"

void ButiEngine::FlashAnimation::OnUpdate()
{
	if (m_loopCount == m_intervalFrame)
	{
		m_loopCount = 0;

		ChangeMeshDrawColor();
		ChangeModelDrawColor();
		return;
	}
	m_loopCount++;
}

void ButiEngine::FlashAnimation::OnSet()
{
	m_vwp_meshDraw = gameObject.lock()->GetGameComponent<MeshDrawComponent>();
	m_vwp_modelDraw = gameObject.lock()->GetGameComponent<ModelDrawComponent>();

	if (m_vwp_meshDraw.lock())
	{
		m_startColor = m_vwp_meshDraw.lock()->GetColor();
	}
	else if (m_vwp_modelDraw.lock())
	{
		m_startColor = m_vwp_modelDraw.lock()->GetColor();
	}

	m_loopCount = 0;
	m_changeColorCount = 0;
}

void ButiEngine::FlashAnimation::OnRemove()
{
	SetStartColor(m_startColor);
}

void ButiEngine::FlashAnimation::OnShowUI()
{
	if (GUI::DragFloat3("StartColor", &m_startColor.x, 0.1f, 0.0f, 1.0f))
	{
		SetStartColor(m_startColor);
	}
	GUI::DragFloat3("TargetColor", &m_targetColor.x, 0.1f, 0.0f, 1.0f);

	if (GUI::DragInt("IntervalFrame", m_intervalFrame, 1.0f, 0, 100))
	{
		m_loopCount = 0;
	}
}

void ButiEngine::FlashAnimation::Start()
{
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::FlashAnimation::Clone()
{
	auto clone = ObjectFactory::Create<FlashAnimation>();
	clone->m_startColor = m_startColor;
	clone->m_targetColor = m_targetColor;
	clone->m_intervalFrame = m_intervalFrame;
	return clone;
}

void ButiEngine::FlashAnimation::SetStartColor(const Vector3& arg_startColor)
{
	m_startColor = arg_startColor;

	if (m_vwp_meshDraw.lock())
	{
		float alpha = m_vwp_meshDraw.lock()->GetColor().w;
		m_vwp_meshDraw.lock()->SetColor(Vector4(m_startColor, alpha));
	}
	else if (m_vwp_modelDraw.lock())
	{
		float alpha = m_vwp_modelDraw.lock()->GetColor().w;
		m_vwp_modelDraw.lock()->SetColor(Vector4(m_startColor, alpha));
	}
}

void ButiEngine::FlashAnimation::ChangeMeshDrawColor()
{
	if (!m_vwp_meshDraw.lock())
	{
		return;
	}

	float alpha = m_vwp_meshDraw.lock()->GetColor().w;
	if (m_changeColorCount % 2 == 0)
	{
		m_vwp_meshDraw.lock()->SetColor(Vector4(m_startColor, alpha));
	}
	else
	{
		m_vwp_meshDraw.lock()->SetColor(Vector4(m_targetColor, alpha));
	}

	m_changeColorCount++;
}

void ButiEngine::FlashAnimation::ChangeModelDrawColor()
{
	if (!m_vwp_modelDraw.lock())
	{
		return;
	}

	float alpha = m_vwp_modelDraw.lock()->GetColor().w;
	if (m_changeColorCount % 2 == 0)
	{
		m_vwp_modelDraw.lock()->SetColor(Vector4(m_startColor, alpha));
	}
	else
	{
		m_vwp_modelDraw.lock()->SetColor(Vector4(m_targetColor, alpha));
	}

	m_changeColorCount++;
}
