#include "cMain.h"
#include <wx/timer.h>

wxBEGIN_EVENT_TABLE(cMain, wxFrame)
EVT_BUTTON(wxID_ANY, OnStraightLineButtonClicked)
EVT_BUTTON(wxID_ANY+1, OnFreeCurveButtonClicked)
EVT_BUTTON(wxID_ANY+2, OnBezierCurveButtonClicked)
EVT_BUTTON(wxID_ANY+3, OnAndGateButtonClicked)
EVT_BUTTON(wxID_ANY+4, OnOrGateButtonClicked)
EVT_BUTTON(wxID_ANY+5, OnNotGateButtonClicked)
EVT_BUTTON(wxID_ANY+6, OnTextBoxButtonClicked)
wxEND_EVENT_TABLE()

enum DrawMode {
	MODE_NONE,          // �޲���
	MODE_CUSTOM,		// �Զ���
	MODE_LINE,          // ֱ�߻���
	MODE_FREE_CURVE,    // ��������
	MODE_BEZIER_CURVE,  // ����������
	MODE_AND_GATE,      // ����
	MODE_OR_GATE,       // ����
	MODE_NOT_GATE,      // ����
	MODE_TEXT_BOX,		//�ı���
	MODE_RESISTOR,
	MODE_CAPACITOR,
	MODE_DIODE,
	MODE_POWER
};

DrawMode currentMode = MODE_NONE;

//��ť���ü��¼���
cMain::cMain(wxFrame* parent) : wxFrame(nullptr, wxID_ANY, "Hello wxWidgets!", wxPoint(30, 30), wxSize(800, 600))
{	
	refreshTimer = new wxTimer(this);
	refreshTimer->Start(1000);  // ÿ��1000���봥��һ��
	Bind(wxEVT_TIMER, &cMain::OnTimer, this);

	wxInitAllImageHandlers();

	customImage = CreateImage(customImage, "resource/image/custom.png");
	noneImage = CreateImage(noneImage, "resource/image/select.png");
	lineImage = CreateImage(lineImage, "resource/image/line.png");
	freeImage = CreateImage(freeImage, "resource/image/free.png");
	curveImage = CreateImage(curveImage, "resource/image/curve.png");
	andGateImage = CreateImage(andGateImage, "resource/image/andgate.png");
	orGateImage = CreateImage(orGateImage, "resource/image/orgate.png");
	notGateImage = CreateImage(notGateImage, "resource/image/notgate.png");
	textBoxImage = CreateImage(textBoxImage, "resource/image/textbox.png");
	resistorImage = CreateImage(resistorImage, "resource/image/resistor.png");
	capacitorImage = CreateImage(capacitorImage, "resource/image/capacitor.png");
	diodeImage = CreateImage(diodeImage, "resource/image/diode.png");
	powerImage = CreateImage(powerImage, "resource/image/power.png");

	custom_btn = new wxBitmapButton(this, wxID_ANY + 300, customImage, wxPoint(10, 10), wxSize(40, 40));
	Bind(wxEVT_BUTTON, &cMain::OnCustomComponentButtonClicked, this, custom_btn->GetId());
	none_btn = new wxBitmapButton(this, wxID_ANY + 301, noneImage, wxPoint(10, 60), wxSize(40, 40));
	Bind(wxEVT_BUTTON, &cMain::OnNoneButtonClicked, this, none_btn->GetId());
	Line_btn = new wxBitmapButton(this, wxID_ANY, lineImage, wxPoint(10, 110), wxSize(40, 40));
	Bind(wxEVT_BUTTON, &cMain::OnStraightLineButtonClicked, this, Line_btn->GetId());
	Free_btn = new wxBitmapButton(this, wxID_ANY + 1, freeImage, wxPoint(10, 160), wxSize(40, 40));
	Bind(wxEVT_BUTTON, &cMain::OnFreeCurveButtonClicked, this, Free_btn->GetId());
	BezierCurve_btn = new wxBitmapButton(this, wxID_ANY + 2, curveImage, wxPoint(10, 210), wxSize(40, 40));
	Bind(wxEVT_BUTTON, &cMain::OnBezierCurveButtonClicked, this, BezierCurve_btn->GetId());
	andGate_btn = new wxBitmapButton(this, wxID_ANY + 3, andGateImage, wxPoint(10, 260), wxSize(40, 40));
	Bind(wxEVT_BUTTON, &cMain::OnAndGateButtonClicked, this, andGate_btn->GetId());
	orGate_btn = new wxBitmapButton(this, wxID_ANY + 4, orGateImage, wxPoint(10, 310), wxSize(40, 40));
	Bind(wxEVT_BUTTON, &cMain::OnOrGateButtonClicked, this, orGate_btn->GetId());
	notGate_btn = new wxBitmapButton(this, wxID_ANY + 5, notGateImage, wxPoint(10, 360), wxSize(40, 40));
	Bind(wxEVT_BUTTON, &cMain::OnNotGateButtonClicked, this, notGate_btn->GetId());
	textBox_btn = new wxBitmapButton(this, wxID_ANY + 6, textBoxImage, wxPoint(10, 410), wxSize(40, 40));
	Bind(wxEVT_BUTTON, &cMain::OnTextBoxButtonClicked, this, textBox_btn->GetId());
	resistor_btn = new wxBitmapButton(this, wxID_ANY + 7, resistorImage, wxPoint(10, 460), wxSize(40, 40));
	Bind(wxEVT_BUTTON, &cMain::OnResistorButtonClicked, this, resistor_btn->GetId());
	capacitor_btn = new wxBitmapButton(this, wxID_ANY + 8, capacitorImage, wxPoint(10, 510), wxSize(40, 40));
	Bind(wxEVT_BUTTON, &cMain::OnCapacitorButtonClicked, this, capacitor_btn->GetId());
	diode_btn = new wxBitmapButton(this, wxID_ANY + 9, diodeImage, wxPoint(10, 560), wxSize(40, 40));
	Bind(wxEVT_BUTTON, &cMain::OnDiodeButtonClicked, this, diode_btn->GetId());
	power_btn = new wxBitmapButton(this, wxID_ANY + 10, powerImage, wxPoint(10, 610), wxSize(40, 40));
	Bind(wxEVT_BUTTON, &cMain::OnPowerButtonClicked, this, power_btn->GetId());
#pragma region ��������
	//����������
	wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
	SetSizer(mainSizer);

	//�Ϸ���ť�Ĳ���
	wxBoxSizer* topSizer = new wxBoxSizer(wxHORIZONTAL);
	wxPanel* topPanel = new wxPanel(this);
	topPanel->SetBackgroundColour(*wxWHITE);
	fileBtn = new wxButton(topPanel, wxID_ANY, "�ļ�");
	propertyBtn = new wxButton(topPanel, wxID_ANY, "����");
	windowBtn = new wxButton(topPanel, wxID_ANY, "�Ӵ�");
	dataBtn = new wxButton(topPanel, wxID_ANY, "����");
	topSizer->Add(fileBtn, 0, wxALL | wxEXPAND, 5);
	topSizer->Add(propertyBtn, 0, wxALL | wxEXPAND, 5);
	topSizer->Add(windowBtn, 0, wxALL | wxEXPAND, 5);
	topSizer->Add(dataBtn, 0, wxALL | wxEXPAND, 5);
	topPanel->SetSizerAndFit(topSizer);
	mainSizer->Add(topPanel, 0, wxALL | wxEXPAND, 5);

	//�����壬����͹���������
	wxBoxSizer* bottomSizer = new wxBoxSizer(wxHORIZONTAL);
	propertyPanel = new wxPanel(this);
	propertyPanel->SetBackgroundColour(*wxLIGHT_GREY);
	propertySizer = new wxBoxSizer(wxVERTICAL);

	// ����
	wxStaticText* nameLabel = new wxStaticText(propertyPanel, wxID_ANY, "����:");
	wxTextCtrl* nameCtrl = new wxTextCtrl(propertyPanel, wxID_ANY + 100, "-");
	propertySizer->Add(nameLabel, 0, wxALL | wxEXPAND, 5);
	propertySizer->Add(nameCtrl, 0, wxALL | wxEXPAND, 5);

	// λ��
	wxStaticText* positionLabel = new wxStaticText(propertyPanel, wxID_ANY, "λ�� (x, y):");
	wxTextCtrl* positionX = new wxTextCtrl(propertyPanel, wxID_ANY + 101, "-");
	wxTextCtrl* positionY = new wxTextCtrl(propertyPanel, wxID_ANY + 102, "-");
	wxBoxSizer* positionSizer = new wxBoxSizer(wxHORIZONTAL);
	positionSizer->Add(positionX, 1, wxALL | wxEXPAND, 5);
	positionSizer->Add(positionY, 1, wxALL | wxEXPAND, 5);
	propertySizer->Add(positionLabel, 0, wxALL | wxEXPAND, 5);
	propertySizer->Add(positionSizer, 0, wxALL | wxEXPAND, 5);

	// ��С
	wxStaticText* sizeLabel = new wxStaticText(propertyPanel, wxID_ANY, "��С (��, ��):");
	wxTextCtrl* sizeWidth = new wxTextCtrl(propertyPanel, wxID_ANY + 103, "-");
	wxTextCtrl* sizeHeight = new wxTextCtrl(propertyPanel, wxID_ANY + 104, "-");
	wxBoxSizer* sizeSizer = new wxBoxSizer(wxHORIZONTAL);
	sizeSizer->Add(sizeWidth, 1, wxALL | wxEXPAND, 5);
	sizeSizer->Add(sizeHeight, 1, wxALL | wxEXPAND, 5);
	propertySizer->Add(sizeLabel, 0, wxALL | wxEXPAND, 5);
	propertySizer->Add(sizeSizer, 0, wxALL | wxEXPAND, 5);

	// �Ƕ�
	wxStaticText* angleLabel = new wxStaticText(propertyPanel, wxID_ANY, "�Ƕ�:");
	wxTextCtrl* angleCtrl = new wxTextCtrl(propertyPanel, wxID_ANY + 105, "-");
	propertySizer->Add(angleLabel, 0, wxALL | wxEXPAND, 5);
	propertySizer->Add(angleCtrl, 0, wxALL | wxEXPAND, 5);

	//����״̬(in1, in2)
	wxStaticText* inLabel = new wxStaticText(propertyPanel, wxID_ANY, "����״̬:");
	wxTextCtrl* in1Ctrl = new wxTextCtrl(propertyPanel, wxID_ANY + 106, "-");
	wxTextCtrl* in2Ctrl = new wxTextCtrl(propertyPanel, wxID_ANY + 107, "-");
	wxBoxSizer* inSizer = new wxBoxSizer(wxHORIZONTAL);
	inSizer->Add(in1Ctrl, 1, wxALL | wxEXPAND, 5);
	inSizer->Add(in2Ctrl, 1, wxALL | wxEXPAND, 5);

	// ���״̬ (out)
	wxStaticText* outLabel = new wxStaticText(propertyPanel, wxID_ANY, "���״̬:");
	wxTextCtrl* outCtrl = new wxTextCtrl(propertyPanel, wxID_ANY + 108, "-");
	wxBoxSizer* outSizer = new wxBoxSizer(wxHORIZONTAL);
	outSizer->Add(outCtrl, 1, wxALL | wxEXPAND, 5);
	propertySizer->Add(inLabel, 0, wxALL | wxEXPAND, 5);
	propertySizer->Add(inSizer, 0, wxALL | wxEXPAND, 5);
	propertySizer->Add(outLabel, 0, wxALL | wxEXPAND, 5);
	propertySizer->Add(outSizer, 0, wxALL | wxEXPAND, 5);

	propertyPanel->SetSizerAndFit(propertySizer);
	propertyPanel->Show(false);  // ����

	drawboard = new DrawBoard(this);
	wxBoxSizer* controlBoard = new wxBoxSizer(wxVERTICAL);
	controlBoard->Add(custom_btn, 0, wxALL | wxEXPAND, 5);
	controlBoard->Add(none_btn, 0, wxALL | wxEXPAND, 5);
	controlBoard->Add(Line_btn, 0, wxALL | wxEXPAND, 5);
	controlBoard->Add(Free_btn, 0, wxALL | wxEXPAND, 5);
	controlBoard->Add(BezierCurve_btn, 0, wxALL | wxEXPAND, 5);
	controlBoard->Add(andGate_btn, 0, wxALL | wxEXPAND, 5);
	controlBoard->Add(orGate_btn, 0, wxALL | wxEXPAND, 5);
	controlBoard->Add(notGate_btn, 0, wxALL | wxEXPAND, 5);
	controlBoard->Add(textBox_btn, 0, wxALL | wxEXPAND, 5);
	controlBoard->Add(resistor_btn, 0, wxALL | wxEXPAND, 5);
	controlBoard->Add(capacitor_btn, 0, wxALL | wxEXPAND, 5);
	controlBoard->Add(diode_btn, 0, wxALL | wxEXPAND, 5);
	controlBoard->Add(power_btn, 0, wxALL | wxEXPAND, 5);
	bottomSizer->Add(propertyPanel, 0, wxALL | wxEXPAND, 5);
	bottomSizer->Add(drawboard, 1, wxALL | wxEXPAND, 5);
	bottomSizer->Add(controlBoard, 0, wxALL | wxEXPAND, 5);
	mainSizer->Add(bottomSizer, 1, wxALL | wxEXPAND, 5);

#pragma endregion

	
	//���ð�ť���¼�
	fileBtn->Bind(wxEVT_BUTTON, &cMain::OnFileButtonClicked, this);
	propertyBtn->Bind(wxEVT_BUTTON, &cMain::OnPropertyButtonClicked, this);
	windowBtn->Bind(wxEVT_BUTTON, &cMain::OnWindowButtonClicked, this);
	dataBtn->Bind(wxEVT_BUTTON, &cMain::OnDataButtonClicked, this);
	custom_btn->Bind(wxEVT_BUTTON, &cMain::OnCustomComponentButtonClicked, this);
}

//����ͼƬ
wxBitmap cMain::CreateImage(wxBitmap image, wxString path) {
	bool ifok = image.LoadFile(path, wxBITMAP_TYPE_PNG);
	wxImage imageForSelect = image.ConvertToImage();
	imageForSelect = imageForSelect.Scale(20, 20, wxIMAGE_QUALITY_HIGH);
	image = wxBitmap(imageForSelect);
	return image;

}

void cMain::OnNoneButtonClicked(wxCommandEvent& evt) {
	currentMode = MODE_NONE;
	UpdateDrawingState();
}
void cMain::OnStraightLineButtonClicked(wxCommandEvent& evt) {
	currentMode = MODE_LINE;
	UpdateDrawingState();
}
void cMain::OnFreeCurveButtonClicked(wxCommandEvent& evt)
{
	currentMode = MODE_FREE_CURVE;
	UpdateDrawingState();
}
void cMain::OnBezierCurveButtonClicked(wxCommandEvent& evt) {
	currentMode = MODE_BEZIER_CURVE;
	UpdateDrawingState();
}
void cMain::OnAndGateButtonClicked(wxCommandEvent& evt) {
	currentMode = MODE_AND_GATE;
	UpdateDrawingState();
}
void cMain::OnOrGateButtonClicked(wxCommandEvent& evt) {
	currentMode = MODE_OR_GATE;
	UpdateDrawingState();
}
void cMain::OnNotGateButtonClicked(wxCommandEvent& evt) {
	currentMode = MODE_NOT_GATE;
	UpdateDrawingState();
}
void cMain::OnTextBoxButtonClicked(wxCommandEvent& evt) {
	currentMode = MODE_TEXT_BOX;
	UpdateDrawingState();
}

void cMain::UpdateDrawingState() {
	// ��ʼ������ģʽΪ false
	drawboard->isLineDrawing = false;
	drawboard->isFreeDrawing = false;
	drawboard->isBezierDrawing = false;
	drawboard->placingAndGate = false;
	drawboard->placingOrGate = false;
	drawboard->placingNotGate = false;
	drawboard->placingResistor = false;
	drawboard->placingPower = false;

	switch (currentMode) {
	case MODE_LINE:
		drawboard->isLineDrawing = true;
		break;
	case MODE_FREE_CURVE:
		drawboard->isFreeDrawing = true;
		break;
	case MODE_BEZIER_CURVE:
		drawboard->isBezierDrawing = true;
		break;
	case MODE_AND_GATE:
		drawboard->placingAndGate = true;
		break;
	case MODE_OR_GATE:
		drawboard->placingOrGate = true;
		break;
	case MODE_NOT_GATE:
		drawboard->placingNotGate = true;
		break;
	case MODE_TEXT_BOX:
		drawboard->placingTextBox = true;
		break;
	case MODE_RESISTOR:
		drawboard->placingResistor = true;
		break;
	case MODE_CAPACITOR:
		drawboard->placingCapacitor = true;
		break;
	case MODE_DIODE:
		drawboard->placingDiode = true;
		break;
	case MODE_POWER:
		drawboard->placingPower = true;
		break;

	default:
		break;
	}
}
void cMain::OnFileButtonClicked(wxCommandEvent& event)
{
	wxMenu fileMenu;

	fileMenu.Append(wxID_ANY, "����json");
	fileMenu.Append(wxID_ANY, "����json");
	fileMenu.Append(wxID_ANY, "����txt");
	fileMenu.Append(wxID_ANY, "����txt");
	fileMenu.Append(wxID_ANY, "��ӡPNG");
	fileMenu.Append(wxID_ANY, "�汾");

	PopupMenu(&fileMenu);
}
void cMain::OnPropertyButtonClicked(wxCommandEvent& event) {
	// �л�����������ʾ������
	if (propertyPanel->IsShown()) {
		propertyPanel->Hide();
	}
	else {
		propertyPanel->Show();
	}
	Layout();
}
void cMain::OnWindowButtonClicked(wxCommandEvent& event) {
	// ��������Ի���
	ZoomSliderDialog* dialog = new ZoomSliderDialog(this, drawboard->scale);
	if (dialog->ShowModal() == wxID_OK) {
		float newScale = dialog->GetScale();
		drawboard->SetScale(newScale); // ���»��������ű���
	}
	dialog->Destroy();
}
void cMain::OnDataButtonClicked(wxCommandEvent& event) {
	std::vector<Component> components = drawboard->GetAllComponents(); // ��ȡ����Ԫ����
	ComponentDataDialog* dialog = new ComponentDataDialog(this, components);
	dialog->ShowModal(); // ��ʾ�Ի���
	dialog->Destroy();
}
void cMain::UpdatePropertyPanel() {
	Component* selectedComponent = drawboard->GetSelectedComponent();

	if (!selectedComponent) {
		for (auto child : propertyPanel->GetChildren()) {
			wxTextCtrl* textCtrl = dynamic_cast<wxTextCtrl*>(child);
			if (textCtrl) {
				textCtrl->SetValue("-");  // ����ΪĬ��ֵ
			}
		}
		return;
	}
	else if (selectedComponent) {
		//����
		nameCtrl = dynamic_cast<wxTextCtrl*>(propertyPanel->FindWindowById(wxID_ANY + 100));
		if (nameCtrl) {
			if (selectedComponent->type == ANDGate) {
				nameCtrl->SetValue("����");
			}
			else if (selectedComponent->type == ORGate) {
				nameCtrl->SetValue("����");
			}
			else if (selectedComponent->type == NOTGate) {
				nameCtrl->SetValue("����");
			}
			else if (selectedComponent->type == Resistor) {
				nameCtrl->SetValue("����");
			}
			else if (selectedComponent->type == Capacitor) {
				nameCtrl->SetValue("����");
			}
			else if (selectedComponent->type == Diode) {
				nameCtrl->SetValue("������");
			}
			else if (selectedComponent->type == Power) {
				nameCtrl->SetValue("����Դ");
			}
		}

		//λ��
		positionX = dynamic_cast<wxTextCtrl*>(propertyPanel->FindWindowById(wxID_ANY + 101));
		positionY = dynamic_cast<wxTextCtrl*>(propertyPanel->FindWindowById(wxID_ANY + 102));
		if (positionX && positionY) {
			positionX->SetValue(wxString::Format("%d", selectedComponent->center.x));
			positionY->SetValue(wxString::Format("%d", selectedComponent->center.y));
		}

		//��С
		scaleX = dynamic_cast<wxTextCtrl*>(propertyPanel->FindWindowById(wxID_ANY + 103));
		scaleY = dynamic_cast<wxTextCtrl*>(propertyPanel->FindWindowById(wxID_ANY + 104));
		if (scaleX && scaleY) {
			scaleX->SetValue(wxString::Format("%.2f", selectedComponent->scale));
			scaleY->SetValue(wxString::Format("%.2f", selectedComponent->scale));
		}
		
		//�Ƕ�
		angle = dynamic_cast<wxTextCtrl*>(propertyPanel->FindWindowById(wxID_ANY + 105));
		if (angle) {
			angle->SetValue(wxString::Format("%.2f", selectedComponent->rotationAngle));
		}
	}
	
	propertyPanel->Layout();
}
void cMain::OnTimer(wxTimerEvent& event) {
	UpdatePropertyPanel();
}
void cMain::OnCustomComponentButtonClicked(wxCommandEvent& event) {
	currentMode = MODE_CUSTOM;
	UpdateDrawingState();
	CustomComponentEditor* editor = new CustomComponentEditor(this);
	editor->ShowModal();
	editor->Destroy();
}
void cMain::OnResistorButtonClicked(wxCommandEvent& event) {
	currentMode = MODE_RESISTOR;
	UpdateDrawingState();
}
void cMain::OnCapacitorButtonClicked(wxCommandEvent& event) {
	currentMode = MODE_CAPACITOR;
	UpdateDrawingState();
}
void cMain::OnDiodeButtonClicked(wxCommandEvent& event) {
	currentMode = MODE_DIODE;
	UpdateDrawingState();
}
void cMain::OnPowerButtonClicked(wxCommandEvent& event) {
	currentMode = MODE_POWER;
	UpdateDrawingState();
}
cMain::~cMain() {
	if (refreshTimer) {
		refreshTimer->Stop();
		delete refreshTimer;
	}
}
