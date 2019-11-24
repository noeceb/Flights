# Flights

<h3> Introduction </h3>

You will be completing the implementation of flights_structs.h and flights.c, a flight system that keeps track of the flights between a series of airports. The flight system, represented by the struct flightSys_t, will hold all the airports in this system. Each airport, represented by the struct airport_t, will hold both its name (as a string) and a schedule of all the flights departing from it. Each entry in the schedule should contain:

- Item a pointer to the destination airport,
- Item time of departure,
- Item time of arrival,
- Item and the cost of the flight.

These will be the contents of your flight_t struct. We have provided a program, RouteTime.c, which will both provide the data to your flight system and use the data you store to figure out the cost of flying via a certain route. You do not need to know how RouteTime.c works to complete the assignment. We have also provided you a struct, timeHM_t, defined in timeHM.h that is used to represent time in hours and minutes. It also contains several useful functions.
