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

	m_vlp_deadTimer = ObjectFactory::Create<RelativeTimer>(10);
	m_vlp_deadTimer->Start();

	m_vlp_lifeTimer = ObjectFactory::Create<RelativeTimer>(600);

	std::int32_t gameLevel = m_vwp_gameLevelManager.lock()->GetGameLevel();
	if (gameLevel != 0)
	{
		m_vlp_lifeTimer->Start();
	}

	m_state = FacePartState::Move;

	if (m_param.isRandom)
	{
		SetRandomVelocity();
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
	gameObject.lock()->GetGameComponent<SeparateDrawObject>()->GetDrawObject().lock()->SetIsRemove(true);

	RemovePartCount();
}

void ButiEngine::FriendFacePart::SetParam_Stay()
{
	m_param.isRandom = false;
	m_param.velocity = Vector3Const::Zero;
}

void ButiEngine::FriendFacePart::Move()
{
	m_param.gravity += m_param.gravityAcceleration * GameDevice::GetWorldSpeed();
	m_param.velocity += m_param.gravityDirection * m_param.gravity * GameDevice::GetWorldSpeed();

	gameObject.lock()->transform->Translate(m_param.velocity * GameDevice::GetWorldSpeed());
}

void ButiEngine::FriendFacePart::StickToHead()
{
	RemovePartCount();

	gameObject.lock()->transform->SetLocalPosition(GetStickPos());
	gameObject.lock()->transform->SetBaseTransform(m_vwp_head.lock()->transform);

	m_vwp_partHitArea.lock()->GetGameComponent<FriendHead_PartHitArea>()->StickPart(gameObject, m_param.type);

	SpawnStickEffect();

	m_state = FacePartState::Stick;
}

void ButiEngine::FriendFacePart::SpawnStickEffect()
{
	auto drawObject = gameObject.lock()->GetGameComponent<SeparateDrawObject>()->GetDrawObject();
	drawObject.lock()->AddGameComponent<PartStickAnimation>();

	auto partHitFlash = GetManager().lock()->AddObjectFromCereal("Effect_PartHitFlash");
	partHitFlash.lock()->transform->SetLocalPosition(gameObject.lock()->transform->GetWorldPosition());
	partHitFlash.lock()->transform->SetBaseTransform(gameObject.lock()->transform);

	auto partHitBeam = GetManager().lock()->AddObjectFromCereal("Effect_PartHitBeam");
	partHitBeam.lock()->transform->SetLocalPosition(gameObject.lock()->transform->GetWorldPosition());
	partHitBeam.lock()->transform->SetLocalRotation(drawObject.lock()->transform->GetLocalRotation());
	partHitBeam.lock()->transform->SetBaseTransform(gameObject.lock()->transform);

	auto sound = gameObject.lock()->GetResourceContainer()->GetSound(SoundTag("Sound/PartHit.wav"));
	GetManager().lock()->GetApplication().lock()->GetSoundManager()->PlaySE(sound, 0.5f);
}

void ButiEngine::FriendFacePart::SetRandomVelocity()
{
	Vector3 pos = gameObject.lock()->transform->GetLocalPosition();
	Vector3 centerPos = Vector3Const::Zero;
	centerPos.z = pos.z;

	Vector3 diff = centerPos - pos;
	m_param.velocity = diff * Matrix4x4::RollZ(MathHelper::ToRadian(ButiRandom::GetRandom(-20.0f, 20.0f, 10)));
	m_param.velocity.Normalize();

	m_param.velocity *= ButiRandom::GetRandom(m_param.minMoveSpeed, m_param.maxMoveSpeed, 100);
}

void ButiEngine::FriendFacePart::OnCollisionPartHitArea(Value_weak_ptr<GameObject> arg_vwp_partHitArea)
{
	m_vwp_partHitArea = arg_vwp_partHitArea;
	auto partHitAreaComponent = arg_vwp_partHitArea.lock()->GetGameComponent<FriendHead_PartHitArea>();
	m_vwp_head = partHitAreaComponent->GetParent();

	if (partHitAreaComponent->CanStickPart(m_param.type))
	{
		if (m_param.type != PartType::Dummy)
		{
			partHitAreaComponent->SetCanStickPart(false);
		}

		gameObject.lock()->GetGameComponent<TriggerComponent>()->SetIsRemove(true);

		m_vlp_deadTimer->Stop();
		m_vlp_lifeTimer->Stop();

		auto swayComponent = gameObject.lock()->GetGameComponent<SeparateDrawObject>()->GetDrawObject().lock()->GetGameComponent<SwayAnimation>();
		if (swayComponent)
		{
			swayComponent->SetIsRemove(true);
		}

		StickToHead();
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
}

void ButiEngine::FriendFacePart::GUI_SetPartType()
{
}

void ButiEngine::FriendFacePart::GUI_SetMoveSpeed()
{
}
