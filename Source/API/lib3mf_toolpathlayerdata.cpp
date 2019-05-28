/*++

Copyright (C) 2019 3MF Consortium (Original Author)

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

Abstract: This is a stub class definition of CToolpathLayerData

*/

#include "lib3mf_toolpathlayerdata.hpp"
#include "lib3mf_interfaceexception.hpp"

// Include custom headers here.
#include "Common/Platform/NMR_ImportStream_Shared_Memory.h"
#include "Common/Platform/NMR_ImportStream_Unique_Memory.h"
#include "Common/Platform/NMR_XmlWriter_Native.h"
#include "Model/Classes/NMR_ModelConstants.h"

using namespace Lib3MF::Impl;

/*************************************************************************************************************************
 Class definition of CToolpathLayerData 
**************************************************************************************************************************/

CToolpathLayerData::CToolpathLayerData()
{
	m_pExportStream = std::make_shared<NMR::CExportStreamMemory>();

	m_pXmlWriter = std::make_shared<NMR::CXmlWriter_Native>(m_pExportStream);
	m_pXmlWriter->WriteStartDocument();
	m_pXmlWriter->WriteStartElement(nullptr, XML_3MF_TOOLPATHELEMENT_LAYER, XML_3MF_NAMESPACE_TOOLPATHLAYER);

	m_bWritingHeader = true;
	m_bWritingData = false;
	m_bWritingFinished = false;

	m_nIDCounter = 1;
}


Lib3MF_uint32 CToolpathLayerData::RegisterProfile(IToolpathProfile* pProfile)
{
	if (!m_bWritingHeader)
		throw ELib3MFInterfaceException(LIB3MF_ERROR_TOOLPATH_NOTWRITINGHEADER);

	unsigned int nNewID = m_nIDCounter;
	m_nIDCounter++;

	m_Profiles.insert(std::make_pair(nNewID, pProfile));

	return nNewID;
}

Lib3MF_uint32 CToolpathLayerData::RegisterPart(IObject* pPart)
{
	if (!m_bWritingHeader)
		throw ELib3MFInterfaceException(LIB3MF_ERROR_TOOLPATH_NOTWRITINGHEADER);

	unsigned int nNewID = m_nIDCounter;
	m_nIDCounter++;

	m_Parts.insert(std::make_pair(nNewID, pPart));

	return nNewID;
}

void CToolpathLayerData::finishHeader()
{
	if (!m_bWritingHeader)
		throw ELib3MFInterfaceException(LIB3MF_ERROR_TOOLPATH_NOTWRITINGHEADER);

	m_bWritingHeader = false;

	m_pXmlWriter->WriteStartElement(nullptr, XML_3MF_TOOLPATHELEMENT_PARTS, nullptr);
	for (auto iPart : m_Parts) {
		bool bHasUUID = false;

		m_pXmlWriter->WriteStartElement(nullptr, XML_3MF_TOOLPATHELEMENT_PART, nullptr);
		std::string sID = std::to_string(iPart.first);
		std::string sUUID = iPart.second->GetUUID (bHasUUID);

		m_pXmlWriter->WriteAttributeString(nullptr, XML_3MF_TOOLPATHATTRIBUTE_ID, nullptr, sID.c_str());
		m_pXmlWriter->WriteAttributeString(nullptr, XML_3MF_TOOLPATHATTRIBUTE_UUID, nullptr, sUUID.c_str());
		m_pXmlWriter->WriteEndElement();
	}

	m_pXmlWriter->WriteFullEndElement();

	m_pXmlWriter->WriteStartElement(nullptr, XML_3MF_TOOLPATHELEMENT_PROFILES, nullptr);
	for (auto iProfile : m_Profiles) {
		m_pXmlWriter->WriteStartElement(nullptr, XML_3MF_TOOLPATHELEMENT_PROFILE, nullptr);
		std::string sID = std::to_string(iProfile.first);
		std::string sUUID = iProfile.second->GetUUID();

		m_pXmlWriter->WriteAttributeString(nullptr, XML_3MF_TOOLPATHATTRIBUTE_ID, nullptr, sID.c_str());
		m_pXmlWriter->WriteAttributeString(nullptr, XML_3MF_TOOLPATHATTRIBUTE_UUID, nullptr, sUUID.c_str());
		m_pXmlWriter->WriteEndElement();
	}

	m_pXmlWriter->WriteFullEndElement();

	m_pXmlWriter->WriteStartElement(nullptr, XML_3MF_TOOLPATHELEMENT_SEGMENTS, nullptr);

	m_bWritingData = true;


}


void CToolpathLayerData::WriteHatchData(const Lib3MF_uint32 nProfileID, const Lib3MF_uint32 nPartID, const Lib3MF_uint64 nPointDataBufferSize, const Lib3MF::sPosition2D * pPointDataBuffer)
{
	if (nPointDataBufferSize % 2 != 0)
		throw ELib3MFInterfaceException(LIB3MF_ERROR_INVALIDPARAM);

	if (m_bWritingHeader)
		finishHeader();
	if (!m_bWritingData)
		throw ELib3MFInterfaceException(LIB3MF_ERROR_TOOLPATH_NOTWRITINGDATA);

	std::string sPartID = std::to_string(nPartID);
	std::string sProfileID = std::to_string(nProfileID);

	m_pXmlWriter->WriteStartElement(nullptr, XML_3MF_TOOLPATHELEMENT_SEGMENT, nullptr);
	m_pXmlWriter->WriteAttributeString(nullptr, XML_3MF_TOOLPATHATTRIBUTE_TYPE, nullptr, XML_3MF_TOOLPATHTYPE_HATCH);
	m_pXmlWriter->WriteAttributeString(nullptr, XML_3MF_TOOLPATHATTRIBUTE_PROFILEID, nullptr, sProfileID.c_str());
	m_pXmlWriter->WriteAttributeString(nullptr, XML_3MF_TOOLPATHATTRIBUTE_PARTID, nullptr, sPartID.c_str());


	Lib3MF_uint64 nIndex;
	const Lib3MF::sPosition2D * pPointData = pPointDataBuffer;
	for (nIndex = 0; nIndex < nPointDataBufferSize / 2; nIndex++) {
		std::string sX1 = std::to_string(pPointData->m_Coordinates[0]);
		std::string sY1 = std::to_string(pPointData->m_Coordinates[1]);
		pPointData++;
		std::string sX2 = std::to_string(pPointData->m_Coordinates[0]);
		std::string sY2 = std::to_string(pPointData->m_Coordinates[1]);
		pPointData++;

		m_pXmlWriter->WriteStartElement(nullptr, XML_3MF_TOOLPATHELEMENT_HATCH, nullptr);
		m_pXmlWriter->WriteAttributeString(nullptr, XML_3MF_TOOLPATHATTRIBUTE_X1, nullptr, sX1.c_str());
		m_pXmlWriter->WriteAttributeString(nullptr, XML_3MF_TOOLPATHATTRIBUTE_Y1, nullptr, sY1.c_str());
		m_pXmlWriter->WriteAttributeString(nullptr, XML_3MF_TOOLPATHATTRIBUTE_X2, nullptr, sX2.c_str());
		m_pXmlWriter->WriteAttributeString(nullptr, XML_3MF_TOOLPATHATTRIBUTE_Y2, nullptr, sY2.c_str());
		m_pXmlWriter->WriteEndElement();

	}

	m_pXmlWriter->WriteFullEndElement();

}

void CToolpathLayerData::WriteLoop(const Lib3MF_uint32 nProfileID, const Lib3MF_uint32 nPartID, const Lib3MF_uint64 nPointDataBufferSize, const Lib3MF::sPosition2D * pPointDataBuffer)
{
	if (m_bWritingHeader)
		finishHeader();
	if (!m_bWritingData)
		throw ELib3MFInterfaceException(LIB3MF_ERROR_TOOLPATH_NOTWRITINGDATA);

	std::string sPartID = std::to_string(nPartID);
	std::string sProfileID = std::to_string(nProfileID);

	m_pXmlWriter->WriteStartElement(nullptr, XML_3MF_TOOLPATHELEMENT_SEGMENT, nullptr);
	m_pXmlWriter->WriteAttributeString(nullptr, XML_3MF_TOOLPATHATTRIBUTE_TYPE, nullptr, XML_3MF_TOOLPATHTYPE_LOOP);
	m_pXmlWriter->WriteAttributeString(nullptr, XML_3MF_TOOLPATHATTRIBUTE_PROFILEID, nullptr, sProfileID.c_str());
	m_pXmlWriter->WriteAttributeString(nullptr, XML_3MF_TOOLPATHATTRIBUTE_PARTID, nullptr, sPartID.c_str());
	m_pXmlWriter->WriteFullEndElement();
}

void CToolpathLayerData::WritePolyline(const Lib3MF_uint32 nProfileID, const Lib3MF_uint32 nPartID, const Lib3MF_uint64 nPointDataBufferSize, const Lib3MF::sPosition2D * pPointDataBuffer)
{
	if (m_bWritingHeader)
		finishHeader();
	if (!m_bWritingData)
		throw ELib3MFInterfaceException(LIB3MF_ERROR_TOOLPATH_NOTWRITINGDATA);

	std::string sPartID = std::to_string(nPartID);
	std::string sProfileID = std::to_string(nProfileID);

	m_pXmlWriter->WriteStartElement(nullptr, XML_3MF_TOOLPATHELEMENT_SEGMENT, nullptr);
	m_pXmlWriter->WriteAttributeString(nullptr, XML_3MF_TOOLPATHATTRIBUTE_TYPE, nullptr, XML_3MF_TOOLPATHTYPE_POLYLINE);
	m_pXmlWriter->WriteAttributeString(nullptr, XML_3MF_TOOLPATHATTRIBUTE_PROFILEID, nullptr, sProfileID.c_str());
	m_pXmlWriter->WriteAttributeString(nullptr, XML_3MF_TOOLPATHATTRIBUTE_PARTID, nullptr, sPartID.c_str());
	m_pXmlWriter->WriteFullEndElement();
}

NMR::PImportStream CToolpathLayerData::createStream()
{
	if (m_bWritingHeader)
		finishHeader();
	if (!m_bWritingData)
		throw ELib3MFInterfaceException(LIB3MF_ERROR_TOOLPATH_NOTWRITINGDATA);

	m_bWritingData = false;
	if (m_bWritingFinished)
		throw ELib3MFInterfaceException(LIB3MF_ERROR_TOOLPATH_DATAHASBEENWRITTEN);

	m_bWritingFinished = true;

	m_pXmlWriter->WriteFullEndElement(); // segments

	m_pXmlWriter->WriteFullEndElement(); // layer
	m_pXmlWriter->WriteEndDocument();
	m_pXmlWriter->Flush();
	
	// TODO: Do not copy but use Pipe-based importexportstream!
	NMR::CImportStream_Shared_Memory pImportStream (m_pExportStream->getData(), m_pExportStream->getDataSize());	
	return pImportStream.copyToMemory();
}

