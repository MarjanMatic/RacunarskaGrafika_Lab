
// RG_LV2View.h : interface of the CRGLV2View class
//

#pragma once


class CRGLV2View : public CView
{
protected: // create from serialization only
	CRGLV2View() noexcept;
	DECLARE_DYNCREATE(CRGLV2View)

// Attributes
public:
	CRGLV2Doc* GetDocument() const;

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
	virtual ~CRGLV2View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	int m_sizePx;
	int m_cellCount;
	int m_cellSizePx;
	CRect m_background;
	CPoint* m_jarBottom;
	CRect m_jarTop;
	int rootAngle;
	int leafAngle;
	bool m_grid;

	void Translate(CDC* pDC, float dX, float dY, bool rightMultiply = false);
	void Scale(CDC* pDC, float sX, float sY, bool rightMultiply = false);
	void Rotate(CDC* pDC, float angle, bool rightMultiply = false);
	void DrawFigure(CDC* pDC);

// Generated message map functions
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};

#ifndef _DEBUG  // debug version in RG_LV2View.cpp
inline CRGLV2Doc* CRGLV2View::GetDocument() const
   { return reinterpret_cast<CRGLV2Doc*>(m_pDocument); }
#endif

