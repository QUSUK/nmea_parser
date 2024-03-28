#include "nmea-parser.h"


void NMEA_Parse(uint8_t* Rx_Buff, NMEA* NMEA_Struct)
{
	uint8_t* Token = (uint8_t*)strtok((char*)Rx_Buff, "$");
	while (Token != NULL)
	{
		printf("%s\n", Token);
		char Message_Type[7];

		if (sscanf((const char*)Token, "%6[^,]", Message_Type) == 0)
		{
			Token = (uint8_t*)strtok(NULL, (const char*)"$");
			continue;
		}

		Message_Type[6] = '\0';

		if ((strcmp(Message_Type, "GNRMC") == 0) || (strcmp(Message_Type, "GPRMC") == 0))
		{
			if (Compare_Checksum((const char*)Token))
			{
				NMEA_Parse_RMC((uint8_t*)Token, NMEA_Struct);
			}
		}
		else if (strcmp(Message_Type, "GNGGA") == 0)
		{
			if (Compare_Checksum((const char*)Token))
			{
				NMEA_Parse_GGA((uint8_t*)Token, NMEA_Struct);
			}
		}

		Token = (uint8_t*)strtok(NULL, (const char*)"$");
	}
}


uint8_t NMEA_Parse_RMC(const uint8_t* NMEA_Data, NMEA* NMEA_Struct)
{
	memset(&NMEA_Struct->RMCData, 0, sizeof(struct NMEA_RMC));

	uint8_t	Num_Scan_Chars = 0;
	uint8_t* Delim = (uint8_t*)",";
	uint8_t* Token = (uint8_t*)strstr((const char*)NMEA_Data, (const char*)Delim);

	for (uint8_t Counter = 0; Token != NULL; Counter++)
	{
		*Token = '\0';

		switch (Counter)
		{
		case 1:
			Num_Scan_Chars += (uint8_t)sscanf((const char*)NMEA_Data, "%2s%2s%2s.%3s",
				NMEA_Struct->RMCData.h,
				NMEA_Struct->RMCData.m,
				NMEA_Struct->RMCData.s,
				NMEA_Struct->RMCData.ms
			);
			break;
		case 2:
			Num_Scan_Chars += (uint8_t)sscanf((const char*)NMEA_Data, "%c", &NMEA_Struct->RMCData.Status);
			break;
		case 3:
			Num_Scan_Chars += (uint8_t)sscanf((const char*)NMEA_Data, "%f", &NMEA_Struct->RMCData.Latitude);
			break;
		case 4:
			Num_Scan_Chars += (uint8_t)sscanf((const char*)NMEA_Data, "%c", &NMEA_Struct->RMCData.NS);
			break;
		case 5:
			Num_Scan_Chars += (uint8_t)sscanf((const char*)NMEA_Data, "%f", &NMEA_Struct->RMCData.Longitude);
			break;
		case 6:
			Num_Scan_Chars += (uint8_t)sscanf((const char*)NMEA_Data, "%c", &NMEA_Struct->RMCData.EW);
			break;
		case 7:
			Num_Scan_Chars += (uint8_t)sscanf((const char*)NMEA_Data, "%f", &NMEA_Struct->RMCData.Speed);
			break;
		case 8:
			break;
		case 9:
			Num_Scan_Chars += (uint8_t)sscanf((const char*)NMEA_Data, "%2s%2s%2s",
				NMEA_Struct->RMCData.D,
				NMEA_Struct->RMCData.M,
				NMEA_Struct->RMCData.Y
			);
			break;
		}
		NMEA_Data = Token + 1;
		Token = (uint8_t*)strstr((const char*)NMEA_Data, (const char*)Delim);
	}

	Convert_Coords_To_Degs(NMEA_Struct->RMCData.Deg, &NMEA_Struct->RMCData.Latitude, &NMEA_Struct->RMCData.Longitude,
		NMEA_Struct->RMCData.NS, NMEA_Struct->RMCData.EW);

	NMEA_Struct->RMCData.Speed *= (float)1.852;

	return Num_Scan_Chars;
}



uint8_t NMEA_Parse_GGA(const uint8_t* NMEA_Data, NMEA* NMEA_Struct)
{
	memset(&NMEA_Struct->GGAData, 0, sizeof(struct NMEA_GGA));

	uint8_t	Num_Scan_Chars = 0;
	uint8_t* Delim = (uint8_t*)",";
	uint8_t* Token = (uint8_t*)strstr((const char*)NMEA_Data, (const char*)Delim);

	for (uint8_t Counter = 0; Token != NULL; Counter++)
	{
		*Token = '\0';

		switch (Counter)
		{
		case 1:
			Num_Scan_Chars += (uint8_t)sscanf((const char*)NMEA_Data, "%2s%2s%2s.%3s",
				NMEA_Struct->GGAData.h,
				NMEA_Struct->GGAData.m,
				NMEA_Struct->GGAData.s,
				NMEA_Struct->GGAData.ms
			);
			break;
		case 2:
			Num_Scan_Chars += (uint8_t)sscanf((const char*)NMEA_Data, "%f", &NMEA_Struct->GGAData.Latitude);
			break;
		case 3:
			Num_Scan_Chars += (uint8_t)sscanf((const char*)NMEA_Data, "%c", &NMEA_Struct->GGAData.NS);
			break;
		case 4:
			Num_Scan_Chars += (uint8_t)sscanf((const char*)NMEA_Data, "%f", &NMEA_Struct->GGAData.Longitude);
			break;
		case 5:
			Num_Scan_Chars += (uint8_t)sscanf((const char*)NMEA_Data, "%c", &NMEA_Struct->GGAData.EW);
			break;
		case 6:
			break;
		case 7:
			break;
		case 8:
			Num_Scan_Chars += (uint8_t)sscanf((const char*)NMEA_Data, "%f", &NMEA_Struct->GGAData.HDOP);
			break;
		case 9:
			Num_Scan_Chars += (uint8_t)sscanf((const char*)NMEA_Data, "%f", &NMEA_Struct->GGAData.Height);
			break;
		case 10:
			Num_Scan_Chars += (uint8_t)sscanf((const char*)NMEA_Data, "%c", &NMEA_Struct->GGAData.Height_Unit);
			break;
		}
		NMEA_Data = Token + 1;
		Token = (uint8_t*)(char*)strstr((const char*)NMEA_Data, (const char*)Delim);
	}

	Convert_Coords_To_Degs(NMEA_Struct->GGAData.Deg, &NMEA_Struct->GGAData.Latitude, &NMEA_Struct->GGAData.Longitude,
		NMEA_Struct->GGAData.NS, NMEA_Struct->GGAData.EW);

	return Num_Scan_Chars;
}


uint16_t NMEA_Check_Sum_Calc(const char* NMEA_Data)
{
	uint16_t Checksum = 0;
	char i = 0;

	while (NMEA_Data[i] != '*' && NMEA_Data[i] != '\0')
	{
		Checksum ^= NMEA_Data[i];
		i++;
	}
	printf("Checksum: %X\n", Checksum);

	return Checksum;
}


uint8_t Compare_Checksum(const char* nmea_data)
{
	uint16_t Checksum = NMEA_Check_Sum_Calc(nmea_data);
	int32_t Expected_Checksum;
	char* Checksum_Str = strchr(nmea_data, '*');

	if (Checksum_Str == NULL)
		return 0;

	if (sscanf(Checksum_Str + 1, "%x", &Expected_Checksum) != 1)
		return 0;

	return Checksum == Expected_Checksum;
}


void Convert_Coords_To_Degs(int8_t* Degs, float* Latitude, float* Longitude, uint8_t NS, uint8_t EW)
{
	float Lat_Deg, Lon_Deg;

	Lat_Deg = (float)((int)*Latitude / 100 + fmod(*Latitude, 100) / 60.0);
	Lat_Deg = (NS == 'N') ? +Lat_Deg : -Lat_Deg;

	Lon_Deg = (float)((int)*Longitude / 100 + (float)fmod(*Longitude, 100) / 60.0);
	Lon_Deg = (EW == 'E') ? +Lon_Deg : -Lon_Deg;

	sprintf((char*)Degs, "%.8lf %.8lf", Lat_Deg, Lon_Deg);
}


void print_RMC(NMEA* NMEA_Struct)
{
	if (&NMEA_Struct->RMCData == NULL)
	{
		printf("RMC is NULL\n");
		return;
	}
	printf("\n========================================================================\n");
	printf("Command RMC:\n");
	printf("h: %s\n",					NMEA_Struct->RMCData.h);
	printf("m: %s\n",					NMEA_Struct->RMCData.m);
	printf("s: %s\n",					NMEA_Struct->RMCData.s);
	printf("ms: %s\n",					NMEA_Struct->RMCData.ms);
	printf("Status: %c\n",				NMEA_Struct->RMCData.Status);
	printf("Latitude: %f\n",			NMEA_Struct->RMCData.Latitude);
	printf("NS: %c\n",					NMEA_Struct->RMCData.NS);
	printf("Longitude: %f\n",			NMEA_Struct->RMCData.Longitude);
	printf("EW: %c\n",					NMEA_Struct->RMCData.EW);
	printf("Speed: %f\n",				NMEA_Struct->RMCData.Speed);
	printf("Y: %s\n",					NMEA_Struct->RMCData.Y);
	printf("M: %s\n",					NMEA_Struct->RMCData.M);
	printf("D: %s\n",					NMEA_Struct->RMCData.D);
	printf("Deg: %s\n",					NMEA_Struct->RMCData.Deg);
	printf("========================================================================\n");
}



void print_GGA(NMEA* NMEA_Struct)
{
	if (&NMEA_Struct->GGAData == NULL)
	{
		printf("GGA is NULL\n");
		return;
	}
	printf("\n========================================================================\n");
	printf("Command GGA:\n");
	printf("h: %s\n",					NMEA_Struct->GGAData.h);
	printf("m: %s\n",					NMEA_Struct->GGAData.m);
	printf("s: %s\n",					NMEA_Struct->GGAData.s);
	printf("ms: %s\n",					NMEA_Struct->GGAData.ms);
	printf("Latitude: %f\n",			NMEA_Struct->GGAData.Latitude);
	printf("NS: %c\n",					NMEA_Struct->GGAData.NS);
	printf("Longitude: %f\n",			NMEA_Struct->GGAData.Longitude);
	printf("EW: %c\n",					NMEA_Struct->GGAData.EW);
	printf("HDOP: %f\n",				NMEA_Struct->GGAData.HDOP);
	printf("Height: %f\n",				NMEA_Struct->GGAData.Height);
	printf("HeightUnit: %c\n",			NMEA_Struct->GGAData.Height_Unit);
	printf("Deg: %s\n",					NMEA_Struct->GGAData.Deg);
	printf("========================================================================\n");
}

