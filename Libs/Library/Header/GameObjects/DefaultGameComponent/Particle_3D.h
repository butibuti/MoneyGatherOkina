
#include "MeshDrawComponent.h"
namespace ButiEngine {

	class Particle_3D:public GameComponent {
	public:
		Particle_3D(){}
		std::string GetGameComponentName()const override {
			return "Particle_3D";
		}
		void OnSet()override;
		Value_ptr<GameComponent> Clone()override;
		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(vlp_drawComponent);
			archive(vlp_particleBuffer);
			archive(time);
		}
	protected:

		void OnUpdate() override;
	private:
		Value_ptr<MeshDrawComponent> vlp_drawComponent;
		Value_ptr<ButiRendering::CBuffer<ButiRendering::ParticleParameter>> vlp_particleBuffer;
		float time=0;
	};

}
BUTI_REGIST_GAMECOMPONENT(Particle_3D,true)