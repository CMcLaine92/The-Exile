#pragma once
#include "IObject.h"
#include <iostream>


//this is the prop object class
class CTestObject :	public IObject
{
public:
	enum EProps { eTree, eBoulder, eTent };
	CTestObject(string _szTag, XMFLOAT3 position, unsigned int propID);//id matches enum
	~CTestObject();

	void Update();

};

