#include <iostream>
#include <string.h>
#include <sstream>
#include <fstream>
#include <string>
#include "parameter.h"
#include "statement.h"
using namespace std;
int main()
{
	//声明所有的星历对象数组
	parameter s[1329];

	//规定文件地址
	string broadcastfile = "D:/学习要紧/GNSS/编程作业一/卫星数据/brdm3350.19p";//请自行修改广播星历文件位置
	string precisefile = "D:/学习要紧/GNSS/编程作业一/卫星数据/WUM0MGXFIN_20193350000_01D_15M_ORB.SP3";//请自行修改精密星历文件位置
	string outfile = "D:/学习要紧/GNSS/编程作业一/卫星数据/得出数据";//请自行修改输出文件位置

	//读取广播星历
	readeph(s, broadcastfile);//在控制台中显示广播星历中每颗卫星的位置等信息
	//导出位置文件
	G1_position(s, outfile);
	G1_precise(precisefile,outfile);
	G2_position(s, outfile);
	G2_precise(precisefile,outfile);
	G3_position(s, outfile);
	G3_precise(precisefile,outfile);
	C6_position(s, outfile);
	C6_precise(precisefile,outfile);
	C7_position(s, outfile);
	C7_precise(precisefile,outfile);
	C20_position(s, outfile);
	C20_precise(precisefile,outfile);
	C21_position(s, outfile);
	C21_precise(precisefile,outfile);
	C01_position(s, outfile);
	C01_precise(precisefile,outfile);
	C02_position(s, outfile);
	C02_precise(precisefile,outfile);
	return 0;
}
