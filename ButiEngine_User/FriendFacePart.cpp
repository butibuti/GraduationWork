#include "stdafx_u.h"
#include "FriendFacePart.h"
#include "Header/GameObjects/DefaultGameComponent/RigidBodyComponent.h"
#include "Header/GameObjects/DefaultGameComponent/TriggerComponent.h"
#include "FriendHead.h"
#include "ButiBulletWrap/ButiBulletWrap/PhysicsManager.h"
#include "ButiBulletWrap/ButiBulletWrap/PhysicsWorld.h"
#include "PauseManager.h"
#include "StageManager.h"
#include "SeparateDrawObject.h"
#include "PartStickAnimation.h"
#include "FriendHead_PartHitArea.h"
#include "GameLevelManager.h"
#include "SwayAnimation.h"

std::int32_t ButiEngine::FriendFacePart::g_eyeCount = 0;
std::int32_t ButiEngine::FriendFacePart::g_noseCount = 0;
std::int32_t ButiEngine::FriendFacePart::g_mouthCount = 0;
std::int32_t ButiEngine::FriendFacePart::g_dummyCount = 0;

void ButiEngine::FriendFacePart::OnUpdate()
{
	if (!CanUpdate())
	{
		return;
	}

	if (m_vlp_deadTimer->Update())
	{
		m_vlp_deadTimer->Stop();
		Dead();
	}

	if (m_vlp_lifeTimer->Update())
	{
		m_vlp_lifeTimer->Stop();
		Dead();
	}

	if (m_state == FacePartState::Move)
	{
		Move();
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
	GUI_SetPartParam();
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

	m_startZ = gameObject.lock()->transform->GetLocalPosition().z;
	m_isHitHead = false;

	m_vlp_deadTimer = ObjectFactory::Create<RelativeTimer>(10);
	m_vlp_deadTimer->Start();

	m_vlp_lifeTimer = ObjectFactory::Create<RelativeTimer>(m_param.life);

	std::int32_t gameLevel = m_vwp_gameLevelManager.lock()->GetGameLevel();
	if (gameLevel != 0)
	{
		m_vlp_lifeTimer->Start();
	}

	m_state = FacePartState::Move;

	if (!m_param.isGravity)
	{
		m_param.velocity.Normalize();
		m_param.velocity *= m_param.moveSpeed;
	}

	if (m_param.isSway)
	{
		gameObject.lock()->AddGameComponent<SwayAnimation>();
	}

	AddPartCount();
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::FriendFacePart::Clone()
{
	auto clone = ObjectFactory::Create<FriendFacePart>();
	clone->m_param = m_param;
	return clone;
}

void ButiEngine::FriendFacePart::Dead()
{
	gameObject.lock()->SetIsRemove(true);
	//gameObject.lock()->GetGameComponent<SeparateDrawObject>()->GetDrawObject().lock()->SetIsRemove(true);

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
	gameObject.lock()->transform->SetLocalPositionZ(m_startZ);

	gameObject.lock()->GetGameComponent<TriggerComponent>()->Regist();

	m_vlp_deadTimer->Start();
	m_vlp_lifeTimer->Start();

	if (m_param.isSway)
	{
		gameObject.lock()->AddGameComponent<SwayAnimation>();
	}

	Blow();


	m_state = FacePartState::Move;
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

	m_vwp_partHitArea.lock()->GetGameComponent<FriendHead_PartHitArea>()->StickPart(gameObject, m_param.type);

	SpawnStickEffect();

	m_state = FacePartState::Stick;
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
}

void ButiEngine::FriendFacePart::OnCollisionPartHitArea(Value_weak_ptr<GameObject> arg_vwp_partHitArea)
{
	m_vwp_partHitArea = arg_vwp_partHitArea;
	auto partHitAreaComponent = arg_vwp_partHitArea.lock()->GetGameComponent<FriendHead_PartHitArea>();
	m_vwp_head = partHitAreaComponent->GetParent();

	if (partHitAreaComponent->CanStickPart(m_param.type))
	{
		if (m_param.type == PartType::Dummy)
		{
			auto headCompontne = m_vwp_head.lock()->GetGameComponent<FriendHead>();
			if (!m_isHitHead && headCompontne->IsExistPartStuckArea())
			{
				SpawnDummyPartHitEffect();
				headCompontne->LeavePart();
				Dead();
				m_isHitHead = true;
			}
		}
		else
		{
			StickHead();
		}
	}
}

bool ButiEngine::FriendFacePart::CanUpdate()
{
	if (m_vwp_stageManager.lock() && !m_vwp_stageManager.lock()->IsGameStart())
	{
		return false;
	}

	if (m_vwp_pauseManager.lock()->IsPause())
	{
		return false;
	}

	return true;
}

ButiEngine::Vector3 ButiEngine::FriendFacePart::GetStickPos()
{
	Vector3 rayStartPos = m_vwp_partHitArea.lock()->transform->GetWorldPosition();
	if (m_param.type == PartType::Dummy)
	{
		rayStartPos = gameObject.lock()->transform->GetLocalPosition();
	}
	rayStartPos.z += 50.0f;

	Vector3 stickPos = m_vwp_partHitArea.lock()->transform->GetWorldPosition();
	stickPos.z = m_vwp_partHitArea.lock()->GetGameComponent<FriendHead_PartHitArea>()->GetStickPos().z;

	List<ButiBullet::PhysicsRaycastResult> list_rayRes;
	if (gameObject.lock()->GetGameObjectManager().lock()->GetScene().lock()->GetPhysicsManager()->GetActivePhysicsWorld()->
		RaycastAllHit(rayStartPos, -Vector3Const::ZAxis, 100.0f, 1, &list_rayRes))
	{
		float nearestDistance = 1000.0f;
		float chaseTargetPosZ = stickPos.z;

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
					chaseTargetPosZ = res.point.z;
				}
			}
		}
	}

	return stickPos;
}

void ButiEngine::FriendFacePart::Blow()
{
	m_param.isGravity = true;
	m_param.gravity = 0.0003f;
	m_param.velocity.y = 0.03f;

	Vector3 pos = gameObject.lock()->transform->GetWorldPosition();
	float force = 0.02f;
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

void ButiEngine::FriendFacePart::GUI_SetPartParam()
{
	GUI_SetPartType();
	GUI_SetGravityParam();
	GUI_SetMoveParam();
	GUI_SetLife();
	GUI_SetRotationParam();
}

void ButiEngine::FriendFacePart::GUI_SetPartType()
{
	std::string typeStr;
	switch (m_param.type)
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
		m_param.type = PartType::Eye;
	}
	GUI::SameLine();
	if (GUI::Button("Nose"))
	{
		m_param.type = PartType::Nose;
	}
	GUI::SameLine();
	if (GUI::Button("Mouth"))
	{
		m_param.type = PartType::Mouth;
	}
	GUI::SameLine();
	if (GUI::Button("Dummy"))
	{
		m_param.type = PartType::Dummy;
	}
}

void ButiEngine::FriendFacePart::GUI_SetLife()
{
	GUI::BulletText("Life");
	GUI::DragInt("##life", m_param.life, 1.0f, 0, 1000);
}

void ButiEngine::FriendFacePart::GUI_SetMoveParam()
{
	if (m_param.isGravity)
	{
		GUI::BulletText("Velocity");
		GUI::DragFloat3("##velocity", &m_param.velocity.x, 0.01f, -100.0f, 100.0f, "%.7f");

		GUI::BulletText("MaxMoveSpeed");
		GUI::DragFloat("##maxMoveSpeed", m_param.maxMoveSpeed, 0.01f, 0.0f, 100.0f);
	}
	else
	{
		GUI::BulletText("MoveDirection");
		GUI::DragFloat3("##velocity", &m_param.velocity.x, 0.01f, -100.0f, 100.0f);

		GUI::BulletText("MoveSpeed");
		GUI::DragFloat("##moveSpeed", m_param.moveSpeed, 0.01f, 0.0f, 100.0f);
	}
}

void ButiEngine::FriendFacePart::GUI_SetGravityParam()
{
	GUI::Checkbox("isGravity", m_param.isGravity);

	if (m_param.isGravity)
	{
		GUI::BulletText("Gravity");
		GUI::DragFloat("##gravity", m_param.gravity, 0.01f, 0.0f, 100.0f, "%.7f");

		GUI::BulletText("GravityDirection");
		GUI::DragFloat3("##gravityDirection", &m_param.gravityDirection.x, 0.01f, -1.0f, 1.0f);
	}
}

void ButiEngine::FriendFacePart::GUI_SetRotationParam()
{
	GUI::Checkbox("isSway", m_param.isSway);

	if (!m_param.isSway)
	{
		GUI::BulletText("RotateSpeed");
		GUI::DragFloat("##rotateSpeed", m_param.rotateSpeed, 0.1f, -100.0f, 100.0f);
	}
}
