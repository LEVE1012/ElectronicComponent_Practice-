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
    NOTGate
};

class Component {
public:	
    //类型相关
    ComponentType type;

    // 位置相关
    wxPoint center;
    wxPoint newCenter;
    wxPoint pin1;
    wxPoint pin2;
    wxPoint pout;
    wxPoint points[4];
    wxPoint closePoint;
    wxPoint rotationCenter; // 旋转中心
    double rotationAngle = 0;   // 旋转角度，以弧度为单位
    double scale;

    // 交互相关
    int pointIndex;
    bool showPoints = true;
    bool draggingTab = false;
    bool movingTab = false;
    bool connectingPin1 = false;
    bool connectingPin2 = false;
    bool connectingPout = false;
    bool isRotating;

    //构造函数
    Component(wxPoint center, ComponentType type) {
        this->scale = 1.0;
        this->type = type;
        this->center = center;
    }

    bool PointInside(const wxPoint& p) {
        wxPoint pPrime = RotatePoint(p, rotationCenter, -rotationAngle);
        int left = center.x - 40 * scale;
        int right = center.x + 60 * scale;
        int top = center.y - 20 * scale;
        int bottom = center.y + 20 * scale;
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
        //动态计算 点位 和 针脚
        if (type == ANDGate) {
            points[0] = wxPoint(center.x - 40 * scale, center.y - 20 * scale);//左上
            points[1] = wxPoint(center.x - 40 * scale, center.y + 20 * scale);//左下
            points[2] = wxPoint(center.x + 60 * scale, center.y - 20 * scale);//右上
            points[3] = wxPoint(center.x + 60 * scale, center.y + 20 * scale);//右下
            closePoint = wxPoint(center.x  + 10 * scale, center.y + 40 * scale);
            rotationCenter = wxPoint(center.x + 10 * scale, center.y - 40 * scale);

            pin1 = wxPoint(center.x - 40 * scale, center.y + 10 * scale);
            pin2 = wxPoint(center.x - 40 * scale, center.y - 10 * scale);
            pout = wxPoint(center.x + 60 * scale, center.y);
        }
        if (type == ORGate) {
            points[0] = wxPoint(center.x - 40 * scale, center.y - 20 * scale);//左上
            points[1] = wxPoint(center.x - 40 * scale, center.y + 20 * scale);//左下
            points[2] = wxPoint(center.x + 60 * scale, center.y - 20 * scale);//右上
            points[3] = wxPoint(center.x + 60 * scale, center.y + 20 * scale);//右下
            closePoint = wxPoint(center.x + 10 * scale, center.y + 40 * scale);
            rotationCenter = wxPoint(center.x + 10 * scale, center.y - 40 * scale);

            pin1 = wxPoint(center.x - 40 * scale, center.y + 10 * scale);
            pin2 = wxPoint(center.x - 40 * scale, center.y - 10 * scale);
            pout = wxPoint(center.x + 60 * scale, center.y);
        }
        if (type == NOTGate) {
            points[0] = wxPoint(center.x - 40 * scale, center.y - 20 * scale);//左上
            points[1] = wxPoint(center.x - 40 * scale, center.y + 20 * scale);//左下
            points[2] = wxPoint(center.x + 48 * scale, center.y - 20 * scale);//右上
            points[3] = wxPoint(center.x + 48 * scale, center.y + 20 * scale);//右下
            closePoint = wxPoint(center.x + 4 * scale, center.y + 40 * scale);
            rotationCenter = wxPoint(center.x + 4 * scale, center.y - 40 * scale);

            pin1 = wxPoint(center.x - 40 * scale, center.y);
            pout = wxPoint(center.x + 48 * scale, center.y);
        }
        // 旋转所有点位
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
        
        gc->StrokePath(path);
        delete(gc);
    }
	~Component() {}        
};



class TextBox {
public:
    wxPoint position;
    wxSize size;//文本框大小
    wxString text;//文本内容
    wxString wrappedText;//文本内容
    wxFont font;//字体
    double rotation;//旋转角度
    bool resizing;     // 是否正在调整大小
    bool isDrawn;
    wxPoint dragStart; // 鼠标点击时的位置

    TextBox(wxPoint pos, const wxString& initialText)
        : position(pos), size(100, 50), text(initialText), font(12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL), rotation(0) {}

    void Draw(wxMemoryDC& memDC) {
        memDC.SetFont(font);
        AutoWrapText(memDC);
        // 获取文本框的高度和每行的高度
        int textBoxHeight = this->size.GetHeight();
        int charHeight;
        memDC.GetTextExtent("W", NULL, &charHeight);  // 获取字符高度

        // 计算最大行数
        int maxLines = textBoxHeight / charHeight;

        wxString line;
        wxStringTokenizer tokenizer(wrappedText, "\n");
        int y = position.y;

        // 只绘制能显示的行数
        int linesDrawn = 0;
        while (tokenizer.HasMoreTokens()) {
            line = tokenizer.GetNextToken();
            if (linesDrawn < maxLines) {
                memDC.DrawText(line, position.x, y);
                y += charHeight;
                linesDrawn++;
            }
            else {
                break;  // 超过可显示的行数，停止绘制
            }
        }

        // 如果没有足够的空间显示一行，则不显示任何内容
        if (linesDrawn == 0) {
            wrappedText = "";
        }
    }

    void DrawResizeBox(wxMemoryDC& memDC) {
        // 绘制调整区域（四个角的小方形框）
        int cornerSize = 10; // 小方形框的大小
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
            this->wrappedText = "";  // 宽度不足以显示任何字符，不显示文本
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

        this->wrappedText = wrappedText;  // 存储拆分后的文本
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