#pragma once

#include <wx/wx.h>

class MyApp : public wxApp
{
public:
	MyApp();
	~MyApp();
public:
	virtual bool OnInit();
};

