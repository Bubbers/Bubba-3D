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
#pragma once

#include <string>
#include <GL/glew.h>

class CubeMapTexture
{
public:
    CubeMapTexture(const std::string& posXFilename, const std::string& negXFilename,
                   const std::string& posYFilename, const std::string& negYFilename,
                   const std::string& posZFilename, const std::string& negZFilename);

    ~CubeMapTexture();

    void load();
    void bind(GLenum textureUnit);

private:
    GLuint m_texture;

    void loadCubeMapFace(std::string filename, GLenum face);
};
