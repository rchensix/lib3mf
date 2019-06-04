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

Abstract: This is a stub class definition of CToolpath

*/

#include "lib3mf_toolpath.hpp"
#include "lib3mf_toolpathprofile.hpp"
#include "lib3mf_toolpathlayerdata.hpp"
#include "lib3mf_attachment.hpp"
#include "lib3mf_interfaceexception.hpp"
#include "lib3mf_binarystream.hpp"

// Include custom headers here.
#include "Common/Platform/NMR_ImportStream_Shared_Memory.h"
#include "Model/Classes/NMR_ModelConstants.h"

using namespace Lib3MF::Impl;

/*************************************************************************************************************************
 Class definition of CToolpath 
**************************************************************************************************************************/

CToolpath::CToolpath(NMR::PModelToolpath pToolpath)
	: CResource(pToolpath), m_pToolpath(pToolpath)
{

}


Lib3MF_double CToolpath::GetUnits()
{
	return m_pToolpath->getUnitFactor();
}

Lib3MF_uint32 CToolpath::GetLayerCount()
{
	return m_pToolpath->getLayerCount();
}

Lib3MF_uint32 CToolpath::GetProfileCount()
{
	throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
}

IAttachment * CToolpath::GetLayerAttachment(const Lib3MF_uint32 nLayerIndex)
{
	auto pLayer = m_pToolpath->getLayer(nLayerIndex);
	return new CAttachment(pLayer->getAttachment());
}

Lib3MF_uint32 CToolpath::GetLayerZ(const Lib3MF_uint32 nLayerIndex)
{
	auto pLayer = m_pToolpath->getLayer(nLayerIndex);
	return pLayer->getMaxZ();
}

IToolpathProfile * CToolpath::AddProfile(const std::string & sName, const Lib3MF_double dLaserPower, const Lib3MF_double dLaserSpeed, const Lib3MF_double dLaserFocus, const Lib3MF_uint32 nLaserIndex)
{
	auto pProfile = m_pToolpath->addProfile(sName, dLaserPower, dLaserSpeed, dLaserFocus, nLaserIndex);

	return new CToolpathProfile(pProfile);
}

IToolpathProfile * CToolpath::GetProfile(const Lib3MF_uint32 nProfileIndex)
{
	auto pProfile = m_pToolpath->getProfile(nProfileIndex);

	return new CToolpathProfile(pProfile);

}

IToolpathProfile * CToolpath::GetProfileUUID(const std::string & sProfileUUID)
{
	auto pProfile = m_pToolpath->getProfileByUUID(sProfileUUID);

	return new CToolpathProfile(pProfile);
}


IAttachment * CToolpath::AddLayer(const Lib3MF_uint32 nZMax, IToolpathLayerData* pLayerData, const std::string & sPath)
{
	NMR::CModel * pModel = m_pToolpath->getModel();

	CToolpathLayerData * pInternalLayerData = dynamic_cast<CToolpathLayerData *> (pLayerData);
	if (pInternalLayerData == nullptr)
		throw ELib3MFInterfaceException(LIB3MF_ERROR_INVALIDPARAM);

	NMR::PImportStream pStream = pInternalLayerData->createStream ();

	auto pAttachment = pModel->addAttachment (sPath, PACKAGE_TOOLPATH_RELATIONSHIP_TYPE, pStream);
	m_pToolpath->addLayer(pAttachment, nZMax);

	return new CAttachment (pAttachment);
}


IToolpathLayerData * CToolpath::CreateEmptyLayerData()
{
	
	return new CToolpathLayerData(std::make_shared<NMR::CModelToolpathLayerData> (m_pToolpath.get ()));
}

