#pragma once
#include"../../GameComponentHeader.h"
#include"ButiRendering_Dx12/Header/Rendering_Dx12/CArrayBuffer_Dx12.h"
#include"ButiRendering_Dx12/Header/Rendering_Dx12/CBuffer_Dx12.h"


BUTI_REGIST_CBUFFER(ButiEngine::ButiRendering::TestGSVariable);


BUTI_REGIST_CBUFFER(ButiEngine::ButiRendering::ObjectInformation);
BUTI_REGIST_CBUFFER(ButiEngine::ButiRendering::GausVariable);

BUTI_REGIST_CBUFFER(ButiEngine::ButiRendering::ParticleParameter);
BUTI_REGIST_CARRAYBUFFER(ButiEngine::Matrix4x4);