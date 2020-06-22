#include <iostream>
#include "parameter.h"
using namespace std;

//��������ת��Ϊ������
MJD CalendarTime2MJD(CalendarTime calT)
{
	MJD mjd;
	int y, m;
	if (calT.mon <= 2)
	{
		y = calT.year - 1;
		m = calT.mon + 12;
	}
	else
	{
		y = calT.year;
		m = calT.mon;
	}
	//ǰ���������
	mjd.day = (int)((int)(365.25*y) + (int)(30.6001*((double)m + 1)) + calT.day + calT.hour / 24 + 1720981.5 - 2400000.5);
	//��������ʱ����Ҳ��Ϊ��
	mjd.fracDay = 1.0*calT.hour / 24 + 1.0*calT.min / 24 / 60 + calT.sec / 24 / 3600;
	return mjd;
}

//������ת��ΪGPSʱ
GPSTime MJD2GPSTime(MJD mjd)
{
	GPSTime gpst;
	gpst.wn = (mjd.day - 44244) / 7;
	gpst.tos = (mjd.day + mjd.fracDay - 44244 - (double)gpst.wn * 7) * 86400;
	return gpst;
}