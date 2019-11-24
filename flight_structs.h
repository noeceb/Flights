/*
 * CS61C Summer 2019
 * Name: Noe Cebreros
 * Login: cs61c-afh
 */

#ifndef FLIGHT_STRUCTS_H
#define FLIGHT_STRUCTS_H

#include "timeHM.h"

typedef struct flightSys flightSys_t;
typedef struct airport airport_t;
typedef struct flight flight_t;

struct flightSys {
  // Place the members you think are necessary for the flightSys struct here.
	airport_t *airportList;
};

struct airport {
  // Place the members you think are necessary for the airport struct here.
	char *airportName;
	airport_t *airportsToBeNext;
	flight_t *nextFlights; //current flight
	//struct node *next; 
};

struct flight {
  // Place the members you think are necessary for the flight struct here.
	airport_t *destination;
	timeHM_t *arrivalTime;
	timeHM_t *timeDeparture;
	flight_t *next;	// link to the next flight
	int flightCost;

};

#endif

