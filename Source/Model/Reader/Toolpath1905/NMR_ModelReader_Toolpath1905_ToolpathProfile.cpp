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

NMR_ModelReaderNode_Toolpath1905_ToolpathProfile.h covers the official 3MF Toolpath extension.


--*/

#include "Model/Reader/Toolpath1905/NMR_ModelReader_Toolpath1905_ToolpathProfile.h"

#include "Model/Classes/NMR_ModelConstants.h"
#include "Common/NMR_Exception.h"
#include "Common/NMR_Exception_Windows.h"
#include "Common/NMR_StringUtils.h"
#include <climits>
#include <cmath>

namespace NMR {

	CModelReaderNode_Toolpath1905_ToolpathProfile::CModelReaderNode_Toolpath1905_ToolpathProfile (_In_ CModel * pModel, _In_ PModelReaderWarnings pWarnings)
		: CModelReaderNode(pWarnings), 
		m_bHasUUID (false),
		m_bHasName (false),
		m_bHasLaserPower (false),
		m_dLaserPower (0.0),
		m_bHasLaserSpeed (false),
		m_dLaserSpeed (0.0),
		m_bHasLaserFocus (false),
		m_dLaserFocus (0.0),
		m_bHasLaserIndex (false),
		m_nLaserIndex (0),
		m_pModel(pModel)

	{
	}
	
	void CModelReaderNode_Toolpath1905_ToolpathProfile::parseXML(_In_ CXmlReader * pXMLReader)
	{
		// Parse name
		parseName(pXMLReader);

		// Parse attribute
		parseAttributes(pXMLReader);

		// Parse Content
		parseContent(pXMLReader);
		
	}


	void CModelReaderNode_Toolpath1905_ToolpathProfile::OnAttribute(_In_z_ const nfChar * pAttributeName, _In_z_ const nfChar * pAttributeValue)
	{

		__NMRASSERT(pAttributeName);
		__NMRASSERT(pAttributeValue);


		if (strcmp(pAttributeName, XML_3MF_ATTRIBUTE_TOOLPATHPROFILE_UUID) == 0) {
			if (m_bHasUUID)
				throw CNMRException(NMR_ERROR_DUPLICATEUUID);

			// Convert to integer and make a input and range check!
			m_sUUID = pAttributeValue;
			m_bHasUUID = true;
		}

		if (strcmp(pAttributeName, XML_3MF_ATTRIBUTE_TOOLPATHPROFILE_NAME) == 0) {
			if (m_bHasName)
				throw CNMRException(NMR_ERROR_DUPLICATEVALUE);

			// Convert to integer and make a input and range check!
			m_sName = pAttributeValue;
			m_bHasName = true;
		}

		if (strcmp(pAttributeName, XML_3MF_ATTRIBUTE_TOOLPATHPROFILE_LASERPOWER) == 0) {
			if (m_bHasLaserPower)
				throw CNMRException(NMR_ERROR_DUPLICATEVALUE);

			m_dLaserPower = strtof(pAttributeValue, nullptr);
			if (std::isnan(m_dLaserPower))
				throw CNMRException(NMR_ERROR_INVALIDFLOATVALUE);
			if (fabs(m_dLaserPower) > XML_3MF_MAXIMUMCOORDINATEVALUE)
				throw CNMRException(NMR_ERROR_INVALIDFLOATVALUE);
			m_bHasLaserPower = true;
		}


		if (strcmp(pAttributeName, XML_3MF_ATTRIBUTE_TOOLPATHPROFILE_LASERSPEED) == 0) {
			if (m_bHasLaserSpeed)
				throw CNMRException(NMR_ERROR_DUPLICATEVALUE);

			m_dLaserSpeed = strtof(pAttributeValue, nullptr);
			if (std::isnan(m_dLaserSpeed))
				throw CNMRException(NMR_ERROR_INVALIDFLOATVALUE);
			if (fabs(m_dLaserSpeed) > XML_3MF_MAXIMUMCOORDINATEVALUE)
				throw CNMRException(NMR_ERROR_INVALIDFLOATVALUE);
			m_bHasLaserSpeed = true;
		}

		if (strcmp(pAttributeName, XML_3MF_ATTRIBUTE_TOOLPATHPROFILE_LASERFOCUS) == 0) {
			if (m_bHasLaserFocus)
				throw CNMRException(NMR_ERROR_DUPLICATEVALUE);

			m_dLaserFocus = strtof(pAttributeValue, nullptr);
			if (std::isnan(m_dLaserFocus))
				throw CNMRException(NMR_ERROR_INVALIDFLOATVALUE);
			if (fabs(m_dLaserFocus) > XML_3MF_MAXIMUMCOORDINATEVALUE)
				throw CNMRException(NMR_ERROR_INVALIDFLOATVALUE);
			m_bHasLaserFocus = true;
		}

		if (strcmp(pAttributeName, XML_3MF_ATTRIBUTE_TOOLPATHPROFILE_LASERINDEX) == 0) {
			if (m_bHasLaserIndex)
				throw CNMRException(NMR_ERROR_DUPLICATEVALUE);

			// Convert to integer and make a input and range check!
			m_nLaserIndex = fnStringToUint32(pAttributeValue);
			m_bHasLaserIndex = true;			
		}


	}
	
	void CModelReaderNode_Toolpath1905_ToolpathProfile::OnNSAttribute(_In_z_ const nfChar * pAttributeName, _In_z_ const nfChar * pAttributeValue, _In_z_ const nfChar * pNameSpace)
	{
	}
	
	void CModelReaderNode_Toolpath1905_ToolpathProfile::OnNSChildElement(_In_z_ const nfChar * pChildName, _In_z_ const nfChar * pNameSpace, _In_ CXmlReader * pXMLReader)
	{
	}
	
	void CModelReaderNode_Toolpath1905_ToolpathProfile::createProfile(CModelToolpath * pToolpath)
	{
		__NMRASSERT(pToolpath);

		if (!m_bHasUUID)
			throw CNMRException(NMR_ERROR_MISSINGUUID);
		if (!m_bHasName)
			throw CNMRException(NMR_ERROR_MISSINGNAME);
		if (!m_bHasLaserPower)
			throw CNMRException(NMR_ERROR_MISSINGLASERPOWER);
		if (!m_bHasLaserSpeed)
			throw CNMRException(NMR_ERROR_MISSINGLASERSPEED);
		if (!m_bHasLaserFocus)
			throw CNMRException(NMR_ERROR_MISSINGLASERFOCUS);

		// Laserindex is optional
		pToolpath->addExistingProfile(m_sUUID, m_sName, m_dLaserPower, m_dLaserSpeed, m_dLaserFocus, m_nLaserIndex);
	}

}

