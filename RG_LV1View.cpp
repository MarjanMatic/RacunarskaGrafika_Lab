
// RG_LV1View.cpp : implementation of the CRGLV1View class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "RG_LV1.h"
#endif

#include "RG_LV1Doc.h"
#include "RG_LV1View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CRGLV1View

IMPLEMENT_DYNCREATE(CRGLV1View, CView)

BEGIN_MESSAGE_MAP(CRGLV1View, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_KEYDOWN()
//	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

// CRGLV1View construction/destruction

CRGLV1View::CRGLV1View() noexcept
{
	// TODO: add construction code here
	m_sizePx = 500;
	m_cellCount = 20;
	m_cellSizePx = m_sizePx / m_cellCount;
	m_background = CRect(0, 0, m_sizePx, m_sizePx);

	m_whiteTriangle = new CPoint[3] {
		{ m_cellSizePx * 1, m_cellSizePx * 6 },
		{ m_cellSizePx * 4, m_cellSizePx * 3 },
		{ m_cellSizePx * 7, m_cellSizePx * 6 }
	};

	m_purpleRectangle = new CPoint[4] {
		{ m_cellSizePx * 4,  m_cellSizePx * 6 },
		{ m_cellSizePx * 7,  m_cellSizePx * 6 },
		{ m_cellSizePx * 10, m_cellSizePx * 9 },
		{ m_cellSizePx * 7,  m_cellSizePx * 9 }
	};

	m_orangeTriangle = new CPoint[3] {
		{ m_cellSizePx * 7,  m_cellSizePx * 9  },
		{ m_cellSizePx * 15, m_cellSizePx * 9  },
		{ m_cellSizePx * 11, m_cellSizePx * 13 }
	};

	m_yellowTriangle = new CPoint[3] {
		{ m_cellSizePx * 13, m_cellSizePx * 11 },
		{ m_cellSizePx * 17, m_cellSizePx * 7  },
		{ m_cellSizePx * 17, m_cellSizePx * 15 }
	};

	m_redTriangle = new CPoint[3] {
		{ m_cellSizePx * 14, m_cellSizePx * 14 },
		{ m_cellSizePx * 14, m_cellSizePx * 17 },
		{ m_cellSizePx * 11, m_cellSizePx * 17 }
	};

	m_greenTriangle = new CPoint[3] {
		{ m_cellSizePx * 17, m_cellSizePx * 13 + 8 },
		{ m_cellSizePx * 19, m_cellSizePx * 15 + 8 },
		{ m_cellSizePx * 17, m_cellSizePx * 17 + 8 }
	};

	m_grid = false;
}

CRGLV1View::~CRGLV1View()
{
	delete[] m_whiteTriangle;
	delete[] m_purpleRectangle;
	delete[] m_orangeTriangle;
	delete[] m_yellowTriangle;
	delete[] m_redTriangle;
	delete[] m_greenTriangle;
}

BOOL CRGLV1View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CRGLV1View drawing
float CRGLV1View::DegreeToRadian(float degree) {
	return degree * 3.14159 / 180;
}

void CRGLV1View::DrawRegularPolygon(CDC* pDC, int cx, int cy, int r, int n, float rotAngle) {
	CPoint* points = new CPoint[n];
	float angle = 360.0 / n;
	float radianAngle = DegreeToRadian(angle);
	for (int i = 0; i < n; i++) {
		points[i].x = r * cos(i * radianAngle);
		points[i].y = r * sin(i * radianAngle);
	}

	int prevMode = pDC->SetGraphicsMode(GM_ADVANCED);

	float rotRadian = DegreeToRadian(rotAngle);
	XFORM m = {
		cos(rotRadian), sin(rotRadian),
		-sin(rotRadian), cos(rotRadian),
		cx, cy
	};

	XFORM oldXform;
	bool b = pDC->GetWorldTransform(&oldXform);
	pDC->ModifyWorldTransform(&m, MWT_RIGHTMULTIPLY);

	pDC->Polygon(points, n);

	b = pDC->SetWorldTransform(&oldXform);
	pDC->SetGraphicsMode(prevMode);

	delete[] points;
}

void CRGLV1View::DrawPolygon(CDC* pDC, CPoint* points, int n) {
	pDC->Polygon(points, n);
}

void CRGLV1View::OnDraw(CDC* pDC)
{
	CRGLV1Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
	CPen greyPen(PS_SOLID, 0, RGB(210, 210, 210));
	CPen* pOldPen = (CPen*)pDC->SelectObject(&greyPen);
	CBrush grey(RGB(210, 210, 210));
	CBrush* pOldBrush = (CBrush*)pDC->SelectObject(&grey);
	pDC->Rectangle(m_background);

	int prevGraphicsMode = pDC->SetGraphicsMode(GM_ADVANCED);
	XFORM oldXform;
	bool b = pDC->GetWorldTransform(&oldXform);

	XFORM xform = {
		1, 0,
		0, 1,
		-2, -5
	};
	b = pDC->SetWorldTransform(&xform);


	// White triangle
	CPen yellowPen(PS_SOLID, 4, RGB(255, 255, 0));
	CBrush white(HS_FDIAGONAL, RGB(0, 0, 255));
	pDC->SelectObject(&yellowPen);
	pDC->SelectObject(&white);
	DrawPolygon(pDC, m_whiteTriangle, 3);

	// Purple rectangle
	CBrush purple(RGB(150, 0, 150));
	pDC->SelectObject(&purple);
	DrawPolygon(pDC, m_purpleRectangle, 4);

	// Orange triangle
	CBrush orange(RGB(255, 85, 51));
	pDC->SelectObject(&orange);
	DrawPolygon(pDC, m_orangeTriangle, 3);

	// Yellow triangle
	CBrush yellow(RGB(255, 255, 0));
	pDC->SelectObject(&yellow);
	DrawPolygon(pDC, m_yellowTriangle, 3);

	// Pink rectangle
	CBrush pink(RGB(255, 150, 240));
	pDC->SelectObject(&pink);
	DrawRegularPolygon(pDC, m_cellSizePx * 13, m_cellSizePx * 13, 50, 4, 0);

	// Red triangle
	CBrush red(RGB(255, 0, 0));
	pDC->SelectObject(&red);
	DrawPolygon(pDC, m_redTriangle, 3);

	// Green triangle
	CBrush green(RGB(0, 255, 0));
	pDC->SelectObject(&green);
	DrawPolygon(pDC, m_greenTriangle, 3);

	// Inner polygons
	CPen yellowPenThin(PS_SOLID, 3, RGB(255, 255, 0));
	pDC->SelectObject(&yellowPenThin);
	pDC->SelectStockObject(HOLLOW_BRUSH);
	DrawRegularPolygon(pDC, m_cellSizePx * 4,    m_cellSizePx * 4.8,  16, 10, 0);
	DrawRegularPolygon(pDC, m_cellSizePx * 11,   m_cellSizePx * 10.6, 20, 7, 0);
	DrawRegularPolygon(pDC, m_cellSizePx * 13.1, m_cellSizePx * 16.1, 12, 4, 0);
	DrawRegularPolygon(pDC, m_cellSizePx * 17.8, m_cellSizePx * 15 + 8, 12, 5, 0);

	b = pDC->SetWorldTransform(&oldXform);
	pDC->SetGraphicsMode(prevGraphicsMode);

	if (m_grid) {
		CPen penWhite(PS_SOLID, 0, RGB(245, 245, 245));
		pDC->SelectObject(&penWhite);

		for (int i = 1; i < m_cellCount; i++) {
			pDC->MoveTo(i * m_cellSizePx, 0);
			pDC->LineTo(i * m_cellSizePx, m_sizePx);

			pDC->MoveTo(0, i * m_cellSizePx);
			pDC->LineTo(m_sizePx, i * m_cellSizePx);
		}
	}

	pDC->SelectObject(pOldBrush);
	pDC->SelectObject(pOldPen);
}


// CRGLV1View printing

BOOL CRGLV1View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CRGLV1View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CRGLV1View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CRGLV1View diagnostics

#ifdef _DEBUG
void CRGLV1View::AssertValid() const
{
	CView::AssertValid();
}

void CRGLV1View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CRGLV1Doc* CRGLV1View::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CRGLV1Doc)));
	return (CRGLV1Doc*)m_pDocument;
}
#endif //_DEBUG


// CRGLV1View message handlers

void CRGLV1View::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default
	if (nChar == 'G') {
		m_grid = !m_grid;
		Invalidate();
	}
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}
