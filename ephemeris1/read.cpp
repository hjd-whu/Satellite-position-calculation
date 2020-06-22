#include <iostream>
#include <string.h>
#include <sstream>
#include <fstream>
#include <string>
#include "parameter.h"
#include "statement.h"
using namespace std;

//��������
MJD CalendarTime2MJD(CalendarTime calT);
GPSTime MJD2GPSTime(MJD mjd);
double calculate_x(GPSTime dstGPST, parameter ep);
double calculate_y(GPSTime dstGPST, parameter ep);
double calculate_z(GPSTime dstGPST, parameter ep);

void readeph(parameter s[1329],string broadcastfile)
{
	//��ȡ����
	string fileName = broadcastfile;
	fstream file;
	file.open(fileName);//�����ļ�

	if (!file)//�ж��Ƿ��ҵ��ļ�
	{
		cout << "�ļ���ַ���󣬴�ʧ�ܣ�" << endl;
	}

	string buf;//��ʱ�洢���ݵ�ÿһ�У������ַ����飬��Ȼ���治�ö���
	int realnumber = 0;//ʵ�ʵ��ļ�������
	int number = 0;//��ʼ���ļ����Сѭ������Ϊһ���ṹ��8�У���������жϵ�����һ��
	int order = -1;//��������������ڼ�����Ч���ǵ�������ͬʱΪ���������������Ϊһ��Ҫ��һ���������0��ʼ������ȡ-1
	int length = 0;//Ϊ�����ַ���ת�ַ�������׼��
	string blank = "               ";//15�����ȵĿ��ַ��������������ıȽϣ���Ȼ�յ��ַ���û����ֵ
	int broadcastnum = 0;//���ǵ����������
	string PRN;//��������ÿһ����PRN�ţ������������ж�������

	//�����ļ�ͷ����
	while (1)//ֱ�ӽ�ѭ������ʼ���ļ�
	{
		getline(file, buf);//ÿһ��ȡһ�е�����
		realnumber++;//ʵ�ʵ���������һ
		//�ж��ļ�ͷ�Ƿ���꣬�������ļ�ͷ������ѭ��
		if (buf.find("END OF HEADER") == 60)
			break;
	}

	//������ļ������
	for (realnumber; realnumber < 77943; )
	{
		getline(file, buf);//ÿһ��ȡһ�е�����
		realnumber++;//ʵ�ʵ���������һ
		if (realnumber < 70464)
			number = (realnumber - 11) % 8;//��ʼ���ļ����Сѭ������Ϊһ���ṹ��8�У���������жϵ�����һ��
		else
			number = (realnumber - 70463) % 8;//��ʼ���ļ����Сѭ������Ϊһ���ṹ��8�У���������жϵ�����һ�У�����70463֮��Ϳ�ʼ��C��ͷ��������
		//���жϿ�ͷ��ֻ����C��G��ͷ������
		string initial_judge;//ȡ����ͷ�ĵ�һ����ĸ
		initial_judge = buf.substr(0, 1);
		if (initial_judge == "S")
		{
			realnumber = realnumber + 3;
			for (int i = 0; i < 3; i++)
			{
				getline(file, buf);//����һ��
			}
			continue;
		}
		if (initial_judge == "R")
		{
			realnumber = realnumber + 3;
			for (int i = 0; i < 3; i++)
			{
				getline(file, buf);//����һ��
			}
			continue;
		}
		if (initial_judge == "E")
		{
			realnumber = realnumber + 7;
			for (int i = 0; i < 7; i++)
			{
				getline(file, buf);//����һ��
			}
			continue;
		}
		switch (number)
		{
		case (1)://������1����һ�е�����
		{
			//�����������һ
			order++;
			//��һ�еĲ���
			string PRN_num;//PRN��
			string year;//��
			string month;//��
			string day;//��
			string hour;//ʱ
			string min;//��
			string sec;//��
			string SClockBias;//�����ӵ�ƫ������
			string SClockBias_power;//�����ӵ�ƫ�����
			string SClockDri;//�����ӵ�Ư������
			string SClockDri_power;//�����ӵ�Ư�Ƶ���
			string SClockDriV;//�����ӵ�Ư���ٶ�����
			string SClockDriV_power;//�����ӵ�Ư���ٶȵ���
			//��ʼ��ȡ�ַ���
			PRN_num = buf.substr(0, 3);
			year = buf.substr(4, 4);
			month = buf.substr(9, 2);
			day = buf.substr(12, 2);
			hour = buf.substr(15, 2);
			min = buf.substr(18, 2);
			sec = buf.substr(21, 2);
			SClockBias = buf.substr(23, 15);
			SClockBias_power = buf.substr(39, 3);
			SClockDri = buf.substr(42, 15);
			SClockDri_power = buf.substr(58, 3);
			SClockDriV = buf.substr(61, 15);
			SClockDriV_power = buf.substr(77, 3);
			//��ʼ��ֵ
			length = PRN_num.copy(s[order].PRN, 19);
			s[order].PRN[length] = '\0';
			s[order].year = stod(year);
			s[order].month = stod(month);
			s[order].day = stod(day);
			s[order].hour = stod(hour);
			s[order].min = stod(min);
			s[order].sec = stod(sec);
			s[order].SClockBias = stod(SClockBias) * pow(10, stod(SClockBias_power));
			s[order].SClockDri = stod(SClockDri) * pow(10, stod(SClockDri_power));
			s[order].SClockDriV = stod(SClockDriV) * pow(10, stod(SClockDriV_power));
			if (PRN != PRN_num)
			{
				broadcastnum++;
				s[order].broadcastnum = broadcastnum;
			}
			else
			{
				s[order].broadcastnum = broadcastnum;
			}
			PRN = buf.substr(0, 3);
			break;
		}
		case (2)://������2���ڶ��е�����
		{
			//�ڶ��еĲ���
			string IODE;//���ݡ���������ʱ������
			string IODE_power;//���ݡ���������ʱ����
			string Crs;//����
			string Crs_power;//��
			string deltan;//����
			string deltan_power;//��
			string M0;//����
			string M0_power;//��
			//��ʼ��ȡ�ַ���
			IODE = buf.substr(4, 15);
			IODE_power = buf.substr(20, 3);
			Crs = buf.substr(23, 15);
			Crs_power = buf.substr(39, 3);
			deltan = buf.substr(42, 15);
			deltan_power = buf.substr(58, 3);
			M0 = buf.substr(61, 15);
			M0_power = buf.substr(77, 3);
			//��ʼ��ֵ
			s[order].IODE = stod(IODE) * pow(10, stod(IODE_power));
			s[order].Crs = stod(Crs) * pow(10, stod(Crs_power));
			s[order].deltan = stod(deltan) * pow(10, stod(deltan_power));
			s[order].M0 = stod(M0) * pow(10, stod(M0_power));
			break;
		}
		case (3)://������3�������е�����
		{
			//�����еĲ���
			string Cuc;//����
			string Cuc_power;//��
			string e;//���ƫ��������
			string e_power;//���ƫ������
			string Cus;//����
			string Cus_power;//��
			string sqrtA;//����
			string sqrtA_power;//��
			Cuc = buf.substr(4, 15);
			Cuc_power = buf.substr(20, 3);
			e = buf.substr(23, 15);
			e_power = buf.substr(39, 3);
			Cus = buf.substr(42, 15);
			Cus_power = buf.substr(58, 3);
			sqrtA = buf.substr(61, 15);
			sqrtA_power = buf.substr(77, 3);
			//��ʼ��ֵ
			s[order].Cuc = stod(Cuc) * pow(10, stod(Cuc_power));
			s[order].e = stod(e) * pow(10, stod(e_power));
			s[order].Cus = stod(Cus) * pow(10, stod(Cus_power));
			s[order].sqrtA = stod(sqrtA) * pow(10, stod(sqrtA_power));
			break;
		}
		case (4)://������4�������е�����
		{
			//�����еĲ���
			string TOE;//�����Ĳο�ʱ������
			string TOE_power;//�����Ĳο�ʱ��������
			string Cic;//����
			string Cic_power;//��
			string omega;//����
			string omega_power;//��
			string Cis;//����
			string Cis_power;//��
			TOE = buf.substr(4, 15);
			TOE_power = buf.substr(20, 3);
			Cic = buf.substr(23, 15);
			Cic_power = buf.substr(39, 3);
			omega = buf.substr(42, 15);
			omega_power = buf.substr(58, 3);
			Cis = buf.substr(61, 15);
			Cis_power = buf.substr(77, 3);
			//��ʼ��ֵ
			s[order].TOE = stod(TOE) * pow(10, stod(TOE_power));
			s[order].Cic = stod(Cic) * pow(10, stod(Cic_power));
			s[order].omega = stod(omega) * pow(10, stod(omega_power));
			s[order].Cis = stod(Cis) * pow(10, stod(Cis_power));
			break;
		}
		case (5)://������5�������е�����
		{
			//�����еĲ���
			string i0;//����
			string i0_power;//��
			string Crc;//����
			string Crc_power;//����
			string omega1;//����
			string omega1_power;//��
			string dOmega;//����
			string dOmega_power;//��
			i0 = buf.substr(4, 15);
			i0_power = buf.substr(20, 3);
			Crc = buf.substr(23, 15);
			Crc_power = buf.substr(39, 3);
			omega1 = buf.substr(42, 15);
			omega1_power = buf.substr(58, 3);
			dOmega = buf.substr(61, 15);
			dOmega_power = buf.substr(77, 3);
			//��ʼ��ֵ
			s[order].i0 = stod(i0) * pow(10, stod(i0_power));
			s[order].Crc = stod(Crc) * pow(10, stod(Crc_power));
			s[order].omega1 = stod(omega1) * pow(10, stod(omega1_power));
			s[order].dOmega = stod(dOmega) * pow(10, stod(dOmega_power));
			break;
		}
		case (6)://������6�������е�����
		{
			//�����еĲ���
			string di;//��IDOT������
			string di_power;//��IDOT����
			string l2_code;//L2�ϵ�������
			string l2_code_power;//L2�ϵ�����
			string wn;//GPS��������TOEһͬ��ʾʱ�䣩����
			string wn_power;//GPS��������TOEһͬ��ʾʱ�䣩��
			string l2_P_tag;//L2��P�����ݱ������
			string l2_P_tag_power;//L2��P�����ݱ����
			di = buf.substr(4, 15);
			di_power = buf.substr(20, 3);
			l2_code = buf.substr(23, 15);
			l2_code_power = buf.substr(39, 3);
			wn = buf.substr(42, 15);
			wn_power = buf.substr(58, 3);
			l2_P_tag = buf.substr(61, 15);
			l2_P_tag_power = buf.substr(77, 3);
			//��ʼ��ֵ
			s[order].di = stod(di) * pow(10, stod(di_power));
			s[order].l2_code = stod(l2_code) * pow(10, stod(l2_code_power));
			s[order].wn = stod(wn) * pow(10, stod(wn_power));
			s[order].l2_P_tag = stod(l2_P_tag) * pow(10, stod(l2_P_tag_power));
			break;
		}
		case (7)://������7�������е�����
		{
			//�����еĲ���
			string SatAccur;//���Ǿ���(m)����
			string SatAccur_power;//���Ǿ���(m)��
			string SatState;//���ǽ���״̬����
			string SatState_power;//���ǽ���״̬��
			string TGD;//(sec)����
			string TGD_power;//(sec)��
			string IODC;//�ӵ�������������
			string IODC_power;//�ӵ�����������
			SatAccur = buf.substr(4, 15);
			SatAccur_power = buf.substr(20, 3);
			SatState = buf.substr(23, 15);
			SatState_power = buf.substr(39, 3);
			TGD = buf.substr(42, 15);
			TGD_power = buf.substr(58, 3);
			IODC = buf.substr(61, 15);
			IODC_power = buf.substr(77, 3);
			//��ʼ��ֵ
			s[order].SatAccur = stod(SatAccur) * pow(10, stod(SatAccur_power));
			s[order].SatState = stod(SatState) * pow(10, stod(SatState_power));
			s[order].TGD = stod(TGD) * pow(10, stod(TGD_power));
			s[order].IODC = stod(IODC) * pow(10, stod(IODC_power));
			break;
		}
		case (0)://������0���ڰ��е�����
		{
			//�ڰ��еĲ���
			string SendTime;//���ķ���ʱ�̣���λΪGPS�ܵ��룬ͨ�������֣�HOW���е�Z�����ó�������
			string SendTime_power;//���ķ���ʱ�̣���λΪGPS�ܵ��룬ͨ�������֣�HOW���е�Z�����ó�����
			string FitRange;//�����������
			string FitRange_power;//���������
			string spare1;//��������
			string spare1_power;//������
			string spare2;//��������
			string spare2_power;//������
			SendTime = buf.substr(4, 15);
			SendTime_power = buf.substr(20, 3);
			FitRange = buf.substr(23, 15);
			FitRange_power = buf.substr(39, 3);
			spare1 = buf.substr(42, 15);
			spare1_power = buf.substr(58, 3);
			spare2 = buf.substr(61, 15);
			spare2_power = buf.substr(77, 3);
			//��ʼ��ֵ
			s[order].SendTime = stod(SendTime) * pow(10, stod(SendTime_power));//�ڰ��е�һ����һֱ���ڣ��ʲ��ж�
			if (FitRange != blank)//ֻ�в�Ϊȫ�ǿո���ַ����Żḳֵ
			{
				s[order].FitRange = stod(FitRange) * pow(10, stod(FitRange_power));
			}
			else//����ֵΪ0
			{
				s[order].FitRange = 0;
			}
			if (spare1 != blank)//ֻ�в�Ϊȫ�ǿո���ַ����Żḳֵ
			{
				s[order].spare1 = stod(spare1) * pow(10, stod(spare1_power));
			}
			else//����ֵΪ0
			{
				s[order].spare1 = 0;
			}
			if (spare2 != blank)//ֻ�в�Ϊȫ�ǿո���ַ����Żḳֵ
			{
				s[order].spare2 = stod(spare2) * pow(10, stod(spare2_power));
			}
			else//����ֵΪ0
			{
				s[order].spare2 = 0;
			}
			break;
		}
		default:
			cout << "��Ч�ĸ�ֵ" << endl;
		}
	}
	file.close();

	//�ļ���ȡ��ϣ���ʼ�����е����Ǹ�ֵ����λ��
	for (int i = 0; i < 1329; i++)
	{
		//�����ǵ�ʱ�丳ֵ����
		CalendarTime tem;
		tem.year = (int)s[i].year;
		tem.mon = (int)s[i].month;
		tem.day = (int)s[i].day;
		tem.hour = (int)s[i].hour;
		tem.min = (int)s[i].min;
		tem.sec = s[i].sec;
		GPSTime temp = MJD2GPSTime(CalendarTime2MJD(tem));
		s[i].x = calculate_x(temp, s[i]);
		s[i].y = calculate_y(temp, s[i]);
		s[i].z = calculate_z(temp, s[i]);
	}

	//��ʾ����
	for (int i = 0; i < 1329; i++)
	{
		//�����ǵ�ʱ���λ����ʾ����
		cout << "Ŀǰ��ȡ�ļ�����Ϊ��" << i + 1 << endl;
		cout << "�������ڶ�ȡ��GPS���Ǻͱ��������е����к�Ϊ��" << s[i].broadcastnum << endl;
		cout << "�����ǵ�PRN��Ϊ" << s[i].PRN << endl;
		cout << "�����ǵ�ʱ��Ϊ��" << s[i].year << "." << s[i].month << "." << s[i].day << "." << s[i].hour << "." << s[i].min << "." << s[i].sec << endl;
		cout << "X��" << s[i].x << "   " << "Y��" << s[i].y << "   " << "Z��" << s[i].z << endl;
		cout << endl;
	}
}