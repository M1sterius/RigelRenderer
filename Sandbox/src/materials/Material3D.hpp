#pragma once

#include "core/components/Material.hpp"
#include "core/components/Texture.hpp"

class Material3D : public rgr::Material
{
public:
	Material3D(rgr::Texture* texture, rgr::Shader* shader);
	~Material3D() override;

	void SetUniforms() override;
private:
	rgr::Texture* m_Texture;
};

