#include "stdafx_u.h"
#include "ScoreNumber.h"
float rotateFrame = 10.0f;
void ButiEngine::ScoreNumber::OnUpdate()
{
	if (!m_vlp_timer->IsOn()) {
		return;
	}
	if (m_vlp_timer->Update()) {
		m_vlp_timer->Reset();
		if (m_displayNum == m_num) {
			m_vlp_timer->Stop();
		}
		m_isChanged = false;
	}
	if (m_vlp_timer->GetPercent() >= 0.25f && !m_isChanged) {
		ChangeNumberMesh();
		m_vlp_timer->SetCount(m_vlp_timer->GetCurrentCountFrame() + rotateFrame * 0.5f);
	}
	gameObject.lock()->transform->SetLocalRotationX_Degrees(-360.0f * m_vlp_timer->GetPercent());
}

void ButiEngine::ScoreNumber::Start()
{
	m_list_vlp_numMeshes = gameObject.lock()->GetGameComponents<MeshDrawComponent>();
	m_vlp_timer = ObjectFactory::Create<RelativeTimer>(rotateFrame);
}

void ButiEngine::ScoreNumber::OnRemove()
{
}

void ButiEngine::ScoreNumber::OnShowUI()
{
	GUI::DragFloat("XRotate", rotateFrame, 0.1f, 100.0f);
}

void ButiEngine::ScoreNumber::SetNumber(const std::int8_t arg_num)
{
	if (m_num == arg_num) { return; }
	m_num = arg_num;
	m_vlp_timer->Start();
}

void ButiEngine::ScoreNumber::ChangeNumberMesh()
{
	m_displayNum = m_num;
	for (std::int32_t index = 0, size = m_list_vlp_numMeshes.GetSize(); index < size; index++) {
		m_list_vlp_numMeshes[index]->GetTransform()->SetLocalScale(index == m_displayNum ? Vector3(1.0f, 1.0f, 1.0f) : Vector3());
	}
	m_isChanged = true;
}

ButiEngine::Value_ptr< ButiEngine::GameComponent> ButiEngine::ScoreNumber::Clone()
{
	auto output = ObjectFactory::Create<ScoreNumber>();
    return output;
}
