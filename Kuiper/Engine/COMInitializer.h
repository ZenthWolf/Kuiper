#pragma once

#include "WinDefs.h"

class COMInitializer
{
public:
	COMInitializer();
	~COMInitializer();
private:
	HRESULT hr;
};