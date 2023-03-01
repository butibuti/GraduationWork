#include "stdafx_u.h"
#include "SpriteParticleGenerator.h"

void ButiEngine::SpriteParticleGenerator::OnUpdate()
{
}

void ButiEngine::SpriteParticleGenerator::OnSet()
{
}

void ButiEngine::SpriteParticleGenerator::OnRemove()
{
}

void ButiEngine::SpriteParticleGenerator::OnShowUI()
{
}

void ButiEngine::SpriteParticleGenerator::Start()
{
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::SpriteParticleGenerator::Clone()
{
	return ObjectFactory::Create<SpriteParticleController>();
}

void ButiEngine::SpriteParticleGenerator::Spark(const Vector3& arg_pos, Value_weak_ptr<Transform> arg_parentTransform)
{
    Particle2D particle;
    Vector3 position = arg_pos;

    float offsetX = ButiRandom::GetRandom(0.025f, 0.05f, 100);
    offsetX = ButiRandom::GetInt(0, 1) ? offsetX : -offsetX;
    position.x += offsetX;

    float offsetY = ButiRandom::GetRandom(0.025f, 0.05f, 100);
    offsetY = ButiRandom::GetInt(0, 1) ? offsetY : -offsetY;
    position.y += offsetY;

    position.z += 0.1f;

    particle.position = position;

    Vector4 color = Vector4(1.0f, 1.0f, 0.0f, 1.0f);
    color.y = ButiRandom::GetRandom(0.3f, 1.0f, 10);
    particle.color = color;

    particle.size = ButiRandom::GetRandom(40.0f, 60.0f, 10);
    particle.life = 6;
    particle.angle = MathHelper::ToRadian(ButiRandom::GetInt(0, 90));

    particle.m_parentTransform = arg_parentTransform;

    gameObject.lock()->GetGameComponent<SpriteParticleController>()->AddParticle(particle);
}
