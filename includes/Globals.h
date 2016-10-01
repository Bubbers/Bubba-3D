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
// Created by simon on 2016-01-09.
//

#ifndef BUBBA_3D_GLOBALS_H
#define BUBBA_3D_GLOBALS_H

class Globals{
public:
    enum Key { WINDOW_HEIGHT=0, WINDOW_WIDTH=1,MOUSE_WINDOW_X=2,MOUSE_WINDOW_Y=3, FONT_TEXTURE_WIDTH=4, FONT_TEXTURE_HEIGHT=5};
    static void set(Key key, int value);
    static int get(Key key);

private:
    static int masterFunc(Key key, int value, bool getElseSet);

};

#endif //BUBBA_3D_GLOBALS_H
