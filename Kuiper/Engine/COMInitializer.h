#pragma once

#include "Engine/WinDefs.h"

class COMInitializer
{
public:
	COMInitializer();
	~COMInitializer();
private:
	HRESULT hr;
};