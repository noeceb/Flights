/*
 * CS61C Summer 2019
 * Name: Noe Cebreros
 * Login: cs61c-afh
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "flights.h"
#include "flight_structs.h"
#include "timeHM.h"

// prototypes
void printScheduleHelper(flight_t *schedule); 
void deleteAirport(airport_t *link);

/*
 *  This should be called if memory allocation failed.
 */
static void allocation_failed(void) {
  fprintf(stderr, "Out of memory.\n");
  exit(EXIT_FAILURE);
}

/*
 *  Creates and initializes a flight system, which stores the flight schedules of several airports.
 *  Returns a pointer to the system created.
 */
flightSys_t* createSystem(void) {
  flightSys_t *flightSystem = (flightSys_t*)malloc(sizeof(flightSys_t));
  if(flightSystem == NULL) {
    allocation_failed();
  }
  flightSystem -> airportList = NULL; // from adding a Node to the List lecture 4 slides. Must be able to handle a NULL input. Points to null for new system.
  return flightSystem;
}


/*
 *   Given a destination airport, departure and arrival times, and a cost, return a pointer to new flight_t.
 *
 *   Note that this pointer must be available to use even after this function returns.
 *   (What does this mean in terms of how this pointer should be instantiated?)
 *   Additionally you CANNOT assume that the `departure` and `arrival` pointers will persist after this function completes.
 *   (What does this mean about copying dep and arr?)
 */
flight_t* createFlight(airport_t* destination, timeHM_t* departure, timeHM_t* arrival, int cost) {
  if (cost >= 0 && arrival && departure && destination && 
    isAfter(arrival, departure)) {
        flight_t *flight = (flight_t*) malloc(sizeof(flight_t)); // gets memory
        flight -> arrivalTime = (timeHM_t *) malloc(sizeof(timeHM_t));
        flight -> timeDeparture = (timeHM_t *) malloc(sizeof(timeHM_t));
        if(!flight || !flight -> arrivalTime || !flight -> timeDeparture) {
          allocation_failed();
        }

        /* initialize my values in linked list */
        flight -> destination = destination;
        flight -> flightCost = cost;
        flight -> next = NULL; // next link will point to null (end of list).

        memcpy(flight -> timeDeparture, departure, sizeof(timeHM_t));
        memcpy(flight -> arrivalTime, arrival, sizeof(timeHM_t));

        return flight; // returns pointer to the address of memory for flight
  }
  return NULL;
 }
/*
 *  Frees all memory associated with this system; that's all memory you dynamically allocated in your code.
 */
void deleteSystem(flightSys_t* system) {
  if(system) { 
    deleteAirport(system->airportList); // deletes the system's airport list.
  }
  free(system);
}

// Deletes a linked airport (list)
void deleteAirport(airport_t *link) {
  airport_t *next = NULL;
  // free elements of airport : airportName, nextFlights, airportsToBeNext (link)
  while (link) {
    next = link->airportsToBeNext; // save next link
    free(link->airportName);     // free the name
    deleteFlight(link->nextFlights); // free the flights
    free(link); // free airport
    link = next; // move ptr to the next link.
  }
}
/*
 *  Frees all memory allocated for a single flight. You may or may not decide
 *  to use this in delete system but you must implement it.
 */
void deleteFlight(flight_t* flight) {
  flight_t* next = NULL;

  while (flight) { // assuming destinations are taken careof in flightSys
    next = flight->next; // next link into the flight list.
    free(flight -> timeDeparture);  // struct elements were not dynamic
    free(flight -> arrivalTime); // ^
    free(flight); 
    flight = next;
  }
}


/*
 *  Adds a airport with the given name to the system. You must copy the string and store it.
 *  Do not store `name` (the pointer) as the contents it point to may change.
 */
void addAirport(flightSys_t* system, char* name) {
  if(system == NULL || name == NULL) {
    return;
  }

  airport_t *airportToAdd = (airport_t*)malloc(sizeof(airport_t)); // allocate mem for link
  if(airportToAdd == NULL) {
    allocation_failed(); // check allocation
  } 

  airportToAdd -> airportName = (char*) malloc(strlen(name)+1); // gets memory for new airport plus 1 for null
  if (airportToAdd -> airportName == NULL) {
    allocation_failed(); // check allocation of the name.
  }
  strcpy(airportToAdd -> airportName,name);  //strcopy(destination, source)

  airportToAdd -> airportsToBeNext = NULL;
  airportToAdd ->nextFlights = NULL;

  if (system -> airportList == NULL) {
    system -> airportList = airportToAdd; // first link?
  } else {
    airport_t *ptr = system -> airportList; // ptr to not lose my pointer to the head of the list
    while(ptr) { // already did first check to see if there's a link
      if (ptr->airportsToBeNext != NULL) {
        ptr = ptr -> airportsToBeNext; 
      } else {
        break;
      }
    }  
    ptr -> airportsToBeNext = airportToAdd; // this was null.
  }
} 

/*
 *  Returns a pointer to the airport with the given name.
 *  If the airport doesn't exist, return NULL.
 */
airport_t* getAirport(flightSys_t* system, char* name) {
  if (system) {
    if(name == NULL) {
      return NULL;
    }
    airport_t *ptr = system -> airportList;
    while(ptr != NULL) {
      if (strcmp(ptr -> airportName, name) == 0) {
        return ptr;
      } 
      ptr = ptr -> airportsToBeNext; // continues the while loop
    }
  }
  return NULL; //if it reaches this statement then the airport name we want is not in the system.
}

/*
 *  Print each airport name in the order they were added through addAirport, one on each line.
 *  Make sure to end with a new line. You should compare your output with the correct output
 *  in `flights.out` to make sure your formatting is correct.
 */
void printAirports(flightSys_t* system) {
  if(system == NULL) {
    return;
  }
  airport_t *ptr = system -> airportList;
  while(ptr) {
    printf("%s\n", ptr -> airportName );
    ptr = ptr -> airportsToBeNext;
  }
}
/*
 *  Adds a flight to source's schedule, stating a flight will leave to destination at departure time and arrive at arrival time.
 */
void addFlight(airport_t* source, airport_t* destination, timeHM_t* departure, timeHM_t* arrival, int cost) {
  if(source == NULL || destination == NULL) {
    return;
  }

  if (departure == NULL || arrival == NULL) {
    return;
  }

  if (cost < 0) {
    return;
  }
  flight_t *generateFlight = createFlight(destination, departure, arrival, cost);
  if (source -> nextFlights == NULL) {
    source -> nextFlights = generateFlight;
    return; 
  }
  flight_t *ptr = source -> nextFlights;
  while(ptr) {
    if(ptr -> next != NULL) {
      ptr = ptr -> next;
    } else {
      break;
    }
  }
  ptr -> next = generateFlight;
}
/*
 *  Prints the schedule of flights of the given airport.
 *
 *  Prints the airport name on the first line, then prints a schedule entry on each
 *  line that follows, with the format: "destination_name departure_time 88arrival_time $cost_of_flight".
 *
 *  You should use `printTime()` (look in `timeHM.h`) to print times, and the order should be the same as
 *  the order they were added in through addFlight. Make sure to end with a new line.
 *  You should compare your output with the correct output in flights.out to make sure your formatting is correct.
 */
void printSchedule(airport_t* airport) {
  if(airport == NULL) {
    return;
  }
  printf("%s\n", airport -> airportName); // airport name
  printScheduleHelper(airport -> nextFlights);
}

void printScheduleHelper(flight_t *schedule) {
  if(schedule == NULL) {
    return;
  }
  printf("%s ", schedule -> destination -> airportName); // destination name
  printTime(schedule -> timeDeparture); // departure time
  printf(" ");
  printTime(schedule -> arrivalTime); // arrival time
  printf(" $%d\n", schedule -> flightCost); // cost of flight 
  printScheduleHelper(schedule -> next); // goes next in my linked list by using recursion

}
/*
 *   Given a source and destination airport, and the time now, finds the next flight to take based on the following rules:
 *   1) Finds the earliest arriving flight from source to destination that departs after now.
 *   2) If there are multiple earliest flights, take the one that costs the least.
 *
 *   If a flight is found, you should store the flight's departure time, arrival time, and cost in the `departure`, `arrival`,
 *   and `cost` params and return true. Otherwise, return false.
 *
 *   Please use the functions `isAfter()` and `isEqual()` from `timeHM.h` when comparing two timeHM_t objects and compare
 *   the airport names to compare airports, not the pointers.
 */
bool getNextFlight(airport_t* source, airport_t* destination, timeHM_t* now, timeHM_t* departure, timeHM_t* arrival,
                   int* cost) {

if(source && destination && now && departure && arrival && cost) {
    
  flight_t *flightGiven = source -> nextFlights; // flight that was given as argument through the function
  
  flight_t *earliestArrivingFlight = NULL; // my program will decide which flight to point to. For now its set to null.

  flight_t *ptrNext = flightGiven -> next; //pointer to the second item in the linked list
  
  // Base line comparison, first flight is best so far.
  if((strcmp(flightGiven -> destination -> airportName, destination -> airportName) == 0) && isAfter(flightGiven -> timeDeparture, now)) {
    earliestArrivingFlight = flightGiven;
  } 

  while (ptrNext) {
    // basic check
    if(ptrNext -> destination -> airportName == NULL || destination -> airportName == NULL) {
      return false;
    }
    // meets first criterion
    if((strcmp(flightGiven -> destination -> airportName, destination -> airportName) == 0) && isAfter(flightGiven -> timeDeparture, now)) {
      // If there is no baseline, assign one now.
      if (earliestArrivingFlight == NULL) {
        earliestArrivingFlight = ptrNext;
      } else {
        if (isAfter((earliestArrivingFlight -> arrivalTime), (ptrNext -> arrivalTime))) {
          earliestArrivingFlight = ptrNext; // optimal time thus far.
        } else if(isEqual(earliestArrivingFlight -> arrivalTime, ptrNext -> arrivalTime)) {
          if (earliestArrivingFlight -> flightCost < 0 || ptrNext -> flightCost < 0) { // check if costs are less than 0. If so return false
            return false;
          }
          if(earliestArrivingFlight -> flightCost > ptrNext -> flightCost) {
           earliestArrivingFlight = ptrNext; // pick cheapest flight. second criterion.
          }
        }
      }
    }
    ptrNext = ptrNext -> next;
    }
    if (earliestArrivingFlight) {
      *departure = *earliestArrivingFlight -> timeDeparture;
      *arrival = *earliestArrivingFlight -> arrivalTime;
      *cost = earliestArrivingFlight -> flightCost;
     return true;
   } 
  } 
  return false;
}
/*
 *  Given a list of flight_t pointers (`flight_list`) and a list of destination airport names (`airport_name_list`),
 *  first confirm that it is indeed possible to take these sequences of flights, (i.e. be sure that the i+1th flight departs
 *  after or at the same time as the ith flight arrives) (HINT: use the `isAfter()` and `isEqual()` functions).
 *  Then confirm that the list of destination airport names match the actual destination airport names of the provided flight_t structs.
 *
 *  `size` tells you the number of flights and destination airport names to consider. Be sure to extensively test for errors.
 *  As one example, if you encounter NULL's for any values that you might expect to be non-NULL return -1, but test for other possible errors too.
 *
 *  Return from this function the total cost of taking these sequence of flights.
 *  If it is impossible to take these sequence of flights,
 *  if the list of destination airport names doesn't match the actual destination airport names provided in the flight_t struct's,
 *  or if you run into any errors mentioned previously or any other errors, return -1.
 */
int validateFlightPath(flight_t** flight_list, char** airport_name_list, int size) {
  if(!flight_list || !airport_name_list) {
    return -1;
  }
 if(size < 0) {
    return -1;
  }

  int totalCost = 0;
  int i = 0;
  while(i < size) {
    flight_t *flightGiven = flight_list[i];
    char *airportGiven = airport_name_list[i];

    if (flightGiven == NULL || airportGiven == NULL) {
      return -1;
    }
    if(strcmp(flightGiven->destination->airportName, airportGiven) != 0) {
      return -1;
    }
    if (i < size - 1) {
      flight_t* flightPlusOne = flight_list[i + 1];
      timeHM_t* arrival = flightPlusOne -> arrivalTime;
      timeHM_t* depart = flightGiven -> timeDeparture;
      if (!isEqual(depart, arrival) && !isAfter(arrival, depart)) {
        return -1;
      }

    } 
    totalCost += flightGiven -> flightCost;
    i++;
  }
  return totalCost; 
}