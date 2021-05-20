#include "Map/Switch/StageSwitch.h"
#include "Scene/SceneObjHolder.h"
#include "Util/SceneUtil.h"

bool BitFlag128::get(int idx) const
{
    return (mFlags[idx / 32] & (1 << (idx % 32))) == 1 << (idx % 32);
}

void BitFlag128::set(int bit_index, bool setTrue)
{
    u32 valIdx = bit_index / 32;
    u32& flag = mFlags[valIdx];

    if (setTrue)
    {
        flag |= (1 << (bit_index % 32));
    }
    else
    {
        flag &= ~(1 << (bit_index % 32));
    }
}

ZoneSwitch::ZoneSwitch() : BitFlag128()
{
    s32 idx = 0;

    while (idx < 0x80)
    {
        set(idx, false);
    }
}

SwitchIdInfo::SwitchIdInfo(s32 switchID, const JMapInfoIter &rIter)
{
    mIDInfo = NULL;
    mIsGlobal = switchID >= 1000;
    mIDInfo = new JMapIdInfo(switchID, rIter);
}

s32 SwitchIdInfo::getSwitchNo() const
{
    if (mIsGlobal)
    {
        return mIDInfo->mData - 1000;
    }

    return mIDInfo->mData;
}

void StageSwitchContainer::createAndAddZone(const SwitchIdInfo &rInfo)
{
    if (findZoneSwitchFromTable(rInfo))
    {
        return;
    }

    ZoneSwitch* s = new ZoneSwitch();
    ContainerSwitch sw;
    sw.mData = rInfo.mIDInfo->mZoneId;
    sw.mSwitch = s;
    mSwitches[mCount++] = sw;
}

ZoneSwitch* StageSwitchContainer::getZoneSwitch(const SwitchIdInfo &rInfo)
{
    if (rInfo.mIsGlobal)
    {
        return mGlobalSwitches;
    }

    return findZoneSwitchFromTable(rInfo);
}

ZoneSwitch* StageSwitchContainer::findZoneSwitchFromTable(const SwitchIdInfo &rInfo)
{
    for (int i = 0; i < mCount; i++)
    {
        ContainerSwitch* s = &mSwitches[i];

        if (s->mData == rInfo.mIDInfo->mZoneId)
        {
            return s->mSwitch;
        }
    }

    return NULL;
}

void StageSwitchCtrl::onSwitchA()
{
    StageSwitchFunction::onSwitchBySwitchIdInfo(*mSW_A);
}

void StageSwitchCtrl::offSwitchA()
{
    StageSwitchFunction::offSwitchBySwitchIdInfo(*mSW_A);
}

bool StageSwitchCtrl::isOnSwitchA() const
{
    return StageSwitchFunction::isOnSwitchBySwitchIdInfo(*mSW_A);
}

bool StageSwitchCtrl::isValidSwitchA() const
{
    return mSW_A != 0;
}

void StageSwitchCtrl::onSwitchB()
{
    StageSwitchFunction::onSwitchBySwitchIdInfo(*mSW_B);
}

void StageSwitchCtrl::offSwitchB()
{
    StageSwitchFunction::offSwitchBySwitchIdInfo(*mSW_B);
}

bool StageSwitchCtrl::isOnSwitchB() const
{
    return StageSwitchFunction::isOnSwitchBySwitchIdInfo(*mSW_B);
}

bool StageSwitchCtrl::isValidSwitchB() const
{
    return mSW_B != 0;
}

bool StageSwitchCtrl::isOnSwitchAppear() const
{
    return StageSwitchFunction::isOnSwitchBySwitchIdInfo(*mSW_Appear);
}

bool StageSwitchCtrl::isValidSwitchAppear() const
{
    return mSW_Appear != 0;
}

void StageSwitchCtrl::onSwitchDead()
{
    StageSwitchFunction::onSwitchBySwitchIdInfo(*mSW_Dead);
}

void StageSwitchCtrl::offSwitchDead()
{
    StageSwitchFunction::offSwitchBySwitchIdInfo(*mSW_Dead);
}

bool StageSwitchCtrl::isValidSwitchDead() const
{
    return mSW_Dead != 0;
}

// todo -- literally cannot get the proper registers for the life of me
bool StageSwitchCtrl::isOnAllSwitchAfterB(int idx) const
{
    for (s32 i = 0; i < idx; i++)
    {
        s32 switchNo = i + mSW_B->getSwitchNo();
        SwitchIdInfo* wtf = mSW_B;
        SceneObjHolder* objHolder = MR::getSceneObjHolder();
        StageSwitchContainer* container = static_cast<StageSwitchContainer*>(objHolder->getObj(0xA));
        if (!container->getZoneSwitch(*wtf)->get(switchNo))
        {
            return false;
        }
    }

    return true;
}

// same for this one
bool StageSwitchCtrl::isOnAnyOneSwitchAfterB(int idx) const
{
    for (s32 i = 0; i < idx; i++)
    {
        s32 switchNo = i + mSW_B->getSwitchNo();
        SwitchIdInfo* wtf = mSW_B;
        SceneObjHolder* objHolder = MR::getSceneObjHolder();
        StageSwitchContainer* container = static_cast<StageSwitchContainer*>(objHolder->getObj(0xA));
        if (!container->getZoneSwitch(*wtf)->get(switchNo))
        {
            return true;
        }
    }

    return false;
}

StageSwitchCtrl::StageSwitchCtrl(const JMapInfoIter &rIter)
    : mSW_A(NULL), mSW_B(NULL), mSW_Appear(NULL), mSW_Dead(NULL)
{
    mSW_A = StageSwitchFunction::createSwitchIdInfo("SW_A", rIter, true);
    mSW_B = StageSwitchFunction::createSwitchIdInfo("SW_B", rIter, true);
    mSW_Appear = StageSwitchFunction::createSwitchIdInfo("SW_APPEAR", rIter, true);
    mSW_Dead = StageSwitchFunction::createSwitchIdInfo("SW_DEAD", rIter, true);
}

SwitchIdInfo* StageSwitchFunction::createSwitchIdInfo(const char *pSwitchName, const JMapInfoIter &rIter, bool unused)
{
    s32 switchNo;
    bool res = rIter.getValue<s32>(pSwitchName, &switchNo);
    
    if (!res)
    {
        return NULL;
    }

    if (switchNo < 0)
    {
        return NULL;
    }

    SwitchIdInfo* inf = new SwitchIdInfo(switchNo, rIter);

    if (switchNo < 0x3E8)
    {
        SceneObjHolder* objHolder = MR::getSceneObjHolder();
        StageSwitchContainer* container = static_cast<StageSwitchContainer*>(objHolder->getObj(0xA));
        container->createAndAddZone(*inf); 
    }

    return inf;
}

void StageSwitchFunction::onSwitchBySwitchIdInfo(const SwitchIdInfo &rSwitchId)
{
    s32 switchNo = rSwitchId.getSwitchNo();
    SceneObjHolder* objHolder = MR::getSceneObjHolder();
    StageSwitchContainer* container = static_cast<StageSwitchContainer*>(objHolder->getObj(0xA));

    ZoneSwitch* zoneSwitch = container->getZoneSwitch(rSwitchId);

    if (zoneSwitch->get(switchNo))
    {
        zoneSwitch->set(switchNo, true);
    }
}

void StageSwitchFunction::offSwitchBySwitchIdInfo(const SwitchIdInfo &rSwitchId)
{
    s32 switchNo = rSwitchId.getSwitchNo();
    SceneObjHolder* objHolder = MR::getSceneObjHolder();
    StageSwitchContainer* container = static_cast<StageSwitchContainer*>(objHolder->getObj(0xA));

    ZoneSwitch* zoneSwitch = container->getZoneSwitch(rSwitchId);

    if (zoneSwitch->get(switchNo))
    {
        zoneSwitch->set(switchNo, false);
    }
}

bool StageSwitchFunction::isOnSwitchBySwitchIdInfo(const SwitchIdInfo &rSwitchId)
{
    s32 switchNo = rSwitchId.getSwitchNo();
    SceneObjHolder* objHolder = MR::getSceneObjHolder();
    StageSwitchContainer* container = static_cast<StageSwitchContainer*>(objHolder->getObj(0xA));

    ZoneSwitch* zoneSwitch = container->getZoneSwitch(rSwitchId);

    return zoneSwitch->get(switchNo);
}