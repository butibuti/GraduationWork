#include "stdafx_u.h"
#include "GameSettings.h"

void ButiEngine::GameSettings::OnUpdate()
{
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

	GUI::BulletText(U8("�Q�[�����œ�����͈�"));
	GUI::DragFloat3("##HeadMoveLimit", &m_data.headMoveLimit.x, 1.0f, 0.0f, 100.0f);

	GUI::BulletText(U8("�Q�[���O�̈ړ��͈�"));
	GUI::BulletText(U8("�O�E��"));
	GUI::Text("x:" + std::to_string(m_data.moveAreaFrontRightTop.x) + " y:" + std::to_string(m_data.moveAreaFrontRightTop.y) + " z:" + std::to_string(m_data.moveAreaFrontRightTop.z));
	if (GUI::Button("SetFRT"))
	{
		SetMoveAreaFrontRightTop();
	}
	GUI::BulletText(U8("�㍶��"));
	GUI::Text("x:" + std::to_string(m_data.moveAreaBackLeftBottom.x) + " y:" + std::to_string(m_data.moveAreaBackLeftBottom.y) + " z:" + std::to_string(m_data.moveAreaBackLeftBottom.z));
	if (GUI::Button("SetBLB"))
	{
		SetMoveAreaBackLeftBottom();
	}

	GUI::BulletText(U8("��̈ʒu"));
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
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::GameSettings::Clone()
{
	return ObjectFactory::Create<GameSettings>();
}

ButiEngine::Vector3 ButiEngine::GameSettings::GetCorrection()
{
	Matrix4x4 deviceMatrix;
	GameDevice::GetVRTrackerInput().GetDevicePoseMatrix(GameDevice::GetVRTrackerInput().GetAllDeviceNames()[m_data.trackerIndex], deviceMatrix);
	Vector3 pos = deviceMatrix.GetPosition();

	Vector3 moveAreaSize;
	if (pos.x >= 0.0f)
	{
		moveAreaSize.x = abs(m_data.moveAreaFrontRightTop.x);
	}
	else
	{
		moveAreaSize.x = abs(m_data.moveAreaBackLeftBottom.x);
	}

	if (moveAreaSize.x == 0.0f)
	{
		moveAreaSize.x = 0.01f;
	}


	if (pos.y >= 0.0f)
	{
		moveAreaSize.y = abs(m_data.moveAreaFrontRightTop.y);
	}
	else
	{
		moveAreaSize.y = abs(m_data.moveAreaBackLeftBottom.y);
	}

	if (moveAreaSize.y == 0.0f)
	{
		moveAreaSize.y = 0.01f;
	}

	if (pos.z >= 0.0f)
	{
		moveAreaSize.z = abs(m_data.moveAreaFrontRightTop.z);
	}
	else
	{
		moveAreaSize.z = abs(m_data.moveAreaBackLeftBottom.z);
	}

	if (moveAreaSize.z == 0.0f)
	{
		moveAreaSize.z = 0.01f;
	}

	Vector3 correction = m_data.headMoveLimit / moveAreaSize;


	return correction;
}

void ButiEngine::GameSettings::SetOrigin()
{
	GameDevice::GetVRTrackerInput().SetOrigin(m_data.trackerIndex);
}

void ButiEngine::GameSettings::SetMoveAreaFrontRightTop()
{
	Matrix4x4 deviceMatrix;
	GameDevice::GetVRTrackerInput().GetDevicePoseMatrix(GameDevice::GetVRTrackerInput().GetAllDeviceNames()[m_data.trackerIndex], deviceMatrix);
	m_data.moveAreaFrontRightTop = deviceMatrix.GetPosition();
}

void ButiEngine::GameSettings::SetMoveAreaBackLeftBottom()
{
	Matrix4x4 deviceMatrix;
	GameDevice::GetVRTrackerInput().GetDevicePoseMatrix(GameDevice::GetVRTrackerInput().GetAllDeviceNames()[m_data.trackerIndex], deviceMatrix);
	m_data.moveAreaBackLeftBottom = deviceMatrix.GetPosition();
}

void ButiEngine::GameSettings::SetTablePos()
{
	Matrix4x4 deviceMatrix;
	GameDevice::GetVRTrackerInput().GetDevicePoseMatrix(GameDevice::GetVRTrackerInput().GetAllDeviceNames()[m_data.trackerIndex], deviceMatrix);
	m_data.tablePos = deviceMatrix.GetPosition();
}