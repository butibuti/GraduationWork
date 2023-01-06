#include "stdafx_u.h"
#include "SceneTransition_Fade.h"

void ButiEngine::SceneTransition_Fade::OnUpdate()
{
	if (m_vlp_waitTimer->Update())
	{
		m_vlp_waitTimer->Stop();
		m_vlp_fadeTimer->Start();
	}

	if (!m_vlp_fadeTimer->IsOn())
	{
		return;
	}

	float alpha = m_vlp_fadeTimer->GetPercent();
	Vector4 color = m_startColor;
	color.w = m_isSceneEnd ? alpha : (1.0f - alpha);
	m_vwp_meshDraw.lock()->SetColor(color);

	if (m_vlp_fadeTimer->Update())
	{
		m_vlp_fadeTimer->Stop();

		color.w = m_isSceneEnd ? 1.0f : 0.0f;
		m_vwp_meshDraw.lock()->SetColor(color);

		gameObject.lock()->SetIsRemove(true);

		if (m_isSceneEnd)
		{
			auto sceneManager = gameObject.lock()->GetApplication().lock()->GetSceneManager();
			std::string sceneName = m_nextSceneName;
			sceneManager->RemoveScene(sceneName);
			sceneManager->LoadScene(sceneName);
			sceneManager->ChangeScene(sceneName);
		}
	}
}

void ButiEngine::SceneTransition_Fade::OnSet()
{
}

void ButiEngine::SceneTransition_Fade::OnRemove()
{
}

void ButiEngine::SceneTransition_Fade::OnShowUI()
{
	GUI::Checkbox("IsFadeIn", m_isSceneEnd);

	GUI::BulletText("WaitFrame");
	GUI::DragInt("##WaitFrame", m_waitFrame, 1.0f, 0, 300);

	GUI::BulletText("TransitionFrame");
	GUI::DragInt("##TransitionFrame", m_transitionFrame, 1.0f, 0, 300);

	GUI::BulletText("NextSceneName:" + m_nextSceneName);
	if (GUI::InputText("##NextSceneName")) 
	{
		m_nextSceneName = GUI::GetTextBuffer("##NextSceneName");
	}
}

void ButiEngine::SceneTransition_Fade::Start()
{
	m_vwp_meshDraw = gameObject.lock()->GetGameComponent<MeshDrawComponent>();

	m_vlp_waitTimer = ObjectFactory::Create<RelativeTimer>(m_waitFrame);
	m_vlp_waitTimer->Start();

	m_vlp_fadeTimer = ObjectFactory::Create<RelativeTimer>(m_transitionFrame);

	m_startColor = m_vwp_meshDraw.lock()->GetColor();
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::SceneTransition_Fade::Clone()
{
	auto clone = ObjectFactory::Create<SceneTransition_Fade>();
	clone->m_isSceneEnd = m_isSceneEnd;
	clone->m_waitFrame = m_waitFrame;
	clone->m_transitionFrame = m_transitionFrame;
	clone->m_nextSceneName = m_nextSceneName;
	return clone;
}
