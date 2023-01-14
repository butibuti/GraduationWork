#include "stdafx_u.h"
#include "UI_FriendCount.h"
#include "FriendManager.h"

void ButiEngine::UI_FriendCount::OnUpdate()
{
	SetText();
}

void ButiEngine::UI_FriendCount::OnSet()
{
}

void ButiEngine::UI_FriendCount::OnRemove()
{
}

void ButiEngine::UI_FriendCount::OnShowUI()
{
}

void ButiEngine::UI_FriendCount::Start()
{
	m_vwp_textDraw = gameObject.lock()->GetGameComponent<TextDrawComponent>();

	SetText();
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::UI_FriendCount::Clone()
{
	return ObjectFactory::Create<UI_FriendCount>();
}

void ButiEngine::UI_FriendCount::SetText()
{
	std::int32_t friendCount = FriendManager::GetFriendDatas().size();
	std::string friendCountStr = std::to_string(friendCount);
	if (friendCount < 10)
	{
		friendCountStr = "0" + friendCountStr;
	}

	m_vwp_textDraw.lock()->SetText(U8("¡‚Ì—F’B@" + friendCountStr + "l"));
}
