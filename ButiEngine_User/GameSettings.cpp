#include "stdafx_u.h"
#include "GameSettings.h"
#include "PauseManager.h"

void ButiEngine::GameSettings::OnUpdate()
{
	if (GameDevice::GetInput().CheckKey(ButiInput::Keys::LeftCtrl) &&
		GameDevice::GetInput().CheckKey(ButiInput::Keys::LeftShift) &&
		GameDevice::GetInput().TriggerKey(ButiInput::Keys::D))
	{
		{
			m_isDebugMode = !m_isDebugMode;

			auto pauseManager = GetManager().lock()->GetGameObject("PauseManager").lock()->GetGameComponent<PauseManager>();

			if (m_isDebugMode)
			{
				gameObject.lock()->GetGameComponent<MeshDrawComponent>(0)->ReRegist();
				m_vwp_debugHead.lock()->GetTransform()->SetLocalScale(1.0f);
				pauseManager->SetIsPause(true);
			}
			else
			{
				gameObject.lock()->GetGameComponent<MeshDrawComponent>(0)->UnRegist();
				m_vwp_debugHead.lock()->GetTransform()->SetLocalScale(0.0f);
				pauseManager->SetIsPause(false);
			}
		}
	}

	if (!m_isDebugMode)
	{
		return;
	}

	SetOffset();

	if (GameDevice::GetInput().TriggerKey(ButiInput::Keys::O))
	{
		SetOrigin();
		OutputCereal(m_data, "GameSettings.savedata");
	}
	if (GameDevice::GetInput().TriggerKey(ButiInput::Keys::F))
	{
		//SetMoveAreaFront();
	}
	if (GameDevice::GetInput().TriggerKey(ButiInput::Keys::R))
	{
		SetMoveAreaRightTop();
		OutputCereal(m_data, "GameSettings.savedata");
	}
	if (GameDevice::GetInput().TriggerKey(ButiInput::Keys::B))
	{
		//SetMoveAreaBack();
	}
	if (GameDevice::GetInput().TriggerKey(ButiInput::Keys::L))
	{
		SetMoveAreaLeftBottom();
		OutputCereal(m_data, "GameSettings.savedata");
	}

	ControlDebugHead();
}

void ButiEngine::GameSettings::OnSet()
{
}

void ButiEngine::GameSettings::OnRemove()
{
}

void ButiEngine::GameSettings::OnShowUI()
{
	GUI::DragInt("TrackerIndex", m_data.trackerIndex, 1.0, 0, 16);

	if (GUI::Button("SetOrigin"))
	{
		SetOrigin();
	}

	GUI::BulletText(U8("ゲーム内で動ける範囲"));
	GUI::DragFloat3("##HeadMoveLimit", &m_data.headMoveLimit.x, 1.0f, 0.0f, 100.0f);

	GUI::BulletText(U8("ゲーム外の移動範囲"));
	GUI::BulletText(U8("前右上"));
	GUI::Text("x:" + std::to_string(m_data.moveAreaFrontRightTop.x) + " y:" + std::to_string(m_data.moveAreaFrontRightTop.y) + " z:" + std::to_string(m_data.moveAreaFrontRightTop.z));
	if (GUI::Button("SetFront"))
	{
		SetMoveAreaFront();
	}

	if (GUI::Button("SetRT"))
	{
		SetMoveAreaRightTop();
	}

	GUI::BulletText(U8("後左下"));
	GUI::Text("x:" + std::to_string(m_data.moveAreaBackLeftBottom.x) + " y:" + std::to_string(m_data.moveAreaBackLeftBottom.y) + " z:" + std::to_string(m_data.moveAreaBackLeftBottom.z));

	if (GUI::Button("SetBack"))
	{
		SetMoveAreaBack();
	}

	if (GUI::Button("SetLB"))
	{
		SetMoveAreaLeftBottom();
	}

	GUI::BulletText(U8("台の位置"));
	GUI::Text("x:" + std::to_string(m_data.tablePos.x) + " y:" + std::to_string(m_data.tablePos.y) + " z:" + std::to_string(m_data.tablePos.z));
	if (GUI::Button("SetTablePos"))
	{
		SetTablePos();
	}

	if (GUI::Button("Save"))
	{
		OutputCereal(m_data, "GameSettings.savedata");
	}
	if (GUI::Button("Load"))
	{
		InputCereal(m_data, "GameSettings.savedata");
	}
}

void ButiEngine::GameSettings::Start()
{
	InputCereal(m_data, "GameSettings.savedata");
	GameDevice::GetVRTrackerInput().SetOrigin(m_data.trackerOrigin);

	m_headMoveLimit = GetManager().lock()->GetGameObject("FieldOfView").lock()->transform->GetLocalScale().x;
	m_offset = Vector3Const::Zero;

	m_isDebugMode = false;

	gameObject.lock()->GetGameComponent<MeshDrawComponent>(0)->UnRegist();
	m_vwp_debugHead = gameObject.lock()->GetGameComponent<MeshDrawComponent>(1);
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::GameSettings::Clone()
{
	return ObjectFactory::Create<GameSettings>();
}

float ButiEngine::GameSettings::GetCorrection()
{
	float moveArea = abs(m_data.moveAreaFrontRightTop.x - m_data.moveAreaBackLeftBottom.x);

	float correction = m_headMoveLimit / moveArea;

	return correction;
}

ButiEngine::Vector3 ButiEngine::GameSettings::GetTrackerPos(const std::int32_t arg_trackerIndex)
{
	if (!(GameDevice::GetVRTrackerInput().GetAllDeviceNames().GetSize() > arg_trackerIndex))
	{
		return Vector3Const::Zero;
	}

	Matrix4x4 deviceMatrix;
	GameDevice::GetVRTrackerInput().GetDevicePoseMatrix(GameDevice::GetVRTrackerInput().GetAllDeviceNames()[arg_trackerIndex], deviceMatrix);
	Vector3 pos = deviceMatrix.GetPosition();
	pos *= GetCorrection();
	pos.x *= -1.0f;
	pos.z = 0.0f;

	pos += m_offset;

	return pos;
}

ButiEngine::Matrix4x4 ButiEngine::GameSettings::GetTrackerRotation(const std::int32_t arg_trackerIndex)
{
	if (!(GameDevice::GetVRTrackerInput().GetAllDeviceNames().GetSize() > arg_trackerIndex))
	{
		return Matrix4x4().Identity();
	}

	Matrix4x4 deviceMatrix;
	GameDevice::GetVRTrackerInput().GetDevicePoseMatrix(GameDevice::GetVRTrackerInput().GetAllDeviceNames()[arg_trackerIndex], deviceMatrix);
	auto rotation = deviceMatrix.GetRemovePosition();
	rotation._13 *= -1;
	rotation._31 *= -1;
	rotation._12 *= -1;
	rotation._21 *= -1;

	return rotation;
}

void ButiEngine::GameSettings::SetOrigin()
{
	GameDevice::GetVRTrackerInput().SetOrigin(m_data.trackerIndex);
	m_data.trackerOrigin = GameDevice::GetVRTrackerInput().GetOrigin();
}

void ButiEngine::GameSettings::SetMoveAreaFront()
{
	Matrix4x4 deviceMatrix;
	GameDevice::GetVRTrackerInput().GetDevicePoseMatrix(GameDevice::GetVRTrackerInput().GetAllDeviceNames()[m_data.trackerIndex], deviceMatrix);
	m_data.moveAreaFrontRightTop.z = deviceMatrix.GetPosition().z;
}

void ButiEngine::GameSettings::SetMoveAreaRightTop()
{
	Matrix4x4 deviceMatrix;
	GameDevice::GetVRTrackerInput().GetDevicePoseMatrix(GameDevice::GetVRTrackerInput().GetAllDeviceNames()[m_data.trackerIndex], deviceMatrix);
	float z = m_data.moveAreaFrontRightTop.z;
	m_data.moveAreaFrontRightTop = deviceMatrix.GetPosition();
	m_data.moveAreaFrontRightTop.z = z;
}

void ButiEngine::GameSettings::SetMoveAreaBack()
{
	Matrix4x4 deviceMatrix;
	GameDevice::GetVRTrackerInput().GetDevicePoseMatrix(GameDevice::GetVRTrackerInput().GetAllDeviceNames()[m_data.trackerIndex], deviceMatrix);
	m_data.moveAreaBackLeftBottom.z = deviceMatrix.GetPosition().z;
}

void ButiEngine::GameSettings::SetMoveAreaLeftBottom()
{
	Matrix4x4 deviceMatrix;
	GameDevice::GetVRTrackerInput().GetDevicePoseMatrix(GameDevice::GetVRTrackerInput().GetAllDeviceNames()[m_data.trackerIndex], deviceMatrix);
	float z = m_data.moveAreaBackLeftBottom.z;
	m_data.moveAreaBackLeftBottom = deviceMatrix.GetPosition();
	m_data.moveAreaBackLeftBottom.z = z;
}

void ButiEngine::GameSettings::SetTablePos()
{
	Matrix4x4 deviceMatrix;
	GameDevice::GetVRTrackerInput().GetDevicePoseMatrix(GameDevice::GetVRTrackerInput().GetAllDeviceNames()[m_data.trackerIndex], deviceMatrix);
	m_data.tablePos = deviceMatrix.GetPosition();

	Vector3 bodyPos = deviceMatrix.GetPosition();
	bodyPos *= GetCorrection();
	bodyPos.x *= -1.0f;
	m_data.bodyPos = bodyPos;
}

void ButiEngine::GameSettings::SetOffset()
{
	if (GameDevice::GetInput().CheckKey(ButiInput::Keys::LeftCtrl))
	{
		return;
	}

	if (GameDevice::GetInput().TriggerKey(ButiInput::Keys::R))
	{
		m_offset = Vector3Const::Zero;
	}

	float speed = 0.01f;
	if (GameDevice::GetInput().CheckKey(ButiInput::Keys::LeftShift))
	{
		speed *= 10;
	}

	if (GameDevice::GetInput().CheckKey(ButiInput::Keys::A))
	{
		m_offset.x += speed;
	}
	else if (GameDevice::GetInput().CheckKey(ButiInput::Keys::D))
	{
		m_offset.x -= speed;
	}

	if (GameDevice::GetInput().CheckKey(ButiInput::Keys::W))
	{
		m_offset.y += speed;
	}
	else if (GameDevice::GetInput().CheckKey(ButiInput::Keys::S))
	{
		m_offset.y -= speed;
	}
}

void ButiEngine::GameSettings::ControlDebugHead()
{
	if (GameDevice::GetVRTrackerInput().GetAllDeviceNames().GetSize() > m_data.trackerIndex)
	{
		m_vwp_debugHead.lock()->GetTransform()->SetLocalPosition(GetTrackerPos(m_data.trackerIndex));
		m_vwp_debugHead.lock()->GetTransform()->SetLocalRotation(GetTrackerRotation(m_data.trackerIndex));
	}
}
