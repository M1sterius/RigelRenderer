#include "Renderable.hpp"

namespace rgr
{
    Renderable::Renderable(rgr::Mesh* mesh, rgr::Material* material) : Object()
    {
        m_Mesh = mesh;
        m_Material = material;
    }
    Renderable::~Renderable()
    {

    }

    rgr::Mesh* Renderable::GetMesh() const
    {
        return m_Mesh;
    }

    rgr::Material* Renderable::GetMaterial() const
    {
        return m_Material;
    }
}