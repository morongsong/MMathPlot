/**
*@file         MapCoordLine2d.cpp
*@brief        MapCoordLine2d
*@author       jimsmorong
*@date         2014/11/08 00:28:41
*/
#include "stdafx.h"
#include "MapCoordLine2d.h"

MapCoordLine2d::MapCoordLine2d()
{
    deps = 1e-10;
    a = 1;
    b = 0;
    c = 0;
}

MapCoordLine2d::~MapCoordLine2d()
{
}

void MapCoordLine2d::InitKKb(double dK, double dKb)
{
    a = dK;
    b = -1;
    c = dKb;
}


void MapCoordLine2d::Init2Pt(double dX0, double dY0, double dX1, double dY1)
{
#if 1
    a = dY1 - dY0;
    b = dX0 - dX1;
    c = dX1*dY0 - dX0*dY1;
#else
    if (fabs(dX0 - dX1) < deps)
    {
        a = 1;
        b = 0;
        c = -dX0;
    }
    else
    {
        a = (dY1 - dY0) / (dX1 - dX0);    // ¼ÆËãÐ±ÂÊ: k  = (y1 - y0) / (x1 - x0)
        b = -1;
        c = dY0 - dX0*a;             // ¼ÆËãb:    kb = y1 - x1*k
    }
#endif
}


/// pt1 angle(un clock)(rad)
void MapCoordLine2d::InitPtAngle(double dX0, double dY0, double dAngle)
{
    if (fabs(dAngle - M_PI_2) < deps)
    {
        a = 1;
        b = 0;
        c = -dX0;
    }
    else
    {
        a = tan(dAngle);
        b = -1;
        c = dY0 - a*dX0;
    }
}


void MapCoordLine2d::InitABC(double dA, double dB, double dC)
{
    a = dA;
    b = dB;
    c = dC;
}

bool MapCoordLine2d::IsVert()
{
    return fabs(b) < deps;
}

bool MapCoordLine2d::IsHort()
{
    return fabs(a) < deps;
}

bool MapCoordLine2d::CrossPt(const MapCoordLine2d& line, double* x/*=NULL*/, double* y/*=NULL*/)
{
    //ax+by+c=0 aex+bey+ce=0  adx+bdy+cd=0
    //dx+ey+f=0 dbx+eby+fb=0  dax+eay+fa=0
    //x = (fb-ce)/(ae-db);
    //y = -(fa-cd)/(ea-bd);
    double d = line.a, e = line.b, f = line.c;
    double r = (a*e - d*b);
    if (!(fabs(r) < deps))
    {
        *x = (f*b - c*e) / r;
        *y = -(f*a - c*d) / r;
        return true;
    }
    return false;
}


double MapCoordLine2d::GetXAtY(double y)
{
    if (!IsVert())
    {
        return (b * y + c) / a *-1;
    }
    else
    {
        return -1 * c;
    }
}


double MapCoordLine2d::GetYAtX(double x)
{
    if (!IsVert())
    {
        return (a * x + c) / b *-1;
    }
    else
    {
        return deps;
    }
    return deps;
}

double MapCoordLine2d::GetK()
{
    if (!IsVert())
    {
        return a / -b;
    }
    throw std::exception("no alloc to do that");
}

double MapCoordLine2d::GetKb()
{
    if (!IsVert())
    {
        return c / -b;
    }
    throw std::exception("no alloc");
}

double MapCoordLine2d::GetAngle()
{
    if (!IsVert())
    {
        return atan(a / -b);
    }
    else
    {
        return M_PI_2;
    }
}


double MapCoordLine2d::ptDisLine(double x, double y)
{
    double d = fabs(a*x + b*y + c) / sqrt(a*a + b*b);
    return d;
}

void MapCoordLine2d::GetVertFoot(double x, double y, double& dtx, double& dty)
{
    double dr = a*a + b*b;
    dtx = (b*b*x - a*b*y - a*c) / dr;
    dty = (a*a*y - a*b*x - b*c) / dr;
    return;
}

void MapCoordLine2d::Draw(HDC hdc)
{
    if (!IsVert())
    {
        double xd = 0;
        double yd = GetYAtX(xd);
        MoveToEx(hdc, (int)xd, (int)yd, 0);
        double xd1 = 1000;
        double yd1 = GetYAtX(xd1);
        LineTo(hdc, (int)xd1, (int)yd1);
    }
    else
    {
        MoveToEx(hdc, (int)GetXAtY(0), 0, 0);
        LineTo(hdc, (int)GetXAtY(1000), 1000);
    }
}

void MapCoordLine2d::GetVertical(double x, double y, MapCoordLine2d& oline)
{
    if (!IsVert())
    {
        if (IsHort())
        {
            oline.a = 1;
            oline.b = 0;
            oline.c = -1 * x;
        }
        else
        {
            oline.a = -1 / a;
            oline.b = -1;
            oline.c = y - oline.a * x;
        }

    }
    else
    {
        oline.a = 0;
        oline.b = 1;
        oline.c = -1 * y;
    }
    return;
}

void MapCoordLine2d::GetParallel(double x, double y, MapCoordLine2d& oline)
{
    if (!IsVert())
    {
        if (fabs(a)<deps)
        {
            oline.a = 0;
            oline.b = 1;
            oline.c = -1 * y;
        }
        else
        {
            oline.a = a;
            oline.b = -1;
            oline.c = y - oline.a * x;
        }

    }
    else
    {
        oline.a = 1;
        oline.b = 0;
        oline.c = -1 * x;
    }
    return;
}