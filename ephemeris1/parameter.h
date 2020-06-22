#pragma once
#include <string.h>
using namespace std;
class parameter
{
    public:
	//��һ�еĲ���
		char PRN[20]="initialization";//PRN��
		double year=0;//��
		double month = 0;//��
		double day = 0;//��
		double hour = 0;//ʱ
		double min = 0;//��
		double sec = 0;//��
		double SClockBias = 0;//�����ӵ�ƫ��(s)
		double SClockDri = 0;//�����ӵ�Ư��(s/s)
		double SClockDriV = 0;//�����ӵ�Ư���ٶ�(s/s^2)
	//�ڶ��еĲ������㲥���1��
		double IODE = 0;//���ݡ���������ʱ��
		double Crs = 0;//(m)
		double deltan = 0;//(rad/s)
		double M0 = 0;//(rad)
	//�����еĲ������㲥���2��
		double Cuc = 0;//(rad)
		double e = 0;//���ƫ����
		double Cus = 0;//(rad)
		double sqrtA = 0;//(m^0.5)
	//�����еĲ������㲥���3��
		double TOE = 0;//�����Ĳο�ʱ�̣�GPS���ڵ�������
		double Cic = 0;//(rad)
		double omega = 0;//(rad)
		double Cis = 0;//(rad)
	//�����еĲ������㲥���4��
		double i0 = 0;//(rad)
		double Crc = 0;//(m)
		double omega1 = 0;//(rad)
		double dOmega = 0;//(rad/s)
	//�����еĲ������㲥���5��
		double di = 0;//��IDOT��
		double l2_code = 0;//L2�ϵ���
		double wn = 0;//GPS��������TOEһͬ��ʾʱ�䣩
		double l2_P_tag = 0;//L2��P�����ݱ��
	//�����еĲ������㲥���6��
		double SatAccur = 0;//���Ǿ���(m)
		double SatState = 0;//���ǽ���״̬
		double TGD = 0;//(sec)
		double IODC = 0;//�ӵ���������
	//�ڰ��еĲ������㲥���7��
		double SendTime = 0;//���ķ���ʱ�̣���λΪGPS�ܵ��룬ͨ�������֣�HOW���е�Z�����ó���
		double FitRange = 0;//�������
		double spare1 = 0;//����
		double spare2 = 0;//����
	//���ǵ�λ��
		double x = 0;
		double y = 0;
		double z = 0;
	//���ǵ����������
		int broadcastnum = 0;
};

class CalendarTime
{
public:
	int year;//��
	int mon;//��
	int day;//��
	int hour;//ʱ
	int min;//��
	double sec;//��
};
class GPSTime
{
public:
	int wn;//����
	double tos;//����
};
class MJD
{
public:
	int day;//������
	double fracDay;//�����С��
};