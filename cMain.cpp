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
	MODE_NONE,          // 无操作
	MODE_LINE,          // 直线绘制
	MODE_FREE_CURVE,    // 自由曲线
	MODE_BEZIER_CURVE,  // 贝塞尔曲线
	MODE_AND_GATE,      // 与门
	MODE_OR_GATE,       // 或门
	MODE_NOT_GATE,      // 非门
	MODE_TEXT_BOX		//文本框
};

DrawMode currentMode = MODE_NONE;

//按钮设置及事件绑定
cMain::cMain() : wxFrame(nullptr, wxID_ANY, "Hello wxWidgets!", wxPoint(30, 30), wxSize(800, 600))
{	
	refreshTimer = new wxTimer(this);
	refreshTimer->Start(1000);  // 每隔1000毫秒触发一次
	Bind(wxEVT_TIMER, &cMain::OnTimer, this);

	wxInitAllImageHandlers();

	lineImage = CreateImage(lineImage, "resource/image/select.png");
	andGateImage = CreateImage(andGateImage, "resource/image/andgate.png");
	orGateImage = CreateImage(orGateImage, "resource/image/orgate.png");
	notGateImage = CreateImage(notGateImage, "resource/image/notgate.png");
	textBoxImage = CreateImage(textBoxImage, "resource/image/textbox.png");

	m_btn1 = new wxBitmapButton(this, wxID_ANY, lineImage, wxPoint(10, 70), wxSize(50, 50));
	Bind(wxEVT_BUTTON, &cMain::OnStraightLineButtonClicked, this, m_btn1->GetId());
	m_btn2 = new wxBitmapButton(this, wxID_ANY + 1, lineImage, wxPoint(10, 130), wxSize(50, 50));
	Bind(wxEVT_BUTTON, &cMain::OnFreeCurveButtonClicked, this, m_btn2->GetId());
	BezierCurve_btn = new wxBitmapButton(this, wxID_ANY + 2, lineImage, wxPoint(10, 190), wxSize(50, 50));
	Bind(wxEVT_BUTTON, &cMain::OnBezierCurveButtonClicked, this, BezierCurve_btn->GetId());
	andGate_btn = new wxBitmapButton(this, wxID_ANY + 3, andGateImage, wxPoint(10, 250), wxSize(50, 50));
	Bind(wxEVT_BUTTON, &cMain::OnAndGateButtonClicked, this, andGate_btn->GetId());
	orGate_btn = new wxBitmapButton(this, wxID_ANY + 4, orGateImage, wxPoint(10, 310), wxSize(50, 50));
	Bind(wxEVT_BUTTON, &cMain::OnOrGateButtonClicked, this, orGate_btn->GetId());
	notGate_btn = new wxBitmapButton(this, wxID_ANY + 5, notGateImage, wxPoint(10, 370), wxSize(50, 50));
	Bind(wxEVT_BUTTON, &cMain::OnNotGateButtonClicked, this, notGate_btn->GetId());
	textBox_btn = new wxBitmapButton(this, wxID_ANY + 6, textBoxImage, wxPoint(10, 430), wxSize(50, 50));
	Bind(wxEVT_BUTTON, &cMain::OnTextBoxButtonClicked, this, textBox_btn->GetId());

	//创建主布局
	wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
	SetSizer(mainSizer);
	
	//上方按钮的布局
	wxBoxSizer* topSizer = new wxBoxSizer(wxHORIZONTAL);
	wxPanel* topPanel = new wxPanel(this);
	topPanel->SetBackgroundColour(*wxWHITE);
	fileBtn = new wxButton(topPanel, wxID_ANY, "文件");
	propertyBtn = new wxButton(topPanel, wxID_ANY, "属性");
	windowBtn = new wxButton(topPanel, wxID_ANY, "视窗");
	dataBtn = new wxButton(topPanel, wxID_ANY, "数据");
	topSizer->Add(fileBtn, 0, wxALL | wxEXPAND, 5);
	topSizer->Add(propertyBtn, 0, wxALL | wxEXPAND, 5);
	topSizer->Add(windowBtn, 0, wxALL | wxEXPAND, 5);
	topSizer->Add(dataBtn, 0, wxALL | wxEXPAND, 5);
	topPanel->SetSizerAndFit(topSizer);
	mainSizer->Add(topPanel, 0, wxALL | wxEXPAND, 5);

	//左侧面板，画板和工具栏布局
	wxBoxSizer* bottomSizer = new wxBoxSizer(wxHORIZONTAL);
	propertyPanel = new wxPanel(this);
	propertyPanel->SetBackgroundColour(*wxLIGHT_GREY);
	propertySizer = new wxBoxSizer(wxVERTICAL);

	// 名称
	wxStaticText* nameLabel = new wxStaticText(propertyPanel, wxID_ANY, "名称:");
	wxTextCtrl* nameCtrl = new wxTextCtrl(propertyPanel, wxID_ANY + 100, "-");
	propertySizer->Add(nameLabel, 0, wxALL | wxEXPAND, 5);
	propertySizer->Add(nameCtrl, 0, wxALL | wxEXPAND, 5);

	// 位置
	wxStaticText* positionLabel = new wxStaticText(propertyPanel, wxID_ANY, "位置 (x, y):");
	wxTextCtrl* positionX = new wxTextCtrl(propertyPanel, wxID_ANY + 101, "-");
	wxTextCtrl* positionY = new wxTextCtrl(propertyPanel, wxID_ANY + 102, "-");
	wxBoxSizer* positionSizer = new wxBoxSizer(wxHORIZONTAL);
	positionSizer->Add(positionX, 1, wxALL | wxEXPAND, 5);
	positionSizer->Add(positionY, 1, wxALL | wxEXPAND, 5);
	propertySizer->Add(positionLabel, 0, wxALL | wxEXPAND, 5);
	propertySizer->Add(positionSizer, 0, wxALL | wxEXPAND, 5);

	// 大小
	wxStaticText* sizeLabel = new wxStaticText(propertyPanel, wxID_ANY, "大小 (宽, 高):");
	wxTextCtrl* sizeWidth = new wxTextCtrl(propertyPanel, wxID_ANY + 103, "-");
	wxTextCtrl* sizeHeight = new wxTextCtrl(propertyPanel, wxID_ANY + 104, "-");
	wxBoxSizer* sizeSizer = new wxBoxSizer(wxHORIZONTAL);
	sizeSizer->Add(sizeWidth, 1, wxALL | wxEXPAND, 5);
	sizeSizer->Add(sizeHeight, 1, wxALL | wxEXPAND, 5);
	propertySizer->Add(sizeLabel, 0, wxALL | wxEXPAND, 5);
	propertySizer->Add(sizeSizer, 0, wxALL | wxEXPAND, 5);

	// 角度
	wxStaticText* angleLabel = new wxStaticText(propertyPanel, wxID_ANY, "角度:");
	wxTextCtrl* angleCtrl = new wxTextCtrl(propertyPanel, wxID_ANY + 105, "-");
	propertySizer->Add(angleLabel, 0, wxALL | wxEXPAND, 5);
	propertySizer->Add(angleCtrl, 0, wxALL | wxEXPAND, 5);

	//输入状态(in1, in2)
	wxStaticText * inLabel = new wxStaticText(propertyPanel, wxID_ANY, "输入状态:");
	wxTextCtrl* in1Ctrl = new wxTextCtrl(propertyPanel, wxID_ANY + 106, "-");
	wxTextCtrl* in2Ctrl = new wxTextCtrl(propertyPanel, wxID_ANY + 107, "-");
	wxBoxSizer* inSizer = new wxBoxSizer(wxHORIZONTAL);
	inSizer->Add(in1Ctrl, 1, wxALL | wxEXPAND, 5);
	inSizer->Add(in2Ctrl, 1, wxALL | wxEXPAND, 5);

	// 输出状态 (out)
	wxStaticText* outLabel = new wxStaticText(propertyPanel, wxID_ANY, "输出状态:");
	wxTextCtrl* outCtrl = new wxTextCtrl(propertyPanel, wxID_ANY + 108, "-");
	wxBoxSizer* outSizer = new wxBoxSizer(wxHORIZONTAL);
	outSizer->Add(outCtrl, 1, wxALL | wxEXPAND, 5);
	propertySizer->Add(inLabel, 0, wxALL | wxEXPAND, 5);
	propertySizer->Add(inSizer, 0, wxALL | wxEXPAND, 5);
	propertySizer->Add(outLabel, 0, wxALL | wxEXPAND, 5);
	propertySizer->Add(outSizer, 0, wxALL | wxEXPAND, 5);

	propertyPanel->SetSizerAndFit(propertySizer);
	propertyPanel->Show(false);  // 隐藏

	drawboard = new DrawBoard(this);
	wxBoxSizer* controlBoard = new wxBoxSizer(wxVERTICAL);
	controlBoard->Add(m_btn1, 0, wxALL | wxEXPAND, 5);
	controlBoard->Add(m_btn2, 0, wxALL | wxEXPAND, 5);
	controlBoard->Add(BezierCurve_btn, 0, wxALL | wxEXPAND, 5);
	controlBoard->Add(andGate_btn, 0, wxALL | wxEXPAND, 5);
	controlBoard->Add(orGate_btn, 0, wxALL | wxEXPAND, 5);
	controlBoard->Add(notGate_btn, 0, wxALL | wxEXPAND, 5);
	controlBoard->Add(textBox_btn, 0, wxALL | wxEXPAND, 5);
	bottomSizer->Add(propertyPanel, 0, wxALL | wxEXPAND, 5);
	bottomSizer->Add(drawboard, 1, wxALL | wxEXPAND, 5); 
	bottomSizer->Add(controlBoard, 0, wxALL | wxEXPAND, 5); 
	mainSizer->Add(bottomSizer, 1, wxALL | wxEXPAND, 5);

	//设置按钮绑定事件
	fileBtn->Bind(wxEVT_BUTTON, &cMain::OnFileButtonClicked, this);
	propertyBtn->Bind(wxEVT_BUTTON, &cMain::OnPropertyButtonClicked, this);
	windowBtn->Bind(wxEVT_BUTTON, &cMain::OnWindowButtonClicked, this);
	dataBtn->Bind(wxEVT_BUTTON, &cMain::OnDataButtonClicked, this);
}

//加载图片
wxBitmap cMain::CreateImage(wxBitmap image, wxString path) {
	bool ifok = image.LoadFile(path, wxBITMAP_TYPE_PNG);
	wxImage imageForSelect = image.ConvertToImage();
	imageForSelect = imageForSelect.Scale(20, 20, wxIMAGE_QUALITY_HIGH);
	image = wxBitmap(imageForSelect);
	return image;

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
	// 初始化所有模式为 false
	drawboard->isLineDrawing = false;
	drawboard->isFreeDrawing = false;
	drawboard->isBezierDrawing = false;
	drawboard->placingAndGate = false;
	drawboard->placingOrGate = false;
	drawboard->placingNotGate = false;

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
	default:
		break;
	}
}
void cMain::OnFileButtonClicked(wxCommandEvent& event)
{
	wxMenu fileMenu;

	fileMenu.Append(wxID_ANY, "导入");
	fileMenu.Append(wxID_ANY, "导出");
	fileMenu.Append(wxID_ANY, "保存");
	fileMenu.Append(wxID_ANY, "另存为");
	fileMenu.Append(wxID_ANY, "主题");
	fileMenu.Append(wxID_ANY, "版本");

	PopupMenu(&fileMenu);
}
void cMain::OnPropertyButtonClicked(wxCommandEvent& event) {
	// 切换属性面板的显示和隐藏
	if (propertyPanel->IsShown()) {
		propertyPanel->Hide();
	}
	else {
		propertyPanel->Show();
	}
	Layout();
}
void cMain::OnWindowButtonClicked(wxCommandEvent& event) {
	// 弹出滑块对话框
	ZoomSliderDialog* dialog = new ZoomSliderDialog(this, drawboard->scale);
	if (dialog->ShowModal() == wxID_OK) {
		float newScale = dialog->GetScale();
		drawboard->SetScale(newScale); // 更新画布的缩放比例
	}
	dialog->Destroy();
}
void cMain::OnDataButtonClicked(wxCommandEvent& event) {
	std::vector<Component> components = drawboard->GetAllComponents(); // 获取所有元器件
	ComponentDataDialog* dialog = new ComponentDataDialog(this, components);
	dialog->ShowModal(); // 显示对话框
	dialog->Destroy();
}
void cMain::UpdatePropertyPanel() {
	Component* selectedComponent = drawboard->GetSelectedComponent();

	if (!selectedComponent) {
		for (auto child : propertyPanel->GetChildren()) {
			wxTextCtrl* textCtrl = dynamic_cast<wxTextCtrl*>(child);
			if (textCtrl) {
				textCtrl->SetValue("-");  // 设置为默认值
			}
		}
		return;
	}
	else if (selectedComponent) {
		//名称
		nameCtrl = dynamic_cast<wxTextCtrl*>(propertyPanel->FindWindowById(wxID_ANY + 100));
		if (nameCtrl) {
			if (selectedComponent->type == ANDGate) {
				nameCtrl->SetValue("与门");
			}
			else if (selectedComponent->type == ORGate) {
				nameCtrl->SetValue("或门");
			}
			else if (selectedComponent->type == NOTGate) {
				nameCtrl->SetValue("非门");
			}
		}

		//位置
		positionX = dynamic_cast<wxTextCtrl*>(propertyPanel->FindWindowById(wxID_ANY + 101));
		positionY = dynamic_cast<wxTextCtrl*>(propertyPanel->FindWindowById(wxID_ANY + 102));
		if (positionX && positionY) {
			positionX->SetValue(wxString::Format("%d", selectedComponent->center.x));
			positionY->SetValue(wxString::Format("%d", selectedComponent->center.y));
		}

		//大小
		scaleX = dynamic_cast<wxTextCtrl*>(propertyPanel->FindWindowById(wxID_ANY + 103));
		scaleY = dynamic_cast<wxTextCtrl*>(propertyPanel->FindWindowById(wxID_ANY + 104));
		if (scaleX && scaleY) {
			scaleX->SetValue(wxString::Format("%.2f", selectedComponent->scale));
			scaleY->SetValue(wxString::Format("%.2f", selectedComponent->scale));
		}
		
		//角度
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

cMain::~cMain() {
	if (refreshTimer) {
		refreshTimer->Stop();
		delete refreshTimer;
	}
}