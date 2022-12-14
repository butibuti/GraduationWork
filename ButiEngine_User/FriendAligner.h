#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class FriendAligner :public GameComponent
	{
	public:

		std::string GetGameComponentName()const override {
			return "FriendAligner";
		}
		void OnUpdate()override;
		void OnSet()override;
		void OnRemove()override;
		void OnShowUI()override;
		void Start()override;
		Value_ptr<GameComponent> Clone()override;
		template<class Archive>
		void serialize(Archive& archive)
		{
			ARCHIVE_BUTI(isActive);
			ARCHIVE_BUTI(m_maxFriendCount);
		}

		Vector3 GetCalcFriendPos(std::int32_t arg_friendNum);
	private:
		std::int32_t m_maxFriendCount;
	};

}

BUTI_REGIST_GAMECOMPONENT(FriendAligner, true);