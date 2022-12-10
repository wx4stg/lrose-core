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
// ** 2) Redistributions of source code must retain the above conypyright      
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
///////////////////////////////////////////////////////////////
// RadxClutter.cc
//
// Mike Dixon, RAP, NCAR, P.O.Box 3000, Boulder, CO, 80307-3000, USA
//
// Feb 2013
//
///////////////////////////////////////////////////////////////

#include "RadxClutter.hh"
#include <Radx/RadxSweep.hh>
#include <Radx/RadxRay.hh>
#include <Radx/RadxField.hh>
#include <Radx/RadxTime.hh>
#include <Radx/RadxTimeList.hh>
#include <Radx/RadxPath.hh>
#include <Radx/RadxComplex.hh>
#include <Ncxx/Nc3xFile.hh>
#include <Mdv/GenericRadxFile.hh>
#include <dsserver/DsLdataInfo.hh>
#include <toolsa/pmu.h>
#include <toolsa/toolsa_macros.h>
#include <toolsa/sincos.h>
#include <toolsa/TaArray.hh>
#include <toolsa/Path.hh>
#include <toolsa/file_io.h>
#include <toolsa/LogStream.hh>
#include <toolsa/LogStreamInit.hh>
#include <algorithm>

using namespace std;

const double RadxClutter::missingVal = -9999.0;

// Constructor

RadxClutter::RadxClutter(int argc, char **argv)
  
{

  OK = TRUE;
  _nVols = 0;
  _nGates = 0;
  
  // set programe name

  _progName = "RadxClutter";
  
  // parse command line args
  
  if (_args.parse(argc, argv, _progName)) {
    cerr << "ERROR: " << _progName << endl;
    cerr << "Problem with command line args." << endl;
    OK = FALSE;
    return;
  }
  
  // get TDRP params
  
  _paramsPath = (char *) "unknown";
  if (_params.loadFromArgs(argc, argv,
			   _args.override.list, &_paramsPath)) {
    cerr << "ERROR: " << _progName << endl;
    cerr << "Problem with TDRP parameters." << endl;
    OK = FALSE;
    return;
  }

  // init process mapper registration

  if (_params.mode == Params::REALTIME) {
    PMU_auto_init( _progName.c_str(),
                  _params.instance,
                  PROCMAP_REGISTER_INTERVAL);
  }

  // initialize logging
  LogStreamInit::init(false, false, true, true);
  LOG_STREAM_DISABLE(LogStream::WARNING);
  LOG_STREAM_DISABLE(LogStream::DEBUG);
  LOG_STREAM_DISABLE(LogStream::DEBUG_VERBOSE);
  LOG_STREAM_DISABLE(LogStream::DEBUG_EXTRA);
  if (_params.debug >= Params::DEBUG_EXTRA) {
    LOG_STREAM_ENABLE(LogStream::DEBUG_EXTRA);
    LOG_STREAM_ENABLE(LogStream::DEBUG_VERBOSE);
    LOG_STREAM_ENABLE(LogStream::DEBUG);
  } else if (_params.debug >= Params::DEBUG_VERBOSE) {
    LOG_STREAM_ENABLE(LogStream::DEBUG_VERBOSE);
    LOG_STREAM_ENABLE(LogStream::DEBUG);
  } else if (_params.debug) {
    LOG_STREAM_ENABLE(LogStream::DEBUG);
    LOG_STREAM_ENABLE(LogStream::WARNING);
  }

}

// destructor

RadxClutter::~RadxClutter()

{

  // unregister process

  PMU_auto_unregister();

}

//////////////////////////////////////////////////
// Run

int RadxClutter::Run()
{

  // set up angle list

  _initAngleList();

  // run based on mode
  
  if (_params.mode == Params::ARCHIVE) {
    return _runArchive();
  } else if (_params.mode == Params::FILELIST) {
    return _runFilelist();
  } else {
    return _runRealtime();
  }
}

//////////////////////////////////////////////////
// Run in filelist mode

int RadxClutter::_runFilelist()
{

  // loop through the input file list

  int iret = 0;

  for (int ii = 0; ii < (int) _args.inputFileList.size(); ii++) {

    string inputPath = _args.inputFileList[ii];
    if (_processFile(inputPath)) {
      iret = -1;
    }

  }

  return iret;

}

//////////////////////////////////////////////////
// Run in archive mode

int RadxClutter::_runArchive()
{

  // get the files to be processed

  RadxTimeList tlist;
  tlist.setDir(_params.input_dir);
  tlist.setModeInterval(_args.startTime, _args.endTime);
  if (tlist.compile()) {
    LOG(ERROR) << "ERROR - RadxClutter::_runFilelist()";
    LOG(ERROR) << "  Cannot compile time list, dir: " << _params.input_dir;
    LOG(ERROR) << "  Start time: " << RadxTime::strm(_args.startTime);
    LOG(ERROR) << "  End time: " << RadxTime::strm(_args.endTime);
    LOG(ERROR) << tlist.getErrStr();
    return -1;
  }

  const vector<string> &paths = tlist.getPathList();
  if (paths.size() < 1) {
    LOG(ERROR) << "ERROR - RadxClutter::_runFilelist()";
    LOG(ERROR) << "  No files found, dir: " << _params.input_dir;
    return -1;
  }
  
  // loop through the input file list
  
  int iret = 0;
  for (size_t ii = 0; ii < paths.size(); ii++) {
    if (_processFile(paths[ii])) {
      iret = -1;
    }
  }

  return iret;

}

//////////////////////////////////////////////////
// Run in realtime mode

int RadxClutter::_runRealtime()
{

  // init process mapper registration

  PMU_auto_init(_progName.c_str(), _params.instance,
                PROCMAP_REGISTER_INTERVAL);

  // watch for new data to arrive

  LdataInfo ldata(_params.input_dir,
                  _params.debug >= Params::DEBUG_VERBOSE);
  
  int iret = 0;

  while (true) {
    ldata.readBlocking(_params.max_realtime_data_age_secs,
                       1000, PMU_auto_register);
    
    const string path = ldata.getDataPath();
    if (_processFile(path)) {
      iret = -1;
    }
  }

  return iret;

}

//////////////////////////////////////////////////
// Process a file
// Returns 0 on success, -1 on failure

int RadxClutter::_processFile(const string &filePath)
{

  // read in the file

  if (_readFile(filePath)) {
    return -1;
  }

  // check for substr
  
  string subStr = _params.file_name_substr;
  if (subStr.size() > 0) {
    RadxPath rpath(filePath);
    string fileName = rpath.getFile();
    if (fileName.find(subStr) == string::npos) {
      LOG(DEBUG) << "Looking for substr: " << subStr;
      LOG(DEBUG) << "No substr found, ignoring file: " << filePath;
      return 0;
    }
  }
  
  // check if this is an RHI
  
  _isRhi = _readVol.checkIsRhi();
  if (_params.scan_mode == Params::PPI) {
    if (_isRhi) {
      LOG(ERROR) << "Scan mode is not PPI, ignoring file: " << filePath;
      return -1;
    }
  } else {
    if (!_isRhi) {
      LOG(ERROR) << "Scan mode is not RHI, ignoring file: " << filePath;
      return -1;
    }
  }

  // initialize the clutter volume from the relevant scans in the file

  if (_initClutterVol()) {
    return -1;
  }
  
  // initialize the histogram for the clutter frequency
  
  _clutFreqHist.init(0.0, 0.02, 1.0);

  // analyze this data set
  
  if (_analyzeClutter()) {
    LOG(ERROR) << "ERROR - RadxClutter::Run";
    LOG(ERROR) << "  Cannot process data in file: " << filePath;
    return -1;
  }

  // write out the results

  if (_writeClutterVol()) {
    LOG(ERROR) << "ERROR - RadxClutter::Run";
    LOG(ERROR) << "  Cannot write out clutter volume";
    return -1;
  }

  return 0;

}

//////////////////////////////////////////////////
// set up read

void RadxClutter::_setupRead(RadxFile &file)
{

  if (_params.debug >= Params::DEBUG_VERBOSE) {
    file.setDebug(true);
  }
  if (_params.debug >= Params::DEBUG_EXTRA) {
    file.setVerbose(true);
  }

  file.setReadIgnoreTransitions(true);
  file.setReadMaxRangeKm(_params.max_range_km);

  file.addReadField(_params.dbz_field_name);

  if (_params.use_vel_field) {
    file.addReadField(_params.vel_field_name);
  }

  if (_params.debug >= Params::DEBUG_EXTRA) {
    file.printReadRequest(cerr);
  }

}

//////////////////////////////////////////////////
// Process in a file
// Returns 0 on success, -1 on failure

int RadxClutter::_readFile(const string &filePath)
{

  // check we have not already processed this file
  // in the file aggregation step
  
  RadxPath thisPath(filePath);
  for (size_t ii = 0; ii < _readPaths.size(); ii++) {
    RadxPath rpath(_readPaths[ii]);
    if (thisPath.getFile() == rpath.getFile()) {
      LOG(DEBUG_VERBOSE) << "Skipping file: " << filePath;
      LOG(DEBUG_VERBOSE) << "  Previously processed in aggregation step";
      return 0;
    }
  }
  
  LOG(DEBUG) << "INFO - RadxClutter::_readFile";
  LOG(DEBUG) << "  Input path: " << filePath;
  
  GenericRadxFile inFile;
  _setupRead(inFile);
  
  // read in file
  
  _readVol.clear();
  if (inFile.readFromPath(filePath, _readVol)) {
    LOG(ERROR) << "ERROR - RadxClutter::_readFile";
    LOG(ERROR) << inFile.getErrStr();
    return -1;
  }
  _readPaths = inFile.getReadPaths();

  // convert to floats
  
  _readVol.convertToFl32();
  
  // set number of gates constant
  
  _readVol.setNGatesConstant();

  if (_readVol.getNRays() < 3) {
    return -1;
  } else {
    return 0;
  }
  
}

///////////////////////////////////////////
// process this data set

int RadxClutter::_analyzeClutter()
  
{

  LOG(DEBUG) << "Processing data set ...";

  // add to DBZ sum and compute mean

  vector<RadxRay *> &rays = _clutterVol.getRays();
  for (size_t iray = 0; iray < _nRaysClutter; iray++) {
    RadxRay *ray = rays[iray];

    RadxField *dbzFld = ray->getField(_params.dbz_field_name);
    if (dbzFld == NULL) {
      continue;
    }
    Radx::fl32 dbzMiss = dbzFld->getMissingFl32();
    Radx::fl32 *dbzVals = dbzFld->getDataFl32();
    
    RadxField *velFld = ray->getField(_params.vel_field_name);
    Radx::fl32 velMiss = Radx::missingFl32;
    if (velFld != NULL) {
      velMiss = velFld->getMissingFl32();
    }
    Radx::fl32 *velVals = NULL;
    if (velFld) {
      velVals = velFld->getDataFl32();
    }
    
    Radx::fl32 *dbzCount = _dbzCount[iray];
    Radx::fl32 *dbzSum = _dbzSum[iray];
    Radx::fl32 *dbzSqSum = _dbzSqSum[iray];
    Radx::fl32 *dbzMean = _dbzMean[iray];
    Radx::fl32 *dbzSdev = _dbzSdev[iray];
    Radx::fl32 *clutSum = _clutSum[iray];
    Radx::fl32 *clutFreq = _clutFreq[iray];
    
    for (size_t igate = 0; igate < _nGates; igate++) {
      Radx::fl32 dbzVal = dbzVals[igate];
      if (dbzVal != dbzMiss) {
        dbzSum[igate] += dbzVal;
        dbzSqSum[igate] += (dbzVal * dbzVal);
        dbzCount[igate] += 1.0;
        bool isClutter = false;
        if (dbzVal >= _params.clutter_dbz_threshold) {
          isClutter = true; // dbz exceeds threshold - could be clutter
        }
        if (velVals) {
          Radx::fl32 velVal = velVals[igate];
          if (velVal != velMiss && fabs(velVal) > _params.max_abs_vel) {
            isClutter = false; // vel exceeds threshold - not clutter
          }
        }
        if (isClutter) {
          clutSum[igate] += 1.0;
        }
      }
      double count = dbzCount[igate];
      if (count > 0) {
        dbzMean[igate] = dbzSum[igate] / count;
        clutFreq[igate] = clutSum[igate] / count;
      }
      if (count > 1) {
        double sum = dbzSum[igate];
        double sumSq = dbzSqSum[igate];
        double var =
          (sumSq - (sum * sum) / count) / (count - 1.0);
        if (var > 0.0) {
          dbzSdev[igate] = sqrt(var);
        } else {
          dbzSdev[igate] = 0.0;
        }
      }
    } // igate

    // add output fields to ray

    RadxField *dbzMeanFldOut = new RadxField(*dbzFld);
    dbzMeanFldOut->setName(_params.dbz_mean_field_name);
    dbzMeanFldOut->setDataFl32(_nGates, dbzMean, true);
    
    RadxField *dbzSdevFldOut = new RadxField(*dbzFld);
    dbzSdevFldOut->setName(_params.dbz_sdev_field_name);
    dbzSdevFldOut->setDataFl32(_nGates, dbzSdev, true);
    
    RadxField *clutFreqFldOut = new RadxField(*dbzFld);
    clutFreqFldOut->setName(_params.clut_freq_field_name);
    clutFreqFldOut->setLongName("ClutterTimeFraction");
    clutFreqFldOut->setUnits("");
    clutFreqFldOut->setDataFl32(_nGates, clutFreq, true);

    ray->addField(dbzMeanFldOut);
    ray->addField(dbzSdevFldOut);
    ray->addField(clutFreqFldOut);
    
  } // iray

  // compute the clutter frequency histogram
  
  for (size_t iray = 0; iray < _nRaysClutter; iray++) {
    Radx::fl32 *clutFreq = _clutFreq[iray];
    for (size_t igate = 0; igate < _nGates; igate++) {
      _clutFreqHist.update(clutFreq[igate]);
    }
  }
  _clutFreqHist.computeVariance();
  
  if (_params.debug >= Params::DEBUG_VERBOSE) {
    _clutFreqHist.print(stderr);
    _clutFreqHist.printVariance(stderr);
  }

  // set the clutter flag

  double maxVariance;
  double clutFracThreshold = _clutFreqHist.getFreqForMaxVar(maxVariance);
  if (_params.specify_clutter_fraction_threshold) {
    clutFracThreshold = _params.clutter_fraction_threshold;
  }
  
  LOG(DEBUG) << "  Freq histogram stats - maxVar, clutFracThreshold: "
             << maxVariance << ", " << clutFracThreshold;
  
  for (size_t iray = 0; iray < _nRaysClutter; iray++) {
    Radx::fl32 *clutFreq = _clutFreq[iray];
    Radx::fl32 *clutFlag = _clutFlag[iray];
    for (size_t igate = 0; igate < _nGates; igate++) {
      if (clutFreq[igate] > clutFracThreshold) {
        clutFlag[igate] = 1.0;
      } else {
        clutFlag[igate] = 0.0;
      }
    } // igate
  } // iray

  // add the clutter flag to the output
  
  for (size_t iray = 0; iray < _nRaysClutter; iray++) {
    RadxRay *ray = rays[iray];
    RadxField *dbzFld = ray->getField(_params.dbz_field_name);
    if (dbzFld == NULL) {
      continue;
    }
    Radx::fl32 *clutFlag = _clutFlag[iray];
    RadxField *clutFlagFldOut = new RadxField(*dbzFld);
    clutFlagFldOut->setName(_params.clut_flag_field_name);
    clutFlagFldOut->setLongName("ClutterFlag");
    clutFlagFldOut->setUnits("");
    clutFlagFldOut->setDataFl32(_nGates, clutFlag, true);
    ray->addField(clutFlagFldOut);
  } // iray

  return 0;

}

//////////////////////////////////////////////////
// Initialize the angle list
// Returns 0 on success, -1 on failure

void RadxClutter::_initAngleList()
{

  _fixedAngles.clear();
  _scanAngles.clear();
  
  for (int ii = 0; ii < _params.sweep_fixed_angles_n; ii++) {
    _fixedAngles.push_back(_params._sweep_fixed_angles[ii]);
  }
  sort(_fixedAngles.begin(), _fixedAngles.end());

  if (_params.scan_mode == Params::PPI) {
    
    // PPI clutter scan - compute azimuths
    
    double sectorDelta = _params.last_ray_angle - _params.first_ray_angle;
    if (_params.first_ray_angle > _params.last_ray_angle) {
      sectorDelta += 360.0;
    }

    double az = _params.first_ray_angle;
    double sumDelta = 0.0;
    while (sumDelta < sectorDelta) {
      _scanAngles.push_back(az);
      az = RadxComplex::computeSumDeg(az, _params.delta_ray_angle);
      if (az < 0) {
        az += 360.0;
      }
      sumDelta += fabs(_params.delta_ray_angle);
    } // while (sumDelta < sectorDelta)
    
  } else {

    // RHI clutter scan - compute RHI elevations
    
    double sectorDelta = _params.last_ray_angle - _params.first_ray_angle;

    double el = _params.first_ray_angle;
    double sumDelta = 0.0;
    while (sumDelta < sectorDelta) {
      _scanAngles.push_back(el);
      el = RadxComplex::computeSumDeg(el, _params.delta_ray_angle);
      sumDelta += fabs(_params.delta_ray_angle);
    } // while (sumDelta < sectorDelta)

  } // if (_params.scan_mode == Params::PPI)
  
}

/////////////////////////////////////////////////////////////
// initialize the clutter volume from the selected angles
// this is not optimized for speed, but is simple and
// performance is not limiting here.

int RadxClutter::_initClutterVol()
{

  const vector<RadxRay *> &rays = _readVol.getRays();
  if (rays.size() < 1) {
    return -1;
  }

  // initialize the clutter volume with the read volume,
  // and then clear out the rays

  _clutterVol.clear();
  _clutterVol = _readVol;
  _clutterVol.clearRays();

  // create empty ray with all gates missing
  
  RadxRay emptyRay(*rays[0]);
  RadxField *dbzFld = emptyRay.getField(_params.dbz_field_name);
  if (dbzFld == NULL) {
    LOG(ERROR) << "DBZ field name not found in ray: " << _params.dbz_field_name;
    return -1;
  }
  dbzFld->setGatesToMissing(0, dbzFld->getNPoints() - 1);
  
  if (_params.debug >= Params::DEBUG_EXTRA) {
    cerr << "==>> looking for rays that match clutter volume <<==" << endl;
  }
  
  // loop through the sweeps and scan angles

  for (size_t isweep = 0; isweep < _fixedAngles.size(); isweep++) {
    for (size_t iang = 0; iang < _scanAngles.size(); iang++) {

      // get the clutter angle
      
      double clutEl = _fixedAngles[isweep];
      double clutAz = _scanAngles[iang];
      if (_isRhi) {
        clutEl = _scanAngles[iang];
        clutAz = _fixedAngles[isweep];
      }

      // find the closest ray in the measured volume
      
      double minAngDist = 1.0e6;
      double matchAz = -9999;
      double matchEl = -9999;
      size_t matchRayIndex = 0;
      RadxRay *matchRay = NULL;
      bool matchFound = false;
      
      for (size_t ii = 0; ii < rays.size(); ii++) {
        RadxRay *ray = rays[ii];
        double rayEl = ray->getElevationDeg();
        double rayAz = ray->getAzimuthDeg();
        double dEl = fabs(rayEl - clutEl);
        double dAz = fabs(rayAz - clutAz);
        if (dEl > _params.elev_tolerance_deg ||
            dAz > _params.az_tolerance_deg) {
          continue;
        }
        double angDist = sqrt(dEl * dEl + dAz * dAz);
        if (angDist < minAngDist) {
          minAngDist = angDist;
          matchEl = rayEl;
          matchAz = rayAz;
          matchRay = ray;
          matchRayIndex = ii;
          matchFound = true;
        }
      } // ii

      // create the clutter ray
      
      RadxRay *clutRay = NULL;
      if (matchFound) {
        clutRay = new RadxRay(*matchRay);
        if (_params.debug >= Params::DEBUG_EXTRA) {
          fprintf(stderr,
                  "====>> isweep, iang, index, rayEl, matchEl, "
                  "rayAz, matchAz, dEl, dAz: "
                  "%3ld %3ld %5ld %7.3f %7.3f %7.3f %7.3f %7.3f %7.3f\n",
                  isweep, iang, matchRayIndex,
                  clutEl, matchEl, clutAz, matchAz,
                  fabs(clutEl - matchEl),
                  fabs(clutAz - matchAz));
        }
      } else {
        clutRay = new RadxRay(emptyRay);
        if (_params.debug >= Params::DEBUG_EXTRA) {
          fprintf(stderr,
                  "====>> isweep, iang, index, rayEl, matchEl, "
                  "rayAz, matchAz, dEl, dAz: "
                  "%3ld %3ld %5ld %7.3f %7.3f %7.3f %7.3f %7.3f %7.3f\n",
                  isweep, iang, matchRayIndex,
                  clutEl, matchEl, clutAz, matchAz,
                  fabs(clutEl - matchEl),
                  fabs(clutAz - matchAz));
        }
      }
      clutRay->setElevationDeg(clutEl);
      clutRay->setAzimuthDeg(clutAz);
      if (_isRhi) {
        clutRay->setFixedAngleDeg(clutAz);
      } else {
        clutRay->setFixedAngleDeg(clutEl);
      }
      clutRay->clearEventFlags();
      clutRay->setSweepNumber(isweep);
      
      // add to the clutter vol

      _clutterVol.addRay(clutRay);

    } // iang
  } // isweep

  _clutterVol.loadSweepInfoFromRays();
  _clutterVol.loadVolumeInfoFromRays();
  _nRaysClutter = _clutterVol.getNRays();
  
  // set up geom

  if (_nVols == 0) {
    
    _nGates = _clutterVol.getMaxNGates();
    _radxStartRange = _clutterVol.getStartRangeKm();
    _radxGateSpacing = _clutterVol.getGateSpacingKm();
    
    _radarName = _clutterVol.getInstrumentName();
    _radarLatitude = _clutterVol.getLatitudeDeg();
    _radarLongitude = _clutterVol.getLongitudeDeg();
    _radarAltitude = _clutterVol.getAltitudeKm();

    // allocate arrays for analysis
    
    _dbzSum = _dbzSumArray.alloc(_nRaysClutter, _nGates);
    _dbzSqSum = _dbzSqSumArray.alloc(_nRaysClutter, _nGates);
    _dbzCount = _dbzCountArray.alloc(_nRaysClutter, _nGates);
    _dbzMean = _dbzMeanArray.alloc(_nRaysClutter, _nGates);
    _dbzSdev = _dbzSdevArray.alloc(_nRaysClutter, _nGates);
    _clutSum = _clutSumArray.alloc(_nRaysClutter, _nGates);
    _clutFreq = _clutFreqArray.alloc(_nRaysClutter, _nGates);
    _clutFlag = _clutFlagArray.alloc(_nRaysClutter, _nGates);

    // initialize to 0
    
    Radx::fl32 *dbzSum1D = _dbzSumArray.dat1D();
    memset(dbzSum1D, 0, _nRaysClutter * _nGates * sizeof(Radx::fl32));
    Radx::fl32 *dbzSqSum1D = _dbzSqSumArray.dat1D();
    memset(dbzSqSum1D, 0, _nRaysClutter * _nGates * sizeof(Radx::fl32));
    Radx::fl32 *dbzCount1D = _dbzCountArray.dat1D();
    memset(dbzCount1D, 0, _nRaysClutter * _nGates * sizeof(Radx::fl32));
    Radx::fl32 *dbzMean1D = _dbzMeanArray.dat1D();
    memset(dbzMean1D, 0, _nRaysClutter * _nGates * sizeof(Radx::fl32));
    Radx::fl32 *dbzSdev1D = _dbzSdevArray.dat1D();
    memset(dbzSdev1D, 0, _nRaysClutter * _nGates * sizeof(Radx::fl32));

    Radx::fl32 *clutSum1D = _clutSumArray.dat1D();
    memset(clutSum1D, 0, _nRaysClutter * _nGates * sizeof(Radx::fl32));
    Radx::fl32 *clutFreq1D = _clutFreqArray.dat1D();
    memset(clutFreq1D, 0, _nRaysClutter * _nGates * sizeof(Radx::fl32));
    Radx::fl32 *clutFlag1D = _clutFlagArray.dat1D();
    memset(clutFlag1D, 0, _nRaysClutter * _nGates * sizeof(Radx::fl32));

  } else {
    
    _clutterVol.setNGates(_nGates);
    
  }

  _nVols++;

  return 0;
  
}

//////////////////////////////////////////////////
// set up write

void RadxClutter::_setupWrite(RadxFile &file)
{

  if (_params.debug) {
    file.setDebug(true);
  }
  if (_params.debug >= Params::DEBUG_EXTRA) {
    file.setVerbose(true);
  }

  file.setWriteFileNameMode(RadxFile::FILENAME_WITH_START_AND_END_TIMES);

  file.setWriteCompressed(true);
  file.setCompressionLevel(4);

}

//////////////////////////////////////
// Write the clutter volume

int RadxClutter::_writeClutterVol()

{

  // output file

  GenericRadxFile outFile;
  _setupWrite(outFile);
  
  string outputDir = _params.clutter_stats_output_dir;
  
  // write to dir
  
  if (outFile.writeToDir(_clutterVol, outputDir, true, false)) {
    LOG(ERROR) << "ERROR - RadxConvert::_writeVol";
    LOG(ERROR) << "  Cannot write file to dir: " << outputDir;
    LOG(ERROR) << outFile.getErrStr();
    return -1;
  }

  string outputPath = outFile.getPathInUse();

  // write latest data info file if requested 
  
  if (_params.write_latest_data_info) {
    DsLdataInfo ldata(outputDir);
    if (_params.debug >= Params::DEBUG_VERBOSE) {
      ldata.setDebug(true);
    }
    string relPath;
    RadxPath::stripDir(outputDir, outputPath, relPath);
    ldata.setRelDataPath(relPath);
    ldata.setWriter(_progName);
    ldata.setDataFileExt("nc");
    ldata.setDataType("netCDF");
    
    string fileName;
    Path::stripDir(_params.clutter_stats_output_dir, outputPath, fileName);
    ldata.setRelDataPath(fileName);
    
    ldata.setIsFcast(false);
    ldata.write(_readVol.getStartTimeSecs());
    
    LOG(DEBUG) << "RadxClutter::_writeClutterVol(): Data written to "
               << outputPath;
    
  }

  return 0;

}

