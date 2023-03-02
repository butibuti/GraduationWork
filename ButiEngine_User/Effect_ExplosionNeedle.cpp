#include "stdafx_u.h"
#include "Effect_ExplosionNeedle.h"
constexpr float div7 = 1.0f / 7;
void ButiEngine::Effect_ExplosionNeedle::OnUpdate()
{
	for (std::int32_t index = 0; index < m_list_vlp_mesh.GetSize(); index++) {
		auto meshDraw = m_list_vlp_mesh[index];
		auto mul = m_initScale * (0.9f + ButiRandom::GetInt(0, 3) * 0.1f);
		meshDraw->GetTransform()->SetLocalScale(Vector3(1, 7, 1) * mul);
	}
	if (m_frame <= 7) {
		auto scale = Easing::EaseInExpo((m_frame) * div7);
		gameObject.lock()->transform->SetLocalScale(Vector3(scale));
	}
	else if(m_frame<=14) {
		auto scale = Easing::EaseInExpo((m_frame - 7) * div7);
		gameObject.lock()->transform->SetLocalScale(Vector3(1.0f - scale));
	}
	m_frame++;
}

void ButiEngine::Effect_ExplosionNeedle::OnShowUI()
{
	GUI::DragFloat("Scale", m_initScale, 0.01f, 0, 10.0f);
}

void ButiEngine::Effect_ExplosionNeedle::Start()
{
	for (std::int32_t index = 0; index < 12; index++) {
		auto meshDraw = gameObject.lock()->GetGameComponent<MeshDrawComponent>(index);
		meshDraw->GetTransform()->SetLocalScale(Vector3(1,7,1)*m_initScale);
		Vector3 randRotation = Vector3(ButiRandom::GetInt(0, 360), ButiRandom::GetInt(0, 360), ButiRandom::GetInt(0, 360));
		meshDraw->GetTransform()->SetLocalRotation(randRotation);
		m_list_vlp_mesh.Add(meshDraw);
	}
}

ButiEngine::Value_ptr< ButiEngine::GameComponent> ButiEngine::Effect_ExplosionNeedle::Clone()
{
	auto output = ObjectFactory::Create<Effect_ExplosionNeedle>();
	output->m_initScale = m_initScale;
    return output;
}
