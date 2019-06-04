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

#include "Common/ChunkedBinaryStream/NMR_ChunkedBinaryStreamWriter.h" 
#include "Common/NMR_Exception.h" 
#include <vector>


namespace NMR {

#define BINARYCHUNKFILEHEADERSIGN 0x5A464D33
#define BINARYCHUNKFILEHEADERVERSION 0x00000001
#define BINARYCHUNKFILEHEADERRESERVED 8

#pragma pack (1)
	typedef struct {
		nfUint32 m_Sign;
		nfUint32 m_Version;
		nfUint32 m_ChunkCount;
		nfUint64 m_ChunkTableStart;
		nfUint32 m_Reserved[BINARYCHUNKFILEHEADERRESERVED];
	} BINARYCHUNKFILEHEADER;
#pragma pack()


	CChunkedBinaryStreamWriter::CChunkedBinaryStreamWriter(PExportStream pExportStream)
		: m_pExportStream (pExportStream), m_elementIDCounter (1), m_CurrentContext (nullptr), m_ChunkTableStart (0)
	{
		writeHeader();
	}

	void CChunkedBinaryStreamWriter::beginChunk()
	{
		if (m_CurrentContext != nullptr)
			throw CNMRException(NMR_ERROR_STREAMCHUNKALREADYOPEN);

		m_CurrentContext = new char[4];

	}

	void CChunkedBinaryStreamWriter::finishChunk()
	{
		if (m_CurrentContext == nullptr)
			throw CNMRException(NMR_ERROR_NOSTREAMCHUNKOPEN);

		delete m_CurrentContext;
		m_CurrentContext = nullptr;
	}

	void CChunkedBinaryStreamWriter::finishWriting()
	{
		writeHeader();
	}

	//compressionType = (uncompressed, deltaPrediction, Sparse, RLE);

	nfUint32 CChunkedBinaryStreamWriter::addIntArray(const nfInt32 * pData, nfUint32 nLength)
	{
		if (pData == nullptr)
			throw CNMRException(NMR_ERROR_INVALIDPARAM);

		if (m_CurrentContext == nullptr)
			beginChunk ();

		unsigned int nElementID = m_elementIDCounter;
		m_elementIDCounter++;

		return nElementID;

	}


	void CChunkedBinaryStreamWriter::writeHeader()
	{
		BINARYCHUNKFILEHEADER Header;
		Header.m_Sign = BINARYCHUNKFILEHEADERSIGN;
		Header.m_Version = BINARYCHUNKFILEHEADERVERSION;
		Header.m_ChunkCount = getChunkCount ();
		Header.m_ChunkTableStart = m_ChunkTableStart;
		for (int j = 0; j < BINARYCHUNKFILEHEADERRESERVED; j++)
			Header.m_Reserved[j] = 0;
	}

	nfUint32 CChunkedBinaryStreamWriter::getChunkCount()
	{
		return 0;
	}

}
