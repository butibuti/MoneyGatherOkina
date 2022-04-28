//cerial
#include "../../cereal/include/cereal/cereal.hpp"
#include "../../cereal/include/cereal/archives/binary.hpp"
#include "../../cereal/include/cereal/archives/portable_binary.hpp"
#include "../../cereal/include/cereal/types/polymorphic.hpp"
#include "../../cereal/include/cereal/types/utility.hpp"
#include "../../cereal/include/cereal/types/string.hpp"
#include "../../cereal/include/cereal/types/unordered_map.hpp"
#include "../../cereal/include/cereal/types/map.hpp"
#include "../../cereal/include/cereal/types/tuple.hpp"
#include "../../cereal/include/cereal/types/vector.hpp"
#include "../../cereal/include/cereal/types/array.hpp"

#define BUTI_REGIST_CBUFFER(T)\
CEREAL_REGISTER_TYPE(ButiEngine::ButiRendering::CBuffer_Dx12<T>);\
CEREAL_REGISTER_POLYMORPHIC_RELATION(ButiEngine::ButiRendering::ICBuffer, ButiEngine::ButiRendering::CBuffer_Dx12<T>);\


#define BUTI_REGIST_CARRAYBUFFER(T)\
	CEREAL_REGISTER_TYPE(ButiEngine::ButiRendering::CArrayBuffer_Dx12<T>);\
	CEREAL_REGISTER_POLYMORPHIC_RELATION(ButiEngine::ButiRendering::ICBuffer, ButiEngine::ButiRendering::CArrayBuffer_Dx12<T>);\


#define BUTI_REGIST_BUTISCRIPTSHAREDTYPE(T)\
CEREAL_REGISTER_TYPE(ButiEngine::ValuePtrRestoreObject <Value_ptr< T >>); \
CEREAL_REGISTER_POLYMORPHIC_RELATION(ButiEngine::IValuePtrRestoreObject,ButiEngine::ValuePtrRestoreObject <Value_ptr< T >>); \


#define BUTI_REGIST_GAMECOMPONENT(T , IsOverrideOnUpdate)\
	CEREAL_REGISTER_TYPE(ButiEngine:: T);\
	CEREAL_REGISTER_POLYMORPHIC_RELATION(ButiEngine::GameComponent, ButiEngine:: T);\
namespace ComponentRegister{ namespace T {\
	static const ButiEngine::GameComponentRegister  componentregister = ButiEngine::GameComponentRegister(ButiEngine::ObjectFactory::Create<ButiEngine:: T>(),IsOverrideOnUpdate);\
	\
}}

#define BUTI_REGIST_BUTISCRIPTTYPE(T)\
	CEREAL_REGISTER_TYPE(ButiEngine::ValuePtrRestoreObject < T >);\
	CEREAL_REGISTER_POLYMORPHIC_RELATION(ButiEngine::IValuePtrRestoreObject, ButiEngine::ValuePtrRestoreObject< T >);\


