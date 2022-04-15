
#include "MeshDrawComponent.h"
namespace ButiEngine {

	class Particle_Standard :public GameComponent {
	public:
		Particle_Standard() {}
		void OnSet()override;

		std::string GetGameComponentName()const override{
			return "Particle_Standard";
		}
		Value_ptr<GameComponent> Clone()override;

		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(shp_drawComponent);
			archive(shp_particleBuffer);
			archive(time);
		}
	protected:

		void OnUpdate() override;
	private:
		Value_ptr<MeshDrawComponent> shp_drawComponent;
		Value_ptr<CBuffer<ParticleParameter>> shp_particleBuffer;
		float time = 0;
	};

}
BUTI_REGIST_GAMECOMPONENT(Particle_Standard,true)