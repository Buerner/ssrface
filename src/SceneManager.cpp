//
//  SceneManager.cpp
//  SSRconnector
//
//  Created by Martin Bürner on 09.06.16.
//  Copyright © 2016 Martin Bürner. All rights reserved.
//

#include "SceneManager.hpp"
#include <stdio.h>

ssrface::SceneManager::SceneManager() :
    _client( "localhost", 4711, &_ioservice ),
    _parser( &_audio_scene )
{

}

ssrface::SceneManager::~SceneManager()
{
    stop();
    disconnect();
}

bool ssrface::SceneManager::connect()
{
    return _client.connect();
}

void ssrface::SceneManager::disconnect()
{
    if ( is_running() ) stop();
    _client.disconnect();
    _audio_scene.clear();
}

bool ssrface::SceneManager::is_connected()
{
    return _client.is_connected();
}

void ssrface::SceneManager::_update()
{    
    int scene_status;
    while ( _running.load() && _client.is_connected() ) {
        
        const char* message = _client.read();
        //printf("Parsing message:\n%s\n", message);
        // only call parser if a message was received.
        if ( message != nullptr )
        {
            //printf("Parsing message:\n%s\n", message);
            scene_status =_parser.parse_update( (char*)message );

            //printf("STATUS: %i \n", scene_status);
            
            _mtx.lock();
            // Execute callbacks in case changes have been made.
            if ( scene_status & SCENE_UPDATED ) {
                for ( unsigned idx = 0; idx < _update_callbacks.size(); idx++) {
                    _update_callbacks[idx]( &_audio_scene, _update_io_data[idx] );
                }
            }

            // Execute callbacks in case new sources have been created.
            if ( scene_status & SCENE_NEW_SOURCES ) {
                for ( unsigned idx = 0; idx < _new_source_callbacks.size(); idx++) {
                    _new_source_callbacks[idx]( &_audio_scene, _new_source_io_data[idx] );
                }
            }
            
            // Execute callbacks in case sources have been deleted.
            if ( scene_status & SCENE_DELETED ) {
                for ( unsigned idx = 0; idx < _deleted_callbacks.size(); idx++) {
                    _deleted_callbacks[idx]( &_audio_scene, _deleted_io_data[idx] );
                }
            }
            
            // Execute callbacks in the reference changed.
            if ( scene_status & SCENE_REFERENCE_CHANGED ) {
                for ( unsigned idx = 0; idx < _reference_callbacks.size(); idx++) {
                    _reference_callbacks[idx]( &_audio_scene, _reference_io_data[idx] );
                }
            }
            _mtx.unlock();

        }
    }
    _running.store( false );
    printf("SceneManager update process stopped.\n");
    
}

/* This functions starts a thread keeps the audio scene updated */
void ssrface::SceneManager::run()
{
    if ( !is_running() && is_connected() ) {
        _running.store(true);
        _run_thread = std::thread{ &ssrface::SceneManager::_update, this };
        printf("SceneManager update process started.\n");
        printf("\tUpdate functions: %lu\n", _update_callbacks.size());
        printf("\tNewSrc functions: %lu\n", _new_source_callbacks.size());
        printf("\tDelSrc functions: %lu\n", _deleted_callbacks.size());
        printf("\tRefere functions: %lu\n", _reference_callbacks.size());
    }
    
}

bool ssrface::SceneManager::is_running()
{
    return _running.load();
}

void ssrface::SceneManager::stop()
{
    if ( _running.load() ) {
        _running.store( false );
        // Wait for update thread to stop.
        _run_thread.join();
    }
}

unsigned short ssrface::SceneManager::get_n_sources()
{
    return _audio_scene.get_n_sources();
};


void ssrface::SceneManager::set_update_callback( UpdateCallback callback_func, void* io_data )
{
    _mtx.lock();
    _update_callbacks.push_back( callback_func );
    _update_io_data.push_back( io_data );
    _mtx.unlock();
}

void ssrface::SceneManager::set_new_source_callback( UpdateCallback callback_func, void* io_data )
{
    _mtx.lock();
    _new_source_callbacks.push_back( callback_func );
    _new_source_io_data.push_back( io_data );
    _mtx.unlock();
}

void ssrface::SceneManager::set_deleted_callback( UpdateCallback callback_func, void* io_data )
{
    _mtx.lock();
    _deleted_callbacks.push_back( callback_func );
    _deleted_io_data.push_back( io_data );
    _mtx.unlock();
}

void ssrface::SceneManager::set_reference_callback( UpdateCallback callback_func, void* io_data )
{
    _mtx.lock();
    _reference_callbacks.push_back( callback_func );
    _reference_io_data.push_back( io_data );
    _mtx.unlock();
}

void ssrface::SceneManager::setup_new_source( const char* name, float x_pos, float y_pos, bool fixed )
{
    sprintf( _message_buffer, "<request><source new=\"true\" port=\"%s\" name=\"%s\"><position x=\"%f\" y=\"%f\" fixed=\"%s\" /></source></request>", name, name, x_pos, y_pos, fixed ? "true" : "false" );
    _client.write( std::string( _message_buffer ) );
}

void ssrface::SceneManager::move_source( unsigned id, float x_pos, float y_pos )
{

    sprintf( _message_buffer, "<request><source id=\"%i\"><position x=\"%f\" y=\"%f\" /></source></request>", id, x_pos, y_pos );
    _client.write( std::string( _message_buffer ) );
}

void ssrface::SceneManager::delete_source( unsigned int id )
{
    sprintf( _message_buffer, "<request><delete><source id=\"%i\"/></delete></request>", id );
    _client.write( std::string( _message_buffer ) );
}

void ssrface::SceneManager::fix_source( unsigned int id, bool fixed )
{
    sprintf( _message_buffer, "<request><source id=\"%i\"><position fixed=\"%s\"/></source></request>", id, fixed ? "true" : "false" );
    
    _client.write( std::string( _message_buffer ) );
}

void ssrface::SceneManager::move_reference( float x_pos, float y_pos )
{
    sprintf( _message_buffer, "<request><reference><position x=\"%f\" y=\"%f\"/></reference></request>", x_pos, y_pos );
    _client.write( std::string( _message_buffer ) );
}

void ssrface::SceneManager::turn_reference( float azimuth )
{
    sprintf( _message_buffer, "<request><reference><orientation azimuth=\"%f\"/></reference></request>", azimuth );
    _client.write( std::string( _message_buffer ) );
}

void ssrface::SceneManager::clear_scene()
{
    sprintf( _message_buffer, "<request><scene clear=\"true\"/></request>" );
    _client.write( std::string( _message_buffer ) );
    _audio_scene.clear();
}

void ssrface::SceneManager::set_ssr_address( const char* address, unsigned port )
{
    _client.set_host( address );
    _client.set_port( port );
}

void ssrface::SceneManager::set_volume( float gain )
{
    if ( gain > SSR_MAX_GAIN ) gain = SSR_MAX_GAIN;
    else if ( gain < SSR_MIN_GAIN ) gain = SSR_MIN_GAIN;
    sprintf( _message_buffer, "<request><scene volume=\"%f\"/></request>", gain );
    _client.write( std::string(_message_buffer) );
}

ssrface::Scene* ssrface::SceneManager::get_scene()
{
    return &_audio_scene;
}


