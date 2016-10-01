/*
 * This file is part of Bubba-3D.
 *
 * Bubba-3D is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Bubba-3D is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with Bubba-3D. If not, see http://www.gnu.org/licenses/.
 */
#ifndef BUBBA_3D_STANDARDRENDERER_H
#define BUBBA_3D_STANDARDRENDERER_H


#include "IRenderComponent.h"


class Mesh;
class ShaderProgram;
class GameObject;
class Chunk;

/**
 * \brief Renders meshes with the default shader
 *
 * Class is responsible for rendering a Mesh/GameObject using
 * the default shader simple.vert and simple.frag. 
 *
 */
class StandardRenderer : public IRenderComponent {
public:
    StandardRenderer();
    StandardRenderer(Mesh*, GameObject*, ShaderProgram*);

    void update(float dt);

    void render();
    void renderShadow(ShaderProgram *shaderProgram);
private:
    Mesh* mesh;
    GameObject *gameObject;
};


#endif //BUBBA_3D_STANDARDRENDERER_H
