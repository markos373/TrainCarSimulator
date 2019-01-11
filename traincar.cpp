#include <iostream>
#include <iomanip>
#include <string>
#include <cassert>
#include <cmath>
#include <cstdlib>
#include <vector>
#include "traincar.h"

//Given the linked list and element to be added to the end; my algorithm finds the end of the linked list
//and points train's last element to train2, and vice versa.
void PushBack(TrainCar*& train, TrainCar* train2){
    TrainCar* temp = train;
    //NULL check
    if(train == NULL){
        train = train2;
        return;
    }
    //Reach the end of train
    while(temp->next != NULL){
        temp = temp->next;
    }
    //Assign pointers
    temp->next = train2;
    train2->prev = temp;
}
//Given the linked list and element to add to the front, my algorithm iterates to the beginning of train and
//inserts train2 before the first element in train, with pointers reassigned.
void PushFront(TrainCar*& train, TrainCar* train2){
    //NULL handler
    if(train == NULL){
        train = train2;
        train->prev = NULL;
        train->next = NULL;
        return;
    }
    //train2 NULL handler
    if(train2 == NULL){
        return;
    }
    TrainCar* temp = train;
    //Iterate to beginning
    while(temp->prev != NULL){
        temp = temp->prev;
    }
    //Reassign pointers to properly adjust linked list
    temp->prev = train2;
    train = train2;
    train2->prev = NULL;
    train2->next = temp;
}
//This function returns the last element of a train
TrainCar* PopBack(TrainCar*& train){
    //NULL handler
    if(train == NULL){
        return NULL;
    }
    TrainCar* temp = train;
    //Reach the end of train to find which element to pop off
    while(temp->next != NULL){
        temp = temp->next;
    }

    temp->prev->next = NULL;
    temp->prev = NULL;
    return temp;
}

//Same as PopBack, but finds beginning of train (if not there already) to pop first element off.
TrainCar* PopFront(TrainCar*& train){
    if(train == NULL){
        return NULL;
    }
    
    TrainCar* temp = train->next;
    if(train->next == NULL){
        TrainCar* temp2 = train;
        train = NULL;
        return temp2;
    }
    train->next->prev = NULL;
    train->next = NULL;
    TrainCar* ret = train;
    train = temp;
    return ret;
}
//Recursively delete the passed linked list
void DeleteAllCars(TrainCar* a){
    //Base case, when the end of linked list has been reached
    if(a == NULL){
        return;
    }
    //Self call to delete next element
    DeleteAllCars(a->next);
    delete a;
}


//This function edits referenced values of total weight, and each type of car on the train
void TotalWeightAndCountCars(TrainCar* start, int& total_weight, int& num_engines, int& num_freight_cars, int& num_passenger_cars, int& num_dining_cars, int& num_sleep_cars){
    //Initialize referenced variables to 0
    total_weight = 0; num_engines = 0; num_freight_cars = 0; 
    num_passenger_cars = 0; num_dining_cars = 0; num_sleep_cars = 0;

    //Iterate through entire train
    while(start != NULL){
        //Increment each type found
        if(start->isDiningCar()) num_dining_cars++;
        if(start->isEngine()) num_engines++;
        if(start->isFreightCar()) num_freight_cars++;
        if(start->isSleepingCar()) num_sleep_cars++;
        if(start->isPassengerCar()) num_passenger_cars++; 
        //Continuous summation of weight
        total_weight += start->getWeight();
        start = start->next;
    }

}

//My ShipFreight algorithm creates as many single engine trains as possible, and then combines each combination of trains and
//checks whether it is below the max car count, and above the minimum speed.  Trains with too many cars are PopBack'ed onto
//freight and later reassigned to the trains after being combined, again, rechecking speed and max car count.
std::vector<TrainCar*> ShipFreight(TrainCar*& engines, TrainCar*& freight, int minspeed, int maxcars){
    std::vector<TrainCar*> trains;
    std::vector<TrainCar*> optimized_trains;
    
    while(freight != NULL){

        TrainCar* single_engine_train = PopFront(engines);

        float speed = CalculateSpeed(single_engine_train);
        //Add as many freight trains to one engine until it is below minspeed
        while((speed >= minspeed) && (freight != NULL)){
            PushBack(single_engine_train, PopFront(freight));
            speed = CalculateSpeed(single_engine_train);
        }

        if(freight != NULL){
            PushFront(freight, PopBack(single_engine_train));
        }
        trains.push_back(single_engine_train);
        if(engines == NULL){
            break;
        }
    }

    while(trains.size() > 1){
        unsigned int j = 1;
        unsigned int i = 0;
        TrainCar* temp = trains[i];
        TrainCar* temp_end = temp;
        TrainCar* temp2 = trains[j];
        //Find the end of trains[i], reassign pointers from trains[j] to create one linked list
        //that would look like trains[i](end)->trains[j];
        while(temp_end->next != NULL){
            temp_end = temp_end->next;
            }
            
        temp_end->next = temp2;
        temp2->prev = temp_end;

        int total_weight,num_engines,num_freight_cars,num_passenger_cars,num_dining_cars,num_sleeping_cars;
        TotalWeightAndCountCars(temp,total_weight,num_engines,num_freight_cars,num_passenger_cars
                                ,num_dining_cars,num_sleeping_cars);
        int car_count = num_engines+num_freight_cars;

        if((CalculateSpeed(temp) >= minspeed) && (car_count <= maxcars)){
            optimized_trains.push_back(temp);
            //Erase the two trains that were combined
            trains.erase(trains.begin(), trains.begin()+2);
        }else{
            while(car_count > maxcars){
                //Remove cars from temp and add them back to freight until car_count is within bounds
                PushBack(freight,PopBack(temp));
                car_count--;
            }
            optimized_trains.push_back(temp);
            trains.erase(trains.begin(), trains.begin()+2);
            
        }
    }

    unsigned int trainIndex = 0;
    while(freight != NULL){

        if(trainIndex == optimized_trains.size()){
            break;
        }

        float speed = CalculateSpeed(optimized_trains[trainIndex]);

        int total_weight,num_engines,num_freight_cars,num_passenger_cars,num_dining_cars,num_sleeping_cars;
        TotalWeightAndCountCars(optimized_trains[trainIndex],total_weight,num_engines,num_freight_cars,num_passenger_cars
                                ,num_dining_cars,num_sleeping_cars);
        int car_count = num_engines+num_freight_cars;

        //Add one freight to optimized_trains[trainIndex]) and see if it is valid under given specifications.
        while((speed >= minspeed) && (car_count < maxcars) && (freight != NULL)){
            PushBack(optimized_trains[trainIndex], PopFront(freight));
            car_count++;
            speed = CalculateSpeed(optimized_trains[trainIndex]);
        }
        if(freight != NULL && speed < minspeed){
            PushBack(freight, PopBack(optimized_trains[trainIndex]));
        }
        trainIndex++;
    }
    return optimized_trains;
}

//Arithmetic calculation of speed using formula given,
//all values found as float to avoid integer division inaccuracies.
//Engine count and total weight is found by iterating through the whole train first.
float CalculateSpeed(TrainCar* train){
    float engines = 0; float total_weight = 0;

    while(train != NULL){

        if(train->isEngine()){
            engines++;
        }
        total_weight += train->getWeight();
        train = train->next;

    }

    float total_hp = (engines * 3000);
    float num = total_hp * 550 * 3600;
    float denom = 20 * 2 * 5280 * total_weight;

    return num/denom;
}

//This function iterates through train and finds the position of each
//passenger train, and when a passenger car is found, iterator moves
//through the linked list from that position until a dining car is found
float AverageDistanceToDiningCar(TrainCar* train){
    float passengerCars = 0;
    float totalDistance = 0;

    //Only works if dining car is after passenger car... 
    //Couldn't figure out how to reverse.
    TrainCar* iterator;
    while(train->next != NULL){
        if(train->isPassengerCar()){
            
            iterator = train;
            while(!iterator->isDiningCar()){
                if(iterator->next == NULL){
                    break;
                }
                iterator = iterator->next;
                totalDistance++;
            }
        passengerCars++;
        }
        train = train->next;
    }

    return totalDistance/passengerCars;
}

//This function traverses train in front->back->front order.  This is achieved by two iterations through train, with the 
//end of train being found first and then using that position to reverse back through train.  Engines/sleeping cars found
//have their position recorded in iterator and move through/back the list until its' counterpart is found.
int ClosestEngineToSleeperCar(TrainCar* train){

    int counter = 0;
    int closest = 100;
    TrainCar* iterator;
    //Iterate through train
    while(train->next != NULL){

        if(train->isEngine()){
            //Record position of engine and iterate with iterator until sleeping car is found
            iterator = train;
            while(!iterator->isSleepingCar()){
                if(iterator->next == NULL){
                    break;
                }
                iterator = iterator->next;
                counter++;
            }
        //Compare each engine->sleepingcar distance with the current lowest distance.
        closest = (counter < closest) ? counter : closest;
        counter = 0;
        }
        train = train->next;
    }

    //Go backwards through train.
    while(train->prev != NULL){
        if(train->isEngine()){
            iterator = train;
            while(!iterator->isSleepingCar()){
                if(iterator->prev == NULL){
                    break;
                }
                iterator = iterator->prev;
                counter++;
            }
            closest = (counter < closest) ? counter : closest;
            counter = 0;
        }
        train = train->prev;
    }
    return closest;
}

//Separate function works by evenly distributing engines and then evenly distributing 
//the cars on the train.  The last car assigned is outside of the first while loop to avoid
//segmentation faults.  I was unable to properly implement the matching of speeds between
//the two parts of the trains, but my algorithm was to remove the last car of whichever train
//was slower, and add it to the other train until the speed difference was within 5.
void Separate(TrainCar*& train, TrainCar*& first, TrainCar*& second){
    bool alternate = true;
    bool engine_alternate = false;
    first = NULL;
    second = NULL;
    float max_speed_difference = 5;
    while(train->next != NULL){
        if(train->isEngine()){
            //Evenly distribute engines
            if(engine_alternate){    
            PushBack(first, PopFront(train));
            engine_alternate = false;
            }else{
            PushBack(second, PopFront(train));
            engine_alternate = true;
            }    

        }

        if(alternate){
            PushBack(first, PopFront(train));
            alternate = false;
        }else{
            PushBack(second, PopFront(train));
            alternate = true;
        }
    }
    if(alternate){
        PushBack(first, PopFront(train));
        alternate = false;
    }else{
          PushBack(second, PopFront(train));
        alternate = true;
    }
    float first_speed = CalculateSpeed(first);
    float second_speed = CalculateSpeed(second);
    if(abs(first_speed - second_speed) < max_speed_difference){
        return;
    }
}

//I didn't really understand how to find unlinks, links, and shifts.  I was able to check for duplicates and missing cars through iterating
//through left and right, and comparing values against original values.
void SeparateStatistics(std::vector<int>& original, std::vector<int>& left, std::vector<int>& right, int& unlinks, int& links, int& shifts){
    unsigned int countInstances = 0;
    unlinks = 0; links = 0; shifts = 0;
    for(unsigned int i = 0; i < left.size(); i++){
        for(unsigned int j = 0; j < original.size(); j++){
            if(left[i] == original[j]){
                countInstances++;
            }
        }
    }
    for(unsigned int i = 0; i < right.size(); i++){
        for(unsigned int j = 0; j < original.size(); j++){
            if(right[i] == original[j]){
                countInstances++;
            }
        }
    }
    if(countInstances != original.size()){
        std::cout << "Duplicates or missing cars are in the original train" << std::endl;
    }

}