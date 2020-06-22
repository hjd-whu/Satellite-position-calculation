#include <iostream>
#include <string.h>
#include <sstream>
#include <fstream>
#include <string>
#include "parameter.h"
using namespace std;
//用于计算卫星的位置的三个函数
double pi = 3.1415926535898;

//第一个计算x
//放入瞬时时间和对应的卫星参数
double calculate_x(GPSTime dstGPST, parameter ep)
{
	//计算
	double n0, n, tk, t, mk, ek, f, uk, toe,  ek0, dEk,omegak,y,z;
	double corr_u, corr_r, corr_i, u, r, i;
	double xpk, ypk, l, x;
	double GM = 3.986005*pow(10, 14);
	double WE = 7.292115*pow(10, -5);
	//观测瞬间t
	t = dstGPST.tos;
	//计算参考时刻toe的平均角速度n0
	n0 = sqrt(GM) / pow(ep.sqrtA, 3);
	//计算观测时刻卫星的平均角速度n 
	n = n0 + ep.deltan;
	//计算参考时刻toe 
	toe = ep.TOE;
	if (ep.PRN[0] == 'G')
		tk = dstGPST.tos - toe;
	else
		tk = dstGPST.tos - toe-14;
	//计算平近点角mk 
	mk = ep.M0 + n * tk;
	//迭代计算偏近点角ek 
	ek = mk; 
	ek0 = 0;
	dEk = 1;
	while (dEk > pow(10,-14))
	{
		ek0 = mk + ep.e*sin(ek);
		dEk = abs(ek - ek0);
		ek = ek0;
	}
	//计算真近点角f 
	f = atan2(sqrt(1.0 - ep.e*ep.e)*sin(ek), cos(ek) - ep.e);
	//计算升交角距uk 
	uk = ep.omega1 + f;
	//进行摄动改正 
	corr_u = ep.Cuc*cos(2 * uk) + ep.Cus*sin(2 * uk);
	corr_r = ep.Crc*cos(2 * uk) + ep.Crs*sin(2 * uk);
	corr_i = ep.Cic*cos(2 * uk) + ep.Cis*sin(2 * uk);
	u = uk + corr_u;
	r = ep.sqrtA*ep.sqrtA*(1.0 - ep.e*cos(ek)) + corr_r;
	i = ep.i0 + corr_i + ep.di*tk;
	//卫星在轨道面坐标系中的位置 
	xpk = r * cos(u); 
	ypk = r * sin(u);
	//瞬时升交点精度L
	l = ep.omega + (ep.dOmega - WE)* t- ep.dOmega * toe;
	//卫星在瞬时地球坐标系中的位置 
	if (ep.PRN[0] == 'G')
		x = xpk * cos(l) - ypk * cos(i) * sin(l);//单位为m
	else
	{
		if ((ep.PRN[1]+ ep.PRN[2] == '01') | (ep.PRN[1] + ep.PRN[2] == '02') | (ep.PRN[1] + ep.PRN[2] == '03') | (ep.PRN[1] + ep.PRN[2] == '04') | (ep.PRN[1] + ep.PRN[2] == '05'))//GEO卫星，其实一共有五颗
		{
			omegak = ep.omega + ep.dOmega * tk - WE * toe;
			x = xpk * cos(omegak) - ypk * cos(i) * sin(omegak);//单位为m
			y = xpk * sin(omegak) + ypk * cos(i) * cos(omegak);//单位为m
			z = ypk * sin(i);//单位为m
			x = cos(WE * tk) * x + sin(WE * tk) * cos(-5  * pi/180) * y + sin(WE * tk) * sin(-5  * pi / 180) * z;
		}
		else
		{
			omegak = ep.omega + (ep.dOmega - WE) * tk - WE * toe;
			x = xpk * cos(omegak) - ypk * cos(i) * sin(omegak);//单位为m
		}
	}
	x = x / 1000;//单位为千米，因为精密星历的单位是km
	return x;
}

//第二个计算y
//放入瞬时时间和对应的卫星参数
double calculate_y(GPSTime dstGPST, parameter ep)
{
	//计算
	double n0, n, tk, t, mk, ek, f, uk, toe,  ek0, dEk,omegak,x,z;
	double corr_u, corr_r, corr_i, u, r, i;
	double xpk, ypk, l, y;
	double GM = 3.986005 * pow(10, 14);
	double WE = 7.292115 * pow(10, -5);
	//观测瞬间t
	t = dstGPST.tos;
	//计算参考时刻toe的平均角速度n0
	n0 = sqrt(GM) / pow(ep.sqrtA, 3);
	//计算观测时刻卫星的平均角速度n 
	n = n0 + ep.deltan;
	//计算参考时刻toe 
	toe = ep.TOE;
	if (ep.PRN[0] == 'G')
		tk = dstGPST.tos - toe;
	else
		tk = dstGPST.tos - toe - 14;
	//计算平近点角mk 
	mk = ep.M0 + n * tk;
	//迭代计算偏近点角ek 
	ek = mk;
	ek0 = 0;
	dEk = 1;
	while (dEk > pow(10, -14))
	{
		ek0 = mk + ep.e * sin(ek);
		dEk = abs(ek - ek0);
		ek = ek0;
	}
	//计算真近点角f 
	f = atan2(sqrt(1.0 - ep.e * ep.e) * sin(ek), cos(ek) - ep.e);
	//计算升交角距uk 
	uk = ep.omega1 + f;
	//进行摄动改正 
	corr_u = ep.Cuc * cos(2 * uk) + ep.Cus * sin(2 * uk);
	corr_r = ep.Crc * cos(2 * uk) + ep.Crs * sin(2 * uk);
	corr_i = ep.Cic * cos(2 * uk) + ep.Cis * sin(2 * uk);
	u = uk + corr_u;
	r = ep.sqrtA * ep.sqrtA * (1.0 - ep.e * cos(ek)) + corr_r;
	i = ep.i0 + corr_i + ep.di * tk;
	//卫星在轨道面坐标系中的位置 
	xpk = r * cos(u);
	ypk = r * sin(u);
	//瞬时升交点精度L
	l = ep.omega + (ep.dOmega - WE) * t - ep.dOmega * toe;
	//卫星在瞬时地球坐标系中的位置 
	if (ep.PRN[0] == 'G')
		y = xpk * sin(l) + ypk * cos(i) * cos(l);//单位为m
	else
	{
		if ((ep.PRN[1] + ep.PRN[2] == '01') | (ep.PRN[1] + ep.PRN[2] == '02') | (ep.PRN[1] + ep.PRN[2] == '03') | (ep.PRN[1] + ep.PRN[2] == '04') | (ep.PRN[1] + ep.PRN[2] == '05'))//GEO卫星，其实一共有五颗
		{
			omegak = ep.omega + ep.dOmega * tk - WE * toe;
			x = xpk * cos(omegak) - ypk * cos(i) * sin(omegak);//单位为m
			y = xpk * sin(omegak) + ypk * cos(i) * cos(omegak);//单位为m
			z = ypk * sin(i);//单位为m
			y = -sin(WE * tk) * x + cos(WE * tk) * cos(-5 * pi / 180) * y + cos(WE * tk) * sin(-5 * pi / 180) * z;
		}
		else
		{
			omegak = ep.omega + (ep.dOmega - WE) * tk - WE * toe;
			y = xpk * sin(omegak) + ypk * cos(i) * cos(omegak);//单位为m
		}
	}
	y = y / 1000;//单位为千米，因为精密星历的单位是km
	return y;
}

//第三个计算z
//放入瞬时时间和对应的卫星参数
double calculate_z(GPSTime dstGPST, parameter ep)
{
	//计算
	double n0, n, tk, t, mk, ek, f, uk, toe,  ek0, dEk;
	double corr_u, corr_r, corr_i, u, r, i,omegak;
	double xpk, ypk, l,x,y, z;
	double GM = 3.986005 * pow(10, 14);
	double WE = 7.292115 * pow(10, -5);
	//观测瞬间t
	t = dstGPST.tos;
	//计算参考时刻toe的平均角速度n0
	n0 = sqrt(GM) / pow(ep.sqrtA, 3);
	//计算观测时刻卫星的平均角速度n 
	n = n0 + ep.deltan;
	//计算参考时刻toe 
	toe = ep.TOE;
	if (ep.PRN[0] == 'G')
		tk = dstGPST.tos - toe;
	else
		tk = dstGPST.tos - toe - 14;
	//计算平近点角mk 
	mk = ep.M0 + n * tk;
	//迭代计算偏近点角ek 
	ek = mk;
	ek0 = 0;
	dEk = 1;
	while (dEk > pow(10, -14))
	{
		ek0 = mk + ep.e * sin(ek);
		dEk = abs(ek - ek0);
		ek = ek0;
	}
	//计算真近点角f 
	f = atan2(sqrt(1.0 - ep.e * ep.e) * sin(ek), cos(ek) - ep.e);
	//计算升交角距uk 
	uk = ep.omega1 + f;
	//进行摄动改正 
	corr_u = ep.Cuc * cos(2 * uk) + ep.Cus * sin(2 * uk);
	corr_r = ep.Crc * cos(2 * uk) + ep.Crs * sin(2 * uk);
	corr_i = ep.Cic * cos(2 * uk) + ep.Cis * sin(2 * uk);
	u = uk + corr_u;
	r = ep.sqrtA * ep.sqrtA * (1.0 - ep.e * cos(ek)) + corr_r;
	i = ep.i0 + corr_i + ep.di * tk;
	//卫星在轨道面坐标系中的位置 
	xpk = r * cos(u);
	ypk = r * sin(u);
	//瞬时升交点精度L
	l = ep.omega + (ep.dOmega - WE) * t - ep.dOmega * toe;
	//卫星在瞬时地球坐标系中的位置  
	if (ep.PRN[0] == 'G')
		z = ypk * sin(i);//单位为m
	else
	{
		if ((ep.PRN[1] + ep.PRN[2] == '01') | (ep.PRN[1] + ep.PRN[2] == '02') | (ep.PRN[1] + ep.PRN[2] == '03') | (ep.PRN[1] + ep.PRN[2] == '04') | (ep.PRN[1] + ep.PRN[2] == '05'))//GEO卫星，其实一共有五颗
		{
			omegak = ep.omega + ep.dOmega * tk - WE * toe;
			x = xpk * cos(omegak) - ypk * cos(i) * sin(omegak);//单位为m
			y = xpk * sin(omegak) + ypk * cos(i) * cos(omegak);//单位为m
			z = ypk * sin(i);//单位为m
			z =  -sin(-5 * pi / 180) * y + cos(-5 * pi / 180) * z;
		}
		else
		{
			z = ypk * sin(i);//单位为m
		}
	}
	z = z / 1000;//单位为千米，因为精密星历的单位是km
	return z;
}