
// JLUProjectView.cpp: CJLUProjectView 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "JLUProject.h"
#endif

#include "JLUProjectDoc.h"
#include "JLUProjectView.h"
#include "SetColor.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CJLUProjectView

IMPLEMENT_DYNCREATE(CJLUProjectView, CView)

BEGIN_MESSAGE_MAP(CJLUProjectView, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_COMMAND(ID_DRAW_SETCOLOR, &CJLUProjectView::OnDrawSetcolor)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_COMMAND(ID_BUTTONELLIPSE, &CJLUProjectView::OnButtonEllipse)
	ON_COMMAND(ID_BUTTONCIRCLE, &CJLUProjectView::OnButtonCircle)
	ON_COMMAND(ID_BUTTONLINE, &CJLUProjectView::OnButtonLine)
	ON_COMMAND(ID_BUTTONDEFAULT, &CJLUProjectView::OnButtonDefault)
	ON_WM_LBUTTONDBLCLK()
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
END_MESSAGE_MAP()

// CJLUProjectView 构造/析构

CJLUProjectView::CJLUProjectView() noexcept
{
	// TODO: 在此处添加构造代码

	IsStartedToDraw = 0;
}

CJLUProjectView::~CJLUProjectView()
{
}

BOOL CJLUProjectView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CJLUProjectView 绘图

void CJLUProjectView::OnDraw(CDC* pDC)
{
	CJLUProjectDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	switch (DrawType) {
	case 1: DDALine(pDC, _StartPoint, _EndPoint, RGB(_ColorR, _ColorG, _ColorB));
		break;
	case 2: BresenhamCircle(pDC, CPoint(_StartPoint),
		(int)sqrt((_EndPoint.x - _StartPoint.x)*(_EndPoint.x - _StartPoint.x) + (_EndPoint.y - _StartPoint.y)*(_EndPoint.y - _StartPoint.y)),
		RGB(_ColorR, _ColorG, _ColorB));
		break;
		//int x1 = _startPoint.x;
		//int x2 = _endPoint.x;
		//int y1 = _startPoint.y;
		//int y2 = _endPoint.y;
		//int radius = (int)sqrt((x2 - x1)*(x2 - x1) + (y2 - y1)*(y2 - y1));
	case 3: MidpointEllipse(pDC, CPoint(_StartPoint.x + (_EndPoint.x - _StartPoint.x) / 2, _StartPoint.y + (_EndPoint.y - _StartPoint.y) / 2),
		(long long)fabs(_EndPoint.x - _StartPoint.x) / 2, (long long)fabs(_EndPoint.y - _StartPoint.y) / 2, RGB(_ColorR, _ColorG, _ColorB));
		break;
		//long long semiMajorAxis = (int)fabs(_endPoint.x - _startPoint.x) / 2; //半长轴
		//long long semiShortAxis = (int)fabs(_endPoint.y - _startPoint.y) / 2; //半短轴
		//int midX = _startPoint.x + (_endPoint.x - _startPoint.x) / 2; //椭圆圆心点坐标
		//int midY = _startPoint.y + (_endPoint.y - _startPoint.y) / 2;
	default: break;
	}
}


// CJLUProjectView 打印

BOOL CJLUProjectView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CJLUProjectView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CJLUProjectView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}


// CJLUProjectView 诊断

#ifdef _DEBUG
void CJLUProjectView::AssertValid() const
{
	CView::AssertValid();
}

void CJLUProjectView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CJLUProjectDoc* CJLUProjectView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CJLUProjectDoc)));
	return (CJLUProjectDoc*)m_pDocument;
}
#endif //_DEBUG


// CJLUProjectView 消息处理程序


void CJLUProjectView::OnDrawSetcolor()
{
	SetColor dia;
	dia.m_r = _ColorR;
	dia.m_g = _ColorG;
	dia.m_b = _ColorB;
	if (dia.DoModal() == IDOK) {
		_ColorR = dia.m_r;
		_ColorG = dia.m_g;
		_ColorB = dia.m_b;
	}
}


void CJLUProjectView::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (DrawType > 0) {
		IsStartedToDraw = 1;
		_StartPoint = point;
		_EndPoint = point;
	}	
	CView::OnLButtonDown(nFlags, point);
}


void CJLUProjectView::OnLButtonUp(UINT nFlags, CPoint point)
{
	if (DrawType > 0) {
		IsStartedToDraw = 0;
	}
	CView::OnLButtonUp(nFlags, point);
}


void CJLUProjectView::OnRButtonDown(UINT nFlags, CPoint point)
{
	if (IsStartedToDraw > 0) {
		CDC* pdc = this->GetDC();
		pdc->SetROP2(R2_NOTXORPEN);
		OnDraw(pdc); //覆盖掉最后一次画的图
		ReleaseDC(pdc);
	}
	IsStartedToDraw = 0;
	CView::OnRButtonDown(nFlags, point);
}


void CJLUProjectView::OnMouseMove(UINT nFlags, CPoint point)
{
	if (DrawType > 0 && IsStartedToDraw > 0) {
		CDC* pdc = this->GetDC();
		pdc->SetROP2(R2_NOTXORPEN);
		OnDraw(pdc); //覆盖掉上一次画的图
		_EndPoint = point;
		OnDraw(pdc); //画一个新图
		ReleaseDC(pdc);
	}
	CView::OnMouseMove(nFlags, point);
}


void CJLUProjectView::OnButtonEllipse()
{
	DrawType = 3;
}


void CJLUProjectView::OnButtonCircle()
{
	DrawType = 2;
}


void CJLUProjectView::OnButtonLine()
{
	DrawType = 1;
}

/*
void CJLUProjectView::DrawThing(CDC* pdc)
{
	switch (DrawType) {
	case 1: DDALine(pdc, _StartPoint, _EndPoint, RGB(_ColorR, _ColorG, _ColorB));
		break;
	case 2: BresenhamCircle(pdc, CPoint(_StartPoint), 
		(int)sqrt((_EndPoint.x - _StartPoint.x)*(_EndPoint.x - _StartPoint.x) + (_EndPoint.y - _StartPoint.y)*(_EndPoint.y - _StartPoint.y)),
		RGB(_ColorR, _ColorG, _ColorB));
		break;
//int x1 = _startPoint.x;
//int x2 = _endPoint.x;
//int y1 = _startPoint.y;
//int y2 = _endPoint.y;
//int radius = (int)sqrt((x2 - x1)*(x2 - x1) + (y2 - y1)*(y2 - y1));
	case 3: MidpointEllipse(pdc, CPoint(_StartPoint.x + (_EndPoint.x - _StartPoint.x) / 2, _StartPoint.y + (_EndPoint.y - _StartPoint.y) / 2),
		(long long)fabs(_EndPoint.x - _StartPoint.x) / 2, (long long)fabs(_EndPoint.y - _StartPoint.y) / 2, RGB(_ColorR, _ColorG, _ColorB));
		break;
//long long semiMajorAxis = (int)fabs(_endPoint.x - _startPoint.x) / 2; //半长轴
//long long semiShortAxis = (int)fabs(_endPoint.y - _startPoint.y) / 2; //半短轴
//int midX = _startPoint.x + (_endPoint.x - _startPoint.x) / 2; //椭圆圆心点坐标
//int midY = _startPoint.y + (_endPoint.y - _startPoint.y) / 2;
	default: break;
	}
}
*/

void CJLUProjectView::DDALine(CDC* pdc, CPoint startPoint, CPoint endPoint, COLORREF color)
{
	double dx = endPoint.x - startPoint.x;
	double dy = endPoint.y - startPoint.y;
	double e = (fabs(dx) > fabs(dy) ? fabs(dx) : fabs(dy));
	dx /= e;
	dy /= e;
	double x = startPoint.x;
	double y = startPoint.y;
	for (int i = 1; i <= e; ++i) {
		pdc->SetPixel((int)(x + 0.5), (int)(y + 0.5), color);
		x += dx;
		y += dy;
	}
}


void CJLUProjectView::BresenhamCircle(CDC* pdc, CPoint center, int radius, COLORREF color)
{
	int x1 = center.x;
	int y1 = center.y;
	int x = 0;
	int y = radius;
	int p = 3 - 2 * radius;
	pdc->SetPixel(-x + center.x, y + center.y, color);
	pdc->SetPixel(x + center.x, -y + center.y, color);
	pdc->SetPixel(-y + center.x, x + center.y, color);
	pdc->SetPixel(y + center.x, -x + center.y, color);
	//防止对称性造成的一些点(正上方、正下方、正左方、正右方）二次着色而产生视觉上的缺点现象，额外着色一次
	while (x <= y) {
		if (x != y) {
			pdc->SetPixel(x1 + x, y1 + y, color);
			pdc->SetPixel(x1 + y, y1 - x, color);
			pdc->SetPixel(x1 - y, y1 + x, color);
			pdc->SetPixel(x1 - y, y1 - x, color);
			//防止对称性造成的一些点(左上、右上、左下、右下）二次着色而产生视觉上的缺点现象，只着色一次
		}
		pdc->SetPixel(x1 + x, y1 - y, color);
		pdc->SetPixel(x1 - x, y1 + y, color);
		pdc->SetPixel(x1 - x, y1 - y, color);
		pdc->SetPixel(x1 + y, y1 + x, color);
		if (p >= 0) {
			p += 4 * (x - y) + 10;
			y--;
		}
		else {
			p += 4 * x + 6;
		}
		x++;
	}
}


void CJLUProjectView::MidpointEllipse(CDC* pdc, CPoint center, long long semiMajorAxis, long long semiShortAxis, COLORREF color)
{
	int x = 0;
	int y = semiShortAxis;
	double d1 = semiShortAxis * semiShortAxis + semiMajorAxis * semiMajorAxis * (-semiShortAxis + 0.25);
	pdc->SetPixel(center.x + x, center.y + y, color);
	pdc->SetPixel(center.x + x, center.y - y, color);
	//防止对称性造成的一些点(正上方、正下方）二次着色而产生视觉上的缺点现象，少着色一次
	while (semiShortAxis*semiShortAxis * (x + 1) < semiMajorAxis * semiMajorAxis * (y - 0.5)) {
		if (d1 < 0) {
			d1 += semiShortAxis * semiShortAxis * (2 * x + 3);
			x++;
		}
		else {
			d1 += semiShortAxis * semiShortAxis * (2 * x + 3) + semiMajorAxis * semiMajorAxis * (-2 * y + 2);
			x++;
			y--;
		}
		pdc->SetPixel(center.x + x, center.y + y, color);
		pdc->SetPixel(center.x - x, center.y + y, color);
		pdc->SetPixel(center.x + x, center.y - y, color);
		pdc->SetPixel(center.x - x, center.y - y, color);
	}
	double d2 = semiShortAxis * semiShortAxis * (x + 0.5)*(x + 0.5) + semiMajorAxis * semiMajorAxis *(y - 1) *(y - 1) - semiShortAxis * semiShortAxis * semiMajorAxis * semiMajorAxis;
	while (y > 0) {
		if (d2 < 0) {
			d2 += semiShortAxis * semiShortAxis * (2 * x + 2) + semiMajorAxis * semiMajorAxis * (-2 * y + 3);
			x++;
			y--;
		}
		else {
			d2 += semiMajorAxis * semiMajorAxis * (-2 * y + 3);
			y--;
		}
		pdc->SetPixel(center.x + x, center.y + y, color);
		pdc->SetPixel(center.x - x, center.y + y, color);
		pdc->SetPixel(center.x + x, center.y - y, color);
		pdc->SetPixel(center.x - x, center.y - y, color);
	}
	pdc->SetPixel(center.x + x, center.y, color);
	pdc->SetPixel(center.x - x, center.y, color);
	//防止对称性造成的一些点(正左方、正右方）二次着色而产生视觉上的缺点现象，额外着色一次
}


void CJLUProjectView::OnButtonDefault()
{
	DrawType = 0;
	IsStartedToDraw = 0;
}


void CJLUProjectView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CView::OnLButtonDblClk(nFlags, point);
}


void CJLUProjectView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}


void CJLUProjectView::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CView::OnKeyUp(nChar, nRepCnt, nFlags);
}


void CJLUProjectView::Move(CPoint* point, int x, int y)
{
	// TODO: 在此处添加实现代码.
}
