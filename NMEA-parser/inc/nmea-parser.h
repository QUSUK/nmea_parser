#pragma once
#pragma warning(disable : 4996)

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <memory.h>
#include <string.h>
#include <math.h>

typedef struct NMEA_RMC
{
	uint8_t		Y[3];
	uint8_t		M[3];
	uint8_t		D[3];
	uint8_t		h[3];
	uint8_t		m[3];
	uint8_t		s[3];
	uint8_t		ms[4];
	uint8_t		Status;
	float		Latitude;
	uint8_t		NS;
	float		Longitude;
	uint8_t		EW;
	float		Speed;
	int8_t		Deg[128];
}RMC;

typedef struct NMEA_GGA
{
	uint8_t		h[3];
	uint8_t		m[3];
	uint8_t		s[3];
	uint8_t		ms[4];
	float		Latitude;
	uint8_t		NS;
	float		Longitude;
	uint8_t		EW;
	uint8_t		Satellites;
	float		HDOP;
	float		Height;
	uint8_t		Height_Unit;
	int8_t		Deg[128];
}GGA;


typedef struct NMEA_Struct
{
	RMC RMCData;
	GGA GGAData;
}NMEA;



void NMEA_Parse(uint8_t* Rx_Buff, NMEA* NMEA_Struct);
uint8_t NMEA_Parse_RMC(const uint8_t* NMEA_Data, NMEA* NMEA_Struct);
uint8_t NMEA_Parse_GGA(const uint8_t* NMEA_Data, NMEA* NMEA_Struct);
uint16_t NMEA_Check_Sum_Calc(const char* NMEA_Data);
uint8_t Compare_Checksum(const char* Str);
void Convert_Coords_To_Degs(int8_t* Degs, float* Latitude, float* Longitude, uint8_t NS, uint8_t EW);
void print_RMC(NMEA* NMEA_Struct);
void print_GGA(NMEA* NMEA_Struct);

