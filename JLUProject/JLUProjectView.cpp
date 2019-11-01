
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
	ON_COMMAND(ID_BUTTONELLIPSE, &CJLUProjectView::OnButtonellipse)
	ON_COMMAND(ID_BUTTONCIRCLE, &CJLUProjectView::OnButtoncircle)
	ON_COMMAND(ID_BUTTONLINE, &CJLUProjectView::OnButtonline)
	ON_COMMAND(ID_BUTTONDEFAULT, &CJLUProjectView::OnButtondefault)
END_MESSAGE_MAP()

// CJLUProjectView 构造/析构

CJLUProjectView::CJLUProjectView() noexcept
{
	// TODO: 在此处添加构造代码

	isStartedToDraw = 0;
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

void CJLUProjectView::OnDraw(CDC* /*pDC*/)
{
	CJLUProjectDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
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
	dia.m_r = ColorR;
	dia.m_g = ColorG;
	dia.m_b = ColorB;
	if (dia.DoModal() == IDOK) {
		ColorR = dia.m_r;
		ColorG = dia.m_g;
		ColorB = dia.m_b;
	}
}


void CJLUProjectView::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (drawType > 0) {
		isStartedToDraw = 1;
		startPoint = point;
		endPoint = point;
	}	
	CView::OnLButtonDown(nFlags, point);
}


void CJLUProjectView::OnLButtonUp(UINT nFlags, CPoint point)
{
	if (drawType > 0) {
		isStartedToDraw = 0;
	}
	CView::OnLButtonUp(nFlags, point);
}


void CJLUProjectView::OnRButtonDown(UINT nFlags, CPoint point)
{
	if (isStartedToDraw > 0) {
		CDC* pdc = this->GetDC();
		pdc->SetROP2(R2_NOTXORPEN);
		DrawThing(pdc); //覆盖掉最后一次画的图
		ReleaseDC(pdc);
	}
	isStartedToDraw = 0;
	CView::OnRButtonDown(nFlags, point);
}


void CJLUProjectView::OnMouseMove(UINT nFlags, CPoint point)
{
	if (drawType > 0 && isStartedToDraw > 0) {
		CDC* pdc = this->GetDC();
		pdc->SetROP2(R2_NOTXORPEN);
		DrawThing(pdc); //覆盖掉上一次画的图
		endPoint = point;
		DrawThing(pdc); //画一个新图
		ReleaseDC(pdc);
	}
	CView::OnMouseMove(nFlags, point);
}


void CJLUProjectView::OnButtonellipse()
{
	drawType = 3;
}


void CJLUProjectView::OnButtoncircle()
{
	drawType = 2;
}


void CJLUProjectView::OnButtonline()
{
	drawType = 1;
}


void CJLUProjectView::DrawThing(CDC* pdc)
{
	switch (drawType) {
	case 1: DDALine(pdc); break;
	case 2: BresenhamCircle(pdc); break;
	case 3: MidpointEllipse(pdc); break;
	default: break;
	}
}


void CJLUProjectView::DDALine(CDC* pdc)
{
	double dx = endPoint.x - startPoint.x;
	double dy = endPoint.y - startPoint.y;
	double e = (fabs(dx) > fabs(dy) ? fabs(dx) : fabs(dy));
	dx /= e;
	dy /= e;
	double x = startPoint.x;
	double y = startPoint.y;
	for (int i = 1; i <= e; ++i) {
		pdc->SetPixel((int)(x + 0.5), (int)(y + 0.5), RGB(ColorR, ColorG, ColorB));
		x += dx;
		y += dy;
	}
}


void CJLUProjectView::BresenhamCircle(CDC* pdc)
{
	int x1 = startPoint.x;
	int x2 = endPoint.x;
	int y1 = startPoint.y;
	int y2 = endPoint.y;
	int radius = (int)sqrt((x2 - x1)*(x2 - x1) + (y2 - y1)*(y2 - y1));
	int x = 0;
	int y = radius;
	int p = 3 - 2 * radius;
	while (x <= y) {
		pdc->SetPixel(x1 + x, y1 + y, RGB(ColorR, ColorG, ColorB));
		pdc->SetPixel(x1 - x, y1 + y, RGB(ColorR, ColorG, ColorB));
		pdc->SetPixel(x1 + x, y1 - y, RGB(ColorR, ColorG, ColorB));
		pdc->SetPixel(x1 - x, y1 - y, RGB(ColorR, ColorG, ColorB));
		pdc->SetPixel(x1 + y, y1 + x, RGB(ColorR, ColorG, ColorB));
		pdc->SetPixel(x1 - y, y1 + x, RGB(ColorR, ColorG, ColorB));
		pdc->SetPixel(x1 + y, y1 - x, RGB(ColorR, ColorG, ColorB));
		pdc->SetPixel(x1 - y, y1 - x, RGB(ColorR, ColorG, ColorB));
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


void CJLUProjectView::MidpointEllipse(CDC* pdc)
{
	int _m = (int)fabs(endPoint.x - startPoint.x);
	int _n = (int)fabs(endPoint.y - startPoint.y);
	int majorAxis = _m / 2; //长轴
	int shortAxis = _n / 2; //短轴
	int midX = startPoint.x + (endPoint.x - startPoint.x) / 2; //椭圆圆心点坐标
	int midY = startPoint.y + (endPoint.y - startPoint.y) / 2;
	int x = 0;
	int y = shortAxis;
	double d1 = shortAxis * shortAxis + majorAxis * majorAxis * (-shortAxis + 0.25);
	pdc->SetPixel(midX + x, midY + y, RGB(ColorR, ColorG, ColorB));
	pdc->SetPixel(midX - x, midY + y, RGB(ColorR, ColorG, ColorB));
	pdc->SetPixel(midX + x, midY - y, RGB(ColorR, ColorG, ColorB));
	pdc->SetPixel(midX - x, midY - y, RGB(ColorR, ColorG, ColorB));
	while (shortAxis*shortAxis * (x + 1) < majorAxis * majorAxis * (y - 0.5)) {
		if (d1 < 0) {
			d1 += shortAxis * shortAxis * (2 * x + 3);
			x++;
		}
		else {
			d1 += shortAxis * shortAxis * (2 * x + 3) + majorAxis * majorAxis * (-2 * y + 2);
			x++;
			y--;
		}
		pdc->SetPixel(midX + x, midY + y, RGB(ColorR, ColorG, ColorB));
		pdc->SetPixel(midX - x, midY + y, RGB(ColorR, ColorG, ColorB));
		pdc->SetPixel(midX + x, midY - y, RGB(ColorR, ColorG, ColorB));
		pdc->SetPixel(midX - x, midY - y, RGB(ColorR, ColorG, ColorB));
	}
	double d2 = shortAxis * shortAxis * (x + 0.5)*(x + 0.5) + majorAxis * majorAxis *(y - 1) *(y - 1) - shortAxis * shortAxis * majorAxis * majorAxis;
	while (y > 0) {
		if (d2 < 0) {
			d2 += shortAxis * shortAxis * (2 * x + 2) + majorAxis * majorAxis * (-2 * y + 3);
			x++;
			y--;
		}
		else {
			d2 += majorAxis * majorAxis * (-2 * y + 3);
			y--;
		}
		pdc->SetPixel(midX + x, midY + y, RGB(ColorR, ColorG, ColorB));
		pdc->SetPixel(midX - x, midY + y, RGB(ColorR, ColorG, ColorB));
		pdc->SetPixel(midX + x, midY - y, RGB(ColorR, ColorG, ColorB));
		pdc->SetPixel(midX - x, midY - y, RGB(ColorR, ColorG, ColorB));
	}
}


void CJLUProjectView::OnButtondefault()
{
	drawType = 0;
	isStartedToDraw = 0;
}
