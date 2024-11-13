#include "DrawBoard.h"
#include "windows.h"
#include <vector>
#pragma region ��
wxBEGIN_EVENT_TABLE(DrawBoard, wxPanel)
EVT_PAINT(DrawBoard::OnPaint)
EVT_LEFT_DOWN(DrawBoard::OnLeftClick)
EVT_LEFT_UP(DrawBoard::OnLeftUp)
EVT_RIGHT_DOWN(DrawBoard::OnRightClick)
EVT_MOTION(DrawBoard::OnMouseMove)
EVT_KEY_DOWN(DrawBoard::OnKeyDown)
EVT_KEY_UP(DrawBoard::OnKeyUp)
wxEND_EVENT_TABLE()
#pragma endregion
#pragma region ���캯������ʼ��
DrawBoard::DrawBoard(wxFrame* parent) : wxPanel(parent)
{
    //ȫ�ּ��̼����¼�
    this->Bind(wxEVT_CHAR_HOOK, &DrawBoard::OnKeyDown, this);
    //��·Ԫ����
    wxPoint ppp = wxPoint(100, 100);


    //��λ
    isShiftPressed = false;
    isLineDrawing = false;
    firstPointSet = false;
    isFreeDrawing = false;
    isBezierDrawing = false;
    SetBackgroundStyle(wxBG_STYLE_PAINT);
    mousePositionText = new wxTextCtrl(this, wxID_ANY, "", wxPoint(10, 10), wxSize(100, 30));
}

DrawBoard::~DrawBoard() { }
#pragma endregion

void DrawBoard::OnPaint(wxPaintEvent& event) {
#pragma region ��������
    wxAutoBufferedPaintDC dc(this);
    dc.SetBackground(*wxWHITE_BRUSH);
    dc.Clear();

    wxBitmap bufferBitmap(GetClientSize());
    wxMemoryDC memDC;
    memDC.SelectObject(bufferBitmap);
    memDC.SetBackground(*wxWHITE_BRUSH);
    memDC.Clear();
#pragma endregion
    AStarSearchPath(wxPoint(100, 100), wxPoint(200, 200), memDC);
    //Ԫ��������
    for (auto& component : Components) {
        component.Show(memDC);
        if (component.beConnected) {

        }
    }
    
    wxGraphicsContext* gc = wxGraphicsContext::Create(memDC);
    if (gc) {
#pragma region ǰ�ò���
        gc->SetAntialiasMode(wxANTIALIAS_DEFAULT);
        gc->SetInterpolationQuality(wxINTERPOLATION_DEFAULT);
        DrawGrid(gc, 20);
        DrawHorizontalAndVerticalLines(gc);
#pragma endregion

#pragma region ��ʾ�ɽ�����
        for (const auto& component : Components) {
            if (component.showPoints) {
                gc->SetPen(wxPen(wxColour(0, 0, 0), 1));
                for (int j = 0; j < 4; j++) {
                    gc->DrawEllipse(component.points[j].x - 5, component.points[j].y - 5, 10, 10);
                }
            }
            gc->SetPen(wxPen(wxColour(0, 0, 255), 3));
            if (component.connectingPin1) {
                gc->DrawEllipse(component.pin1.x - 5, component.pin1.y - 5, 10, 10);
            }
            else if (component.connectingPin2) {
                gc->DrawEllipse(component.pin2.x - 5, component.pin2.y - 5, 10, 10);
            }
            else if (component.connectingPout) {
                gc->DrawEllipse(component.pout.x - 5, component.pout.y - 5, 10, 10);
            }
        }
#pragma endregion

        gc->SetPen(wxPen(wxColour(0, 0, 0), 1));
        gc->SetBrush(*wxTRANSPARENT_BRUSH);

        //Ԫ��������
        for (const auto& connection : connections) {
            gc->StrokeLine(connection.startPoint.x, connection.startPoint.y, connection.endPoint.x, connection.endPoint.y);
        }

#pragma region �߻���
        if (isLineDrawing && firstPointSet) {
            gc->SetPen(wxPen(wxColour(0, 0, 255), 1));
            gc->StrokeLine(startPoint.x, startPoint.y, endPoint.x, endPoint.y);
        }
        if (!freeDrawPoints.empty()) {
            for (size_t i = 1; i < freeDrawPoints.size(); ++i) {
                gc->StrokeLine(freeDrawPoints[i - 1].x, freeDrawPoints[i - 1].y, freeDrawPoints[i].x, freeDrawPoints[i].y);
            }
        }
        for (const auto& line : lines) {
            gc->StrokeLine(line.first.x, line.first.y, line.second.x, line.second.y);
        }
        for (const auto& drawing : savedFreeDrawings) {
            for (size_t i = 1; i < drawing.size(); ++i) {
                gc->StrokeLine(drawing[i - 1].x, drawing[i - 1].y, drawing[i].x, drawing[i].y);
            }
        }
        for (const wxPoint& point : BezierControlPoints) {
            gc->DrawEllipse(point.x - 2.5, point.y - 2.5, 5, 5);
        }
        if (isBezierDrawing && BezierControlPoints.size() == 2) {
            gc->SetPen(wxPen(wxColour(0, 0, 0), 2, wxPENSTYLE_DOT));//����
            DrawQuadraticBezier(gc, true, BezierControlPoints[0], BezierControlPoints[1], tempPoint);
        }
        if (isBezierDrawing && BezierControlPoints.size() == 3) {
            gc->SetPen(wxPen(wxColour(0, 0, 0), 2, wxPENSTYLE_DOT));//����
            DrawCubicBezier(gc, true, BezierControlPoints[0], BezierControlPoints[1], BezierControlPoints[2], tempPoint);
        }
        for (const auto& curve : bezierCurveStorage) {
            if (curve.pointCount == 3) {
                //���ƶ��α���������
                gc->SetPen(wxPen(wxColour(0, 0, 0), 2));//ʵ��
                DrawQuadraticBezier(gc, false, curve.controlPoints[0], curve.controlPoints[1], curve.controlPoints[2]);
            }
            else if (curve.pointCount == 4) {
                //�������α���������
                gc->SetPen(wxPen(wxColour(0, 0, 0), 2));//ʵ��
                DrawCubicBezier(gc, false, curve.controlPoints[0], curve.controlPoints[1], curve.controlPoints[2], curve.controlPoints[3]);
            }
        }
#pragma endregion
        delete gc;
    }
    dc.Blit(0, 0, GetClientSize().GetWidth(), GetClientSize().GetHeight(), &memDC, 0, 0);
}

#pragma region ������߻��ƺ���
//���Ʒ�����
void DrawBoard::DrawGrid(wxGraphicsContext* gc, int gridSize) {
    wxSize size = GetSize();

    gc->SetPen(wxPen(wxColour(180, 180, 180), 1, wxPENSTYLE_DOT_DASH));
    for (int x = 0; x < size.x; x += gridSize) {
        gc->StrokeLine(x, 0, x, size.y);
    }

    for (int y = 0; y < size.y; y += gridSize) {
        gc->StrokeLine(0, y, size.x, y);
    }
}

//������뺯��
void DrawBoard::SnapToGrid(const wxPoint& pos, int gridSize) {
    wxPoint tempPos = ScreenToClient(pos);
    wxPoint posTo;
    if (tempPos.x % gridSize <= gridSize / 2) {
        posTo.x = gridSize * (tempPos.x / gridSize);
    }
    else {
        posTo.x = gridSize * (tempPos.x / gridSize + 1);
    }

    if (tempPos.y % gridSize <= gridSize / 2) {
        posTo.y = gridSize * (tempPos.y / gridSize);
    }
    else {
        posTo.y = gridSize * (tempPos.y / gridSize + 1);
    }
    wxPoint screenPoint = ClientToScreen(posTo);
    SetCursorPos(screenPoint.x, screenPoint.y);
}

//�߻��ƺ���
void DrawBoard::DrawHorizontalAndVerticalLines(wxGraphicsContext* gc) {
    if (mousePos.x >= 0 && mousePos.y >= 0) {
        gc->SetPen(wxPen(wxColour(0, 0, 0), 2));
        gc->StrokeLine(0, mousePos.y, GetSize().x, mousePos.y);
        gc->StrokeLine(mousePos.x, 0, mousePos.x, GetSize().y);

    }
}
void DrawBoard::DrawQuadraticBezier(wxGraphicsContext* gc, bool isWithLine, const wxPoint p1, const wxPoint p2, const wxPoint p3) {
    if (isWithLine) {
        gc->StrokeLine(p1.x, p1.y, p2.x, p2.y);
        gc->StrokeLine(p2.x, p2.y, p3.x, p3.y);
    }
    //�������α���������·��
    wxGraphicsPath path = gc->CreatePath();
    path.MoveToPoint(p1.x, p1.y);
    path.AddQuadCurveToPoint(p2.x, p2.y, p3.x, p3.y);

    gc->StrokePath(path);
}
void DrawBoard::DrawCubicBezier(wxGraphicsContext* gc, bool isWithLine, const wxPoint p1, const wxPoint p2, const wxPoint p3, const wxPoint p4) {
    if (isWithLine) {
        gc->StrokeLine(p1.x, p1.y, p2.x, p2.y);
        gc->StrokeLine(p2.x, p2.y, p3.x, p3.y);
        gc->StrokeLine(p3.x, p3.y, p4.x, p4.y);
    }
    //�������α���������·��
    wxGraphicsPath path = gc->CreatePath();
    path.MoveToPoint(p1.x, p1.y);
    path.AddCurveToPoint(p2.x, p2.y, p3.x, p3.y, p4.x, p4.y);

    gc->StrokePath(path);
}
#pragma endregion


//����ƶ��¼�
void DrawBoard::OnMouseMove(wxMouseEvent& event) {
    //��ȡ���λ��
    mousePos = event.GetPosition();
#pragma region ��ʾ���λ��
    wxString msg;
    msg.Printf(wxT("X:%d, Y:%d"), event.GetX(), event.GetY());
    mousePositionText->SetValue(msg);
#pragma endregion

#pragma region ��λ�ƶ�״̬���ݶ���
    if (isShiftPressed) {
        wxPoint currentMousePos = wxGetMousePosition();
        wxPoint deltaMousePos = currentMousePos - lastMousePos;
        if (abs(deltaMousePos.x) > 5 || abs(deltaMousePos.y) > 5) {
            if (abs(deltaMousePos.x) > abs(deltaMousePos.y)) {
                //�����ƶ�
                if (deltaMousePos.x > 0) {
                    currentMousePos.x += 20;
                }
                else {
                    currentMousePos.x -= 20;
                }
            }
            else {
                if (deltaMousePos.y > 0) {
                    currentMousePos.y += 20;
                }
                else {
                    currentMousePos.y -= 20;
                }
            }
            SnapToGrid(currentMousePos, 20);
            lastMousePos = currentMousePos;
        }
    }
#pragma endregion

    //��·Ԫ������
    for (auto& component : Components) {
        if (IsPointInCircle(mousePos, component.pin1, 5)) {
            component.connectingPin1 = true;
        }
        else if (IsPointInCircle(mousePos, component.pin2, 5)) {
            component.connectingPin2 = true;
        }
        else if (IsPointInCircle(mousePos, component.pout, 5)) {
            component.connectingPout = true;
        }
        else {
            component.connectingPin1 = component.connectingPin2 = component.connectingPout = false;
        }

        if (component.draggingTab && event.LeftIsDown()) {
            double targetScale = sqrt(pow(mousePos.x - component.center.x, 2) + pow(mousePos.y - component.center.y, 2))
                / sqrt(pow(component.points[component.pointIndex].x - component.center.x, 2) + pow(component.points[component.pointIndex].y - component.center.y, 2));
            component.scale += (targetScale - component.scale) * 0.5 * pow(component.scale, 0.5);

            if (component.scale < 0.5) {
                component.scale = 0.5;
            }
        }
        else if (component.movingTab && event.LeftIsDown()) {
            component.center = mousePos - XYoffset;
        }
    }
   
#pragma region �߻���
    if (isLineDrawing && firstPointSet) {
        endPoint = mousePos;
    }
    if (isFreeDrawing && event.LeftIsDown()) {
        freeDrawPoints.push_back(mousePos);
    }
    if (isBezierDrawing) {
        //�����߱�ʾ����
        tempPoint = mousePos;
    }
#pragma endregion

    Refresh();
}

//���������¼�
void DrawBoard::OnLeftClick(wxMouseEvent& event) {
    wxPoint mousePosition = event.GetPosition();
    
    for (auto& component : Components) {
        if (component.PointInside(mousePosition)) {
            //��������Ȧ��
            for (int i = 0; i < 4; i++) {
                if (IsPointInCircle(mousePosition, component.points[i], 5)) {
                    component.pointIndex = i;
                    component.draggingTab = true;
                }
            }
            //�����ڲ��հ�
            if (!component.draggingTab) {
                if (component.PointInside(mousePosition)) {
                    if (component.connectingPin1 = true) {
                        if (isConnecting = false) {
                            isConnecting = true;
                            connectStartPoint = component.pin1;
                        }
                        else {
                            isConnecting = false;
                            
                        }
                    }
                    component.movingTab = true;
                    XYoffset = wxPoint(mousePosition.x - component.center.x, mousePosition.y - component.center.y);
                }
            }
        }
    }
    
#pragma region �߻���״̬
    if (isLineDrawing) {
        //��һ��δ����----�����׶˵�
        if (!firstPointSet) {
            startPoint = mousePosition;
            endPoint = startPoint;
            firstPointSet = true;
        }
        //��һ��������----����β�˵㣬�����߶δ洢
        else {
            endPoint = mousePosition;
            lines.push_back(std::make_pair(startPoint, endPoint));
            firstPointSet = false;//��λ
        }
    }
    if (isFreeDrawing) {
        //�����ʱ�洢��Ϊ�գ�������ʱ�洢����
        if (freeDrawPoints.empty()) {
            freeDrawPoints.push_back(event.GetPosition());
        }
        //�洢����Ϊ�գ����ڻ��ƣ����������б��У����ҽ���ʱ�洢�����㣬�ص�if״̬���д洢
        else {
            savedFreeDrawings.push_back(freeDrawPoints);
            freeDrawPoints.clear();
        }
    }
    if (isBezierDrawing) {
        //��һ����͵ڶ�������������
        BezierControlPoints.push_back(mousePosition);

        //������������ĸ���ֹͣ���ƣ������ĸ���
        if (BezierControlPoints.size() == 4) {
            BezierCurve newCurve;
            newCurve.controlPoints = BezierControlPoints;
            newCurve.pointCount = 4;
            bezierCurveStorage.push_back(newCurve);
            BezierControlPoints.clear();
        }
    }
#pragma endregion

#pragma region Ԫ��������״̬
    if (placingAndGate || placingOrGate || placingNotGate) {
        if (placingAndGate) {
            Component newComponent = Component(mousePos, ANDGate);
            Components.push_back(newComponent);

        }
        else if (placingOrGate) {
            Component newComponent = Component(mousePos, ORGate);
            Components.push_back(newComponent);

        }
        else if (placingNotGate) {
            Component newComponent = Component(mousePos, NOTGate);
            Components.push_back(newComponent);
        }
        placingAndGate = false;
        placingOrGate = false;
        placingNotGate = false;
    }
#pragma endregion
    
    Refresh();
}

#pragma region �޹��¼�
//���̧������¼�
void DrawBoard::OnLeftUp(wxMouseEvent& event) {
    for (auto& component : Components) {
        if (component.draggingTab) {
            component.draggingTab = false;
            break;
        }
        else if (component.movingTab) {
            component.movingTab = false;
            break;
        }
    }
}

//������Ҽ��¼�
void DrawBoard::OnRightClick(wxMouseEvent& event) {
    if (isBezierDrawing) {
        if (BezierControlPoints.size() < 3) {
            BezierControlPoints.clear();
        }
        else if (BezierControlPoints.size() == 3) {
            BezierCurve newCurve;
            newCurve.controlPoints = BezierControlPoints;
            newCurve.pointCount = 3;
            bezierCurveStorage.push_back(newCurve);
            BezierControlPoints.clear();
        }
    }

    Refresh();
}

//���̰��¼��
void DrawBoard::OnKeyDown(wxKeyEvent& event) {
    //Shift�Խ�����
    if (event.GetKeyCode() == WXK_SHIFT) {
        isShiftPressed = true;
        wxPoint mousePos = wxGetMousePosition();
        SnapToGrid(mousePos, 20);
        lastMousePos = mousePos;
    }
    //BackSpaceɾ��Ԫ����
    if (event.GetKeyCode() == WXK_BACK) {
        for (auto it = Components.begin(); it != Components.end(); ++it) {
            if (it->showPoints) {
                it = Components.erase(it);
                break;
            }
        }
    }

    Refresh();
}

//�����ɿ����
void DrawBoard::OnKeyUp(wxKeyEvent& event) {
    //Shift�ɿ�ֹͣ�������
    if (event.GetKeyCode() == WXK_SHIFT) {
        isShiftPressed = false;
    }

    Refresh();
}
#pragma endregion



//���Ƿ���Բ��Χ��
bool DrawBoard::IsPointInCircle(const wxPoint& mousePos, const wxPoint& circlePos, int radius) {
    int distance = pow(mousePos.x - circlePos.x, 2) + pow(mousePos.y - circlePos.y, 2);
    return distance <= radius * radius;
}

void DrawBoard::AStarSearchPath(wxPoint startPoint, wxPoint endPoint, wxMemoryDC& memDC) {
    wxGraphicsContext* gc = wxGraphicsContext::Create(memDC);
    gc->SetPen(wxPen(wxColour(0, 0, 255), 3));
    if (gc) {
        barrierStorage.clear();
        bool isCollipse = false;
        for (auto& component : Components) {
            Barrier barrier = {
                component.points[0],
                component.points[1],
                component.points[2],
                component.points[3]
            };
            barrierStorage.push_back(barrier);
        }

        wxPoint turnOffPoint = wxPoint((startPoint.x + endPoint.x) / 2, startPoint.y);
        wxPoint turnBackPoint = wxPoint((startPoint.x + endPoint.x) / 2, endPoint.y);

        for (auto& rectangle : barrierStorage) {
            //��ʱ�������߶� startPoint->turnOffPoint, turnOffPoint->turnBackPoint, turnBackPoint->endPoint
            if (isSegmentIntersectRectangle(startPoint, turnOffPoint, rectangle) ||
                isSegmentIntersectRectangle(turnOffPoint, turnBackPoint, rectangle) ||
                isSegmentIntersectRectangle(turnBackPoint, endPoint, rectangle)) {
                isCollipse = true;
            }
        }
        if (!isCollipse) {
            wxGraphicsPath path = gc->CreatePath();
            path.MoveToPoint(startPoint);
            path.AddLineToPoint(turnOffPoint);
            path.AddLineToPoint(turnBackPoint);
            path.AddLineToPoint(endPoint);
            gc->StrokePath(path);
        }
    }
}

float crossProduct(const wxPoint& p1, const wxPoint& p2, const wxPoint& p3) {
    return (p2.x - p1.x) * (p3.y - p1.y) - (p2.y - p1.y) * (p3.x - p1.x);
}

bool isOnSegment(const wxPoint p1, const wxPoint& p2, const wxPoint& q) {
    return(q.x >= std::min(p1.x, p2.x) && q.x <= std::max(p1.x, p2.x) &&
           q.y >= std::min(p1.y, p2.y) && q.y <= std::max(p1.y, p2.y));
}

bool doLinesIntersect(const wxPoint& p1, const wxPoint& p2, const wxPoint& q1, const wxPoint& q2) {
    float d1 = crossProduct(q1, p1, p2);
    float d2 = crossProduct(q2, p1, p2);
    float d3 = crossProduct(p1, q1, q2);
    float d4 = crossProduct(p2, q1, q2);
    if (d1 * d2 < 0 && d3 * d4 < 0) {
        return true;
    }

    if (d1 == 0 && isOnSegment(p1, p2, q1)) return true;
    if (d2 == 0 && isOnSegment(p1, p2, q2)) return true;
    if (d3 == 0 && isOnSegment(q1, q2, p1)) return true;
    if (d4 == 0 && isOnSegment(q1, q2, p2)) return true;

    return false;
}

bool DrawBoard::isSegmentIntersectRectangle(const wxPoint& p1, const wxPoint& p2, const Barrier& rectangle) {
    if (doLinesIntersect(p1, p2, rectangle.leftUpNode, rectangle.leftDownNode) ||
        doLinesIntersect(p1, p2, rectangle.leftUpNode, rectangle.rightUpNode) ||
        doLinesIntersect(p1, p2, rectangle.leftDownNode, rectangle.rightDownNode) ||
        doLinesIntersect(p1, p2, rectangle.rightUpNode, rectangle.rightDownNode)) {
        return true;
    }
    return false;
}