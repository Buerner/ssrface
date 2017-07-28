//
//  Scene.cpp
//  SSRconnector
//
//  Created by Martin Bürner on 09.06.16.
//  Copyright © 2016 Martin Bürner. All rights reserved.
//

#include "Scene.hpp"

ssrface::Scene::Scene()
{
    _reference.name = "reference";
    _reference.orientation = 90.;
}

void ssrface::Scene::add_source( Source* new_source )
{
    // At this point it is not certain if the source indices in the first
    // transmission of the scene are ordered – thus a map.
    //printf("CREATING SOURCE. ID: %2i\n", new_source->id );
    
    // Check if source already exists before adding it.
    if ( !source_exists(new_source->id)) {
        _source_list[new_source->id] = *new_source;
    }
    
}

void ssrface::Scene::delete_source( unsigned short id )
{
    //printf( "DELETING SOURCE. ID:\t%i\n", id );
    _source_list.erase( id );
}

unsigned ssrface::Scene::get_n_sources()
{
    return unsigned(_source_list.size());
}

std::vector< unsigned short > ssrface::Scene::get_source_ids()
{
    std::vector< unsigned short > ids;
    for ( auto src = _source_list.begin(); src != _source_list.end(); src++ )
    {
        ids.push_back( src->first );
    }
    return ids;
}

void ssrface::Scene::clear()
{
    _source_list.clear();
}

ssrface::Source* ssrface::Scene::get_source( unsigned short id )
{
    if ( source_exists( id ) ) {
        return &(_source_list[id]);
    } else return nullptr;
}

ssrface::Source* ssrface::Scene::get_reference()
{
    return &_reference;
}

bool ssrface::Scene::source_exists( unsigned int id )
{
    if ( _source_list.count( id ) > 0 ) return true;
    else return false;
}

void ssrface::Scene::print()
{
    printf("--------- SCENE ---------\n");
    for ( auto iterator = _source_list.begin(); iterator != _source_list.end(); iterator++ ) {
        printf("ID: %2i \t NAME: %s\n", iterator->first, iterator->second.name.c_str());
    }
    printf("\n");
}

