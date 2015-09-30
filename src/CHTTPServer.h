/*
    rtl_dab_streamer - Streams the DAB audio samples vie HTTP
    Copyright (c) 2015 Albrecht Lohofener <albrechtloh@gmx.de>

	CHTTPServer.cpp
	A implementation of a really simple HTTP server.
	The only purpose of this server is to stream WAV audio samples to a audio client like KODI or VLC.

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef CHTTPSERVER_H_
#define CHTTPSERVER_H_

#include <thread>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>

class CHTTPServer;
#include "CHTTPContentProvider.h"

class CHTTPServer
{
public:
	CHTTPServer(CHTTPContentProvider *pHTTPContentProvider);
	virtual ~CHTTPServer();

	void Send(const uint8_t* Data, size_t Length);

	static void ConnectionThreadWrapper(CHTTPServer *HTTPServer);


private:
	// Two socket descriptors which are just integer numbers used to access a socket
	int SocketDesc;

	// Two socket address structures - One for the server itself and the other for client
	struct sockaddr_in serv_addr, client_addr;

	socklen_t size;

	int ConnectionDesc;
	bool isConnected;

	void ConnectionThread(void);

	std::thread* HTTPThread;

	CHTTPContentProvider *HTTPContentProvider;
};

#endif /* CHTTPSERVER_H_ */
