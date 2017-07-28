    //
//  SceneParser.cpp
//  SSRconnector
//
//  Created by Martin Bürner on 07.06.16.
//  Copyright © 2016 Martin Bürner. All rights reserved.
//

#include "SceneParser.hpp"
#include <fstream>
#include <map>
#include <clocale>
#include <math.h>

ssrface::SceneParser::SceneParser( Scene* scene_ptr )
{
    _scene_ptr = scene_ptr;
    // TOOK ME 3 HOURS TO FIND OUT!!! Set locale in case of language with comma as deciaml seperator
    std::setlocale(LC_ALL, "C");
}

bool ssrface::SceneParser::_set_all_attributes( Source* source, rapidxml::xml_node<>* source_node )
{
    bool attribute_changed = false;
    
    // Loop through all source attributes stored directly in the source node.
    rapidxml::xml_attribute<>* attribute = source_node->first_attribute();
    while ( attribute )
    {
        attribute_changed |= _set_source_attribute( source, attribute);
        attribute = attribute->next_attribute();
        
    }
    
    // Loop through all attributed stored in subnodes of the source node.
    rapidxml::xml_node<>* attr_node = source_node->first_node();
    while ( attr_node )
    {
        attribute = attr_node->first_attribute();
        while ( attribute )
        {
            attribute_changed |= _set_source_attribute( source, attribute);
            attribute = attribute->next_attribute();
            
        }
        attr_node = attr_node->next_sibling();
    }
    
    return attribute_changed;
}

// Run the update on a SSR update message
int ssrface::SceneParser::parse_update( char* update_txt )
{   
    int new_status = SCENE_UNCHANGED;
    
    try {
        _doc.parse<0>(update_txt);
    }
    catch ( const rapidxml::parse_error& err )
    {
        printf( "SceneParser cannot parse XML message.\n\t%s\nScipping...\n", update_txt );
        _doc.clear();
        return SCENE_UNCHANGED;
    }
    
    rapidxml::xml_node<> * update_node;
    update_node = _doc.first_node("update");
    if ( update_node == nullptr )
    {
        _doc.clear();
        return SCENE_UNCHANGED;
    }
    if ( update_node->first_node() == nullptr )
    {
        _doc.clear();
        return SCENE_UNCHANGED;
    }
    
    rapidxml::xml_node<>* source_node;
    rapidxml::xml_node<>* delete_node;
    rapidxml::xml_attribute<>* attribute;
    
    // Check for changes in reference
    source_node = update_node->first_node("reference");
    if ( source_node != nullptr ) {
        if ( _set_all_attributes( _scene_ptr->get_reference(), source_node ) ) {
            new_status |= SCENE_REFERENCE_CHANGED;
        }
    }
    
    // Handle source delete.
    delete_node = update_node->first_node("delete");
    if ( delete_node != nullptr ) {
        source_node  = delete_node->first_node("source");
        if ( source_node != nullptr ) {
            unsigned short source_id = std::stoi(source_node->first_attribute("id")->value());
            if ( _scene_ptr->source_exists( source_id ) ) {
                _scene_ptr->delete_source( source_id );
                new_status |= SCENE_DELETED;
            }
        }
    }
    
    // Loop through all sources.
    source_node = update_node->first_node("source");
    while ( source_node != nullptr )
    {
        unsigned short source_id = std::stoi(source_node->first_attribute("id")->value());
        
        // Handle out of sync level messages.
        attribute = source_node->first_attribute()->next_attribute();
        if ( attribute != nullptr && !strcmp(attribute->name(), "level") ) {
            source_node = source_node->next_sibling();
            continue;
        }
        
        // Create new source if it does not exist.
        if ( !_scene_ptr->source_exists( source_id ) )
        {
            _create_source(source_node);
            new_status |= SCENE_NEW_SOURCES;
        }
        else {
            if( _set_all_attributes(_scene_ptr->get_source(source_id), source_node))
            {
                new_status |= SCENE_UPDATED;
            }
        }

        // go to next source
        source_node = source_node->next_sibling();
        
    }
    
    _doc.clear();
    
    return new_status;
}

// Called in case source does not yet exist
void ssrface::SceneParser::_create_source(rapidxml::xml_node<>* source_node)
{
    // Create a source.
    Source new_source;
    
    _set_all_attributes( &new_source, source_node );
    
    // Add new source to scene.
    _scene_ptr->add_source( &new_source );
}

// This is where all the casting of the parameters to source struct members happens.
bool ssrface::SceneParser::_set_source_attribute(Source* source, rapidxml::xml_attribute<>* attribute)
{
    switch ( param_map.find(attribute->name())->second ) {
        case id:
            source->id = std::stoi( attribute->value() );
            return true;
        case name:
            source->name = attribute->value();
            return true;
        case x:
            source->x = std::stof( attribute->value() );
            return true;
        case y:
            source->y = std::stof( attribute->value() );
            return true;
        case orientation:
            source->orientation = std::stof( attribute->value() ) * 0.01745329251994329;
            return false;
        case model:
            source->model = attribute->value();
            return true;
        case mute:
            strcmp( attribute->value(), "true" ) ? source->mute = false : source->mute = true;
            return true;
        case volume:
            source->volume = std::stof( attribute->value() );
            return true;
        case fixed:
            strcmp( attribute->value(), "true" ) ? source->fixed = false : source->fixed = true;
            return true;
        default:
            return false;
    }
}


