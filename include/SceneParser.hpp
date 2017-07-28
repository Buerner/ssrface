//
//  SceneParser.hpp
//  SSRconnector
//
//  Created by Martin Bürner on 07.06.16.
//  Copyright © 2016 Martin Bürner. All rights reserved.
//

#ifndef SceneParser_hpp
#define SceneParser_hpp

#include "../rapidxml-1.13/rapidxml.hpp"
#include <string>
#include "Scene.hpp"
#include "TCPclient.hpp"

namespace ssrface {

/**
 @class SceneParser
 @brief Parses ADSF XML Messages form SSR to Scene format.
 
 Parses Audio Scene Desription XML Messages form the TCP/IP Network Socket interface of the SoundScape Renderer to the Scene and Source format. Workes with <a href="http://rapidxml.sourceforge.net/">RapidXml</a>.
 */
class SceneParser
{
public:
    /** @param scene_ptr Pointer to the Scene instance the Parser writes to. */
    SceneParser( Scene* scene_ptr );
    
    /**
     @brief Parses a message.
     @param update_txt to char array containing the XML Message.
     @returns Status of the Scene after parsing (defined in Scene.hpp).
     */
    int parse_update( char* update_txt );
    
private:
    rapidxml::xml_document<> _doc;
    Scene* _scene_ptr;
    
    void _create_source( rapidxml::xml_node<>* source_node );
    bool _set_source_attribute( Source* source, rapidxml::xml_attribute<>* attribute );
    bool _set_all_attributes( Source* source, rapidxml::xml_node<>* node );
};

} // Namespace ssrface

#endif /* SceneParser_hpp */
