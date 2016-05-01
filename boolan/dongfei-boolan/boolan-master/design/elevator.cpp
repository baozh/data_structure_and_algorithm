// define some constants
dispatch_queue_t printingQueue;
enum ElevatorState {
    UP,           // moving up
    DOWN,    // moving down
    STAND    // stay at a level, waiting for request
};

class Elevator {
private:
    ElevatorState state;
    int id;
    int currentLevel;
    int maxLevel;
    int minLevel;
    vector<int> requests;    // floor requests from low level to high level
    pthread_mutex_t lock;
    pthread_t runThread;    // runLoop is called on this thread

    void mutexLock();       // utility methods to lock and unlock
    void mutexUnlock();

    bool needStop();        // check if need to stop on current floor
    void updateState();    // switch to STAND if no request pending.
                                      // go DOWN if currently going UP, and no further higher level requests
                                      // go UP if currently going DOWN, and no further lower level requests

    void move();    // move one floor up/down. Open door if needed. Update state
    void openDoor();    // open door and close door, delete request from array
    void runLoop();    // thread safe operation, main logic for elevator operation (move, open door)
    static void *elevatorProc(void *parameter);

public:
    Elevator(int id, int minLevel, int maxLevel);
    ~Elevator();

    ErrorCode addRequest(int newRequest);    // thread safe operation, caller adds a new stop request
    void startOperation();    // thread safe operation, start elevatorProc to activate elevator
    void stopOperation();    // thread safe operation, stop elevatorProc to deactivate elevator
    ElevatorState getState();    // thread safe operation, get elevator state
    int getCurrentLevel();    // thread safe operation, get elevator floor

#pragma -mark Test Methods
    void printRequests() {
        // caller should have the data lock
        dispatch_async(printingQueue, ^{
            cout << "Elevator " << id << " current requests: ";
            for (int i = 0 ; i < requests.size(); i++) {
                cout << requests[i] << ' ';
            }
            cout << endl;
        });
    }
};

#pragma -mark Private Methods

bool Elevator::needStop() {
    for (int i = 0; i < requests.size(); i++) {
        if (requests[i] == currentLevel) {
            return true;
        }
    }
    return false;
}

void Elevator::updateState() {
    switch (state) {
        case UP:
            if (requests.size() == 0) {
                state = STAND;
            } else {
                if (requests.back() < currentLevel) {
                    state = DOWN;
                }
            }
            break;
        case DOWN:
            if (requests.size() == 0) {
                state = STAND;
            } else {
                if (requests.front() > currentLevel) {
                    state = UP;
                }
            }
            break;
        default:
            break;
    }

}

void Elevator::move() {
    switch (state) {
        case UP:
            if (needStop()) {
                openDoor();
                // flip state
                updateState();
            } else {
                dispatch_async(printingQueue, ^{
                    cout << "Elevator " << id << " move from " << currentLevel
                    << " to " << currentLevel + 1 << endl;
                });
                currentLevel++;
            }
            break;

        case DOWN:
            if (needStop()) {
                openDoor();
                // flip state
                updateState();
            } else {
                dispatch_async(printingQueue, ^{
                    cout << "Elevator " << id << " move from " << currentLevel
                    << " to " << currentLevel - 1 << endl;
                });
                currentLevel--;
            }
            break;
        default:
            break;
    }
}

void Elevator::openDoor() {
    dispatch_async(printingQueue, ^{
        cout << "Elevator " << id << " arriving at " << currentLevel << endl;
        cout << "Elevator " << id << " door open" << endl;
    });
    for (vector<int>::iterator it = requests.begin(); it != requests.end(); ) {
        if (*it == currentLevel) {
            it = requests.erase(it);
        } else {
            ++it;
        }
    }
    dispatch_async(printingQueue, ^{
        cout << "Elevator " << id << " door close" << endl;
    });
    printRequests();
}

void Elevator::runLoop() {
    mutexLock();
    switch (state) {
        case UP:
        case DOWN:
            move();
            break;
        default:
            break;
    }
    mutexUnlock();
}

void *Elevator::elevatorProc(void *parameter) {
    Elevator *elevator = (Elevator *)parameter;
    while (1) {
        elevator->runLoop();
        usleep(4000000);
    }
    return NULL;
}

#pragma -mark Public Methods

Elevator::Elevator(int inID, int inMinLevel, int inMaxLevel) {
    state = STAND;
    id = inID;
    maxLevel = inMaxLevel;
    minLevel = inMinLevel;
    currentLevel = inMinLevel;
    pthread_mutex_init(&lock, NULL);
}

Elevator::~Elevator() {
    stopOperation();
    pthread_mutex_destroy(&lock);
}

void Elevator::mutexLock() {
    pthread_mutex_lock(&lock);
}

void Elevator::mutexUnlock() {
    pthread_mutex_unlock(&lock);
}

void Elevator::startOperation() {
    mutexLock();
    pthread_create(&runThread, NULL, elevatorProc, this);
    mutexUnlock();
}

void Elevator::stopOperation() {
    mutexLock();
    if (runThread) {
        pthread_cancel(runThread);
    }
    mutexUnlock();
}

ErrorCode Elevator::addRequest(int newRequest) {
    mutexLock();
    if (newRequest > maxLevel || newRequest < minLevel) {
        cerr << "Invalid request " << newRequest << endl;
        return ERROR;
    }
    if (requests.size() == 0) {
        requests.push_back(newRequest);
        if (newRequest > currentLevel) {
            dispatch_async(printingQueue, ^{
                cout << "Elevator " << id << " moving up" << endl;
            });
            state = UP;
        } else if (newRequest == currentLevel) {
            state = STAND;
            openDoor();
        } else {
            dispatch_async(printingQueue, ^{
                cout << "Elevator " << id << " moving down" << endl;
            });
            state = DOWN;
        }
        goto Done;
    }

    for (vector<int>::iterator i = requests.begin(); i != requests.end(); i++) {
        if (newRequest == *i) {
            goto Done;
        }
        if(*i > newRequest) {
            requests.insert(i, newRequest);
            goto Done;
        }
    }
    requests.push_back(newRequest);
Done:
    printRequests();
    mutexUnlock();
    return NO_ERROR;
}

ElevatorState Elevator::getState() {
    mutexLock();
    ElevatorState currentState = state;
    mutexUnlock();
    return currentState;
}

int Elevator::getCurrentLevel() {
    mutexLock();
    int level = currentLevel;
    mutexUnlock();
    return level;
}

class ElevatorBank {
private:
    int numberOfElevators;
    vector<Elevator> elevators;
    int calculateScore(int index, int level);
    bool isBetterNewCandidate(int currentCandidateIndex, int newCandidateIndex, int level);
    int selectAnElevator(int level);
public:
    ElevatorBank(int numberOfElevators, int minFloor, int maxFloor);
    ~ElevatorBank();
    ErrorCode startAnElevator(int index);       // index start from 0
    ErrorCode stopAnElevator(int index);        // index start from 0
    ErrorCode setARequest(int level);
};

#pragma -mark Private Methods
int ElevatorBank::calculateScore(int index, int level) {
    if (elevators[index].getState() == STAND) {
        return abs(elevators[index].getCurrentLevel() - level);
    }

    if (elevators[index].getCurrentLevel() > level
        && elevators[index].getState() == DOWN) {
        return elevators[index].getCurrentLevel() - level;
    }

    if (elevators[index].getCurrentLevel() < level
        && elevators[index].getState() == UP) {
        return level - elevators[index].getCurrentLevel();
    }
    return INT_MAX;
}

bool ElevatorBank::isBetterNewCandidate(int currentCandidateIndex, int newCandidateIndex, int level) {
    return calculateScore(currentCandidateIndex, level) > calculateScore(newCandidateIndex, level);
}

int ElevatorBank::selectAnElevator(int level) {
    int selectedIndex = 0;
    for (int i = 1; i < numberOfElevators; i++) {
        if (isBetterNewCandidate(selectedIndex, i, level)) {
            selectedIndex = i;
        }
    }
    return selectedIndex;
}

#pragma -mark Public Methods

ElevatorBank::ElevatorBank(int inNumberOfElevators, int minFloor, int maxFloor) {
    numberOfElevators = inNumberOfElevators;
    for (int i = 0; i < inNumberOfElevators; i++) {
        elevators.push_back(Elevator(i, minFloor, maxFloor));
    }
}

ElevatorBank::~ElevatorBank() {
    elevators.clear();
}

ErrorCode ElevatorBank::startAnElevator(int index) {
    if (index > numberOfElevators || index < 0) {
        cerr << "Index " << index << " invalid!" << endl;
        return ERROR;
    }
    elevators[index].startOperation();
    return NO_ERROR;
}

ErrorCode ElevatorBank::stopAnElevator(int index) {
    if (index > numberOfElevators || index < 0) {
        cerr << "Index " << index << " invalid!" << endl;
        return ERROR;
    }
    elevators[index].stopOperation();
    return NO_ERROR;
}

ErrorCode ElevatorBank::setARequest(int level) {
    int index = selectAnElevator(level);
    return elevators[index].addRequest(level);
}


int main() {
    printingQueue = dispatch_queue_create("elevatorBank.printingQueue", NULL);
    ElevatorBank elevatorBank = ElevatorBank(2, 1, 10);
    for (int i = 0; i < 2; i++) {
        elevatorBank.startAnElevator(i);
    }
    int request;
    while (cin >> request) {
        elevatorBank.setARequest(request);
    };
    return 0;
}

