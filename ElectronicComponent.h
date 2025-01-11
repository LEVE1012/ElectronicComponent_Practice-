#pragma once
#define wxUSE_COLOURPICKERCTRL 1

#include <wx/wx.h>
#include <wx/dcgraph.h>
#include <wx/dcbuffer.h>
#include <wx/tokenzr.h>
#include <wx/spinctrl.h>     // wxSpinCtrl
#include <wx/colordlg.h>
enum ComponentType {
    ANDGate,
    ORGate,
    NOTGate,
    Resistor,
    Capacitor,
    Diode,
    Power
};

class Component {
public:	
    //�������
    ComponentType type;

    // λ�����
    wxPoint center;
    wxPoint newCenter;
    wxPoint pin1;
    wxPoint pin2;
    wxPoint pout;
    wxPoint points[4];
    wxPoint closePoint;
    wxPoint rotationCenter; // ��ת����
    double rotationAngle = 0;   // ��ת�Ƕȣ��Ի���Ϊ��λ
    double scale;

    // �������
    int pointIndex;
    bool showPoints = true;
    bool draggingTab = false;
    bool movingTab = false;
    bool connectingPin1 = false;
    bool connectingPin2 = false;
    bool connectingPout = false;
    bool isRotating;

    //���캯��
    Component(wxPoint center, ComponentType type) {
        this->scale = 1.0;
        this->type = type;
        this->center = center;
    }

    bool PointInside(const wxPoint& p) {
        wxPoint pPrime = RotatePoint(p, rotationCenter, -rotationAngle);
        int left = points[0].x;
        int right = points[2].x;
        int top = points[0].y;
        int bottom = points[1].y;
        return (pPrime.x >= left - 5  && pPrime.x <= right + 5 &&
            pPrime.y >= top - 5 && pPrime.y <= bottom + 5);
    }

    wxPoint RotatePoint(const wxPoint& p, const wxPoint& center, double angle) {
        double rad = angle;
        double cosTheta = std::cos(rad);
        double sinTheta = std::sin(rad);
        int x = center.x + cosTheta * (p.x - center.x) - sinTheta * (p.y - center.y);
        int y = center.y + sinTheta * (p.x - center.x) + cosTheta * (p.y - center.y);
        return wxPoint(x, y);
    }

    void UpdateGeometry() {
        //��̬���� ��λ �� ���
        if (type == ANDGate) {
            points[0] = wxPoint(center.x - 40 * scale, center.y - 20 * scale);//����
            points[1] = wxPoint(center.x - 40 * scale, center.y + 20 * scale);//����
            points[2] = wxPoint(center.x + 60 * scale, center.y - 20 * scale);//����
            points[3] = wxPoint(center.x + 60 * scale, center.y + 20 * scale);//����
            closePoint = wxPoint(center.x  + 10 * scale, center.y + 40 * scale);
            rotationCenter = wxPoint(center.x + 10 * scale, center.y - 40 * scale);

            pin1 = wxPoint(center.x - 40 * scale, center.y + 10 * scale);
            pin2 = wxPoint(center.x - 40 * scale, center.y - 10 * scale);
            pout = wxPoint(center.x + 60 * scale, center.y);
        }
        if (type == ORGate) {
            points[0] = wxPoint(center.x - 40 * scale, center.y - 20 * scale);//����
            points[1] = wxPoint(center.x - 40 * scale, center.y + 20 * scale);//����
            points[2] = wxPoint(center.x + 60 * scale, center.y - 20 * scale);//����
            points[3] = wxPoint(center.x + 60 * scale, center.y + 20 * scale);//����
            closePoint = wxPoint(center.x + 10 * scale, center.y + 40 * scale);
            rotationCenter = wxPoint(center.x + 10 * scale, center.y - 40 * scale);

            pin1 = wxPoint(center.x - 40 * scale, center.y + 10 * scale);
            pin2 = wxPoint(center.x - 40 * scale, center.y - 10 * scale);
            pout = wxPoint(center.x + 60 * scale, center.y);
        }
        if (type == NOTGate) {
            points[0] = wxPoint(center.x - 40 * scale, center.y - 20 * scale);//����
            points[1] = wxPoint(center.x - 40 * scale, center.y + 20 * scale);//����
            points[2] = wxPoint(center.x + 48 * scale, center.y - 20 * scale);//����
            points[3] = wxPoint(center.x + 48 * scale, center.y + 20 * scale);//����
            closePoint = wxPoint(center.x + 4 * scale, center.y + 40 * scale);
            rotationCenter = wxPoint(center.x + 4 * scale, center.y - 40 * scale);

            pin1 = wxPoint(center.x - 40 * scale, center.y);
            pout = wxPoint(center.x + 48 * scale, center.y);
        }
        if (type == Resistor) {
            points[0] = wxPoint(center.x - 50 * scale, center.y - 10 * scale);//����
            points[1] = wxPoint(center.x - 50 * scale, center.y + 10 * scale);//����
            points[2] = wxPoint(center.x + 50 * scale, center.y - 10 * scale);//����
            points[3] = wxPoint(center.x + 50 * scale, center.y + 10 * scale);//����
            closePoint = wxPoint(center.x, center.y + 30 * scale);
            rotationCenter = wxPoint(center.x, center.y - 30 * scale);

            pin1 = wxPoint(center.x - 50 * scale, center.y);
            pout = wxPoint(center.x + 50 * scale, center.y);
        }
        if (type == Capacitor) {
            points[0] = wxPoint(center.x - 40 * scale, center.y - 20 * scale);//����
            points[1] = wxPoint(center.x - 40 * scale, center.y + 20 * scale);//����
            points[2] = wxPoint(center.x + 40 * scale, center.y - 20 * scale);//����
            points[3] = wxPoint(center.x + 40 * scale, center.y + 20 * scale);//����
            closePoint = wxPoint(center.x, center.y + 40 * scale);
            rotationCenter = wxPoint(center.x, center.y - 40 * scale);

            pin1 = wxPoint(center.x - 40 * scale, center.y);
            pout = wxPoint(center.x + 40 * scale, center.y);
        }
        if (type == Diode) {
            points[0] = wxPoint(center.x - 40 * scale, center.y - 20 * scale);//����
            points[1] = wxPoint(center.x - 40 * scale, center.y + 20 * scale);//����
            points[2] = wxPoint(center.x + 40 * scale, center.y - 20 * scale);//����
            points[3] = wxPoint(center.x + 40 * scale, center.y + 20 * scale);//����
            closePoint = wxPoint(center.x, center.y + 40 * scale);
            rotationCenter = wxPoint(center.x, center.y - 40 * scale);

            pin1 = wxPoint(center.x - 40 * scale, center.y);
            pout = wxPoint(center.x + 40 * scale, center.y);
        }
        if (type == Power) {
            points[0] = wxPoint(center.x - 40 * scale, center.y - 20 * scale);//����
            points[1] = wxPoint(center.x - 40 * scale, center.y + 20 * scale);//����
            points[2] = wxPoint(center.x + 40 * scale, center.y - 20 * scale);//����
            points[3] = wxPoint(center.x + 40 * scale, center.y + 20 * scale);//����
            closePoint = wxPoint(center.x, center.y + 40 * scale);
            rotationCenter = wxPoint(center.x, center.y - 40 * scale);

            pin1 = wxPoint(center.x - 40 * scale, center.y);
            pout = wxPoint(center.x + 40 * scale, center.y);
        }

        // ��ת���е�λ
        for (int i = 0; i < 4; ++i) {
            points[i] = RotatePoint(points[i], rotationCenter, rotationAngle);
        }
        pin1 = RotatePoint(pin1, rotationCenter, rotationAngle);
        pin2 = RotatePoint(pin2, rotationCenter, rotationAngle);
        pout = RotatePoint(pout, rotationCenter, rotationAngle);
        closePoint = RotatePoint(closePoint, rotationCenter, rotationAngle);
    }

    void Show(wxMemoryDC& memDC) {
        UpdateGeometry();

        wxGraphicsContext* gc = wxGraphicsContext::Create(memDC);
        if (!gc) return;

        gc->SetPen(wxPen(wxColour(0, 0, 0), 3));
        wxGraphicsPath path = gc->CreatePath();
        newCenter = RotatePoint(center, rotationCenter, rotationAngle);
        if (type == ANDGate) {
            wxPoint p1 = RotatePoint(wxPoint(center.x + 20 * scale, center.y - 20 * scale), rotationCenter, rotationAngle);
            wxPoint p2 = RotatePoint(wxPoint(center.x - 20 * scale, center.y - 20 * scale), rotationCenter, rotationAngle);
            wxPoint p3 = RotatePoint(wxPoint(center.x - 20 * scale, center.y + 20 * scale), rotationCenter, rotationAngle);
            wxPoint p4 = RotatePoint(wxPoint(center.x + 20 * scale, center.y + 20 * scale), rotationCenter, rotationAngle);
            wxPoint arcCenter = RotatePoint(wxPoint(center.x + 20 * scale, center.y), rotationCenter, rotationAngle);

            path.MoveToPoint(p1.x, p1.y);
            path.AddLineToPoint(p2.x, p2.y);
            path.AddLineToPoint(p3.x, p3.y);
            path.AddLineToPoint(p4.x, p4.y);
            path.AddArc(arcCenter.x, arcCenter.y, 20 * scale, M_PI / 2 + rotationAngle, -M_PI / 2 + rotationAngle, false);

            wxPoint pin1 = RotatePoint(wxPoint(center.x - 40 * scale, center.y + 10 * scale), rotationCenter, rotationAngle);
            wxPoint pin2 = RotatePoint(wxPoint(center.x - 40 * scale, center.y - 10 * scale), rotationCenter, rotationAngle);
            wxPoint pinOut = RotatePoint(wxPoint(center.x + 60 * scale, center.y), rotationCenter, rotationAngle);

            path.MoveToPoint(pin1.x, pin1.y);
            path.AddLineToPoint(pin1.x + 20 * scale * std::cos(rotationAngle), pin1.y + 20 * scale * std::sin(rotationAngle));
            path.MoveToPoint(pin2.x, pin2.y);
            path.AddLineToPoint(pin2.x + 20 * scale * std::cos(rotationAngle), pin2.y + 20 * scale * std::sin(rotationAngle));
            path.MoveToPoint(pinOut.x, pinOut.y);
            path.AddLineToPoint(pinOut.x - 20 * scale * std::cos(rotationAngle), pinOut.y - 20 * scale * std::sin(rotationAngle));
        }
        else if (type == ORGate) {
            wxPoint p1 = RotatePoint(wxPoint(center.x, center.y - 20 * scale), rotationCenter, rotationAngle);
            wxPoint p2 = RotatePoint(wxPoint(center.x - 20 * scale, center.y - 20 * scale), rotationCenter, rotationAngle);
            wxPoint p3 = RotatePoint(wxPoint(center.x - 20 * scale, center.y + 20 * scale), rotationCenter, rotationAngle);
            wxPoint p4 = RotatePoint(wxPoint(center.x - 10 * scale, center.y + 20 * scale), rotationCenter, rotationAngle);
            wxPoint p5 = RotatePoint(wxPoint(center.x + 20 * scale, center.y + 20 * scale), rotationCenter, rotationAngle);
            wxPoint p6 = RotatePoint(wxPoint(center.x + 40 * scale, center.y), rotationCenter, rotationAngle);
            wxPoint p7 = RotatePoint(wxPoint(center.x + 20 * scale, center.y - 20 * scale), rotationCenter, rotationAngle);

            path.MoveToPoint(p1.x, p1.y);
            path.AddLineToPoint(p2.x, p2.y);
            path.AddQuadCurveToPoint(newCenter.x, newCenter.y, p3.x, p3.y);
            path.AddLineToPoint(p4.x, p4.y);
            path.AddQuadCurveToPoint(p5.x, p5.y, p6.x, p6.y);
            path.AddQuadCurveToPoint(p7.x, p7.y, p1.x, p1.y);

            wxPoint pin1 = RotatePoint(wxPoint(center.x - 40 * scale, center.y + 10 * scale), rotationCenter, rotationAngle);
            wxPoint pin2 = RotatePoint(wxPoint(center.x - 40 * scale, center.y - 10 * scale), rotationCenter, rotationAngle);
            wxPoint pinOut = RotatePoint(wxPoint(center.x + 60 * scale, center.y), rotationCenter, rotationAngle);

            path.MoveToPoint(pin1.x, pin1.y);
            path.AddLineToPoint(pin1.x + 26 * scale * std::cos(rotationAngle), pin1.y + 26 * scale * std::sin(rotationAngle));
            path.MoveToPoint(pin2.x, pin2.y);
            path.AddLineToPoint(pin2.x + 26 * scale * std::cos(rotationAngle), pin2.y + 26 * scale * std::sin(rotationAngle));
            path.MoveToPoint(pinOut.x, pinOut.y);
            path.AddLineToPoint(pinOut.x - 20 * scale * std::cos(rotationAngle), pinOut.y - 20 * scale * std::sin(rotationAngle));
        }
        else if (type == NOTGate) {
            wxPoint p1 = RotatePoint(wxPoint(center.x + 20 * scale, center.y), rotationCenter, rotationAngle);
            wxPoint p2 = RotatePoint(wxPoint(center.x - 20 * scale, center.y - 20 * scale), rotationCenter, rotationAngle);
            wxPoint p3 = RotatePoint(wxPoint(center.x - 20 * scale, center.y + 20 * scale), rotationCenter, rotationAngle);
            wxPoint circleCenter = RotatePoint(wxPoint(center.x + 24 * scale, center.y), rotationCenter, rotationAngle);
            wxPoint inputStart = RotatePoint(wxPoint(center.x - 40 * scale, center.y), rotationCenter, rotationAngle);
            wxPoint inputEnd = RotatePoint(wxPoint(center.x - 20 * scale, center.y), rotationCenter, rotationAngle);
            wxPoint outputStart = RotatePoint(wxPoint(center.x + 28 * scale, center.y), rotationCenter, rotationAngle);
            wxPoint outputEnd = RotatePoint(wxPoint(center.x + 48 * scale, center.y), rotationCenter, rotationAngle);

            path.MoveToPoint(p1.x, p1.y);
            path.AddLineToPoint(p2.x, p2.y);
            path.AddLineToPoint(p3.x, p3.y);
            path.AddLineToPoint(p1.x, p1.y);
            path.AddEllipse(circleCenter.x - 4 * scale, circleCenter.y - 4 * scale, 8 * scale, 8 * scale);
            path.MoveToPoint(inputStart.x, inputStart.y);
            path.AddLineToPoint(inputEnd.x, inputEnd.y);
            path.MoveToPoint(outputStart.x, outputStart.y);
            path.AddLineToPoint(outputEnd.x, outputEnd.y);
        }
        else if (type == Resistor) {
            wxPoint p1 = RotatePoint(wxPoint(center.x - 40 * scale, center.y - 10 * scale), rotationCenter, rotationAngle); // ����
            wxPoint p2 = RotatePoint(wxPoint(center.x - 40 * scale, center.y + 10 * scale), rotationCenter, rotationAngle); // ����
            wxPoint p3 = RotatePoint(wxPoint(center.x - 30 * scale, center.y - 10 * scale), rotationCenter, rotationAngle); // ���һ����
            wxPoint p4 = RotatePoint(wxPoint(center.x - 30 * scale, center.y + 10 * scale), rotationCenter, rotationAngle); // ���һ����
            wxPoint p5 = RotatePoint(wxPoint(center.x - 20 * scale, center.y - 10 * scale), rotationCenter, rotationAngle); // ��ڶ�����
            wxPoint p6 = RotatePoint(wxPoint(center.x - 20 * scale, center.y + 10 * scale), rotationCenter, rotationAngle); // ��ڶ�����
            wxPoint p7 = RotatePoint(wxPoint(center.x - 10 * scale, center.y - 10 * scale), rotationCenter, rotationAngle); // ���������
            wxPoint p8 = RotatePoint(wxPoint(center.x - 10 * scale, center.y + 10 * scale), rotationCenter, rotationAngle); // ���������
            wxPoint p9 = RotatePoint(wxPoint(center.x, center.y - 10 * scale), rotationCenter, rotationAngle); // �м����
            wxPoint p10 = RotatePoint(wxPoint(center.x, center.y + 10 * scale), rotationCenter, rotationAngle); // �м����
            wxPoint p11 = RotatePoint(wxPoint(center.x + 10 * scale, center.y - 10 * scale), rotationCenter, rotationAngle); // �ҵ�������
            wxPoint p12 = RotatePoint(wxPoint(center.x + 10 * scale, center.y + 10 * scale), rotationCenter, rotationAngle); // �ҵ�������
            wxPoint p13 = RotatePoint(wxPoint(center.x + 20 * scale, center.y - 10 * scale), rotationCenter, rotationAngle); // �ҵڶ�����
            wxPoint p14 = RotatePoint(wxPoint(center.x + 20 * scale, center.y + 10 * scale), rotationCenter, rotationAngle); // �ҵڶ�����
            wxPoint p15 = RotatePoint(wxPoint(center.x + 30 * scale, center.y - 10 * scale), rotationCenter, rotationAngle); // �ҵ�һ����
            wxPoint p16 = RotatePoint(wxPoint(center.x + 30 * scale, center.y + 10 * scale), rotationCenter, rotationAngle); // �ҵ�һ����
            wxPoint p17 = RotatePoint(wxPoint(center.x + 40 * scale, center.y - 10 * scale), rotationCenter, rotationAngle); // ����
            wxPoint p18 = RotatePoint(wxPoint(center.x + 40 * scale, center.y + 10 * scale), rotationCenter, rotationAngle); // ����

            wxPoint inputStart = RotatePoint(wxPoint(center.x - 50 * scale, center.y), rotationCenter, rotationAngle); // �������
            wxPoint inputEnd = RotatePoint(wxPoint(center.x - 40 * scale, center.y), rotationCenter, rotationAngle);   // �����յ�
            wxPoint outputStart = RotatePoint(wxPoint(center.x + 40 * scale, center.y), rotationCenter, rotationAngle); // ������
            wxPoint outputEnd = RotatePoint(wxPoint(center.x + 50 * scale, center.y), rotationCenter, rotationAngle);   // ����յ�

            path.MoveToPoint(inputStart.x, inputStart.y);   // ������
            path.AddLineToPoint(inputEnd.x, inputEnd.y);

            path.MoveToPoint(p1.x, p1.y);   // ����
            path.AddLineToPoint(p2.x, p2.y); // ����
            path.AddLineToPoint(p4.x, p4.y); // ��һ����
            path.AddLineToPoint(p3.x, p3.y); // ��һ����
            path.AddLineToPoint(p5.x, p5.y); // �ڶ�����
            path.AddLineToPoint(p6.x, p6.y); // �ڶ�����
            path.AddLineToPoint(p8.x, p8.y); // ��������
            path.AddLineToPoint(p7.x, p7.y); // ��������
            path.AddLineToPoint(p9.x, p9.y); // �м����
            path.AddLineToPoint(p10.x, p10.y); // �м����
            path.AddLineToPoint(p12.x, p12.y); // �ҵ�������
            path.AddLineToPoint(p11.x, p11.y); // �ҵ�������
            path.AddLineToPoint(p13.x, p13.y); // �ҵڶ�����
            path.AddLineToPoint(p14.x, p14.y); // �ҵڶ�����
            path.AddLineToPoint(p16.x, p16.y); // �ҵ�һ����
            path.AddLineToPoint(p15.x, p15.y); // �ҵ�һ����
            path.AddLineToPoint(p17.x, p17.y); // ����
            path.AddLineToPoint(p18.x, p18.y); // ����
            path.CloseSubpath();             // �պ�·��

            path.MoveToPoint(outputStart.x, outputStart.y); // �����
            path.AddLineToPoint(outputEnd.x, outputEnd.y);
        }
        else if (type == Capacitor) {
            // ���ݵ�������������
            wxPoint line1Start = RotatePoint(wxPoint(center.x - 20 * scale, center.y - 20 * scale), rotationCenter, rotationAngle);
            wxPoint line1End = RotatePoint(wxPoint(center.x - 20 * scale, center.y + 20 * scale), rotationCenter, rotationAngle);
            wxPoint line2Start = RotatePoint(wxPoint(center.x + 20 * scale, center.y - 20 * scale), rotationCenter, rotationAngle);
            wxPoint line2End = RotatePoint(wxPoint(center.x + 20 * scale, center.y + 20 * scale), rotationCenter, rotationAngle);

            // �����������
            wxPoint inputStart = RotatePoint(wxPoint(center.x - 40 * scale, center.y), rotationCenter, rotationAngle);
            wxPoint inputEnd = RotatePoint(wxPoint(center.x - 20 * scale, center.y), rotationCenter, rotationAngle);
            wxPoint outputStart = RotatePoint(wxPoint(center.x + 20 * scale, center.y), rotationCenter, rotationAngle);
            wxPoint outputEnd = RotatePoint(wxPoint(center.x + 40 * scale, center.y), rotationCenter, rotationAngle);

            // ���Ƶ��ݵ�������������
            path.MoveToPoint(line1Start.x, line1Start.y);
            path.AddLineToPoint(line1End.x, line1End.y);

            path.MoveToPoint(line2Start.x, line2Start.y);
            path.AddLineToPoint(line2End.x, line2End.y);

            // ������������
            path.MoveToPoint(inputStart.x, inputStart.y);
            path.AddLineToPoint(inputEnd.x, inputEnd.y);

            // �����������
            path.MoveToPoint(outputStart.x, outputStart.y);
            path.AddLineToPoint(outputEnd.x, outputEnd.y);
        }
        else if (type == Diode) {
            // �����������εĶ���
            wxPoint triangleTip = RotatePoint(wxPoint(center.x + 20 * scale, center.y), rotationCenter, rotationAngle);
            wxPoint triangleBaseLeft = RotatePoint(wxPoint(center.x - 20 * scale, center.y - 20 * scale), rotationCenter, rotationAngle);
            wxPoint triangleBaseRight = RotatePoint(wxPoint(center.x - 20 * scale, center.y + 20 * scale), rotationCenter, rotationAngle);

            // ����������������
            wxPoint cathodeLineStart = RotatePoint(wxPoint(center.x + 20 * scale, center.y - 20 * scale), rotationCenter, rotationAngle);
            wxPoint cathodeLineEnd = RotatePoint(wxPoint(center.x + 20 * scale, center.y + 20 * scale), rotationCenter, rotationAngle);

            // �����������
            wxPoint inputStart = RotatePoint(wxPoint(center.x - 40 * scale, center.y), rotationCenter, rotationAngle);
            wxPoint inputEnd = RotatePoint(wxPoint(center.x - 20 * scale, center.y), rotationCenter, rotationAngle);
            wxPoint outputStart = RotatePoint(wxPoint(center.x + 20 * scale, center.y), rotationCenter, rotationAngle);
            wxPoint outputEnd = RotatePoint(wxPoint(center.x + 40 * scale, center.y), rotationCenter, rotationAngle);

            // ������������
            path.MoveToPoint(inputStart.x, inputStart.y);
            path.AddLineToPoint(inputEnd.x, inputEnd.y);

            // ���ƶ����ܵ�������
            path.MoveToPoint(triangleTip.x, triangleTip.y);           // �����ζ���
            path.AddLineToPoint(triangleBaseLeft.x, triangleBaseLeft.y); // ��������׽�
            path.AddLineToPoint(triangleBaseRight.x, triangleBaseRight.y); // �������ҵ׽�
            path.AddLineToPoint(triangleTip.x, triangleTip.y);        // �ص������ζ���

            // ������������
            path.MoveToPoint(cathodeLineStart.x, cathodeLineStart.y);
            path.AddLineToPoint(cathodeLineEnd.x, cathodeLineEnd.y);

            // �����������
            path.MoveToPoint(outputStart.x, outputStart.y);
            path.AddLineToPoint(outputEnd.x, outputEnd.y);
        }
        else if(type == Power) {
            // ����Դ��Բ�����ĺͰ뾶
            wxPoint circleCenter = RotatePoint(wxPoint(center.x, center.y), rotationCenter, rotationAngle);
            double circleRadius = 20 * scale;

            // ��ͷ����㡢�����ͼ�ͷ�����
            wxPoint arrowStart = RotatePoint(wxPoint(center.x - 10 * scale, center.y), rotationCenter, rotationAngle);
            wxPoint arrowTip = RotatePoint(wxPoint(center.x + 10 * scale, center.y), rotationCenter, rotationAngle);
            wxPoint arrowLeft = RotatePoint(wxPoint(center.x + 5 * scale, center.y - 5 * scale), rotationCenter, rotationAngle);
            wxPoint arrowRight = RotatePoint(wxPoint(center.x + 5 * scale, center.y + 5 * scale), rotationCenter, rotationAngle);

            // �����������
            wxPoint inputStart = RotatePoint(wxPoint(center.x - 40 * scale, center.y), rotationCenter, rotationAngle);
            wxPoint inputEnd = RotatePoint(wxPoint(center.x - 20 * scale, center.y), rotationCenter, rotationAngle);
            wxPoint outputStart = RotatePoint(wxPoint(center.x + 20 * scale, center.y), rotationCenter, rotationAngle);
            wxPoint outputEnd = RotatePoint(wxPoint(center.x + 40 * scale, center.y), rotationCenter, rotationAngle);

            // ������������
            path.MoveToPoint(inputStart.x, inputStart.y);
            path.AddLineToPoint(inputEnd.x, inputEnd.y);

            // ����Բ��
            path.AddCircle(circleCenter.x, circleCenter.y, circleRadius);

            // ���Ƽ�ͷ
            path.MoveToPoint(arrowStart.x, arrowStart.y);   // ��ͷ�����
            path.AddLineToPoint(arrowTip.x, arrowTip.y);   // ��ͷ�ļ��
            path.AddLineToPoint(arrowLeft.x, arrowLeft.y); // ��ͷ���
            path.MoveToPoint(arrowTip.x, arrowTip.y);      // �ص���ͷ���
            path.AddLineToPoint(arrowRight.x, arrowRight.y); // ��ͷ�Ҳ�

            // �����������
            path.MoveToPoint(outputStart.x, outputStart.y);
            path.AddLineToPoint(outputEnd.x, outputEnd.y);
        }

        gc->StrokePath(path);
        delete(gc);
    }
	~Component() {}        
};



class TextBox {
public:
    wxPoint position;
    wxSize size;//�ı����С
    wxString text;//�ı�����
    wxString wrappedText;//�ı�����
    wxFont font;//����
    double rotation;//��ת�Ƕ�
    bool resizing;     // �Ƿ����ڵ�����С
    bool isDrawn;
    wxPoint dragStart; // �����ʱ��λ��

    TextBox(wxPoint pos, const wxString& initialText)
        : position(pos), size(100, 50), text(initialText), font(12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL), rotation(0) {}

    void Draw(wxMemoryDC& memDC) {
        memDC.SetFont(font);
        AutoWrapText(memDC);
        // ��ȡ�ı���ĸ߶Ⱥ�ÿ�еĸ߶�
        int textBoxHeight = this->size.GetHeight();
        int charHeight;
        memDC.GetTextExtent("W", NULL, &charHeight);  // ��ȡ�ַ��߶�

        // �����������
        int maxLines = textBoxHeight / charHeight;

        wxString line;
        wxStringTokenizer tokenizer(wrappedText, "\n");
        int y = position.y;

        // ֻ��������ʾ������
        int linesDrawn = 0;
        while (tokenizer.HasMoreTokens()) {
            line = tokenizer.GetNextToken();
            if (linesDrawn < maxLines) {
                memDC.DrawText(line, position.x, y);
                y += charHeight;
                linesDrawn++;
            }
            else {
                break;  // ��������ʾ��������ֹͣ����
            }
        }

        // ���û���㹻�Ŀռ���ʾһ�У�����ʾ�κ�����
        if (linesDrawn == 0) {
            wrappedText = "";
        }
    }

    void DrawResizeBox(wxMemoryDC& memDC) {
        // ���Ƶ��������ĸ��ǵ�С���ο�
        int cornerSize = 10; // С���ο�Ĵ�С
        memDC.DrawRectangle(position.x - cornerSize, position.y - cornerSize, cornerSize, cornerSize);
        memDC.DrawRectangle(position.x + size.x - cornerSize, position.y - cornerSize, cornerSize, cornerSize);
        memDC.DrawRectangle(position.x - cornerSize, position.y + size.y - cornerSize, cornerSize, cornerSize);
        memDC.DrawRectangle(position.x + size.x - cornerSize , position.y + size.y - cornerSize, cornerSize, cornerSize);
    }

    void AutoWrapText(wxMemoryDC& memDC) {
        int textBoxWidth = this->size.GetWidth();
        int charWidth, charHeight;
        memDC.SetFont(font);
        memDC.GetTextExtent("W", &charWidth, &charHeight);

        if (textBoxWidth < charWidth) {
            this->wrappedText = "";  // ��Ȳ�������ʾ�κ��ַ�������ʾ�ı�
            return;
        }

        int maxCharsPerLine = textBoxWidth / charWidth;
        wxString wrappedText;
        wxString currentLine;
        wxString remainingText = this->text;

        while (!remainingText.IsEmpty()) {
            wxString word = remainingText.SubString(0, 0);
            remainingText = remainingText.Mid(1);

            if (currentLine.Length() + word.Length() <= maxCharsPerLine) {
                currentLine += word;
            }
            else {
                wrappedText += currentLine + "\n";
                currentLine = word;
            }
        }

        if (!currentLine.IsEmpty()) {
            wrappedText += currentLine;
        }

        this->wrappedText = wrappedText;  // �洢��ֺ���ı�
    }

    void SetRotation(double angle) {
        rotation = angle;
    }

    void SetFont(const wxFont& newFont) {
        font = newFont;
    }

    void SetText(const wxString& newText) {
        text = newText;
    }

    bool IsMouseOnResizeCorner(const wxPoint& mousePos) {
        int resizeArea = 10;
        wxPoint resizeCorner = position + size;
        wxRect resizeRect(resizeCorner.x - resizeArea, resizeCorner.y - resizeArea, resizeArea * 2, resizeArea * 2);
        return resizeRect.Contains(mousePos);
    }
};