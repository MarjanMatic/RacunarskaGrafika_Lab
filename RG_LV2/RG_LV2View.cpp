
// RG_LV2View.cpp : implementation of the CRGLV2View class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "RG_LV2.h"
#endif

#include "RG_LV2Doc.h"
#include "RG_LV2View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CRGLV2View

IMPLEMENT_DYNCREATE(CRGLV2View, CView)

BEGIN_MESSAGE_MAP(CRGLV2View, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CRGLV2View::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

// CRGLV2View construction/destruction

CRGLV2View::CRGLV2View() noexcept
{
	// TODO: add construction code here
	m_sizePx = 500;
	m_cellCount = 20;
	m_cellSizePx = m_sizePx / m_cellCount;
	m_background = CRect(0, 0, m_sizePx, m_sizePx);

	m_jarBottom = new CPoint[4]{
		{ int(m_cellSizePx * 8.5), m_cellSizePx * 20 },
		{ m_cellSizePx * 8, m_cellSizePx * 18 },
		{ m_cellSizePx * 12, m_cellSizePx * 18 },
		{ int(m_cellSizePx * 11.5), m_cellSizePx * 20 }
	};

	m_jarTop = CRect(
		m_cellSizePx * 7.5, m_cellSizePx * 17,
		m_cellSizePx * 12.5, m_cellSizePx * 18
	);

	rootAngle = 0;
	leafAngle = 0;

	m_grid = false;
}

CRGLV2View::~CRGLV2View()
{
	delete[] m_jarBottom;
}

BOOL CRGLV2View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CRGLV2View drawing
void CRGLV2View::Translate(CDC* pDC, float dX, float dY, bool rightMultiply)
{
	XFORM Xform{
		1, 0,
		0, 1,
		dX, dY
	};

	DWORD iMode = rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY;
	pDC->ModifyWorldTransform(&Xform, iMode);
}

void CRGLV2View::Scale(CDC* pDC, float sX, float sY, bool rightMultiply)
{
	XFORM Xform{
		sX, 0,
		0, sY,
		0, 0
	};

	DWORD iMode = rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY;
	pDC->ModifyWorldTransform(&Xform, iMode);
}

void CRGLV2View::Rotate(CDC* pDC, float angle, bool rightMultiply)
{
	float radianAngle = angle * 3.14 / 180;
	XFORM Xform{
		cos(radianAngle), sin(radianAngle),
		-sin(radianAngle), cos(radianAngle),
		0, 0
	};

	DWORD iMode = rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY;
	pDC->ModifyWorldTransform(&Xform, iMode);
}

void CRGLV2View::DrawFigure(CDC* pDC)
{
	int prevMode = pDC->SetGraphicsMode(GM_ADVANCED);
	XFORM oldXform;
	pDC->GetWorldTransform(&oldXform);

	CBrush green(RGB(0, 204, 0));
	CBrush* pOldBrush = (CBrush*)pDC->SelectObject(&green);
	CPen blackPen(PS_SOLID, 0, RGB(0, 0, 0));
	CPen* pOldPen = (CPen*)pDC->SelectObject(&blackPen);

	HENHMETAFILE cactusLight = GetEnhMetaFile(_T("cactus_part_light.emf"));
	HENHMETAFILE cactus = GetEnhMetaFile(_T("cactus_part.emf"));

	Translate(pDC, m_cellSizePx * 10, m_cellSizePx * 17);
	Rotate(pDC, 180 + rootAngle); // With root angle

	pDC->PlayMetaFile(cactusLight, CRect(-32, 4, 32, 70));

	pDC->Ellipse(-14, -14, 14, 14);

	Translate(pDC, 0, m_cellSizePx * 3);
	XFORM XformCenter;
	pDC->GetWorldTransform(&XformCenter);
	
	// Left side
	Rotate(pDC, -45);
	Scale(pDC, 0.4, 1);
	pDC->PlayMetaFile(cactus, CRect(-32, 4, 32, 70));

	Scale(pDC, 1 / 0.4, 1);
	Translate(pDC, 0, m_cellSizePx * 3);
	Rotate(pDC, -45);
	Scale(pDC, 0.6, 1);
	pDC->PlayMetaFile(cactus, CRect(-32, 4, 32, 70));
	Scale(pDC, 1 / 0.6, 1);
	Rotate(pDC, 90);
	Scale(pDC, 0.6, 1);
	pDC->PlayMetaFile(cactus, CRect(-32, 4, 32, 70));
	Scale(pDC, 1 / 0.6, 1);
	pDC->Ellipse(-14, -14, 14, 14);
	Translate(pDC, 0, m_cellSizePx * 3);
	pDC->PlayMetaFile(cactus, CRect(-32, 4, 32, 70));
	pDC->Ellipse(-14, -14, 14, 14);

	// Center side
	pDC->SetWorldTransform(&XformCenter);
	Scale(pDC, 0.4, 1);
	pDC->PlayMetaFile(cactus, CRect(-32, 4, 32, 70));

	// Right side
	pDC->SetWorldTransform(&XformCenter);
	Rotate(pDC, 45);
	Scale(pDC, 0.4, 1);
	pDC->PlayMetaFile(cactus, CRect(-32, 4, 32, 70));
	Scale(pDC, 1 / 0.4, 1);
	pDC->Ellipse(-14, -14, 14, 14);
	Translate(pDC, 0, m_cellSizePx * 3);
	Rotate(pDC, -45 + leafAngle); // With leaf angle
	Scale(pDC, 0.6, 1);
	pDC->PlayMetaFile(cactusLight, CRect(-32, 4, 32, 70));
	Scale(pDC, 1 / 0.6, 1);
	Rotate(pDC, 90 - leafAngle);
	Scale(pDC, 0.6, 1);
	pDC->PlayMetaFile(cactus, CRect(-32, 4, 32, 70));
	Scale(pDC, 1 / 0.6, 1);
	pDC->Ellipse(-14, -14, 14, 14);
	Translate(pDC, 0, m_cellSizePx * 3);
	Rotate(pDC, -45);
	Scale(pDC, 0.6, 1);
	pDC->PlayMetaFile(cactus, CRect(-32, 4, 32, 70));
	Scale(pDC, 1 / 0.6, 1);
	Rotate(pDC, 90);
	Scale(pDC, 0.6, 1);
	pDC->PlayMetaFile(cactus, CRect(-32, 4, 32, 70));
	Scale(pDC, 1 / 0.6, 1);
	pDC->Ellipse(-14, -14, 14, 14);

	pDC->SelectObject(pOldPen);
	pDC->SelectObject(pOldBrush);
	pDC->SetWorldTransform(&oldXform);
	pDC->SetGraphicsMode(prevMode);
}

void CRGLV2View::OnDraw(CDC* pDC)
{
	CRGLV2Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
	CPen greyPen(PS_SOLID, 0, RGB(135, 206, 235));
	CPen* pOldPen = (CPen*)pDC->SelectObject(&greyPen);
	CBrush grey(RGB(135, 206, 235));
	CBrush* pOldBrush = (CBrush*)pDC->SelectObject(&grey);
	pDC->Rectangle(m_background);

	DrawFigure(pDC);

	CPen blackPen(PS_SOLID, 0, RGB(0, 0, 0));
	pDC->SelectObject(&blackPen);
	CBrush brownBrush(RGB(222, 148, 0));
	pDC->SelectObject(&brownBrush);
	pDC->Polygon(m_jarBottom, 4);
	pDC->Rectangle(m_jarTop);

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
	pDC->SelectObject(pOldPen);
	pDC->SelectObject(pOldBrush);
}


// CRGLV2View printing


void CRGLV2View::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CRGLV2View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CRGLV2View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CRGLV2View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CRGLV2View::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CRGLV2View::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CRGLV2View diagnostics

#ifdef _DEBUG
void CRGLV2View::AssertValid() const
{
	CView::AssertValid();
}

void CRGLV2View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CRGLV2Doc* CRGLV2View::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CRGLV2Doc)));
	return (CRGLV2Doc*)m_pDocument;
}
#endif //_DEBUG


// CRGLV2View message handlers

void CRGLV2View::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default
	if (nChar == 'G') {
		m_grid = !m_grid;
		Invalidate();
	}
	else if (nChar == 'A') {
		leafAngle -= 5;
		Invalidate();
	}
	else if (nChar == 'D') {
		leafAngle += 5;
		Invalidate();
	}
	else if (nChar == VK_LEFT) {
		rootAngle -= 5;
		Invalidate();
	}
	else if (nChar == VK_RIGHT) {
		rootAngle += 5;
		Invalidate();
	}

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}
