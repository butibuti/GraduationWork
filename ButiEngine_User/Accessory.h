#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class PauseManager;

	class Accessory :public GameComponent
	{
	public:

		std::string GetGameComponentName()const override {
			return "Accessory";
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

		static std::int32_t GetAccessoryCount() { return g_accessoryCount; }
		static void ResetAccessoryCount() { g_accessoryCount = 0; }
		static void AddAccessoryCount() { g_accessoryCount++; }
		static void RemoveAccessoryCount()
		{
			g_accessoryCount--; 
			g_accessoryCount = max(g_accessoryCount, 0);
		}

		void Dead();
	private:
		void OnCollisionHeadCenter(Value_weak_ptr<GameObject> arg_vwp_partHitArea);
		void Appear();
		void OnAppear();

		Value_weak_ptr<PauseManager> m_vwp_pauseManager;

		Value_weak_ptr<GameObject> m_vwp_drawObject;
		bool m_isAppear;

		static std::int32_t g_accessoryCount;
	};

}

BUTI_REGIST_GAMECOMPONENT(Accessory, true);