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

#include "stdio.h"
#include "CMSCCallback.h"

using namespace anyrpc;

CMSCCallback::CMSCCallback(std::string const& name, std::string const& help, bool deleteOnRemove,SuperframeFilter* DABPlusDecoder_):Method(name,help,deleteOnRemove)
{
	DABPlusDecoder = DABPlusDecoder_;
}

CMSCCallback::~CMSCCallback()
{
}

void CMSCCallback::Execute(Value& params, Value& result)
{
	size_t Size;
	int SCID;
	uint8_t *Data;
	size_t DataSize;

	// Some checks about the received data
	if (params.IsArray() &&
		params.Size() == 3 &&
		params[0].IsBinary() &&
		params[1].IsNumber() &&
		params[2].IsNumber())
	{
		Data = (uint8_t*) (params[0].GetBinary());
		DataSize = params[0].GetBinaryLength();

		Size = params[1].GetInt();
		SCID = params[2].GetInt();

		//fprintf(stderr, "%d\n", Size);
		if(DataSize != Size)
			fprintf(stderr,"Received data is incomplete!\n");

		int superframesize = DataSize / 4; // Why RTKDAB.dll give us only 4 DAB frame??

		uint32_t ReadPos=0;
		for(ReadPos=0;ReadPos<DataSize;ReadPos+=superframesize)
		{
			DABPlusDecoder->Feed(Data+ReadPos,superframesize);
		}
	}
}

