//
//  SceneManager.hpp
//  SSRconnector
//
//  Created by Martin Bürner on 09.06.16.
//  Copyright © 2016 Martin Bürner. All rights reserved.
//

#ifndef SceneManager_hpp
#define SceneManager_hpp

#include <atomic>
#include <vector>
#include <memory>
#include <mutex>
#include <thread>

#include "TCPclient.hpp"
#include "Source.hpp"
#include "Scene.hpp"
#include "SceneParser.hpp"

namespace ssrface {

/**
 @class SceneManager
 @brief Manages connection to <a href="http://spatialaudio.net/ssr/">SSR</a>, stores Scene, offers modification methods for Scene.
 
 This class takes care of using the Network Interface of the SoundScape Renderer. It handles the connection, the parsing of the XML messages sent by the SSR and stores state of the audio scene. Furthermore it offeres multiple methods enabling modifications of sources and reference in the Scene.
 */
class SceneManager
{
public:
    typedef void (*UpdateCallback)(Scene* scene_ptr, void* io_data);

    
    SceneManager();
    ~SceneManager();
    
    /** 
     @brief Try to connect to SoundScape Renderer.
     @returns Whether or not the connection attempt was successful.
     */
    virtual bool connect();
    
    /** @brief Disconnects the client if there is an active conneciton. */
    void disconnect();
    
    /** @brief Starts an update loop which keeps the Scene up to date. Executes Callbacks on Scene Changes. */
    void run();
    
    /** @brief Stopps the update process in case it is active. */
    void stop();
    
    /** @returns Whether or not the update process loop is currently running. */
    bool is_running();
    
    /** @returns Whether a connection to the SoundScape Renderer is active */
    bool is_connected();
    
    /** @returns Number of sources which are currently stored in the Scene */
    unsigned short get_n_sources();
    
    Scene* get_scene();
    
    /**
     @brief Function which is called when there was a change in the audio Scene.
     @param callback_func Function pointer. Must comply with UpdateCallback.
     @param io_data The callback function is called with this pointer as second argument.
     */
    void set_update_callback( UpdateCallback callback_func, void* io_data );
    
    /**
     @brief Function which is called when a new source entered the audio Scene.
     @param callback_func Function pointer. Must comply with UpdateCallback.
     @param io_data The callback function is called with this pointer as second argument.
     */
    void set_new_source_callback( UpdateCallback callback_func, void* io_data );
    
    /**
     @brief Function which is called when there a source was deleted in the scene.
     @param callback_func Function pointer. Must comply with UpdateCallback.
     @param io_data The callback function is called with this io data pointer as second argument.
     */
    void set_deleted_callback( UpdateCallback callback_func, void* io_data );
    
    /**
     @brief Function which is called when changes have been made to the reference positon.
     @param callback_func Function pointer. Must comply with UpdateCallback.
     @param io_data The callback function is called with this io data pointer as second argument.
     */
    void set_reference_callback( UpdateCallback callback_func, void* io_data );
    
    /**
     @brief Sends request for a new Source to SSR.
     @param name Desired name of the Source.
     @param x_pos Desired position in x dimesion.
     @param y_pos Desired position in y dimesion.
     @param fixed Wheter or not the source position is fixed.
     */
    void setup_new_source( const char* name, float x_pos, float y_pos, bool fixed = false );
    
    /**
     @brief Send the request to move a Source to SSR.
     @param id ID of the Source to be moved.
     @param x_pos Desired position in x dimesion.
     @param y_pos Desired position in y dimesion.
     */
    void move_source( unsigned id, float x_pos, float y_pos );
    
    /**
     @brief Send the request to remove a Source to the SSR.
     @param id ID of the Source to be deleted.
     */
    void delete_source( unsigned id );
    
    /**
     @brief Send a request to set the fixed status of a Source to SSR.
     @param id ID of the source to be changed.
     @param fixed New desired status of the Source.
     */
    void fix_source( unsigned id, bool fixed );
    
    /**
     @brief Send a request to change the position of the reference to SSR.
     @param x_pos Desired position in x dimesion.
     @param y_pos Desired position in y dimesion.
     */
    void move_reference( float x_pos, float y_pos );
    
    /**
     @brief Send a request to change the orientation of the reference to SSR.
     @param azimuth Desired viewing direction in horizontal plane.
     */
    void turn_reference( float azimuth );
    
    /** @brief Send request to remove all Sources to SSR. */
    void clear_scene();

    /**
     @brief Set IP address and port of SSR. Does not affect active connection.
     @param address New IP address of SSR network interface server.
     @param port port address of SSR network interface server.
     */
    void set_ssr_address( const char* address, unsigned port );
    
    /**
     @brief Set the master volumen in SSR.
     @param gain in dB from -49 to 12;
     */
    void set_volume( float gain );
    
private:
    asio::io_service _ioservice;
    TCPClient _client;
    Scene _audio_scene;
    SceneParser _parser;
    std::atomic<bool> _running{false};
    std::thread _run_thread;
    
    char _message_buffer[2048];
    
    std::vector< void* > _update_io_data;
    std::vector< UpdateCallback > _update_callbacks;
    
    std::vector< void* > _new_source_io_data;
    std::vector< UpdateCallback > _new_source_callbacks;
    
    std::vector< void* > _deleted_io_data;
    std::vector< UpdateCallback > _deleted_callbacks;
    
    std::vector< void* > _reference_io_data;
    std::vector< UpdateCallback > _reference_callbacks;
    
//    std::vector< void* > _reference_io_data;
//    std::vector< UpdateCallback > _reference_callbacks;
    
    void _update();
    void _clear_message_buffer();
    
    const float SSR_MAX_GAIN = 12.f;
    const float SSR_MIN_GAIN = -49.f;
    
    std::mutex _mtx;
    
};

} // Namespace ssrface

#endif /* SceneManager_hpp */
