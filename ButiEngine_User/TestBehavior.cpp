#include "TestBehavior.h"
#include"Header/Common/CollisionPrimitive.h"
#include"ButiEngineHeader/Header/GameObjects/DefaultGameComponent/RigidBodyComponent.h"
#include"ButiBulletWrap/ButiBulletWrap/PhysicsManager.h"
#include"ButiBulletWrap/ButiBulletWrap/PhysicsWorld.h"
#include"ButiBulletWrap/ButiBulletWrap/Joint.h"
float g_vibe_l = 0.0f, g_vibe_r = 0.0f;
void ButiEngine::TestBehavior::OnUpdate()
{/*
	if (GameDevice::GetVRTrackerInput().GetAllDeviceNames().GetSize()>m_index) {
		Matrix4x4 deviceMatrix;
		GameDevice::GetVRTrackerInput().GetDevicePoseMatrix(GameDevice::GetVRTrackerInput().GetAllDeviceNames()[m_index], deviceMatrix);
		gameObject.lock()->transform->SetLocalPosition(deviceMatrix.GetPosition());
		gameObject.lock()->transform->SetLocalRotation(deviceMatrix.RemovePosition());
	}*/
	if (GameDevice::GetInput().TriggerKey(ButiInput::Keys::Enter)) {
		auto objA = gameObject.lock()->GetGameObjectManager().lock()->GetGameObject("PhysicsObjectA").lock()->GetGameComponent<RigidBodyComponent>()->GetRigidBody();
		auto objB = gameObject.lock()->GetGameObjectManager().lock()->GetGameObject("PhysicsObjectB").lock()->GetGameComponent<RigidBodyComponent>()->GetRigidBody();
		auto joint = ButiBullet::CreateSpringJoint(objA, Matrix4x4(), objB, Matrix4x4(), 0);
		joint->SetLinearUpperLimit(Vector3(2, 2, 2));
		joint->SetLinearLowerLimit(Vector3(-2, -2, -2));
		joint->SetAngularUpperLimit(Vector3(0, MathHelper::ToRadian(360), 0));
		joint->SetLinearStiffness(Vector3(2, 2, 2));
		gameObject.lock()->GetGameObjectManager().lock()->GetScene().lock()->GetPhysicsManager()->GetActivePhysicsWorld()->
			AddJoint(joint);
	}
}


void ButiEngine::TestBehavior::OnSet()
{
	/*
	gameObject.lock()->AddCollisionStayReaction(
		[this](ButiBullet::ContactData&arg_other) {
			if (arg_other.vwp_gameObject.lock()) {
				gameObject.lock()->SetIsRemove(true);
				m_vwp_drawObject.lock()->SetIsRemove(true);
			}
		}
	);*/
}

void ButiEngine::TestBehavior::Start()
{
	//GetCamera("IDPath")->vlp_transform->SetBaseTransform(GetCamera("main")->vlp_transform,true);
}
void ButiEngine::TestBehavior::OnRemove()
{
	
}

void ButiEngine::TestBehavior::OnShowUI()
{
	GUI::DragInt("TrackerIndex", m_index, 1.0, 0, 16);
}

void ButiEngine::TestBehavior::ShowGUI()
{
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::TestBehavior::Clone()
{
	auto clone = ObjectFactory::Create<TestBehavior>();
	clone->m_index = m_index;
	return clone;
}