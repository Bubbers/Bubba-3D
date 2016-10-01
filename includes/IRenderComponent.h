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
#ifndef BUBBA_3D_RENDERCOMPONENT_H
#define BUBBA_3D_RENDERCOMPONENT_H

#include "IComponent.h"
#include "ShaderProgram.h"


class IRenderComponent : public IComponent {
public:
    virtual void render() = 0;
    virtual void renderShadow(ShaderProgram *shaderProgram) = 0;

protected:
    ShaderProgram *shaderProgram;

};


#endif //BUBBA_3D_RENDERCOMPONENT_H
