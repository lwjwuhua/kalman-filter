#ifndef ASSOCIATION_H_
#define ASSOCIATION_H_
#include "Dense"
#include "measurement_package.h"

using Eigen::MatrixXd;
using Eigen::VectorXd;

class Association {
public:



	/**
	 * Constructor
	 */
	Association();

	/**
	 * Destructor
	 */
	virtual ~Association();

	
	void Associat(const MeasurementPackage& measurement_pack, const VectorXd& X, const MatrixXd& P);
	bool is_initialized;
	
	VectorXd x_;

	// state covariance matrix
	MatrixXd P_;

	// state transistion matrix
	MatrixXd F_;

	// process covariance matrix
	MatrixXd Q_;

	// measurement matrix
	MatrixXd H_;

	// measurement covariance matrix
	MatrixXd R_;

	double a;

private:
	bool is_initialized_;
};




#endif  // ASSOCIATION_H_
