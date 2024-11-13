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
    ComponentType type;
	wxPoint pin1;
    wxPoint pin1_connected;
	wxPoint pin2;
    wxPoint pin2_connected;
	wxPoint pout;
    wxPoint pout_connected;
	wxPoint center;
	wxPoint points[4];
	double scale;
	int pointIndex;
	bool showPoints = true;
	bool draggingTab = false;
	bool movingTab = false;
    bool beConnected = false;
	bool connectingPin1 = false;
	bool connectingPin2 = false;
	bool connectingPout = false;

    Component(wxPoint center, ComponentType type) {
        this->center = center;
        this->scale = 1.0;
        this->type = type;
    }

	bool PointInside(const wxPoint& p) {
		showPoints = false;
		if (points[0].x - 5 <= p.x && p.x <= points[2].x + 5 && points[0].y - 5 <= p.y && p.y <= points[1].y + 5) {
			showPoints = !showPoints;
		}
		return showPoints;
	}

    void Show(wxMemoryDC& memDC) {
        if (this->type == ANDGate) {
            points[0] = wxPoint(center.x - 40 * scale, center.y - 20 * scale);//左上
            points[1] = wxPoint(center.x - 40 * scale, center.y + 20 * scale);//左下
            points[2] = wxPoint(center.x + 60 * scale, center.y - 20 * scale);//右上
            points[3] = wxPoint(center.x + 60 * scale, center.y + 20 * scale);//右下


            this->pin1 = wxPoint(center.x - 40 * scale, center.y + 10 * scale);
            this->pin2 = wxPoint(center.x - 40 * scale, center.y - 10 * scale);
            this->pout = wxPoint(center.x + 60 * scale, center.y);

            wxGraphicsContext* gc = wxGraphicsContext::Create(memDC);

            gc->SetPen(wxPen(wxColour(0, 0, 0), 3));
            if (gc) {
                wxGraphicsPath path = gc->CreatePath();
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

                gc->StrokePath(path);
            }
            delete gc;
        }
        else if (this->type == ORGate) {
            this->scale = scale;

            points[0] = wxPoint(center.x - 40 * scale, center.y - 20 * scale);//左上
            points[1] = wxPoint(center.x - 40 * scale, center.y + 20 * scale);//左下
            points[2] = wxPoint(center.x + 60 * scale, center.y - 20 * scale);//右上
            points[3] = wxPoint(center.x + 60 * scale, center.y + 20 * scale);//右下

            this->pin1 = wxPoint(center.x - 40 * scale, center.y + 10 * scale);
            this->pin2 = wxPoint(center.x - 40 * scale, center.y - 10 * scale);
            this->pout = wxPoint(center.x + 60 * scale, center.y);

            wxGraphicsContext* gc = wxGraphicsContext::Create(memDC);
            gc->SetPen(wxPen(wxColour(0, 0, 0), 3));
            if (gc) {
                wxGraphicsPath path = gc->CreatePath();
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

                gc->StrokePath(path);
            }
            delete gc;
        }
        else if(this->type == NOTGate){
            this->scale = scale;

            points[0] = wxPoint(center.x - 40 * scale, center.y - 20 * scale);//左上
            points[1] = wxPoint(center.x - 40 * scale, center.y + 20 * scale);//左下
            points[2] = wxPoint(center.x + 48 * scale, center.y - 20 * scale);//右上
            points[3] = wxPoint(center.x + 48 * scale, center.y + 20 * scale);//右下

            this->pin1 = wxPoint(center.x - 40 * scale, center.y);
            this->pout = wxPoint(center.x + 48 * scale, center.y);

            wxGraphicsContext* gc = wxGraphicsContext::Create(memDC);
            gc->SetPen(wxPen(wxColour(0, 0, 0), 3));
            if (gc) {
                wxGraphicsPath path = gc->CreatePath();
                path.MoveToPoint(center.x + 20 * scale, center.y);
                path.AddLineToPoint(center.x - 20 * scale, center.y - 20 * scale);
                path.AddLineToPoint(center.x - 20 * scale, center.y + 20 * scale);
                path.AddLineToPoint(center.x + 20 * scale, center.y);
                path.AddEllipse(center.x + 20 * scale, center.y - 4 * scale, 8 * scale, 8 * scale);

                path.MoveToPoint(center.x - 40 * scale, center.y);
                path.AddLineToPoint(center.x - 20 * scale, center.y);
                path.MoveToPoint(center.x + 28 * scale, center.y);
                path.AddLineToPoint(center.x + 48 * scale, center.y);

                gc->StrokePath(path);
            }
            delete gc;
        }
    }

	~Component() {}
};

//继承类的实现
//class ANDGate :public ElectronicComponent {
//public:
//    ANDGate(wxPoint center) {
//        this->center = center;
//        this->scale = 1.0;
//    }
//
//    //显示函数
//    void Show(wxMemoryDC& memDC) override {
//
//        points[0] = wxPoint(center.x - 40 * scale, center.y - 20 * scale);//左上
//        points[1] = wxPoint(center.x - 40 * scale, center.y + 20 * scale);//左下
//        points[2] = wxPoint(center.x + 60 * scale, center.y - 20 * scale);//右上
//        points[3] = wxPoint(center.x + 60 * scale, center.y + 20 * scale);//右下
//
//
//        this->pin1 = wxPoint(center.x - 40 * scale, center.y + 10 * scale);
//        this->pin2 = wxPoint(center.x - 40 * scale, center.y - 10 * scale);
//        this->pout = wxPoint(center.x + 60 * scale, center.y);
//
//        wxGraphicsContext* gc = wxGraphicsContext::Create(memDC);
//
//        gc->SetPen(wxPen(wxColour(0, 0, 0), 3));
//        if (gc) {
//            wxGraphicsPath path = gc->CreatePath();
//            path.MoveToPoint(center.x + 20 * scale, center.y - 20 * scale);
//            path.AddLineToPoint(center.x - 20 * scale, center.y - 20 * scale);
//            path.AddLineToPoint(center.x - 20 * scale, center.y + 20 * scale);
//            path.AddLineToPoint(center.x + 20 * scale, center.y + 20 * scale);
//            path.AddArc(center.x + 20 * scale, center.y, 20 * scale, M_PI / 2, -M_PI / 2, false);
//
//            path.MoveToPoint(center.x - 40 * scale, center.y + 10 * scale);
//            path.AddLineToPoint(center.x - 20 * scale, center.y + 10 * scale);
//            path.MoveToPoint(center.x - 40 * scale, center.y - 10 * scale);
//            path.AddLineToPoint(center.x - 20 * scale, center.y - 10 * scale);
//            path.MoveToPoint(center.x + 40 * scale, center.y);
//            path.AddLineToPoint(center.x + 60 * scale, center.y);
//
//            gc->StrokePath(path);
//        }
//        delete gc;
//    }
//};
//class ORGate :public ElectronicComponent {
//public:
//    ORGate(wxPoint center) {
//        this->center = center;
//        this->scale = 1.0;
//    }
//
//    //显示函数
//    void Show(wxMemoryDC& memDC) override {
//        this->scale = scale;
//
//        points[0] = wxPoint(center.x - 40 * scale, center.y - 20 * scale);//左上
//        points[1] = wxPoint(center.x - 40 * scale, center.y + 20 * scale);//左下
//        points[2] = wxPoint(center.x + 60 * scale, center.y - 20 * scale);//右上
//        points[3] = wxPoint(center.x + 60 * scale, center.y + 20 * scale);//右下
//
//        this->pin1 = wxPoint(center.x - 40 * scale, center.y + 10 * scale);
//        this->pin2 = wxPoint(center.x - 40 * scale, center.y - 10 * scale);
//        this->pout = wxPoint(center.x + 60 * scale, center.y);
//
//        wxGraphicsContext* gc = wxGraphicsContext::Create(memDC);
//        gc->SetPen(wxPen(wxColour(0, 0, 0), 3));
//        if (gc) {
//            wxGraphicsPath path = gc->CreatePath();
//            path.MoveToPoint(center.x, center.y - 20 * scale);
//            path.AddLineToPoint(center.x - 20 * scale, center.y - 20 * scale);
//            path.AddQuadCurveToPoint(center.x, center.y, center.x - 20 * scale, center.y + 20 * scale);
//            path.AddLineToPoint(center.x - 10 * scale, center.y + 20 * scale);
//            path.AddQuadCurveToPoint(center.x + 20 * scale, center.y + 20 * scale, center.x + 40 * scale, center.y);
//            path.AddQuadCurveToPoint(center.x + 20 * scale, center.y - 20 * scale, center.x, center.y - 20 * scale);
//
//            path.MoveToPoint(center.x - 40 * scale, center.y + 10 * scale);
//            path.AddLineToPoint(center.x - 13 * scale, center.y + 10 * scale);
//            path.MoveToPoint(center.x - 40 * scale, center.y - 10 * scale);
//            path.AddLineToPoint(center.x - 13 * scale, center.y - 10 * scale);
//            path.MoveToPoint(center.x + 40 * scale, center.y);
//            path.AddLineToPoint(center.x + 60 * scale, center.y);
//
//            gc->StrokePath(path);
//        }
//        delete gc;
//    }
//};
//class NOTGate :public ElectronicComponent {
//public:
//    NOTGate(wxPoint center) {
//        this->center = center;
//        this->scale = 1.0;
//    }
//
//    //显示函数
//    void Show(wxMemoryDC& memDC) override {
//        this->scale = scale;
//
//        points[0] = wxPoint(center.x - 40 * scale, center.y - 20 * scale);//左上
//        points[1] = wxPoint(center.x - 40 * scale, center.y + 20 * scale);//左下
//        points[2] = wxPoint(center.x + 48 * scale, center.y - 20 * scale);//右上
//        points[3] = wxPoint(center.x + 48 * scale, center.y + 20 * scale);//右下
//
//        this->pin1 = wxPoint(center.x - 40 * scale, center.y);
//        this->pout = wxPoint(center.x + 48 * scale, center.y);
//
//        wxGraphicsContext* gc = wxGraphicsContext::Create(memDC);
//        gc->SetPen(wxPen(wxColour(0, 0, 0), 3));
//        if (gc) {
//            wxGraphicsPath path = gc->CreatePath();
//            path.MoveToPoint(center.x + 20 * scale, center.y);
//            path.AddLineToPoint(center.x - 20 * scale, center.y - 20 * scale);
//            path.AddLineToPoint(center.x - 20 * scale, center.y + 20 * scale);
//            path.AddLineToPoint(center.x + 20 * scale, center.y);
//            path.AddEllipse(center.x + 20 * scale, center.y - 4 * scale, 8 * scale, 8 * scale);
//
//            path.MoveToPoint(center.x - 40 * scale, center.y);
//            path.AddLineToPoint(center.x - 20 * scale, center.y);
//            path.MoveToPoint(center.x + 28 * scale, center.y);
//            path.AddLineToPoint(center.x + 48 * scale, center.y);
//
//            gc->StrokePath(path);
//        }
//        delete gc;
//    }
//};


//class Connection {
//public:
//	ElectronicComponent startComponent;
//	ElectronicComponent endComponent;
//	wxPoint startPin;
//	wxPoint endPin;
//	struct connectPath {
//
//	};
//
//	Connection(ElectronicComponent startComponent, ElectronicComponent endComponent, wxPoint startPin, wxPoint endPin){
//		this->startComponent = startComponent;
//		this->endComponent = endComponent;
//		this->startPin = startPin;
//		this->endPin = endPin;
//	}
//
//	void Draw(wxMemoryDC& memDC) {
//		wxGraphicsContext* gc = wxGraphicsContext::Create(memDC);
//
//	}
//
//
//};

class Connection {
public:
	bool isFirstOut, isScondOut;//记录两个元器件的头尾情况
	wxPoint startPoint, endPoint;
	float startScale, endScale;

};