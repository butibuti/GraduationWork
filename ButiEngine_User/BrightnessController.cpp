#include "stdafx_u.h"
#include "BrightnessController.h"
#include "Header/GameObjects/DefaultGameComponent/ModelDrawComponent.h"

void ButiEngine::BrightnessController::OnUpdate()
{
}

void ButiEngine::BrightnessController::OnSet()
{
}

void ButiEngine::BrightnessController::OnRemove()
{
}

void ButiEngine::BrightnessController::OnShowUI()
{
	GUI::Checkbox("isMeshDraw", &m_isMeshDraw);
}

void ButiEngine::BrightnessController::Start()
{
	ResetMeshDraw();

	ResetStartColor();
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::BrightnessController::Clone()
{
	auto output = ObjectFactory::Create<BrightnessController>();
	output->m_isMeshDraw = m_isMeshDraw;
	return output;
}

void ButiEngine::BrightnessController::ResetMeshDraw()
{
	m_vec_vwp_meshDrawComponents.clear();
	m_vec_vwp_modelDrawComponents.clear();

	for (std::int32_t i = 0; i < 3; ++i)
	{
		if (m_isMeshDraw)
		{
			auto meshDraw = gameObject.lock()->GetGameComponent<MeshDrawComponent>(i);
			m_vec_vwp_meshDrawComponents.push_back(meshDraw);
		}
		else
		{
			auto modelDraw = gameObject.lock()->GetGameComponent<ModelDrawComponent>(i);
			m_vec_vwp_modelDrawComponents.push_back(modelDraw);
		}
	}
}

void ButiEngine::BrightnessController::ResetStartColor()
{
	m_vec_startColors.clear();

	if (m_isMeshDraw)
	{
		auto meshEnd = m_vec_vwp_meshDrawComponents.end();
		for (auto itr = m_vec_vwp_meshDrawComponents.begin(); itr != meshEnd; ++itr)
		{
			m_vec_startColors.push_back((*itr).lock()->GetColor());
		}
	}
	else
	{
		auto modelEnd = m_vec_vwp_modelDrawComponents.end();
		for (auto itr = m_vec_vwp_modelDrawComponents.begin(); itr != modelEnd; ++itr)
		{
			m_vec_startColors.push_back((*itr).lock()->GetColor());
		}
	}
}

void ButiEngine::BrightnessController::SetBrightness(const float arg_brightness)
{
	for (std::int32_t i = 0; i < 3; ++i)
	{
		Vector3 newColor = m_vec_startColors[i] * arg_brightness;
		if (m_isMeshDraw)
		{
			m_vec_vwp_meshDrawComponents[i].lock()->SetColor(Vector4(newColor, 1.0f));
		}
		else
		{
			m_vec_vwp_modelDrawComponents[i].lock()->SetColor(Vector4(newColor, 1.0f));
		}
	}
}
