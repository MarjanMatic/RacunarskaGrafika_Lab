
// IND_19731View.h : interface of the CIND19731View class
//

#pragma once
#include "DImage.h"

class CIND19731View : public CView
{
protected: // create from serialization only
	CIND19731View() noexcept;
	DECLARE_DYNCREATE(CIND19731View)

// Attributes
public:
	CIND19731Doc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CIND19731View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	int m_backgroundSizePx;
	int m_cellCount;
	int m_cellSizePx;
	CDC bufferDC;

	DImage* m_images;
	DImage* m_denoisedImages;
	CBitmap* m_masks;
	int m_movingPieceIndex;
	bool m_firstPass;

	int m_posX;
	int m_posY;
	int m_moveSpd;

	void DrawGrid(CDC* pDC);
	void Translate(CDC* pDC, float dX, float dY, bool rightMultiply = false);
	void Scale(CDC* pDC, float sX, float sY, bool rightMultiply = false);
	void Rotate(CDC* pDC, float angle, bool rightMultiply = false);
	void Mirror(CDC* pDC, bool mx, bool my, bool rightMultiply = false);

	void DrawPuzzlePiece(CDC* pDC, DImage& img, float x, float y, float angle, float sx = 1, float sy = 1, bool mx = false, bool my = false, bool filter = false);
	void GrayFilter(DImage& img);
	void ColorChannel(DImage& img, bool r, bool g, bool b);

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};

#ifndef _DEBUG  // debug version in IND_19731View.cpp
inline CIND19731Doc* CIND19731View::GetDocument() const
   { return reinterpret_cast<CIND19731Doc*>(m_pDocument); }
#endif

