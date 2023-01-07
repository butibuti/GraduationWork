#include "stdafx_u.h"
#include "Result_BackHuman.h"
#include "ResultManager.h"
#include "Header/GameObjects/DefaultGameComponent/ModelDrawComponent.h"

void ButiEngine::Result_BackHuman::OnUpdate()
{
	m_vlp_bodyAnimationController->Update(m_bodyAnimSpeed);

	if (m_isStartTurnAnimation)
	{
		return;
	}

	m_vlp_mouthAnimationController->Update();

	if (m_vlp_eyeAnimTimer->IsOn())
	{
		float progress = m_vlp_eyeAnimTimer->GetPercent();
		float scaleY = MathHelper::Lerp(m_startEyeScale, m_targetEyeScale, progress);

		Vector3 scale = m_vlp_rightEyeTransform->GetLocalScale();
		scale.y = scaleY;

		m_vlp_rightEyeTransform->SetLocalScale(scale);
		m_vlp_leftEyeTransform->SetLocalScale(scale);

		if (m_vlp_eyeAnimTimer->Update())
		{
			m_vlp_eyeAnimTimer->Stop();

			scale.y = m_targetEyeScale;

			m_vlp_rightEyeTransform->SetLocalScale(scale);
			m_vlp_leftEyeTransform->SetLocalScale(scale);
		}
	}
}

void ButiEngine::Result_BackHuman::OnSet()
{
}

void ButiEngine::Result_BackHuman::OnRemove()
{
}

void ButiEngine::Result_BackHuman::OnShowUI()
{
}

void ButiEngine::Result_BackHuman::Start()
{
	m_vlp_bodyAnimationController = ButiRendering::CreateAnimationController(gameObject.lock()->GetGameComponent<ModelDrawComponent>()->GetBone());
	m_vlp_bodyAnimationController->ChangeAnimation(0.0f, gameObject.lock()->GetResourceContainer()->
		GetModel(gameObject.lock()->GetGameComponent<ModelDrawComponent>()->GetModelTag()).lock()->GetMotion()[3]->GetAnimation());

	auto head = gameObject.lock()->GetGameComponent<ModelDrawComponent>()->GetBone()->searchBoneByName("head");

	auto rightEye = GetManager().lock()->GetGameObject("BackHuman_RightEye");
	rightEye.lock()->transform->SetBaseTransform(head->transform);

	auto leftEye = GetManager().lock()->GetGameObject("BackHuman_LeftEye");
	leftEye.lock()->transform->SetBaseTransform(head->transform);

	auto nose = GetManager().lock()->GetGameObject("BackHuman_Nose");
	nose.lock()->transform->SetBaseTransform(head->transform);

	auto mouth = GetManager().lock()->GetGameObject("BackHuman_Mouth");
	mouth.lock()->transform->SetBaseTransform(head->transform);
	m_vlp_mouthAnimationController = ButiRendering::CreateAnimationController(mouth.lock()->GetGameComponent<ModelDrawComponent>()->GetBone());

	m_bodyAnimSpeed = 1.0f;

	std::int32_t successBorder = GetManager().lock()->GetGameObject("ResultManager").lock()->GetGameComponent<ResultManager>()->GetSuccessBorder();
	auto mouthAnim = gameObject.lock()->GetResourceContainer()->GetModel(mouth.lock()->GetGameComponent<ModelDrawComponent>()->GetModelTag()).lock()->GetMotion()[0]->GetAnimation();
	m_mouthAnimSpeed = mouthAnim->GetEnd() * 0.9f / successBorder;

	m_mouthPoseFrame = 0;
	m_isStartTurnAnimation = false;

	m_vlp_rightEyeTransform = rightEye.lock()->GetGameComponent<ModelDrawComponent>(1)->GetTransform();
	m_vlp_leftEyeTransform = leftEye.lock()->GetGameComponent<ModelDrawComponent>(1)->GetTransform();

	m_eyeAnimSpeed = -m_vlp_rightEyeTransform->GetLocalScale().y * 0.5f / successBorder;

	m_vlp_eyeAnimTimer = ObjectFactory::Create<RelativeTimer>(20);
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::Result_BackHuman::Clone()
{
	return ObjectFactory::Create<Result_BackHuman>();
}

void ButiEngine::Result_BackHuman::AdvanceFacePartAnimation()
{
	if (m_isStartTurnAnimation)
	{
		return;
	}
	m_mouthPoseFrame += m_mouthAnimSpeed;
	
	auto mouth = GetManager().lock()->GetGameObject("BackHuman_Mouth");
	m_vlp_mouthAnimationController->ChangeAnimation(20.0f, gameObject.lock()->GetResourceContainer()->
		GetModel(mouth.lock()->GetGameComponent<ModelDrawComponent>()->GetModelTag()).lock()->GetMotion()[0]->GetAnimation(), m_mouthPoseFrame);

	m_startEyeScale = m_vlp_rightEyeTransform->GetLocalScale().y;
	m_targetEyeScale = m_startEyeScale + m_eyeAnimSpeed;

	m_vlp_eyeAnimTimer->Reset();
	m_vlp_eyeAnimTimer->Start();
}

void ButiEngine::Result_BackHuman::StartTurnAnimation()
{
	m_vlp_bodyAnimationController->ChangeAnimation(0.0f, gameObject.lock()->GetResourceContainer()->
		GetModel(gameObject.lock()->GetGameComponent<ModelDrawComponent>()->GetModelTag()).lock()->GetMotion()[0]->GetAnimation());

	m_isStartTurnAnimation = true;
}

void ButiEngine::Result_BackHuman::StartTurnSuccessAnimation()
{
	m_bodyAnimSpeed = 3.0f;
	m_vlp_bodyAnimationController->ChangeAnimation(0.0f, gameObject.lock()->GetResourceContainer()->
		GetModel(gameObject.lock()->GetGameComponent<ModelDrawComponent>()->GetModelTag()).lock()->GetMotion()[1]->GetAnimation());

	auto mouth = GetManager().lock()->GetGameObject("BackHuman_Mouth");
	m_vlp_mouthAnimationController->ChangeAnimation(0.0f, gameObject.lock()->GetResourceContainer()->
		GetModel(mouth.lock()->GetGameComponent<ModelDrawComponent>()->GetModelTag()).lock()->GetMotion()[0]->GetAnimation());
	m_vlp_mouthAnimationController->SetProgress(1.0f);

	Vector3 scale = m_vlp_rightEyeTransform->GetLocalScale();
	scale.y = 0.0f;

	m_vlp_rightEyeTransform->SetLocalScale(scale);
	m_vlp_leftEyeTransform->SetLocalScale(scale);
}

void ButiEngine::Result_BackHuman::StartTurnFailedAnimation()
{
	auto currentAnim = m_vlp_bodyAnimationController->GetCurrentModelAnimation();
	if (currentAnim->GetResource()->GetName() == "02_Turn")
	{
		m_bodyAnimSpeed = 3.0f;
		m_vlp_bodyAnimationController->ChangeAnimation(0.0f, gameObject.lock()->GetResourceContainer()->
			GetModel(gameObject.lock()->GetGameComponent<ModelDrawComponent>()->GetModelTag()).lock()->GetMotion()[2]->GetAnimation());

		ResetPartAnimation();
	}
}

void ButiEngine::Result_BackHuman::ResetPartAnimation()
{
	auto mouth = GetManager().lock()->GetGameObject("BackHuman_Mouth");
	m_vlp_mouthAnimationController->ChangeAnimation(0.0f, gameObject.lock()->GetResourceContainer()->
		GetModel(mouth.lock()->GetGameComponent<ModelDrawComponent>()->GetModelTag()).lock()->GetMotion()[0]->GetAnimation());
	m_vlp_mouthAnimationController->SetProgress(0.0f);

	Vector3 scale = m_vlp_rightEyeTransform->GetLocalScale();
	scale.y = 1.0f;

	m_vlp_rightEyeTransform->SetLocalScale(scale);
	m_vlp_leftEyeTransform->SetLocalScale(scale);

}
