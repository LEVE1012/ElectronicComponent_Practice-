#pragma once
#include <wx/wx.h>
#include <wx/dcgraph.h>
#include <wx/dcbuffer.h>

enum ComponentType {
    ANDGate,
    ORGate,
    NOTGate
};

class Component {
public:	
    //类型相关
    ComponentType type;

    // 位置相关
    wxPoint center;
    wxPoint pin1;
    wxPoint pin2;
    wxPoint pout;
    wxPoint points[4];
    double scale;

    // 交互相关
    int pointIndex;
    bool showPoints = true;
    bool draggingTab = false;
    bool movingTab = false;
    bool connectingPin1 = false;
    bool connectingPin2 = false;
    bool connectingPout = false;

    //构造函数
    Component(wxPoint center, ComponentType type) {
        this->scale = 1.0;
        this->type = type;
        this->center = center;
    }

	bool PointInside(const wxPoint& p) {
		return (points[0].x - 5 <= p.x && p.x <= points[2].x + 5 &&
                points[0].y - 5 <= p.y && p.y <= points[1].y + 5);
	}

    void UpdateGeometry() {
        //动态计算 点位 和 针脚
        if (type == ANDGate) {
            points[0] = wxPoint(center.x - 40 * scale, center.y - 20 * scale);//左上
            points[1] = wxPoint(center.x - 40 * scale, center.y + 20 * scale);//左下
            points[2] = wxPoint(center.x + 60 * scale, center.y - 20 * scale);//右上
            points[3] = wxPoint(center.x + 60 * scale, center.y + 20 * scale);//右下

            pin1 = wxPoint(center.x - 40 * scale, center.y + 10 * scale);
            pin2 = wxPoint(center.x - 40 * scale, center.y - 10 * scale);
            pout = wxPoint(center.x + 60 * scale, center.y);
        }
        if (type == ORGate) {
            points[0] = wxPoint(center.x - 40 * scale, center.y - 20 * scale);//左上
            points[1] = wxPoint(center.x - 40 * scale, center.y + 20 * scale);//左下
            points[2] = wxPoint(center.x + 60 * scale, center.y - 20 * scale);//右上
            points[3] = wxPoint(center.x + 60 * scale, center.y + 20 * scale);//右下

            pin1 = wxPoint(center.x - 40 * scale, center.y + 10 * scale);
            pin2 = wxPoint(center.x - 40 * scale, center.y - 10 * scale);
            pout = wxPoint(center.x + 60 * scale, center.y);
        }
        if (type == NOTGate) {
            points[0] = wxPoint(center.x - 40 * scale, center.y - 20 * scale);//左上
            points[1] = wxPoint(center.x - 40 * scale, center.y + 20 * scale);//左下
            points[2] = wxPoint(center.x + 48 * scale, center.y - 20 * scale);//右上
            points[3] = wxPoint(center.x + 48 * scale, center.y + 20 * scale);//右下

            pin1 = wxPoint(center.x - 40 * scale, center.y);
            pout = wxPoint(center.x + 48 * scale, center.y);
        }
    }

    void Show(wxMemoryDC& memDC) {
        UpdateGeometry();

        wxGraphicsContext* gc = wxGraphicsContext::Create(memDC);
        if (!gc) return;

        gc->SetPen(wxPen(wxColour(0, 0, 0), 3));
        wxGraphicsPath path = gc->CreatePath();

        if (type == ANDGate) {
            path.MoveToPoint(center.x + 20 * scale, center.y - 20 * scale);
            path.AddLineToPoint(center.x - 20 * scale, center.y - 20 * scale);
            path.AddLineToPoint(center.x - 20 * scale, center.y + 20 * scale);
            path.AddLineToPoint(center.x + 20 * scale, center.y + 20 * scale);
            path.AddArc(center.x + 20 * scale, center.y, 20 * scale, M_PI / 2, -M_PI / 2, false);

            path.MoveToPoint(center.x - 40 * scale, center.y + 10 * scale);
            path.AddLineToPoint(center.x - 20 * scale, center.y + 10 * scale);
            path.MoveToPoint(center.x - 40 * scale, center.y - 10 * scale);
            path.AddLineToPoint(center.x - 20 * scale, center.y - 10 * scale);
            path.MoveToPoint(center.x + 40 * scale, center.y);
            path.AddLineToPoint(center.x + 60 * scale, center.y);
        }
        else if (type == ORGate) {
            path.MoveToPoint(center.x, center.y - 20 * scale);
            path.AddLineToPoint(center.x - 20 * scale, center.y - 20 * scale);
            path.AddQuadCurveToPoint(center.x, center.y, center.x - 20 * scale, center.y + 20 * scale);
            path.AddLineToPoint(center.x - 10 * scale, center.y + 20 * scale);
            path.AddQuadCurveToPoint(center.x + 20 * scale, center.y + 20 * scale, center.x + 40 * scale, center.y);
            path.AddQuadCurveToPoint(center.x + 20 * scale, center.y - 20 * scale, center.x, center.y - 20 * scale);

            path.MoveToPoint(center.x - 40 * scale, center.y + 10 * scale);
            path.AddLineToPoint(center.x - 13 * scale, center.y + 10 * scale);
            path.MoveToPoint(center.x - 40 * scale, center.y - 10 * scale);
            path.AddLineToPoint(center.x - 13 * scale, center.y - 10 * scale);
            path.MoveToPoint(center.x + 40 * scale, center.y);
            path.AddLineToPoint(center.x + 60 * scale, center.y);
        }
        else if (type == NOTGate) {
            path.MoveToPoint(center.x + 20 * scale, center.y);
            path.AddLineToPoint(center.x - 20 * scale, center.y - 20 * scale);
            path.AddLineToPoint(center.x - 20 * scale, center.y + 20 * scale);
            path.AddLineToPoint(center.x + 20 * scale, center.y);
            path.AddEllipse(center.x + 20 * scale, center.y - 4 * scale, 8 * scale, 8 * scale);

            path.MoveToPoint(center.x - 40 * scale, center.y);
            path.AddLineToPoint(center.x - 20 * scale, center.y);
            path.MoveToPoint(center.x + 28 * scale, center.y);
            path.AddLineToPoint(center.x + 48 * scale, center.y);
        }

        gc->StrokePath(path);
        delete(gc);
    }
	~Component() {}
};