#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class UI_FriendCount :public GameComponent
	{
	public:

		std::string GetGameComponentName()const override {
			return "UI_FriendCount";
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

		void AddCount();
		void RemoveCount();

	private:
		void SetText();
		void CheckNextAnimation();
		void CheckRestartAnimation();
		void StartAddCountAnimation();
		void StartRemoveCountAnimation();
		void StartReturnAnimation();
		void Animation();
		void SetNumberPos(const float arg_pos);
		void SetNumberScale(const Vector3& arg_scale);

		Value_weak_ptr<TextDrawComponent> m_vwp_textDraw;

		std::vector<Value_weak_ptr<MeshDrawComponent>> m_vec_vwp_numbers;

		std::int32_t m_targetCount;
		std::int32_t m_drawCount;

		Value_ptr<RelativeTimer> m_vlp_animTimer;
		std::int32_t m_animFrame;
		float m_startNumberPos;
		float m_targetNumberPos;
		Vector3 m_defaultNumberScale;
		Vector3 m_startNumberScale;
		Vector3 m_targetNumberScale;

		bool m_isAddCountAnimation;
		bool m_isRemoveCountAnimation;
		bool m_isReturnAnimation;

		bool m_isFirstAnimation;
	};

}

BUTI_REGIST_GAMECOMPONENT(UI_FriendCount, true);