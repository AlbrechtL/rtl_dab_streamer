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

#include "CFICCallback.h"

using namespace anyrpc;

CFICCallback::CFICCallback(std::string const& name, std::string const& help, bool deleteOnRemove,FICDecoder* DABFICDecoder_):Method(name,help,deleteOnRemove)
{
	DABFICDecoder = DABFICDecoder_;
}

CFICCallback::~CFICCallback()
{
}

void CFICCallback::Execute(Value& params, Value& result)
{
	size_t Size;
	int Mode;
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
		Mode = params[2].GetInt();

		if(DataSize != Size)
			fprintf(stderr,"Received data is incomplete!\n");

		/*for(int i=0;i<DataSize;i+=288)
		{
			//DABPlusDecoder->Feed(Data+i,288);
		}*/
		DABFICDecoder->Process(Data, DataSize);
	}
}
