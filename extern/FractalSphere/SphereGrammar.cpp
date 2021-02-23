// Copyright © 2008 Intel Corporation
// All Rights Reserved
// 
// The sample source code contained or described herein and all documents
// related to the sample source code ("Material") are owned by Intel Corporation
// or its suppliers or licensors. Title to the Material remains with Intel Corporation
// or its suppliers and licensors. The Material may contain trade secrets and
// proprietary and confidential information of Intel Corporation and its suppliers
// and licensors, and is protected by worldwide copyright and trade secret laws and
// treaty provisions. The sample source code is provided AS IS, with no warranties
// of any kind, express or implied. Except as expressly permitted by the Software
// license, neither Intel Corporation nor its suppliers assumes any responsibility
// or liability for any errors or inaccuracies that may appear herein.

#include "SphereGrammar.h"

#pragma warning( push )
#pragma warning( disable : 6011 )

void Grammar::loadLevel(std::string filename)
{
	const char* pcsName;
	const char* pcsValue;
	std::string sTDF("sgtest.tdf");
	TiXmlAttribute *pAset;
	TiXmlDocument   XmlDoc( filename.c_str() );
	int levelID =0;
	int splitCount =0;
	int splitID =0;
	if ( XmlDoc.LoadFile() == false )
	{
		//load failed.  need assert
	}

	//
	// Find the "environment" element.
	//
	TiXmlElement* pTreeGrammarElement = XmlDoc.FirstChildElement();
	TiXmlElement* pGrammarElement=0;
	TiXmlElement* pPropertiesElement=0;
	TiXmlElement* pPropertyElement=0;
	TiXmlElement* pChildElement=0;
	TiXmlElement* pSiblingElement=0;

	assert( pTreeGrammarElement != NULL );

	if(strcmp( pTreeGrammarElement->Value(), "SphereTreeGrammar" ) !=0)
	{
		//Not a valid Grammar file; need assertion

	}

	//Get Grammar Definition Values
	pGrammarElement = pTreeGrammarElement->FirstChildElement();
	if(strcmp(pGrammarElement->Value(),"GrammarElement") ==0)
	{
		pAset = pGrammarElement->FirstAttribute();
		pcsName  = pAset->Name();
		pcsValue = pAset->Value();
		if(!strcmp(pcsName,"Name")&&!strcmp(pcsValue,"GDV"))
		{
			pPropertyElement=pGrammarElement->FirstChildElement();
			while (pPropertyElement != NULL && !strcmp(pPropertyElement->Value(), "Property") !=0 )
			{
				pAset = pPropertyElement->FirstAttribute();
				pcsName  = pAset->Name();
				pcsValue = pAset->Value();
				if(!strcmp(pcsName,"Name")&&!strcmp(pcsValue,"Name"))
				{
					pAset = pAset->Next();
					pcsName  = pAset->Name();
					pcsValue = pAset->Value();
				}
				else if(!strcmp(pcsName,"Name")&&!strcmp(pcsValue,"LevelCount"))
				{
					pAset = pAset->Next();
					pcsName  = pAset->Name();
					pcsValue = pAset->Value();
					m_levelCount = pAset->IntValue();
					m_pLevels = new LevelDetail[m_levelCount];

				}

				pPropertyElement = pPropertyElement->NextSiblingElement();
			}

		}
	}

	//pSiblingElement = pElement->FirstChildElement();
	pGrammarElement = pGrammarElement->NextSiblingElement();
	while ( pGrammarElement != NULL && !strcmp( pGrammarElement->Value(), "GrammarElement" ) != 0 )
	{
		pPropertiesElement = pGrammarElement->FirstChildElement();
		while (pPropertiesElement != NULL && !strcmp(pPropertiesElement->Value(), "Properties") !=0 )
		{
			pAset = pPropertiesElement->FirstAttribute();
			pcsName  = pAset->Name();
			pcsValue = pAset->Value();
			if(!strcmp(pcsName,"GrammarToken")&&!strcmp(pcsValue,"Level"))
			{
				pPropertyElement=pPropertiesElement->FirstChildElement();
				while (pPropertyElement != NULL && !strcmp(pPropertyElement->Value(), "Property") !=0 )
				{
					pAset = pPropertyElement->FirstAttribute();
					pcsName  = pAset->Name();
					pcsValue = pAset->Value();
					if(!strcmp(pcsName,"Name")&&!strcmp(pcsValue,"LevelID"))
					{
						pAset = pAset->Next();
						pcsName  = pAset->Name();
						pcsValue = pAset->Value();
						levelID = pAset->IntValue();
						m_pLevels[levelID].LevelID = levelID;
					}
					else if(!strcmp(pcsName,"Name")&&!strcmp(pcsValue,"latitudePointCount"))
					{
						pAset = pAset->Next();
						pcsName  = pAset->Name();
						pcsValue = pAset->Value();
						m_pLevels[levelID].latitudePointCount = pAset->IntValue();

					}
					else if(!strcmp(pcsName,"Name")&&!strcmp(pcsValue,"longitudePointCount"))
					{
						pAset = pAset->Next();
						pcsName  = pAset->Name();
						pcsValue = pAset->Value();
						m_pLevels[levelID].longitudePointCount = pAset->IntValue();

					}
					else if(!strcmp(pcsName,"Name")&&!strcmp(pcsValue,"AxialBias"))
					{
						pAset = pAset->Next();
						pcsName  = pAset->Name();
						pcsValue = pAset->Value();
						if(!strcmp(pcsName,"MinAngle"))
						{
							m_pLevels[levelID].AxialBias.minAngle = (float)(pAset->DoubleValue()/ RadianDegree);
						} 
						else if(!strcmp(pcsName,"MaxAngle"))
						{
							m_pLevels[levelID].AxialBias.maxAngle = (float)(pAset->DoubleValue()/ RadianDegree);
						} 
					}
					else if(!strcmp(pcsName,"Name")&&!strcmp(pcsValue,"Radius"))
					{
						pAset = pAset->Next();
						pcsName  = pAset->Name();
						pcsValue = pAset->Value();
						m_pLevels[levelID].radius = (float)(pAset->DoubleValue());
					}
					else if(!strcmp(pcsName,"Name")&&!strcmp(pcsValue,"dropAngle"))
					{
						pAset = pAset->Next();
						pcsName  = pAset->Name();
						pcsValue = pAset->Value();
						if(!strcmp(pcsName,"Angle"))
						{
							m_pLevels[levelID].dropAngle = (float)(pAset->DoubleValue()/ RadianDegree);
						} 

					}
					else if(!strcmp(pcsName,"Name")&&!strcmp(pcsValue,"Heading"))
					{
						pAset = pAset->Next();
						pcsName  = pAset->Name();
						pcsValue = pAset->Value();
						if(!strcmp(pcsName,"value1"))
						{
							float x,y,z;
							x = (float)(pAset->DoubleValue());
							pAset = pAset->Next();
							y = (float)(pAset->DoubleValue());
							pAset = pAset->Next();
							z = (float)(pAset->DoubleValue());
							m_pLevels[levelID].heading.Heading = V3(x,y,z);						
						} 

					}
					else if(!strcmp(pcsName,"Name")&&!strcmp(pcsValue,"SphereType"))
					{
						pAset = pAset->Next();
						pcsName  = pAset->Name();
						pcsValue = pAset->Value();
						if(!strcmp(pcsValue,"TRUNK"))
						{
							m_pLevels[levelID].sphrType = sphereType::TRUNK;
						}   
						else if(!strcmp(pcsValue,"CHILD"))
						{
							m_pLevels[levelID].sphrType = sphereType::CHILD;
						}   
						else if(!strcmp(pcsValue,"ELIPSE"))
						{
							m_pLevels[levelID].sphrType = sphereType::ELIPSE;
						}   
					}
					else if(!strcmp(pcsName,"Name")&&!strcmp(pcsValue,"splitListcount"))
					{
						pAset = pAset->Next();
						pcsName  = pAset->Name();
						pcsValue = pAset->Value();
						splitCount = pAset->IntValue();
						m_pLevels[levelID].splitList = new split[splitCount];
						m_pLevels[levelID].splitCount = splitCount;

					}

					pPropertyElement = pPropertyElement->NextSiblingElement();
				}

			}else if(!strcmp(pcsName,"GrammarToken")&&!strcmp(pcsValue,"SplitList"))
			{
				pPropertyElement=pPropertiesElement->FirstChildElement();
				while (pPropertyElement != NULL && !strcmp(pPropertyElement->Value(), "Property") !=0 )
				{
					pAset = pPropertyElement->FirstAttribute();
					pcsName  = pAset->Name();
					pcsValue = pAset->Value();
					if(!strcmp(pcsName,"Name")&&!strcmp(pcsValue,"splitID"))
					{
						pAset = pAset->Next();
						pcsName  = pAset->Name();
						pcsValue = pAset->Value();
						splitID = pAset->IntValue();
					}
					else if(!strcmp(pcsName,"Name")&&!strcmp(pcsValue,"probability"))
					{
						pAset = pAset->Next();
						pcsName  = pAset->Name();
						pcsValue = pAset->Value();
						m_pLevels[levelID].splitList[splitID].probability = (float)(pAset->DoubleValue());
					}
					else if(!strcmp(pcsName,"Name")&&!strcmp(pcsValue,"splitCount"))
					{
						pAset = pAset->Next();
						pcsName  = pAset->Name();
						pcsValue = pAset->Value();
						m_pLevels[levelID].splitList[splitID].splitCount = pAset->IntValue();
					}
					else if(!strcmp(pcsName,"Name")&&!strcmp(pcsValue,"type"))
					{
						pAset = pAset->Next();
						pcsName  = pAset->Name();
						pcsValue = pAset->Value();
						if(!strcmp(pcsValue,"ORDINARY"))
						{
							m_pLevels[levelID].splitList[splitID].type = splitType::ORDINARY;
						}   
						else if(!strcmp(pcsValue,"REPEAT_ORDINARY"))
						{
							m_pLevels[levelID].splitList[splitID].type = splitType::REPEAT_ORDINARY;
						}   
						else if(!strcmp(pcsValue,"FRACTAL_60_FULL"))
						{
							m_pLevels[levelID].splitList[splitID].type = splitType::FRACTAL_60_FULL;
						}   
						else if(!strcmp(pcsValue,"FRACTAL_60"))
						{
							m_pLevels[levelID].splitList[splitID].type = splitType::FRACTAL_60;
						}   
						else if(!strcmp(pcsValue,"FRACTAL_E"))
						{
							m_pLevels[levelID].splitList[splitID].type = splitType::FRACTAL_E;
						}   
						else if(!strcmp(pcsValue,"FRACTAL_Y"))
						{
							m_pLevels[levelID].splitList[splitID].type = splitType::FRACTAL_Y;
						}   
						else if(!strcmp(pcsValue,"FRACTAL_YPEAT"))
						{
							m_pLevels[levelID].splitList[splitID].type = splitType::FRACTAL_YPEAT;
						}   
						else if(!strcmp(pcsValue,"FRACTAL_I"))
						{
							m_pLevels[levelID].splitList[splitID].type = splitType::FRACTAL_I;
						}   
						else if(!strcmp(pcsValue,"FRACTAL_IEND"))
						{
							m_pLevels[levelID].splitList[splitID].type = splitType::FRACTAL_IEND;
						}   
						else if(!strcmp(pcsValue,"FRACTAL_T"))
						{
							m_pLevels[levelID].splitList[splitID].type = splitType::FRACTAL_T;
						}   
						else if(!strcmp(pcsValue,"FRACTAL_S"))
						{
							m_pLevels[levelID].splitList[splitID].type = splitType::FRACTAL_S;
						}   
						else if(!strcmp(pcsValue,"TERMINAL"))
						{
							m_pLevels[levelID].splitList[splitID].type = splitType::TERMINAL;
						}   
					}
					pPropertyElement = pPropertyElement->NextSiblingElement();
				}
			}
			pPropertiesElement = pPropertiesElement->NextSiblingElement();
		}
		pGrammarElement = pGrammarElement->NextSiblingElement();
	}

}
/*  XML Grammar file copied here in case it gets lost
<SphereTreeGrammar>
  <GrammarElement Name="GDV">
    <Property Name="Name" value="STG1"/>
    <Property Name="LevelCount" value="3"/>
  </GrammarElement>
  <GrammarElement>
    <Properties GrammarToken="Level">
      <Property Name="LevelID" value1="0"/>
      <Property Name="longitudePointCount" value1="12"/>
      <Property Name="latitudePointCount" value1="12"/>
      <Property Name="AxialBias" MinAngle="10.0"/>
      <Property Name="AxialBias" MaxAngle="330.0"/>
      <Property Name="Radius" value1="3.0"/>
      <Property Name="dropAngle" Angle="10.0"/>
      <Property Name="Heading" value1="0.0" value2="1.0" value3="0.0"/>
      <Property Name="SphereType" value1="TRUNK"/>
      <Property Name="splitListcount" value1="1"/>
    </Properties>
    <Properties GrammarToken="SplitList">
      <Property Name="splitID" value1="0"/>
      <Property Name="probability" value1="1.0"/>
      <Property Name="splitCount" value1="9"/>
      <Property Name="type" value1="FRACTAL_60"/>
    </Properties>
  </GrammarElement>
  <GrammarElement>
    <Properties GrammarToken="Level">
      <Property Name="LevelID" value1="1"/>
      <Property Name="longitudePointCount" value1="12"/>
      <Property Name="latitudePointCount" value1="12"/>
      <Property Name="AxialBias" MinAngle="30.0"/>
      <Property Name="AxialBias" MaxAngle="330.0"/>
      <Property Name="Radius" value1="1.2"/>
      <Property Name="dropAngle" Angle="10.0"/>
      <Property Name="SphereType" value1="CHILD"/>
      <Property Name="splitListcount" value1="1"/>
    </Properties>
    <Properties GrammarToken="SplitList">
      <Property Name="splitID" value1="0"/>
      <Property Name="probability" value1="1.0"/>
      <Property Name="splitCount" value1="9"/>
      <Property Name="type" value1="FRACTAL_60"/>
    </Properties>
  </GrammarElement>
  <GrammarElement>
    <Properties GrammarToken="Level">
      <Property Name="LevelID" value1="2"/>
      <Property Name="longitudePointCount" value1="12"/>
      <Property Name="latitudePointCount" value1="12"/>
      <Property Name="AxialBias" MinAngle="30.0"/>
      <Property Name="AxialBias" MaxAngle="330.0"/>
      <Property Name="Radius" value1="0.48"/>
      <Property Name="dropAngle" Angle="10.0"/>
      <Property Name="SphereType" value1="CHILD"/>
      <Property Name="splitListcount" value1="1"/>
    </Properties>
    <Properties GrammarToken="SplitList">
      <Property Name="splitID" value1="0"/>
      <Property Name="probability" value1="1.0"/>
      <Property Name="splitCount" value1="0"/>
      <Property Name="type" value1="TERMINAL"/>
    </Properties>
  </GrammarElement>

</SphereTreeGrammar>

*/

#pragma warning( pop )
