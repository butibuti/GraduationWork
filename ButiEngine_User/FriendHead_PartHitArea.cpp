#include "stdafx_u.h"
#include "FriendHead_PartHitArea.h"
#include "FriendFacePart.h"
#include "Header/GameObjects/DefaultGameComponent/TriggerComponent.h"
#include "SeparateDrawObject.h"
#include "GuideMarker.h"

void ButiEngine::FriendHead_PartHitArea::OnUpdate()
{
}

void ButiEngine::FriendHead_PartHitArea::OnSet()
{
}

void ButiEngine::FriendHead_PartHitArea::OnRemove()
{
}

void ButiEngine::FriendHead_PartHitArea::OnShowUI()
{
	std::string typeStr;
	switch (m_type)
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
		m_type = PartType::Eye;
	}
	GUI::SameLine();
	if (GUI::Button("Nose"))
	{
		m_type = PartType::Nose;
	}
	GUI::SameLine();
	if (GUI::Button("Mouth"))
	{
		m_type = PartType::Mouth;
	}
	GUI::SameLine();
	if (GUI::Button("Dummy"))
	{
		m_type = PartType::Dummy;
	}

	GUI::BulletText(U8("正確判定になる角度範囲"));
	GUI::DragFloat("##angleBorder", &m_exactAngleBorder, 1.0f, 0.0f, 100.0f);

	GUI::BulletText(U8("正確判定になる位置範囲"));
	GUI::DragFloat("##posBorder", &m_exactPosBorder, 1.0f, 0.0f, 100.0f);
}

void ButiEngine::FriendHead_PartHitArea::Start()
{
	m_score = -1;
	m_canStickPart = true;

	m_standardPos = gameObject.lock()->transform->GetLocalPosition();
	m_standardPos += m_vwp_parent.lock()->transform->GetLocalPosition();

	CreateGuideMarker();
	SetDefaultPosObject();
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::FriendHead_PartHitArea::Clone()
{
	auto clone = ObjectFactory::Create<FriendHead_PartHitArea>();
	clone->m_type = m_type;
	clone->m_partFurthest = m_partFurthest;
	clone->m_exactAngleBorder = m_exactAngleBorder;
	clone->m_exactPosBorder = m_exactPosBorder;
	return clone;
}

bool ButiEngine::FriendHead_PartHitArea::CanStickPart(const PartType arg_type)
{
	if (arg_type == PartType::Dummy)
	{
		return true;
	}

	return m_canStickPart;
}

void ButiEngine::FriendHead_PartHitArea::StickPart(Value_weak_ptr<GameObject> arg_vwp_part, const PartType arg_type)
{
	if (arg_type == PartType::Dummy)
	{
		m_vec_vwp_dummyParts.push_back(arg_vwp_part);
	}
	else if(!m_vwp_part.lock())
	{
		m_vwp_part = arg_vwp_part;
	}
}

ButiEngine::Vector3 ButiEngine::FriendHead_PartHitArea::GetStickPos()
{
	return m_vwp_defaultPosObject.lock()->transform->GetWorldPosition();
}

std::int32_t ButiEngine::FriendHead_PartHitArea::GetCalcScore()
{
	if (m_score >= 0)
	{
		return m_score;
	}

	m_score = 0;

	Vector3 partPos = m_vwp_part.lock()->transform->GetLocalPosition();
	float distance = partPos.Distance(m_standardPos);

	float progress = 1.0f - (distance / m_partFurthest);
	progress = MathHelper::Clamp(progress, 0.0f, 1.0f);

	std::int32_t addScore = MathHelper::Lerp(0, 25, progress);
	m_score += addScore;

	return m_score;
}

void ButiEngine::FriendHead_PartHitArea::RemoveAllComponent()
{
	gameObject.lock()->GetGameComponent<TriggerComponent>()->SetIsRemove(true);
	m_vwp_guideMarker.lock()->SetIsRemove(true);
	SetIsRemove(true);
}

bool ButiEngine::FriendHead_PartHitArea::IsExact()
{
	return IsExactAngle() && IsExactPos();
}

bool ButiEngine::FriendHead_PartHitArea::IsExactAngle()
{
	if (!m_vwp_part.lock())
	{
		return false;
	}
	
	float partAngle = m_vwp_part.lock()->transform->GetLocalRotation_Euler().z;
	float drawObjectAngle = m_vwp_part.lock()->GetGameComponent<SeparateDrawObject>()->GetDrawObject().lock()->transform->GetLocalRotation_Euler().z;
	
	return abs(partAngle + drawObjectAngle) <= m_exactAngleBorder;
}

bool ButiEngine::FriendHead_PartHitArea::IsExactPos()
{
	if (!m_vwp_part.lock())
	{
		return false;
	}

	float pos = m_vwp_defaultPosObject.lock()->transform->GetLocalPosition().z;
	float partPos = m_vwp_part.lock()->transform->GetLocalPosition().z;
	float diff = partPos - pos;

	return diff >= -m_exactPosBorder;
}

void ButiEngine::FriendHead_PartHitArea::CreateGuideMarker()
{
	m_vwp_guideMarker = GetManager().lock()->AddObjectFromCereal("GuideMarker");
	auto guideMarker = m_vwp_guideMarker.lock()->GetGameComponent<GuideMarker>();
	guideMarker->SetMarkTarget(gameObject);

	if (m_type == PartType::Eye)
	{
	}
	else if (m_type == PartType::Nose)
	{
	}
	else if (m_type == PartType::Mouth)
	{
		guideMarker->SetColor(ButiColor::Red());
	}
}

void ButiEngine::FriendHead_PartHitArea::SetDefaultPosObject()
{
	if (m_type == PartType::Eye)
	{
		m_vwp_defaultPosObject = GetManager().lock()->GetGameObject("Eyes_Default");
	}
	else if (m_type == PartType::Nose)
	{
		m_vwp_defaultPosObject = GetManager().lock()->GetGameObject("Nose_Default");
	}
	else if (m_type == PartType::Mouth)
	{
		m_vwp_defaultPosObject = GetManager().lock()->GetGameObject("Mouth_Default");
	}
}
