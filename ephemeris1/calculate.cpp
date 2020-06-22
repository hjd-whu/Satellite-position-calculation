#include <iostream>
#include <string.h>
#include <sstream>
#include <fstream>
#include <string>
#include "parameter.h"
using namespace std;
//���ڼ������ǵ�λ�õ���������
double pi = 3.1415926535898;

//��һ������x
//����˲ʱʱ��Ͷ�Ӧ�����ǲ���
double calculate_x(GPSTime dstGPST, parameter ep)
{
	//����
	double n0, n, tk, t, mk, ek, f, uk, toe,  ek0, dEk,omegak,y,z;
	double corr_u, corr_r, corr_i, u, r, i;
	double xpk, ypk, l, x;
	double GM = 3.986005*pow(10, 14);
	double WE = 7.292115*pow(10, -5);
	//�۲�˲��t
	t = dstGPST.tos;
	//����ο�ʱ��toe��ƽ�����ٶ�n0
	n0 = sqrt(GM) / pow(ep.sqrtA, 3);
	//����۲�ʱ�����ǵ�ƽ�����ٶ�n 
	n = n0 + ep.deltan;
	//����ο�ʱ��toe 
	toe = ep.TOE;
	if (ep.PRN[0] == 'G')
		tk = dstGPST.tos - toe;
	else
		tk = dstGPST.tos - toe-14;
	//����ƽ�����mk 
	mk = ep.M0 + n * tk;
	//��������ƫ�����ek 
	ek = mk; 
	ek0 = 0;
	dEk = 1;
	while (dEk > pow(10,-14))
	{
		ek0 = mk + ep.e*sin(ek);
		dEk = abs(ek - ek0);
		ek = ek0;
	}
	//����������f 
	f = atan2(sqrt(1.0 - ep.e*ep.e)*sin(ek), cos(ek) - ep.e);
	//���������Ǿ�uk 
	uk = ep.omega1 + f;
	//�����㶯���� 
	corr_u = ep.Cuc*cos(2 * uk) + ep.Cus*sin(2 * uk);
	corr_r = ep.Crc*cos(2 * uk) + ep.Crs*sin(2 * uk);
	corr_i = ep.Cic*cos(2 * uk) + ep.Cis*sin(2 * uk);
	u = uk + corr_u;
	r = ep.sqrtA*ep.sqrtA*(1.0 - ep.e*cos(ek)) + corr_r;
	i = ep.i0 + corr_i + ep.di*tk;
	//�����ڹ��������ϵ�е�λ�� 
	xpk = r * cos(u); 
	ypk = r * sin(u);
	//˲ʱ�����㾫��L
	l = ep.omega + (ep.dOmega - WE)* t- ep.dOmega * toe;
	//������˲ʱ��������ϵ�е�λ�� 
	if (ep.PRN[0] == 'G')
		x = xpk * cos(l) - ypk * cos(i) * sin(l);//��λΪm
	else
	{
		if ((ep.PRN[1]+ ep.PRN[2] == '01') | (ep.PRN[1] + ep.PRN[2] == '02') | (ep.PRN[1] + ep.PRN[2] == '03') | (ep.PRN[1] + ep.PRN[2] == '04') | (ep.PRN[1] + ep.PRN[2] == '05'))//GEO���ǣ���ʵһ�������
		{
			omegak = ep.omega + ep.dOmega * tk - WE * toe;
			x = xpk * cos(omegak) - ypk * cos(i) * sin(omegak);//��λΪm
			y = xpk * sin(omegak) + ypk * cos(i) * cos(omegak);//��λΪm
			z = ypk * sin(i);//��λΪm
			x = cos(WE * tk) * x + sin(WE * tk) * cos(-5  * pi/180) * y + sin(WE * tk) * sin(-5  * pi / 180) * z;
		}
		else
		{
			omegak = ep.omega + (ep.dOmega - WE) * tk - WE * toe;
			x = xpk * cos(omegak) - ypk * cos(i) * sin(omegak);//��λΪm
		}
	}
	x = x / 1000;//��λΪǧ�ף���Ϊ���������ĵ�λ��km
	return x;
}

//�ڶ�������y
//����˲ʱʱ��Ͷ�Ӧ�����ǲ���
double calculate_y(GPSTime dstGPST, parameter ep)
{
	//����
	double n0, n, tk, t, mk, ek, f, uk, toe,  ek0, dEk,omegak,x,z;
	double corr_u, corr_r, corr_i, u, r, i;
	double xpk, ypk, l, y;
	double GM = 3.986005 * pow(10, 14);
	double WE = 7.292115 * pow(10, -5);
	//�۲�˲��t
	t = dstGPST.tos;
	//����ο�ʱ��toe��ƽ�����ٶ�n0
	n0 = sqrt(GM) / pow(ep.sqrtA, 3);
	//����۲�ʱ�����ǵ�ƽ�����ٶ�n 
	n = n0 + ep.deltan;
	//����ο�ʱ��toe 
	toe = ep.TOE;
	if (ep.PRN[0] == 'G')
		tk = dstGPST.tos - toe;
	else
		tk = dstGPST.tos - toe - 14;
	//����ƽ�����mk 
	mk = ep.M0 + n * tk;
	//��������ƫ�����ek 
	ek = mk;
	ek0 = 0;
	dEk = 1;
	while (dEk > pow(10, -14))
	{
		ek0 = mk + ep.e * sin(ek);
		dEk = abs(ek - ek0);
		ek = ek0;
	}
	//����������f 
	f = atan2(sqrt(1.0 - ep.e * ep.e) * sin(ek), cos(ek) - ep.e);
	//���������Ǿ�uk 
	uk = ep.omega1 + f;
	//�����㶯���� 
	corr_u = ep.Cuc * cos(2 * uk) + ep.Cus * sin(2 * uk);
	corr_r = ep.Crc * cos(2 * uk) + ep.Crs * sin(2 * uk);
	corr_i = ep.Cic * cos(2 * uk) + ep.Cis * sin(2 * uk);
	u = uk + corr_u;
	r = ep.sqrtA * ep.sqrtA * (1.0 - ep.e * cos(ek)) + corr_r;
	i = ep.i0 + corr_i + ep.di * tk;
	//�����ڹ��������ϵ�е�λ�� 
	xpk = r * cos(u);
	ypk = r * sin(u);
	//˲ʱ�����㾫��L
	l = ep.omega + (ep.dOmega - WE) * t - ep.dOmega * toe;
	//������˲ʱ��������ϵ�е�λ�� 
	if (ep.PRN[0] == 'G')
		y = xpk * sin(l) + ypk * cos(i) * cos(l);//��λΪm
	else
	{
		if ((ep.PRN[1] + ep.PRN[2] == '01') | (ep.PRN[1] + ep.PRN[2] == '02') | (ep.PRN[1] + ep.PRN[2] == '03') | (ep.PRN[1] + ep.PRN[2] == '04') | (ep.PRN[1] + ep.PRN[2] == '05'))//GEO���ǣ���ʵһ�������
		{
			omegak = ep.omega + ep.dOmega * tk - WE * toe;
			x = xpk * cos(omegak) - ypk * cos(i) * sin(omegak);//��λΪm
			y = xpk * sin(omegak) + ypk * cos(i) * cos(omegak);//��λΪm
			z = ypk * sin(i);//��λΪm
			y = -sin(WE * tk) * x + cos(WE * tk) * cos(-5 * pi / 180) * y + cos(WE * tk) * sin(-5 * pi / 180) * z;
		}
		else
		{
			omegak = ep.omega + (ep.dOmega - WE) * tk - WE * toe;
			y = xpk * sin(omegak) + ypk * cos(i) * cos(omegak);//��λΪm
		}
	}
	y = y / 1000;//��λΪǧ�ף���Ϊ���������ĵ�λ��km
	return y;
}

//����������z
//����˲ʱʱ��Ͷ�Ӧ�����ǲ���
double calculate_z(GPSTime dstGPST, parameter ep)
{
	//����
	double n0, n, tk, t, mk, ek, f, uk, toe,  ek0, dEk;
	double corr_u, corr_r, corr_i, u, r, i,omegak;
	double xpk, ypk, l,x,y, z;
	double GM = 3.986005 * pow(10, 14);
	double WE = 7.292115 * pow(10, -5);
	//�۲�˲��t
	t = dstGPST.tos;
	//����ο�ʱ��toe��ƽ�����ٶ�n0
	n0 = sqrt(GM) / pow(ep.sqrtA, 3);
	//����۲�ʱ�����ǵ�ƽ�����ٶ�n 
	n = n0 + ep.deltan;
	//����ο�ʱ��toe 
	toe = ep.TOE;
	if (ep.PRN[0] == 'G')
		tk = dstGPST.tos - toe;
	else
		tk = dstGPST.tos - toe - 14;
	//����ƽ�����mk 
	mk = ep.M0 + n * tk;
	//��������ƫ�����ek 
	ek = mk;
	ek0 = 0;
	dEk = 1;
	while (dEk > pow(10, -14))
	{
		ek0 = mk + ep.e * sin(ek);
		dEk = abs(ek - ek0);
		ek = ek0;
	}
	//����������f 
	f = atan2(sqrt(1.0 - ep.e * ep.e) * sin(ek), cos(ek) - ep.e);
	//���������Ǿ�uk 
	uk = ep.omega1 + f;
	//�����㶯���� 
	corr_u = ep.Cuc * cos(2 * uk) + ep.Cus * sin(2 * uk);
	corr_r = ep.Crc * cos(2 * uk) + ep.Crs * sin(2 * uk);
	corr_i = ep.Cic * cos(2 * uk) + ep.Cis * sin(2 * uk);
	u = uk + corr_u;
	r = ep.sqrtA * ep.sqrtA * (1.0 - ep.e * cos(ek)) + corr_r;
	i = ep.i0 + corr_i + ep.di * tk;
	//�����ڹ��������ϵ�е�λ�� 
	xpk = r * cos(u);
	ypk = r * sin(u);
	//˲ʱ�����㾫��L
	l = ep.omega + (ep.dOmega - WE) * t - ep.dOmega * toe;
	//������˲ʱ��������ϵ�е�λ��  
	if (ep.PRN[0] == 'G')
		z = ypk * sin(i);//��λΪm
	else
	{
		if ((ep.PRN[1] + ep.PRN[2] == '01') | (ep.PRN[1] + ep.PRN[2] == '02') | (ep.PRN[1] + ep.PRN[2] == '03') | (ep.PRN[1] + ep.PRN[2] == '04') | (ep.PRN[1] + ep.PRN[2] == '05'))//GEO���ǣ���ʵһ�������
		{
			omegak = ep.omega + ep.dOmega * tk - WE * toe;
			x = xpk * cos(omegak) - ypk * cos(i) * sin(omegak);//��λΪm
			y = xpk * sin(omegak) + ypk * cos(i) * cos(omegak);//��λΪm
			z = ypk * sin(i);//��λΪm
			z =  -sin(-5 * pi / 180) * y + cos(-5 * pi / 180) * z;
		}
		else
		{
			z = ypk * sin(i);//��λΪm
		}
	}
	z = z / 1000;//��λΪǧ�ף���Ϊ���������ĵ�λ��km
	return z;
}