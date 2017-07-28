//
//  TCPClient.hpp
//  SSRconnector
//
//  Created by Martin Bürner on 08.06.16.
//  Copyright © 2016 Martin Bürner. All rights reserved.
//

#ifndef TCPClient_hpp
#define TCPClient_hpp

#ifndef ASIO_STANDALONE
#define ASIO_STANDALONE
#endif

#include <stdio.h>
#include <string>
#include <memory>
#include <asio.hpp>

namespace ssrface {

using asio::ip::tcp;

/**
 * @class TCPClient
 * @brief Simple client for TCP Socket servers.
 */
class TCPClient
{
public:
    /**
     * @param host IP adress of host server.
     * @param port Port of host server.
     * @param ioservice Asio io_service instance.
     */
    TCPClient( const char* host, unsigned short port, asio::io_service* ioservice );
    
    /**
     * @brief Tries to connect to host and port currently set.
     * @returns Whether connection attempt was successful or not.
     */
    bool connect();
    
    /** @brief If a connection is currently active this function shuts it down. */
    void disconnect();
    
    /** @returns Whether a connection is currently active. */
    bool is_connected();
    
    /**
     * @brief Read message from host (Only if connection was successful).
     * @returns Pointer to the char array containing the message.
     */
    const char* read();
    
    /**
     * @brief Send message to the host server
     * @param message Message to be transmitted to the server.
     */
    void write( std::string const& message );
    
    /**
     * @brief Change address of host server. This does not affect an active connection.
     * @param new_host New IP adress.
     */
    void set_host( const char* new_host );
    
    /**
     * @brief Change port of host server. This does not affect an active connection.
     * @param new_port New port.
     */
    void set_port( unsigned short new_port );
    
private:
    asio::io_service* _ioservice;
    std::unique_ptr< tcp::socket > _socket;
    tcp::resolver::iterator _endpoint_iterator;
    bool _connected = false;
    size_t _n_read = 0;
    
    unsigned short _port;
    std::string _host;
    const char _eol_char = '\0';
    asio::streambuf _read_buffer;
};

} // Namespace ssrface

#endif /* TCPClient_hpp */
