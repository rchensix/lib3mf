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

NMR_ModelToolpathProfile.h defines the Model Toolpath Profile.

--*/

#ifndef __NMR_MODELTOOLPATHPROFILE
#define __NMR_MODELTOOLPATHPROFILE

#include "Common/NMR_Types.h" 


#include <vector>
#include <memory>
#include <map>
#include <string>

namespace NMR {

	class CModelToolpathProfile {
	private:
		std::string m_sUUID;
		std::string m_sName;
		nfDouble m_dLaserPower;
		nfDouble m_dLaserSpeed;
		nfDouble m_dLaserFocus;
		nfUint32 m_nLaserIndex;
	public:
		CModelToolpathProfile() = delete;
		CModelToolpathProfile(std::string sUUID, std::string sName, nfDouble dLaserPower, nfDouble dLaserSpeed, nfDouble dLaserFocus, nfUint32 nLaserIndex);

		std::string getUUID();
		std::string getName();
		nfDouble getLaserPower();
		nfDouble getLaserSpeed();
		nfDouble getLaserFocus();
		nfUint32 getLaserIndex();

		void setName (std::string sName);
		void setLaserPower (nfDouble dLaserPower);
		void setLaserSpeed (nfDouble dLaserSpeed);
		void setLaserFocus (nfDouble dLaserFocus);
		void setLaserIndex (nfUint32 nLaserIndex);

	};

	typedef std::shared_ptr <CModelToolpathProfile> PModelToolpathProfile;

}

#endif // __NMR_MODELTOOLPATHPROFILE

