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

Abstract: This is a stub class definition of CToolpathProfile

*/

#include "lib3mf_toolpathprofile.hpp"
#include "lib3mf_interfaceexception.hpp"

// Include custom headers here.


using namespace Lib3MF::Impl;

/*************************************************************************************************************************
 Class definition of CToolpathProfile 
**************************************************************************************************************************/

CToolpathProfile::CToolpathProfile(NMR::PModelToolpathProfile pToolpathProfile)
	: m_pToolpathProfile(pToolpathProfile)
{
	if (pToolpathProfile.get() == nullptr)
		throw ELib3MFInterfaceException(LIB3MF_ERROR_INVALIDPARAM);
}


std::string CToolpathProfile::GetUUID()
{
	return m_pToolpathProfile->getUUID();
}

std::string CToolpathProfile::GetName()
{
	return m_pToolpathProfile->getName();
}

Lib3MF_double CToolpathProfile::GetLaserPower()
{
	return m_pToolpathProfile->getLaserPower();
}

Lib3MF_double CToolpathProfile::GetLaserSpeed()
{
	return m_pToolpathProfile->getLaserSpeed();
}

Lib3MF_double CToolpathProfile::GetLaserFocus()
{
	return m_pToolpathProfile->getLaserFocus();
}

Lib3MF_uint32 CToolpathProfile::GetLaserIndex()
{
	return m_pToolpathProfile->getLaserIndex();
}

void CToolpathProfile::SetName(const std::string & sName)
{
	m_pToolpathProfile->setName(sName);
}

void CToolpathProfile::SetLaserPower(const Lib3MF_double dLaserPower)
{
	m_pToolpathProfile->setLaserPower(dLaserPower);
}

void CToolpathProfile::SetLaserSpeed(const Lib3MF_double dLaserSpeed)
{
	m_pToolpathProfile->setLaserSpeed(dLaserSpeed);
}

void CToolpathProfile::SetLaserFocus(const Lib3MF_double dLaserFocus)
{
	m_pToolpathProfile->setLaserFocus(dLaserFocus);
}

void CToolpathProfile::SetLaserIndex(const Lib3MF_uint32 nLaserIndex)
{
	m_pToolpathProfile->setLaserIndex(nLaserIndex);
}