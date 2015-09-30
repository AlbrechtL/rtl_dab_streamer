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
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <vector>

#include "CHTTPServer.h"

#define RECV_BUFFER_SIZE 1024

CHTTPServer::CHTTPServer(CHTTPContentProvider *pHTTPContentProvider)
{
	HTTPContentProvider = pHTTPContentProvider;
	HTTPContentProvider->SetHTTPServer(this);

	ConnectionDesc = 0;
	isConnected = false;

	int optval;

	// Create a TCP socket
	SocketDesc = socket(AF_INET, SOCK_STREAM, 0);

	// Switch on address reuse
	optval = 1;
	setsockopt(SocketDesc, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));

	// Switch off send buffer. This means send out the data immediately
	//optval = 0;
	//setsockopt(SocketDesc, SOL_SOCKET, SO_SNDBUF, &optval, sizeof(optval));

	// A valid descriptor is always a positive value
	if (SocketDesc < 0) fprintf(stderr, "Failed creating socket\n");

	// Reset client_addr
	bzero((char *) &client_addr, sizeof(client_addr));

	// Reset serv_addr
	bzero((char *) &serv_addr, sizeof(serv_addr));

	// Set port
	serv_addr.sin_port = htons(2346); //default port

	// Fill server's address family
	serv_addr.sin_family = AF_INET;

	// Server should allow connections from any ip address
	serv_addr.sin_addr.s_addr = INADDR_ANY;

	size = sizeof(client_addr);

	// Bind to socket
	if (bind(SocketDesc, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
		fprintf(stderr, "Failed to bind\n");

	// Allow 5 connections
	listen(SocketDesc, 5);

	// Start connection thread
	HTTPThread = new std::thread(CHTTPServer::ConnectionThreadWrapper,this);  // spawn new thread that calls bar(0)
}

CHTTPServer::~CHTTPServer()
{

}

void CHTTPServer::Send(const uint8_t* Data, size_t Length)
{
	int SentNum = 0;

	if (isConnected)
	{
		SentNum = send(ConnectionDesc, Data, Length, MSG_NOSIGNAL);
		if (SentNum < 0)
		{
			fprintf(stderr, "Error sending stream: \"%s\". Close the connection!\n", strerror(errno));

			// Close connection
			close(ConnectionDesc);
			HTTPContentProvider->Disconnected();
			isConnected = false;
		}
	}
}

void CHTTPServer::ConnectionThread(void)
{
	char TCPRead[RECV_BUFFER_SIZE];
	size_t TCPReadCount;
	int ConnectionDescNew;

	while(1)
	{
		fprintf(stderr, "Waiting for connection...\n");
		ConnectionDescNew = accept(SocketDesc, (struct sockaddr *) &client_addr, &size);

		// Accept only one connection thus close old connection
		close(ConnectionDesc);

		isConnected = false;
		HTTPContentProvider->Disconnected();

		if (ConnectionDescNew == -1)
		{
			fprintf(stderr, "Failed accepting connection\n");
			continue;
		}
		else
		{
			fprintf(stderr, "Connected\n");
			ConnectionDesc = ConnectionDescNew;

			HTTPContentProvider->Connected();
			isConnected = true;
		}

		// This is mostly working but a better recv solution is to but the received data into a buffer and to check the buffer
		TCPReadCount = recv(ConnectionDesc, TCPRead, RECV_BUFFER_SIZE, 0);

		// Add a zero to the end of the received data. This makes us possible to use the C-string functions.
		// This is only working if we received all data in one chunk.
		if (TCPReadCount > 0)
		{
			TCPRead[TCPReadCount] = '\0';
		}
		else
			fprintf(stderr, "Error: %s (%i)\n", strerror(errno), errno);

		if (!strncmp(TCPRead, "HEAD", 4))
			HTTPContentProvider->HEADResponse();

		if (!strncmp(TCPRead, "GET", 3))
		{
			bool isPart = false;
			int PartStartPos = 0;
			int PartLength = 0;
			std::string URLPart;
			std::vector<std::string> URLPartList;

			// Parse the GET URL
			for(size_t i=4;i< strlen((char*) TCPRead);i++) // Skip "GET "
			{
				if(TCPRead[i] == '/')
				{
					if(isPart)
					{
						// Save part of the URL
						URLPart.assign((char *)TCPRead+PartStartPos+1,PartLength-1);
						URLPartList.push_back(URLPart);
					}

					isPart = true;
					PartStartPos = i;
					PartLength = 0;
				}

				// End of URL
				if(TCPRead[i] == ' ')
				{
					if(isPart)
					{
						// Save URLPart of the URL
						URLPart.assign((char *)TCPRead+PartStartPos+1,PartLength-1);
						URLPartList.push_back(URLPart);
					}
					break;
				}

				if(isPart)
					PartLength++;
			}

			HTTPContentProvider->GETResponse(URLPartList);
		}

		// Wait 1 s
		usleep(100000);
	}
}

void CHTTPServer::ConnectionThreadWrapper(CHTTPServer *HTTPServer)
{
	if(HTTPServer)
		HTTPServer->ConnectionThread();
}
