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
