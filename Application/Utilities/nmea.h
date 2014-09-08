/*
  nmea.cpp - NMEA 0183 sentence decoding library for Wiring & Arduino
  Copyright (c) 2008 Maarten Lamers, The Netherlands.
  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef nmea_h
#define nmea_h

//#include "WConstants.h"
#include "Framework.h"
#include "math.h"

#define	ALL					0					// connect to all datatypes
#define	GPRMC				1					// connect only to GPRMC datatype
#define	MTR					1.0					// meters per meter
#define	KM					0.001				// kilometers per meter
#define	MI					0.00062137112		// miles per meter
#define	NM					0.00053995680		// nautical miles per meter
#define	PARSEC				0.000000000000		// parsecs per meter (approximation)
#define	MPS					0.51444444 			// meters-per-second in one knot
#define	KMPH				1.852  				// kilometers-per-hour in one knot
#define	MPH					1.1507794			// miles-per-hour in one knot
#define	KTS					1.0 				// knots in one knot
#define	LIGHTSPEED			0.000000001716		// lightspeeds in one knot
#define TWO_PI				2*M_PI

#define false 0
#define true  1

void 	nmea_init();
int		nmea_parse(char c);				// parse one character received from GPS; returns 1 when full sentence found w/ checksum OK, 0 otherwise
float	nmea_gprmc_utc();				// returns decimal value of UTC term in last full GPRMC sentence
char	nmea_gprmc_status();				// returns status character in last full GPRMC sentence ('A' or 'V')
float	nmea_gprmc_latitude();			// signed degree-decimal value of latitude terms in last full GPRMC sentence
float	nmea_gprmc_longitude();			// signed degree-decimal value of longitude terms in last full GPRMC sentence
float	nmea_gprmc_speed(float unit);	// speed-on-ground term in last full GPRMC sentence
float	nmea_gprmc_course();				// track-angle-made-good term in last full GPRMC sentence
float	nmea_gprmc_distance_to(float latitude, float longitude, float unit);	// returns distance from last-known GPRMC position to given position
float 	nmea_gprmc_course_to(float latitude, float longitude);			// returns initial course in degrees from last-known GPRMC position to given position
char*	nmea_sentence();						// returns last received full sentence as zero terminated string
int		nmea_terms();							// returns number of terms (including data type and checksum) in last received full sentence
char*	nmea_term(int t);					// returns term t of last received full sentence as zero terminated string
float	nmea_term_decimal(int t);	// returns the base-10 converted value of term[t] in last full sentence received
int		nmea_libversion();					// returns software version number of NMEA library

#endif
