//
//  main.cpp
//  ssrface
//
//  Created by Martin Bürner on 30.11.16.
//  Copyright © 2016 Martin Bürner. All rights reserved.
//

#include "SceneManager.hpp"

void print_scene( ssrface::Scene* scene_ptr, void* )
{
    auto ids = scene_ptr->get_source_ids();
    scene_ptr->print();
}

int main()
{
    ssrface::SceneManager manager;
    
    // Set address for SSR running on localhost.
    manager.set_ssr_address( "127.0.0.1", 4711 );
    manager.set_new_source_callback( print_scene, NULL );
    
    manager.connect();
    
    sleep(1);
    
    // Performe some actions in case connection was successfull.
    if ( manager.is_connected() )
    {
        manager.run();
        manager.clear_scene();
        
        manager.setup_new_source( "Alice", 1.f, 1.f, false );
        sleep(1);
        manager.turn_reference(0);
        sleep(1);
        manager.turn_reference(180);
        manager.setup_new_source( "Bob", 0.f, -1.f, false );
        sleep(1);
        manager.move_source(1, -1, 1);
        sleep(1);
        
        manager.clear_scene();
        manager.disconnect();
    }
    else {
        printf("Unable to connect to SSR.\n");
    }
    
    return 0;
}
