
// ChildView.h: CChildView 클래스의 인터페이스
//


#pragma once


// CChildView 창

class CChildView : public CWnd
{
	CScrollBar m_HBar;
	CButton m_Clear, m_Save, m_Load, m_Replay, m_Stop;
	CList <CPoint> data;
	int select;
	bool play;
	int count, moveSelect;
	CPoint moveRect;
	POSITION movePos, temp;
// 생성입니다.
public:
	CChildView();

// 특성입니다.
public:

// 작업입니다.
public:
	void OnButton1Click();
	void OnButton2Click();
	void OnButton3Click();
	void OnButton4Click();
	void OnButton5Click();
// 재정의입니다.
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// 구현입니다.
public:
	virtual ~CChildView();

	// 생성된 메시지 맵 함수
protected:
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};

