
// ChildView.h : CChildView Ŭ������ �������̽�
//


#pragma once


// CChildView â

class CChildView : public CWnd
{
// �����Դϴ�.
public:
	CChildView();

// Ư���Դϴ�.
public:
	float ballx=0.f, bally=0.f, ballz=-1.5f;
	float balldx=0, balldy=0, balldz=-0.001;
	float bdx, bdy;
	int block[10][10][10];
	GLfloat fr, fp, fy, fphi = 30.f,ftheta=20.f;
	float bx, by,obx=0,oby=0;
	float bwidtho2 = .25;
	float bheighto2 = .25;
	int ox=-1, oy,nx,ny;
	long long score=0;
	bool ck=false;
	CPalette    m_cPalette;
	CPalette    *m_pOldPalette;
	CRect       m_oldRect;
	float       m_fRadius;
	CClientDC   *m_pDC;
// �۾��Դϴ�.
public:

// �������Դϴ�.
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// �����Դϴ�.
public:
	virtual ~CChildView();

	// ������ �޽��� �� �Լ�
protected:
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	void calculateposition();
	void loop();
	void drawframe();
	void drawsphere(float x, float y, float z, float r);
	void drawcircle(float x, float y, float z, float r);
	void moveball();
};

