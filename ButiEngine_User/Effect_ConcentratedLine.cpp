#include "stdafx_u.h"
#include "Effect_ConcentratedLine.h"

void ButiEngine::Effect_ConcentratedLine::OnUpdate()
{
	gameObject.lock()->transform->RollLocalRotationZ_Degrees(m_rotateSpeed);

	if (m_vlp_lifeTimer->Update())
	{
		m_vwp_parent.lock()->SetIsRemove(true);
		gameObject.lock()->SetIsRemove(true);
	}
}

void ButiEngine::Effect_ConcentratedLine::OnSet()
{
}

void ButiEngine::Effect_ConcentratedLine::OnRemove()
{
}

void ButiEngine::Effect_ConcentratedLine::OnShowUI()
{
	GUI::BulletText("LifeTime");
	GUI::DragInt("##Life", m_lifeTime, 1.0f, 0, 600);

	GUI::BulletText("RotateSpeed");
	GUI::DragFloat("##RotateSpeed", m_rotateSpeed, 0.1f, 0.0f, 100.0f);
}

void ButiEngine::Effect_ConcentratedLine::Start()
{
	m_vwp_parent = GetManager().lock()->AddObjectFromCereal("Effect_ConcentratedLineParent");
	gameObject.lock()->transform->SetBaseTransform(m_vwp_parent.lock()->transform);
	gameObject.lock()->transform->SetLocalPosition(Vector3Const::Zero);

	m_vlp_lifeTimer = ObjectFactory::Create<RelativeTimer>(m_lifeTime);
	m_vlp_lifeTimer->Start();
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::Effect_ConcentratedLine::Clone()
{
	auto clone = ObjectFactory::Create<Effect_ConcentratedLine>();
	clone->m_lifeTime = m_lifeTime;
	clone->m_rotateSpeed = m_rotateSpeed;
	return clone;
}
