#include "stdafx_u.h"
#include "Effect_ExplosionSmoke.h"
#include"ButiUtil/ButiUtil/Easing.h"
constexpr float div30 = 1.0f / 30;
void ButiEngine::Effect_ExplosionSmoke::OnUpdate()
{
	for (std::int32_t index = 0; index < m_list_vlp_mesh.GetSize(); index++) {
		auto meshDraw = m_list_vlp_mesh[index];
		auto progress = Easing::EaseOutExpo(m_frame * div30);
		meshDraw->GetTransform()->SetLocalScale(Vector3( progress*(m_targetSmokeScale-m_initSmokeScale)+m_initSmokeScale));
	}
	m_frame++;
	if (m_frame < 17 || m_frame>27) {}
	else {
		auto scale = Easing::EaseInExpo((m_frame - 17) * 0.1f);
		gameObject.lock()->transform->SetLocalScale(Vector3(1.0f - scale));
	}

}

void ButiEngine::Effect_ExplosionSmoke::OnShowUI()
{
	GUI::DragFloat("initSmokeScale", m_initSmokeScale, 0.01f, 0.0f, 10.0f);
	GUI::DragFloat("targetSmokeScale", m_targetSmokeScale, 0.01f, 0.0f, 10.0f);
	GUI::DragFloat("minSmokeDistance", m_minSmokeDistance, 0.01f, 0.0f, 10.0f);
	GUI::DragFloat("maxSmokeDistance", m_maxSmokeDistance, 0.01f, 0.0f, 10.0f);
}

void ButiEngine::Effect_ExplosionSmoke::Start()
{
	for (std::int32_t index = 0; index < 6; index++) {
		auto meshDraw = gameObject.lock()->GetGameComponent<MeshDrawComponent>(index);
		//meshDraw->GetTransform()->SetLocalScale(Vector3(m_initSmokeScale));
		Vector3 randPosition=Vector3(ButiRandom::GetInt(-8, 8), ButiRandom::GetInt(-8, 8), ButiRandom::GetInt(-8, 8)).Normalize();
		randPosition *= ButiRandom::GetRandom(m_minSmokeDistance, m_maxSmokeDistance, 16);
		meshDraw->GetTransform()->SetLocalPosition(randPosition);
		m_list_vlp_mesh.Add(meshDraw);
	}
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::Effect_ExplosionSmoke::Clone()
{
	auto output = ObjectFactory::Create<Effect_ExplosionSmoke>();
	output->m_initSmokeScale = m_initSmokeScale;
	output->m_targetSmokeScale = m_targetSmokeScale;
	output->m_maxSmokeDistance = m_maxSmokeDistance;
	output->m_minSmokeDistance= m_minSmokeDistance;

    return output;
}
