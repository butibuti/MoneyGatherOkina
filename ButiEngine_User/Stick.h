#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class ParticleGenerater;
	class SoundPlayerComponent;

	class Stick :public GameComponent
	{
	public:

		std::string GetGameComponentName()const override {
			return "Stick";
		}
		void OnUpdate()override;
		void OnSet()override;
		void OnShowUI()override;
		void Start()override;
		Value_ptr<GameComponent> Clone()override;
		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(isActive);
		}

		void SetPocket(Value_weak_ptr<GameObject> arg_vwp_pocket);
		bool IsPocketCatch() { return m_isPocketCatch; }
		
		void Dead();
	private:
		void KeepWorldScale();
		void KeepDistance();
		//ポケットへ移動する
		void CheckMoveFinish();
		//移動の中心を設定する
		void CreateCenter();

		Value_weak_ptr<GameObject> m_vwp_pocket;
		Value_weak_ptr<GameObject> m_vwp_center;

		Value_weak_ptr<ParticleGenerater> m_vwp_particleGenerater;
		Value_weak_ptr<SoundPlayerComponent> m_vwp_soundPlayerComponent;
		bool m_isPocketCatch;
		bool m_isMoveToPocket;
	};

}

BUTI_REGIST_GAMECOMPONENT(Stick, true);