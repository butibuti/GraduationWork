#include "stdafx_u.h"
#include "GameSettings.h"

void ButiEngine::GameSettings::OnUpdate()
{
	if (GameDevice::GetInput().CheckKey(ButiInput::Keys::LeftCtrl) &&
		GameDevice::GetInput().CheckKey(ButiInput::Keys::LeftShift) &&
		GameDevice::GetInput().TriggerKey(ButiInput::Keys::D))
	{
		m_isDebugMode = !m_isDebugMode;
		if (m_isDebugMode)
		{
			gameObject.lock()->GetGameComponent<MeshDrawComponent>()->ReRegist();
		}
		else
		{
			gameObject.lock()->GetGameComponent<MeshDrawComponent>()->UnRegist();
		}
	}

	if (!m_isDebugMode)
	{
		return;
	}

	if (GameDevice::GetInput().TriggerKey(ButiInput::Keys::O))
	{
		SetOrigin();
	}
	if (GameDevice::GetInput().TriggerKey(ButiInput::Keys::F))
	{
		SetMoveAreaFront();
	}
	if (GameDevice::GetInput().TriggerKey(ButiInput::Keys::R))
	{
		SetMoveAreaRightTop();
	}
	if (GameDevice::GetInput().TriggerKey(ButiInput::Keys::B))
	{
		SetMoveAreaBack();
	}
	if (GameDevice::GetInput().TriggerKey(ButiInput::Keys::L))
	{
		SetMoveAreaLeftBottom();
	}
	if (GameDevice::GetInput().TriggerKey(ButiInput::Keys::S))
	{
		OutputCereal(m_data, "GameSettings.savedata");
	}
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

	m_isDebugMode = false;

	gameObject.lock()->GetGameComponent<MeshDrawComponent>()->UnRegist();
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::GameSettings::Clone()
{
	return ObjectFactory::Create<GameSettings>();
}

float ButiEngine::GameSettings::GetCorrection()
{
	Matrix4x4 deviceMatrix;
	GameDevice::GetVRTrackerInput().GetDevicePoseMatrix(GameDevice::GetVRTrackerInput().GetAllDeviceNames()[m_data.trackerIndex], deviceMatrix);
	Vector3 pos = deviceMatrix.GetPosition();

	Vector3 moveAreaSize;

	moveAreaSize = m_data.moveAreaFrontRightTop - m_data.moveAreaBackLeftBottom;
	moveAreaSize.x = abs(moveAreaSize.x);
	moveAreaSize.y = abs(moveAreaSize.y);
	moveAreaSize.z = abs(moveAreaSize.z);

	Vector3 correction = m_data.headMoveLimit * 2.0f / moveAreaSize;


	return correction.x;
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
