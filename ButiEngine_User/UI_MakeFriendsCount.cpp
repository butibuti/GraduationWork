#include "stdafx_u.h"
#include "UI_MakeFriendsCount.h"
#include "FriendManager.h"

void ButiEngine::UI_MakeFriendsCount::OnUpdate()
{
}

void ButiEngine::UI_MakeFriendsCount::OnSet()
{
}

void ButiEngine::UI_MakeFriendsCount::OnRemove()
{
}

void ButiEngine::UI_MakeFriendsCount::OnShowUI()
{
}

void ButiEngine::UI_MakeFriendsCount::Start()
{
	std::int32_t friendCount = FriendManager::GetFriendDatas().size();
	std::string friendCountStr = std::to_string(friendCount);
	if (friendCount < 10)
	{
		friendCountStr = "0" + friendCountStr;
	}

	auto textDraw = gameObject.lock()->GetGameComponent<TextDrawComponent>();
	textDraw->SetText(U8("çÏÇ¡ÇΩóFíBÅ@" + friendCountStr + "êl"));
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::UI_MakeFriendsCount::Clone()
{
	return ObjectFactory::Create<UI_MakeFriendsCount>();
}
