#ifndef RayLocationModel_HH
#define RayLocationModel_HH


#include <vector>


#include "RayLoc.hh"


class RayLocationModel {

public:

RayLocationModel();
~RayLocationModel();

void sortRaysIntoRayLocations(float ppi_rendering_beam_width);
size_t getNRayLocations();
double getStartRangeKm(size_t rayIdx);
double getGateSpacingKm(size_t rayIdx);
size_t getEndIndex(size_t rayIdx);
double getStartAngle(size_t rayIdx);
double getStopAngle(size_t rayIdx);

vector <float> *getRayData(size_t rayIdx, string fieldName);

const RadxRay *getClosestRay(double azDeg);

private:
	vector<RayLoc> ray_loc;

};

#endif