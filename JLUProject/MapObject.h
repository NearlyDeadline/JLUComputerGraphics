#pragma once
class MapObject
{
public:
	MapObject();
	virtual ~MapObject();
	CPoint StartPoint;
	CPoint EndPoint;
	int DrawType = 0;
	int r;
	int g;
	int b;
};

