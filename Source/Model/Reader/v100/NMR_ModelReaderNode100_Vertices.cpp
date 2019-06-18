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

NMR_ModelReaderNode100_Vertices.cpp defines the Model Reader Vertices Node Class.
A vertices reader model node is a parser for the vertices node of an
XML Model Stream.

--*/

#include "Model/Reader/v100/NMR_ModelReaderNode100_Vertices.h"
#include "Model/Reader/v100/NMR_ModelReaderNode100_Vertex.h"

#include "Model/Reader/ZCompression1906/NMR_ModelReaderNode_ZCompression1906_Vertex.h"

#include "Model/Classes/NMR_ModelConstants.h"
#include "Common/NMR_StringUtils.h"
#include "Common/NMR_Exception.h"
#include "Common/NMR_Exception_Windows.h"

namespace NMR {

	CModelReaderNode100_Vertices::CModelReaderNode100_Vertices(_In_ CMesh * pMesh, _In_ std::string sBinaryStreamPath, _In_ PModelReaderWarnings pWarnings)
		: CModelReaderNode(pWarnings)
	{
		__NMRASSERT(pMesh);
		m_pMesh = pMesh;
		m_sBinaryStreamPath = sBinaryStreamPath;
	}

	void CModelReaderNode100_Vertices::parseXML(_In_ CXmlReader * pXMLReader)
	{
		// Parse Name
		parseName(pXMLReader);

		// Parse attribute
		parseAttributes(pXMLReader);

		// Parse Content
		parseContent(pXMLReader);
	}

	void CModelReaderNode100_Vertices::OnAttribute(_In_z_ const nfChar * pAttributeName, _In_z_ const nfChar * pAttributeValue)
	{
		__NMRASSERT(pAttributeName);
		__NMRASSERT(pAttributeValue);
	}

	void CModelReaderNode100_Vertices::OnNSChildElement(_In_z_ const nfChar * pChildName, _In_z_ const nfChar * pNameSpace, _In_ CXmlReader * pXMLReader)
	{
		__NMRASSERT(pChildName);
		__NMRASSERT(pXMLReader);
		__NMRASSERT(pNameSpace);

		if (strcmp(pNameSpace, XML_3MF_NAMESPACE_CORESPEC100) == 0) {
			if (strcmp(pChildName, XML_3MF_ELEMENT_VERTEX) == 0)
			{
				PModelReaderNode100_Vertex pXMLNode = std::make_shared<CModelReaderNode100_Vertex>(m_pWarnings);
				pXMLNode->parseXML(pXMLReader);

				// Create Mesh Node
				nfFloat fX, fY, fZ;
				pXMLNode->retrievePosition(fX, fY, fZ);
				m_pMesh->addNode(fnVEC3_make(fX, fY, fZ));
			}
			else
				m_pWarnings->addException(CNMRException(NMR_ERROR_NAMESPACE_INVALID_ELEMENT), mrwInvalidOptionalValue);
		}

		if (strcmp(pNameSpace, XML_3MF_NAMESPACE_ZCOMPRESSION) == 0) {
			if (strcmp(pChildName, XML_3MF_ELEMENT_VERTEX) == 0)
			{
				PModelReaderNode_ZCompression1906_Vertex pXMLNode = std::make_shared<CModelReaderNode_ZCompression1906_Vertex>(m_pWarnings);
				pXMLNode->parseXML(pXMLReader);

				ModelResourceID nXID, nYID, nZID;
				nfFloat fOriginX, fOriginY, fOriginZ;
				pXMLNode->getBinaryIDs(nXID, nYID, nZID);				
				pXMLNode->getOrigin(fOriginX, fOriginY, fOriginZ);

				if ((m_pBinaryStreamCollection.get() == nullptr) || (m_sBinaryStreamPath.empty ()))
					throw CNMRException(NMR_ERROR_NOBINARYSTREAMAVAILABLE);

				auto pReader = m_pBinaryStreamCollection->findReader(m_sBinaryStreamPath);
				if (pReader == nullptr)
					throw CNMRException(NMR_ERROR_BINARYSTREAMNOTFOUND);

				nfUint32 nXCount = pReader->getTypedChunkEntryCount (nXID, edtFloatArray);
				nfUint32 nYCount = pReader->getTypedChunkEntryCount (nYID, edtFloatArray);
				nfUint32 nZCount = pReader->getTypedChunkEntryCount (nZID, edtFloatArray);

				if ((nXCount != nYCount) || (nXCount != nZCount) || (nYCount != nZCount))
					throw CNMRException(NMR_ERROR_INCONSISTENTBINARYSTREAMCOUNT);
				nfUint32 nCount = nXCount;

				if (nCount > 0) {

					std::vector<nfFloat> XValues;
					std::vector<nfFloat> YValues;
					std::vector<nfFloat> ZValues;

					XValues.resize(nCount);
					YValues.resize(nCount);
					ZValues.resize(nCount);

					pReader->readFloatArray(nXID, XValues.data(), nCount);
					pReader->readFloatArray(nYID, YValues.data(), nCount);
					pReader->readFloatArray(nZID, ZValues.data(), nCount);

					auto iX = XValues.begin();
					auto iY = YValues.begin();
					auto iZ = ZValues.begin();

					for (nfUint32 nIndex = 0; nIndex < nCount; nIndex++) {
						m_pMesh->addNode(fnVEC3_make(*iX + fOriginX, *iY + fOriginY, *iZ + fOriginZ));
						iX++; iY++; iZ++;
					}
				}
			}
			else
				m_pWarnings->addException(CNMRException(NMR_ERROR_NAMESPACE_INVALID_ELEMENT), mrwInvalidOptionalValue);
		}

	}

}
