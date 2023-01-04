#include "stdafx_u.h"
#include "Result_BackHuman.h"
#include "ResultManager.h"
#include "Header/GameObjects/DefaultGameComponent/ModelDrawComponent.h"

void ButiEngine::Result_BackHuman::OnUpdate()
{
	m_vlp_bodyAnimationController->Update();
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
	m_vlp_rightEyeAnimationController = ButiRendering::CreateAnimationController(rightEye.lock()->GetGameComponent<ModelDrawComponent>()->GetBone());
	m_vlp_rightEyeAnimationController->ChangeAnimation(0.0f, gameObject.lock()->GetResourceContainer()->
		GetModel(rightEye.lock()->GetGameComponent<ModelDrawComponent>()->GetModelTag()).lock()->GetMotion()[0]->GetAnimation());

	auto leftEye = GetManager().lock()->GetGameObject("BackHuman_LeftEye");
	leftEye.lock()->transform->SetBaseTransform(head->transform);
	m_vlp_leftEyeAnimationController = ButiRendering::CreateAnimationController(leftEye.lock()->GetGameComponent<ModelDrawComponent>()->GetBone());
	m_vlp_leftEyeAnimationController->ChangeAnimation(0.0f, gameObject.lock()->GetResourceContainer()->
		GetModel(leftEye.lock()->GetGameComponent<ModelDrawComponent>()->GetModelTag()).lock()->GetMotion()[0]->GetAnimation());

	auto nose = GetManager().lock()->GetGameObject("BackHuman_Nose");
	nose.lock()->transform->SetBaseTransform(head->transform);

	auto mouth = GetManager().lock()->GetGameObject("BackHuman_Mouth");
	mouth.lock()->transform->SetBaseTransform(head->transform);
	m_vlp_mouthAnimationController = ButiRendering::CreateAnimationController(mouth.lock()->GetGameComponent<ModelDrawComponent>()->GetBone());
	m_vlp_mouthAnimationController->ChangeAnimation(0.0f, gameObject.lock()->GetResourceContainer()->
		GetModel(mouth.lock()->GetGameComponent<ModelDrawComponent>()->GetModelTag()).lock()->GetMotion()[0]->GetAnimation());

	std::int32_t successBorder = GetManager().lock()->GetGameObject("ResultManager").lock()->GetGameComponent<ResultManager>()->GetSuccessBorder();
	float animFrame = 10.0f;
	m_partAnimSpeed = animFrame / successBorder;
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::Result_BackHuman::Clone()
{
	return ObjectFactory::Create<Result_BackHuman>();
}

void ButiEngine::Result_BackHuman::AdvanceFacePartAnimation()
{
	m_vlp_rightEyeAnimationController->Update(m_partAnimSpeed);
	m_vlp_leftEyeAnimationController->Update(m_partAnimSpeed);
	m_vlp_mouthAnimationController->Update(m_partAnimSpeed);
}

void ButiEngine::Result_BackHuman::StartTurnAnimation()
{
	m_vlp_bodyAnimationController->ChangeAnimation(0.0f, gameObject.lock()->GetResourceContainer()->
		GetModel(gameObject.lock()->GetGameComponent<ModelDrawComponent>()->GetModelTag()).lock()->GetMotion()[0]->GetAnimation());
}

void ButiEngine::Result_BackHuman::StartTurnSuccessAnimation()
{
	m_vlp_bodyAnimationController->ChangeAnimation(0.0f, gameObject.lock()->GetResourceContainer()->
		GetModel(gameObject.lock()->GetGameComponent<ModelDrawComponent>()->GetModelTag()).lock()->GetMotion()[1]->GetAnimation());
}

void ButiEngine::Result_BackHuman::StartTurnFailedAnimation()
{
	auto currentAnim = m_vlp_bodyAnimationController->GetCurrentModelAnimation();
	if (currentAnim->GetResource()->GetName() == "02_Turn")
	{
		m_vlp_bodyAnimationController->ChangeAnimation(10.0f, gameObject.lock()->GetResourceContainer()->
			GetModel(gameObject.lock()->GetGameComponent<ModelDrawComponent>()->GetModelTag()).lock()->GetMotion()[2]->GetAnimation());
	}
}

void ButiEngine::Result_BackHuman::ResetPartAnimation()
{
	m_vlp_rightEyeAnimationController->SetProgress(0.0f);
	m_vlp_leftEyeAnimationController->SetProgress(0.0f);
	m_vlp_mouthAnimationController->SetProgress(0.0f);
}
