//
// Created by 43975 on 12/24/2021.
//
#include "../utils/logging.h"
#include "NDKExtractor.h"

int32_t NDKExtractor::decode(int *asset, int *targetData, int targetProperties) {
    LOGD("Using NDK decoder");

    // open asset as file descriptor
    off_t start, length;
    int fd = Asset_openFileDescriptor(asset,&start,&length);

    //Extract the audio frames
    AMediaExtractor *extractor = AMediaExtractor_new();
    media_status_t amresult = AMediaExtractor_setDataSourceFd(extractor, fd, static_cast<off64_t>(start),
                                                              static_cast<off64_t>(length));
    if (amresult != AMEDIA_OK){
        LOGE("Error setting extractor data source, err %d", amresult);
        return 0;
    }


}
