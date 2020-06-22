#pragma once
//用来声明
//声明函数
void readeph(parameter s[1329], string broadcastfile);
MJD CalendarTime2MJD(CalendarTime calT);
GPSTime MJD2GPSTime(MJD mjd);
double calculate_x(GPSTime dstGPST, parameter ep);
double calculate_y(GPSTime dstGPST, parameter ep);
double calculate_z(GPSTime dstGPST, parameter ep);
void G1_position(parameter s[1329], string outfile);
void G1_precise(string precisefile, string outfile);
void G2_position(parameter s[1329], string outfile);
void G2_precise(string precisefile, string outfile);
void G3_position(parameter s[1329], string outfile);
void G3_precise(string precisefile, string outfile);
void C6_position(parameter s[1329], string outfile);
void C6_precise(string precisefile, string outfile);
void C7_position(parameter s[1329], string outfile);
void C7_precise(string precisefile, string outfile);
void C20_position(parameter s[1329], string outfile);
void C20_precise(string precisefile, string outfile);
void C21_position(parameter s[1329], string outfile);
void C21_precise(string precisefile, string outfile);
void C01_position(parameter s[1329], string outfile);
void C01_precise(string precisefile, string outfile);
void C02_position(parameter s[1329], string outfile);
void C02_precise(string precisefile, string outfile);