#include <iostream>
#include <string.h>
#include <sstream>
#include <fstream>
#include <string>
#include "parameter.h"
using namespace std;
MJD CalendarTime2MJD(CalendarTime calT);
GPSTime MJD2GPSTime(MJD mjd);
double calculate_x(GPSTime dstGPST, parameter ep);
double calculate_y(GPSTime dstGPST, parameter ep);
double calculate_z(GPSTime dstGPST, parameter ep);
double Lagrange(double arrX[], double arrY[], double n, double x);

//�ӹ㲥�������G1λ��
void G1_position(parameter s[1329], string outfile)
{
	//�������G01һ���λ�ã�����ĵ�һ���ǣ�
	//����һ���ʱ�����У����Ϊ15���ӣ�
	//��0.00��23.45
	GPSTime onedaytime[96];
	double G1_position_get[96][3];
	int nearnum;//���������������
	for (int i = 0; i < 96; i++)
	{
		double dif = 1000000;//ʵ��ʱ���ÿһ���ο�ʱ�̵Ĳ�ֵ
		CalendarTime tem;
		tem.year = 2019;
		tem.mon = 12;
		tem.day = 1;
		tem.hour = int(i / 4);
		tem.min = (i % 4) * 15;
		tem.sec = 0;
		onedaytime[i] = MJD2GPSTime(CalendarTime2MJD(tem));
		for (int m = 0; m < 12; m++)//��ΪG1�ڹ㲥��������12������Ҫһ�����ȽϺ���һ����ӽ�,��Ŵ�0��11
		{
			if (abs(onedaytime[i].tos - s[m].TOE) < dif)
			{
				dif = abs(onedaytime[i].tos - s[m].TOE);
				nearnum = m;	
			}
		}
		G1_position_get[i][0] = calculate_x(onedaytime[i], s[nearnum]);
		G1_position_get[i][1] = calculate_y(onedaytime[i], s[nearnum]);
		G1_position_get[i][2] = calculate_z(onedaytime[i], s[nearnum]);

	}
	//����ļ�
	ofstream file_result;//��������ļ���
	//��������ļ���ַ
	string name = "/G1_position_get.txt";
	name = outfile + name;
	file_result.open(name);//�����޸��ļ�·��
	//��ʼ�����ά���������ֵ
	for (int i = 0; i < 96; i++)
	{
		file_result.flags(ios::fixed);
		file_result.precision(6);//���ñ���С��λ������ȷ��mm��ȡ6λ
		file_result << G1_position_get[i][0] << "\t" << G1_position_get[i][1] << "\t" << G1_position_get[i][2] << "\n";
	}
	//�ر�����ļ���
	file_result.close();
	cout << "�����G01���ǹ㲥�����ó�����" << endl;
}

//�Ӿ����������G1��λ��
void G1_precise(string precisefile, string outfile)
{
	//��ȡ����
	fstream file;
	file.open(precisefile);//�����ļ�

	if (!file)//�ж��Ƿ��ҵ��ļ�
	{
		cout << "�ļ���ַ���󣬴�ʧ�ܣ�" << endl;
	}
	double time[96][3];
	string buf;//��ʱ�洢���ݵ�ÿһ�У������ַ����飬��Ȼ���治�ö���
	string temp;//������ʱ��������
	int num = 0;//��������96����������ٸ�

	for (int i = 0; i < 10304; i++)
	{
		getline(file, buf);//ÿһ��ȡһ�е�����
		string title;//ȡ���ǵ�����
		title = buf.substr(0, 4);

		if (title == "PG01")
		{
			time[num][0] = stod(buf.substr(5, 12));
			time[num][1] = stod(buf.substr(19, 12));
			time[num][2] = stod(buf.substr(33, 12));
			num++;
		}
	}

	//����ļ�
	ofstream file_result;//��������ļ���
	//��������ļ���ַ
	string name = "/G1_position_precise.txt";
	name = outfile + name;
	file_result.open(name);//�����޸��ļ�·��
	//��ʼ�����ά���������ֵ
	for (int i = 0; i < 96; i++)
	{
		file_result.flags(ios::fixed);
		file_result.precision(6);//���ñ���С��λ������ȷ��mm��ȡ6λ
		file_result << time[i][0] << "\t" << time[i][1] << "\t" << time[i][2] << "\n";
	}
	//�ر�����ļ���
	file_result.close();
	cout << "�����G01���Ǿ��������ó�����" << endl;
}

//�ӹ㲥�������G2λ��
void G2_position(parameter s[1329], string outfile)
{
	//�������G02һ���λ�ã�����ĵ�2���ǣ�
	//����һ���ʱ�����У����Ϊ15���ӣ�
	//��0.00��23.45
	GPSTime onedaytime[96];
	double G2_position_get[96][3];
	int nearnum;//���������������
	for (int i = 0; i < 96; i++)
	{
		double dif = 1000000;//ʵ��ʱ���ÿһ���ο�ʱ�̵Ĳ�ֵ
		CalendarTime tem;
		tem.year = 2019;
		tem.mon = 12;
		tem.day = 1;
		tem.hour = int(i / 4);
		tem.min = (i % 4) * 15;
		tem.sec = 0;
		onedaytime[i] = MJD2GPSTime(CalendarTime2MJD(tem));
		for (int m = 12; m < 25; m++)//��ΪG2�ڹ㲥��������13������Ҫһ�����ȽϺ���һ����ӽ�,��Ŵ�12��24
		{
			if (abs(onedaytime[i].tos - s[m].TOE) < dif)
			{
				dif = abs(onedaytime[i].tos - s[m].TOE);
				nearnum = m;
			}
		}
		G2_position_get[i][0] = calculate_x(onedaytime[i], s[nearnum]);
		G2_position_get[i][1] = calculate_y(onedaytime[i], s[nearnum]);
		G2_position_get[i][2] = calculate_z(onedaytime[i], s[nearnum]);

	}
	//����ļ�
	ofstream file_result;//��������ļ���
	//��������ļ���ַ
	string name = "/G2_position_get.txt";
	name = outfile + name;
	file_result.open(name);//�����޸��ļ�·��
	//��ʼ�����ά���������ֵ
	for (int i = 0; i < 96; i++)
	{
		file_result.flags(ios::fixed);
		file_result.precision(6);//���ñ���С��λ������ȷ��mm��ȡ6λ
		file_result << G2_position_get[i][0] << "\t" << G2_position_get[i][1] << "\t" << G2_position_get[i][2] << "\n";
	}
	//�ر�����ļ���
	file_result.close();
	cout << "�����G02���ǹ㲥�����ó�����" << endl;
}

//�Ӿ����������G2��λ��
void G2_precise(string precisefile, string outfile)
{
	//��ȡ����
	fstream file;
	file.open(precisefile);//�����ļ�

	if (!file)//�ж��Ƿ��ҵ��ļ�
	{
		cout << "�ļ���ַ���󣬴�ʧ�ܣ�" << endl;
	}
	double time[96][3];
	string buf;//��ʱ�洢���ݵ�ÿһ�У������ַ����飬��Ȼ���治�ö���
	string temp;//������ʱ��������
	int num = 0;//��������96����������ٸ�

	for (int i = 0; i < 10304; i++)
	{
		getline(file, buf);//ÿһ��ȡһ�е�����
		string title;//ȡ���ǵ�����
		title = buf.substr(0, 4);

		if (title == "PG02")//����ֱ���޸�����������ж�����
		{
			time[num][0] = stod(buf.substr(5, 12));
			time[num][1] = stod(buf.substr(19, 12));
			time[num][2] = stod(buf.substr(33, 12));
			num++;
		}
	}

	//����ļ�
	ofstream file_result;//��������ļ���
	//��������ļ���ַ
	string name = "/G2_position_precise.txt";
	name = outfile + name;
	file_result.open(name);//�����޸��ļ�·��
	//��ʼ�����ά���������ֵ
	for (int i = 0; i < 96; i++)
	{
		file_result.flags(ios::fixed);
		file_result.precision(6);//���ñ���С��λ������ȷ��mm��ȡ6λ
		file_result << time[i][0] << "\t" << time[i][1] << "\t" << time[i][2] << "\n";
	}
	//�ر�����ļ���
	file_result.close();
	cout << "�����G02���Ǿ��������ó�����" << endl;
}

//�ӹ㲥�������G3λ��
void G3_position(parameter s[1329], string outfile)
{
	//�������G03һ���λ�ã�����ĵ�3���ǣ�
	//����һ���ʱ�����У����Ϊ15���ӣ�
	//��0.00��23.45
	GPSTime onedaytime[96];
	double G3_position_get[96][3];
	int nearnum;//���������������
	for (int i = 0; i < 96; i++)
	{
		double dif = 1000000;//ʵ��ʱ���ÿһ���ο�ʱ�̵Ĳ�ֵ
		CalendarTime tem;
		tem.year = 2019;
		tem.mon = 12;
		tem.day = 1;
		tem.hour = int(i / 4);
		tem.min = (i % 4) * 15;
		tem.sec = 0;
		onedaytime[i] = MJD2GPSTime(CalendarTime2MJD(tem));
		for (int m = 25; m < 37; m++)//��ΪG3�ڹ㲥��������12������Ҫһ�����ȽϺ���һ����ӽ�,��Ŵ�25��36
		{
			if (abs(onedaytime[i].tos - s[m].TOE) < dif)
			{
				dif = abs(onedaytime[i].tos - s[m].TOE);
				nearnum = m;
			}
		}
		G3_position_get[i][0] = calculate_x(onedaytime[i], s[nearnum]);
		G3_position_get[i][1] = calculate_y(onedaytime[i], s[nearnum]);
		G3_position_get[i][2] = calculate_z(onedaytime[i], s[nearnum]);

	}
	//����ļ�
	ofstream file_result;//��������ļ���
	//��������ļ���ַ
	string name = "/G3_position_get.txt";
	name = outfile + name;
	file_result.open(name);//�����޸��ļ�·��
	//��ʼ�����ά���������ֵ
	for (int i = 0; i < 96; i++)
	{
		file_result.flags(ios::fixed);
		file_result.precision(6);//���ñ���С��λ������ȷ��mm��ȡ6λ
		file_result << G3_position_get[i][0] << "\t" << G3_position_get[i][1] << "\t" << G3_position_get[i][2] << "\n";
	}
	//�ر�����ļ���
	file_result.close();
	cout << "�����G03���ǹ㲥�����ó�����" << endl;
}

//�Ӿ����������G3��λ��
void G3_precise(string precisefile, string outfile)
{
	//��ȡ����
	fstream file;
	file.open(precisefile);//�����ļ�

	if (!file)//�ж��Ƿ��ҵ��ļ�
	{
		cout << "�ļ���ַ���󣬴�ʧ�ܣ�" << endl;
	}
	double time[96][3];
	string buf;//��ʱ�洢���ݵ�ÿһ�У������ַ����飬��Ȼ���治�ö���
	string temp;//������ʱ��������
	int num = 0;//��������96����������ٸ�

	for (int i = 0; i < 10304; i++)
	{
		getline(file, buf);//ÿһ��ȡһ�е�����
		string title;//ȡ���ǵ�����
		title = buf.substr(0, 4);

		if (title == "PG03")//����ֱ���޸�����������ж�����
		{
			time[num][0] = stod(buf.substr(5, 12));
			time[num][1] = stod(buf.substr(19, 12));
			time[num][2] = stod(buf.substr(33, 12));
			num++;
		}
	}

	//����ļ�
	ofstream file_result;//��������ļ���
	//��������ļ���ַ
	string name = "/G3_position_precise.txt";
	name = outfile + name;
	file_result.open(name);//�����޸��ļ�·��
	//��ʼ�����ά���������ֵ
	for (int i = 0; i < 96; i++)
	{
		file_result.flags(ios::fixed);
		file_result.precision(6);//���ñ���С��λ������ȷ��mm��ȡ6λ
		file_result << time[i][0] << "\t" << time[i][1] << "\t" << time[i][2] << "\n";
	}
	//�ر�����ļ���
	file_result.close();
	cout << "�����G03���Ǿ��������ó�����" << endl;
}

//�ӹ㲥�������C06λ��(IGSO)
void C6_position(parameter s[1329], string outfile)
{
	//�������C01һ���λ�ã�����ĵ�һ���ǣ�
	//����һ���ʱ�����У����Ϊ15���ӣ�
	//��0.00��23.45
	GPSTime onedaytime[96];
	double C6_position_get[96][3];
	int nearnum;//���������������
	for (int i = 0; i < 96; i++)
	{
		double dif = 1000000;//ʵ��ʱ���ÿһ���ο�ʱ�̵Ĳ�ֵ
		CalendarTime tem;
		tem.year = 2019;
		tem.mon = 12;
		tem.day = 1;
		tem.hour = int(i / 4);
		tem.min = (i % 4) * 15;
		tem.sec = 0;
		onedaytime[i] = MJD2GPSTime(CalendarTime2MJD(tem));
		for (int m = 523; m < 547; m++)//��Ϊc06�ڹ㲥��������24������Ҫһ�����ȽϺ���һ����ӽ�,��Ŵ�523��546
		{
			if (abs(onedaytime[i].tos - s[m].TOE) < dif)
			{
				dif = abs(onedaytime[i].tos - s[m].TOE);
				nearnum = m;
			}
		}
		C6_position_get[i][0] = calculate_x(onedaytime[i], s[nearnum]);
		C6_position_get[i][1] = calculate_y(onedaytime[i], s[nearnum]);
		C6_position_get[i][2] = calculate_z(onedaytime[i], s[nearnum]);

	}
	//����ļ�
	ofstream file_result;//��������ļ���
	//��������ļ���ַ
	string name = "/C6_position_get.txt";
	name = outfile + name;
	file_result.open(name);//�����޸��ļ�·��
	//��ʼ�����ά���������ֵ
	for (int i = 0; i < 96; i++)
	{
		file_result.flags(ios::fixed);
		file_result.precision(6);//���ñ���С��λ������ȷ��mm��ȡ6λ
		file_result << C6_position_get[i][0] << "\t" << C6_position_get[i][1] << "\t" << C6_position_get[i][2] << "\n";
	}
	//�ر�����ļ���
	file_result.close();
	cout << "�����C06���ǹ㲥�����ó�����" << endl;
}

//�Ӿ����������C06��λ��
void C6_precise(string precisefile, string outfile)
{
	//��ȡ����
	fstream file;
	file.open(precisefile);//�����ļ�

	if (!file)//�ж��Ƿ��ҵ��ļ�
	{
		cout << "�ļ���ַ���󣬴�ʧ�ܣ�" << endl;
	}
	double time[96][3];
	string buf;//��ʱ�洢���ݵ�ÿһ�У������ַ����飬��Ȼ���治�ö���
	string temp;//������ʱ��������
	int num = 0;//��������96����������ٸ�

	for (int i = 0; i < 10304; i++)
	{
		getline(file, buf);//ÿһ��ȡһ�е�����
		string title;//ȡ���ǵ�����
		title = buf.substr(0, 4);

		if (title == "PC06")//����ֱ���޸�����������ж�����
		{
			time[num][0] = stod(buf.substr(5, 12));
			time[num][1] = stod(buf.substr(19, 12));
			time[num][2] = stod(buf.substr(33, 12));
			num++;
		}
	}

	//����ļ�
	ofstream file_result;//��������ļ���
	//��������ļ���ַ
	string name = "/C6_position_precise.txt";
	name = outfile + name;
	file_result.open(name);//�����޸��ļ�·��
	//��ʼ�����ά���������ֵ
	for (int i = 0; i < 96; i++)
	{
		file_result.flags(ios::fixed);
		file_result.precision(6);//���ñ���С��λ������ȷ��mm��ȡ6λ
		file_result << time[i][0] << "\t" << time[i][1] << "\t" << time[i][2] << "\n";
	}
	//�ر�����ļ���
	file_result.close();
	cout << "�����C06���Ǿ��������ó�����" << endl;
}

//�ӹ㲥�������C07λ��(IGSO)
void C7_position(parameter s[1329], string outfile)
{
	//�������C07һ���λ�ã�����ĵ�39���ǣ�
	//����һ���ʱ�����У����Ϊ15���ӣ�
	//��0.00��23.45
	GPSTime onedaytime[96];
	double C7_position_get[96][3];
	int nearnum;//���������������
	for (int i = 0; i < 96; i++)
	{
		double dif = 1000000;//ʵ��ʱ���ÿһ���ο�ʱ�̵Ĳ�ֵ
		CalendarTime tem;
		tem.year = 2019;
		tem.mon = 12;
		tem.day = 1;
		tem.hour = int(i / 4);
		tem.min = (i % 4) * 15;
		tem.sec = 0;
		onedaytime[i] = MJD2GPSTime(CalendarTime2MJD(tem));
		for (int m = 547; m < 570; m++)//��Ϊc07�ڹ㲥��������24������Ҫһ�����ȽϺ���һ����ӽ�,��Ŵ�547��570
		{
			if (abs(onedaytime[i].tos - s[m].TOE) < dif)
			{
				dif = abs(onedaytime[i].tos - s[m].TOE);
				nearnum = m;
			}
		}
		C7_position_get[i][0] = calculate_x(onedaytime[i], s[nearnum]);
		C7_position_get[i][1] = calculate_y(onedaytime[i], s[nearnum]);
		C7_position_get[i][2] = calculate_z(onedaytime[i], s[nearnum]);

	}
	//����ļ�
	ofstream file_result;//��������ļ���
	//��������ļ���ַ
	string name = "/C7_position_get.txt";
	name = outfile + name;
	file_result.open(name);//�����޸��ļ�·��
	//��ʼ�����ά���������ֵ
	for (int i = 0; i < 96; i++)
	{
		file_result.flags(ios::fixed);
		file_result.precision(6);//���ñ���С��λ������ȷ��mm��ȡ6λ
		file_result << C7_position_get[i][0] << "\t" << C7_position_get[i][1] << "\t" << C7_position_get[i][2] << "\n";
	}
	//�ر�����ļ���
	file_result.close();
	cout << "�����C07���ǹ㲥�����ó�����" << endl;
}

//�Ӿ����������C07��λ��
void C7_precise(string precisefile, string outfile)
{
	//��ȡ����
	fstream file;
	file.open(precisefile);//�����ļ�

	if (!file)//�ж��Ƿ��ҵ��ļ�
	{
		cout << "�ļ���ַ���󣬴�ʧ�ܣ�" << endl;
	}
	double time[96][3];
	string buf;//��ʱ�洢���ݵ�ÿһ�У������ַ����飬��Ȼ���治�ö���
	string temp;//������ʱ��������
	int num = 0;//��������96����������ٸ�

	for (int i = 0; i < 10304; i++)
	{
		getline(file, buf);//ÿһ��ȡһ�е�����
		string title;//ȡ���ǵ�����
		title = buf.substr(0, 4);

		if (title == "PC07")//����ֱ���޸�����������ж�����
		{
			time[num][0] = stod(buf.substr(5, 12));
			time[num][1] = stod(buf.substr(19, 12));
			time[num][2] = stod(buf.substr(33, 12));
			num++;
		}
	}

	//����ļ�
	ofstream file_result;//��������ļ���
	//��������ļ���ַ
	string name = "/C7_position_precise.txt";
	name = outfile + name;
	file_result.open(name);//�����޸��ļ�·��
	//��ʼ�����ά���������ֵ
	for (int i = 0; i < 96; i++)
	{
		file_result.flags(ios::fixed);
		file_result.precision(6);//���ñ���С��λ������ȷ��mm��ȡ6λ
		file_result << time[i][0] << "\t" << time[i][1] << "\t" << time[i][2] << "\n";
	}
	//�ر�����ļ���
	file_result.close();
	cout << "�����C07���Ǿ��������ó�����" << endl;
}

//�ӹ㲥�������C20λ��(MEO)
void C20_position(parameter s[1329], string outfile)
{
	//�������C20һ���λ�ã�����ĵ�51���ǣ�
	//����һ���ʱ�����У����Ϊ15���ӣ�
	//��0.00��23.45
	GPSTime onedaytime[96];
	double C20_position_get[96][3];
	int nearnum;//���������������
	for (int i = 0; i < 96; i++)
	{
		double dif = 1000000;//ʵ��ʱ���ÿһ���ο�ʱ�̵Ĳ�ֵ
		CalendarTime tem;
		tem.year = 2019;
		tem.mon = 12;
		tem.day = 1;
		tem.hour = int(i / 4);
		tem.min = (i % 4) * 15;
		tem.sec = 0;
		onedaytime[i] = MJD2GPSTime(CalendarTime2MJD(tem));
		for (int m = 821; m < 845; m++)//��Ϊc20�ڹ㲥��������24������Ҫһ�����ȽϺ���һ����ӽ�,��Ŵ�821��844
		{
			if (abs(onedaytime[i].tos - s[m].TOE) < dif)
			{
				dif = abs(onedaytime[i].tos - s[m].TOE);
				nearnum = m;
			}
		}
		C20_position_get[i][0] = calculate_x(onedaytime[i], s[nearnum]);
		C20_position_get[i][1] = calculate_y(onedaytime[i], s[nearnum]);
		C20_position_get[i][2] = calculate_z(onedaytime[i], s[nearnum]);

	}
	//����ļ�
	ofstream file_result;//��������ļ���
	//��������ļ���ַ
	string name = "/C20_position_get.txt";
	name = outfile + name;
	file_result.open(name);//�����޸��ļ�·��
	//��ʼ�����ά���������ֵ
	for (int i = 0; i < 96; i++)
	{
		file_result.flags(ios::fixed);
		file_result.precision(6);//���ñ���С��λ������ȷ��mm��ȡ6λ
		file_result << C20_position_get[i][0] << "\t" << C20_position_get[i][1] << "\t" << C20_position_get[i][2] << "\n";
	}
	//�ر�����ļ���
	file_result.close();
	cout << "�����C20���ǹ㲥�����ó�����" << endl;
}

//�Ӿ����������C20��λ��
void C20_precise(string precisefile, string outfile)
{
	//��ȡ����
	fstream file;
	file.open(precisefile);//�����ļ�

	if (!file)//�ж��Ƿ��ҵ��ļ�
	{
		cout << "�ļ���ַ���󣬴�ʧ�ܣ�" << endl;
	}
	double time[96][3];
	string buf;//��ʱ�洢���ݵ�ÿһ�У������ַ����飬��Ȼ���治�ö���
	string temp;//������ʱ��������
	int num = 0;//��������96����������ٸ�

	for (int i = 0; i < 10304; i++)
	{
		getline(file, buf);//ÿһ��ȡһ�е�����
		string title;//ȡ���ǵ�����
		title = buf.substr(0, 4);

		if (title == "PC20")//����ֱ���޸�����������ж�����
		{
			time[num][0] = stod(buf.substr(5, 12));
			time[num][1] = stod(buf.substr(19, 12));
			time[num][2] = stod(buf.substr(33, 12));
			num++;
		}
	}

	//����ļ�
	ofstream file_result;//��������ļ���
	//��������ļ���ַ
	string name = "/C20_position_precise.txt";
	name = outfile + name;
	file_result.open(name);//�����޸��ļ�·��
	//��ʼ�����ά���������ֵ
	for (int i = 0; i < 96; i++)
	{
		file_result.flags(ios::fixed);
		file_result.precision(6);//���ñ���С��λ������ȷ��mm��ȡ6λ
		file_result << time[i][0] << "\t" << time[i][1] << "\t" << time[i][2] << "\n";
	}
	//�ر�����ļ���
	file_result.close();
	cout << "�����C20���Ǿ��������ó�����" << endl;
}

//�ӹ㲥�������C21λ��(MEO)
void C21_position(parameter s[1329], string outfile)
{
	//�������C21һ���λ�ã�����ĵ�52���ǣ�
	//����һ���ʱ�����У����Ϊ15���ӣ�
	//��0.00��23.45
	GPSTime onedaytime[96];
	double C21_position_get[96][3];
	int nearnum;//���������������
	for (int i = 0; i < 96; i++)
	{
		double dif = 1000000;//ʵ��ʱ���ÿһ���ο�ʱ�̵Ĳ�ֵ
		CalendarTime tem;
		tem.year = 2019;
		tem.mon = 12;
		tem.day = 1;
		tem.hour = int(i / 4);
		tem.min = (i % 4) * 15;
		tem.sec = 0;
		onedaytime[i] = MJD2GPSTime(CalendarTime2MJD(tem));
		for (int m = 845; m < 869; m++)//��Ϊc21�ڹ㲥��������24������Ҫһ�����ȽϺ���һ����ӽ�,��Ŵ�845��868
		{
			if (abs(onedaytime[i].tos - s[m].TOE) < dif)
			{
				dif = abs(onedaytime[i].tos - s[m].TOE);
				nearnum = m;
			}
		}
		C21_position_get[i][0] = calculate_x(onedaytime[i], s[nearnum]);
		C21_position_get[i][1] = calculate_y(onedaytime[i], s[nearnum]);
		C21_position_get[i][2] = calculate_z(onedaytime[i], s[nearnum]);

	}
	//����ļ�
	ofstream file_result;//��������ļ���
	//��������ļ���ַ
	string name = "/C21_position_get.txt";
	name = outfile + name;
	file_result.open(name);//�����޸��ļ�·��
	//��ʼ�����ά���������ֵ
	for (int i = 0; i < 96; i++)
	{
		file_result.flags(ios::fixed);
		file_result.precision(6);//���ñ���С��λ������ȷ��mm��ȡ6λ
		file_result << C21_position_get[i][0] << "\t" << C21_position_get[i][1] << "\t" << C21_position_get[i][2] << "\n";
	}
	//�ر�����ļ���
	file_result.close();
	cout << "�����C21���ǹ㲥�����ó�����" << endl;
}

//�Ӿ����������C21��λ��
void C21_precise(string precisefile, string outfile)
{
	//��ȡ����
	fstream file;
	file.open(precisefile);//�����ļ�

	if (!file)//�ж��Ƿ��ҵ��ļ�
	{
		cout << "�ļ���ַ���󣬴�ʧ�ܣ�" << endl;
	}
	double time[96][3];
	string buf;//��ʱ�洢���ݵ�ÿһ�У������ַ����飬��Ȼ���治�ö���
	string temp;//������ʱ��������
	int num = 0;//��������96����������ٸ�

	for (int i = 0; i < 10304; i++)
	{
		getline(file, buf);//ÿһ��ȡһ�е�����
		string title;//ȡ���ǵ�����
		title = buf.substr(0, 4);

		if (title == "PC21")//����ֱ���޸�����������ж�����
		{
			time[num][0] = stod(buf.substr(5, 12));
			time[num][1] = stod(buf.substr(19, 12));
			time[num][2] = stod(buf.substr(33, 12));
			num++;
		}
	}

	//����ļ�
	ofstream file_result;//��������ļ���
	//��������ļ���ַ
	string name = "/C21_position_precise.txt";
	name = outfile + name;
	file_result.open(name);//�����޸��ļ�·��
	//��ʼ�����ά���������ֵ
	for (int i = 0; i < 96; i++)
	{
		file_result.flags(ios::fixed);
		file_result.precision(6);//���ñ���С��λ������ȷ��mm��ȡ6λ
		file_result << time[i][0] << "\t" << time[i][1] << "\t" << time[i][2] << "\n";
	}
	//�ر�����ļ���
	file_result.close();
	cout << "�����C21���Ǿ��������ó�����" << endl;
}

//�ӹ㲥�������C01λ��(GEO)
void C01_position(parameter s[1329], string outfile)
{
	//�������C01һ���λ�ã�����ĵ�33���ǣ�
	//����һ���ʱ�����У����Ϊ15���ӣ�
	//��0.00��23.45
	GPSTime onedaytime[96];
	double C01_position_get[96][3];
	int nearnum;//���������������
	for (int i = 0; i < 96; i++)
	{
		double dif = 1000000;//ʵ��ʱ���ÿһ���ο�ʱ�̵Ĳ�ֵ
		CalendarTime tem;
		tem.year = 2019;
		tem.mon = 12;
		tem.day = 1;
		tem.hour = int(i / 4);
		tem.min = (i % 4) * 15;
		tem.sec = 0;
		onedaytime[i] = MJD2GPSTime(CalendarTime2MJD(tem));
		for (int m = 394; m < 418; m++)//��Ϊc01�ڹ㲥��������24������Ҫһ�����ȽϺ���һ����ӽ�,��Ŵ�394��417
		{
			if (abs(onedaytime[i].tos - s[m].TOE) < dif)
			{
				dif = abs(onedaytime[i].tos - s[m].TOE);
				nearnum = m;
			}
		}
		C01_position_get[i][0] = calculate_x(onedaytime[i], s[nearnum]);
		C01_position_get[i][1] = calculate_y(onedaytime[i], s[nearnum]);
		C01_position_get[i][2] = calculate_z(onedaytime[i], s[nearnum]);

	}
	//����ļ�
	ofstream file_result;//��������ļ���
	//��������ļ���ַ
	string name = "/C01_position_get.txt";
	name = outfile + name;
	file_result.open(name);//�����޸��ļ�·��
	//��ʼ�����ά���������ֵ
	for (int i = 0; i < 96; i++)
	{
		file_result.flags(ios::fixed);
		file_result.precision(6);//���ñ���С��λ������ȷ��mm��ȡ6λ
		file_result << C01_position_get[i][0] << "\t" << C01_position_get[i][1] << "\t" << C01_position_get[i][2] << "\n";
	}
	//�ر�����ļ���
	file_result.close();
	cout << "�����C01���ǹ㲥�����ó�����" << endl;
}

//�Ӿ����������C01��λ��
void C01_precise(string precisefile, string outfile)
{
	//��ȡ����
	fstream file;
	file.open(precisefile);//�����ļ�

	if (!file)//�ж��Ƿ��ҵ��ļ�
	{
		cout << "�ļ���ַ���󣬴�ʧ�ܣ�" << endl;
	}
	double time[96][3];
	string buf;//��ʱ�洢���ݵ�ÿһ�У������ַ����飬��Ȼ���治�ö���
	string temp;//������ʱ��������
	int num = 0;//��������96����������ٸ�

	for (int i = 0; i < 10304; i++)
	{
		getline(file, buf);//ÿһ��ȡһ�е�����
		string title;//ȡ���ǵ�����
		title = buf.substr(0, 4);

		if (title == "PC01")//����ֱ���޸�����������ж�����
		{
			time[num][0] = stod(buf.substr(5, 12));
			time[num][1] = stod(buf.substr(19, 12));
			time[num][2] = stod(buf.substr(33, 12));
			num++;
		}
	}

	//����ļ�
	ofstream file_result;//��������ļ���
	//��������ļ���ַ
	string name = "/C01_position_precise.txt";
	name = outfile + name;
	file_result.open(name);//�����޸��ļ�·��
	//hjd
	//��ʼ�����ά���������ֵ
	for (int i = 0; i < 96; i++)
	{
		file_result.flags(ios::fixed);
		file_result.precision(6);//���ñ���С��λ������ȷ��mm��ȡ6λ
		file_result << time[i][0] << "\t" << time[i][1] << "\t" << time[i][2] << "\n";
	}
	//�ر�����ļ���
	file_result.close();
	cout << "�����C01���Ǿ��������ó�����" << endl;
}

//�ӹ㲥�������C02λ��(GEO)
void C02_position(parameter s[1329], string outfile)
{
	//�������C02һ���λ�ã�����ĵ�34���ǣ�
	//����һ���ʱ�����У����Ϊ15���ӣ�
	//��0.00��23.45
	GPSTime onedaytime[96];
	double C02_position_get[96][3];
	int nearnum;//���������������
	for (int i = 0; i < 96; i++)
	{
		double dif = 1000000;//ʵ��ʱ���ÿһ���ο�ʱ�̵Ĳ�ֵ
		CalendarTime tem;
		tem.year = 2019;
		tem.mon = 12;
		tem.day = 1;
		tem.hour = int(i / 4);
		tem.min = (i % 4) * 15;
		tem.sec = 0;
		onedaytime[i] = MJD2GPSTime(CalendarTime2MJD(tem));
		for (int m = 418; m < 442; m++)//��Ϊc01�ڹ㲥��������24������Ҫһ�����ȽϺ���һ����ӽ�,��Ŵ�418��441
		{
			if (abs(onedaytime[i].tos - s[m].TOE) < dif)
			{
				dif = abs(onedaytime[i].tos - s[m].TOE);
				nearnum = m;
			}
		}
		C02_position_get[i][0] = calculate_x(onedaytime[i], s[nearnum]);
		C02_position_get[i][1] = calculate_y(onedaytime[i], s[nearnum]);
		C02_position_get[i][2] = calculate_z(onedaytime[i], s[nearnum]);

	}
	//����ļ�
	ofstream file_result;//��������ļ���
	//��������ļ���ַ
	string name = "/C02_position_get.txt";
	name = outfile + name;
	file_result.open(name);//�����޸��ļ�·��
	//��ʼ�����ά���������ֵ
	for (int i = 0; i < 96; i++)
	{
		file_result.flags(ios::fixed);
		file_result.precision(6);//���ñ���С��λ������ȷ��mm��ȡ6λ
		file_result << C02_position_get[i][0] << "\t" << C02_position_get[i][1] << "\t" << C02_position_get[i][2] << "\n";
	}
	//�ر�����ļ���
	file_result.close();
	cout << "�����C02���ǹ㲥�����ó�����" << endl;
}

//�Ӿ����������C02��λ��
void C02_precise(string precisefile, string outfile)
{
	//��ȡ����
	fstream file;
	file.open(precisefile);//�����ļ�

	if (!file)//�ж��Ƿ��ҵ��ļ�
	{
		cout << "�ļ���ַ���󣬴�ʧ�ܣ�" << endl;
	}
	double time[96][3];
	string buf;//��ʱ�洢���ݵ�ÿһ�У������ַ����飬��Ȼ���治�ö���
	string temp;//������ʱ��������
	int num = 0;//��������96����������ٸ�

	for (int i = 0; i < 10304; i++)
	{
		getline(file, buf);//ÿһ��ȡһ�е�����
		string title;//ȡ���ǵ�����
		title = buf.substr(0, 4);

		if (title == "PC02")//����ֱ���޸�����������ж�����
		{
			time[num][0] = stod(buf.substr(5, 12));
			time[num][1] = stod(buf.substr(19, 12));
			time[num][2] = stod(buf.substr(33, 12));
			num++;
		}
	}

	//����ļ�
	ofstream file_result;//��������ļ���
	//��������ļ���ַ
	string name = "/C02_position_precise.txt";
	name = outfile + name;
	file_result.open(name);//�����޸��ļ�·��
	//��ʼ�����ά���������ֵ
	for (int i = 0; i < 96; i++)
	{
		file_result.flags(ios::fixed);
		file_result.precision(6);//���ñ���С��λ������ȷ��mm��ȡ6λ
		file_result << time[i][0] << "\t" << time[i][1] << "\t" << time[i][2] << "\n";
	}
	//�ر�����ļ���
	file_result.close();
	cout << "�����C02���Ǿ��������ó�����" << endl;
}