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


--*/

#include "Model/ToolpathReader/NMR_ToolpathReaderNode_Segment.h" 
#include "Model/ToolpathReader/NMR_ToolpathReaderNode_Hatch.h" 
#include "Model/ToolpathReader/NMR_ToolpathReaderNode_Point.h" 

#include "Model/Classes/NMR_ModelConstants.h" 
#include "Common/3MF_ProgressMonitor.h"
#include "Common/NMR_Exception.h"
#include "Common/NMR_StringUtils.h"

#include <iostream>
#include <sstream>
#include <algorithm>
#include <iterator>

namespace NMR {

	CToolpathReaderNode_Segment::CToolpathReaderNode_Segment(_In_ PModelReaderWarnings pWarnings, _In_ PProgressMonitor pProgressMonitor, CModelToolpathLayerReadData * pReadData)
		: CModelReaderNode(pWarnings, pProgressMonitor), 
		m_pReadData(pReadData), 
		m_nPartID (0), 
		m_nProfileID (0),
		m_bHasSegmentType (false),
		m_eSegmentType (eModelToolpathSegmentType::HatchSegment)
	{
		if (pReadData == nullptr)
			throw CNMRException(NMR_ERROR_INVALIDPARAM);

	}


	void CToolpathReaderNode_Segment::parseXML(_In_ CXmlReader * pXMLReader)
	{
		// Parse name
		parseName(pXMLReader);

		// Parse attribute
		parseAttributes(pXMLReader);

		if (!m_bHasSegmentType)
			throw CNMRException(NMR_ERROR_MISSINGTYPEATTRIBUTE);
		if (!hasPartID ())
			throw CNMRException(NMR_ERROR_MISSINGID);
		if (!hasProfileID())
			throw CNMRException(NMR_ERROR_MISSINGID);

		m_pReadData->beginSegment(m_eSegmentType, getProfileID(), getPartID());

		// Parse Content
		parseContent(pXMLReader);

		m_pReadData->endSegment();

	}

	void CToolpathReaderNode_Segment::OnAttribute(_In_z_ const nfChar * pAttributeName, _In_z_ const nfChar * pAttributeValue)
	{
		__NMRASSERT(pAttributeName);
		__NMRASSERT(pAttributeValue);

		if (strcmp(pAttributeName, XML_3MF_TOOLPATHATTRIBUTE_PROFILEID) == 0) {
			if (hasProfileID())
				throw CNMRException(NMR_ERROR_DUPLICATEID);

			nfInt32 nValue = fnStringToInt32(pAttributeValue);
			if ((nValue > 0) && (nValue < XML_3MF_MAXRESOURCEINDEX))
				m_nProfileID = nValue;
		}

		if (strcmp(pAttributeName, XML_3MF_TOOLPATHATTRIBUTE_PARTID) == 0) {
			if (hasPartID())
				throw CNMRException(NMR_ERROR_DUPLICATEID);

			nfInt32 nValue = fnStringToInt32(pAttributeValue);
			if ((nValue > 0) && (nValue < XML_3MF_MAXRESOURCEINDEX))
				m_nPartID = nValue;
		}

		if (strcmp(pAttributeName, XML_3MF_TOOLPATHATTRIBUTE_TYPE) == 0) {
			if (m_bHasSegmentType)
				throw CNMRException(NMR_ERROR_DUPLICATETYPEATTRIBUTE);

			if (strcmp(pAttributeValue, XML_3MF_TOOLPATHTYPE_HATCH) == 0) {
				m_bHasSegmentType = true;
				m_eSegmentType = eModelToolpathSegmentType::HatchSegment;
			}

			if (strcmp(pAttributeValue, XML_3MF_TOOLPATHTYPE_LOOP) == 0) {
				m_bHasSegmentType = true;
				m_eSegmentType = eModelToolpathSegmentType::LoopSegment;
			}

			if (strcmp(pAttributeValue, XML_3MF_TOOLPATHTYPE_POLYLINE) == 0) {
				m_bHasSegmentType = true;
				m_eSegmentType = eModelToolpathSegmentType::PolylineSegment;
			}

			if (!m_bHasSegmentType)
				throw CNMRException(NMR_ERROR_INVALIDTYPEATTRIBUTE);

		}

	}

	void CToolpathReaderNode_Segment::OnNSAttribute(_In_z_ const nfChar * pAttributeName, _In_z_ const nfChar * pAttributeValue, _In_z_ const nfChar * pNameSpace)
	{

	}

	void CToolpathReaderNode_Segment::OnNSChildElement(_In_z_ const nfChar * pChildName, _In_z_ const nfChar * pNameSpace, _In_ CXmlReader * pXMLReader)
	{

		if (strcmp(pNameSpace, XML_3MF_NAMESPACE_TOOLPATHSPEC) == 0) {
			if (strcmp(pChildName, XML_3MF_TOOLPATHELEMENT_HATCH) == 0) {

				if (m_eSegmentType != eModelToolpathSegmentType::HatchSegment)
					throw CNMRException(NMR_ERROR_INVALIDTYPEATTRIBUTE);

				PToolpathReaderNode_Hatch pXMLNode = std::make_shared<CToolpathReaderNode_Hatch>(m_pWarnings, m_pProgressMonitor, m_pReadData);
				pXMLNode->parseXML(pXMLReader);

				m_pReadData->addPoint((nfFloat)pXMLNode->getX1(), (nfFloat)pXMLNode->getY1());
				m_pReadData->addPoint((nfFloat)pXMLNode->getX2(), (nfFloat)pXMLNode->getY2());

			}
			else if (strcmp(pChildName, XML_3MF_TOOLPATHELEMENT_POINT) == 0) {

				if ((m_eSegmentType != eModelToolpathSegmentType::LoopSegment) && (m_eSegmentType != eModelToolpathSegmentType::PolylineSegment))
					throw CNMRException(NMR_ERROR_INVALIDTYPEATTRIBUTE);

				PToolpathReaderNode_Point pXMLNode = std::make_shared<CToolpathReaderNode_Point>(m_pWarnings, m_pProgressMonitor, m_pReadData);
				pXMLNode->parseXML(pXMLReader);

				m_pReadData->addPoint((nfFloat)pXMLNode->getX(), (nfFloat)pXMLNode->getY());

			}
			else
				m_pWarnings->addException(CNMRException(NMR_ERROR_NAMESPACE_INVALID_ELEMENT), mrwInvalidOptionalValue);
		}

	}

	nfUint32 CToolpathReaderNode_Segment::getProfileID()
	{
		return m_nProfileID;
	}

	bool CToolpathReaderNode_Segment::hasProfileID()
	{
		return (m_nProfileID != 0);
	}

	nfUint32 CToolpathReaderNode_Segment::getPartID()
	{
		return m_nPartID;
	}

	bool CToolpathReaderNode_Segment::hasPartID()
	{
		return (m_nPartID != 0);
	}


}
