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

NMR_ModelToolpathProfile.cpp defines the Model Toolpath Profile.

--*/

#include "Model/Classes/NMR_ModelToolpathProfile.h"
#include "Common/NMR_Exception.h"
#include "Common/NMR_StringUtils.h"
#include <sstream>
#include <algorithm>

namespace NMR {

	CModelToolpathProfile::CModelToolpathProfile(std::string sUUID, std::string sName, nfDouble dLaserPower, nfDouble dLaserSpeed, nfDouble dLaserFocus, nfUint32 nLaserIndex)
		: m_sUUID (sUUID), m_sName (sName), m_dLaserPower (dLaserPower), m_dLaserSpeed (dLaserSpeed), m_dLaserFocus (dLaserFocus), m_nLaserIndex (nLaserIndex)
	{
		
	}

	std::string CModelToolpathProfile::getUUID()
	{
		return m_sUUID;
	}

	std::string CModelToolpathProfile::getName()
	{
		return m_sName;
	}

	nfDouble CModelToolpathProfile::getLaserPower()
	{
		return m_dLaserPower;
	}

	nfDouble CModelToolpathProfile::getLaserSpeed()
	{
		return m_dLaserSpeed;
	}

	nfDouble CModelToolpathProfile::getLaserFocus()
	{
		return m_dLaserFocus;
	}

	nfUint32 CModelToolpathProfile::getLaserIndex()
	{
		return m_nLaserIndex;
	}

	void CModelToolpathProfile::setName(std::string sName)
	{
		m_sName = sName;
	}

	void CModelToolpathProfile::setLaserPower(nfDouble dLaserPower)
	{
		m_dLaserPower = dLaserPower;
	}

	void CModelToolpathProfile::setLaserSpeed(nfDouble dLaserSpeed)
	{
		m_dLaserSpeed = dLaserSpeed;
	}

	void CModelToolpathProfile::setLaserFocus(nfDouble dLaserFocus)
	{
		m_dLaserFocus = dLaserFocus;
	}

	void CModelToolpathProfile::setLaserIndex(nfUint32 nLaserIndex)
	{
		m_nLaserIndex = nLaserIndex;
	}

}
