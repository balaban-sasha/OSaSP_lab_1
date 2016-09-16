#pragma once

#include "stdafx.h"
#include "lab_1.h"
#include<vector>
#include<list>	
#include <commdlg.h>
class Shapes
{
protected:
	int x, y, cx, cy;
	HPEN hpen;
	HBRUSH brush;
public:
	virtual void SetHpen(HPEN) {};
	virtual void SetBrush(COLORREF) {};
	virtual void SetStartPosition(int,int)
	{
	}
	virtual void SetEndPosition(int,int)
	{
	}
	virtual void AddPointToArray(int, int)
	{
	}

	virtual void SetText(WPARAM, int)
	{
	}
	virtual void Draw(HDC){
	}
};
class MyRectangle:public Shapes
{

public:
	using Shapes::Shapes;
	void SetStartPosition(int x1, int y1)
	{
		x = x1;
		y = y1;
	}
	void SetEndPosition(int x2, int y2)
	{
		cx = x2;
		cy = y2;
	}
	void SetHpen(HPEN hPen)
	{
		hpen = hPen;
	}
	void SetBrush(COLORREF hBrush)
	{
		brush = (HBRUSH)CreateSolidBrush(hBrush);
	}
	void Draw(HDC hdc)
	{
		SelectObject(hdc, brush);
			SelectObject(hdc, hpen);
		Rectangle(hdc, x, y, cx, cy);
	}
};
class MyEllipse :public Shapes
{

public:
	using Shapes::Shapes;
	void SetStartPosition(int x1, int y1)
	{
		x = x1;
		y = y1;
	}
	void SetEndPosition(int x2, int y2)
	{
		cx = x2;
		cy = y2;
	}
	void SetBrush(COLORREF hBrush)
	{
		brush = (HBRUSH)CreateSolidBrush(hBrush);
	}
	void SetHpen(HPEN hPen)
	{
		hpen = hPen;
	}
	void Draw(HDC hdc)
	{
		SelectObject(hdc, brush);
		SelectObject(hdc, hpen);
		Ellipse(hdc, x, y, cx, cy);
	}
};
class MyLine :public Shapes
{

public:
	using Shapes::Shapes;
	void SetStartPosition(int x1, int y1)
	{
		x = x1;
		y = y1;
	}
	void SetEndPosition(int x2, int y2)
	{
		cx = x2;
		cy = y2;
	}
	void SetHpen(HPEN hPen)
	{
		hpen = hPen;
	}
	void Draw(HDC hdc)
	{
		SelectObject(hdc, hpen);
		MoveToEx(hdc, x, y,NULL);
		LineTo(hdc, cx, cy);
	}
};
class MyPen:public Shapes
{
private:
	std::vector <POINT> points;
public:
	using Shapes::Shapes;
	void AddPointToArray(int x1, int y1)
	{
		POINT tempPoint;
		tempPoint.x=x1;
		tempPoint.y=y1;
		points.push_back(tempPoint);
	}
	void SetHpen(HPEN hPen)
	{
		hpen = hPen;
	}
	void Draw(HDC hdc)
	{
		SelectObject(hdc, hpen);
		POINT temp;
		if (points.size() > 1)
		{
			temp.x = points[0].x;
			temp.y = points[0].y;
			MoveToEx(hdc, temp.x, temp.y, NULL);
			for (int i = 1; i < points.size(); i++)
			{
				LineTo(hdc, points[i].x, points[i].y);
			}
		}
		else
			SetPixel(hdc, points[0].x, points[0].y, RGB(0, 0, 0));
		
	}
};
class MyPolygone :public Shapes
{
private:
	std::vector <POINT> points;
public:
	using Shapes::Shapes;
	void AddPointToArray(int x1, int y1)
	{
		POINT tempPoint;
		tempPoint.x = x1;
		tempPoint.y = y1;
		points.push_back(tempPoint);
	}
	void SetHpen(HPEN hPen)
	{
		hpen = hPen;
	}
	void SetBrush(COLORREF hBrush)
	{
		brush = (HBRUSH)CreateSolidBrush(hBrush);
	}
	void Draw(HDC hdc)
	{
		SelectObject(hdc, brush);
		SelectObject(hdc, hpen);
		Polygon(hdc, points.data(), points.size());

	}
};
class MyPolyLine :public Shapes
{
private:
	std::vector <POINT> points;
public:
	using Shapes::Shapes;
	void AddPointToArray(int x1, int y1)
	{
		POINT tempPoint;
		tempPoint.x = x1;
		tempPoint.y = y1;
		points.push_back(tempPoint);
	}
	void SetHpen(HPEN hPen)
	{
		hpen = hPen;
	}
	void Draw(HDC hdc)
	{
		SelectObject(hdc, hpen);
		Polyline(hdc, points.data(), points.size());

	}
};
class MyText :public Shapes
{
public:
	 int size=0;
	 COLORREF color;
	 PTCHAR text=(PTCHAR)GlobalAlloc(GPTR, 50000 * sizeof(TCHAR));
public:
	using Shapes::Shapes;

	void SetStartPosition(int x1, int y1)
	{
		x = x1;
		y = y1;
	}
	void setHpen(HPEN hPen)
	{
		hpen = hPen;
	}
	void SetBrush(COLORREF hBrush)
	{
		color = hBrush;
	}
	void SetText(WPARAM wParam,int newSize)
	{
		text[size] = (char)wParam;
		size = newSize;
	}
	void Draw(HDC hdc)
	{
		RECT rec; 
		SetRect(&rec, x, y, x+10*size, y+30);
		SelectObject(hdc, hpen);
		SetTextColor(hdc, color);
	DrawText(hdc, (LPCWSTR)text, size, &rec, DT_TOP | DT_LEFT);

	}
};
class MyEraser :public Shapes
{
private:
	std::vector <POINT> points;
public:
	using Shapes::Shapes;
	void AddPointToArray(int x1, int y1)
	{
		POINT tempPoint;
		tempPoint.x = x1;
		tempPoint.y = y1;
		points.push_back(tempPoint);
	}
	void SetHpen(HPEN hPen)
	{
		hpen = hPen;
	}
	void Draw(HDC hdc)
	{
		SelectObject(hdc, hpen);
		POINT temp;
		if (points.size() > 1)
		{
			temp.x = points[0].x;
			temp.y = points[0].y;
			MoveToEx(hdc, temp.x, temp.y, NULL);
			for (int i = 1; i < points.size(); i++)
			{
				LineTo(hdc, points[i].x, points[i].y);
			}
		}
		else
			SetPixel(hdc, points[0].x, points[0].y, RGB(0, 0, 0));

	}
};
