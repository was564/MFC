
// ChildView.cpp: CChildView 클래스의 구현
//

#include "pch.h"
#include "framework.h"
#include "HW3.h"
#include "ChildView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define RED 100
#define GREEN 101
#define BLUE 102
#define BLACK 103

#define RECTANGLE 200
#define CIRCLE 201

// CChildView

CChildView::CChildView() : color(BLACK), shape(RECTANGLE), count(0)
{
}

CChildView::~CChildView()
{
}


BEGIN_MESSAGE_MAP(CChildView, CWnd)
	ON_WM_PAINT()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_CREATE()
	ON_WM_TIMER()
	ON_COMMAND(ID_SHAPE_CIRCLE, &CChildView::OnShapeCircle)
	ON_COMMAND(ID_SHAPE_RECTANGLE, &CChildView::OnShapeRectangle)
	ON_COMMAND(ID_COLOR_BLUE, &CChildView::OnColorBlue)
	ON_COMMAND(ID_COLOR_GREEN, &CChildView::OnColorGreen)
	ON_COMMAND(ID_COLOR_RED, &CChildView::OnColorRed)
	ON_COMMAND(ID_ERASE, &CChildView::OnErase)
	ON_COMMAND(ID_COLOR_BLACK, &CChildView::OnColorBlack)
	ON_UPDATE_COMMAND_UI(ID_COLOR_BLACK, &CChildView::OnUpdateColorBlack)
	ON_UPDATE_COMMAND_UI(ID_COLOR_BLUE, &CChildView::OnUpdateColorBlue)
	ON_UPDATE_COMMAND_UI(ID_COLOR_GREEN, &CChildView::OnUpdateColorGreen)
	ON_UPDATE_COMMAND_UI(ID_COLOR_RED, &CChildView::OnUpdateColorRed)
	ON_UPDATE_COMMAND_UI(ID_ERASE, &CChildView::OnUpdateErase)
	ON_UPDATE_COMMAND_UI(ID_SHAPE_CIRCLE, &CChildView::OnUpdateShapeCircle)
	ON_UPDATE_COMMAND_UI(ID_SHAPE_RECTANGLE, &CChildView::OnUpdateShapeRectangle)
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()



// CChildView 메시지 처리기

BOOL CChildView::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		::LoadCursor(nullptr, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW+1), nullptr);

	return TRUE;
}

void CChildView::OnPaint() 
{
	CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.
	CRect client;
	GetClientRect(client);

	CDC memDC;
	memDC.CreateCompatibleDC(&dc);
	CBitmap bitmap;
	bitmap.CreateCompatibleBitmap(&dc, client.Width(), client.Height());
	memDC.SelectObject(&bitmap);
	memDC.Rectangle(client);

	CString str;
	str.Format(_T("Mount of drawn shape : %lld"), points.GetSize());
	memDC.TextOut(0, 0, str);
	
	POSITION po = points.GetHeadPosition();
	POSITION sh = shapes.GetHeadPosition();
	POSITION co = colors.GetHeadPosition();
	CRect rect;
	CBrush rbrush, gbrush, bbrush, dbrush;
	rbrush.CreateSolidBrush(RGB(255, 0, 0));
	gbrush.CreateSolidBrush(RGB(0, 255, 0));
	bbrush.CreateSolidBrush(RGB(0, 0, 255));
	dbrush.CreateSolidBrush(RGB(0, 0, 0));

	while (po != NULL) {
		rect.SetRect(points.GetAt(po).x - 5,
			points.GetAt(po).y - 5,
			points.GetAt(po).x + 5,
			points.GetAt(po).y + 5
		);
		if (colors.GetAt(co) == RED) {
			memDC.SelectObject(&rbrush);
			if (shapes.GetAt(sh) == RECTANGLE) {
				memDC.Rectangle(rect);
			}
			else if (shapes.GetAt(sh) == CIRCLE) {
				memDC.Ellipse(rect);
			}
		}
		else if (colors.GetAt(co) == GREEN) {
			memDC.SelectObject(&gbrush);
			if (shapes.GetAt(sh) == RECTANGLE) {
				memDC.Rectangle(rect);
			}
			else if (shapes.GetAt(sh) == CIRCLE) {
				memDC.Ellipse(rect);
			}
		}
		else if (colors.GetAt(co) == BLUE) {
			memDC.SelectObject(&bbrush);
			if (shapes.GetAt(sh) == RECTANGLE) {
				memDC.Rectangle(rect);
			}
			else if (shapes.GetAt(sh) == CIRCLE) {
				memDC.Ellipse(rect);
			}
		}
		else if (colors.GetAt(co) == BLACK) {
			memDC.SelectObject(&dbrush);
			if (shapes.GetAt(sh) == RECTANGLE) {
				memDC.Rectangle(rect);
			}
			else if (shapes.GetAt(sh) == CIRCLE) {
				memDC.Ellipse(rect);
			}
		}
		points.GetNext(po);
		shapes.GetNext(sh);
		colors.GetNext(co);
	}

	dc.BitBlt(0, 0, client.Width(), client.Height(), &memDC, 0, 0, SRCCOPY);
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	
	// 그리기 메시지에 대해서는 CWnd::OnPaint()를 호출하지 마십시오.
}



void CChildView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	if (nFlags & MK_LBUTTON) {
		points.AddTail(point);
		shapes.AddTail(shape);
		colors.AddTail(color);
		count++;
	}
	else if (nFlags & MK_RBUTTON) {
		eraser.bottom = point.y;
		eraser.right = point.x;
	}
	Invalidate();

	CWnd::OnMouseMove(nFlags, point);
}


void CChildView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	colors.AddTail(color);
	shapes.AddTail(shape);
	points.AddTail(point);
	eraser.SetRectEmpty();

	count++;
	Invalidate();

	CWnd::OnLButtonDown(nFlags, point);
}


void CChildView::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	eraser.top = point.y;
	eraser.left = point.x;

	CWnd::OnRButtonDown(nFlags, point);
}


void CChildView::OnRButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	eraser.bottom = point.y;
	eraser.right = point.x;
	Invalidate();

	CWnd::OnRButtonUp(nFlags, point);
}


int CChildView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.

	return 0;
}


void CChildView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CWnd::OnTimer(nIDEvent);
}


void CChildView::OnShapeCircle()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	shape = CIRCLE;
}


void CChildView::OnShapeRectangle()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	shape = RECTANGLE;
}


void CChildView::OnColorBlue()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	color = BLUE;
}


void CChildView::OnColorGreen()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	color = GREEN;
}


void CChildView::OnColorRed()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	color = RED;
}

void CChildView::OnColorBlack()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	color = BLACK;
}

void CChildView::OnErase()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
}


void CChildView::OnUpdateColorBlack(CCmdUI* pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	if (color == BLACK) {
		pCmdUI->SetCheck(true);
	}
	else {
		pCmdUI->SetCheck(false);
	}
}


void CChildView::OnUpdateColorBlue(CCmdUI* pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	if (color == BLUE) {
		pCmdUI->SetCheck(true);
	}
	else {
		pCmdUI->SetCheck(false);
	}
}


void CChildView::OnUpdateColorGreen(CCmdUI* pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	if (color == GREEN) {
		pCmdUI->SetCheck(true);
	}
	else {
		pCmdUI->SetCheck(false);
	}
}


void CChildView::OnUpdateColorRed(CCmdUI* pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	if (color == RED) {
		pCmdUI->SetCheck(true);
	}
	else {
		pCmdUI->SetCheck(false);
	}
}


void CChildView::OnUpdateErase(CCmdUI* pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	
}


void CChildView::OnUpdateShapeCircle(CCmdUI* pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	if (shape == CIRCLE) {
		pCmdUI->SetCheck(true);
	}
	else {
		pCmdUI->SetCheck(false);
	}
}


void CChildView::OnUpdateShapeRectangle(CCmdUI* pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	if (shape == RECTANGLE) {
		pCmdUI->SetCheck(true);
	}
	else {
		pCmdUI->SetCheck(false);
	}
}


BOOL CChildView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	// return CWnd::OnEraseBkgnd(pDC);
	return true;
}
