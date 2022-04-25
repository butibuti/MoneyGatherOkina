#include"stdafx.h"
#include "Resource_RealTimeMesh.h"

void ButiEngine::Resource_RealTimeMesh::SetBackupData(Value_ptr<ButiRendering::MeshPrimitiveBase> arg_vlp_meshPrimitive)
{
    vlp_meshPrimitive = arg_vlp_meshPrimitive;
    vertexType = vlp_meshPrimitive->GetVertexType();
}

std::uint32_t ButiEngine::Resource_RealTimeMesh::GetVertexCount()
{
    return verticesCount;
}

std::uint32_t ButiEngine::Resource_RealTimeMesh::GetIndexCount()
{
    return indexCount;
}


const ButiEngine::ButiRendering::MeshPrimitiveBase* ButiEngine::Resource_RealTimeMesh::GetBackUpdata(std::uint32_t arg_vertexType) const
{
    if (vertexType != arg_vertexType) {
        assert(0&&"àŸÇ»ÇÈí∏ì_å^ÇÃóvãÅÇ≈Ç∑");
        return nullptr;
    }
    return vlp_meshPrimitive.get();
}

ButiEngine::Value_ptr<ButiEngine::ButiRendering::MeshPrimitiveBase> ButiEngine::Resource_RealTimeMesh::GetBackupData_Row()
{
    return vlp_meshPrimitive;
}

bool ButiEngine::Resource_RealTimeMesh::GetPosRayCast(Vector3* arg_p_pos, Vector3* arg_p_normal, const Line& arg_line)
{
    return vlp_meshPrimitive->IsHitRay(arg_line, arg_p_pos,arg_p_normal);
}

const std::string& ButiEngine::Resource_RealTimeMesh::GetTagName() const
{
    return tagName;
}

void ButiEngine::Resource_RealTimeMesh::SetTagName(const std::string& arg_tagName)
{
    tagName = arg_tagName;
}

