
// JLUProjectView.h: CJLUProjectView 类的接口
//

#pragma once


class CJLUProjectView : public CView
{
protected: // 仅从序列化创建
	CJLUProjectView() noexcept;
	DECLARE_DYNCREATE(CJLUProjectView)

// 特性
public:
	CJLUProjectDoc* GetDocument() const;

// 操作
public:

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 实现
public:
	virtual ~CJLUProjectView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDrawSetcolor();
protected:
	int _ColorR = 0;
	int _ColorG = 0;
	int _ColorB = 0;

	int DrawType = 0; //0代表未画图，大于0的值表示处于画图状态中（按了一个图形键）
	int IsStartedToDraw = 0; //1代表按下左键，拖拽鼠标以确定终点中；0代表未处于上述状态
	void DrawThing(CDC* pdc);
	CPoint _StartPoint; //开始点
	CPoint _EndPoint; //终点
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnButtonEllipse();
	afx_msg void OnButtonCircle();
	afx_msg void OnButtonLine();
	void DDALine(CDC* pdc, CPoint startPoint, CPoint endPoint, COLORREF color); //pdc, 线段始点, 线段终点, 颜色
	void BresenhamCircle(CDC* pdc, CPoint center, int radius, COLORREF color); //pdc, 圆心, 半径, 颜色
	void MidpointEllipse(CDC* pdc, CPoint center, long long semiMajorAxis, long long semiShortAxis, COLORREF color); // pdc, 圆心, 半长轴, 半短轴, 颜色
	afx_msg void OnButtonDefault();
};

#ifndef _DEBUG  // JLUProjectView.cpp 中的调试版本
inline CJLUProjectDoc* CJLUProjectView::GetDocument() const
   { return reinterpret_cast<CJLUProjectDoc*>(m_pDocument); }
#endif

