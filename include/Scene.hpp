//
//  Scene.hpp
//  SSRconnector
//
//  Created by Martin Bürner on 09.06.16.
//  Copyright © 2016 Martin Bürner. All rights reserved.
//

#ifndef Scene_hpp
#define Scene_hpp

#include <stdio.h>
#include <vector>
#include "Source.hpp"

namespace ssrface {

const int SCENE_UNCHANGED = 0x00;
const int SCENE_UPDATED = 0x01;
const int SCENE_NEW_SOURCES = 0x02;
const int SCENE_DELETED = 0x04;
const int SCENE_REFERENCE_CHANGED = 0x08;

/**
 * @class Scene
 * @brief Class which stores a SoundScape Renderer scene and has some helpful funtionalites.
 */
class Scene
{
public:
    Scene();
    /**
     * @brief Add a new source to the scene.
     * @param new_source Pointer to the instance of the source to be added.
     */
    void add_source(Source* new_source);
    
    /**
     * @brief Removes a source from the scene.
     * @param id ID of the source to be deleted. Nothing happens if ID does not exist.
     */
    void delete_source( unsigned short id );
    
    /** @brief Removes all the sources form the scene. */
    void clear();
    
    /** @returns Number of sources which are currently stored in Scene instance. */
    unsigned get_n_sources();
    
    /**
     * @brief Returns pointer a source in the Scene if the ID exists.
     * @param id ID of requested source.
     */
    Source* get_source( unsigned short id );
    
    /** @return Pointer to Scene reference position and orientation. Also stored as Source instace. */
    Source* get_reference();
    
    /**
     * @returns Wheather or not a source with the passed ID is currently stored in Scene instance.
     * @param id Source ID to be checked for existance.
     */
    bool source_exists( unsigned id );
    
    /** @returns Vector with IDs of all source currently stored in Scene instance. */
    std::vector< unsigned short > get_source_ids();
    
    /** @brief Prints list of sources (ID and name) of currently stored Sources. */
    void print();
    
private:
    // Data structure ist still a nightmare: All source IDs can be received,
    // and all sources of this map can be accessed via get_source().
    std::map<unsigned short, Source> _source_list;
    Source _reference;

};

} // Namespace ssrface

#endif /* Scene_hpp */
