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
///////////////////////////////////////////////////////////////
// TsDataMgr.cc
//
// Mike Dixon, RAP, NCAR, P.O.Box 3000, Boulder, CO, 80307-3000, USA
//
// Aug 2006
//
///////////////////////////////////////////////////////////////
//
// Data manager for time series data
// TsDataMgr analyses time series data from vertical scans
//
////////////////////////////////////////////////////////////////

#include "TsDataMgr.hh"
#include <toolsa/toolsa_macros.h>
#include <toolsa/TaArray.hh>
#include <Radx/RadxTime.hh>

using namespace std;

// Constructor

TsDataMgr::TsDataMgr(const string &prog_name,
                     const Args &args,
		     const Params &params,
		     StatsMgr &statsMgr) :
  _progName(prog_name),
  _args(args),
  _params(params),
  _statsMgr(statsMgr)
  
{

  _pulseSeqNum = 0;
  _totalPulseCount = 0;
  _reader = NULL;

  _nSamples = _params.n_samples;
  _nSamplesHalf = _nSamples / 2;

  _nGates = 0;
  _nGatesAlloc = 0;

  // set up max ht

  int nLayers = _params.n_layers;
  double startHt = _params.start_height;
  double deltaHt = _params.delta_height;
  _maxHt = startHt + (nLayers + 1) * deltaHt;

  switch (_params.xmit_rcv_mode) {
    
    case Params::DP_ALT_HV_CO_ONLY:
      _xmitRcvMode = IWRF_ALT_HV_CO_ONLY;
      break;
    case Params::DP_ALT_HV_CO_CROSS:
      _xmitRcvMode = IWRF_ALT_HV_CO_CROSS;
      break;
    case Params::DP_ALT_HV_FIXED_HV:
      _xmitRcvMode = IWRF_ALT_HV_FIXED_HV;
      break;
    case Params::DP_SIM_HV_FIXED_HV:
      _xmitRcvMode = IWRF_SIM_HV_FIXED_HV;
      break;
    case Params::DP_SIM_HV_SWITCHED_HV:
      _xmitRcvMode = IWRF_SIM_HV_SWITCHED_HV;
      break;
    default:
      _xmitRcvMode = IWRF_SIM_HV_FIXED_HV;
      
  } // switch

  _mom = NULL;
  _fields = NULL;

}

// destructor

TsDataMgr::~TsDataMgr()

{

  if (_reader) {
    delete _reader;
  }

  if (_mom) {
    delete _mom;
  }

  if (_fields) {
    delete _fields;
  }

  _clearPulseQueue();
  
}

//////////////////////////////////////////////////
// Run

int TsDataMgr::run ()
{

  // read in cal file

  string errStr;
  if (_calib.readFromXmlFile(_params.cal_xml_file_path, errStr)) {
    cerr << "ERROR - TsDataMgr::run" << endl;
    cerr << "  Cannot decode cal file: "
	 << _params.cal_xml_file_path << endl;
    return -1;
  }

  // create reader

  IwrfDebug_t iwrfDebug = IWRF_DEBUG_OFF;
  if (_params.debug) {
    iwrfDebug = IWRF_DEBUG_NORM;
  }
  
  if (_params.input_mode == Params::TS_FILE_INPUT) {
    
    // check if start and end times are set
    
    bool startTimeSet = true;
    time_t startTime = RadxTime::parseDateTime(_params.start_time);
    if (startTime == RadxTime::NEVER || startTime < 1) {
      startTimeSet = false;
    }
    
    bool endTimeSet = true;
    time_t endTime = RadxTime::parseDateTime(_params.end_time);
    if (endTime == RadxTime::NEVER || endTime < 1) {
      endTimeSet = false;
    }
    
    cerr << "  Input dir: " << _params.input_dir << endl;
    cerr << "  Start time: " << RadxTime::strm(startTime) << endl;
    cerr << "  End time: " << RadxTime::strm(endTime) << endl;
    
    vector<string> paths = _args.inputFileList;
    if (paths.size() == 0) {
      if (startTimeSet && endTimeSet) {
        if (_params.debug) {
          cerr << "  Input dir: " << _params.input_dir << endl;
          cerr << "  Start time: " << RadxTime::strm(startTime) << endl;
          cerr << "  End time: " << RadxTime::strm(endTime) << endl;
        }
        DsInputPath dsInput(_progName, _params.debug >= Params::DEBUG_VERBOSE,
                            _params.input_dir,
                            startTime, endTime);
        paths = dsInput.getPathList();
      } // if (startTimeSet && endTimeSet)
      
    }
    
    if (paths.size() < 1) {
      cerr << "ERROR - VertCompute::RadxDataMgr::run()" << endl;
      cerr << "  No files found, dir: " << _params.input_dir << endl;
      return -1;
    }

    _reader = new IwrfTsReaderFile(paths, iwrfDebug);
    
  } else {

    _reader = new IwrfTsReaderFmq(_params.input_fmq_name, iwrfDebug,
				   _params.seek_to_start_of_input);

  }

  int iret = 0;

  while (true) {

    const IwrfTsPulse *pulse = _reader->getNextPulse(true);
    if (pulse == NULL) {
      break;
    }
    
    if (_params.invert_hv_flag) {
      pulse->setInvertHvFlag();
    }
    _processPulse(pulse);
    
  }

  return iret;

}

/////////////////////
// process a pulse

void TsDataMgr::_processPulse(const IwrfTsPulse *pulse)

{

  // at start, print headers

  if (_totalPulseCount == 0) {
    _statsMgr.setStartTime(pulse->getFTime());
  }
  _statsMgr.setEndTime(pulse->getFTime());

  // check that we start with a horizontal pulse

  if (_xmitRcvMode == IWRF_ALT_HV_CO_ONLY ||
      _xmitRcvMode == IWRF_ALT_HV_CO_CROSS ||
      _xmitRcvMode == IWRF_ALT_HV_FIXED_HV) {
    if (_pulseQueue.size() == 0 && !pulse->isHoriz()) {
      return;
    }
  }

  // add the pulse to the queue
  
  _addPulseToQueue(pulse);
  _totalPulseCount++;

  // do we have a full pulse queue?

  if ((int) _pulseQueue.size() < _nSamples) {
    return;
  }

  // does the pulse az or el change drastically?
  
  int qSize = (int) _pulseQueue.size();
  double azStart = _pulseQueue[0]->getAz();
  double elStart = _pulseQueue[0]->getEl();
  double azEnd = _pulseQueue[qSize-1]->getAz();
  double elEnd = _pulseQueue[qSize-1]->getEl();
    
  double azDiff = RadarComplex::diffDeg(azStart, azEnd);
  double elDiff = RadarComplex::diffDeg(elStart, elEnd);
  if (fabs(azDiff) > 5 || fabs(elDiff) > 0.2) {
    if (_params.debug >= Params::DEBUG_EXTRA) {
      cerr << "====>> Clearing pulse queue" << endl;
      cerr << "  azStart, azEnd: " << azStart << ", " << azEnd << endl;
      cerr << "  elStart, elEnd: " << elStart << ", " << elEnd << endl;
    }
    _clearPulseQueue();
    return;
  }

  // find pulse at mid point of queue
  
  int midIndex = _nSamples / 2;
  const IwrfTsPulse *midPulse = _pulseQueue[midIndex];
  _statsMgr.setPrt(midPulse->getPrt());
  _statsMgr.setEl(midPulse->getEl());
  _statsMgr.setAz(midPulse->getAz());

  // transmitter power

  _measXmitPowerDbmH = midPulse->getMeasXmitPowerDbmH();
  _measXmitPowerDbmV = midPulse->getMeasXmitPowerDbmV();

  // compute the moments
  
  _computeMoments();

  // if we have done a full rotation, process the data

  _statsMgr.checkCompute();

  // clear time series data queue

  _clearPulseQueue();
  
}

/////////////////////////////////////////////////
// add the pulse to the pulse queue
    
void TsDataMgr::_addPulseToQueue(const IwrfTsPulse *pulse)
  
{

  // manage the size of the pulse queue, popping off the back

  if ((int) _pulseQueue.size() >= _nSamples) {
    const IwrfTsPulse *oldest = _pulseQueue.front();
    if (oldest->removeClient() == 0) {
      delete oldest;
    }
    _pulseQueue.pop_front();
  }

  int qSize = (int) _pulseQueue.size();
  if (qSize > 1) {

    // check for big azimuth or elevation change
    // if so clear the queue
    
    double az = pulse->getAz();
    double el = pulse->getEl();
    double prevAz = _pulseQueue[qSize-1]->getAz();
    double prevEl = _pulseQueue[qSize-1]->getEl();
    
    double azDiff = RadarComplex::diffDeg(az, prevAz);
    double elDiff = RadarComplex::diffDeg(el, prevEl);
    if (fabs(azDiff) > 0.1 || fabs(elDiff) > 0.1) {
      if (_params.debug >= Params::DEBUG_EXTRA) {
        cerr << "====>> Clearing pulse queue" << endl;
        cerr << "  az, prevAz: " << az << ", " << prevAz << endl;
        cerr << "  el, prevEl: " << el << ", " << prevEl << endl;
      }
      _clearPulseQueue();
      return;
    }

  }
    
  // push pulse onto front of queue
  
  pulse->addClient();
  _pulseQueue.push_back(pulse);

  // print missing pulses in verbose mode
  
  if ((int) pulse->getSeqNum() != _pulseSeqNum + 1) {
    if (_params.debug >= Params::DEBUG_EXTRA && _pulseSeqNum != 0) {
      cerr << "**** Missing seq num: " << _pulseSeqNum
	   << " to " <<  pulse->getSeqNum() << " ****" << endl;
    }
  }
  _pulseSeqNum = pulse->getSeqNum();

  // set _nGates to min number found so far

  qSize = (int) _pulseQueue.size();
  if (_nGates != 0) {
    if (_nGates > _pulseQueue[0]->getNGates()) {
      _nGates = _pulseQueue[0]->getNGates();
    }
  } else {
    _nGates = _pulseQueue[0]->getNGates();
  }

  // _nGates = _pulseQueue[0]->getNGates();
  // for (int ii = 1; ii < qSize; ii++) {
  //   if (_pulseQueue[ii]->getNGates() != _nGates) {
  //     _clearPulseQueue();
  //     return;
  //   }
  // }

}

/////////////////////////////////////////////////
// clear the pulse queue
    
void TsDataMgr::_clearPulseQueue()
  
{

  for (size_t ii = 0; ii < _pulseQueue.size(); ii++) {
    if (_pulseQueue[ii]->removeClient() == 0) {
      delete _pulseQueue[ii];
    }
  }
  _pulseQueue.clear();

}

////////////////////////////////////////////
// compute properties using pulses in queue

void TsDataMgr::_computeMoments()

{

  // initialize

  _initForMoments();

  // compute the moments

  switch (_xmitRcvMode) {
    
    case IWRF_ALT_HV_CO_ONLY:
      _computeDpAltHvCoOnly();
      break;
      
    case IWRF_ALT_HV_CO_CROSS:
    case IWRF_ALT_HV_FIXED_HV:
      _computeDpAltHvCoCross();
      break;
      
    case IWRF_SIM_HV_FIXED_HV:
    case IWRF_SIM_HV_SWITCHED_HV:
    default:
      _computeDpSimHv();
      
  }

  // add to stats

  for (int igate = 0; igate < _nGates; igate++) {
    double range = _startRange + igate * _gateSpacing;
    GateData *gate = _gateData[igate];
    const MomentsFields &flds = gate->fields;
    MomentData mdata;
    mdata.snr = flds.snr;
    mdata.snrhc = flds.snrhc;
    mdata.snrhx = flds.snrhx;
    mdata.snrvc = flds.snrvc;
    mdata.snrvx = flds.snrvx;
    mdata.dbm = flds.dbm;
    mdata.dbmhc = flds.dbmhc;
    mdata.dbmhx = flds.dbmhx;
    mdata.dbmvc = flds.dbmvc;
    mdata.dbmvx = flds.dbmvx;
    mdata.dbz = flds.dbz;
    mdata.vel = flds.vel;
    mdata.width = flds.width;
    mdata.zdrm = flds.zdrm;
    mdata.ldrh = flds.ldrh;
    mdata.ldrv = flds.ldrv;
    mdata.phidp = flds.phidp;
    mdata.rhohv = flds.rhohv;
    _statsMgr.addDataPoint(range, mdata);
  } // igate

}

///////////////////////////////////////////////////////////
// Compute moments DP_ALT_HV_CO_CROSS
// Transmit alternating, receive co/cross

void TsDataMgr::_computeDpAltHvCoCross()
{
  
  for (int igate = 0; igate < _nGates; igate++) {
    GateData *gate = _gateData[igate];
    _mom->dpAltHvCoCross(gate->iqhc, gate->iqvc, gate->iqhx,
                         gate->iqvx, igate, false, gate->fields);
  } // igate

}

///////////////////////////////////////////////////////////
// Compute moments DP_ALT_HV_CO_ONLY
// Transmit alternating, receive copolar only

void TsDataMgr::_computeDpAltHvCoOnly()
{

  for (int igate = 0; igate < _nGates; igate++) {
    GateData *gate = _gateData[igate];
    _mom->dpAltHvCoOnly(gate->iqhc, gate->iqvc, igate, false, gate->fields);
  } // igate

}

///////////////////////////////////////////////////////////
// Compute moments DP_SIM_HV_FIXED_HV
// Simultaneous transmission, fixed receive

void TsDataMgr::_computeDpSimHv()
{

  for (int igate = 0; igate < _nGates; igate++) {
    GateData *gate = _gateData[igate];
    _mom->dpSimHv(gate->iqhc, gate->iqvc, igate, false, gate->fields);
  } // igate

}

///////////////////////////////////////
// initialize ready for moments comps

void TsDataMgr::_initForMoments()
  
{

  // alloc fields at each gate
  
  if (_nGates > _nGatesAlloc) {
    
    if (_mom) delete _mom;

    _mom = new RadarMoments(_nGates,
                            _params.debug >= Params::DEBUG_NORM,
                            _params.debug >= Params::DEBUG_VERBOSE);

    if (_params.adjust_dbz_for_measured_xmit_power) {
      _mom->setAdjustDbzForMeasXmitPower();
    }
    if (_params.adjust_zdr_for_measured_xmit_power) {
      _mom->setAdjustZdrForMeasXmitPower();
    }

    _mom->setMeasXmitPowerDbmH(_measXmitPowerDbmH);
    _mom->setMeasXmitPowerDbmV(_measXmitPowerDbmV);
    
    _allocGateData(_nGates);
    _initFieldData();

    _nGatesAlloc = _nGates;

  }

  const IwrfTsInfo &opsInfo = _reader->getOpsInfo();

  // set range geometry
  
  _startRange = opsInfo.get_proc_start_range_km();
  _gateSpacing = opsInfo.get_proc_gate_spacing_km();

  // set moments object

  double prt = _pulseQueue[0]->getPrt();
  _mom->init(prt, opsInfo);
  _mom->setNSamples(_nSamples);
  _mom->setCorrectForSystemPhidp(true);
  _mom->setCalib(_calib);

  // set up data pointer arrays - channel 0

  TaArray<const fl32 *> iqChan0_;
  const fl32* *iqChan0 = iqChan0_.alloc(_nSamples);
  for (int ii = 0; ii < _nSamples; ii++) {
    iqChan0[ii] = _pulseQueue[ii]->getIq0();
  }
  
  // channel 1 - will be NULLs for single pol

  bool haveChan1 = true;
  for (int ii = 0; ii < _nSamples; ii++) {
    if (_pulseQueue[ii]->getIq1() == NULL) {
      haveChan1 = false;
      break;
    }
  }

  TaArray<const fl32 *> iqChan1_;
  const fl32* *iqChan1 = NULL;
  if (haveChan1) {
    iqChan1 = iqChan1_.alloc(_nSamples);
    for (int ii = 0; ii < _nSamples; ii++) {
      iqChan1[ii] = _pulseQueue[ii]->getIq1();
    }
  }

  _loadGateIq(iqChan0, iqChan1);


}
     
/////////////////////////////////////////////////////////////////
// Allocate or re-allocate gate data

void TsDataMgr::_allocGateData(int nGates)

{
  int nNeeded = nGates - (int) _gateData.size();
  if (nNeeded > 0) {
    for (int ii = 0; ii < nNeeded; ii++) {
      GateData *gate = new GateData();
      _gateData.push_back(gate);
    }
  }
  for (size_t ii = 0; ii < _gateData.size(); ii++) {
    _gateData[ii]->allocArrays(_nSamples, false, false, false);
  }
}

/////////////////////////////////////////////////////////////////
// Free gate data

void TsDataMgr::_freeGateData()

{
  for (int ii = 0; ii < (int) _gateData.size(); ii++) {
    delete _gateData[ii];
  }
  _gateData.clear();
}

/////////////////////////////////////////////////////////////////
// Initialize field data at each gate

void TsDataMgr::_initFieldData()
  
{
  for (int ii = 0; ii < (int) _gateData.size(); ii++) {
    _gateData[ii]->initFields();
  }
}

/////////////////////////////////////////////////////////////////
// Load gate IQ data.
// Apply window as appropriate.
// How this is loaded depends on the polarization mode.
//
// Assumptions:
// 1. For alternating mode, first pulse in sequence is H.
// 2. For non-switching dual receivers,
//    H is channel 0 and V channel 1.
// 3. For single pol mode, data is in channel 0.

void TsDataMgr::_loadGateIq(const fl32 **iqChan0,
                            const fl32 **iqChan1)
  
{
  
  switch (_xmitRcvMode) {
    
    case IWRF_ALT_HV_CO_ONLY: {
      
      // assumes first pulse is H xmit

      for (int igate = 0, ipos = 0; igate < _nGates; igate++, ipos += 2) {
        GateData *gate = _gateData[igate];
        RadarComplex_t *iqhc = gate->iqhc;
        RadarComplex_t *iqvc = gate->iqvc;
        for (int isamp = 0, jsamp = 0; isamp < _nSamplesHalf;
             isamp++, iqhc++, iqvc++) {
          iqhc->re = iqChan0[jsamp][ipos];
          iqhc->im = iqChan0[jsamp][ipos + 1];
          jsamp++;
          iqvc->re = iqChan0[jsamp][ipos];
          iqvc->im = iqChan0[jsamp][ipos + 1];
          jsamp++;
        }
      }
    
    } break;
        
    case IWRF_ALT_HV_CO_CROSS: {

      // assumes first pulse is H xmit
      
      for (int igate = 0, ipos = 0; igate < _nGates; igate++, ipos += 2) {
        GateData *gate = _gateData[igate];
        RadarComplex_t *iqhc = gate->iqhc;
        RadarComplex_t *iqvc = gate->iqvc;
        RadarComplex_t *iqhx = gate->iqhx;
        RadarComplex_t *iqvx = gate->iqvx;
        for (int isamp = 0, jsamp = 0; isamp < _nSamplesHalf;
             isamp++, iqhc++, iqvc++, iqhx++, iqvx++) {
          iqhc->re = iqChan0[jsamp][ipos];
          iqhc->im = iqChan0[jsamp][ipos + 1];
          if (iqChan1) {
            iqvx->re = iqChan1[jsamp][ipos];
            iqvx->im = iqChan1[jsamp][ipos + 1];
          }
          jsamp++;
          iqvc->re = iqChan0[jsamp][ipos];
          iqvc->im = iqChan0[jsamp][ipos + 1];
          if (iqChan1) {
            iqhx->re = iqChan1[jsamp][ipos];
            iqhx->im = iqChan1[jsamp][ipos + 1];
          }
          jsamp++;
        }
      }

    } break;

    case IWRF_ALT_HV_FIXED_HV: {

      // not switching
      for (int igate = 0, ipos = 0; igate < _nGates; igate++, ipos += 2) {
        GateData *gate = _gateData[igate];
        RadarComplex_t *iqhc = gate->iqhc;
        RadarComplex_t *iqvc = gate->iqvc;
        RadarComplex_t *iqhx = gate->iqhx;
        RadarComplex_t *iqvx = gate->iqvx;
        for (int isamp = 0, jsamp = 0; isamp < _nSamplesHalf;
             isamp++, iqhc++, iqvc++, iqhx++, iqvx++) {
          iqhc->re = iqChan0[jsamp][ipos];
          iqhc->im = iqChan0[jsamp][ipos + 1];
          if (iqChan1) {
            iqhx->re = iqChan1[jsamp][ipos];
            iqhx->im = iqChan1[jsamp][ipos + 1];
          }
          jsamp++;
          iqvx->re = iqChan0[jsamp][ipos];
          iqvx->im = iqChan0[jsamp][ipos + 1];
          if (iqChan1) {
            iqvc->re = iqChan1[jsamp][ipos];
            iqvc->im = iqChan1[jsamp][ipos + 1];
          }
          jsamp++;
        }
      }

    } break;

    case IWRF_SIM_HV_SWITCHED_HV:  {

      for (int igate = 0, ipos = 0; igate < _nGates; igate++, ipos += 2) {
        GateData *gate = _gateData[igate];
        RadarComplex_t *iqhc = gate->iqhc;
        RadarComplex_t *iqvc = gate->iqvc;
        for (int isamp = 0; isamp < _nSamples; isamp++, iqhc++, iqvc++) {
          if (isamp % 2 == 0) {
            iqhc->re = iqChan0[isamp][ipos];
            iqhc->im = iqChan0[isamp][ipos + 1];
            if (iqChan1) {
              iqvc->re = iqChan1[isamp][ipos];
              iqvc->im = iqChan1[isamp][ipos + 1];
            }
          } else {
            iqvc->re = iqChan0[isamp][ipos];
            iqvc->im = iqChan0[isamp][ipos + 1];
            if (iqChan1) {
              iqhc->re = iqChan1[isamp][ipos];
              iqhc->im = iqChan1[isamp][ipos + 1];
            }
          }
        }
      }
      
    } break;

    case IWRF_SIM_HV_FIXED_HV:    
    default:
      {

      for (int igate = 0, ipos = 0; igate < _nGates; igate++, ipos += 2) {
        GateData *gate = _gateData[igate];
        RadarComplex_t *iqhc = gate->iqhc;
        RadarComplex_t *iqvc = gate->iqvc;
        for (int isamp = 0; isamp < _nSamples; isamp++, iqhc++, iqvc++) {
          iqhc->re = iqChan0[isamp][ipos];
          iqhc->im = iqChan0[isamp][ipos + 1];
          if (iqChan1) {
            iqvc->re = iqChan1[isamp][ipos];
            iqvc->im = iqChan1[isamp][ipos + 1];
          }
        }
      }
      
    } break;

  } // switch;

}

