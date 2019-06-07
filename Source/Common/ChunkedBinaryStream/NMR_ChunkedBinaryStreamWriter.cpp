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
#include "Common/Platform/NMR_ImportStream_Shared_Memory.h" 
#include "Common/NMR_Exception.h" 
#include <vector>


namespace NMR {


	CChunkedBinaryStreamWriter::CChunkedBinaryStreamWriter(PExportStreamMemory pExportStream)
		: m_pExportStream (pExportStream), m_elementIDCounter (1), m_CurrentChunk (nullptr), m_ChunkTableStart (0)
	{
		if (pExportStream.get() == nullptr)
			throw CNMRException(NMR_ERROR_INVALIDPARAM);

		writeHeader();
	}

	void CChunkedBinaryStreamWriter::beginChunk()
	{
		if (m_CurrentChunk != nullptr)
			finishChunk ();

		BINARYCHUNKFILECHUNK Chunk;
		Chunk.m_EntryCount = 0;
		Chunk.m_EntryTableStart = 0;
		Chunk.m_CompressedDataSize = 0;
		Chunk.m_UncompressedDataSize = 0;
		Chunk.m_CompressedDataStart = 0;
		for (int j = 0; j < BINARYCHUNKFILECHUNKRESERVED; j++)
			Chunk.m_Reserved[j] = 0;
		m_Chunks.push_back(Chunk);

		m_CurrentChunk = &(*m_Chunks.rbegin());

	}

	void CChunkedBinaryStreamWriter::finishChunk()
	{
		if (m_CurrentChunk == nullptr)
			throw CNMRException(NMR_ERROR_NOSTREAMCHUNKOPEN);

		m_CurrentChunk = nullptr;
	}

	void CChunkedBinaryStreamWriter::finishWriting()
	{
		if (m_CurrentChunk != nullptr)
			finishChunk();

		writeChunkTable();
		writeHeader();
	}

	//compressionType = (uncompressed, deltaPrediction, Sparse, RLE);

	nfUint32 CChunkedBinaryStreamWriter::addIntArray(const nfInt32 * pData, nfUint32 nLength)
	{
		if (pData == nullptr)
			throw CNMRException(NMR_ERROR_INVALIDPARAM);

		if (m_CurrentChunk == nullptr)
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

		m_pExportStream->seekPosition(0, true);
		m_pExportStream->writeBuffer(&Header, sizeof(Header));
		m_pExportStream->seekFromEnd(0, true);
	}

	nfUint32 CChunkedBinaryStreamWriter::getChunkCount()
	{
		return (nfUint32) m_Chunks.size ();
	}

	void CChunkedBinaryStreamWriter::copyToStream(PExportStream pStream)
	{
		if (pStream.get() == nullptr)
			throw CNMRException(NMR_ERROR_INVALIDPARAM);

		const nfByte * pData = m_pExportStream->getData();
		nfUint64 nDataSize = m_pExportStream->getDataSize();

		pStream->writeBuffer(pData, nDataSize);

	}


	void CChunkedBinaryStreamWriter::writeChunkTable()
	{
		m_ChunkTableStart = m_pExportStream->getPosition();
		if (m_Chunks.size() > 0) {
			m_pExportStream->writeBuffer (m_Chunks.data(), sizeof (BINARYCHUNKFILECHUNK) * m_Chunks.size());
		}
	}

}
