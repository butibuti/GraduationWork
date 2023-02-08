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
		SpawnFacePart();
	}
	else {
		m_vwp_spawner.lock()->DecreaseParts();
	}
}

void ButiEngine::PartRespawnPoint::Success()
{
	gameObject.lock()->SetIsRemove(true);
	m_vwp_spawner.lock()->DecreaseParts();
	m_vwp_spawner.lock()->Success();
}

void ButiEngine::PartRespawnPoint::Start()
{
	SpawnFacePart();
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::PartRespawnPoint::Clone()
{
	return ObjectFactory::Create<PartRespawnPoint>();
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

	auto gameobj = GetManager().lock()->AddObjectFromCereal(objectName, gameObject.lock()->transform);
	gameobj.lock()->GetGameComponent<FriendFacePart>()->SetRespawnPoint(GetThis<PartRespawnPoint>());
	gameobj.lock()->GetGameComponent<FriendFacePart>()->SetParam(m_param);
}
