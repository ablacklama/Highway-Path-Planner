#ifndef CARDATA_h
#define CARDATA_h
#include <vector>
#include <iostream>
#include <math.h>
#include <string>

using namespace std;

struct Car {
	bool init = false; 		//quick way of telling whether a car object is empty
	double id;				//unique identification
	double x;				//cars x value
	double y;				//cars y value
	double vx;				//cars velocity in the x direction
	double vy;				// cars velocity in the y direction
	double speed;			//speed of the car
	double s;				//cars frenet s coordinate
	double d;				//cars frenet d coordinate
	int lane;				//lane that the car is in
	double check_car_s;		//where the car will be once the current path is done
};

class CarData {
public:
	vector<Car> cars;			//list of cars from sensor fusion
	double ref_vel = 0;			//our speed
	int lane = 0;				//our current or desired lane
	bool change_lane = false;	//keeps track of the lane change state
	bool following = false;		//keeps track of the following state
	double follow_speed = 0.0;	//speed of the car ahead of us
	int lane_change_timeout = 0; //make sure we don't exicute two lange changes too quickly
	
	//return a vecort of all cars in a givin lane. NOTE: this isn't used in this code
	vector<Car> GetLane(int lane);

	//Process all cars from sensor fusion and add them to the cars list
	void NewCars(vector<vector<double>> sfus, int prev_size);

	//return the closest car in a given lane
	Car ClosestInLane(int lane, double car_s);

	//determain if there is a car in a given lane between the behind and infront values relative to your car.
	bool Safe(double car_s, int lane, double behind = -10.0, double infront = 20.0);

	//print information for debugging
	void Print(double car_s, int print_steps);

private:
	int t = 0;
};

void CarData::NewCars(vector<vector<double>> sfus, int prev_size) {
	cars.clear();
	for (int i = 0; i < sfus.size(); i++) {
		if (sfus[i][6] > 0.0 && sfus[i][6] < 12.0) {
			Car c;
			c.id = sfus[i][0];
			c.x = sfus[i][1];
			c.y = sfus[i][2];
			c.vx = sfus[i][3];
			c.vy = sfus[i][4];
			c.s = sfus[i][5];
			c.d = sfus[i][6];
			c.speed = sqrt(c.vx*c.vx + c.vy*c.vy);
			c.check_car_s = c.s + ((double)prev_size * .02*c.speed);
			c.lane = (int)floor(c.d / 4.0);
			c.init = true;
			cars.push_back(c);
		}
	}
}

vector<Car> CarData::GetLane(int lane) {
	vector<Car> temp;
	for (Car car : cars) {
		if (car.lane == lane)
			temp.push_back(car);

	}
	return temp;
}

Car CarData::ClosestInLane(int lane, double car_s) {
	double distance = 9999.0;
	Car close;
	for (Car car : cars) {
		if (car.lane == lane && car.check_car_s - car_s > 0 && car.check_car_s - car_s < distance) {
			
			distance = car.check_car_s - car_s;
			close = car;
		}
	}
	return close;
}

bool CarData::Safe(double car_s, int lane, double behind, double infront) {
	bool safe = true;
	for (Car car : cars) {
		if (car.lane == lane && car.check_car_s - car_s > behind && car.check_car_s - car_s < infront) {
			safe = false;
			break;
		}
	}
	return safe;
}
void CarData::Print(double car_s, int print_steps) {
	t++;
	if (t == print_steps) {
		
		t = 0;
		cout << "\tZERO\tONE\tTWO" << endl;
		cout << "safe:\t" << Safe(car_s, 0) << "\t" << Safe(car_s, 1) << "\t" << Safe(car_s, 2) << endl;
		Car ahead0 = ClosestInLane(0, car_s);
		double d0 = ahead0.check_car_s - car_s;
		Car ahead1 = ClosestInLane(1, car_s);
		double d1 = ahead1.check_car_s - car_s;
		Car ahead2 = ClosestInLane(2, car_s);
		
		double d2 = ahead2.check_car_s - car_s;
		cout << "close: " << ((ahead0.init) ?to_string(d0):"N/A") << "\t" << ((ahead1.init)? to_string(d1) : "N/A") << "\t" << ((ahead2.init)? to_string(d2) : "N/A") << endl << endl;
		
	}
	
}

#endif