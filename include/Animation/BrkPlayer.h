#pragma once

#include "Animation/AnmPlayer.h"
#include "J3DGraphAnimator/J3DModel.h"
#include "System/Resource/ResourceHolder.h"

class BrkPlayer : public MaterialAnmPlayerBase
{
public:
    BrkPlayer(const ResourceHolder *, J3DModelData *);

    virtual void attach(J3DAnmBase *, J3DModelData *);
    virtual void detach(J3DAnmBase *, J3DModelData *);
};