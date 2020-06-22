#include <iostream>
#include <string.h>
#include <sstream>
#include <fstream>
#include <string>
#include "parameter.h"
#include "statement.h"
using namespace std;

//声明函数
MJD CalendarTime2MJD(CalendarTime calT);
GPSTime MJD2GPSTime(MJD mjd);
double calculate_x(GPSTime dstGPST, parameter ep);
double calculate_y(GPSTime dstGPST, parameter ep);
double calculate_z(GPSTime dstGPST, parameter ep);

void readeph(parameter s[1329],string broadcastfile)
{
	//读取数据
	string fileName = broadcastfile;
	fstream file;
	file.open(fileName);//读入文件

	if (!file)//判断是否找到文件
	{
		cout << "文件地址错误，打开失败！" << endl;
	}

	string buf;//临时存储数据的每一行，不用字符数组，不然后面不好断行
	int realnumber = 0;//实际的文件总行数
	int number = 0;//开始读文件体的小循环，因为一个结构有8行，这个用于判断到了哪一行
	int order = -1;//卫星序号数，用于计数有效卫星的数量，同时为对象数组计数，因为一会要加一，而数组从0开始，这里取-1
	int length = 0;//为后面字符串转字符数组做准备
	string blank = "               ";//15个长度的空字符串，用于与后面的比较，不然空的字符串没法赋值
	int broadcastnum = 0;//卫星的整体序号数
	string PRN;//用来留存每一个的PRN号，方便数到底有多少卫星

	//处理文件头程序
	while (1)//直接进循环，开始读文件
	{
		getline(file, buf);//每一次取一行的内容
		realnumber++;//实际的总行数加一
		//判断文件头是否读完，当读完文件头后跳出循环
		if (buf.find("END OF HEADER") == 60)
			break;
	}

	//进入读文件体程序
	for (realnumber; realnumber < 77943; )
	{
		getline(file, buf);//每一次取一行的内容
		realnumber++;//实际的总行数加一
		if (realnumber < 70464)
			number = (realnumber - 11) % 8;//开始读文件体的小循环，因为一个结构有8行，这个用于判断到了哪一行
		else
			number = (realnumber - 70463) % 8;//开始读文件体的小循环，因为一个结构有8行，这个用于判断到了哪一行，到了70463之后就开始是C开头的卫星了
		//先判断开头，只读入C和G开头的卫星
		string initial_judge;//取出开头的第一个字母
		initial_judge = buf.substr(0, 1);
		if (initial_judge == "S")
		{
			realnumber = realnumber + 3;
			for (int i = 0; i < 3; i++)
			{
				getline(file, buf);//往下一行
			}
			continue;
		}
		if (initial_judge == "R")
		{
			realnumber = realnumber + 3;
			for (int i = 0; i < 3; i++)
			{
				getline(file, buf);//往下一行
			}
			continue;
		}
		if (initial_judge == "E")
		{
			realnumber = realnumber + 7;
			for (int i = 0; i < 7; i++)
			{
				getline(file, buf);//往下一行
			}
			continue;
		}
		switch (number)
		{
		case (1)://余数是1，第一行的内容
		{
			//卫星序号数加一
			order++;
			//第一行的参数
			string PRN_num;//PRN号
			string year;//年
			string month;//月
			string day;//日
			string hour;//时
			string min;//分
			string sec;//秒
			string SClockBias;//卫星钟的偏差主体
			string SClockBias_power;//卫星钟的偏差的幂
			string SClockDri;//卫星钟的漂移主体
			string SClockDri_power;//卫星钟的漂移的幂
			string SClockDriV;//卫星钟的漂移速度主体
			string SClockDriV_power;//卫星钟的漂移速度的幂
			//开始截取字符串
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
			//开始赋值
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
		case (2)://余数是2，第二行的内容
		{
			//第二行的参数
			string IODE;//数据、星历发布时间主体
			string IODE_power;//数据、星历发布时间幂
			string Crs;//主体
			string Crs_power;//幂
			string deltan;//主体
			string deltan_power;//幂
			string M0;//主体
			string M0_power;//幂
			//开始截取字符串
			IODE = buf.substr(4, 15);
			IODE_power = buf.substr(20, 3);
			Crs = buf.substr(23, 15);
			Crs_power = buf.substr(39, 3);
			deltan = buf.substr(42, 15);
			deltan_power = buf.substr(58, 3);
			M0 = buf.substr(61, 15);
			M0_power = buf.substr(77, 3);
			//开始赋值
			s[order].IODE = stod(IODE) * pow(10, stod(IODE_power));
			s[order].Crs = stod(Crs) * pow(10, stod(Crs_power));
			s[order].deltan = stod(deltan) * pow(10, stod(deltan_power));
			s[order].M0 = stod(M0) * pow(10, stod(M0_power));
			break;
		}
		case (3)://余数是3，第三行的内容
		{
			//第三行的参数
			string Cuc;//主体
			string Cuc_power;//幂
			string e;//轨道偏心率主体
			string e_power;//轨道偏心率幂
			string Cus;//主体
			string Cus_power;//幂
			string sqrtA;//主体
			string sqrtA_power;//幂
			Cuc = buf.substr(4, 15);
			Cuc_power = buf.substr(20, 3);
			e = buf.substr(23, 15);
			e_power = buf.substr(39, 3);
			Cus = buf.substr(42, 15);
			Cus_power = buf.substr(58, 3);
			sqrtA = buf.substr(61, 15);
			sqrtA_power = buf.substr(77, 3);
			//开始赋值
			s[order].Cuc = stod(Cuc) * pow(10, stod(Cuc_power));
			s[order].e = stod(e) * pow(10, stod(e_power));
			s[order].Cus = stod(Cus) * pow(10, stod(Cus_power));
			s[order].sqrtA = stod(sqrtA) * pow(10, stod(sqrtA_power));
			break;
		}
		case (4)://余数是4，第四行的内容
		{
			//第四行的参数
			string TOE;//星历的参考时刻主体
			string TOE_power;//星历的参考时刻主体幂
			string Cic;//主体
			string Cic_power;//幂
			string omega;//主体
			string omega_power;//幂
			string Cis;//主体
			string Cis_power;//幂
			TOE = buf.substr(4, 15);
			TOE_power = buf.substr(20, 3);
			Cic = buf.substr(23, 15);
			Cic_power = buf.substr(39, 3);
			omega = buf.substr(42, 15);
			omega_power = buf.substr(58, 3);
			Cis = buf.substr(61, 15);
			Cis_power = buf.substr(77, 3);
			//开始赋值
			s[order].TOE = stod(TOE) * pow(10, stod(TOE_power));
			s[order].Cic = stod(Cic) * pow(10, stod(Cic_power));
			s[order].omega = stod(omega) * pow(10, stod(omega_power));
			s[order].Cis = stod(Cis) * pow(10, stod(Cis_power));
			break;
		}
		case (5)://余数是5，第五行的内容
		{
			//第五行的参数
			string i0;//主体
			string i0_power;//幂
			string Crc;//主体
			string Crc_power;//主体
			string omega1;//主体
			string omega1_power;//幂
			string dOmega;//主体
			string dOmega_power;//幂
			i0 = buf.substr(4, 15);
			i0_power = buf.substr(20, 3);
			Crc = buf.substr(23, 15);
			Crc_power = buf.substr(39, 3);
			omega1 = buf.substr(42, 15);
			omega1_power = buf.substr(58, 3);
			dOmega = buf.substr(61, 15);
			dOmega_power = buf.substr(77, 3);
			//开始赋值
			s[order].i0 = stod(i0) * pow(10, stod(i0_power));
			s[order].Crc = stod(Crc) * pow(10, stod(Crc_power));
			s[order].omega1 = stod(omega1) * pow(10, stod(omega1_power));
			s[order].dOmega = stod(dOmega) * pow(10, stod(dOmega_power));
			break;
		}
		case (6)://余数是6，第六行的内容
		{
			//第六行的参数
			string di;//（IDOT）主体
			string di_power;//（IDOT）幂
			string l2_code;//L2上的码主体
			string l2_code_power;//L2上的码幂
			string wn;//GPS周数（与TOE一同表示时间）主体
			string wn_power;//GPS周数（与TOE一同表示时间）幂
			string l2_P_tag;//L2上P码数据标记主体
			string l2_P_tag_power;//L2上P码数据标记幂
			di = buf.substr(4, 15);
			di_power = buf.substr(20, 3);
			l2_code = buf.substr(23, 15);
			l2_code_power = buf.substr(39, 3);
			wn = buf.substr(42, 15);
			wn_power = buf.substr(58, 3);
			l2_P_tag = buf.substr(61, 15);
			l2_P_tag_power = buf.substr(77, 3);
			//开始赋值
			s[order].di = stod(di) * pow(10, stod(di_power));
			s[order].l2_code = stod(l2_code) * pow(10, stod(l2_code_power));
			s[order].wn = stod(wn) * pow(10, stod(wn_power));
			s[order].l2_P_tag = stod(l2_P_tag) * pow(10, stod(l2_P_tag_power));
			break;
		}
		case (7)://余数是7，第七行的内容
		{
			//第七行的参数
			string SatAccur;//卫星精度(m)主体
			string SatAccur_power;//卫星精度(m)幂
			string SatState;//卫星健康状态主体
			string SatState_power;//卫星健康状态幂
			string TGD;//(sec)主体
			string TGD_power;//(sec)幂
			string IODC;//钟的数据龄期主体
			string IODC_power;//钟的数据龄期幂
			SatAccur = buf.substr(4, 15);
			SatAccur_power = buf.substr(20, 3);
			SatState = buf.substr(23, 15);
			SatState_power = buf.substr(39, 3);
			TGD = buf.substr(42, 15);
			TGD_power = buf.substr(58, 3);
			IODC = buf.substr(61, 15);
			IODC_power = buf.substr(77, 3);
			//开始赋值
			s[order].SatAccur = stod(SatAccur) * pow(10, stod(SatAccur_power));
			s[order].SatState = stod(SatState) * pow(10, stod(SatState_power));
			s[order].TGD = stod(TGD) * pow(10, stod(TGD_power));
			s[order].IODC = stod(IODC) * pow(10, stod(IODC_power));
			break;
		}
		case (0)://余数是0，第八行的内容
		{
			//第八行的参数
			string SendTime;//电文发送时刻（单位为GPS周的秒，通过交接字（HOW）中的Z计数得出）主体
			string SendTime_power;//电文发送时刻（单位为GPS周的秒，通过交接字（HOW）中的Z计数得出）幂
			string FitRange;//拟合区间主体
			string FitRange_power;//拟合区间幂
			string spare1;//备用主体
			string spare1_power;//备用幂
			string spare2;//备用主体
			string spare2_power;//备用幂
			SendTime = buf.substr(4, 15);
			SendTime_power = buf.substr(20, 3);
			FitRange = buf.substr(23, 15);
			FitRange_power = buf.substr(39, 3);
			spare1 = buf.substr(42, 15);
			spare1_power = buf.substr(58, 3);
			spare2 = buf.substr(61, 15);
			spare2_power = buf.substr(77, 3);
			//开始赋值
			s[order].SendTime = stod(SendTime) * pow(10, stod(SendTime_power));//第八行第一个数一直存在，故不判断
			if (FitRange != blank)//只有不为全是空格的字符串才会赋值
			{
				s[order].FitRange = stod(FitRange) * pow(10, stod(FitRange_power));
			}
			else//否则赋值为0
			{
				s[order].FitRange = 0;
			}
			if (spare1 != blank)//只有不为全是空格的字符串才会赋值
			{
				s[order].spare1 = stod(spare1) * pow(10, stod(spare1_power));
			}
			else//否则赋值为0
			{
				s[order].spare1 = 0;
			}
			if (spare2 != blank)//只有不为全是空格的字符串才会赋值
			{
				s[order].spare2 = stod(spare2) * pow(10, stod(spare2_power));
			}
			else//否则赋值为0
			{
				s[order].spare2 = 0;
			}
			break;
		}
		default:
			cout << "无效的赋值" << endl;
		}
	}
	file.close();

	//文件读取完毕，开始给所有的卫星赋值计算位置
	for (int i = 0; i < 1329; i++)
	{
		//将卫星的时间赋值出来
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

	//显示函数
	for (int i = 0; i < 1329; i++)
	{
		//将卫星的时间和位置显示出来
		cout << "目前读取文件行数为：" << i + 1 << endl;
		cout << "该卫星在读取的GPS卫星和北斗卫星中的序列号为：" << s[i].broadcastnum << endl;
		cout << "该卫星的PRN码为" << s[i].PRN << endl;
		cout << "该卫星的时刻为：" << s[i].year << "." << s[i].month << "." << s[i].day << "." << s[i].hour << "." << s[i].min << "." << s[i].sec << endl;
		cout << "X：" << s[i].x << "   " << "Y：" << s[i].y << "   " << "Z：" << s[i].z << endl;
		cout << endl;
	}
}