/*
    rtl_dab_streamer - Streams the DAB audio samples vie HTTP
    Copyright (c) 2015 Albrecht Lohofener <albrechtloh@gmx.de>

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

#include "CRTKDABServer.h"

CRTKDABServer::CRTKDABServer(int ListenPort, SuperframeFilter *DABPlusDecoder, FICDecoder* DABFICDecoder)
{
	// New message pack TCP server with threading
	MessagePack_Server = new anyrpc::MessagePackTcpServerTP;

	// Bind to port
	MessagePack_Server->BindAndListen(ListenPort);

	// Add the method calls
	MethodManager = MessagePack_Server->GetMethodManager();

	// Add callback methods
	MethodManager->AddMethod(new CMSCCallback("MSCData", "MSC Data", true, DABPlusDecoder));
	MethodManager->AddMethod(new CFICCallback("FICData", "FIC Data", true, DABFICDecoder));

	MessagePack_Server->StartThread();
}

CRTKDABServer::~CRTKDABServer()
{
	MessagePack_Server->StopThread();
}
