//
// Created by johan on 2016-03-12.
//

#ifndef SUPER_BUBBA_AWESOME_SPACE_ISHADER_H
#define SUPER_BUBBA_AWESOME_SPACE_ISHADER_H

class IShader {
public:
    virtual void compile() = 0;
    virtual void checkErrors() = 0;
};

#endif //SUPER_BUBBA_AWESOME_SPACE_ISHADER_H
