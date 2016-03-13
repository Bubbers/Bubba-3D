#ifndef BUBBA_3D_MATERIAL_H
#define BUBBA_3D_MATERIAL_H

#include "linmath/float3.h"
#include "Texture.h"

struct Material {
    chag::float3 diffuseColor;
    chag::float3 ambientColor;
    chag::float3 specularColor;
    chag::float3 emissiveColor;
    float specularExponent;
    Texture *diffuseTexture = NULL;
    Texture *bumpMapTexture = NULL;
};

#endif //BUBBA_3D_MATERIAL_H
