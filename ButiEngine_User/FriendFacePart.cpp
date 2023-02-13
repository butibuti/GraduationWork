#include "stdafx_u.h"
#include "FriendFacePart.h"
#include "Header/GameObjects/DefaultGameComponent/TriggerComponent.h"
#include "ButiBulletWrap/ButiBulletWrap/PhysicsManager.h"
#include "ButiBulletWrap/ButiBulletWrap/PhysicsWorld.h"
#include "FriendHead.h"
#include "PauseManager.h"
#include "StageManager.h"
#include "PartStickAnimation.h"
#include "FriendHead_PartHitArea.h"
#include "GameLevelManager.h"
#include "SwayAnimation.h"
#include "Header/GameObjects/DefaultGameComponent/ScaleAnimationComponent.h"
#include"PartRespawnPoint.h"
#include "UI_PartEvaluation.h"

std::int32_t ButiEngine::FriendFacePart::g_eyeCount = 0;
std::int32_t ButiEngine::FriendFacePart::g_noseCount = 0;
std::int32_t ButiEngine::FriendFacePart::g_mouthCount = 0;
std::int32_t ButiEngine::FriendFacePart::g_dummyCount = 0;

std::int32_t ButiEngine::FriendFacePart::g_normalScoreBorder = 60;
std::int32_t ButiEngine::FriendFacePart::g_goodScoreBorder = 90;

ButiEngine::Vector3 ButiEngine::FriendFacePart::g_goodAngleBorder;

void ButiEngine::FriendFacePart::OnUpdate()
{
	if (!CanUpdate())
	{
		return;
	}

	if (m_isAppear)
	{
		OnAppear();
		return;
	}

	if (m_isDisappear)
	{
		OnDisappear();
		return;
	}

	if (m_vlp_deadTimer->Update())
	{
		m_vlp_deadTimer->Stop();
		Disappear();
	}

	if (m_vlp_lifeTimer->Update())
	{
		m_vlp_lifeTimer->Stop();
		Disappear();
	}

	if (m_vlp_leaveIntervalTimer->Update())
	{
		m_vlp_leaveIntervalTimer->Stop();
		m_vlp_deadTimer->Start();
		m_vlp_lifeTimer->Start();
		gameObject.lock()->GetGameComponent<TriggerComponent>()->Regist();
	}

	if (m_isMove)
	{
		Move();
		Rotate();
	}
}

void ButiEngine::FriendFacePart::OnSet()
{
	gameObject.lock()->AddCollisionEnterReaction(
		[this](ButiBullet::ContactData& arg_other)
		{
			if (arg_other.vwp_gameObject.lock())
			{
				//ƒ^ƒO”»’è
				if (arg_other.vwp_gameObject.lock()->HasGameObjectTag("PartHitArea"))
				{
					OnCollisionPartHitArea(arg_other.vwp_gameObject);
				}
			}
		}
	);

	gameObject.lock()->AddCollisionStayReaction(
		[this](ButiBullet::ContactData& arg_other)
		{
			if (arg_other.vwp_gameObject.lock())
			{
				if (arg_other.vwp_gameObject.lock()->HasGameObjectTag("AliveArea"))
				{
					m_vlp_deadTimer->Reset();
				}
			}
		}
	);
}

void ButiEngine::FriendFacePart::OnRemove()
{
}

void ButiEngine::FriendFacePart::OnShowUI()
{
	m_param.GUI_SetPartType();
	m_param.GUI_SetLife();
	m_param.GUI_SetMoveParam();
	m_param.GUI_SetGravityParam();
	m_param.GUI_SetRotationParam();


	GUI::BulletText("GoodAngleBorder");
	GUI::DragFloat3("##goodAngleBorder", &g_goodAngleBorder.x, 0.1f, 0.0f, 180.0f);
}

void ButiEngine::FriendFacePart::Start()
{
	auto tutorialManager = GetManager().lock()->GetGameObject("TutorialManager");
	if (tutorialManager.lock())
	{
		m_isTutorial = true;
	}

	if (!m_isTutorial)
	{
		m_vwp_stageManager = GetManager().lock()->GetGameObject("StageManager").lock()->GetGameComponent<StageManager>();
	}

	m_vwp_pauseManager = GetManager().lock()->GetGameObject("PauseManager").lock()->GetGameComponent<PauseManager>();
	m_vwp_gameLevelManager = GetManager().lock()->GetGameObject("GameLevelManager").lock()->GetGameComponent<GameLevelManager>();

	m_isAppear = false;
	m_isDisappear = false;

	m_startZ = gameObject.lock()->transform->GetLocalPosition().z;

	m_vlp_deadTimer = ObjectFactory::Create<RelativeTimer>(10);

	m_vlp_lifeTimer = ObjectFactory::Create<RelativeTimer>(m_param.life);

	std::int32_t gameLevel = m_vwp_gameLevelManager.lock()->GetGameLevel();

	m_vlp_leaveIntervalTimer = ObjectFactory::Create<RelativeTimer>(30);

	m_rank = Rank::NoRank;

	m_isMove = true;

	if (!m_param.isGravity)
	{
		m_param.velocity.Normalize();
		m_param.velocity *= m_param.moveSpeed;
	}

	AddPartCount();

	Appear();
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::FriendFacePart::Clone()
{
	auto clone = ObjectFactory::Create<FriendFacePart>();
	clone->m_param = m_param;
	clone->g_goodAngleBorder = g_goodAngleBorder;
	return clone;
}

void ButiEngine::FriendFacePart::Dead()
{
	gameObject.lock()->SetIsRemove(true);
	//gameObject.lock()->GetGameComponent<SeparateDrawObject>()->GetDrawObject().lock()->SetIsRemove(true);
	if (m_vwp_respawnPoint.lock()) {
		m_vwp_respawnPoint.lock()->Failed();
	}
	RemovePartCount();
}

void ButiEngine::FriendFacePart::SetRandomParam_Straight()
{
	Vector3 pos = gameObject.lock()->transform->GetLocalPosition();
	Vector3 centerPos = Vector3Const::Zero;
	centerPos.z = pos.z;

	Vector3 diff = centerPos - pos;
	m_param.velocity = diff * Matrix4x4::RollZ(MathHelper::ToRadian(ButiRandom::GetRandom(-20.0f, 20.0f, 10)));
	m_param.moveSpeed = ButiRandom::GetRandom(0.05f, 0.075f, 100);
}

void ButiEngine::FriendFacePart::LeaveHead()
{
	AddPartCount();

	gameObject.lock()->transform->SetBaseTransform(nullptr);
	
	float rotation = gameObject.lock()->transform->GetLocalRotation_Euler().z;
	gameObject.lock()->transform->RollIdentity();
	gameObject.lock()->transform->SetLocalRotationZ_Degrees(rotation);
	gameObject.lock()->transform->SetLocalPositionZ(m_startZ);

	Blow();

	m_isMove = true;

	m_vlp_leaveIntervalTimer->Start();
	//m_vlp_deadTimer->Start();

	if (m_vwp_respawnPoint.lock()) {
		m_vwp_respawnPoint.lock()->Damage();
	}
}

void ButiEngine::FriendFacePart::RemoveStickAnimation()
{
	auto partStickAnim = gameObject.lock()->GetGameComponent<PartStickAnimation>();
	if (partStickAnim)
	{
		partStickAnim->SetIsRemove(true);
	}
}

std::int32_t ButiEngine::FriendFacePart::GetCalcScore()
{
	return GetCalcAngleScore() + GetCalcPosScore();
}

std::int32_t ButiEngine::FriendFacePart::GetCalcAngleScore()
{
	std::int32_t angleScore = 0;

	float partAngle = abs(gameObject.lock()->transform->GetLocalRotation_Euler().z);
	std::int32_t exactScore = 45;
	if (IsGoodAngle())
	{
		float progress = 1.0f - (partAngle / g_goodAngleBorder.z);
		progress = MathHelper::Clamp(progress, 0.0f, 1.0f);

		std::int32_t maxScore = 50 - exactScore;
		angleScore = exactScore + MathHelper::Lerp(0, maxScore, progress);
	}
	else
	{
		float progress = 1.0f - (partAngle / 180.0f);
		progress = MathHelper::Clamp(progress, 0.0f, 1.0f);

		std::int32_t maxScore = exactScore - 1;
		angleScore = MathHelper::Lerp(0, maxScore, progress);
	}

	return angleScore;
}

std::int32_t ButiEngine::FriendFacePart::GetCalcPosScore()
{
	std::int32_t posScore = 0;

	float partAngleX = abs(gameObject.lock()->transform->GetLocalRotation_Euler().x);
	float partAngleY = abs(gameObject.lock()->transform->GetLocalRotation_Euler().y);
	std::int32_t exactScoreX = 18;
	std::int32_t exactScoreY = 27;
	if (IsGoodPos())
	{
		float progressX = 1.0f - (partAngleX / g_goodAngleBorder.x);
		progressX = MathHelper::Clamp(progressX, 0.0f, 1.0f);

		std::int32_t maxScoreX = 20 - exactScoreX;
		posScore += exactScoreX + MathHelper::Lerp(0, maxScoreX, progressX);

		float progressY = 1.0f - (partAngleY / g_goodAngleBorder.x);
		progressY = MathHelper::Clamp(progressY, 0.0f, 1.0f);

		std::int32_t maxScoreY = 30 - exactScoreY;
		posScore += exactScoreY + MathHelper::Lerp(0, maxScoreY, progressY);
	}
	else
	{
		float progressX = 1.0f - (partAngleX / 180.0f);
		progressX = MathHelper::Clamp(progressX, 0.0f, 1.0f);

		std::int32_t maxScoreX = exactScoreX - 1;
		posScore += MathHelper::Lerp(0, maxScoreX, progressX);

		float progressY = 1.0f - (partAngleY / 180.0f);
		progressY = MathHelper::Clamp(progressY, 0.0f, 1.0f);

		std::int32_t maxScoreY = exactScoreY - 1;
		posScore += MathHelper::Lerp(0, maxScoreY, progressY);
	}

	return posScore;
}

ButiEngine::Rank ButiEngine::FriendFacePart::GetCalcPartRank()
{
	Rank rank;
	std::int32_t score = GetCalcScore();

	std::int32_t normalBorder = 60;
	std::int32_t goodBorder = 90;
	if (score >= goodBorder)
	{
		rank = Rank::Good;
	}
	else if (score >= normalBorder)
	{
		rank = Rank::Normal;
	}
	else
	{
		rank = Rank::Bad;
	}

	return rank;
}

bool ButiEngine::FriendFacePart::IsGoodAngle()
{
	float partAngle = abs(gameObject.lock()->transform->GetLocalRotation_Euler().z);

	return partAngle <= g_goodAngleBorder.z;
}

bool ButiEngine::FriendFacePart::IsGoodPos()
{
	float partAngleX = abs(gameObject.lock()->transform->GetLocalRotation_Euler().x);
	float partAngleY = abs(gameObject.lock()->transform->GetLocalRotation_Euler().y);

	return partAngleX <= g_goodAngleBorder.x && partAngleY <= g_goodAngleBorder.y;
}

void ButiEngine::FriendFacePart::Move()
{
	if (m_param.isGravity)
	{
		m_param.velocity += m_param.gravityDirection * m_param.gravity * GameDevice::GetWorldSpeed();

		if (m_param.velocity.GetLengthSqr() > m_param.maxMoveSpeed * m_param.maxMoveSpeed)
		{
			m_param.velocity.Normalize();
			m_param.velocity *= m_param.maxMoveSpeed;
		}
	}

	gameObject.lock()->transform->Translate(m_param.velocity * GameDevice::GetWorldSpeed());
}

void ButiEngine::FriendFacePart::Rotate()
{
	if (m_param.isSway)
	{
		return;
	}

	gameObject.lock()->transform->RollLocalRotationZ_Degrees(m_param.rotateSpeed * GameDevice::GetWorldSpeed());
}

void ButiEngine::FriendFacePart::StickHead()
{
	RemovePartCount();

	gameObject.lock()->GetGameComponent<TriggerComponent>()->UnRegist();

	m_vlp_deadTimer->Reset();
	m_vlp_deadTimer->Stop();
	m_vlp_lifeTimer->Reset();
	m_vlp_lifeTimer->Stop();

	auto swayComponent = gameObject.lock()->GetGameComponent<SwayAnimation>();
	if (swayComponent)
	{
		swayComponent->SetIsRemove(true);
	}

	gameObject.lock()->transform->SetLocalPosition(GetStickPos());
	gameObject.lock()->transform->SetBaseTransform(m_vwp_head.lock()->transform);

	if (IsBetterRank())
	{
		m_vwp_partHitArea.lock()->GetGameComponent<FriendHead_PartHitArea>()->OverWrite(gameObject);
	}
	else
	{
		m_vwp_partHitArea.lock()->GetGameComponent<FriendHead_PartHitArea>()->StickPart(gameObject);
	}

	SpawnStickEffect();

	m_isMove = false;
	if (m_vwp_respawnPoint.lock()) {
		m_vwp_respawnPoint.lock()->Success();
	}
}

void ButiEngine::FriendFacePart::SpawnStickEffect()
{
	//auto drawObject = gameObject.lock()->GetGameComponent<SeparateDrawObject>()->GetDrawObject();
	gameObject.lock()->AddGameComponent<PartStickAnimation>();

	auto partHitFlash = GetManager().lock()->AddObjectFromCereal("Effect_PartHitFlash");
	partHitFlash.lock()->transform->SetLocalPosition(gameObject.lock()->transform->GetWorldPosition());
	partHitFlash.lock()->transform->SetBaseTransform(gameObject.lock()->transform);

	auto partHitBeam = GetManager().lock()->AddObjectFromCereal("Effect_PartHitBeam");
	partHitBeam.lock()->transform->SetLocalPosition(gameObject.lock()->transform->GetWorldPosition());
	partHitBeam.lock()->transform->SetLocalRotation(gameObject.lock()->transform->GetLocalRotation());
	partHitBeam.lock()->transform->SetBaseTransform(gameObject.lock()->transform);

	auto sound = gameObject.lock()->GetResourceContainer()->GetSound(SoundTag("Sound/PartHit.wav"));
	GetManager().lock()->GetApplication().lock()->GetSoundManager()->PlaySE(sound, 0.5f);
}

void ButiEngine::FriendFacePart::SpawnDummyPartHitEffect()
{
	auto partHitFlash = GetManager().lock()->AddObjectFromCereal("Effect_DummyPartHitFlash");
	partHitFlash.lock()->transform->SetLocalPosition(gameObject.lock()->transform->GetWorldPosition());

	auto sound = gameObject.lock()->GetResourceContainer()->GetSound(SoundTag("Sound/Damage.wav"));
	GetManager().lock()->GetApplication().lock()->GetSoundManager()->PlaySE(sound, 0.5f);
}

void ButiEngine::FriendFacePart::OnCollisionPartHitArea(Value_weak_ptr<GameObject> arg_vwp_partHitArea)
{
	m_vwp_partHitArea = arg_vwp_partHitArea;
	auto partHitAreaComponent = arg_vwp_partHitArea.lock()->GetGameComponent<FriendHead_PartHitArea>();
	m_vwp_head = partHitAreaComponent->GetParent();

	if (partHitAreaComponent->CanStickPart(m_param.type) || IsBetterRank())
	{
		if (m_param.type == PartType::Dummy)
		{
			auto headComponent = m_vwp_head.lock()->GetGameComponent<FriendHead>();
			if (headComponent->IsExistPartStuckArea())
			{
				SpawnDummyPartHitEffect();
				headComponent->LeavePartRandom();
				Dead();
			}
		}
		else
		{
			StickHead();
			CheckRank();
		}
	}
}

bool ButiEngine::FriendFacePart::CanUpdate()
{
	//if (m_vwp_stageManager.lock() && !m_vwp_stageManager.lock()->IsGameStart())
	//{
	//	return false;
	//}

	if (m_vwp_pauseManager.lock()->IsPause())
	{
		return false;
	}

	return true;
}

ButiEngine::Vector3 ButiEngine::FriendFacePart::GetStickPos()
{
	Vector3 stickPos = m_vwp_partHitArea.lock()->transform->GetWorldPosition();
	stickPos.z = m_vwp_partHitArea.lock()->GetGameComponent<FriendHead_PartHitArea>()->GetStickPos().z;

	Vector3 rayStartPos = m_vwp_partHitArea.lock()->transform->GetWorldPosition();
	rayStartPos.z += 50.0f;

	List<ButiBullet::PhysicsRaycastResult> list_rayRes;
	if (gameObject.lock()->GetGameObjectManager().lock()->GetScene().lock()->GetPhysicsManager()->GetActivePhysicsWorld()->
		RaycastAllHit(rayStartPos, -Vector3Const::ZAxis, 100.0f, 1, &list_rayRes))
	{
		float nearestDistance = 1000.0f;
		float stickPosZ = stickPos.z;

		for (auto& res : list_rayRes)
		{
			auto obj= Value_weak_ptr<GameObject>();
			obj = res.physicsObject->GetOwnerData();
			

			if (obj.lock()->HasGameObjectTag("FriendHead"))
			{
				float distance = abs(res.point.z - rayStartPos.z);
				if (distance <= nearestDistance)
				{
					nearestDistance = distance;
					stickPosZ = res.point.z;
				}
			}
		}
		stickPos.z = stickPosZ;
	}

	return stickPos;
}

void ButiEngine::FriendFacePart::Blow()
{
	m_param.isGravity = true;
	m_param.gravity = 0.0003f;
	m_param.isSway = false;
	m_param.velocity.y = 0.03f;

	Vector3 pos = gameObject.lock()->transform->GetWorldPosition();
	float force = 0.01f;
	float direction = 1.0f;
	if (pos.x > 2.0f)
	{
		direction = -1.0f;
	}
	else if (pos.x < -2.0f)
	{
		direction = 1.0f;
	}
	else
	{
		std::int32_t rand = ButiRandom::GetInt(0, 1);
		direction = rand ? 1.0f : -1.0f;
	}
	m_param.velocity.x = force * direction;
	m_param.rotateSpeed = 10.0f * direction;
}

void ButiEngine::FriendFacePart::CheckRank()
{
	std::int32_t gameLevel = m_vwp_gameLevelManager.lock()->GetGameLevel();
	if (gameLevel == 0)
	{
		m_rank = Rank::Normal;
		return;
	}

	if (m_rank == Rank::Good)
	{
		return;
	}

	Rank newRank = GetCalcPartRank();
	if (static_cast<std::int32_t>(newRank) <= static_cast<std::int32_t>(m_rank))
	{
		return;
	}
	m_rank = newRank;

	CreateEvaluationObject();

	ChangeModel();
}

void ButiEngine::FriendFacePart::ChangeModel()
{
	if (m_param.type == PartType::Eye)
	{
		if (m_rank == Rank::Good)
		{
			gameObject.lock()->GetGameComponent<MeshDrawComponent>(1)->GetTransform()->SetLocalScale(0.0f);
			gameObject.lock()->GetGameComponent<MeshDrawComponent>(2)->GetTransform()->SetLocalScale(0.0f);
		}
		else if (m_rank == Rank::Normal)
		{
			gameObject.lock()->GetGameComponent<MeshDrawComponent>(1)->GetTransform()->SetLocalScale(1.0f);
			gameObject.lock()->GetGameComponent<MeshDrawComponent>(2)->GetTransform()->SetLocalScale(0.0f);
		}
		else if (m_rank == Rank::Bad)
		{
			gameObject.lock()->GetGameComponent<MeshDrawComponent>(1)->GetTransform()->SetLocalScale(0.0f);
			gameObject.lock()->GetGameComponent<MeshDrawComponent>(2)->GetTransform()->SetLocalScale(1.0f);
		}
	}
	else if (m_param.type == PartType::Nose || m_param.type == PartType::Mouth)
	{
		if (m_rank == Rank::Good)
		{
			gameObject.lock()->GetGameComponent<MeshDrawComponent>(0)->GetTransform()->SetLocalScale(0.0f);
			gameObject.lock()->GetGameComponent<MeshDrawComponent>(1)->GetTransform()->SetLocalScale(1.0f);
			gameObject.lock()->GetGameComponent<MeshDrawComponent>(2)->GetTransform()->SetLocalScale(0.0f);
		}
		else if (m_rank == Rank::Normal)
		{
			gameObject.lock()->GetGameComponent<MeshDrawComponent>(0)->GetTransform()->SetLocalScale(1.0f);
			gameObject.lock()->GetGameComponent<MeshDrawComponent>(1)->GetTransform()->SetLocalScale(0.0f);
			gameObject.lock()->GetGameComponent<MeshDrawComponent>(2)->GetTransform()->SetLocalScale(0.0f);
		}
		else if(m_rank == Rank::Bad)
		{
			gameObject.lock()->GetGameComponent<MeshDrawComponent>(0)->GetTransform()->SetLocalScale(0.0f);
			gameObject.lock()->GetGameComponent<MeshDrawComponent>(1)->GetTransform()->SetLocalScale(0.0f);
			gameObject.lock()->GetGameComponent<MeshDrawComponent>(2)->GetTransform()->SetLocalScale(1.0f);
		}
	}
}

void ButiEngine::FriendFacePart::Appear()
{
	m_isAppear = true;
	gameObject.lock()->GetGameComponent<TriggerComponent>()->UnRegist();
	AddScaleAnimation(1.0f, Easing::EasingType::EaseOutBack);
}

void ButiEngine::FriendFacePart::Disappear()
{
	m_isDisappear = true;
	gameObject.lock()->GetGameComponent<TriggerComponent>()->UnRegist();
	AddScaleAnimation(0.0f, Easing::EasingType::EaseInBack);
}

void ButiEngine::FriendFacePart::OnAppear()
{
	auto anim = gameObject.lock()->GetGameComponent<ScaleAnimation>();
	if (!anim)
	{
		m_isAppear = false;
		gameObject.lock()->GetGameComponent<TriggerComponent>()->Regist();
		m_vlp_deadTimer->Start();
		m_vlp_lifeTimer->Start();
		if (m_param.isSway)
		{
			gameObject.lock()->AddGameComponent<SwayAnimation>();
		}
	}
}

void ButiEngine::FriendFacePart::OnDisappear()
{
	auto anim = gameObject.lock()->GetGameComponent<ScaleAnimation>();
	if (!anim)
	{
		m_isDisappear = false;
		Dead();
	}
}

void ButiEngine::FriendFacePart::AddScaleAnimation(const Vector3& arg_targetScale, Easing::EasingType arg_easeType)
{
	auto anim = gameObject.lock()->AddGameComponent<ScaleAnimation>();
	anim->SetInitScale(gameObject.lock()->transform->GetLocalScale());
	anim->SetTargetScale(arg_targetScale);
	anim->SetSpeed(1.0f / 30);
	anim->SetEaseType(arg_easeType);
}

void ButiEngine::FriendFacePart::AddPartCount()
{
	switch (m_param.type)
	{
	case ButiEngine::PartType::Eye:
		g_eyeCount++;
		break;
	case ButiEngine::PartType::Nose:
		g_noseCount++;
		break;
	case ButiEngine::PartType::Mouth:
		g_mouthCount++;
		break;
	case ButiEngine::PartType::Dummy:
		g_dummyCount++;
		break;
	default:
		break;
	}
}

void ButiEngine::FriendFacePart::RemovePartCount()
{
	switch (m_param.type)
	{
	case ButiEngine::PartType::Eye:
		g_eyeCount--;
		break;
	case ButiEngine::PartType::Nose:
		g_noseCount--;
		break;
	case ButiEngine::PartType::Mouth:
		g_mouthCount--;
		break;
	case ButiEngine::PartType::Dummy:
		g_dummyCount--;
		break;
	default:
		break;
	}
}

bool ButiEngine::FriendFacePart::IsBetterRank()
{
	auto havePart = m_vwp_partHitArea.lock()->GetGameComponent<FriendHead_PartHitArea>()->GetPart();
	if (!havePart.lock())
	{
		return true;
	}

	Rank havePartRank = havePart.lock()->GetGameComponent<FriendFacePart>()->GetPartRank();

	return static_cast<std::int32_t>(m_rank) >= static_cast<std::int32_t>(havePartRank);
}

void ButiEngine::FriendFacePart::CreateEvaluationObject()
{
	auto evaluation = GetManager().lock()->AddObjectFromCereal("UI_PartEvaluation");
	Vector3 pos = gameObject.lock()->transform->GetWorldPosition();
	Vector3 screenPos = GetCamera("main")->WorldToScreen(pos);
	screenPos.x += 300.0f;
	if (pos.x < 0.0f)
	{
		screenPos.x -= 600.0f;
	}
	evaluation.lock()->transform->SetLocalPosition(screenPos);
	evaluation.lock()->GetGameComponent<UI_PartEvaluation>()->SetScore(GetCalcScore());
	evaluation.lock()->GetGameComponent<UI_PartEvaluation>()->SetRank(m_rank);
}


bool ButiEngine::FacePartParameter::GUI_SetPartType()
{
	bool ret = false;
	std::string typeStr;
	switch (type)
	{
	case ButiEngine::PartType::Eye:
		typeStr = "Eye";
		break;
	case ButiEngine::PartType::Nose:
		typeStr = "Nose";
		break;
	case ButiEngine::PartType::Mouth:
		typeStr = "Mouth";
		break;
	case ButiEngine::PartType::Dummy:
		typeStr = "Dummy";
		break;
	default:
		break;
	}

	GUI::BulletText("PartType:" + typeStr);

	if (GUI::Button("Eye"))
	{
		type = PartType::Eye;
		ret |= true;
	}
	GUI::SameLine();
	if (GUI::Button("Nose"))
	{
		type = PartType::Nose;
		ret |= true;
	}
	GUI::SameLine();
	if (GUI::Button("Mouth"))
	{
		type = PartType::Mouth;
		ret |= true;
	}
	GUI::SameLine();
	if (GUI::Button("Dummy"))
	{
		type = PartType::Dummy;
		ret |= true;
	}
	return ret;
}

bool ButiEngine::FacePartParameter::GUI_SetLife()
{
	bool ret = false;
	GUI::BulletText("Life");
	ret |= GUI::DragInt("##life", life, 1.0f, 0, 1000);
	return ret;
}

bool ButiEngine::FacePartParameter::GUI_SetMoveParam()
{
	bool ret = false;
	if (isGravity)
	{
		GUI::BulletText("Velocity");
		ret |= GUI::DragFloat3("##velocity", &velocity.x, 0.01f, -100.0f, 100.0f, "%.7f");

		GUI::BulletText("MaxMoveSpeed");
		ret |= GUI::DragFloat("##maxMoveSpeed", maxMoveSpeed, 0.01f, 0.0f, 100.0f);
	}
	else
	{
		GUI::BulletText("MoveDirection");
		ret |= GUI::DragFloat3("##velocity", &velocity.x, 0.01f, -100.0f, 100.0f);

		GUI::BulletText("MoveSpeed");
		ret |= GUI::DragFloat("##moveSpeed", moveSpeed, 0.01f, 0.0f, 100.0f);
	}
	return ret;
}

bool ButiEngine::FacePartParameter::GUI_SetGravityParam()
{
	bool ret = false;
	ret |= GUI::Checkbox("isGravity", isGravity);

	if (isGravity)
	{
		GUI::BulletText("Gravity");
		ret |= GUI::DragFloat("##gravity", gravity, 0.01f, 0.0f, 100.0f, "%.7f");

		GUI::BulletText("GravityDirection");
		ret |= GUI::DragFloat3("##gravityDirection", &gravityDirection.x, 0.01f, -1.0f, 1.0f);
	}
	return ret;
}

bool ButiEngine::FacePartParameter::GUI_SetRotationParam()
{
	bool ret = false;
	ret |= GUI::Checkbox("isSway", isSway);

	if (!isSway)
	{
		GUI::BulletText("RotateSpeed");
		ret|= GUI::DragFloat("##rotateSpeed", rotateSpeed, 0.1f, -100.0f, 100.0f);
	}
	return ret;
}
