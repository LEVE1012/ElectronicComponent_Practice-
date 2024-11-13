#pragma once
#include <wx/wx.h>
#include "cMain.h"

class cApp:public wxApp
{
public:
	cApp();
	~cApp();

private:
	cMain * m_fram1 = nullptr;

public:
	virtual bool OnInit();
};

