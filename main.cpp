// Summary:
//    Skeleton program to open a RINEX observation file and scroll through all
//    of the epochs.  Students in ENGO 465 will use this to develop code to compute
//    a least-squares solution using data from a standalone GPS receiver.
//
// History:
//    Jan 16/13 - Created by Mark Petovello
//
// Copyright:
//    Position, Location And Navigation (PLAN) Group
//    Department of Geomatics Engineering
//    Schulich School of Engineering
//    University of Calgary
//
// Disclaimer:
//    This source code is not freeware nor shareware and is only provided under
//    an agreement between authorized users/licensees and the University of
//    Calgary (Position, Location And Navigation (PLAN) Group, Geomatics
//    Engineering, Schulich School of Engineering) and may only be used under
//    the terms and conditions set forth therein.

#include <iostream>
#include <fstream>
#include <iomanip>

#include "rinex.h"
#include "NRinexUtils.h"

#include <Eigen/Dense> //added by @Talha

using namespace std;
using namespace NGSrinex;

struct SatelliteData
{
   int prn;
   double x, y, z, correction;
};

struct EpochData
{
   double epoch;
   int numSatellites;
   std::vector<SatelliteData> satellites;
};

struct ReceiverState
{
   double x, y, z, cdt; // Initial receiver guess (position + clock bias)
};

std::vector<EpochData> readSatelliteDataAtEachEpoch(const std::string &filename)
{
   std::ifstream file(filename);
   if (!file.is_open())
   {
      std::cerr << "Error opening file: " << filename << "\n";
      return {};
   }

   std::vector<EpochData> allEpochs;
   std::string line;

   while (std::getline(file, line))
   {
      std::istringstream iss(line);
      EpochData epochData;

      // Read the epoch and number of satellites
      if (!(iss >> epochData.epoch >> epochData.numSatellites))
      {
         std::cerr << "Error reading epoch line.\n";
         return {};
      }

      // Read satellite data for this epoch
      for (int i = 0; i < epochData.numSatellites; ++i)
      {
         if (!std::getline(file, line))
         {
            std::cerr << "Unexpected end of file while reading satellite data.\n";
            return {};
         }

         std::istringstream satStream(line);
         SatelliteData sat;
         if (!(satStream >> sat.prn >> sat.x >> sat.y >> sat.z >> sat.correction))
         {
            std::cerr << "Error reading satellite data.\n";
            return {};
         }

         epochData.satellites.push_back(sat);
      }

      allEpochs.push_back(epochData);
   }

   file.close();
   return allEpochs;
}

const EpochData *findEpoch(const std::vector<EpochData> &epochs, double epoch)
{
   for (const auto &e : epochs)
   {
      if (e.epoch == epoch)
         return &e; // Return pointer to found epoch
   }
   return nullptr; // Return nullptr if not found
}

// Compute Design Matrix and Misclosure Vector
void computeDesignMatrixAndMisclosure(
   const std::vector<SatelliteData> &satellites,
   const std::vector<double> &pseudoranges,
   const ReceiverState &receiver,
   Eigen::MatrixXd &A,
   Eigen::VectorXd &w)
{
  int numSat = satellites.size();
  A = Eigen::MatrixXd(numSat, 4);
  w = Eigen::VectorXd(numSat);

  for (int i = 0; i < numSat; ++i)
  {
     const SatelliteData &sat = satellites[i];

     // Compute geometric range (ρ_0)
     double rho_0 = sqrt(pow(receiver.x - sat.x, 2) +
                         pow(receiver.y - sat.y, 2) +
                         pow(receiver.z - sat.z, 2));

     // Compute design matrix row
     A(i, 0) = (receiver.x - sat.x) / rho_0;
     A(i, 1) = (receiver.y - sat.y) / rho_0;
     A(i, 2) = (receiver.z - sat.z) / rho_0;
     A(i, 3) = -1;

     // Compute misclosure vector (w)
     double correctedPseudorange = pseudoranges[i] - sat.correction;
     w(i) = (rho_0 - receiver.cdt) - correctedPseudorange;
  }
}

void leastSquaresSolution(
   std::vector<SatelliteData> &satellites,
   std::vector<double> &pseudoranges,
   std::ofstream &outputFile,
   double epochTime)
{
   ReceiverState receiver = {0.0, 0.0, 0.0, 0.0};
   int maxIterations = 100;
   double threshold = 1e-5;
   Eigen::VectorXd dR(4);

   for (int iter = 0; iter < maxIterations; ++iter) {
       Eigen::MatrixXd A;
       Eigen::VectorXd w;

       // **Recompute A and w using the updated receiver position**
       computeDesignMatrixAndMisclosure(satellites, pseudoranges, receiver, A, w);

       // **Solve for correction dR**
       Eigen::MatrixXd P = Eigen::MatrixXd::Identity(A.rows(), A.rows());
       Eigen::MatrixXd N = A.transpose() * P * A;
       Eigen::VectorXd U = A.transpose() * P * w;
       dR = -N.inverse() * U;

       // **Update receiver position and clock bias**
       receiver.x += dR(0);
       receiver.y += dR(1);
       receiver.z += dR(2);
       receiver.cdt += dR(3);

       // **Check for convergence**
       if (dR.norm() < threshold) break;
   }

   // **Save results to file**
    // **Write full-precision output to solution file**
    outputFile << std::fixed << std::setprecision(6)  
               << epochTime << " " 
               << receiver.x << " " 
               << receiver.y << " " 
               << receiver.z << " " 
               << receiver.cdt << "\n";
}
// Main processing loop
int main(int argc, char *argv[])
{
   string obsFilename = "../data/obsdata.22o";
   string satFilename = "../data/satpos.txt";
   string outputFilename = "../result/solution.txt";

   std::vector<EpochData> epochs = readSatelliteDataAtEachEpoch(satFilename);

   ofstream outputFile(outputFilename);
   if (!outputFile)
   {
      cout << "Could not open output file... quitting." << endl;
      return 0;
   }

   RinexObsFile inObsFile;
   if (!NRinexUtils::OpenRinexObservationFileForInput(inObsFile, obsFilename))
   {
      cout << "Could not open input observation file \"" << obsFilename << "\"...quitting." << endl;
      return 0;
   }

   NGSrinex::ObsEpoch currentRinexObs;

   try
   {
      while (inObsFile.readEpoch(currentRinexObs) != 0)
      {
         double obsTime = currentRinexObs.getEpochTime().GetGPSTime().secsOfWeek;

         std::vector<double> pseudoranges;
         std::vector<int> prns;

         for (unsigned short i = 0; i < currentRinexObs.getNumSat(); ++i)
         {
            NGSrinex::SatObsAtEpoch satObs = currentRinexObs.getSatListElement(i);

            if (satObs.satCode != 'G')
               continue;

            for (unsigned short j = 0; j < MAXOBSTYPES; ++j)
            {
               if (satObs.obsList[j].obsType != C1)
                  continue;
               if (!satObs.obsList[j].obsPresent)
                  continue;

               prns.push_back(satObs.satNum);
               pseudoranges.push_back(satObs.obsList[j].observation);
            }
         }

         if (pseudoranges.empty())
            continue;

         const EpochData *result = findEpoch(epochs, obsTime);
         if (!result)
         {
            continue;
         }

         std::vector<SatelliteData> matchedSatellites;
         for (const auto &sat : result->satellites)
         {
            if (std::find(prns.begin(), prns.end(), sat.prn) != prns.end())
            {
               matchedSatellites.push_back(sat);
            }
         }

         if (matchedSatellites.size() < 4)
         {
            std::cout << "Not enough satellites for epoch " << obsTime << "\n";
            continue;
         }

         // Call Least Squares solution
         leastSquaresSolution(matchedSatellites, pseudoranges, outputFile, obsTime);
      }
   }
   catch (RinexReadingException &readingExcep)
   {
      cout << "RinexReadingException: " << readingExcep.getMessage() << endl;
   }

   return 0;
}