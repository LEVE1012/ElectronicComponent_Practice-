#pragma once
#include "ElectronicComponent.h"


class DrawBoard :public wxPanel
{
public:
	DrawBoard(wxFrame* parent);
	~DrawBoard();
public:
	bool isShiftPressed;

	//画线有关
	bool isLineDrawing;//直线绘制状态
	bool firstPointSet;//直线绘制第一笔落下状态
	bool isFreeDrawing;//普通曲线绘制状态
	bool isBezierDrawing;//贝塞尔曲线绘制状态






	//电路元器件有关
	bool isConnecting;
	wxPoint firstPoint;
	float firstScale;
	float secondScale;
	float isFirstOut;
	std::vector<Connection> connections;//元器件连线列表



	bool placingAndGate;//布置状态
	bool placingOrGate;
	bool placingNotGate;


	wxPoint XYoffset;
	wxPoint connectStartPoint;
	wxPoint connectEndPoint;

	wxPoint currentLineEnd;
	wxPoint startPoint;//首端点
	wxPoint endPoint;//尾端点
	wxPoint mousePos;//鼠标位置点
	wxPoint tempPoint;//贝塞尔曲线临时存储点

	wxTextCtrl* mousePositionText;//光标位置显示文本
	wxPoint lastMousePos;//上一次的鼠标显示位置

	//存储列表
	std::vector<std::pair<wxPoint, wxPoint>> lines;//所有直线列表
	std::vector<wxPoint> freeDrawPoints;//暂存曲线列表
	std::vector<std::vector<wxPoint>> savedFreeDrawings;//所有曲线列表
	std::vector<wxPoint> BezierControlPoints;//暂存贝塞尔曲线列表
	struct BezierCurve {
		std::vector<wxPoint> controlPoints;//存储点的列表
		int pointCount;//点的个数
	};
	std::vector<BezierCurve> bezierCurveStorage;//所有贝塞尔曲线列表

	std::vector<Component> Components;//元器件列表
	struct Barrier {
		wxPoint leftUpNode;
		wxPoint leftDownNode;
		wxPoint rightUpNode;
		wxPoint rightDownNode;
	};
	std::vector<Barrier> barrierStorage;//存储障碍物的列表

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
