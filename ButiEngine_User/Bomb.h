#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class NumberDraw;
	class SpriteParticleGenerator;
	class PauseManager;

	class Bomb :public GameComponent
	{
	public:

		std::string GetGameComponentName()const override {
			return "Bomb";
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

		void SetFrameToExplode(const std::int32_t arg_frame) { m_frameToExplode = arg_frame; }
		void SetParent(Value_weak_ptr<GameObject> arg_parent) { m_vwp_parent = arg_parent; }

		void Dead();
		void StartFall();
	private:
		void Explode();
		void BombAnimation();
		void ColorAnimation(const float arg_progress);
		void ScaleAnimation(const float arg_progress);
		void Fall();
		void SetTimer();

		void TinderAnimation();
		void SpawnSparkEffect();

		Value_weak_ptr<PauseManager> m_vwp_pauseManager;

		Value_weak_ptr<SpriteParticleGenerator> m_vwp_particleGenerator;

		Value_weak_ptr<GameObject> m_vwp_parent;
		Value_weak_ptr<GameObject> m_vwp_timerText;
		Value_weak_ptr<NumberDraw> m_vwp_timerTextComponent;

		Value_weak_ptr<Transform> m_vwp_tinderTransform;
		Value_weak_ptr<Transform> m_vwp_spawnSparkTransform;

		Value_ptr<RelativeTimer> m_vlp_explodeTimer;
		std::int32_t m_frameToExplode;
		Value_ptr<RelativeTimer> m_vlp_waitExplodeTimer;
		Value_weak_ptr<MeshDrawComponent> m_vwp_meshDraw;

		float m_theta;
		bool m_isFall;
	};

}

BUTI_REGIST_GAMECOMPONENT(Bomb, true);