﻿
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
END_MESSAGE_MAP()

// CJLUProjectView 构造/析构

CJLUProjectView::CJLUProjectView() noexcept
{
	// TODO: 在此处添加构造代码

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