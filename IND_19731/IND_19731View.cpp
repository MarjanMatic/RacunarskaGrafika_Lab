
// IND_19731View.cpp : implementation of the CIND19731View class
//

#include "pch.h"
#include "framework.h"
#include <string>
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "IND_19731.h"
#endif

#include "IND_19731Doc.h"
#include "IND_19731View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CIND19731View

IMPLEMENT_DYNCREATE(CIND19731View, CView)

BEGIN_MESSAGE_MAP(CIND19731View, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

// CIND19731View construction/destruction

CIND19731View::CIND19731View() noexcept
{
	// TODO: add construction code here
	m_backgroundSizePx = 500;
	m_cellCount = 20;
	m_cellSizePx = m_backgroundSizePx / m_cellCount;

	m_posX = m_cellSizePx * 17;
	m_posY = m_cellSizePx * 17;
	m_moveSpd = 10;

	m_movingPieceIndex = 5;
	m_images = new DImage[9];
	for (size_t i = 0; i < 9; i++)
	{
		std::string s = "images/" + std::to_string(i + 1) + ".bmp";
		m_images[i].Load(std::wstring(s.begin(), s.end()).c_str());
		GrayFilter(m_images[i]);
		if (i == m_movingPieceIndex)
			ColorChannel(m_images[i], false, false, true);
	}
}

CIND19731View::~CIND19731View()
{
}

BOOL CIND19731View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CIND19731View drawing
void CIND19731View::DrawGrid(CDC* pDC)
{
	CPen penWhite(PS_SOLID, 0, RGB(205, 205, 205));
	CPen* oldPen = (CPen*)pDC->SelectObject(&penWhite);
	for (size_t i = 0; i <= m_cellCount; i++)
	{
		pDC->MoveTo(i * m_cellSizePx - 1, 0);
		pDC->LineTo(i * m_cellSizePx - 1, 500);

		pDC->MoveTo(0, i * m_cellSizePx - 1);
		pDC->LineTo(500, i * m_cellSizePx - 1);
	}

	pDC->SelectObject(oldPen);
}

void CIND19731View::Translate(CDC* pDC, float dX, float dY, bool rightMultiply)
{
	XFORM matrix{
		1, 0,
		0, 1,
		dX, dY
	};
	DWORD iMode = rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY;
	pDC->ModifyWorldTransform(&matrix, iMode);
}

void CIND19731View::Scale(CDC* pDC, float sX, float sY, bool rightMultiply)
{
	XFORM matrix{
		sX, 0,
		0, sY,
		0, 0
	};
	DWORD iMode = rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY;
	pDC->ModifyWorldTransform(&matrix, iMode);
}

void CIND19731View::Rotate(CDC* pDC, float angle, bool rightMultiply)
{
	float radians = angle * 3.14 / 180;
	XFORM matrix{
		cos(radians), sin(radians),
		-sin(radians), cos(radians),
		0, 0
	};
	DWORD iMode = rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY;
	pDC->ModifyWorldTransform(&matrix, iMode);
}

void CIND19731View::Mirror(CDC* pDC, bool mx, bool my, bool rightMultiply)
{
	Scale(pDC, mx ? -1 : 1, my ? -1 : 1, rightMultiply);
}

void CIND19731View::DrawPuzzlePiece(CDC* pDC, DImage& img, float x, float y, float angle, float sx, float sy, bool mx, bool my)
{
	int width = img.Width();
	int height = img.Height();
	CBitmap bitmap;
	bitmap.CreateBitmap(width, height, 1, 32, NULL);
	CBitmap bmpMask;
	bmpMask.CreateBitmap(width, height, 1, 1, NULL);

	CDC* initialDC = new CDC();
	initialDC->CreateCompatibleDC(pDC);
	CBitmap* prevBitmap = initialDC->SelectObject(img.GetBitmap());

	CDC* srcDC = new CDC();
	srcDC->CreateCompatibleDC(pDC);
	CDC* dstDC = new CDC();
	dstDC->CreateCompatibleDC(pDC);

	srcDC->SelectObject(&bitmap);
	srcDC->BitBlt(0, 0, width, height, initialDC, 0, 0, SRCCOPY);
	dstDC->SelectObject(&bmpMask);

	COLORREF clrTopLeft = srcDC->GetPixel(0, 0);
	srcDC->SetBkColor(clrTopLeft);
	dstDC->BitBlt(0, 0, width, height, srcDC, 0, 0, SRCCOPY);

	srcDC->SetTextColor(RGB(255, 255, 255));
	srcDC->SetBkColor(RGB(0, 0, 0));
	srcDC->BitBlt(0, 0, width, height, dstDC, 0, 0, SRCAND);

	XFORM oldXform;
	pDC->GetWorldTransform(&oldXform);

	Translate(pDC, x, y);
	Rotate(pDC, angle);
	Scale(pDC, sx, sy);
	Mirror(pDC, mx, my);

	pDC->BitBlt(-width / 2, -height / 2, width, height, dstDC, 0, 0, SRCAND);
	pDC->BitBlt(-width / 2, -height / 2, width, height, srcDC, 0, 0, SRCPAINT);

	pDC->SetWorldTransform(&oldXform);
	initialDC->SelectObject(prevBitmap);
	delete srcDC;
	delete dstDC;
	delete initialDC;
}

void CIND19731View::GrayFilter(DImage& img)
{
	unsigned char* imageBytes = img.GetDIBBits();
	int bytes = img.BPP();
	for (size_t i = 0; i < img.Width() * img.Height(); i++)
	{
		int colorIndex = i * bytes;
		unsigned char& b = imageBytes[colorIndex];
		unsigned char& g = imageBytes[colorIndex + 1];
		unsigned char& r = imageBytes[colorIndex + 2];
		int gr = 64 + (r + g + b) / 3;
		if (gr > 255)
			gr = 255;
		b = g = r = gr;
	}
	img.Update();
}

void CIND19731View::ColorChannel(DImage& img, bool r, bool g, bool b)
{
	unsigned char* imageBytes = img.GetDIBBits();
	int bytes = img.BPP();
	for (size_t i = 0; i < img.Width() * img.Height(); i++)
	{
		int colorIndex = i * bytes;
		imageBytes[colorIndex] =     b ? imageBytes[colorIndex]     : 0;
		imageBytes[colorIndex + 1] = g ? imageBytes[colorIndex + 1] : 0;
		imageBytes[colorIndex + 2] = r ? imageBytes[colorIndex + 2] : 0;
	}
	img.Update();
}

void CIND19731View::OnDraw(CDC* pDC)
{
	CIND19731Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
	if (bufferDC.m_hDC != nullptr)
	{
		pDC->BitBlt(0, 0, m_backgroundSizePx, m_backgroundSizePx, &bufferDC, 0, 0, SRCCOPY);
		bufferDC.DeleteDC();
	}
	bufferDC.CreateCompatibleDC(pDC);

	int prevMode = bufferDC.SetGraphicsMode(GM_ADVANCED);
	CBitmap combined;
	combined.CreateBitmap(m_backgroundSizePx, m_backgroundSizePx, 1, 32, NULL);
	bufferDC.SelectObject(&combined);

	CBrush whiteBrush(RGB(255, 255, 255));
	CBrush* oldBrush = (CBrush*)bufferDC.SelectObject(&whiteBrush);
	bufferDC.Rectangle(0, 0, m_backgroundSizePx, m_backgroundSizePx);
	DrawGrid(&bufferDC);

	XFORM oldXform;
	bufferDC.GetWorldTransform(&oldXform);
	Mirror(&bufferDC, true, false);
	Rotate(&bufferDC, 90);

	DrawPuzzlePiece(&bufferDC, m_images[0], 5 + m_cellSizePx * 4, 5 + m_cellSizePx * 4, 115);
	DrawPuzzlePiece(&bufferDC, m_images[1], 5 + m_cellSizePx * 10,    m_cellSizePx * 3.7,    17);
	DrawPuzzlePiece(&bufferDC, m_images[2], 5 +  m_cellSizePx * 15.6,  m_cellSizePx * 3.75,  -71);
	DrawPuzzlePiece(&bufferDC, m_images[3],    m_cellSizePx * 4,     m_cellSizePx * 10.3,   145);
	DrawPuzzlePiece(&bufferDC, m_images[4],  m_cellSizePx * 9.9,   m_cellSizePx * 9.65,  -32);
	//DrawPuzzlePiece(&bufferDC, m_images[5],   m_cellSizePx * 16.3,  m_cellSizePx * 9.7,    23);
	DrawPuzzlePiece(&bufferDC, m_images[6],    m_cellSizePx * 3.7,   m_cellSizePx * 16.25, -166);
	DrawPuzzlePiece(&bufferDC, m_images[7],  m_cellSizePx * 9.95,  m_cellSizePx * 15.6,  -17.9);
	DrawPuzzlePiece(&bufferDC, m_images[8],   m_cellSizePx * 16.35, m_cellSizePx * 16,     71);

	bufferDC.SetWorldTransform(&oldXform);
	DrawPuzzlePiece(&bufferDC, m_images[m_movingPieceIndex],   m_posX,  m_posY,    -113, 1, 1, true);

	bufferDC.SetGraphicsMode(prevMode);
	bufferDC.SelectObject(oldBrush);

	pDC->BitBlt(0, 0, m_backgroundSizePx, m_backgroundSizePx, &bufferDC, 0, 0, SRCCOPY);
}


// CIND19731View printing

BOOL CIND19731View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CIND19731View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CIND19731View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CIND19731View diagnostics

#ifdef _DEBUG
void CIND19731View::AssertValid() const
{
	CView::AssertValid();
}

void CIND19731View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CIND19731Doc* CIND19731View::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CIND19731Doc)));
	return (CIND19731Doc*)m_pDocument;
}
#endif //_DEBUG


// CIND19731View message handlers

void CIND19731View::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default
	if (nChar == VK_UP)
	{
		m_posY -= m_moveSpd;
		Invalidate();
	}
	else if (nChar == VK_DOWN)
	{
		m_posY += m_moveSpd;
		Invalidate();
	}
	else if (nChar == VK_LEFT)
	{
		m_posX -= m_moveSpd;
		Invalidate();
	}
	else if (nChar == VK_RIGHT)
	{
		m_posX += m_moveSpd;
		Invalidate();
	}

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}
