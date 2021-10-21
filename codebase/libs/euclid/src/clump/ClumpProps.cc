// *=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=* 
// ** Copyright UCAR (c) 1990 - 2016                                         
// ** University Corporation for Atmospheric Research (UCAR)                 
// ** National Center for Atmospheric Research (NCAR)                        
// ** Boulder, Colorado, USA                                                 
// ** BSD licence applies - redistribution and use in source and binary      
// ** forms, with or without modification, are permitted provided that       
// ** the following conditions are met:                                      
// ** 1) If the software is modified to produce derivative works,            
// ** such modified software should be clearly marked, so as not             
// ** to confuse it with the version available from UCAR.                    
// ** 2) Redistributions of source code must retain the above copyright      
// ** notice, this list of conditions and the following disclaimer.          
// ** 3) Redistributions in binary form must reproduce the above copyright   
// ** notice, this list of conditions and the following disclaimer in the    
// ** documentation and/or other materials provided with the distribution.   
// ** 4) Neither the name of UCAR nor the names of its contributors,         
// ** if any, may be used to endorse or promote products derived from        
// ** this software without specific prior written permission.               
// ** DISCLAIMER: THIS SOFTWARE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS  
// ** OR IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED      
// ** WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.    
// *=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=* 
/////////////////////////////////////////////////////////////
// ClumpProps.cc
//
// ClumpProps class - combines a clump with grid geometry so that
// computations may be done on the clump using that grid geometry.
//
// Mike Dixon, EOL, NCAR, P.O.Box 3000, Boulder, CO, 80307-3000, USA
//
// July 2021
//
///////////////////////////////////////////////////////////////

#include <euclid/ClumpProps.hh>
#include <toolsa/umisc.h>
#include <rapmath/math_macros.h>
#include <cassert>
#include <iostream>
#include <algorithm>
using namespace std;

///////////////
// constructors
//

ClumpProps::ClumpProps()
{
  _initDone = FALSE;
}

/////////////
// destructor
//

ClumpProps::~ClumpProps()

{
  // assert (_initDone);
}

//////////////
// initializer
//

void ClumpProps::init(const Clump_order *clump,
                      const PjgGridGeom &gridGeom)

{

  _clump = clump;
  _nIntervals = _clump->size;

  if(clump->size > 0) {
    Interval *intvl = clump->ptr[0];
    _id = intvl->id;
  } else {
    _initDone = FALSE;
    return;
  }

  _gridGeom = gridGeom;
  
  // compute the bounding box for the clump, and create
  // an array of intervals relative to these bounds.

  _shrinkWrap();

  // compute projected area grid

  _compute2DGrid();

  // compute the properties

  _computeProps();

  _initDone = TRUE;

}

////////////////////////////////////////////////////////////////
// _shrinkWrap()
//
// Compute the grid indices which bound the clump.
// Create a set of intervals relative to the bounding box.
//

void ClumpProps::_shrinkWrap()

{

  // determine the spatial limits

  _minIx = 1000000000;
  _minIy = 1000000000;
  _maxIx = 0;
  _maxIy = 0;

  for (int intv = 0; intv < _clump->size; intv++) {
    const Interval *intvl = _clump->ptr[intv];
    _minIx = MIN(intvl->begin, _minIx);
    _maxIx = MAX(intvl->end, _maxIx);
    _minIy = MIN(intvl->row_in_plane, _minIy);
    _maxIy = MAX(intvl->row_in_plane, _maxIy);
  }

  // load up local intervals, adjusting for the limits

  _intvLocal.clear();
  for (int intv = 0; intv < _clump->size; intv++) {
    Interval intvl = *_clump->ptr[intv];
    intvl.row_in_plane -= _minIy;
    intvl.begin -= _minIx;
    intvl.end -= _minIx;
    _intvLocal.push_back(intvl);
  }

  // set the various public measures of clump position and bounding size

  _nXLocal = _maxIx - _minIx + 1;
  _nYLocal = _maxIy - _minIy + 1;

  _offsetX = _minIx * _gridGeom.dx();
  _offsetY = _minIy * _gridGeom.dy();
  
  _startXLocal = _gridGeom.minx() + _offsetX;
  _startYLocal = _gridGeom.miny() + _offsetY;

}


////////////////////////////////////////////////////////////////
// _compute2DGrid()
//
// Compute the projected area grid
//

void ClumpProps::_compute2DGrid()
  
{

  // initialize

  _grid2DArray.alloc(_nYLocal, _nXLocal);
  memset(_grid2DArray.dat1D(), 0, _nYLocal * _nXLocal);
  _grid2DVals = _grid2DArray.dat2D();

  // populate projected area grid - local

  for (size_t intv = 0; intv < _intvLocal.size(); intv++) {
    const Interval &intvl = _intvLocal[intv];
    int iy = intvl.row_in_plane;
    for (int ix = intvl.begin; ix <= intvl.end; ix++) {
      _grid2DVals[iy][ix] = 1;
    }
  }

  _nPoints2D = 0;
  for (int iy = 0; iy < _nYLocal; iy++) {
    for (int ix = 0; ix < _nXLocal; ix++) {
      if (_grid2DVals[iy][ix] == 1) {
        _nPoints2D++;
      }
    } 
  } 

  // compute centroid relative to global grid

  double sumx = 0.0, sumy = 0.0, sumz = 0.0, nn = 0.0;
  for (int intv = 0; intv < _clump->size; intv++) {
    const Interval *intvl = _clump->ptr[intv];
    int iz = intvl->plane;
    int iy = intvl->row_in_plane;
    double zz = _gridGeom.zKm(iz);
    double yy = _gridGeom.miny() + iy * _gridGeom.dy();
    for (int ix = intvl->begin; ix <= intvl->end; ix++) {
      double xx = _gridGeom.minx() + ix * _gridGeom.dx();
      sumx += xx;
      sumy += yy;
      sumz += zz;
      nn++;
      // cerr << "1111111 ix, iy, xx, yy: " << ix << ", " << xx << ", " << iy << ", " << yy << endl;
    }
  }

  _centroidX = sumx / nn;
  _centroidY = sumy / nn;
  _centroidZ = sumz / nn;

  {

    double sumx = 0.0, sumy = 0.0, nn = 0.0;
    for (size_t intv = 0; intv < _intvLocal.size(); intv++) {
      const Interval *intvl = &_intvLocal[intv];
      int iy = intvl->row_in_plane;
      double yy = _startYLocal + iy * _gridGeom.dy();
      for (int ix = intvl->begin; ix <= intvl->end; ix++) {
        double xx = _startXLocal + ix * _gridGeom.dx();
        sumx += xx;
        sumy += yy;
        // cerr << "2222222 ix, iy, xx, yy: " << ix << ", " << xx << ", " << iy << ", " << yy << endl;
        nn++;
      }
    }
    
    double centroidX = sumx / nn;
    double centroidY = sumy / nn;

    if (fabs(centroidX - _centroidX) > 0.001) {
      cerr << "VVVVVVVVVV centroidX, _centroidX: " << centroidX << ", " << _centroidX << endl;
      cerr << "WWWWWWWWWW _clump->size, _intvLocal.size(), nn: " << _clump->size << ", " <<  _intvLocal.size() << ", " << nn << endl;

      cerr << "AAAAAAAA _minIx, _minIy: " <<  _minIx << ", " << _minIy << endl;
      cerr << "AAAAAAAA _maxIx, _maxIy: " <<  _maxIx << ", " << _maxIy << endl;
      cerr << "AAAAAAAA _offsetX, _offsetY: " <<  _offsetX << ", " << _offsetY << endl;
      cerr << "AAAAAAAA _startXLocal, _startYLocal: " <<  _startXLocal << ", " << _startYLocal << endl;
      cerr << "AAAAAAAA minx, miny: " <<  _gridGeom.minx() << ", " << _gridGeom.miny() << endl;
    }
    if (fabs(centroidY - _centroidY) > 0.001) {
      cerr << "VVVVVVVVVV centroidY, _centroidY: " << centroidY << ", " << _centroidY << endl;
      cerr << "WWWWWWWWWW _clump->size, _intvLocal.size(): " << _clump->size << ", " <<  _intvLocal.size() << endl;
    }
    
  }

  // determine the scale in km
  // based on whether this is a (lat,lon) or (km,km) grid
  
  if (_gridGeom.isLatLon()) {
    
    // latlon grid

    // we need to multiply by
    // a (111.12 squared) to get km2 for area. The delta_z is
    // set nominally to 1.0, so area and volume will be the same.
    // The volume and area computations are adjusted later for the
    // latitude of the storm.
    
    _dXKmAtCentroid =
      _gridGeom.dx() * KM_PER_DEG_AT_EQ * cos(_centroidY * DEG_TO_RAD);
    _dYKmAtCentroid = _gridGeom.dy() * KM_PER_DEG_AT_EQ;

  } else {
  
    // projection-based (km) grid
    
    _dXKmAtCentroid = _gridGeom.dx();
    _dYKmAtCentroid = _gridGeom.dy();

  }

  // compute delta area and vol
  
  _dAreaAtCentroid = _dXKmAtCentroid * _dYKmAtCentroid;

  // cerr << "DDDDDDDDDDDD _dAreaAtCentroid: " << _dAreaAtCentroid << endl;
  // cerr << "DDDDDDDDDDDD _centroidY: " << _centroidY << endl;
  // cerr << "DDDDDDDDDDDD _minIY: " << _minIy << endl;

  _dVolAtCentroid.clear();
  for (size_t iz = 0; iz < nZ(); iz++) {
    _dVolAtCentroid.push_back(_dAreaAtCentroid * _gridGeom.dzKm(iz));
  }

}

////////////////////////////////////////////////////////////////
// Compute the properties of this clump.
//

void ClumpProps::_computeProps()

{

  // projected area
  
  _projAreaKm2 = 0.0;
  _nPoints2D = 0;
  for (int iy = 0; iy < _nYLocal; iy++) {
    for (int ix = 0; ix < _nXLocal; ix++) {
      if (_grid2DVals[iy][ix]) {
        _nPoints2D++;
        _projAreaKm2 += _dAreaAtCentroid;
      }
    } // ix
  } // iy

  // volume
  
  _nPoints3D = _clump->pts;
  _volumeKm3 = 0.0;
  for (int intv = 0; intv < _clump->size; intv++) {
    const Interval *intvl = _clump->ptr[intv];
    _volumeKm3 += intvl->len * _dVolAtCentroid[intvl->plane];
  }

  // clump size - generic
  // area for 2D, volume for 3D
  
  if (_gridGeom.nz() <= 1) {
    _clumpSize = _projAreaKm2;
  } else {
    _clumpSize = _volumeKm3;
  }

  // vertical extent
  
  _minZKm = 9999.0;
  _maxZKm = -9999.0;

  for (int intv = 0; intv < _clump->size; intv++) {
    
    const Interval *intvl = _clump->ptr[intv];
    
    int iz = intvl->plane;
    double zKm = _gridGeom.zKm(iz);
    _minZKm = min(zKm, _minZKm);
    _maxZKm = max(zKm, _maxZKm);

  }

  _vertExtentKm = _maxZKm - _minZKm;

}

////////////////////////////////////////////////////////////////
// Print out the clump metadata
//

void ClumpProps::printMeta(ostream &out) const

{
  
  out << "--------------ClumpProps--------------------" << endl;

  out << "  id: " << _id << endl;
  out << "  nIntervals: " << _nIntervals << endl;
  out << "  nPoints3D: " << _nPoints3D << endl;
  out << "  nPoints2D: " << _nPoints2D << endl;

  out << "  minIx, minIy: " << _minIx << ", " << _minIy << endl;
  out << "  maxIx, maxIy: " << _maxIx << ", " << _maxIy << endl;
  out << "  nXLocal, nYLocal: " << _nXLocal << ", " << _nYLocal << endl;

  out << "  offsetX, offsetY: " << _offsetX << ", " << _offsetY << endl;
  out << "  startXLocal, startYLocal: " << _startXLocal << ", " << _startYLocal << endl;

  out << "  dXKmAtCentroid, dYKmAtCentroid: " << _dXKmAtCentroid << ", " << _dYKmAtCentroid << endl;
  out << "  dAreaAtCentroid: " << _dAreaAtCentroid << endl;
  out << "  dVolAtCentroid: ";
  for (size_t ii = 0; ii < _dVolAtCentroid.size(); ii++) {
    out << _dVolAtCentroid[ii];
    if (ii != _dVolAtCentroid.size() - 1) {
      out << ",";
    }
  }
  out << endl;
  
  out << "  clumpSize: " << _clumpSize << endl;
  out << "  projAreaKm2: " << _projAreaKm2 << endl;
  out << "  volumeKm3: " << _volumeKm3 << endl;
  out << "  nPoints2D: " << _nPoints2D << endl;

  out << "  minZKm, maxZKm, vertExtentKm: " 
      << _minZKm << ", " << _maxZKm << ", " << _vertExtentKm << endl;

  out << "  centroidX, centroidY, centroidZ: " 
      << _centroidX << ", " << _centroidY << ", " << _centroidZ << endl;

  _gridGeom.print(out);

  out << "--------------------------------------------" << endl;

}


////////////////////////////////////////////////////////////////
// Print out the full clump data
//

void ClumpProps::printFull(ostream &out) const

{

  printMeta(out);

  out << "------------- intervals --------------------" << endl;

  for (size_t ii = 0; ii < _intvLocal.size(); ii++) {

    out << "  ii, iz, iy, ix1, ix2, len: "
        << ii << ", "
        << _intvLocal[ii].plane << ", "
        << _intvLocal[ii].row_in_plane << ", "
        << _intvLocal[ii].begin << ", "
        << _intvLocal[ii].end << ", "
        << _intvLocal[ii].len << endl;

  } // ii

  out << "--------------------------------------------" << endl;

}

