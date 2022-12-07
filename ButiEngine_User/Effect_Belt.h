#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class Effect_Belt :public GameComponent
	{
	public:

		std::string GetGameComponentName()const override {
			return "Effect_Belt";
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
		}

		void SetIsSpawnToRight(const bool arg_isSpawnToRight) { m_isSpawnToRight = arg_isSpawnToRight; }

		void Disappear(const std::int32_t arg_disappearFrame);
	private:
		void AddPositionAnimation(const Vector3& arg_targetPos, const std::int32_t arg_animFrame);

		bool m_isSpawnToRight;
		Vector3 m_startPos;
	};

}

BUTI_REGIST_GAMECOMPONENT(Effect_Belt, true);