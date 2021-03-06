#include "Scene/StageFileLoader.h"
#include "Util/MemoryUtil.h"
#include "Util/FileUtil.h"
#include "System/Galaxy/GalaxyStatusAccessor.h"
#include <stdio.h>

void StageFileLoader::startLoadingStageFile()
{
    s32 curZoneIdx = 0;

    while (mZoneNum > curZoneIdx)
    {
        MR::mountAsyncArchive(mNames[curZoneIdx], MR::getAproposHeapForSceneArchive(0.029999999));
        curZoneIdx++;
    }
}

void StageFileLoader::waitLoadedStageFile()
{
    s32 curZoneIdx = 0;

    while(mZoneNum > curZoneIdx)
    {
        const char* curFile = mNames[curZoneIdx];
        MR::recieveFile(curFile);
        mountFilesInStageMapFile(curFile);
        curZoneIdx++;
    }
}

void StageFileLoader::makeStageArchiveName(char *pOut, u32 len, const char *pStageName)
{
    snprintf(pOut, len, "/StageData/%s.arc", pStageName);
}