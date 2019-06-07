/*++

Copyright (C) 2018 3MF Consortium

All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice,
this list of conditions and the following disclaimer in the documentation
and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

Abstract:

NMR_ModelToolpath.h defines the Model Toolpath Layer Data.

--*/

#ifndef __NMR_MODELTOOLPATHLAYERDATA
#define __NMR_MODELTOOLPATHLAYERDATA

#include "Common/NMR_Types.h" 

#include "Model/Classes/NMR_ModelToolpath.h" 
#include "Common/Platform/NMR_XmlWriter.h" 
#include "Common/Platform/NMR_ExportStream_Memory.h" 
#include "Common/ChunkedBinaryStream/NMR_ChunkedBinaryStreamWriter.h" 

#include <vector>
#include "Common/Platform/NMR_ImportStream.h"  
#include <memory>
#include <map>
#include <string>

namespace NMR {


	class CModelToolpath;
	typedef std::shared_ptr <CModelToolpath> PModelToolpath;

	class CModelToolpathLayerData {
	private:
		CModelToolpath * m_pModelToolpath;

		NMR::PXmlWriter m_pXmlWriter;
		NMR::PExportStreamMemory m_pExportStream;

		std::string m_sBinaryStreamPath;
		std::string m_sBinaryStreamUUID;

		std::map <unsigned int, PModelToolpathProfile> m_Profiles;
		std::map <unsigned int, PModelObject> m_Parts;

		bool m_bWritingHeader;
		bool m_bWritingData;
		bool m_bWritingFinished;
		unsigned int m_nIDCounter;

		double m_dUnits;

		NMR::CChunkedBinaryStreamWriter * getStreamWriter();
		
	public:
		CModelToolpathLayerData() = delete;
		CModelToolpathLayerData(_In_ CModelToolpath * pModelToolpath);

		nfUint32 RegisterProfile(_In_ PModelToolpathProfile pProfile);

		nfUint32 RegisterPart(_In_ PModelObject pObject);

		void WriteHatchData(const nfUint32 nProfileID, const nfUint32 nPartID, const nfUint32 nHatchCount, const nfInt32 * pX1Buffer, const nfInt32 * pY1Buffer, const nfInt32 * pX2Buffer, const nfInt32 * pY2Buffer);

		void WriteLoop(const nfUint32 nProfileID, const nfUint32 nPartID, const nfUint32 nPointCount, const nfInt32 * pXBuffer, const nfInt32 * pYBuffer);

		void WritePolyline(const nfUint32 nProfileID, const nfUint32 nPartID, const nfUint32 nPointCount, const nfInt32 * pXBuffer, const nfInt32 * pYBuffer);

		void finishHeader();

		void SetBinaryStream(const std::string sBinaryStreamUUID, const std::string sBinaryStreamPath);

		NMR::PImportStream createStream();

		double getUnits();
		
	};

	typedef std::shared_ptr <CModelToolpathLayerData> PModelToolpathLayerData;

}

#endif // __NMR_MODELTOOLPATHLAYERDATA
