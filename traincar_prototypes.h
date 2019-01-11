#include <vector>
#include <stdlib.h>
void PushBack(TrainCar*& train, TrainCar* train2);

void PushFront(TrainCar*& train, TrainCar* train2);

std::vector<TrainCar*> ShipFreight(TrainCar*& engines, TrainCar*& freights, int minspeed, int maxcars);

void TotalWeightAndCountCars(TrainCar* start, int& total_weight, int& num_engines, int& num_freight_cars, int& num_passenger_cars, int& num_dining_cars, int& num_sleep_cars);

float CalculateSpeed(TrainCar* train);

float AverageDistanceToDiningCar(TrainCar* train);

int ClosestEngineToSleeperCar(TrainCar* train);

TrainCar* PopFront(TrainCar*& train);

void DeleteAllCars(TrainCar* train);

TrainCar* PopBack(TrainCar*& train);

void Separate(TrainCar*& train, TrainCar*& first, TrainCar*& second);

void SeparateStatistics(std::vector<int>& original, std::vector<int>& left, std::vector<int>& right, int& unlinks, int& links, int& shifts);