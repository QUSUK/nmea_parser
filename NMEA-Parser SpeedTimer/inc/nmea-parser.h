#pragma once
#pragma warning(disable : 4996)

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <memory.h>
#include <string.h>
#include <math.h>

typedef struct NMEA_Struct
{
	struct NMEA_RMC* RMCData;
	struct NMEA_GGA* GGAData;
}NMEA;


struct NMEA_RMC
{
	uint8_t		Y[3];
	uint8_t		M[3];
	uint8_t		D[3];
	uint8_t		h[3];
	uint8_t		m[3];
	uint8_t		s[3];
	uint8_t		ms[4];
	uint8_t		Status;
	double		Latitude;
	uint8_t		NS;
	double		Longitude;
	uint8_t		EW;
	double		Speed;
	int8_t		Deg[128];
};

struct NMEA_GGA
{
	uint8_t		h[3];
	uint8_t		m[3];
	uint8_t		s[3];
	uint8_t		ms[4];
	double		Latitude;
	uint8_t		NS;
	double		Longitude;
	uint8_t		EW;
	uint8_t		Satellites;
	double		HDOP;
	double		Height;
	uint8_t		Height_Unit;
	int8_t		Deg[128];
};

void Init_NMEA_Structs(NMEA* NMEA_Struct);
void NMEA_Parse(uint8_t* Rx_Buff, NMEA* NMEA_Struct);
uint8_t NMEA_Parse_RMC(const uint8_t* NMEA_Data, NMEA* NMEA_Struct);
uint8_t NMEA_Parse_GGA(const uint8_t* NMEA_Data, NMEA* NMEA_Struct);
uint16_t NMEA_Check_Sum_Calc(const char* NMEA_Data);
uint8_t Compare_Checksum(const char* Str);
void Convert_Coords_To_Degs(int8_t* Degs, double* Latitude, double* Longitude, uint8_t NS, uint8_t EW);
void print_RMC(NMEA* NMEA_Struct);
void print_GGA(NMEA* NMEA_Struct);
void Free_NMEA_Struct(NMEA* NMEA_Struct);

