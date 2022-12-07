#include "association.h"
#include "kf.h"
#include <iostream>
using Eigen::MatrixXd;
using Eigen::VectorXd;
using namespace std;


Association::Association() {
    is_initialized_ = false;
	H_ = MatrixXd(2, 4);
	H_ << 1, 0, 0, 0,
		0, 1, 0, 0;

	F_ = MatrixXd(4, 4);
	F_ << 1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1;
	P_ = MatrixXd(4, 4);
	P_ << 1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1000, 0,
		0, 0, 0, 1000;

	R_ = MatrixXd(2, 2);
	R_ << 0.0225, 0,
		0, 0.0225;

	

}

Association::~Association() {

}




void Association::Associat(const MeasurementPackage& measurement_pack, const VectorXd& X, const MatrixXd& P) {

	
	  x_ = VectorXd(4);
	  x_ << X;

	//  cout << "assocai_X = " << X << endl;

	  P_ = MatrixXd(4, 4);
	
	  //cout << "assocai_P = " << P << endl;

	 P_ << P;
	 float dt = 200 / 1000000.0;
	 F_(0, 2) = dt;
	 F_(1, 3) = dt;

	VectorXd z_pred = H_ * x_;
    VectorXd y = (measurement_pack.raw_measurements_ )- z_pred;
	MatrixXd Ht = H_.transpose();
    VectorXd yt = y.transpose();
	MatrixXd S = H_ * P_ * Ht + R_;
	MatrixXd Si = S.inverse();

	  a =y.transpose() * Si * y;

	//  cout << "assocai_a = " << a << endl;
}