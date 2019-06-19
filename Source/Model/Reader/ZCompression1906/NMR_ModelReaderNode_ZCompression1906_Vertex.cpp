/*++

Copyright (C) 2019 3MF Consortium

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
NMR_ModelReaderNode100_Vertex.cpp implements the Model Reader Vertex Node Class.
A vertex reader model node is a parser for the vertex node of an XML Model Stream.

--*/

#include "Model/Reader/ZCompression1906/NMR_ModelReaderNode_ZCompression1906_Vertex.h"

#include "Model/Classes/NMR_ModelConstants.h"
#include "Common/NMR_StringUtils.h"
#include "Common/NMR_Exception.h"
#include "Common/NMR_Exception_Windows.h"
#include <cmath>
#include <climits>
#include <stdlib.h>

namespace NMR {

	CModelReaderNode_ZCompression1906_Vertex::CModelReaderNode_ZCompression1906_Vertex(_In_ PModelReaderWarnings pWarnings)
		: CModelReaderNode(pWarnings)
	{
		m_nXBinaryID = 0;
		m_nYBinaryID = 0;
		m_nZBinaryID = 0;
		m_fOriginX = 0.0f;
		m_fOriginY = 0.0f;
		m_fOriginZ = 0.0f;
	}

	void CModelReaderNode_ZCompression1906_Vertex::parseXML(_In_ CXmlReader * pXMLReader)
	{
		// Parse name
		parseName(pXMLReader);

		// Parse attribute
		parseAttributes(pXMLReader);

		// Parse Content
		parseContent(pXMLReader);
	}

	void CModelReaderNode_ZCompression1906_Vertex::OnAttribute(_In_z_ const nfChar * pAttributeName, _In_z_ const nfChar * pAttributeValue)
	{
		__NMRASSERT(pAttributeName);
		__NMRASSERT(pAttributeValue);

		nfInt32 nValue;

		if (strcmp(pAttributeName, XML_3MF_ATTRIBUTE_VERTEX_X) == 0) {
			nValue = fnStringToInt32(pAttributeValue);
			if ((nValue > 0) && (nValue < XML_3MF_MAXBINARYID))
				m_nXBinaryID = nValue;
		}
		else if (strcmp(pAttributeName, XML_3MF_ATTRIBUTE_VERTEX_Y) == 0) {
			nValue = fnStringToInt32(pAttributeValue);
			if ((nValue > 0) && (nValue < XML_3MF_MAXBINARYID))
				m_nYBinaryID = nValue;
		}
		else if (strcmp(pAttributeName, XML_3MF_ATTRIBUTE_VERTEX_Z) == 0) {
			nValue = fnStringToInt32(pAttributeValue);
			if ((nValue > 0) && (nValue < XML_3MF_MAXBINARYID))
				m_nZBinaryID = nValue;
		}
		else if (strcmp(pAttributeName, XML_3MF_ATTRIBUTE_VERTEX_ORIGINX) == 0) {
			m_fOriginX = strtof(pAttributeValue, nullptr);
			if (std::isnan(m_fOriginX))
				throw CNMRException(NMR_ERROR_INVALIDMODELCOORDINATES);
			if (fabs(m_fOriginX) > XML_3MF_MAXIMUMCOORDINATEVALUE)
				throw CNMRException(NMR_ERROR_INVALIDMODELCOORDINATES);
		}
		else if (strcmp(pAttributeName, XML_3MF_ATTRIBUTE_VERTEX_ORIGINY) == 0) {
			m_fOriginY = strtof(pAttributeValue, nullptr);
			if (std::isnan(m_fOriginY))
				throw CNMRException(NMR_ERROR_INVALIDMODELCOORDINATES);
			if (fabs(m_fOriginY) > XML_3MF_MAXIMUMCOORDINATEVALUE)
				throw CNMRException(NMR_ERROR_INVALIDMODELCOORDINATES);
		}
		else if (strcmp(pAttributeName, XML_3MF_ATTRIBUTE_VERTEX_ORIGINZ) == 0) {
			m_fOriginZ = strtof(pAttributeValue, nullptr);
			if (std::isnan(m_fOriginZ))
				throw CNMRException(NMR_ERROR_INVALIDMODELCOORDINATES);
			if (fabs(m_fOriginZ) > XML_3MF_MAXIMUMCOORDINATEVALUE)
				throw CNMRException(NMR_ERROR_INVALIDMODELCOORDINATES);
		}
		else
			m_pWarnings->addException(CNMRException(NMR_ERROR_NAMESPACE_INVALID_ATTRIBUTE), mrwInvalidOptionalValue);
	}


	void CModelReaderNode_ZCompression1906_Vertex::getBinaryIDs(ModelResourceID & XBinaryID, ModelResourceID & YBinaryID, ModelResourceID & ZBinaryID)
	{
		if ((m_nXBinaryID == 0) && (m_nYBinaryID == 0) && (m_nZBinaryID == 0))
			throw CNMRException(NMR_ERROR_INVALIDBINARYELEMENTID);

		XBinaryID = m_nXBinaryID;
		YBinaryID = m_nYBinaryID;
		ZBinaryID = m_nZBinaryID;
	}

	void CModelReaderNode_ZCompression1906_Vertex::getOrigin(nfFloat & fOriginX, nfFloat & fOriginY, nfFloat & fOriginZ)
	{
		fOriginX = m_fOriginX;
		fOriginY = m_fOriginY;
		fOriginZ = m_fOriginZ;
	}

}
