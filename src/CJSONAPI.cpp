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

#include "CJSONAPI.h"

using namespace anyrpc;

CJSON_API::CJSON_API(int ListenPort, FICDecoder *DABFICDecoder, CRTKDABClient *RTKDABClient)
{
	// New message pack TCP server with threading
	//JSON_RPC_Server = new JsonTcpServerTP;
	JSON_RPC_Server = new JsonHttpServerTP();

	// Bind to port
	JSON_RPC_Server->BindAndListen(ListenPort);

	// Add the method calls
	MethodManager = JSON_RPC_Server->GetMethodManager();

	// Add callback methods
	MethodManager->AddMethod(new CSetChannel("SetChannel", "No help available", true, RTKDABClient));
	MethodManager->AddMethod(new CGetChannelInformation("GetChannelInformation", "No help available", true, DABFICDecoder));

	JSON_RPC_Server->StartThread();
}

CJSON_API::~CJSON_API()
{

}

CFunctionCallbackRTKDAB::CFunctionCallbackRTKDAB(std::string const& name, std::string const& help, bool deleteOnRemove, CRTKDABClient* RTKDABClient_) :Method(name, help, deleteOnRemove)
{
	RTKDABClient = RTKDABClient_;
}

CFunctionCallbackFIC::CFunctionCallbackFIC(std::string const& name, std::string const& help, bool deleteOnRemove, FICDecoder* DABFICDecoder_) :Method(name, help, deleteOnRemove)
{
	DABFICDecoder = DABFICDecoder_;
}

void CSetChannel::Execute(anyrpc::Value& params, anyrpc::Value& result)
{
	int Frequency = 0;
	std::string Channel;

	std::cout << "SetChannel() ";

	// Some checks about the received data
	if (params.IsArray() &&
			params.Size() == 1 &&
			params[0].IsString())
	{
		Channel = params[0].GetString();

		std::cout<< "Channel: " << Channel;

		auto dab_channels_it = dab_channels.find(Channel);

		// Abort if channel frequency is not found
		if(dab_channels_it == dab_channels.end())
		{
			return;
		}

		// Get the channel frequency
		uint32_t Frequency = dab_channels_it->second;

		// Set frequency
		RTKDABClient->SetFreq(Frequency);
	}
	std::cout << std::endl;
	result.SetNull();
}

void CGetChannelInformation::Execute(anyrpc::Value& params, anyrpc::Value& result)
{
	std::cout << "GetChannelInformation() ";

	// Get available DAB channels
	services_t new_services = DABFICDecoder->GetServices();

	result.SetArray();

	// Find audio service in database
	int i = 0;
	for (auto &any : new_services) // TODO Maybe to use of find is better
	{
		result[i].SetArray();

		// Channel name
		result[i][0].SetString((const char*)any.label.label,16,true);

		// Channel SID in hex
		std::stringstream SID_HEX;
		SID_HEX << "0x" << std::hex << any.sid;
		result[i][1].SetString(SID_HEX.str().c_str());

		i++;
	}

	std::cout << std::endl;
}
