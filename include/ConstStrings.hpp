#pragma once
#include "beatsaber-hook/shared/utils/typedefs.h"

#ifndef CONST_STRING
#define CONST_STRING(identifier)                    \
    static StringW identifier()                     \
    {                                               \
        static ConstString identifier(#identifier); \
        return identifier;                          \
    }
#endif

namespace ConstStrings
{
    CONST_STRING(ObstacleFakeGlow);
    CONST_STRING(HideWrapper);
    CONST_STRING(ObstacleCore);
    CONST_STRING(DepthWrite);
    CONST_STRING(ObstacleFrame);
    CONST_STRING(Frame);
    CONST_STRING(Core);
}