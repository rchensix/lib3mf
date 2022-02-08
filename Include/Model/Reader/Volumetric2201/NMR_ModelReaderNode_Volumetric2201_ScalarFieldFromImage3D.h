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
NMR_ModelReaderNode_Volumetric2201_ScalarFieldFromImage3D.h covers the official 3MF volumetric extension.

--*/

#ifndef __NMR_MODELREADERNODE_VOLUMETRIC2201_SCALARFIELDFROMIMAGE3D
#define __NMR_MODELREADERNODE_VOLUMETRIC2201_SCALARFIELDFROMIMAGE3D

#include "Model/Reader/NMR_ModelReaderNode.h"
#include "Model/Classes/NMR_ModelScalarFieldFromImage3D.h"

namespace NMR {

	class CModelReaderNode_Volumetric2201_ScalarFieldFromImage3D : public CModelReaderNode {
	private:
	protected:

		CModelScalarFieldFromImage3D* m_pScalarFieldFromImage3D;
		CModel* m_pModel;

		nfBool m_bHasImage3DID = false;
		nfBool m_bHasOffset = false;
		nfBool m_bHasScale = false;
		nfBool m_bHasChannel = false;
		nfBool m_bHasTileStyleU = false;
		nfBool m_bHasTileStyleV = false;
		nfBool m_bHasTileStyleW = false;
		nfBool m_bHasFilter = false;

		virtual void OnAttribute(_In_z_ const nfChar * pAttributeName, _In_z_ const nfChar * pAttributeValue);
		virtual void OnNSChildElement(_In_z_ const nfChar * pChildName, _In_z_ const nfChar * pNameSpace, _In_ CXmlReader * pXMLReader);

	public:
		CModelReaderNode_Volumetric2201_ScalarFieldFromImage3D() = delete;
		CModelReaderNode_Volumetric2201_ScalarFieldFromImage3D(_In_ CModel* pModel, _In_ CModelScalarFieldFromImage3D* pScalarFieldFromImage3D, _In_ PModelWarnings pWarnings);

		virtual void parseXML(_In_ CXmlReader * pXMLReader);

	};

	typedef std::shared_ptr <CModelReaderNode_Volumetric2201_ScalarFieldFromImage3D> PModelReaderNode_Volumetric2201_ScalarFieldFromImage3D;

}

#endif // __NMR_MODELREADERNODE_VOLUMETRIC2201_SCALARFIELDFROMIMAGE3D
