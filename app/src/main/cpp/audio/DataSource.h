//
// Created by 43975 on 12/24/2021.
//

#ifndef OBOE_AUDIO_PLAYER_DATASOURCE_H
#define OBOE_AUDIO_PLAYER_DATASOURCE_H

#include "AudioProperties.h"

class DataSource{
public:
    virtual ~DataSource(){}
    virtual int64_t getSize() const =0;

    virtual AudioProperties getProperties() const =0;
    virtual const float* getData() const =0;
};

#endif //OBOE_AUDIO_PLAYER_DATASOURCE_H
