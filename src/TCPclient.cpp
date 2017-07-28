//
//  newTCPClient.cpp
//  SSRconnector
//
//  Created by Martin Bürner on 08.06.16.
//  Copyright © 2016 Martin Bürner. All rights reserved.
//

#include "TCPclient.hpp"


ssrface::TCPClient::TCPClient(const char* host, unsigned short port, asio::io_service* ioservice)
{
    _ioservice = ioservice;
    _host = host;
    _port = port;
    
    _socket.reset( new tcp::socket( *_ioservice ) );
}

bool ssrface::TCPClient::connect()
{
    if ( !_connected && !_socket->is_open() ) {
        // Convert port to char.
        char ch_port[5];
        sprintf(ch_port, "%i", _port);
        
        // Create resolver and query.
        tcp::resolver resolver( *_ioservice );
        tcp::resolver::query query( _host, ch_port );
        
        // Try to resolve host addresse.
        try {
            _endpoint_iterator = resolver.resolve( query );
        }
        catch( std::exception ) {
            return false;
        }
        
        try {
            _socket->connect( *_endpoint_iterator );
            _connected = true;
            return true;
        }
        catch( std::exception ) {
            return false;
        }
    } else return true;
}

void ssrface::TCPClient::disconnect()
{
    _socket.reset( new tcp::socket( *_ioservice ) );
    _connected = false;
}

bool ssrface::TCPClient::is_connected()
{
    return _connected;
}

const char* ssrface::TCPClient::read()
{
    _read_buffer.consume( _n_read );
    
    asio::error_code error;
    
    // Read until line terminator.
    _n_read = asio::read_until( *_socket, _read_buffer, '\0', error );
    
    if ( error == asio::error::eof ) {
        disconnect();
        return nullptr; // Connection closed.
    }
    else if (error) {
        disconnect();
        return nullptr;
    }
    
    return asio::buffer_cast<const char*>(_read_buffer.data());
}

void ssrface::TCPClient::write( std::string const& message )
{
    if ( _connected ) {
        _socket->send( asio::buffer( message ) );
        _socket->send( asio::buffer( std::string{'\0'} ) );
    }
}

void ssrface::TCPClient::set_host( const char* new_host )
{
    _host = new_host;
}

void ssrface::TCPClient::set_port( unsigned short new_port )
{
    _port = new_port;
}
