#pragma once
#include "ElectronicComponent.h"
#include <list>

class DrawBoard :public wxPanel
{
public:
	DrawBoard(wxFrame* parent);
	~DrawBoard();
public:
	bool isShiftPressed;

	//�����й�
	bool isLineDrawing;//ֱ�߻���״̬
	bool firstPointSet;//ֱ�߻��Ƶ�һ������״̬
	bool isFreeDrawing;//��ͨ���߻���״̬
	bool isBezierDrawing;//���������߻���״̬
	//��·Ԫ�����й�
	bool isConnecting;

	bool placingAndGate;//����״̬
	bool placingOrGate;
	bool placingNotGate;
	bool placingTextBox;
	double initialAngle;

	float scale = 1.0f;//������С����

	wxPoint XYoffset;
	wxPoint connectStartPoint;
	wxPoint connectEndPoint;

	wxPoint startPoint;//�׶˵�
	wxPoint endPoint;//β�˵�
	wxPoint mousePos;//���λ�õ�
	wxPoint tempPoint;//������������ʱ�洢��
	wxPoint connectTempPoint;
	wxTextCtrl* mousePositionText;//���λ����ʾ�ı�
	wxPoint lastMousePos;//��һ�ε������ʾλ��

	wxPoint RotateVector;
	wxPoint Textoffset;
	TextBox* draggingTextBox;


	//�洢�б�
	std::vector<std::pair<wxPoint, wxPoint>> lines;//����ֱ���б�
	std::vector<wxPoint> freeDrawPoints;//�ݴ������б�
	std::vector<std::vector<wxPoint>> savedFreeDrawings;//���������б�
	std::vector<wxPoint> BezierControlPoints;//�ݴ汴���������б�
	struct BezierCurve {
		std::vector<wxPoint> controlPoints;//�洢����б�
		int pointCount;//��ĸ���
	};
	std::vector<BezierCurve> bezierCurveStorage;//���б����������б�

	std::vector<Component> Components;//Ԫ�����б�
	std::vector<wxPoint> connectPoints;//�ݴ�Ԫ���������б�
	std::vector<std::vector<wxPoint>> connectPaths;//Ԫ���������б�

	std::list<TextBox> textBoxes;//�ı����б�
	void OnPaint(wxPaintEvent& event);
	void DrawGrid(wxGraphicsContext* gc, int gridSize);
	void DrawHorizontalAndVerticalLines(wxGraphicsContext* gc);
	void OnMouseMove(wxMouseEvent& event);
	void OnLeftClick(wxMouseEvent& event);
	void OnLeftDoubleClick(wxMouseEvent& event);
	void OnLeftUp(wxMouseEvent& event);
	void OnRightClick(wxMouseEvent& event);
	void OnKeyUp(wxKeyEvent& event);
	void DrawQuadraticBezier(wxGraphicsContext* gc,bool isWithLine, const wxPoint p1, const wxPoint p2, const wxPoint p3);
	void DrawCubicBezier(wxGraphicsContext* gc, bool isWithLine, const wxPoint p1, const wxPoint p2, const wxPoint p3, const wxPoint p4);
	bool IsPointInCircle(const wxPoint& mousePos, const wxPoint& circlePos, int radius);
	bool IsPointInTextBox(wxPoint mousePos, TextBox textBox);
	void SnapToGrid(const wxPoint& pos, int gridSize);
	void SetScale(float newScale) {
		if (newScale > 0.1f && newScale <= 10.0f) { // �������ŷ�Χ
			scale = newScale;
			Refresh(); // ���»��ƻ���
		}
	}

	Component* GetSelectedComponent() {
		for (auto& component : Components) {
			if (component.showPoints) {
				return &component;  // ����ѡ�е�Ԫ����
			}
		}
		return nullptr;  // ���û���ҵ������� nullptr
	}
	std::vector<Component> GetAllComponents() const {
		return Components;
	}

	wxDECLARE_EVENT_TABLE();
};