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

//从广播星历获得G1位置
void G1_position(parameter s[1329], string outfile)
{
	//输出卫星G01一天的位置（总体的第一颗星）
	//创建一天的时间序列（间隔为15分钟）
	//从0.00到23.45
	GPSTime onedaytime[96];
	double G1_position_get[96][3];
	int nearnum;//相距最近的卫星序号
	for (int i = 0; i < 96; i++)
	{
		double dif = 1000000;//实际时间和每一个参考时刻的差值
		CalendarTime tem;
		tem.year = 2019;
		tem.mon = 12;
		tem.day = 1;
		tem.hour = int(i / 4);
		tem.min = (i % 4) * 15;
		tem.sec = 0;
		onedaytime[i] = MJD2GPSTime(CalendarTime2MJD(tem));
		for (int m = 0; m < 12; m++)//因为G1在广播星历中有12个，需要一个个比较和哪一个最接近,序号从0到11
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
	//输出文件
	ofstream file_result;//创建输出文件流
	//创建输出文件地址
	string name = "/G1_position_get.txt";
	name = outfile + name;
	file_result.open(name);//自行修改文件路径
	//开始输出二维数组里的数值
	for (int i = 0; i < 96; i++)
	{
		file_result.flags(ios::fixed);
		file_result.precision(6);//设置保留小数位数，精确到mm，取6位
		file_result << G1_position_get[i][0] << "\t" << G1_position_get[i][1] << "\t" << G1_position_get[i][2] << "\n";
	}
	//关闭输出文件流
	file_result.close();
	cout << "已输出G01卫星广播星历得出坐标" << endl;
}

//从精密星历获得G1的位置
void G1_precise(string precisefile, string outfile)
{
	//读取数据
	fstream file;
	file.open(precisefile);//读入文件

	if (!file)//判断是否找到文件
	{
		cout << "文件地址错误，打开失败！" << endl;
	}
	double time[96][3];
	string buf;//临时存储数据的每一行，不用字符数组，不然后面不好断行
	string temp;//用来暂时储存数字
	int num = 0;//用来计数96个点读出多少个

	for (int i = 0; i < 10304; i++)
	{
		getline(file, buf);//每一次取一行的内容
		string title;//取卫星的名称
		title = buf.substr(0, 4);

		if (title == "PG01")
		{
			time[num][0] = stod(buf.substr(5, 12));
			time[num][1] = stod(buf.substr(19, 12));
			time[num][2] = stod(buf.substr(33, 12));
			num++;
		}
	}

	//输出文件
	ofstream file_result;//创建输出文件流
	//创建输出文件地址
	string name = "/G1_position_precise.txt";
	name = outfile + name;
	file_result.open(name);//自行修改文件路径
	//开始输出二维数组里的数值
	for (int i = 0; i < 96; i++)
	{
		file_result.flags(ios::fixed);
		file_result.precision(6);//设置保留小数位数，精确到mm，取6位
		file_result << time[i][0] << "\t" << time[i][1] << "\t" << time[i][2] << "\n";
	}
	//关闭输出文件流
	file_result.close();
	cout << "已输出G01卫星精密星历得出坐标" << endl;
}

//从广播星历获得G2位置
void G2_position(parameter s[1329], string outfile)
{
	//输出卫星G02一天的位置（总体的第2颗星）
	//创建一天的时间序列（间隔为15分钟）
	//从0.00到23.45
	GPSTime onedaytime[96];
	double G2_position_get[96][3];
	int nearnum;//相距最近的卫星序号
	for (int i = 0; i < 96; i++)
	{
		double dif = 1000000;//实际时间和每一个参考时刻的差值
		CalendarTime tem;
		tem.year = 2019;
		tem.mon = 12;
		tem.day = 1;
		tem.hour = int(i / 4);
		tem.min = (i % 4) * 15;
		tem.sec = 0;
		onedaytime[i] = MJD2GPSTime(CalendarTime2MJD(tem));
		for (int m = 12; m < 25; m++)//因为G2在广播星历中有13个，需要一个个比较和哪一个最接近,序号从12到24
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
	//输出文件
	ofstream file_result;//创建输出文件流
	//创建输出文件地址
	string name = "/G2_position_get.txt";
	name = outfile + name;
	file_result.open(name);//自行修改文件路径
	//开始输出二维数组里的数值
	for (int i = 0; i < 96; i++)
	{
		file_result.flags(ios::fixed);
		file_result.precision(6);//设置保留小数位数，精确到mm，取6位
		file_result << G2_position_get[i][0] << "\t" << G2_position_get[i][1] << "\t" << G2_position_get[i][2] << "\n";
	}
	//关闭输出文件流
	file_result.close();
	cout << "已输出G02卫星广播星历得出坐标" << endl;
}

//从精密星历获得G2的位置
void G2_precise(string precisefile, string outfile)
{
	//读取数据
	fstream file;
	file.open(precisefile);//读入文件

	if (!file)//判断是否找到文件
	{
		cout << "文件地址错误，打开失败！" << endl;
	}
	double time[96][3];
	string buf;//临时存储数据的每一行，不用字符数组，不然后面不好断行
	string temp;//用来暂时储存数字
	int num = 0;//用来计数96个点读出多少个

	for (int i = 0; i < 10304; i++)
	{
		getline(file, buf);//每一次取一行的内容
		string title;//取卫星的名称
		title = buf.substr(0, 4);

		if (title == "PG02")//后面直接修改这里，这里是判断依据
		{
			time[num][0] = stod(buf.substr(5, 12));
			time[num][1] = stod(buf.substr(19, 12));
			time[num][2] = stod(buf.substr(33, 12));
			num++;
		}
	}

	//输出文件
	ofstream file_result;//创建输出文件流
	//创建输出文件地址
	string name = "/G2_position_precise.txt";
	name = outfile + name;
	file_result.open(name);//自行修改文件路径
	//开始输出二维数组里的数值
	for (int i = 0; i < 96; i++)
	{
		file_result.flags(ios::fixed);
		file_result.precision(6);//设置保留小数位数，精确到mm，取6位
		file_result << time[i][0] << "\t" << time[i][1] << "\t" << time[i][2] << "\n";
	}
	//关闭输出文件流
	file_result.close();
	cout << "已输出G02卫星精密星历得出坐标" << endl;
}

//从广播星历获得G3位置
void G3_position(parameter s[1329], string outfile)
{
	//输出卫星G03一天的位置（总体的第3颗星）
	//创建一天的时间序列（间隔为15分钟）
	//从0.00到23.45
	GPSTime onedaytime[96];
	double G3_position_get[96][3];
	int nearnum;//相距最近的卫星序号
	for (int i = 0; i < 96; i++)
	{
		double dif = 1000000;//实际时间和每一个参考时刻的差值
		CalendarTime tem;
		tem.year = 2019;
		tem.mon = 12;
		tem.day = 1;
		tem.hour = int(i / 4);
		tem.min = (i % 4) * 15;
		tem.sec = 0;
		onedaytime[i] = MJD2GPSTime(CalendarTime2MJD(tem));
		for (int m = 25; m < 37; m++)//因为G3在广播星历中有12个，需要一个个比较和哪一个最接近,序号从25到36
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
	//输出文件
	ofstream file_result;//创建输出文件流
	//创建输出文件地址
	string name = "/G3_position_get.txt";
	name = outfile + name;
	file_result.open(name);//自行修改文件路径
	//开始输出二维数组里的数值
	for (int i = 0; i < 96; i++)
	{
		file_result.flags(ios::fixed);
		file_result.precision(6);//设置保留小数位数，精确到mm，取6位
		file_result << G3_position_get[i][0] << "\t" << G3_position_get[i][1] << "\t" << G3_position_get[i][2] << "\n";
	}
	//关闭输出文件流
	file_result.close();
	cout << "已输出G03卫星广播星历得出坐标" << endl;
}

//从精密星历获得G3的位置
void G3_precise(string precisefile, string outfile)
{
	//读取数据
	fstream file;
	file.open(precisefile);//读入文件

	if (!file)//判断是否找到文件
	{
		cout << "文件地址错误，打开失败！" << endl;
	}
	double time[96][3];
	string buf;//临时存储数据的每一行，不用字符数组，不然后面不好断行
	string temp;//用来暂时储存数字
	int num = 0;//用来计数96个点读出多少个

	for (int i = 0; i < 10304; i++)
	{
		getline(file, buf);//每一次取一行的内容
		string title;//取卫星的名称
		title = buf.substr(0, 4);

		if (title == "PG03")//后面直接修改这里，这里是判断依据
		{
			time[num][0] = stod(buf.substr(5, 12));
			time[num][1] = stod(buf.substr(19, 12));
			time[num][2] = stod(buf.substr(33, 12));
			num++;
		}
	}

	//输出文件
	ofstream file_result;//创建输出文件流
	//创建输出文件地址
	string name = "/G3_position_precise.txt";
	name = outfile + name;
	file_result.open(name);//自行修改文件路径
	//开始输出二维数组里的数值
	for (int i = 0; i < 96; i++)
	{
		file_result.flags(ios::fixed);
		file_result.precision(6);//设置保留小数位数，精确到mm，取6位
		file_result << time[i][0] << "\t" << time[i][1] << "\t" << time[i][2] << "\n";
	}
	//关闭输出文件流
	file_result.close();
	cout << "已输出G03卫星精密星历得出坐标" << endl;
}

//从广播星历获得C06位置(IGSO)
void C6_position(parameter s[1329], string outfile)
{
	//输出卫星C01一天的位置（总体的第一颗星）
	//创建一天的时间序列（间隔为15分钟）
	//从0.00到23.45
	GPSTime onedaytime[96];
	double C6_position_get[96][3];
	int nearnum;//相距最近的卫星序号
	for (int i = 0; i < 96; i++)
	{
		double dif = 1000000;//实际时间和每一个参考时刻的差值
		CalendarTime tem;
		tem.year = 2019;
		tem.mon = 12;
		tem.day = 1;
		tem.hour = int(i / 4);
		tem.min = (i % 4) * 15;
		tem.sec = 0;
		onedaytime[i] = MJD2GPSTime(CalendarTime2MJD(tem));
		for (int m = 523; m < 547; m++)//因为c06在广播星历中有24个，需要一个个比较和哪一个最接近,序号从523到546
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
	//输出文件
	ofstream file_result;//创建输出文件流
	//创建输出文件地址
	string name = "/C6_position_get.txt";
	name = outfile + name;
	file_result.open(name);//自行修改文件路径
	//开始输出二维数组里的数值
	for (int i = 0; i < 96; i++)
	{
		file_result.flags(ios::fixed);
		file_result.precision(6);//设置保留小数位数，精确到mm，取6位
		file_result << C6_position_get[i][0] << "\t" << C6_position_get[i][1] << "\t" << C6_position_get[i][2] << "\n";
	}
	//关闭输出文件流
	file_result.close();
	cout << "已输出C06卫星广播星历得出坐标" << endl;
}

//从精密星历获得C06的位置
void C6_precise(string precisefile, string outfile)
{
	//读取数据
	fstream file;
	file.open(precisefile);//读入文件

	if (!file)//判断是否找到文件
	{
		cout << "文件地址错误，打开失败！" << endl;
	}
	double time[96][3];
	string buf;//临时存储数据的每一行，不用字符数组，不然后面不好断行
	string temp;//用来暂时储存数字
	int num = 0;//用来计数96个点读出多少个

	for (int i = 0; i < 10304; i++)
	{
		getline(file, buf);//每一次取一行的内容
		string title;//取卫星的名称
		title = buf.substr(0, 4);

		if (title == "PC06")//后面直接修改这里，这里是判断依据
		{
			time[num][0] = stod(buf.substr(5, 12));
			time[num][1] = stod(buf.substr(19, 12));
			time[num][2] = stod(buf.substr(33, 12));
			num++;
		}
	}

	//输出文件
	ofstream file_result;//创建输出文件流
	//创建输出文件地址
	string name = "/C6_position_precise.txt";
	name = outfile + name;
	file_result.open(name);//自行修改文件路径
	//开始输出二维数组里的数值
	for (int i = 0; i < 96; i++)
	{
		file_result.flags(ios::fixed);
		file_result.precision(6);//设置保留小数位数，精确到mm，取6位
		file_result << time[i][0] << "\t" << time[i][1] << "\t" << time[i][2] << "\n";
	}
	//关闭输出文件流
	file_result.close();
	cout << "已输出C06卫星精密星历得出坐标" << endl;
}

//从广播星历获得C07位置(IGSO)
void C7_position(parameter s[1329], string outfile)
{
	//输出卫星C07一天的位置（总体的第39颗星）
	//创建一天的时间序列（间隔为15分钟）
	//从0.00到23.45
	GPSTime onedaytime[96];
	double C7_position_get[96][3];
	int nearnum;//相距最近的卫星序号
	for (int i = 0; i < 96; i++)
	{
		double dif = 1000000;//实际时间和每一个参考时刻的差值
		CalendarTime tem;
		tem.year = 2019;
		tem.mon = 12;
		tem.day = 1;
		tem.hour = int(i / 4);
		tem.min = (i % 4) * 15;
		tem.sec = 0;
		onedaytime[i] = MJD2GPSTime(CalendarTime2MJD(tem));
		for (int m = 547; m < 570; m++)//因为c07在广播星历中有24个，需要一个个比较和哪一个最接近,序号从547到570
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
	//输出文件
	ofstream file_result;//创建输出文件流
	//创建输出文件地址
	string name = "/C7_position_get.txt";
	name = outfile + name;
	file_result.open(name);//自行修改文件路径
	//开始输出二维数组里的数值
	for (int i = 0; i < 96; i++)
	{
		file_result.flags(ios::fixed);
		file_result.precision(6);//设置保留小数位数，精确到mm，取6位
		file_result << C7_position_get[i][0] << "\t" << C7_position_get[i][1] << "\t" << C7_position_get[i][2] << "\n";
	}
	//关闭输出文件流
	file_result.close();
	cout << "已输出C07卫星广播星历得出坐标" << endl;
}

//从精密星历获得C07的位置
void C7_precise(string precisefile, string outfile)
{
	//读取数据
	fstream file;
	file.open(precisefile);//读入文件

	if (!file)//判断是否找到文件
	{
		cout << "文件地址错误，打开失败！" << endl;
	}
	double time[96][3];
	string buf;//临时存储数据的每一行，不用字符数组，不然后面不好断行
	string temp;//用来暂时储存数字
	int num = 0;//用来计数96个点读出多少个

	for (int i = 0; i < 10304; i++)
	{
		getline(file, buf);//每一次取一行的内容
		string title;//取卫星的名称
		title = buf.substr(0, 4);

		if (title == "PC07")//后面直接修改这里，这里是判断依据
		{
			time[num][0] = stod(buf.substr(5, 12));
			time[num][1] = stod(buf.substr(19, 12));
			time[num][2] = stod(buf.substr(33, 12));
			num++;
		}
	}

	//输出文件
	ofstream file_result;//创建输出文件流
	//创建输出文件地址
	string name = "/C7_position_precise.txt";
	name = outfile + name;
	file_result.open(name);//自行修改文件路径
	//开始输出二维数组里的数值
	for (int i = 0; i < 96; i++)
	{
		file_result.flags(ios::fixed);
		file_result.precision(6);//设置保留小数位数，精确到mm，取6位
		file_result << time[i][0] << "\t" << time[i][1] << "\t" << time[i][2] << "\n";
	}
	//关闭输出文件流
	file_result.close();
	cout << "已输出C07卫星精密星历得出坐标" << endl;
}

//从广播星历获得C20位置(MEO)
void C20_position(parameter s[1329], string outfile)
{
	//输出卫星C20一天的位置（总体的第51颗星）
	//创建一天的时间序列（间隔为15分钟）
	//从0.00到23.45
	GPSTime onedaytime[96];
	double C20_position_get[96][3];
	int nearnum;//相距最近的卫星序号
	for (int i = 0; i < 96; i++)
	{
		double dif = 1000000;//实际时间和每一个参考时刻的差值
		CalendarTime tem;
		tem.year = 2019;
		tem.mon = 12;
		tem.day = 1;
		tem.hour = int(i / 4);
		tem.min = (i % 4) * 15;
		tem.sec = 0;
		onedaytime[i] = MJD2GPSTime(CalendarTime2MJD(tem));
		for (int m = 821; m < 845; m++)//因为c20在广播星历中有24个，需要一个个比较和哪一个最接近,序号从821到844
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
	//输出文件
	ofstream file_result;//创建输出文件流
	//创建输出文件地址
	string name = "/C20_position_get.txt";
	name = outfile + name;
	file_result.open(name);//自行修改文件路径
	//开始输出二维数组里的数值
	for (int i = 0; i < 96; i++)
	{
		file_result.flags(ios::fixed);
		file_result.precision(6);//设置保留小数位数，精确到mm，取6位
		file_result << C20_position_get[i][0] << "\t" << C20_position_get[i][1] << "\t" << C20_position_get[i][2] << "\n";
	}
	//关闭输出文件流
	file_result.close();
	cout << "已输出C20卫星广播星历得出坐标" << endl;
}

//从精密星历获得C20的位置
void C20_precise(string precisefile, string outfile)
{
	//读取数据
	fstream file;
	file.open(precisefile);//读入文件

	if (!file)//判断是否找到文件
	{
		cout << "文件地址错误，打开失败！" << endl;
	}
	double time[96][3];
	string buf;//临时存储数据的每一行，不用字符数组，不然后面不好断行
	string temp;//用来暂时储存数字
	int num = 0;//用来计数96个点读出多少个

	for (int i = 0; i < 10304; i++)
	{
		getline(file, buf);//每一次取一行的内容
		string title;//取卫星的名称
		title = buf.substr(0, 4);

		if (title == "PC20")//后面直接修改这里，这里是判断依据
		{
			time[num][0] = stod(buf.substr(5, 12));
			time[num][1] = stod(buf.substr(19, 12));
			time[num][2] = stod(buf.substr(33, 12));
			num++;
		}
	}

	//输出文件
	ofstream file_result;//创建输出文件流
	//创建输出文件地址
	string name = "/C20_position_precise.txt";
	name = outfile + name;
	file_result.open(name);//自行修改文件路径
	//开始输出二维数组里的数值
	for (int i = 0; i < 96; i++)
	{
		file_result.flags(ios::fixed);
		file_result.precision(6);//设置保留小数位数，精确到mm，取6位
		file_result << time[i][0] << "\t" << time[i][1] << "\t" << time[i][2] << "\n";
	}
	//关闭输出文件流
	file_result.close();
	cout << "已输出C20卫星精密星历得出坐标" << endl;
}

//从广播星历获得C21位置(MEO)
void C21_position(parameter s[1329], string outfile)
{
	//输出卫星C21一天的位置（总体的第52颗星）
	//创建一天的时间序列（间隔为15分钟）
	//从0.00到23.45
	GPSTime onedaytime[96];
	double C21_position_get[96][3];
	int nearnum;//相距最近的卫星序号
	for (int i = 0; i < 96; i++)
	{
		double dif = 1000000;//实际时间和每一个参考时刻的差值
		CalendarTime tem;
		tem.year = 2019;
		tem.mon = 12;
		tem.day = 1;
		tem.hour = int(i / 4);
		tem.min = (i % 4) * 15;
		tem.sec = 0;
		onedaytime[i] = MJD2GPSTime(CalendarTime2MJD(tem));
		for (int m = 845; m < 869; m++)//因为c21在广播星历中有24个，需要一个个比较和哪一个最接近,序号从845到868
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
	//输出文件
	ofstream file_result;//创建输出文件流
	//创建输出文件地址
	string name = "/C21_position_get.txt";
	name = outfile + name;
	file_result.open(name);//自行修改文件路径
	//开始输出二维数组里的数值
	for (int i = 0; i < 96; i++)
	{
		file_result.flags(ios::fixed);
		file_result.precision(6);//设置保留小数位数，精确到mm，取6位
		file_result << C21_position_get[i][0] << "\t" << C21_position_get[i][1] << "\t" << C21_position_get[i][2] << "\n";
	}
	//关闭输出文件流
	file_result.close();
	cout << "已输出C21卫星广播星历得出坐标" << endl;
}

//从精密星历获得C21的位置
void C21_precise(string precisefile, string outfile)
{
	//读取数据
	fstream file;
	file.open(precisefile);//读入文件

	if (!file)//判断是否找到文件
	{
		cout << "文件地址错误，打开失败！" << endl;
	}
	double time[96][3];
	string buf;//临时存储数据的每一行，不用字符数组，不然后面不好断行
	string temp;//用来暂时储存数字
	int num = 0;//用来计数96个点读出多少个

	for (int i = 0; i < 10304; i++)
	{
		getline(file, buf);//每一次取一行的内容
		string title;//取卫星的名称
		title = buf.substr(0, 4);

		if (title == "PC21")//后面直接修改这里，这里是判断依据
		{
			time[num][0] = stod(buf.substr(5, 12));
			time[num][1] = stod(buf.substr(19, 12));
			time[num][2] = stod(buf.substr(33, 12));
			num++;
		}
	}

	//输出文件
	ofstream file_result;//创建输出文件流
	//创建输出文件地址
	string name = "/C21_position_precise.txt";
	name = outfile + name;
	file_result.open(name);//自行修改文件路径
	//开始输出二维数组里的数值
	for (int i = 0; i < 96; i++)
	{
		file_result.flags(ios::fixed);
		file_result.precision(6);//设置保留小数位数，精确到mm，取6位
		file_result << time[i][0] << "\t" << time[i][1] << "\t" << time[i][2] << "\n";
	}
	//关闭输出文件流
	file_result.close();
	cout << "已输出C21卫星精密星历得出坐标" << endl;
}

//从广播星历获得C01位置(GEO)
void C01_position(parameter s[1329], string outfile)
{
	//输出卫星C01一天的位置（总体的第33颗星）
	//创建一天的时间序列（间隔为15分钟）
	//从0.00到23.45
	GPSTime onedaytime[96];
	double C01_position_get[96][3];
	int nearnum;//相距最近的卫星序号
	for (int i = 0; i < 96; i++)
	{
		double dif = 1000000;//实际时间和每一个参考时刻的差值
		CalendarTime tem;
		tem.year = 2019;
		tem.mon = 12;
		tem.day = 1;
		tem.hour = int(i / 4);
		tem.min = (i % 4) * 15;
		tem.sec = 0;
		onedaytime[i] = MJD2GPSTime(CalendarTime2MJD(tem));
		for (int m = 394; m < 418; m++)//因为c01在广播星历中有24个，需要一个个比较和哪一个最接近,序号从394到417
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
	//输出文件
	ofstream file_result;//创建输出文件流
	//创建输出文件地址
	string name = "/C01_position_get.txt";
	name = outfile + name;
	file_result.open(name);//自行修改文件路径
	//开始输出二维数组里的数值
	for (int i = 0; i < 96; i++)
	{
		file_result.flags(ios::fixed);
		file_result.precision(6);//设置保留小数位数，精确到mm，取6位
		file_result << C01_position_get[i][0] << "\t" << C01_position_get[i][1] << "\t" << C01_position_get[i][2] << "\n";
	}
	//关闭输出文件流
	file_result.close();
	cout << "已输出C01卫星广播星历得出坐标" << endl;
}

//从精密星历获得C01的位置
void C01_precise(string precisefile, string outfile)
{
	//读取数据
	fstream file;
	file.open(precisefile);//读入文件

	if (!file)//判断是否找到文件
	{
		cout << "文件地址错误，打开失败！" << endl;
	}
	double time[96][3];
	string buf;//临时存储数据的每一行，不用字符数组，不然后面不好断行
	string temp;//用来暂时储存数字
	int num = 0;//用来计数96个点读出多少个

	for (int i = 0; i < 10304; i++)
	{
		getline(file, buf);//每一次取一行的内容
		string title;//取卫星的名称
		title = buf.substr(0, 4);

		if (title == "PC01")//后面直接修改这里，这里是判断依据
		{
			time[num][0] = stod(buf.substr(5, 12));
			time[num][1] = stod(buf.substr(19, 12));
			time[num][2] = stod(buf.substr(33, 12));
			num++;
		}
	}

	//输出文件
	ofstream file_result;//创建输出文件流
	//创建输出文件地址
	string name = "/C01_position_precise.txt";
	name = outfile + name;
	file_result.open(name);//自行修改文件路径
	//hjd
	//开始输出二维数组里的数值
	for (int i = 0; i < 96; i++)
	{
		file_result.flags(ios::fixed);
		file_result.precision(6);//设置保留小数位数，精确到mm，取6位
		file_result << time[i][0] << "\t" << time[i][1] << "\t" << time[i][2] << "\n";
	}
	//关闭输出文件流
	file_result.close();
	cout << "已输出C01卫星精密星历得出坐标" << endl;
}

//从广播星历获得C02位置(GEO)
void C02_position(parameter s[1329], string outfile)
{
	//输出卫星C02一天的位置（总体的第34颗星）
	//创建一天的时间序列（间隔为15分钟）
	//从0.00到23.45
	GPSTime onedaytime[96];
	double C02_position_get[96][3];
	int nearnum;//相距最近的卫星序号
	for (int i = 0; i < 96; i++)
	{
		double dif = 1000000;//实际时间和每一个参考时刻的差值
		CalendarTime tem;
		tem.year = 2019;
		tem.mon = 12;
		tem.day = 1;
		tem.hour = int(i / 4);
		tem.min = (i % 4) * 15;
		tem.sec = 0;
		onedaytime[i] = MJD2GPSTime(CalendarTime2MJD(tem));
		for (int m = 418; m < 442; m++)//因为c01在广播星历中有24个，需要一个个比较和哪一个最接近,序号从418到441
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
	//输出文件
	ofstream file_result;//创建输出文件流
	//创建输出文件地址
	string name = "/C02_position_get.txt";
	name = outfile + name;
	file_result.open(name);//自行修改文件路径
	//开始输出二维数组里的数值
	for (int i = 0; i < 96; i++)
	{
		file_result.flags(ios::fixed);
		file_result.precision(6);//设置保留小数位数，精确到mm，取6位
		file_result << C02_position_get[i][0] << "\t" << C02_position_get[i][1] << "\t" << C02_position_get[i][2] << "\n";
	}
	//关闭输出文件流
	file_result.close();
	cout << "已输出C02卫星广播星历得出坐标" << endl;
}

//从精密星历获得C02的位置
void C02_precise(string precisefile, string outfile)
{
	//读取数据
	fstream file;
	file.open(precisefile);//读入文件

	if (!file)//判断是否找到文件
	{
		cout << "文件地址错误，打开失败！" << endl;
	}
	double time[96][3];
	string buf;//临时存储数据的每一行，不用字符数组，不然后面不好断行
	string temp;//用来暂时储存数字
	int num = 0;//用来计数96个点读出多少个

	for (int i = 0; i < 10304; i++)
	{
		getline(file, buf);//每一次取一行的内容
		string title;//取卫星的名称
		title = buf.substr(0, 4);

		if (title == "PC02")//后面直接修改这里，这里是判断依据
		{
			time[num][0] = stod(buf.substr(5, 12));
			time[num][1] = stod(buf.substr(19, 12));
			time[num][2] = stod(buf.substr(33, 12));
			num++;
		}
	}

	//输出文件
	ofstream file_result;//创建输出文件流
	//创建输出文件地址
	string name = "/C02_position_precise.txt";
	name = outfile + name;
	file_result.open(name);//自行修改文件路径
	//开始输出二维数组里的数值
	for (int i = 0; i < 96; i++)
	{
		file_result.flags(ios::fixed);
		file_result.precision(6);//设置保留小数位数，精确到mm，取6位
		file_result << time[i][0] << "\t" << time[i][1] << "\t" << time[i][2] << "\n";
	}
	//关闭输出文件流
	file_result.close();
	cout << "已输出C02卫星精密星历得出坐标" << endl;
}