#pragma once
#include <wx/wx.h>
#include "DrawBoard.h"
#include <wx/listctrl.h>
class cMain : public wxFrame
{
public:
	cMain();
	~cMain();

public:
	wxButton* custom_btn = nullptr;
	wxButton* none_btn = nullptr;
	wxButton* Line_btn = nullptr;
	wxButton* Free_btn = nullptr;
	wxButton* BezierCurve_btn = nullptr;
	wxButton* andGate_btn = nullptr;
	wxButton* orGate_btn = nullptr;
	wxButton* notGate_btn = nullptr;
	wxButton* textBox_btn = nullptr;
	wxButton* resistor_btn = nullptr;
	wxButton* capacitor_btn = nullptr;
	wxButton* diode_btn = nullptr;
	wxButton* power_btn = nullptr;
	
	wxButton* fileBtn;
	wxButton* propertyBtn;
	wxButton* designBtn;
	wxButton* windowBtn;
	wxButton* dataBtn;
	DrawBoard* drawboard;

	wxBitmap customImage;
	wxBitmap noneImage;
	wxBitmap lineImage;
	wxBitmap freeImage;
	wxBitmap curveImage;
	wxBitmap andGateImage;
	wxBitmap orGateImage;
	wxBitmap notGateImage;
	wxBitmap textBoxImage;
	wxBitmap resistorImage;
	wxBitmap capacitorImage;
	wxBitmap diodeImage;
	wxBitmap powerImage;


	wxPanel* propertyPanel;
	wxBoxSizer* propertySizer;

	wxTextCtrl* nameCtrl;
	wxTextCtrl* positionX;
	wxTextCtrl* positionY;
	wxTextCtrl* scaleX;
	wxTextCtrl* scaleY;
	wxTextCtrl* angle;

	wxBitmap CreateImage(wxBitmap image, wxString path);
	void OnNoneButtonClicked(wxCommandEvent& evt);
	void OnStraightLineButtonClicked(wxCommandEvent& evt);
	void OnFreeCurveButtonClicked(wxCommandEvent& event);
	void OnBezierCurveButtonClicked(wxCommandEvent& event);
	void OnAndGateButtonClicked(wxCommandEvent& event);
	void OnOrGateButtonClicked(wxCommandEvent& event);
	void OnNotGateButtonClicked(wxCommandEvent& event);
	void OnTextBoxButtonClicked(wxCommandEvent& evt);
	void UpdateDrawingState();
	void OnFileButtonClicked(wxCommandEvent& event);
	void OnPropertyButtonClicked(wxCommandEvent& event);
	void OnWindowButtonClicked(wxCommandEvent& event);
	void OnDataButtonClicked(wxCommandEvent& event);
	void OnCustomComponentButtonClicked(wxCommandEvent& event);
	

	void OnTimer(wxTimerEvent& event);
	void UpdatePropertyPanel();
	void OnResistorButtonClicked(wxCommandEvent& event);
	void OnCapacitorButtonClicked(wxCommandEvent& event);
	void OnDiodeButtonClicked(wxCommandEvent& event);
	void OnPowerButtonClicked(wxCommandEvent& event);
private:
	wxTimer* refreshTimer;  // 定时器
	wxDECLARE_EVENT_TABLE();
};

class ZoomSliderDialog : public wxDialog {
public:
	ZoomSliderDialog(wxWindow* parent, float initialScale)
		: wxDialog(parent, wxID_ANY, "调整缩放比例", wxDefaultPosition, wxSize(300, 150)),
		scale(initialScale) {

		wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

		// 创建滑块（范围 10% 到 200%）
		slider = new wxSlider(this, wxID_ANY, initialScale * 100, 10, 200, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL | wxSL_LABELS);
		mainSizer->Add(slider, 1, wxALL | wxEXPAND, 10);

		// 确认和取消按钮
		wxBoxSizer* buttonSizer = new wxBoxSizer(wxHORIZONTAL);
		wxButton* okButton = new wxButton(this, wxID_OK, "确认");
		wxButton* cancelButton = new wxButton(this, wxID_CANCEL, "取消");
		buttonSizer->Add(okButton, 0, wxALL, 5);
		buttonSizer->Add(cancelButton, 0, wxALL, 5);

		mainSizer->Add(buttonSizer, 0, wxALIGN_CENTER);

		SetSizerAndFit(mainSizer);
	}

	float GetScale() const {
		return slider->GetValue() / 100.0f; // 返回缩放比例
	}

private:
	wxSlider* slider;
	float scale;
};

class ComponentDataDialog : public wxDialog {
public:
	ComponentDataDialog(wxWindow* parent, const std::vector<Component>& components)
		: wxDialog(parent, wxID_ANY, "元器件数据", wxDefaultPosition, wxSize(500, 300)) {

		wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

		// 创建列表控件
		wxListCtrl* listCtrl = new wxListCtrl(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_REPORT | wxLC_SINGLE_SEL);
		listCtrl->AppendColumn("名称", wxLIST_FORMAT_LEFT, 150);
		listCtrl->AppendColumn("位置 (x, y)", wxLIST_FORMAT_LEFT, 150);
		listCtrl->AppendColumn("大小", wxLIST_FORMAT_LEFT, 100);
		listCtrl->AppendColumn("角度", wxLIST_FORMAT_LEFT, 100);

		// 填充数据
		for (const auto& component : components) {
			wxListItem item;
			item.SetId(listCtrl->GetItemCount());
			listCtrl->InsertItem(item);

			// 填入元器件的属性
			listCtrl->SetItem(item.GetId(), 0, GetComponentName(component.type)); // 名称
			listCtrl->SetItem(item.GetId(), 1, wxString::Format("(%d, %d)", component.center.x, component.center.y)); // 位置
			listCtrl->SetItem(item.GetId(), 2, wxString::Format("%.2f", component.scale)); // 大小
			listCtrl->SetItem(item.GetId(), 3, wxString::Format("%.2f", component.rotationAngle)); // 角度
		}

		mainSizer->Add(listCtrl, 1, wxALL | wxEXPAND, 10);

		// 添加关闭按钮
		wxButton* closeButton = new wxButton(this, wxID_OK, "关闭");
		mainSizer->Add(closeButton, 0, wxALIGN_CENTER | wxALL, 10);

		SetSizerAndFit(mainSizer);
	}

private:
	wxString GetComponentName(ComponentType type) {
		switch (type) {
		case ANDGate: return "与门";
		case ORGate: return "或门";
		case NOTGate: return "非门";
		case Resistor: return "电阻";
		case Capacitor: return "电容";
		case Diode: return "二极管";
		case Power: return "电流源";
		default: return "未知";
		}
	}
};

class CustomComponentEditor : public wxDialog {
public:
	CustomComponentEditor(wxWindow* parent)
		: wxDialog(parent, wxID_ANY, "自定义元器件", wxDefaultPosition, wxSize(500, 500)) {
		wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
		wxBoxSizer* topSizer = new wxBoxSizer(wxHORIZONTAL);
		customDrawBoard = new CustomDrawBoard(this);
		customDrawBoard->SetMinSize(wxSize(400, 400));
		topSizer->Add(customDrawBoard, 1, wxALL | wxALIGN_CENTER, 10);

		// 右侧工具按钮区域
		wxBoxSizer* toolSizer = new wxBoxSizer(wxVERTICAL);
		wxButton* lineToolButton = new wxButton(this, wxID_ANY, "直线工具");
		wxButton* bezierToolButton = new wxButton(this, wxID_ANY, "曲线工具");
		wxButton* triangleToolButton = new wxButton(this, wxID_ANY, "三角形工具");
		wxButton* rectangleToolButton = new wxButton(this, wxID_ANY, "矩形工具");
		wxButton* circleToolButton = new wxButton(this, wxID_ANY, "圆形工具");
		wxButton* pinToolButton = new wxButton(this, wxID_ANY, "引脚工具");
		toolSizer->Add(lineToolButton, 0, wxALL | wxEXPAND, 5);
		toolSizer->Add(bezierToolButton, 0, wxALL | wxEXPAND, 5);
		toolSizer->Add(triangleToolButton, 0, wxALL | wxEXPAND, 5);
		toolSizer->Add(rectangleToolButton, 0, wxALL | wxEXPAND, 5);
		toolSizer->Add(circleToolButton, 0, wxALL | wxEXPAND, 5);
		toolSizer->Add(pinToolButton, 0, wxALL | wxEXPAND, 5);

		// 将工具按钮区添加到主布局
		topSizer->Add(toolSizer, 0, wxALL | wxEXPAND, 10);


		wxBoxSizer* bottomButtonSizer = new wxBoxSizer(wxHORIZONTAL);
		wxButton* saveButton = new wxButton(this, wxID_ANY, "保存");
		wxButton* cancelButton = new wxButton(this, wxID_ANY, "取消");
		bottomButtonSizer->Add(saveButton, 0, wxALL, 5);
		bottomButtonSizer->Add(cancelButton, 0, wxALL, 5);

		mainSizer->Add(topSizer, 1, wxALL | wxEXPAND, 5);
		mainSizer->Add(bottomButtonSizer, 0, wxALIGN_CENTER);

		SetSizerAndFit(mainSizer);

		lineToolButton->Bind(wxEVT_BUTTON, &CustomComponentEditor::ONLINE, this);
		bezierToolButton->Bind(wxEVT_BUTTON, &CustomComponentEditor::ONCURVE, this);

		saveButton->Bind(wxEVT_BUTTON, &CustomComponentEditor::OnSave, this);
		cancelButton->Bind(wxEVT_BUTTON, &CustomComponentEditor::OnCancel, this);
	}

private:
	CustomDrawBoard* customDrawBoard;

	void ONLINE(wxCommandEvent& event) {
		customDrawBoard->isLineDrawing = true;
	}
	void ONCURVE(wxCommandEvent& event) {
		customDrawBoard->isBezierDrawing = true;

	}

	void OnSave(wxCommandEvent& event) {
		// 获取 CustomDrawBoard 中绘制的数据
		//auto shapes = customDrawBoard->GetShapes(); // 假设有一个方法可以获取绘制的图形
		//SaveComponent(shapes); // 保存逻辑
		wxMessageBox("元器件已保存！", "提示", wxOK | wxICON_INFORMATION);
		Close();
	}

	void OnCancel(wxCommandEvent& event) {
		Close(); // 关闭窗口
	}

	//void SaveComponent(const std::vector<Shape>& shapes) {
		// 将 shapes 数据保存到文件或内存中
	//}
};