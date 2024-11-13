#include "cMain.h"

wxBEGIN_EVENT_TABLE(cMain, wxFrame)
EVT_BUTTON(wxID_ANY, OnStraightLineButtoClicked)
EVT_BUTTON(wxID_ANY+1, OnFreeCurveButtonClicked)
EVT_BUTTON(wxID_ANY+2, OnBezierCurveButtonClicked)
EVT_BUTTON(wxID_ANY+3, OnAndGateButtonClicked)
EVT_BUTTON(wxID_ANY+4, OnOrGateButtonClicked)
EVT_BUTTON(wxID_ANY+5, OnNotGateButtonClicked)

wxEND_EVENT_TABLE()


//��ť���ü��¼���
cMain::cMain() : wxFrame(nullptr, wxID_ANY, "Hello wxWidgets!", wxPoint(30, 30), wxSize(800, 600))
{	
	//ͼƬ·���Լ���ʽת��
	wxInitAllImageHandlers();

	lineImage = CreateImage(lineImage, "resource/image/select.png");
	andGateImage = CreateImage(andGateImage, "resource/image/andgate.png");
	orGateImage = CreateImage(orGateImage, "resource/image/orgate.png");
	notGateImage = CreateImage(notGateImage, "resource/image/notgate.png");

	//��ť����
	m_btn1 = new wxBitmapButton(this, wxID_ANY, lineImage, wxPoint(10, 10), wxSize(50, 50));
	Bind(wxEVT_BUTTON, &cMain::OnStraightLineButtoClicked, this, m_btn1->GetId());

	m_btn2 = new wxBitmapButton(this, wxID_ANY + 1, lineImage, wxPoint(10, 70), wxSize(50, 50));
	Bind(wxEVT_BUTTON, &cMain::OnFreeCurveButtonClicked, this, m_btn2->GetId());

	BezierCurve_btn = new wxBitmapButton(this, wxID_ANY + 2, lineImage, wxPoint(10, 130), wxSize(50, 50));
	Bind(wxEVT_BUTTON, &cMain::OnBezierCurveButtonClicked, this, BezierCurve_btn->GetId());

	//��������----��������壬�Ҳ��ͼ���򴴽�
	drawboard = new DrawBoard(this);
	wxBoxSizer* controlboard = new wxBoxSizer(wxVERTICAL);
	controlboard->Add(m_btn1, 0, wxALL | wxEXPAND, 5);
	controlboard->Add(m_btn2, 0, wxALL | wxEXPAND, 5);
	wxBoxSizer* mainSizer = new wxBoxSizer(wxHORIZONTAL);
	mainSizer->Add(controlboard, 0, wxALL | wxEXPAND, 5);
	mainSizer->Add(drawboard, 1, wxALL | wxEXPAND, 5);
	SetSizer(mainSizer);

	andGate_btn = new wxBitmapButton(this, wxID_ANY + 3, andGateImage, wxPoint(10, 190), wxSize(50, 50));
	Bind(wxEVT_BUTTON, &cMain::OnAndGateButtonClicked, this, andGate_btn->GetId());

	orGate_btn = new wxBitmapButton(this, wxID_ANY + 4, orGateImage, wxPoint(10, 250), wxSize(50, 50));
	Bind(wxEVT_BUTTON, &cMain::OnOrGateButtonClicked, this, orGate_btn->GetId());

	notGate_btn = new wxBitmapButton(this, wxID_ANY + 5, notGateImage, wxPoint(10, 310), wxSize(50, 50));
	Bind(wxEVT_BUTTON, &cMain::OnNotGateButtonClicked, this, notGate_btn->GetId());

}

//����ͼƬ
wxBitmap cMain::CreateImage(wxBitmap image, wxString path) {
	bool ifok = image.LoadFile(path, wxBITMAP_TYPE_PNG);
	wxImage imageForSelect = image.ConvertToImage();
	imageForSelect = imageForSelect.Scale(20, 20, wxIMAGE_QUALITY_HIGH);
	image = wxBitmap(imageForSelect);
	return image;

}

//ֱ�ߣ��������ߣ����������ߵ���¼�
void cMain::OnStraightLineButtoClicked(wxCommandEvent& evt) {
	drawboard->isLineDrawing = true;
	drawboard->isFreeDrawing = false;
	drawboard->firstPointSet = false;
	drawboard->isBezierDrawing = false;
}
void cMain::OnFreeCurveButtonClicked(wxCommandEvent& evt)
{
	drawboard->isLineDrawing = false;
	drawboard->isFreeDrawing = true;
	drawboard->isBezierDrawing = false;
}
void cMain::OnBezierCurveButtonClicked(wxCommandEvent& evt) {
	drawboard->isLineDrawing = false;
	drawboard->isFreeDrawing = false;
	drawboard->isBezierDrawing = true;
}

//�����Ű�ť����¼�
void cMain::OnAndGateButtonClicked(wxCommandEvent& evt) {
	drawboard->isLineDrawing = false;
	drawboard->isFreeDrawing = false;
	drawboard->isBezierDrawing = false;
	drawboard->placingAndGate = true;
	drawboard->placingOrGate = false;
	drawboard->placingNotGate = false;
}
void cMain::OnOrGateButtonClicked(wxCommandEvent& evt) {
	drawboard->isLineDrawing = false;
	drawboard->isFreeDrawing = false;
	drawboard->isBezierDrawing = false;
	drawboard->placingAndGate = false;
	drawboard->placingOrGate = true;
	drawboard->placingNotGate = false;
}
void cMain::OnNotGateButtonClicked(wxCommandEvent& evt) {
	drawboard->isLineDrawing = false;
	drawboard->isFreeDrawing = false;
	drawboard->isBezierDrawing = false;
	drawboard->placingAndGate = false;
	drawboard->placingOrGate = false;
	drawboard->placingNotGate = true;
}


cMain::~cMain() {

}
