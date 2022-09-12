
// ChildView.cpp : CChildView 클래스의 구현
//

#include "stdafx.h"
#include "MFCApplication2.h"
#include "ChildView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CChildView

CChildView::CChildView()
{
	m_pDC = NULL;
	m_pOldPalette = NULL;
}

CChildView::~CChildView()
{
}


BEGIN_MESSAGE_MAP(CChildView, CWnd)
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_TIMER()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_SIZE()
	ON_WM_DESTROY()
END_MESSAGE_MAP()



// CChildView 메시지 처리기

BOOL CChildView::PreCreateWindow(CREATESTRUCT& cs)
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS,
		::LoadCursor(NULL, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1), NULL);

	return TRUE;
}

void CChildView::OnPaint()
{
	CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

					   // TODO: 여기에 메시지 처리기 코드를 추가합니다.

					   // 그리기 메시지에 대해서는 CWnd::OnPaint()를 호출하지 마십시오.
}



int CChildView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	for (int i = 0;i < 10;i++)
		for (int j = 0;j < 10;j++)
			for (int k = 0;k < 10;k++)
				block[i][j][k] = rand() % 10;
	// TODO:  여기에 특수화된 작성 코드를 추가합니다.
	int         n;
	HGLRC       hrc;
	GLfloat     fMaxObjSize, fAspect;
	GLfloat     fNearPlane, fFarPlane;

	m_pDC = new CClientDC(this);

	ASSERT(m_pDC != NULL);
	static PIXELFORMATDESCRIPTOR pfd =
	{
		sizeof(PIXELFORMATDESCRIPTOR),  // size of this pfd
		1,                              // version wyrber
		PFD_DRAW_TO_WINDOW |            // support window
		PFD_SUPPORT_OPENGL |          // support OpenGL
		PFD_DOUBLEBUFFER,             // double buffered
		PFD_TYPE_RGBA,                  // RGBA type
		24,                             // 24-bit color depth
		0, 0, 0, 0, 0, 0,               // color bits ignored
		0,                              // no alpha buffer
		0,                              // shift bit ignored
		0,                              // no accumulation buffer
		0, 0, 0, 0,                     // accum bits ignored
		32,                             // 32-bit z-buffer
		0,                              // no stencil buffer
		0,                              // no auxiliary buffer
		PFD_MAIN_PLANE,                 // main layer
		0,                              // reserved
		0, 0, 0                         // layer masks ignored
	};
	int pixelformat;

	pixelformat = ChoosePixelFormat(m_pDC->GetSafeHdc(), &pfd);

	SetPixelFormat(m_pDC->GetSafeHdc(), pixelformat, &pfd);

	n = ::GetPixelFormat(m_pDC->GetSafeHdc());
	::DescribePixelFormat(m_pDC->GetSafeHdc(), n, sizeof(pfd), &pfd);

	hrc = wglCreateContext(m_pDC->GetSafeHdc());
	wglMakeCurrent(m_pDC->GetSafeHdc(), hrc);

	GetClientRect(&m_oldRect);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);

	if (m_oldRect.bottom)
		fAspect = (GLfloat)m_oldRect.right / m_oldRect.bottom;
	else    // don't divide by zero, not that we should ever run into that...
		fAspect = 1.0f;
	fNearPlane = 3.0f;
	fFarPlane = 7.0f;
	fMaxObjSize = 3.0f;
	m_fRadius = fNearPlane + fMaxObjSize / 2.0f;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, fAspect, fNearPlane, fFarPlane);
	glMatrixMode(GL_MODELVIEW);
	SetTimer(1, 100, 0);
	return 0;
}


void CChildView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	loop();
	CWnd::OnTimer(nIDEvent);
}
void CChildView::calculateposition() {
	if (ox != -1) {
		if (ck == true) {
			fphi -= (float)(ox - nx);
			ftheta -= (float)(oy - ny);
			while (ftheta > 90) { ftheta -= 90;fphi += 180; }
			while(ftheta<-90) { ftheta += 90;fphi += 180; }
		}
		else {
			obx = bx;
			oby = by;
			bx += 0.01*(-cos(fphi/ 57.295779513082320876798154814105)*(ox - nx)+sin(fphi/57.295779513082320876798154814105)*(oy-ny));
			by += 0.01*(-sin(fphi/ 57.295779513082320876798154814105)*(ox - nx)- cos(fphi/ 57.295779513082320876798154814105)*(oy - ny));
			if (bx > 1 - bwidtho2) bx = 1 - bwidtho2;
			else if (bx < bwidtho2 - 1) bx = bwidtho2 - 1;
			if (by > 1 - bheighto2) by = 1 - bheighto2;
			else if (by < bheighto2 - 1) by = bheighto2 - 1;
			bdx = bx - obx;
			bdy = by - oby;
		}
	}
	ox = nx;
	oy = ny;
}
#define gd 10.f
void CChildView::drawcircle(float x, float y, float z,float r) {
		glBegin(GL_POLYGON);
		for (float b = 0.f;b < 2.01f*PI;b += PI / gd) 
			glVertex3f(r*cos(b) + x, z, r*sin(b) + y);
		glEnd();
	
}

inline float lt(float a, float b) {
	return((a > b) ? (a - b) : (b - a));
}
HFONT fnt; HDC strgrphdc;
bool bf = false; int fsize = 20;
void buildfont() {
	strgrphdc = wglGetCurrentDC();
	fnt = CreateFontA(fsize, 0, 0, 0, FW_EXTRABOLD,0, FALSE, FALSE, ANSI_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS,
		ANTIALIASED_QUALITY, FF_DONTCARE | DEFAULT_PITCH, "Arial");
	SelectObject(strgrphdc, fnt);
}

void pc(float x, float y, float z, char *dest) {
	if (bf == false) {
		bf = true; buildfont();
	}
	
	strcat(dest, "ㄱ");
	wchar_t *wchar;
	wchar = new wchar_t[strlen(dest)];
	int wcharlength = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, dest, -1, wchar, strlen(dest));
	wchar[wcharlength - 2] = 0;
	dest[strlen(dest) - 2] = 0;
	glRasterPos3f(x, y, z);
	float t = 0; int ListNum = glGenLists(1);
	for (int i = 0; i < wcharlength; i++) {

		wglUseFontBitmapsW(strgrphdc, wchar[i], 1, ListNum);
		glCallList(ListNum);

	}
	glDeleteLists(ListNum, 1);
	delete[]wchar;
}
void CChildView::moveball() {
	if (ballz < -5) {
		ballz = -1.5;bally = 0;ballx = 0;balldz = -0.001;balldy = 0;balldx = 0;
	}
	int f, g, h;
	for (int i = 0;i < 100;i++) {
		ballx += balldx;
		bally += balldy;
		ballz += balldz;
		if (lt(ballx, 0.92) < 0.01 && balldx > 0) balldx *= -1;
		else if (lt(ballx, -0.92) < 0.01 && balldx < 0) balldx *= -1;
		else if (lt(bally, 0.92) < 0.01 && balldy > 0) balldy *= -1;
		else if (lt(bally, -0.92) < 0.01 && balldy < 0) balldy *= -1;
		else if (lt(ballz, 1.62) < 0.01 && balldz > 0) balldz *= -1;
		else if (lt(ballz, -1.92) < 0.25 && balldz < 0 && bx - bwidtho2 < ballx && ballx < bx + bwidtho2&& by - bheighto2 < bally && bally < by + bheighto2) { balldz *= -1; balldx = bdx/100;balldy = bdy/100; }
		f = (int)round(ballx * 5 + 4.5);
		g = (int)round(bally * 5 + 4.5);
		h = (int)round(ballz * 5 + 1);
		if (f == -1) f = 0;
		if (g == -1)g = 0;
		if (h == -1) h = 0;
		if (h>=0 && h<10 && block[f][g][h] != 0) {
			if (balldz > 0 && lt((ballz+0.08) * 5 + 1.5, round((ballz + 0.08) * 5 + 1.5)) < 0.01) {
				balldz *= -1; score++;
				if (block[f][g][h] > 5) block[f][g][h] -= 5;
				else if (block[f][g][h] != 0)block[f][g][h] = 0;
			}
			if (balldy > 0 && lt((bally+0.08) * 5 + 5, round((bally + 0.08) * 5 + 5)) < 0.01) {
				balldy *= -1; score++;
				if (block[f][g][h] > 5) block[f][g][h] -= 5;
				else if (block[f][g][h] != 0)block[f][g][h] = 0;
			}
			if (balldx > 0 && lt((ballx + 0.08) * 5 + 5, round((ballx + 0.08) * 5 + 5)) < 0.01) {
				balldx *= -1; score++;
				if (block[f][g][h] > 5) block[f][g][h] -= 5;
				else if (block[f][g][h] != 0)block[f][g][h] = 0;
			}
			if (balldz < 0 && lt((ballz - 0.08) * 5 + 0.5, round((ballz - 0.08) * 5 + 0.5)) < 0.01) {
				balldz *= -1; score++;
				if (block[f][g][h] > 5) block[f][g][h] -= 5;
				else if (block[f][g][h] != 0)block[f][g][h] = 0;
			}
			if (balldy < 0 && lt((bally - 0.08) * 5 + 4, round((bally - 0.08) * 5 + 4)) < 0.01) {
				balldy *= -1; score++;
				if (block[f][g][h] > 5) block[f][g][h] -= 5;
				else if (block[f][g][h] != 0)block[f][g][h] = 0;
			}
			if (balldx < 0 && lt((ballx - 0.08) * 5 + 4, round((ballx - 0.08) * 5 + 4)) < 0.01) {
				balldx *= -1; score++;
				if (block[f][g][h] > 5) block[f][g][h] -= 5;
				else if (block[f][g][h] != 0)block[f][g][h] = 0;
			}
		}
	}
	
}
void CChildView::drawsphere(float x, float y, float z, float r) {
	for (float a = 0.f;a < PI;a += PI / gd) {
		glBegin(GL_TRIANGLE_STRIP);
		for (float b = 0.f;b < 2.01f*PI;b += PI / gd) {
			glVertex3f(r*cos(b)*sin(a) + x,r*sin(b)*sin(a)+z,r*cos(a)+y);
			glVertex3f(r*cos(b)*sin(a+PI/gd) + x, r*sin(b)*sin(a + PI / gd) + z, r*cos(a+PI/gd) + y);
		}
		glEnd();
	}
}
char scr[100];
void CChildView::drawframe() {
	sprintf(scr, "%lld", score);
	
	moveball();
	
	glColor4f(1.f, 1.f, 0.f,1.f);
	drawsphere(ballx, bally, ballz, 0.08f);
	if(ballz>-1.80 || ballz<-2.20)glColor4f(0.f, 1.f, 1.f, 0.8f);
	else glColor4f(1.f, 1.f, 0.f, 0.8f);
	drawcircle(ballx, bally, -2.01f, 0.08f);
	
	glBegin(GL_LINE_STRIP);
	glVertex3f(-1.01, ballz, -1.01);
	glVertex3f(1.01, ballz, -1.01);
	glVertex3f(1.01, ballz, 1.01);
	glVertex3f(-1.01, ballz, 1.01);
	glEnd(); glColor4f(1.f, 1.f, 1.f, 1.f);
	pc(2., 2., 2., scr);
	for (float i = 1.f;i >0.f;i -= 0.2f) {
		glBegin(GL_LINE_STRIP);
		glVertex3f(-i, -2.f, i);
		glVertex3f(i, -2.f, i);
		glVertex3f(i, -2.f, -i);
		glVertex3f(-i, -2.f, -i);
		glVertex3f(-i, -2.f, i);
		glEnd();
	}
	glBegin(GL_LINES);
	glVertex3f(-1.f, -2.f, 0);
	glVertex3f(1.f, -2.f, 0);
	glEnd();
	glBegin(GL_LINES);
	glVertex3f(0, -2.f, -1.f);
	glVertex3f(0, -2.f, 1.f);
	glEnd();
	glBegin(GL_LINES);
	glVertex3f(-1.f, -2.f, -1.f);
	glVertex3f(-1.f, 1.7f, -1.f);
	glEnd();
	glBegin(GL_LINES);
	glVertex3f(1.f, -2.f, -1.f);
	glVertex3f(1.f, 1.7f, -1.f);
	glEnd();
	glBegin(GL_LINES);
	glVertex3f(-1.f, -2.f, 1.f);
	glVertex3f(-1.f, 1.7f, 1.f);
	glEnd();
	glBegin(GL_LINES);
	glVertex3f(1.f, -2.f, 1.f);
	glVertex3f(1.f, 1.7f, 1.f);
	glEnd();
	glBegin(GL_QUADS);
	glVertex3f((bx + bwidtho2) , -2.f, (by + bheighto2) );
	glVertex3f((bx - bwidtho2) , -2.f, (by + bheighto2) );
	glVertex3f((bx - bwidtho2) , -2.f, (by - bheighto2) );
	glVertex3f((bx + bwidtho2) , -2.f, (by - bheighto2) );
	glEnd();
	for (int i = 0;i<10;i++)
		for (int j = 0;j<10;j++)
			for (int k = 0;k<10;k++)
				if (block[i][j][k] != 0) {
					float br, bg, bb;
					switch (block[i][j][k]) {
					case 1:
						br = 1;bg = 1;bb = 1;
						break;
					case 2:
						br = 1;bg = 0;bb = 0;
						break;
					case 3:
						br = 1;bg = 0.5;bb = 0;
						break;
					case 4:
						br = 1;bg = 1;bb = 0;
						break;
					case 5:
						br = 0.5;bg = 1;bb = 0;
						break;
					case 6:
						br = 0;bg = 1;bb = 0;
						break;
					case 7:
						br = 0;bg = 1;bb = 1;
						break;
					case 8:
						br = 0;bg = 0;bb = 1;
						break;
					default:
						br = 1;bg = 0;bb = 1;
						break;
					}
					
					glColor4f(br, bg, bb, 1);
					glBegin(GL_QUAD_STRIP);
					glVertex3f((float)(i - 4.98)*0.2f, ((float)k - 1.48f)*0.2f, (float)(j - 4.98)*0.2f);
					glVertex3f((float)(i - 4.02)*0.2f, ((float)k - 1.48f)*0.2f, (float)(j - 4.98)*0.2f);
					glVertex3f((float)(i - 4.98)*0.2f, ((float)k - 1.48f)*0.2f, (float)(j - 4.02)*0.2f);
					glVertex3f((float)(i - 4.02)*0.2f, ((float)k - 1.48f)*0.2f, (float)(j - 4.02)*0.2f);
					glVertex3f((float)(i - 4.98)*0.2f, ((float)k - 0.52f)*0.2f, (float)(j - 4.02)*0.2f);
					glVertex3f((float)(i - 4.02)*0.2f, ((float)k - 0.52f)*0.2f, (float)(j - 4.02)*0.2f);
					glVertex3f((float)(i - 4.98)*0.2f, ((float)k - 0.52f)*0.2f, (float)(j - 4.98)*0.2f);
					glVertex3f((float)(i - 4.02)*0.2f, ((float)k - 0.52f)*0.2f, (float)(j - 4.98)*0.2f);
					glVertex3f((float)(i - 4.98)*0.2f, ((float)k - 1.48f)*0.2f, (float)(j - 4.98)*0.2f);
					glVertex3f((float)(i - 4.02)*0.2f, ((float)k - 1.48f)*0.2f, (float)(j - 4.98)*0.2f);
					glEnd();
					glBegin(GL_QUADS);
					glVertex3f((float)(i - 4.98)*0.2f, ((float)k - 1.48f)*0.2f, (float)(j - 4.98)*0.2f);
					glVertex3f((float)(i - 4.98)*0.2f, ((float)k - 1.48f)*0.2f, (float)(j - 4.02)*0.2f);
					glVertex3f((float)(i - 4.98)*0.2f, ((float)k - 0.52f)*0.2f, (float)(j - 4.02)*0.2f);
					glVertex3f((float)(i - 4.98)*0.2f, ((float)k - 0.52f)*0.2f, (float)(j - 4.98)*0.2f);
					glEnd();

				}
	
}
void CChildView::loop() {
	calculateposition();
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	glEnable(GL_BLEND);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glPushMatrix();
	
	glOrtho(-2.f, 2.f, -2.f, 2.f, -100, 100);
	glTranslatef(0.0f, 0.0f, -3);
	glRotatef(fphi, 0, 1, 0);
	glRotatef(sin(fphi/ 57.295779513082320876798154814105)*ftheta, 0, 0, 1);
	glRotatef(cos(fphi/ 57.295779513082320876798154814105)*ftheta,1, 0, 0);
	drawframe();
	
	glPopMatrix();
	glFinish();
	SwapBuffers(wglGetCurrentDC());
}

void CChildView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	nx = point.x;
	ny = point.y;
	CWnd::OnMouseMove(nFlags, point);
}


void CChildView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	ck = true;
	CWnd::OnLButtonDown(nFlags, point);
}


void CChildView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	ck = false;
	CWnd::OnLButtonUp(nFlags, point);
}


void CChildView::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);
	if (cy > 0)
	{
		glViewport(0, 0, cx, cy);

		if ((m_oldRect.right > cx) || (m_oldRect.bottom > cy))
			RedrawWindow();

		m_oldRect.right = cx;
		m_oldRect.bottom = cy;

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity(); glOrtho(-3.33, 3.33, -1.3, 1.3, -100, 100);		//gluPerspective(45.0f, (GLdouble)cx / cy, 3.0f, 7.0f);
		glMatrixMode(GL_MODELVIEW);
	}
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}


void CChildView::OnDestroy()
{
	CWnd::OnDestroy();

	HGLRC   hrc;

	KillTimer(1);

	hrc = ::wglGetCurrentContext();

	::wglMakeCurrent(NULL, NULL);

	if (hrc)
		::wglDeleteContext(hrc);

	if (m_pOldPalette)
		m_pDC->SelectPalette(m_pOldPalette, FALSE);

	if (m_pDC)
		delete m_pDC;
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}
