#include "stdafx_u.h"
#include "PartRespawnPoint.h"
#include"FacePartSpawner.h"
#include"FriendFacePart.h"

void ButiEngine::PartRespawnPoint::OnUpdate()
{
}

void ButiEngine::PartRespawnPoint::Failed()
{
	if (!m_isContinue) {
		m_vwp_part = nullptr;
		if (m_param.type != PartType::Dummy) {
			SpawnFacePart();
		}
	}
	else {
		m_vwp_spawner.lock()->DecreaseParts();
		m_vwp_part = nullptr;
	}
}

void ButiEngine::PartRespawnPoint::Success()
{
	m_vwp_part = nullptr;
	m_vwp_spawner.lock()->DecreaseParts();
	m_vwp_spawner.lock()->Success();
}

void ButiEngine::PartRespawnPoint::Damage()
{
	m_vwp_spawner.lock()->Damage();
}

void ButiEngine::PartRespawnPoint::Start()
{
	SpawnFacePart();
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::PartRespawnPoint::Clone()
{
	return ObjectFactory::Create<PartRespawnPoint>();
}

void ButiEngine::PartRespawnPoint::Clear()
{
	if (m_vwp_part.lock()) {
		m_vwp_part.lock()->SetIsRemove(true);
	}
}

void ButiEngine::PartRespawnPoint::SpawnFacePart()
{
	std::string objectName = "FriendFacePart_";
	switch (m_param.type)
	{
	case PartType::Eye:
		objectName += "Eyes";
		break;
	case PartType::Nose:
		objectName += "Nose";
		break;
	case PartType::Mouth:
		objectName += "Mouth";
		break;
	case PartType::Dummy:
		objectName += "Dummy";
		break;
	default:
		break;
	}


	m_vwp_part =  GetManager().lock()->AddObjectFromCereal(objectName, gameObject.lock()->transform);
	m_vwp_part.lock()->GetGameComponent<FriendFacePart>()->SetRespawnPoint(GetThis<PartRespawnPoint>());
	m_vwp_part.lock()->GetGameComponent<FriendFacePart>()->SetParam(m_param);
}
