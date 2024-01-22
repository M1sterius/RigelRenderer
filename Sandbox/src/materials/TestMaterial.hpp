#pragma once

#include "core/components/Material.hpp"
#include "core/components/Texture.hpp"

#include <string>

class TestMaterial : public rgr::Material
{
public:
	TestMaterial(rgr::Texture* texture);
	void SetUniforms() override;
	~TestMaterial() override;
private:
	rgr::Texture* m_Texture;
};
