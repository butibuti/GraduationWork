#include "stdafx_u.h"
#include "Effect_CompleteFriend.h"
#include "Header/GameObjects/DefaultGameComponent/ScaleAnimationComponent.h"

void ButiEngine::Effect_CompleteFriend::OnUpdate()
{
	float progress = m_vlp_animationTimer->GetPercent();
	float newRotation = MathHelper::Lerp(-10.0f, -370.0f, Easing::EaseOutQuart(progress));

	gameObject.lock()->transform->SetLocalRotationZ_Degrees(newRotation);

	if (m_vlp_animationTimer->Update())
	{
		m_vlp_animationTimer->Stop();

		gameObject.lock()->transform->SetLocalRotationZ_Degrees(-10.0f);
		Dead();
	}
}

void ButiEngine::Effect_CompleteFriend::OnSet()
{
}

void ButiEngine::Effect_CompleteFriend::OnRemove()
{
}

void ButiEngine::Effect_CompleteFriend::OnShowUI()
{
	GUI::BulletText("AnimationFrame");
	GUI::DragInt("##animFrame", m_animationFrame, 1.0f, 0, 300);
}

void ButiEngine::Effect_CompleteFriend::Start()
{
	auto head = GetManager().lock()->GetGameObject(GameObjectTag("FriendHead"));
	Vector3 headPos = head.lock()->transform->GetWorldPosition();
	Vector3 neckPos = headPos;
	neckPos.y -= 1.2f;
	gameObject.lock()->transform->SetLocalPosition(neckPos);

	CreateBeams();

	m_vlp_animationTimer = ObjectFactory::Create<RelativeTimer>(m_animationFrame);
	m_vlp_animationTimer->Start();
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::Effect_CompleteFriend::Clone()
{
	auto clone = ObjectFactory::Create<Effect_CompleteFriend>();
	clone->m_animationFrame = m_animationFrame;
	return clone;
}

void ButiEngine::Effect_CompleteFriend::CreateBeams()
{
	std::int32_t beamCount = 3;
	for (std::int32_t i = 0; i < beamCount; i++)
	{
		auto beam = GetManager().lock()->AddObjectFromCereal("Beam");
		beam.lock()->transform->SetBaseTransform(gameObject.lock()->transform, true);

		beam.lock()->transform->SetLocalRotationZ_Degrees(360.0f / beamCount * i);

		auto anim = beam.lock()->GetGameComponent<ScaleAnimation>();
		anim->SetSpeed(1.0f / m_animationFrame);

		m_vec_vwp_beams.push_back(beam);
	}
}

void ButiEngine::Effect_CompleteFriend::Dead()
{
	auto end = m_vec_vwp_beams.end();
	for (auto itr = m_vec_vwp_beams.begin(); itr != end; ++itr)
	{
		(*itr).lock()->SetIsRemove(true);
	}

	gameObject.lock()->SetIsRemove(true);
}
