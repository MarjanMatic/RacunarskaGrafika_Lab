
// RG_LV1View.h : interface of the CRGLV1View class
//

#pragma once


class CRGLV1View : public CView
{
protected: // create from serialization only
	CRGLV1View() noexcept;
	DECLARE_DYNCREATE(CRGLV1View)

// Attributes
public:
	CRGLV1Doc* GetDocument() const;

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
	virtual ~CRGLV1View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	int m_sizePx;
	int m_cellCount;
	int m_cellSizePx;
	CRect m_background;
	CPoint* m_redTriangle;
	CPoint* m_whiteTriangle;
	CPoint* m_purpleRectangle;
	CPoint* m_orangeTriangle;
	CPoint* m_yellowTriangle;
	CPoint* m_pinkTriangle;
	CPoint* m_greenRectangle;
	bool m_grid;

	float DegreeToRadian(float degree);
	void DrawRegularPolygon(CDC* pDC, int cx, int cy, int r, int n, float rotAngle);
	void DrawPolygon(CDC* pDC, CPoint* points, int n);

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};

#ifndef _DEBUG  // debug version in RG_LV1View.cpp
inline CRGLV1Doc* CRGLV1View::GetDocument() const
   { return reinterpret_cast<CRGLV1Doc*>(m_pDocument); }
#endif

