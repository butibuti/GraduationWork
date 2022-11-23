#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class GameSettings;
	class PauseManager;

	class FriendBody :public GameComponent
	{
	public:

		std::string GetGameComponentName()const override {
			return "FriendBody";
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

		void SetHead(Value_weak_ptr<GameObject> arg_vwp_head);
	private:
		void Rotate();
		void SpawnNewHead();
		void SpawnNewBody();

		Value_weak_ptr<GameSettings> m_vwp_gameSettings;
		Value_weak_ptr<PauseManager> m_vwp_pauseManager;



		bool m_isRotate;
	};

}

BUTI_REGIST_GAMECOMPONENT(FriendBody, true);