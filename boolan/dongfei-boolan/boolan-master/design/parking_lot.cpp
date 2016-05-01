// define some constants
enum ErrorCode {
    NO_ERROR,
    ERROR
};

enum SpotType {
    COMPACT,
    SUV,
    RESERVED
};
#define NO_PARKING (-1)

class Spot {
public:
    bool     available;
    SpotType type;
};

class Vehicle {
private:
    int     length;
    int     width;
    bool    parked;
    Spot    *spot;
public:
    // omit some setters / getters
    // virtual function here because subclasses will have different behavior
    virtual SpotType getRequiredSpotType() = 0;
    // no need for virtual functions here because subclasses will have the same "behavior"
    bool isParked();
    void parkVehicle(Spot *s);  // park at spot S;
    Spot *removeVehicle();      // move the vehicle away, return parked spot
};

//every type of vehicle has default value of length and width;
class Motor:public Vehicle{};
class Car:public Vehicle{};
class SUV:public Vehicle{};


class Level {
private:
    vector<Spot> spots;
public:
    Spot *findASpot(Vehicle *v);    // find an available spot for a vehicle
                                                     // return NULL if all spots are taken
};

class ParkingLot {
private:
    vector<Level> levels;
    static ParkingLot *pInstance;
    unordered_map<Vehicle *, time_t> parkingInfo;

    ParkingLot();
    // Stop the compiler generating methods of copy the object
    ParkingLot(const ParkingLot &copy);    // Not Implemented
    ParkingLot& operator = (const ParkingLot & copy);    // Not Implemented

    time_t getCurrentTime();
    double calculateFee(Vehicle *v);

public:
    static ParkingLot *getInstance();
    // NOTE: vehicleEnter and leave is not thread safe!
    ErrorCode vehicleEnter(Vehicle *v);
    ErrorCode vehicleLeave(Vehicle *v, double *fee);
};

ErrorCode ParkingLot::vehicleEnter(Vehicle *v) {
    Spot *spot = NULL;
    for (int i = 0; i < levels.size();i++) {
        spot = levels[i].findASpot(v);
        if (spot)
            break;
    }
    if (!spot) {
        return ERROR;
    }
    v->parkVehicle(spot);
    spot->available = false;
    parkingInfo[v] = getCurrentTime();
    return NO_ERROR;
}

ErrorCode ParkingLot::vehicleLeave(Vehicle *v, double *fee) {
    *fee = 0;
    if (!v->isParked()) {
        return ERROR;
    }
    Spot *spot = v->removeVehicle();
    spot->available = true;
    *fee = calculateFee(v);
    parkingInfo.erase(v);
    return NO_ERROR;
}
