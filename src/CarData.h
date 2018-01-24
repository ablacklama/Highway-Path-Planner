#ifndef CARDATA_h
#define CARDATA_h
#include <vector>

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
	int lane = 2;
	bool change_lane = false;
	bool following = false;
	double follow_speed = 0.0;
	

	
	vector<Car> GetLane(int lane);
	void NewCars(vector<vector<double>> sfus, int prev_size);
	Car ClosestInLane(int lane, double car_s);
	bool Safe(double car_s, int lane, double behind = -10.0, double infront = 20.0);
};

void CarData::NewCars(vector<vector<double>> sfus, int prev_size) {
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
#endif