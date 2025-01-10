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
	wxButton* m_btn1 = nullptr;
	wxButton* m_btn2 = nullptr;
	wxButton* BezierCurve_btn = nullptr;
	wxButton* andGate_btn = nullptr;
	wxButton* orGate_btn = nullptr;
	wxButton* notGate_btn = nullptr;
	wxButton* textBox_btn = nullptr;
	wxButton* fileBtn;
	wxButton* propertyBtn;
	wxButton* designBtn;
	wxButton* windowBtn;
	wxButton* dataBtn;
	DrawBoard* drawboard;
	wxBitmap lineImage;
	wxBitmap andGateImage;
	wxBitmap orGateImage;
	wxBitmap notGateImage;
	wxBitmap textBoxImage;
	wxPanel* propertyPanel;
	wxBoxSizer* propertySizer;

	wxTextCtrl* nameCtrl;
	wxTextCtrl* positionX;
	wxTextCtrl* positionY;
	wxTextCtrl* scaleX;
	wxTextCtrl* scaleY;
	wxTextCtrl* angle;

	wxBitmap CreateImage(wxBitmap image, wxString path);
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

	void OnTimer(wxTimerEvent& event);
	void UpdatePropertyPanel();

private:
	wxTimer* refreshTimer;  // ��ʱ��
	wxDECLARE_EVENT_TABLE();
};

class ZoomSliderDialog : public wxDialog {
public:
	ZoomSliderDialog(wxWindow* parent, float initialScale)
		: wxDialog(parent, wxID_ANY, "�������ű���", wxDefaultPosition, wxSize(300, 150)),
		scale(initialScale) {

		wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

		// �������飨��Χ 10% �� 200%��
		slider = new wxSlider(this, wxID_ANY, initialScale * 100, 10, 200, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL | wxSL_LABELS);
		mainSizer->Add(slider, 1, wxALL | wxEXPAND, 10);

		// ȷ�Ϻ�ȡ����ť
		wxBoxSizer* buttonSizer = new wxBoxSizer(wxHORIZONTAL);
		wxButton* okButton = new wxButton(this, wxID_OK, "ȷ��");
		wxButton* cancelButton = new wxButton(this, wxID_CANCEL, "ȡ��");
		buttonSizer->Add(okButton, 0, wxALL, 5);
		buttonSizer->Add(cancelButton, 0, wxALL, 5);

		mainSizer->Add(buttonSizer, 0, wxALIGN_CENTER);

		SetSizerAndFit(mainSizer);
	}

	float GetScale() const {
		return slider->GetValue() / 100.0f; // �������ű���
	}

private:
	wxSlider* slider;
	float scale;
};

class ComponentDataDialog : public wxDialog {
public:
	ComponentDataDialog(wxWindow* parent, const std::vector<Component>& components)
		: wxDialog(parent, wxID_ANY, "Ԫ��������", wxDefaultPosition, wxSize(500, 300)) {

		wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

		// �����б�ؼ�
		wxListCtrl* listCtrl = new wxListCtrl(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_REPORT | wxLC_SINGLE_SEL);
		listCtrl->AppendColumn("����", wxLIST_FORMAT_LEFT, 150);
		listCtrl->AppendColumn("λ�� (x, y)", wxLIST_FORMAT_LEFT, 150);
		listCtrl->AppendColumn("��С", wxLIST_FORMAT_LEFT, 100);
		listCtrl->AppendColumn("�Ƕ�", wxLIST_FORMAT_LEFT, 100);

		// �������
		for (const auto& component : components) {
			wxListItem item;
			item.SetId(listCtrl->GetItemCount());
			listCtrl->InsertItem(item);

			// ����Ԫ����������
			listCtrl->SetItem(item.GetId(), 0, GetComponentName(component.type)); // ����
			listCtrl->SetItem(item.GetId(), 1, wxString::Format("(%d, %d)", component.center.x, component.center.y)); // λ��
			listCtrl->SetItem(item.GetId(), 2, wxString::Format("%.2f", component.scale)); // ��С
			listCtrl->SetItem(item.GetId(), 3, wxString::Format("%.2f", component.rotationAngle)); // �Ƕ�
		}

		mainSizer->Add(listCtrl, 1, wxALL | wxEXPAND, 10);

		// ��ӹرհ�ť
		wxButton* closeButton = new wxButton(this, wxID_OK, "�ر�");
		mainSizer->Add(closeButton, 0, wxALIGN_CENTER | wxALL, 10);

		SetSizerAndFit(mainSizer);
	}

private:
	wxString GetComponentName(ComponentType type) {
		switch (type) {
		case ANDGate: return "����";
		case ORGate: return "����";
		case NOTGate: return "����";
		default: return "δ֪";
		}
	}
};