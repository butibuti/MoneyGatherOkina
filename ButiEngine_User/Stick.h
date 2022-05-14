#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class RigidBodyComponent;
	class ParticleGenerater;

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

	private:
		void Move();
		//�G�ɐڂ���ʒu�܂ňړ�����
		void MoveToEnemy();
		//�|�P�b�g�ֈړ�����
		void MoveToPocket();
		//�ړ��̒��S��ݒ肷��
		void SetCenter();

		Value_weak_ptr<GameObject> m_vwp_pocket;
		Value_ptr<Transform> m_vlp_center;
		Value_ptr<Transform> m_vlp_rotationTarget;

		Value_weak_ptr<ParticleGenerater> m_vwp_particleGenerater;
		float m_angle;
		bool m_isPocketCatch;
	};

}

BUTI_REGIST_GAMECOMPONENT(Stick, true);