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

NMR_ModelReaderNode100_Triangle.cpp implements the Model Reader Triangle Node
Class. A triangle reader model node is a parser for the triangle node of an
XML Model Stream.

--*/

#include "Model/Reader/ZCompression1906/NMR_ModelReaderNode_ZCompression1906_Triangle.h"

#include "Model/Classes/NMR_ModelConstants.h"
#include "Common/NMR_Exception.h"
#include "Common/NMR_Exception_Windows.h"
#include "Common/NMR_StringUtils.h"
#include <climits>
#include <cmath>

namespace NMR {

	CModelReaderNode_ZCompression1906_Triangle::CModelReaderNode_ZCompression1906_Triangle(_In_ PModelReaderWarnings pWarnings)
		: CModelReaderNode(pWarnings)
	{

		m_nVertices1BinaryID = 0;
		m_nVertices2BinaryID = 0;
		m_nVertices3BinaryID = 0;
	}

	void CModelReaderNode_ZCompression1906_Triangle::parseXML(_In_ CXmlReader * pXMLReader)
	{
		// Parse name
		parseName(pXMLReader);

		// Parse attribute
		parseAttributes(pXMLReader);

		// Parse Content
		parseContent(pXMLReader);
	}

	
	void CModelReaderNode_ZCompression1906_Triangle::OnAttribute(_In_z_ const nfChar * pAttributeName, _In_z_ const nfChar * pAttributeValue)
	{
		__NMRASSERT(pAttributeName);
		__NMRASSERT(pAttributeValue);
		nfInt32 nValue;

		if (strcmp(pAttributeName, XML_3MF_ATTRIBUTE_TRIANGLE_V1) == 0) {
			nValue = fnStringToInt32(pAttributeValue);
			if ((nValue > 0) && (nValue < XML_3MF_MAXBINARYID))
				m_nVertices1BinaryID = nValue;
		}
		else if (strcmp(pAttributeName, XML_3MF_ATTRIBUTE_TRIANGLE_V2) == 0) {
			nValue = fnStringToInt32(pAttributeValue);
			if ((nValue > 0) && (nValue < XML_3MF_MAXBINARYID))
				m_nVertices2BinaryID = nValue;
		}
		else if (strcmp(pAttributeName, XML_3MF_ATTRIBUTE_TRIANGLE_V3) == 0) {
			nValue = fnStringToInt32(pAttributeValue);
			if ((nValue > 0) && (nValue < XML_3MF_MAXBINARYID))
				m_nVertices3BinaryID = nValue;
		}		
		else
			m_pWarnings->addException(CNMRException(NMR_ERROR_NAMESPACE_INVALID_ATTRIBUTE), mrwInvalidOptionalValue);
	}

	void CModelReaderNode_ZCompression1906_Triangle::getBinaryIDs(ModelResourceID & nV1BinaryID, ModelResourceID & nV2BinaryID, ModelResourceID & nV3BinaryID)
	{
		if ((m_nVertices1BinaryID == 0) && (m_nVertices2BinaryID == 0) && (m_nVertices3BinaryID == 0))
			throw CNMRException(NMR_ERROR_INVALIDBINARYELEMENTID);

		nV1BinaryID = m_nVertices1BinaryID;
		nV2BinaryID = m_nVertices2BinaryID;
		nV3BinaryID = m_nVertices3BinaryID;
	}

}

