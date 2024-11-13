#pragma once
#include "ElectronicComponent.h"


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
	wxPoint firstPoint;
	float firstScale;
	float secondScale;
	float isFirstOut;
	std::vector<Connection> connections;//Ԫ���������б�



	bool placingAndGate;//����״̬
	bool placingOrGate;
	bool placingNotGate;


	wxPoint XYoffset;
	wxPoint connectStartPoint;
	wxPoint connectEndPoint;

	wxPoint currentLineEnd;
	wxPoint startPoint;//�׶˵�
	wxPoint endPoint;//β�˵�
	wxPoint mousePos;//���λ�õ�
	wxPoint tempPoint;//������������ʱ�洢��

	wxTextCtrl* mousePositionText;//���λ����ʾ�ı�
	wxPoint lastMousePos;//��һ�ε������ʾλ��

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
	struct Barrier {
		wxPoint leftUpNode;
		wxPoint leftDownNode;
		wxPoint rightUpNode;
		wxPoint rightDownNode;
	};
	std::vector<Barrier> barrierStorage;//�洢�ϰ�����б�

	std::vector<std::pair<wxPoint, wxPoint>> electronicLines;

	void OnPaint(wxPaintEvent& event);
	void DrawGrid(wxGraphicsContext* gc, int gridSize);
	void DrawHorizontalAndVerticalLines(wxGraphicsContext* gc);
	void OnMouseMove(wxMouseEvent& event);
	void OnLeftClick(wxMouseEvent& event);
	void OnLeftUp(wxMouseEvent& event);
	void OnRightClick(wxMouseEvent& event);
	void OnKeyDown(wxKeyEvent& event);
	void OnKeyUp(wxKeyEvent& event);
	void DrawQuadraticBezier(wxGraphicsContext* gc,bool isWithLine, const wxPoint p1, const wxPoint p2, const wxPoint p3);
	void DrawCubicBezier(wxGraphicsContext* gc, bool isWithLine, const wxPoint p1, const wxPoint p2, const wxPoint p3, const wxPoint p4);
	bool IsPointInCircle(const wxPoint& mousePos, const wxPoint& circlePos, int radius);
	void SnapToGrid(const wxPoint& pos, int gridSize);
	void AStarSearchPath(wxPoint startPoint, wxPoint endPoint, wxMemoryDC& memDC);
	bool isSegmentIntersectRectangle(const wxPoint& p1, const wxPoint& p2, const Barrier& rectangle);
	
	wxDECLARE_EVENT_TABLE();
};
