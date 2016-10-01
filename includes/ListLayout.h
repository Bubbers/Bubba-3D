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

#include <Layout.h>

/**
 * A layout to create a list of other layouts.
 */
class ListLayout : public Layout {
public:
    enum Orientation {HORIZONTAL,VERTICAL};

    /**
     * The child of a wrapping layout can only use
     * Dimension::WRAP or Dimension::PIXELS
     */
    virtual void addChild(Layout* child);

    virtual Dimension getWidth();
    virtual Dimension getHeight();

    /**
     * Creates a list where the children are placed after
     * each other in the specified \p orientation
     */
    ListLayout(Orientation orientation, Dimension width, Dimension height);
    virtual ~ListLayout() = default;
    virtual void getGLSquares(float layoutXPos,float layoutYPos, float layoutWidth,
                              float layoutHeight, std::map<std::string, IHudDrawable*> *map);

protected:
    Orientation orientation;
    Dimension width,height;

    unsigned int wrapSize(Orientation orientation);
    void checkChildCompatibility(Layout* child);
};
