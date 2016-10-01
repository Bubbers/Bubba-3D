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
#ifndef BUBBA_3D_DESTRUCTOR_UTILS_H
#define BUBBA_3D_DESTRUCTOR_UTILS_H


#include <vector>

template <class T>
void deleteLoop(std::vector<T> *v) {
    for (size_t n = 0; n < v->size(); ++n)
    {
        delete &(v[n]);
    }
    delete v;
}
#endif //BUBBA_3D_DESTRUCTOR_UTILS_H
