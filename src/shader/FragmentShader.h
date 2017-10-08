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
//
// Created by johan on 2016-03-12.
//

#ifndef SUPER_BUBBA_AWESOME_SPACE_FRAGMENTSHADER_H
#define SUPER_BUBBA_AWESOME_SPACE_FRAGMENTSHADER_H

#include <IShader.h>

/**
* \brief Compiles Fragment Shaders
*
* Class that takes a fragment shader string and compiles it.
* It also checks for any possible errors that occured during compilation.
*
*/
class FragmentShader : public IShader{
public:
    FragmentShader() {};
    FragmentShader(std::string shaderName);

    virtual void compile();
    virtual void checkErrors(GLuint &shader);
    virtual GLuint getGLId();

private:
    std::string shaderString;
    GLuint fragmentShader;
};


#endif //SUPER_BUBBA_AWESOME_SPACE_FRAGMENTSHADER_H
