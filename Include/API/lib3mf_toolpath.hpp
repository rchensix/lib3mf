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

Abstract: This is the class declaration of CToolpath

*/


#ifndef __LIB3MF_TOOLPATH
#define __LIB3MF_TOOLPATH

#include "lib3mf_interfaces.hpp"

// Parent classes
#include "lib3mf_resource.hpp"
#pragma warning( push)
#pragma warning( disable : 4250)

// Include custom headers here.
#include "Model/Classes/NMR_ModelToolpath.h"

namespace Lib3MF {
namespace Impl {


/*************************************************************************************************************************
 Class declaration of CToolpath 
**************************************************************************************************************************/

class CToolpath : public virtual IToolpath, public virtual CResource {
private:

protected:

	NMR::PModelToolpath m_pToolpath;

public:

	CToolpath(NMR::PModelToolpath pToolpath);

	Lib3MF_double GetUnits();

	Lib3MF_uint32 GetLayerCount();

	Lib3MF_uint32 GetProfileCount();

	IAttachment * AddLayer(const Lib3MF_uint32 nZMax, IToolpathLayerData* pLayerData);

	IAttachment * GetLayerAttachment(const Lib3MF_uint32 nLayerIndex);

	Lib3MF_uint32 GetLayerZ(const Lib3MF_uint32 nLayerIndex);

	IToolpathProfile * AddProfile(const std::string & sName, const Lib3MF_double dLaserPower, const Lib3MF_double dLaserSpeed, const Lib3MF_double dLaserFocus, const Lib3MF_uint32 nLaserIndex);

	IToolpathProfile * GetProfile(const Lib3MF_uint32 nProfileIndex);

	IToolpathProfile * GetProfileUUID(const std::string & sProfileUUID);

};

} // namespace Impl
} // namespace Lib3MF

#pragma warning( pop )
#endif // __LIB3MF_TOOLPATH
