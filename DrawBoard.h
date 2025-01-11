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

	//画线有关
	bool isLineDrawing;//直线绘制状态
	bool firstPointSet;//直线绘制第一笔落下状态
	bool isFreeDrawing;//普通曲线绘制状态
	bool isBezierDrawing;//贝塞尔曲线绘制状态
	//电路元器件有关
	bool isConnecting;

	bool placingAndGate;//布置状态
	bool placingOrGate;
	bool placingNotGate;
	bool placingTextBox;
	bool placingResistor;
	bool placingCapacitor;
	bool placingDiode;
	bool placingPower;

	double initialAngle;

	float scale = 1.0f;//画布大小比例

	wxPoint XYoffset;
	wxPoint connectStartPoint;
	wxPoint connectEndPoint;

	wxPoint startPoint;//首端点
	wxPoint endPoint;//尾端点
	wxPoint mousePos;//鼠标位置点
	wxPoint tempPoint;//贝塞尔曲线临时存储点
	wxPoint connectTempPoint;
	wxTextCtrl* mousePositionText;//光标位置显示文本
	wxPoint lastMousePos;//上一次的鼠标显示位置

	wxPoint RotateVector;
	wxPoint Textoffset;
	TextBox* draggingTextBox;


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
	std::vector<wxPoint> connectPoints;//暂存元器件连线列表
	std::vector<std::vector<wxPoint>> connectPaths;//元器件连线列表

	std::list<TextBox> textBoxes;//文本框列表
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
		if (newScale > 0.1f && newScale <= 10.0f) { // 限制缩放范围
			scale = newScale;
			Refresh(); // 重新绘制画布
		}
	}

	Component* GetSelectedComponent() {
		for (auto& component : Components) {
			if (component.showPoints) {
				return &component;  // 返回选中的元器件
			}
		}
		return nullptr;  // 如果没有找到，返回 nullptr
	}
	std::vector<Component> GetAllComponents() const {
		return Components;
	}
	void StartBezierDrawing() {
		isBezierDrawing = true;  // 开始贝塞尔曲线绘制
		isLineDrawing = false;   // 停止直线绘制
		wxLogMessage("贝塞尔曲线绘制模式已启用");
	}

	void StartLineDrawing() {
		isLineDrawing = true;    // 开始直线绘制
		isBezierDrawing = false; // 停止贝塞尔曲线绘制
		wxLogMessage("直线绘制模式已启用");
	}
	wxDECLARE_EVENT_TABLE();
};

class CustomDrawBoard : public DrawBoard {
public:
	CustomDrawBoard(wxWindow* parent)
		: DrawBoard(static_cast<wxFrame*>(parent)) { // 传递父窗口参数
		SetSize(800, 800); // 限制绘图板大小
		SetBackgroundColour(*wxWHITE); // 设置背景色
	}

	void OnPaint(wxPaintEvent& event) {
		wxBufferedPaintDC dc(this);
		PrepareDC(dc);

		wxSize size = GetSize();
		dc.SetBrush(*wxTRANSPARENT_BRUSH);
		dc.SetPen(wxPen(*wxBLACK, 2, wxPENSTYLE_DOT));
		dc.DrawRectangle(0, 0, size.x, size.y);

		DrawBoard::OnPaint(event);
	}
};