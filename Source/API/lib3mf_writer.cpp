/*++

Copyright (C) 2018 3MF Consortium (Original Author)

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

Abstract: This is a stub class definition of CLib3MFWriter

*/

#include "lib3mf_writer.hpp"
#include "lib3mf_interfaceexception.hpp"

// Include custom headers here.
// Include custom headers here.
#include "Common/Platform/NMR_Platform.h"
#include "Common/Platform/NMR_ExportStream_Memory.h"
#include "Common/Platform/NMR_ExportStream_Dummy.h"

using namespace Lib3MF::Impl;

/*************************************************************************************************************************
 Class definition of CLib3MFWriter 
**************************************************************************************************************************/

CLib3MFWriter::CLib3MFWriter(std::string sWriterClass, NMR::PModel model)
{
	m_pWriter = nullptr;

	// Create specified writer instance
	if (sWriterClass.compare("3mf") == 0) {
		m_pWriter = std::make_shared<NMR::CModelWriter_3MF_Native>(model);
	}
	else if (sWriterClass.compare("stl") == 0) {
		m_pWriter = std::make_shared<NMR::CModelWriter_STL>(model);
	}

	if (!m_pWriter)
		throw ELib3MFInterfaceException(LIB3MF_ERROR_WRITERCLASSUNKNOWN);
}

NMR::CModelWriter& CLib3MFWriter::writer()
{
	return *m_pWriter;
}

void CLib3MFWriter::WriteToFile (const std::string & sFilename)
{
	setlocale(LC_ALL, "C");
	NMR::PExportStream pStream = NMR::fnCreateExportStreamInstance(sFilename.c_str());
	writer().exportToStream(pStream);
}

Lib3MF_uint64 CLib3MFWriter::GetStreamSize ()
{
	// Write to a special dummy stream just to calculate the size
	NMR::PExportStreamDummy pStream = std::make_shared<NMR::CExportStreamDummy>();
	writer().exportToStream(pStream);

	return pStream->getDataSize();
}

void CLib3MFWriter::WriteToBuffer (Lib3MF_uint64 nBufferBufferSize, Lib3MF_uint64* pBufferNeededCount, Lib3MF_uint8 * pBufferBuffer)
{
	NMR::PExportStreamMemory pStream = std::make_shared<NMR::CExportStreamMemory>();
	writer().exportToStream(pStream);

	Lib3MF_uint64 cbStreamSize = pStream->getDataSize();
	if (pBufferNeededCount)
		*pBufferNeededCount = cbStreamSize;

	if (nBufferBufferSize >= cbStreamSize) {
		// TODO eliminate this copy, perhaps by allowing CExportStreamMemory to use existing buffers
		memcpy(pBufferBuffer, pStream->getData(), static_cast<size_t>(cbStreamSize));
	}
}

void CLib3MFWriter::SetProgressCallback(const Lib3MFProgressCallback callback, Lib3MF_int64 nUserData)
{
	// Outer: typedef void(*Lib3MFProgressCallback)(bool*, Lib3MF_double, eLib3MFProgressIdentifier, Lib3MF_int64);
	// Inner: typedef std::function<bool(int, ProgressIdentifier, void*)> Lib3MFProgressCallback;

	 // store a lambda
	std::function<bool(int, NMR::ProgressIdentifier, void*)> func = 
		[callback](int a, NMR::ProgressIdentifier b, void* c)
		{
			bool ret;
			(*callback)(&ret, 1/100.0f, eLib3MFProgressIdentifier::eProgressIdentifierCLEANUP, reinterpret_cast<Lib3MF_uint64>(c)); return ret;
		};
	m_pWriter->SetProgressCallback(func, reinterpret_cast<void*>(nUserData));
}
