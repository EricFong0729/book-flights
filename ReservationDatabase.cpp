// ReservationDatabase.cpp
// Member-function definitions for class ReservationDatabase.
#include <iostream>
#include <iomanip>
#include <fstream>
#include "ReservationDatabase.h" // ReservationDatabase class definition
#include "FlightSchedule.h" // FlightSchedule class definition

// ReservationDatabase default constructor loads flight reservations from the file Reservations.dat
ReservationDatabase::ReservationDatabase()
{
    loadReservations();
}

// ReservationDatabase destructor stores flight reservations into the file Reservations.dat
ReservationDatabase::~ReservationDatabase()
{
    storeReservations();
}

void ReservationDatabase::addReservation(Reservation reservation)
{
    reservations.push_back(reservation);
}

void ReservationDatabase::cancelReservation(string id, int n)
{
    //loadReservations();
    //fstream loadFile("Reservations.dat", ios::in | ios::out | ios::binary);
    //while (!loadFile.eof()) {
    //    loadFile.read(reinterpret_cast<char*>(&reservation), sizeof(Reservation));
    //}
    int count = 0;
    vector<Reservation>::iterator it = reservations.begin();
    for (; it != reservations.end(); ++it) {
        if (it->getId() == id) {
            count++;
            if (count == n) {
                it = reservations.erase(it);
                break;
            }
        }
    }
} // end function cancelReservation

bool ReservationDatabase::empty()
{
    return (reservations.size() == 0);
}

bool ReservationDatabase::existReservation(string id)
{
    vector<Reservation>::iterator it = reservations.begin();
    for (; it != reservations.end(); ++it) {
        if (it->getId() == id) {
            return true;
        }
    }
    return false;
}

void ReservationDatabase::display(string id)
{
    char ticketType[8][24] = { "", "Full Fare", "Child Fare", "Infant Fare",
                                        "Senior Citizen Fare", "Impaired Fare",
                                        "Impaired Companion Fare", "Military Fare" };

    int cnt = 1;
    vector<Reservation>::iterator rit = reservations.begin();
    for (rit = reservations.begin(); rit != reservations.end(); ++rit) {
        if (rit->getId() == id) {
            cout << endl << cnt << ". ";
            cout << "Ticket information:\n\n";
            cout << "Date: " << rit->getDate() << endl;
            cout << "Flight: B7-" << rit->getFlightNo() << endl << endl;

            FlightSchedule theFlightSchedule;
            int departureAirport = theFlightSchedule.getDepartureAirport(rit->getFlightNo());
            int arrivalAirport = theFlightSchedule.getArrivalAirport(rit->getFlightNo());

            cout << setw(9) << right << airportName[departureAirport] << " -> "
                << setw(9) << left << airportName[arrivalAirport] << endl;

            cout << setw(9) << right << theFlightSchedule.getDepartureTime(rit->getFlightNo()) << "    ";
            cout << setw(9) << left << theFlightSchedule.getArrivalTime(rit->getFlightNo()) << endl << endl;

            char ticketType[8][24] = { "", "Full Fare", "Child Fare", "Infant Fare",
                                                "Senior Citizen Fare",     "Impaired Fare",
                                                "Impaired Companion Fare", "Military Fare" };

            int total = 0;
            int fare;
            for (int i = 1; i <= 7; i++) {
                if (rit->getTicket(i) > 0) {
                    fare = fullFare[departureAirport][arrivalAirport] * discount[i] / 100;
                    total += (fare * rit->getTicket(i));
                    cout << right << setw(23) << ticketType[i] << "  TWD "
                        << setw(4) << fare << " x " << rit->getTicket(i) << endl;
                }
            }
            cout << "\nTotal: " << total << endl;
            cnt++;
        }
    }

}

void ReservationDatabase::loadReservations()
{
    fstream loadFile("Reservations.dat", ios::in | ios::out | ios::binary);
    Reservation reservation;
    loadFile.read(reinterpret_cast<char*>(&reservation), sizeof(Reservation));
    while (!loadFile.eof()) {
        reservations.push_back(reservation);
        loadFile.read(reinterpret_cast<char*>(&reservation), sizeof(Reservation));
    }
}

void ReservationDatabase::storeReservations()
{
    fstream saveFile("Reservations.dat", ios::in | ios::out | ios::binary);
    vector<Reservation>::iterator it = reservations.begin();
    saveFile.seekg(0, 0);
    for (; it != reservations.end(); ++it) {
        //const Reservation reservation = *it;
        saveFile.write(reinterpret_cast<const char*>(&*it), sizeof(Reservation));
    }
}