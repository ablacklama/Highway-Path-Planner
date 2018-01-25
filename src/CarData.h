#ifndef CARDATA_h
#define CARDATA_h
#include <vector>
#include <iostream>
#include <math.h>
#include <string>

using namespace std;

struct Car {
	bool init = false;
	double id;
	double x;
	double y;
	double vx;
	double vy;
	double speed;
	double s;
	double d;
	int lane;
	double check_car_s;
};

class CarData {
public:
	vector<Car> cars;
	double ref_vel = 0;
	int lane = 0;
	bool change_lane = false;
	bool following = false;
	double follow_speed = 0.0;
	int lane_change_timeout = 0;
	
	///get all cars in a lane
	vector<Car> GetLane(int lane);
	void NewCars(vector<vector<double>> sfus, int prev_size);
	Car ClosestInLane(int lane, double car_s);
	bool Safe(double car_s, int lane, double behind = -10.0, double infront = 20.0);
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
			//car must be present in both lanes
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
		//error starts lane 2 same as lane 1
		double d2 = ahead2.check_car_s - car_s;
		cout << "close: " << ((ahead0.init) ?to_string(d0):"N/A") << "\t" << ((ahead1.init)? to_string(d1) : "N/A") << "\t" << ((ahead2.init)? to_string(d2) : "N/A") << endl << endl;
		
	}
	
}

#endif