#pragma once
#include <wx/wx.h>
#include "DrawBoard.h"

class cMain : public wxFrame
{
public:
	cMain();
	~cMain();

public:
	wxButton* m_btn1 = nullptr;
	wxButton* m_btn2 = nullptr;
	wxButton* BezierCurve_btn = nullptr;
	wxButton* andGate_btn = nullptr;
	wxButton* orGate_btn = nullptr;
	wxButton* notGate_btn = nullptr;
	DrawBoard* drawboard;
	wxBitmap lineImage;
	wxBitmap andGateImage;
	wxBitmap orGateImage;
	wxBitmap notGateImage;


	wxBitmap CreateImage(wxBitmap image, wxString path);
	void OnStraightLineButtoClicked(wxCommandEvent& evt);
	void OnFreeCurveButtonClicked(wxCommandEvent& event);
	void OnBezierCurveButtonClicked(wxCommandEvent& event);
	void OnAndGateButtonClicked(wxCommandEvent& event);
	void OnOrGateButtonClicked(wxCommandEvent& event);
	void OnNotGateButtonClicked(wxCommandEvent& event);
	wxDECLARE_EVENT_TABLE();
};

