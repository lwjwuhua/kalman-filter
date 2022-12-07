#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <map>
#include "Dense"
#include "measurement_package.h"
#include "tracking.h"
#include"association.h"
#include <cmath>

using Eigen::MatrixXd;
using Eigen::VectorXd;
using std::cout;
using std::endl;
using std::ifstream;
using std::ofstream;
using std::istringstream;
using std::string;
using std::vector;
using std::map;


int main() {

    /**
     * Set Measurements
     */
    vector<MeasurementPackage> measurement_pack_list;

    vector<VectorXd> prex_;
 
     vector <MatrixXd> predict_p ;

     vector<double> disa;
     vector<double> ::iterator iter ;
    // hardcoded input file with laser and radar measurements
  //  string in_file_name_ = "obj_pose-laser-radar-synthetic-input.txt";
    string in_file_name_ = "123456.txt";

    string out_file_name_ = "1234-output.txt";

    ifstream in_file(in_file_name_.c_str(), ifstream::in);
    ofstream out_file(out_file_name_.c_str(), ofstream::out);

    if (!in_file.is_open()) {
        cout << "Cannot open input file: " << in_file_name_ << endl;
    }

    string line;
    // set i to get only first 3 measurments


   //  Tracking tracking;

    
    
      int i = 0;
   

        while (getline(in_file, line) && (i <= 2200)) {

            MeasurementPackage meas_package;

            istringstream iss(line);
            string sensor_type;
            iss >> sensor_type; // reads first element from the current line
            int64_t timestamp;
            float a_;
            if (sensor_type.compare("L") == 0) {  // laser measurement
              // read measurements
                meas_package.sensor_type_ = MeasurementPackage::LASER;
                meas_package.raw_measurements_ = VectorXd(2);
                float x;
                float y;
                float z;
                iss >> x;
                iss >> y;

                meas_package.raw_measurements_ << x, y;
                iss >> timestamp;
                meas_package.timestamp_ = timestamp;

                

                measurement_pack_list.push_back(meas_package);


            }
            else if (sensor_type.compare("R") == 0) {
                // Skip Radar measurements
                continue;
            }
           
          
            ++i;

        }



        // Create a Tracking instance
       //  Tracking tracking;

        // call the ProcessingMeasurement() function for each measurement
        size_t N = measurement_pack_list.size();
        // start filtering from the second frame 
        // (the speed is unknown in the first frame)

        VectorXd x(4);
        x << 103, 0, 0, 0;
        MatrixXd p(4, 4);
        p << 1, 0, 0, 0,
            0, 1, 0, 0,
            0, 0, 1000, 0,
            0, 0, 0, 1000;

        prex_.push_back(x);
        predict_p.push_back(p);

         /* data association */

         Association association;


         Tracking tracking;



       
        int a = 0;

        for (size_t k = 0; k < N; ++k) {
          
 
            
            for ( i = k; i < k+3; i++)
            {
                if (i>N-2)
                {
                    return 0;
                }

                if (i==0)
                {
                    association.Associat(measurement_pack_list[i], prex_[a], predict_p[a]);
                }
                  
                association.Associat(measurement_pack_list[i], prex_[a], predict_p[a]);

                
                disa.push_back(association.a);
               
                 cout << "disa: "<<i<<"=" << disa[i] << endl;
             //    cout << "measure_list " <<i<<"=" << measurement_pack_list[i].raw_measurements_ << endl;


            }


            double min = disa[k];

            if (disa[k+1]<disa[k+2])
            {
                if (disa[k+1]<disa[k])
                
                min = disa[k+1];
            }
            else
            {
                if (disa[k]<disa[k+2])
                {
                    min = disa[k];
                }
            }
         
            iter = find(disa.begin(),disa.end(),min);
            int key = iter - disa.begin();
            
          //   cout << "meas_key =" << measurement_pack_list[key].raw_measurements_ << endl;

            tracking.ProcessMeasurement(measurement_pack_list[key]);

              prex_.push_back(tracking.kf_.x_);
             predict_p.push_back(tracking.kf_.P_);

             out_file << tracking.kf_.x_[0] << "\n";

           //   cout <<"pre="<< prex_[k] << "\n";

             k = k + 2;
           
             a++;

             cout << "--------------------" << k << "------------------" << endl;

        }

       
    if (in_file.is_open()) {
        in_file.close();
    }
    return 0;
}