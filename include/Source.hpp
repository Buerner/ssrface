//
//  Source.hpp
//  SSRconnector
//
//  Created by Martin Bürner on 09.06.16.
//  Copyright © 2016 Martin Bürner. All rights reserved.
//

#ifndef Source_hpp
#define Source_hpp

#include <string>
#include <map>
#include <math.h>

namespace ssrface {

enum param_idcs {
    name,
    id,
    x,
    y,
    orientation,
    model,
    mute,
    volume,
    fixed,
    n_params
};


const std::map<std::string, param_idcs> param_map =
{
    { "name",    param_idcs::name },
    { "id",      param_idcs::id },
    { "x",       param_idcs::x },
    { "y",       param_idcs::y },
    { "azimuth", param_idcs::orientation },
    { "model",   param_idcs::model },
    { "mute",    param_idcs::mute },
    { "volume",  param_idcs::volume },
    { "fixed",   param_idcs::fixed }
};


/**
 @struct Source
 @brief This struct stores all the attributes of a source object in the SoundScape Renderer.
 */
struct Source {
    unsigned short id;
    std::string name{"Bob"};
    float x = 0.f;
    float y = 0.f;
    float orientation = 0.f;
    std::string model = "point";
    bool mute = false;
    float volume = 1.f;
    bool fixed = false;
    
    float distance_to( Source* src )
    {
        return sqrtf( powf(x - src->x, 2.f) + powf(y - src->y, 2.f) );
    }
    
    float angle_to( Source* src )
    {
        float delta_x = src->x - x;
        float delta_y = src->y - y * M_PI;
        
        return atanf( delta_y / delta_x ) - orientation;
    }
    
    void print();
};

} // Namespace ssrface

#endif /* Source_hpp */
