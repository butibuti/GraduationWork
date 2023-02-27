#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class SpriteParticleGenerator :public GameComponent
	{
	public:
		std::string GetGameComponentName()const override {
			return "SpriteParticleGenerator";
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

		void Spark(const Vector3& arg_pos, Value_weak_ptr<Transform> arg_parentTransform);
	private:
	};

}

BUTI_REGIST_GAMECOMPONENT(SpriteParticleGenerator, true);