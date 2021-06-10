#pragma once

#include "Biblioteki.h"
class CElementy_rysowalne
{
public:
	CElementy_rysowalne();
	virtual void rysuj(RenderWindow* ptr)=0;
	virtual ~CElementy_rysowalne();
};