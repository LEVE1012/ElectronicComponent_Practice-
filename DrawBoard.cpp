#include "DrawBoard.h"
#include "windows.h"
#include <vector>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <chrono>

wxBEGIN_EVENT_TABLE(DrawBoard, wxPanel)
EVT_PAINT(DrawBoard::OnPaint)
EVT_LEFT_DOWN(DrawBoard::OnLeftClick)
EVT_LEFT_DCLICK(DrawBoard::OnLeftDoubleClick)
EVT_LEFT_UP(DrawBoard::OnLeftUp)
EVT_RIGHT_DOWN(DrawBoard::OnRightClick)
EVT_MOTION(DrawBoard::OnMouseMove)
wxEND_EVENT_TABLE()

DrawBoard::DrawBoard(wxFrame* parent) : wxPanel(parent)
{
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

void DrawBoard::OnPaint(wxPaintEvent& event) {
    wxAutoBufferedPaintDC dc(this);
    dc.SetUserScale(scale, scale);
    dc.SetBackground(*wxWHITE_BRUSH);
    dc.Clear();

    wxBitmap bufferBitmap(GetClientSize());
    wxMemoryDC memDC;
    memDC.SelectObject(bufferBitmap);
    memDC.SetBackground(*wxWHITE_BRUSH);
    memDC.Clear();
    
    //Ԫ��������
    for (auto& component : Components) {
        component.Show(memDC);
    }
    for (auto& textBox : textBoxes) {
        textBox.Draw(memDC);
        textBox.AutoWrapText(memDC);
        int y = 0;
        wxString line;
        wxStringTokenizer tokenizer(textBox.wrappedText, "\n");
        while (tokenizer.HasMoreTokens()) {
            line = tokenizer.GetNextToken();
            dc.DrawText(line, 0, y);
            y += dc.GetTextExtent(line).y;  // ÿ�еĸ߶�����
        }

        if (textBox.isDrawn) {
            textBox.DrawResizeBox(memDC);
        }
    }
    wxGraphicsContext* gc = wxGraphicsContext::Create(memDC);
    if (gc) {
        gc->SetAntialiasMode(wxANTIALIAS_DEFAULT);
        gc->SetInterpolationQuality(wxINTERPOLATION_DEFAULT);
        DrawGrid(gc, 20);
        //DrawHorizontalAndVerticalLines(gc);

        for (const auto& component : Components) {
            if (component.showPoints) {
                gc->SetPen(wxPen(wxColour(0, 0, 0), 1));
                for (int j = 0; j < 4; j++) {
                    gc->DrawEllipse(component.points[j].x - 5, component.points[j].y - 5, 10, 10);
                }
                wxPoint p1(component.closePoint.x - 5, component.closePoint.y - 5);  // ���Ͻ�
                wxPoint p2(component.closePoint.x + 5, component.closePoint.y + 5);  // ���½�
                wxPoint p3(component.closePoint.x + 5, component.closePoint.y - 5);  // ���Ͻ�
                wxPoint p4(component.closePoint.x - 5, component.closePoint.y + 5);  // ���½�
                gc->DrawEllipse(component.closePoint.x - 5 * sqrt(2), component.closePoint.y - 5 * sqrt(2), 10 * sqrt(2), 10 * sqrt(2));
                gc->StrokeLine(p1.x, p1.y, p2.x, p2.y);
                gc->StrokeLine(p3.x, p3.y, p4.x, p4.y);

                double radius = 10 * component.scale;
                double arrowSize = 5 * component.scale;

                double arrowAngle = component.rotationAngle + M_PI;
                wxPoint arrowTip(
                    component.rotationCenter.x + radius * std::cos(arrowAngle),
                    component.rotationCenter.y + radius * std::sin(arrowAngle)
                );

                wxPoint arrowLeft(
                    arrowTip.x + arrowSize * std::cos(arrowAngle + 11 * M_PI / 24),
                    arrowTip.y + arrowSize * std::sin(arrowAngle + 11 * M_PI / 24)
                );
                wxPoint arrowRight(
                    arrowTip.x + arrowSize * std::cos(arrowAngle - 5 * M_PI / 4),
                    arrowTip.y + arrowSize * std::sin(arrowAngle - 5 * M_PI / 4)
                );

                gc->SetPen(wxPen(wxColour(0, 0, 0), 2));
                wxGraphicsPath arcPath = gc->CreatePath();
                arcPath.AddArc(component.rotationCenter.x, component.rotationCenter.y, radius, component.rotationAngle, component.rotationAngle + M_PI, false);
                gc->StrokePath(arcPath);

                gc->SetPen(wxPen(wxColour(0, 0, 0), 2));
                wxGraphicsPath arrowPath = gc->CreatePath();
                arrowPath.MoveToPoint(arrowLeft.x, arrowLeft.y);
                arrowPath.AddLineToPoint(arrowTip.x, arrowTip.y);
                arrowPath.MoveToPoint(arrowRight.x, arrowRight.y);
                arrowPath.AddLineToPoint(arrowTip.x, arrowTip.y);
                gc->StrokePath(arrowPath);
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

        gc->SetPen(wxPen(wxColour(0, 0, 0), 3));
        gc->SetBrush(*wxTRANSPARENT_BRUSH);

        if (isLineDrawing && firstPointSet) {
            gc->SetPen(wxPen(wxColour(0, 0, 255), 1));
            gc->StrokeLine(startPoint.x, startPoint.y, endPoint.x, endPoint.y);
        }
        if (!freeDrawPoints.empty()) {
            for (size_t i = 1; i < freeDrawPoints.size(); ++i) {
                gc->StrokeLine(freeDrawPoints[i - 1].x, freeDrawPoints[i - 1].y, freeDrawPoints[i].x, freeDrawPoints[i].y);
            }
        }
        if (!connectPoints.empty()) {
            for (size_t i = 1; i < connectPoints.size(); ++i) {
                wxPoint p1 = connectPoints[i - 1];
                wxPoint p2 = connectPoints[i];
                gc->StrokeLine(p1.x, p1.y, p2.x, p1.y);
                gc->StrokeLine(p2.x, p1.y, p2.x, p2.y);
            }
            wxPoint p1 = connectPoints[connectPoints.size() - 1];
            wxPoint p2 = connectTempPoint;
            gc->StrokeLine(p1.x, p1.y, p2.x, p1.y);
            gc->StrokeLine(p2.x, p1.y, p2.x, p2.y);
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
        for (const auto& path : connectPaths) {
            for (size_t i = 1; i < path.size(); ++i) {
                wxPoint p1 = path[i - 1];
                wxPoint p2 = path[i];
                gc->StrokeLine(p1.x, p1.y, p2.x, p1.y);
                gc->StrokeLine(p2.x, p1.y, p2.x, p2.y);
            }
        }
        
        delete gc;
    }
    dc.Blit(0, 0, GetClientSize().GetWidth(), GetClientSize().GetHeight(), &memDC, 0, 0);
}

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

//�߻��ƹ��ߺ���
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


//����ƶ��¼�
void DrawBoard::OnMouseMove(wxMouseEvent& event) {
    //��ȡ���λ��
    mousePos = event.GetPosition();
    wxString msg;
    msg.Printf(wxT("X:%d, Y:%d"), event.GetX(), event.GetY());
    mousePositionText->SetValue(msg);

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
        if (component.isRotating) {
            RotateVector = wxPoint(mousePos.x - component.rotationCenter.x, mousePos.y - component.rotationCenter.y);
            double currentAngle = std::atan2(RotateVector.y, RotateVector.x); // ��ǰ�Ƕ�
            component.rotationAngle += currentAngle - initialAngle; // ������ת�Ƕ�
            initialAngle = currentAngle; // ���³�ʼ�Ƕ�
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

        connectTempPoint = mousePos;
    }
    if (draggingTextBox) {
        wxPoint mousePos = event.GetPosition();
        draggingTextBox->position = mousePos - Textoffset;
        Refresh();
    }
    if (event.Dragging()) {
        wxPoint mousePos = event.GetPosition();
        for (auto& textBox : textBoxes) {
            if (textBox.resizing) {
                // ��������϶��ľ���
                int dx = mousePos.x - textBox.dragStart.x;
                int dy = mousePos.y - textBox.dragStart.y;

                // �����ı���Ŀ�Ⱥ͸߶�
                textBox.size.x += dx;
                textBox.size.y += dy;

                // �����϶���ʼλ��
                textBox.dragStart = mousePos;

                // ������С�ߴ�
                if (textBox.size.x < 50) textBox.size.x = 50;
                if (textBox.size.y < 20) textBox.size.y = 20;

                // ˢ�»��壬���»����ı���
                Refresh();
                break;
            }
        }
    }
    //Ԫ��������״̬
    if (isConnecting) {
        connectEndPoint = mousePos;
    }
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
    Refresh();
}

//���������¼�
void DrawBoard::OnLeftClick(wxMouseEvent& event) {
    wxPoint mousePosition = event.GetPosition();
    bool clickedOnTextBox = false;
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
            isLineDrawing = false;
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
            isBezierDrawing = false;
        }
    }
    if (isConnecting) {
        connectPoints.push_back(mousePosition);
        connectTempPoint = mousePosition;
    }

    for (auto& component : Components) {
        size_t index = 0;

        //����ɾ����ť��ɾ��Ԫ����
        if (IsPointInCircle(mousePosition, component.closePoint, 5 * sqrt(2))) {
            auto it = Components.begin();
            while (it != Components.end()) {
                if (it->showPoints) {
                    for (auto it2 = connectPaths.begin(); it2 != connectPaths.end();) {
                        auto& firstNode = it2->front();
                        auto& lastNode = it2->back();

                        if (firstNode == it->pin1 || firstNode == it->pin2 || firstNode == it->pout ||
                            lastNode == it->pin1 || lastNode == it->pin2 || lastNode == it->pout) {
                            it2 = connectPaths.erase(it2);
                        }
                        else {
                            ++it2;
                        }
                    }
                    it = Components.erase(it);
                    break;
                }
                else {
                    ++it;                }
            }
        }
        if (component.PointInside(mousePosition)) {
            component.showPoints = true;
            //��������Ȧ�ڣ�����
            for (int i = 0; i < 4; i++) {
                if (IsPointInCircle(mousePosition, component.points[i], 5)) {
                    component.pointIndex = i;
                    component.draggingTab = true;
                }
            }
            //�����ڲ��հף��ƶ� �� ����
            if (!component.draggingTab) {
                if (!isConnecting) {
                    //Ԫ�������ӿ�ʼ
                    if (IsPointInCircle(mousePosition, component.pin1, 5)) {
                        isConnecting = true;
                        connectPoints.push_back(component.pin1);
                    }
                    else if (IsPointInCircle(mousePosition, component.pin2, 5)) {
                        isConnecting = true;
                        connectPoints.push_back(component.pin2);
                    }
                    else if (IsPointInCircle(mousePosition, component.pout, 5)) {
                        isConnecting = true;
                        connectPoints.push_back(component.pout);
                    }
                }
                else {
                    //Ԫ�������ӽ���
                    if (IsPointInCircle(connectPoints.back(), component.pin1, 5)) {
                        isConnecting = false;
                        connectPoints.back() = component.pin1;
                        connectPaths.push_back(connectPoints);
                        connectPoints.clear();
                    }
                    else if (IsPointInCircle(connectPoints.back(), component.pin2, 5)) {
                        isConnecting = false;
                        connectPoints.back() = component.pin2;
                        connectPaths.push_back(connectPoints);
                        connectPoints.clear();
                    }
                    else if (IsPointInCircle(connectPoints.back(), component.pout, 5)) {
                        isConnecting = false;
                        connectPoints.back() = component.pout;
                        connectPaths.push_back(connectPoints);
                        connectPoints.clear();
                    }
                }
                component.movingTab = true;
                XYoffset = wxPoint(mousePosition.x - component.center.x, mousePosition.y - component.center.y);
            }
        }
        else {
            component.showPoints = false;
        }
        if (IsPointInCircle(mousePosition, component.rotationCenter, 10 * component.scale)) {
            RotateVector = wxPoint(mousePos.x - component.rotationCenter.x, mousePos.y - component.rotationCenter.y);
            initialAngle = std::atan2(RotateVector.y, RotateVector.x); // ��ʼ�Ƕ�
            component.isRotating = true; // ���������ת
            component.showPoints = true;
        }
    }
    for (auto& textBox : textBoxes) {
        if (IsPointInTextBox(mousePos, textBox) && !clickedOnTextBox) {
            textBox.isDrawn = true;
            clickedOnTextBox = true;
        }
        else {
            textBox.isDrawn = false;
        }
        if (textBox.isDrawn) {
            //�϶�
            if (IsPointInTextBox(mousePos, textBox) && !textBox.IsMouseOnResizeCorner(mousePos)) {
                draggingTextBox = &textBox;
                Textoffset = mousePos - textBox.position;
            }
            //����
            if (textBox.IsMouseOnResizeCorner(mousePos)) {
                textBox.resizing = true;
                textBox.dragStart = mousePos;
                CaptureMouse();
                break;
            }
        }
    }
    if (placingAndGate || placingOrGate || placingNotGate || placingResistor || placingCapacitor ||
        placingDiode || placingPower) {
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
        else if (placingResistor) {
            Component newComponent = Component(mousePos, Resistor);
            Components.push_back(newComponent);
        }
        else if (placingCapacitor) {
            Component newComponent = Component(mousePos, Capacitor);
            Components.push_back(newComponent);
        }
        else if (placingDiode) {
            Component newComponent = Component(mousePos, Diode);
            Components.push_back(newComponent);
        }
        else if (placingPower) {
            Component newComponent = Component(mousePos, Power);
            Components.push_back(newComponent);
        }
        placingAndGate = false;
        placingOrGate = false;
        placingNotGate = false;
        placingResistor = false;
        placingCapacitor = false;
        placingDiode = false;
        placingPower = false;

    }
    if (placingTextBox) {
        TextBox newTextBox(mousePosition, "���ı�");
        textBoxes.push_back(newTextBox);
        placingTextBox = false;
    }
    

    Refresh();
}

//���˫������¼�
void DrawBoard::OnLeftDoubleClick(wxMouseEvent& event) {
    wxPoint mousePos = event.GetPosition();

    for (auto& textBox : textBoxes) {
        if (IsPointInTextBox(mousePos, textBox)) {
            wxDialog dialog(this, wxID_ANY, "�޸��ı��������С");

            wxTextCtrl* textInput = new wxTextCtrl(&dialog, wxID_ANY, textBox.text, wxPoint(10, 10), wxSize(200, 50), wxTE_MULTILINE);
            wxTextCtrl* sizeInput = new wxTextCtrl(&dialog, wxID_ANY, std::to_string(textBox.font.GetPointSize()), wxPoint(10, 70), wxSize(100, 30));
            wxButton* okButton = new wxButton(&dialog, wxID_OK, "ȷ��", wxPoint(10, 110));
            dialog.SetSize(wxSize(230, 200));

            if (dialog.ShowModal() == wxID_OK) {
                wxString newText = textInput->GetValue();
                long newSize;
                if (sizeInput->GetValue().ToLong(&newSize)) {
                    textBox.SetText(newText);

                    wxFont newFont = textBox.font;
                    newFont.SetPointSize(static_cast<int>(newSize));
                    textBox.SetFont(newFont);
                }
            }
        }
    }
}

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
        component.isRotating = false; // ֹͣ��ת
    }

    draggingTextBox = nullptr;

    for (auto& textBox : textBoxes) {
        if (textBox.resizing) {
            textBox.resizing = false;
            ReleaseMouse();  // �ͷ���겶��
            break;
        }
    }

}

//������Ҽ��¼�
void DrawBoard::OnRightClick(wxMouseEvent& event) {
    wxPoint mousePosition = event.GetPosition();

    if (isBezierDrawing) {
        if (BezierControlPoints.size() < 3) {
            BezierControlPoints.clear();
            isBezierDrawing = false;
        }
        else if (BezierControlPoints.size() == 3) {
            BezierCurve newCurve;
            newCurve.controlPoints = BezierControlPoints;
            newCurve.pointCount = 3;
            bezierCurveStorage.push_back(newCurve);
            BezierControlPoints.clear();
            isBezierDrawing = false;
        }
    }

    if (isConnecting) {
        isConnecting = false;
        connectPoints.push_back(mousePos);
        connectPaths.push_back(connectPoints);
        connectPoints.clear();
    }

    Refresh();
}


//���Ƿ���Բ��Χ��
bool DrawBoard::IsPointInCircle(const wxPoint& mousePos, const wxPoint& circlePos, int radius) {
    int distance = pow(mousePos.x - circlePos.x, 2) + pow(mousePos.y - circlePos.y, 2);
    return distance <= radius * radius;
}
bool DrawBoard::IsPointInTextBox(wxPoint mousePos, TextBox textBox) {
    wxRect rect(textBox.position, textBox.size);
    return rect.Contains(mousePos);
}

