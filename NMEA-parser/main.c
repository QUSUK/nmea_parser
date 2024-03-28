#include "inc/nmea-parser.h"
#include <stdio.h>
#include <time.h>
#include <windows.h>
uint8_t RxBuff[1024] = "$GNGGL,022014.001,A,5550.608252,N,03732.248871,E,0.1000,092.9,010518,15.00000,0.0,A,V*0F$GNGGA,040717.00,5550.610226,S,03732.250730,W,9,18,0.6,00187.429,M,0014.442,M,,*7B$GNRMC,022014.001,A,5550.213422,S,02132.248871,W,0.1000,092.9,010518,15.00000,0.0,A,V*08$GNGGA,040717.00,5550.610226,S,03732.250730,W,9,18,0.6,00187.429,M,0014.442,M,,*7B$GNRMC,022014.001,A,5550.608252,N,03732.248871,E,0.1000,092.9,010518,15.00000,0.0,A,V*0F$GNGGA,040717.00,5550.610226,S,03732.250730,W,9,18,0.6,00187.429,M,0014.442,M,,*7B$GNRMC,022014.001,A,5550.608252,N,03732.248871,E,0.1000,092.9,010518,15.00000,0.0,A,V*0F$GNGGA,040717.00,5550.610226,S,03732.250730,W,9,18,0.6,00187.429,M,0014.442,M,,*7B";

//uint8_t RxBuff_[1024] = "$GNGGA,153556.00,5550.602948,N,03732.239612,E,4,15,0.7,00192.245,M,0014.4,M,000.6,0100*58$GNRMC,153556.00,A,5550.602948,N,03732.239612,E,000.00000,000.0,310518,,,R*50$GPRMC,125504.049,A,5542.2389,N,03741.6063,E,0.06,25.82,200906,,,*17 ";

int main()
{

	NMEA GPSData;
	Init_NMEA_Structs(&GPSData);
	
	
	LARGE_INTEGER frequency, start, end;
	double elapsed;

	QueryPerformanceFrequency(&frequency);
	QueryPerformanceCounter(&start);


	NMEA_Parse(RxBuff, &GPSData);

	QueryPerformanceCounter(&end);
	elapsed = (double)(end.QuadPart - start.QuadPart) / frequency.QuadPart;
	printf("\n\ntime: %.10lf sec\n", elapsed );
	//print_RMC(&GPSData);
	//print_GGA(&GPSData);
	//NMEA_Parse(RxBuff_, &GPSData);
	print_RMC(&GPSData);
	print_GGA(&GPSData);
	Free_NMEA_Struct(&GPSData);
	return (0);
}