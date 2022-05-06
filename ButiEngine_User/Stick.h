#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class RigidBodyComponent;

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
	private:
		void Move();
		//敵に接する位置まで移動する
		void MoveToEnemy();
		//ポケットへ移動する
		void MoveToPocket();
		//移動の中心を設定する
		void SetCenter();

		Value_weak_ptr<GameObject> m_vwp_pocket;
		Value_ptr<Transform> m_vlp_center;
		Value_ptr<Transform> m_vlp_rotationTarget;
	};

}

BUTI_REGIST_GAMECOMPONENT(Stick, true);