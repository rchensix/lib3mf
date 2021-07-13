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

UnitTest_Writer.cpp: Defines Unittests for the Writer classes

--*/

#include "UnitTest_Utilities.h"
#include "lib3mf_implicit.hpp"

namespace Lib3MF
{
	class Writer : public ::testing::Test {
	protected:
		virtual void SetUp() {
			model = wrapper->CreateModel();
			auto reader = model->QueryReader("3mf");
			reader->ReadFromFile(InFolder + "Pyramid.3mf");
			writer3MF = model->QueryWriter("3mf");
			writerSTL = model->QueryWriter("stl");
			writer3MFz = model->QueryWriter("3mfz");
		}
		virtual void TearDown() {
			model.reset();
			writer3MF.reset();
			writerSTL.reset();
			writer3MFz.reset();
		}
	
		PModel model;
		PWriter writer3MF;
		PWriter writerSTL;
		PWriter writer3MFz;

		static std::string InFolder;
		static std::string OutFolder;

		static void SetUpTestCase() {
			wrapper = CWrapper::loadLibrary();
		}
		static PWrapper wrapper;
	};
	PWrapper Writer::wrapper;
	std::string Writer::InFolder(sTestFilesPath + "/Writer/");
	std::string Writer::OutFolder(sOutFilesPath + "/Writer/");
	

	TEST_F(Writer, 3MFWriteToFile)
	{
		 Writer::writer3MF->WriteToFile(Writer::OutFolder + "Pyramid.3mf");
	}

	TEST_F(Writer, STLWriteToFile)
	{
		Writer::writerSTL->WriteToFile(Writer::OutFolder + "Pyramid.stl");
	}

	TEST_F(Writer, 3MFWriteToBuffer)
	{
		// This is a bit silly, as it essentially performs two writes!
		std::vector<Lib3MF_uint8> buffer;
		Writer::writer3MF->WriteToBuffer(buffer);
	}

	TEST_F(Writer, STLWriteToBuffer)
	{
		// This is a bit silly, as it essentially performs two writes!
		std::vector<Lib3MF_uint8> buffer;
		Writer::writerSTL->WriteToBuffer(buffer);
	}

	TEST_F(Writer, 3MFCompare)
	{
		// This test is atleast functional
		std::vector<Lib3MF_uint8> buffer;
		Writer::writer3MF->WriteToBuffer(buffer);

		Writer::writer3MF->WriteToFile(Writer::OutFolder + "Pyramid.3mf");
		auto bufferFromFile = ReadFileIntoBuffer(Writer::OutFolder + "Pyramid.3mf");

		ASSERT_TRUE(std::equal(buffer.begin(), buffer.end(), bufferFromFile.begin()));
	}

	TEST_F(Writer, 3MFPrecision)
	{
		std::vector<sPosition> vctVertices;
		std::vector<sTriangle> vctTriangles;
		fnCreateBox(vctVertices, vctTriangles);
		auto mesh = model->AddMeshObject();
		mesh->SetGeometry(vctVertices, vctTriangles);

		// This test is atleast functional
		std::vector<Lib3MF_uint8> buffer;
		Writer::writer3MF->WriteToBuffer(buffer);

		Lib3MF_uint32 nPrecission = writer3MF->GetDecimalPrecision();
		writer3MF->SetDecimalPrecision(nPrecission+2);
		std::vector<Lib3MF_uint8> bufferLargr;
		Writer::writer3MF->WriteToBuffer(bufferLargr);

		ASSERT_TRUE(buffer.size() < bufferLargr.size());
	}

	TEST_F(Writer, STLCompare)
	{
		// This test is atleast functional
		std::vector<Lib3MF_uint8> buffer;
		Writer::writerSTL->WriteToBuffer(buffer);

		Writer::writerSTL->WriteToFile(Writer::OutFolder + "Pyramid.stl");
		auto bufferFromFile = ReadFileIntoBuffer(Writer::OutFolder + "Pyramid.stl");

		ASSERT_TRUE(std::equal(buffer.begin(), buffer.end(), bufferFromFile.begin()));
	}

	TEST_F(Writer, 3MFWriteToCallback)
	{
		PositionedVector<Lib3MF_uint8> callbackBuffer;
		Writer::writer3MF->WriteToCallback(PositionedVector<Lib3MF_uint8>::writeCallback,
			PositionedVector<Lib3MF_uint8>::seekCallback, reinterpret_cast<Lib3MF_pvoid>(&callbackBuffer));

		std::vector<Lib3MF_uint8> buffer;
		Writer::writer3MF->WriteToBuffer(buffer);

		ASSERT_TRUE(std::equal(buffer.begin(), buffer.end(), callbackBuffer.vec.begin()));
	}

	TEST_F(Writer, STLWriteToCallback)
	{
		PositionedVector<Lib3MF_uint8> callbackBuffer;
		Writer::writerSTL->WriteToCallback(PositionedVector<Lib3MF_uint8>::writeCallback,
			PositionedVector<Lib3MF_uint8>::seekCallback, reinterpret_cast<Lib3MF_pvoid>(&callbackBuffer));

		std::vector<Lib3MF_uint8> buffer;
		Writer::writerSTL->WriteToBuffer(buffer);

		ASSERT_TRUE(std::equal(buffer.begin(), buffer.end(), callbackBuffer.vec.begin()));
	}

	TEST_F(Writer, ToolpathTest)
	{

		auto pModel = Writer::model;
		auto pObject = pModel->AddMeshObject();
		pObject->SetName("TestObject");

		auto pBinaryStream = Writer::writer3MFz->CreateBinaryStream("Toolpath/layers.dat");

		auto pToolpath = pModel->AddToolpath(0.001);
		auto pProfile1 = pToolpath->AddProfile("profile1");
		pProfile1->SetParameterDoubleValue("", "laserpower", 100.0);
		pProfile1->SetParameterDoubleValue("", "laserspeed", 200.0);

		auto pProfile2 = pToolpath->AddProfile("profile2");
		pProfile1->SetParameterDoubleValue("", "laserpower", 120.0);
		pProfile1->SetParameterDoubleValue("", "laserspeed", 220.0);

		auto pLayer1 = pToolpath->AddLayer(100, "/Toolpath/layer1.xml", Writer::writer3MFz.get());
		Writer::writer3MFz->AssignBinaryStream(pLayer1.get(), pBinaryStream.get() );
		auto nProfileID1 = pLayer1->RegisterProfile(pProfile1.get());
		auto nPartID1 = pLayer1->RegisterPart(pObject.get());
		pLayer1 = nullptr;

/*		std::vector<Lib3MF::sPosition2D> PointsA;
		PointsA.push_back(Lib3MF::sPosition2D{ 100.0, 200.0 });
		PointsA.push_back(Lib3MF::sPosition2D{ 100.0, 300.0 });
		PointsA.push_back(Lib3MF::sPosition2D{ 110.0, 200.0 });
		PointsA.push_back(Lib3MF::sPosition2D{ 110.0, 300.0 });
		pLayer1->WriteHatchData(nProfileID1, nPartID1, PointsA);

		auto pLayer2 = pToolpath->AddLayer(200, "/Toolpath/layer2.xml", Writer::writer3MFz.get());
		auto nProfileID2 = pLayer2->RegisterProfile(pProfile2.get());
		auto nPartID2 = pLayer2->RegisterPart(pObject.get());
		std::vector<Lib3MF::sPosition2D> PointsB;
		PointsB.push_back(Lib3MF::sPosition2D{ 100.0, 200.0 });
		PointsB.push_back(Lib3MF::sPosition2D{ 100.0, 300.0 });
		PointsB.push_back(Lib3MF::sPosition2D{ 110.0, 200.0 });
		PointsB.push_back(Lib3MF::sPosition2D{ 110.0, 300.0 });
		pLayer2->WriteHatchData(nProfileID2, nPartID2, PointsB); */

		Writer::writer3MFz->WriteToFile(Writer::OutFolder + "toolpath.3mf");

	}


	TEST_F(Writer, BinaryMeshTest)
	{

		auto pBinaryStream = Writer::writer3MFz->CreateBinaryStream("Binary/mesh.dat");

		auto Iterator = Writer::model->GetMeshObjects();
		while (Iterator->MoveNext()) {
			auto pMeshObject = Writer::model->GetMeshObjectByID(Iterator->GetCurrent()->GetResourceID());
			Writer::writer3MFz->AssignBinaryStream (pMeshObject.get(), pBinaryStream.get());
		}
		Writer::writer3MFz->WriteToFile(Writer::OutFolder + "binarymesh.3mf");

	}

	TEST_F(Writer, BinaryMeshTestPart)
	{
		auto pModel = wrapper->CreateModel();
		auto pReader = pModel->QueryReader("3mf");
		pReader->ReadFromFile(sTestFilesPath + "Writer/" + "Part4.3mf");

		auto pWriter = pModel->QueryWriter("3mfz");
		auto pBinaryStream = pWriter->CreateBinaryStream("Binary/mesh.dat");

		auto Iterator = pModel->GetMeshObjects();
		while (Iterator->MoveNext()) {
			auto pMeshObject = pModel->GetMeshObjectByID(Iterator->GetCurrent()->GetResourceID());
			pWriter->AssignBinaryStream(pMeshObject.get(), pBinaryStream.get());
		}
		pWriter->WriteToFile(Writer::OutFolder + "binarypart4.3mf");

	}

}
