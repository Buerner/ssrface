//
//  Source.cpp
//  SSRconnector
//
//  Created by Martin Bürner on 09.06.16.
//  Copyright © 2016 Martin Bürner. All rights reserved.
//

#include "Source.hpp"

void ssrface::Source::print()
{
    printf( "ID:\t%i\n", id );
    printf( "name:\t%s\n", name.c_str() );
    printf( "position:\t(%f, %f)\n", x, y );
    printf( "volume:\t%f\n", volume );
    printf( "orientation:\t%f\n", orientation );
    printf( "fixed:\t%s\n", fixed ? "true": "false" );
    printf( "mute:\t%s\n", mute ? "true": "false" );
    printf( "model:\t%s\n", model.c_str() );
}
