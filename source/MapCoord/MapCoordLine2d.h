/** 
   *@file         MapCoordLine2d.hpp
   *@brief        直线
 */
#ifndef MGVLINE2D_HPP__
#define MGVLINE2D_HPP__
#pragma once
#include <math.h>
#include <exception>
#include <Windows.h>

#ifndef M_PI_2
#define M_PI_2     1.57079632679489661923
#endif //M_PI_2

/// 直线
class MapCoordLine2d
{
public:
    MapCoordLine2d();
    ~MapCoordLine2d();
public:
    void InitKKb(double dK,double dKb);                        ///< k,kb
    void Init2Pt(double dX0,double dY0,double dX1,double dY1); ///< pt1,pt2
    void InitABC(double dA,double dB,double dC);               ///< a,b,c
    void InitPtAngle(double dX0,double dY0,double dAngle);     ///< pt1 angle(un clock)(rad)
public:
    double GetXAtY(double y);            ///< get x
    double GetYAtX(double x);            ///< get y
    double GetK();                       ///< get k
    double GetKb();                      ///< get kb
    double GetAngle();                   ///< (un clock)(rad)
    double GetA() {return a;}
    double GetB() {return b;}
    double GetC() {return c;}
public:
    bool CrossPt(const MapCoordLine2d& oline,double* x=nullptr,double* y=nullptr);///< CrossPt
    double ptDisLine(double x,double y); ///< point dis to line
    void Draw(HDC hdc);
    void GetVertical(double x,double y,MapCoordLine2d& oline);
    void GetParallel(double x,double y,MapCoordLine2d& oline);
    bool IsVert();
    bool IsHort();
    void GetVertFoot(double x,double y,double& dtx,double& dty);
private:
    /// 三参数方式 ax+by+c=0 ; 
    //Vert x=3 ;  (1)x+(0)y+(-3)=0; so a=1,b=0,c=[-3]
    //Hort y=4 ;  (0)x+(1)y+(-4)=0; so a=0,b=1,c=[-4]
    //KKb  kx+kb=y  so a=k,b=-1,c=kb  so k=a/-b kb=c/-b
    double a,b,c;
    double deps;
};


#endif //MGVLINE2D_HPP__
