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

#ifndef __NMR_CHUNKEDBINARYSTREAMWRITER
#define __NMR_CHUNKEDBINARYSTREAMWRITER

#include "Common/NMR_Types.h"
#include "Common/NMR_Local.h"
#include "Common/Platform/NMR_ExportStream.h"
#include "Common/Platform/NMR_ExportStream_Memory.h"

#define BINARYCHUNKFILEHEADERSIGN 0x5A464D33
#define BINARYCHUNKFILEHEADERVERSION 0x00000001
#define BINARYCHUNKFILEHEADERRESERVED 8
#define BINARYCHUNKFILECHUNKRESERVED 2

#define BINARYCHUNKFILEENTRYTYPE_INT32ARRAY_NOPREDICTION 1
#define BINARYCHUNKFILEENTRYTYPE_INT32ARRAY_DELTAPREDICTION 2


namespace NMR {

#pragma pack (1)
	typedef struct {
		nfUint32 m_Sign;
		nfUint32 m_Version;
		nfUint32 m_ChunkCount;
		nfUint64 m_ChunkTableStart;
		nfUint32 m_Reserved[BINARYCHUNKFILEHEADERRESERVED];
	} BINARYCHUNKFILEHEADER;

	typedef struct {
		nfUint32 m_EntryCount;
		nfUint64 m_EntryTableStart;
		nfUint64 m_CompressedDataStart;
		nfUint64 m_CompressedDataSize;
		nfUint64 m_UncompressedDataSize;
		nfUint32 m_Reserved[BINARYCHUNKFILECHUNKRESERVED];
	} BINARYCHUNKFILECHUNK;

	typedef struct {
		nfUint32 m_EntryID;
		nfUint32 m_EntryType;
		nfUint32 m_PositionInChunk;
		nfUint32 m_SizeInBytes;
	} BINARYCHUNKFILEENTRYTABLE;

#pragma pack()


	class CChunkedBinaryStreamWriter {
	private:
		PExportStreamMemory m_pExportStream;
		nfUint32 m_elementIDCounter;
		void * m_CurrentContext;

		nfUint64 m_ChunkTableStart;

		void writeHeader();

	public:
	
		CChunkedBinaryStreamWriter (PExportStreamMemory pExportStream);

		void beginChunk();
		void finishChunk();
		nfUint32 getChunkCount ();

		void finishWriting ();

		nfUint32 addIntArray (const nfInt32 * pData, nfUint32 nLength);

		void copyToStream (PExportStream pStream);

	};

	typedef std::shared_ptr <CChunkedBinaryStreamWriter> PChunkedBinaryStreamWriter;

}

#endif // __NMR_CHUNKEDBINARYSTREAMWRITER
