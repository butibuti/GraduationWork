#include "stdafx_u.h"
#include "UI_TutorialText.h"
#include "Header/GameObjects/DefaultGameComponent/ScaleAnimationComponent.h"

void ButiEngine::UI_TutorialText::OnUpdate()
{
	if (m_vlp_waitDisappearTimer->Update())
	{
		m_vlp_waitDisappearTimer->Stop();
		m_vlp_disappearTimer->Start();
	}

	if (m_vlp_disappearTimer->IsOn())
	{

		float progress = m_vlp_disappearTimer->GetPercent();
		Vector3 scale = MathHelper::LerpPosition(m_startScale, Vector3Const::Zero, Easing::GetEase(progress, Easing::EasingType::EaseOutCirc));
		m_vwp_meshDraw.lock()->GetTransform()->SetLocalScale(scale);

		if (m_vlp_disappearTimer->Update())
		{
			m_vlp_disappearTimer->Stop();
			gameObject.lock()->SetIsRemove(true);
		}
	}
}

void ButiEngine::UI_TutorialText::OnSet()
{
}

void ButiEngine::UI_TutorialText::OnRemove()
{
}

void ButiEngine::UI_TutorialText::OnShowUI()
{
}

void ButiEngine::UI_TutorialText::Start()
{
	m_vwp_meshDraw = gameObject.lock()->GetGameComponent<MeshDrawComponent>();
	m_vlp_waitDisappearTimer = ObjectFactory::Create<RelativeTimer>(95);

	m_vlp_disappearTimer = ObjectFactory::Create<RelativeTimer>(10);

	m_startScale = m_vwp_meshDraw.lock()->GetTransform()->GetLocalScale();
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::UI_TutorialText::Clone()
{
	return ObjectFactory::Create<UI_TutorialText>();
}

void ButiEngine::UI_TutorialText::Clear()
{
	GetManager().lock()->AddObjectFromCereal("Text_OK");
	m_vlp_waitDisappearTimer->Start();
}
