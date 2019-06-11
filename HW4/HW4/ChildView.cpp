
// ChildView.cpp: CChildView 클래스의 구현
//

#include "pch.h"
#include "framework.h"
#include "HW4.h"
#include "ChildView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CChildView

CChildView::CChildView()
	:select(0), play(false)
{
}

CChildView::~CChildView()
{
}


BEGIN_MESSAGE_MAP(CChildView, CWnd)
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_LBUTTONDOWN()
	ON_WM_HSCROLL()
	ON_BN_CLICKED(101, OnButton1Click)
	ON_BN_CLICKED(102, OnButton2Click)
	ON_BN_CLICKED(103, OnButton3Click)
	ON_BN_CLICKED(104, OnButton4Click)
	ON_BN_CLICKED(105, OnButton5Click)
	ON_WM_TIMER()
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
	CRect rect;
	GetClientRect(rect);
	CDC memDC;
	memDC.CreateCompatibleDC(&dc);
	CBitmap bitmap;
	POSITION pos;
	CPoint point, temp;
	temp.SetPoint(-1, -1);
	bitmap.CreateCompatibleBitmap(&dc, rect.Width(), rect.Height());
	memDC.SelectObject(&bitmap);
	memDC.Rectangle(rect);
	CPen origin(PS_SOLID, 3, RGB(0, 0, 0));
	if (play) {
		pos = data.GetHeadPosition();
		CPen pen(PS_DASH, 1, RGB(150, 150, 150));
		while (pos != NULL) {
			memDC.SelectObject(&pen);
			point = data.GetAt(pos);
			if (temp != CPoint(-1, -1)) {
				memDC.MoveTo(temp);
				memDC.LineTo(point);
			}
			temp = point;
			data.GetNext(pos);
		}
		memDC.SelectObject(&origin);
		memDC.Rectangle(moveRect.x - 40, moveRect.y - 40, moveRect.x + 40, moveRect.y + 40);
	}
	else {
		CPen pen(PS_SOLID, 1, RGB(150, 150, 150));
		CPen red(PS_SOLID, 3, RGB(255, 0, 0));
		pos = data.GetHeadPosition();
		memDC.SelectObject(&pen);
		while (pos != NULL) {
			point = data.GetAt(pos);
			if (temp != CPoint(-1, -1)) {
				memDC.MoveTo(temp);
				memDC.LineTo(point);
			}
			temp = point;
			data.GetNext(pos);
		}
		pos = data.GetHeadPosition();
		int count = 1;
		while (pos != NULL) {
			if (select == count) {
				memDC.SelectObject(&red);
			}
			else {
				memDC.SelectObject(&origin);
			}
			point = data.GetAt(pos);
			memDC.Rectangle(point.x - 40, point.y - 40, point.x + 40, point.y + 40);
			data.GetNext(pos);
			count++;
		}
	}
	dc.BitBlt(0, 0, rect.Width(), rect.Height(), &memDC, 0, 0, SRCCOPY);
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	
	// 그리기 메시지에 대해서는 CWnd::OnPaint()를 호출하지 마십시오.
}



int CChildView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.
	m_Clear.Create(_T("CLEAR"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		CRect(0, 0, 60, 30), this, 101);
	m_Save.Create(_T("SAVE"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		CRect(60, 0, 120, 30), this, 102);
	m_Load.Create(_T("LOAD"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		CRect(120, 0, 180, 30), this, 103);
	m_Replay.Create(_T("REPLAY"), WS_CHILD | WS_VISIBLE | BS_RADIOBUTTON | BS_PUSHLIKE,
		CRect(180, 0, 240, 30), this, 104);
	m_Stop.Create(_T("STOP"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		CRect(240, 0, 300, 30), this, 105);

	m_HBar.Create(SBS_HORZ, CRect(300, 0, 700, 30), this, 106);
	m_HBar.ShowWindow(true);
	return 0;
}


void CChildView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (play) {
		return;
	}
	data.AddTail(point);
	
	m_HBar.SetScrollRange(1, data.GetSize());
	m_HBar.SetScrollPos(data.GetSize());
	select = m_HBar.GetScrollPos();
	Invalidate();
	CWnd::OnLButtonDown(nFlags, point);
}


void CChildView::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	select = pScrollBar->GetScrollPos();
	if (pScrollBar != NULL) {
		if (nSBCode == SB_THUMBTRACK || nSBCode == SB_THUMBPOSITION) {
			pScrollBar->SetScrollPos(nPos);
			this->Invalidate();
		}
		else if (nSBCode == SB_LINELEFT) {
			select -= 1;
			if (select < 1) {
				select = 1;
			}
			pScrollBar->SetScrollPos(select);
			this->Invalidate();
		}
		else if (nSBCode == SB_LINERIGHT) {
			select += 1;
			if (select > data.GetSize()) {
				select = data.GetSize();
			}
			pScrollBar->SetScrollPos(select);
			this->Invalidate();
		}
	}

	CWnd::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CChildView::OnButton1Click() { // clear
	data.RemoveAll();
	Invalidate();
}

void CChildView::OnButton2Click() { // save
	CFile file(_T("SaveData.dat"), CFile::modeWrite | CFile::modeCreate);

	CArchive ar(&file, CArchive::store);

	data.Serialize(ar);
}

void CChildView::OnButton3Click() { // load
	CFileException e;
	CFile file;
	if (!file.Open(_T("SaveData.dat"), CFile::modeRead, &e)) {
		e.ReportError();
		return;
	}

	CArchive ar(&file, CArchive::load);

	data.RemoveAll();
	data.Serialize(ar);
	Invalidate();
}

void CChildView::OnButton4Click() { // replay
	m_Replay.SetCheck(true);
	movePos = data.GetHeadPosition();
	if (movePos == NULL) {
		play = false;
		m_Replay.SetCheck(false);
		return;
	}
	moveRect = data.GetHead();
	moveSelect = 1;
	count = -1;
	temp = movePos;
	if (temp == NULL) {
		play = false;
		m_Replay.SetCheck(false);
		return;
	}
	data.GetNext(temp);
	if (temp == NULL) {
		play = false;
		m_Replay.SetCheck(false);
		return;
	}
	temp = movePos;
	SetTimer(0, 30, NULL);
	play = true;
	Invalidate();
}

void CChildView::OnButton5Click() { // stop
	m_Replay.SetCheck(false);
	m_HBar.SetScrollPos(select);
	KillTimer(0);
	play = false;
	Invalidate();
}

void CChildView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (nIDEvent == 0) {
		count++;
		temp = movePos;
		data.GetNext(temp);
		if (moveRect == data.GetAt(temp)) {
			moveSelect++;
			count = 0;
			data.GetNext(movePos);
			data.GetNext(temp);
		}
		if (temp == NULL) {
			moveSelect = 1;
			count = 0;
			movePos = data.GetHeadPosition();
			temp = movePos;
			data.GetNext(temp);
		}
		moveRect.x = data.GetAt(movePos).x + (data.GetAt(temp).x - data.GetAt(movePos).x) * count / 30;
		moveRect.y = data.GetAt(movePos).y + (data.GetAt(temp).y - data.GetAt(movePos).y) * count / 30;
		m_HBar.SetScrollPos(moveSelect);
		Invalidate();
		
	}
	CWnd::OnTimer(nIDEvent);
}


BOOL CChildView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	// return CWnd::OnEraseBkgnd(pDC);
	return true;
}
