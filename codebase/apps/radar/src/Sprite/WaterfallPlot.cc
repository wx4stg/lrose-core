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
// WaterfallPlot.cc
//
// Plotting of spectra vs range
//
// Mike Dixon, EOL, NCAR, P.O.Box 3000, Boulder, CO, 80307-3000, USA
//
// April 2021
//
///////////////////////////////////////////////////////////////

#include <assert.h>
#include <cmath>
#include <iostream>
#include <fstream>
#include <toolsa/toolsa_macros.h>
#include <toolsa/DateTime.hh>
#include <toolsa/Path.hh>
#include <toolsa/TaArray.hh>
#include <toolsa/TaArray2D.hh>
#include <toolsa/pjg.h>
#include <toolsa/sincos.h>
#include <radar/FilterUtils.hh>

#include <QTimer>
#include <QBrush>
#include <QPalette>
#include <QPaintEngine>
#include <QPen>
#include <QResizeEvent>
#include <QStylePainter>
#include <algorithm>

#include "WaterfallPlot.hh"
#include "SpectraMgr.hh"
#include "Beam.hh"

double WaterfallPlot::missingVal = -9999.0;

using namespace std;

WaterfallPlot::WaterfallPlot(QWidget* parent,
                             const Params &params,
                             int id) :
        _parent(parent),
        _params(params),
        _id(id)
        
{
  _isZoomed = false;
  _xGridLinesOn = _params.waterfall_x_grid_lines_on;
  _yGridLinesOn = _params.waterfall_y_grid_lines_on;
  _plotType = Params::WATERFALL_HC;
  _fftWindow = Params::FFT_WINDOW_VONHANN;
  _useAdaptFilt = false;
  _clutWidthMps = 0.75;
  _useRegrFilt = false;
  _regrOrder = 3;
  _medianFiltLen = 3;
}

/*************************************************************************
 * Destructor
 */

WaterfallPlot::~WaterfallPlot()
{

}


/*************************************************************************
 * clear()
 */

void WaterfallPlot::clear()
{

}

/*************************************************************************
 * perform zoom
 */

void WaterfallPlot::zoom(int x1, int y1, int x2, int y2)
{

  _zoomWorld.setZoomLimits(x1, y1, x2, y2);
  _isZoomed = true;

}

/*************************************************************************
 * unzoom the view
 */

void WaterfallPlot::unzoom()
{

  _zoomWorld = _fullWorld;
  _isZoomed = false;

}

/*************************************************************************
 * plot a beam
 */

void WaterfallPlot::plotBeam(QPainter &painter,
                             Beam *beam,
                             int nSamples,
                             double selectedRangeKm)
  
{

  if (beam == NULL) {
    cerr << "WARNING - WaterfallPlot::plotBeam() - got NULL beam, ignoring"
         << endl;
    return;
  }
  
  if(_params.debug >= Params::DEBUG_VERBOSE) {
    cerr << "======== Waterfall - plotting beam data ================" << endl;
    DateTime beamTime(beam->getTimeSecs(), true, beam->getNanoSecs() * 1.0e-9);
    cerr << "  Beam time: " << beamTime.asString(3) << endl;
    cerr << "  Max range: " << beam->getMaxRange() << endl;
  }

  int nGates = beam->getNGates();
  if (_params.set_max_range) {
    int nGatesMax =
      (_params.max_range_km - beam->getStartRangeKm()) / beam->getGateSpacingKm();
    if (nGatesMax < nGates) {
      nGates = nGatesMax;
    }
  }

  // perform the relevant plot

  switch (_plotType) {

    case Params::WATERFALL_HC:
      _plotHc(painter, beam, nSamples, nGates, selectedRangeKm);
      break;
    case Params::WATERFALL_VC:
      _plotVc(painter, beam, nSamples, nGates, selectedRangeKm);
      break;
    case Params::WATERFALL_HX:
      _plotHx(painter, beam, nSamples, nGates, selectedRangeKm);
      break;
    case Params::WATERFALL_VX:
      _plotVx(painter, beam, nSamples, nGates, selectedRangeKm);
      break;
    case Params::WATERFALL_ZDR:
      _plotZdr(painter, beam, nSamples, nGates, selectedRangeKm);
      break;
    case Params::WATERFALL_PHIDP:
      _plotPhidp(painter, beam, nSamples, nGates, selectedRangeKm);
      break;
    case Params::WATERFALL_SDEV_ZDR:
      _plotSdevZdr(painter, beam, nSamples, nGates, selectedRangeKm);
      break;
    case Params::WATERFALL_SDEV_PHIDP:
      _plotSdevPhidp(painter, beam, nSamples, nGates, selectedRangeKm);
      break;
    case Params::WATERFALL_CMD:
      _plotCmd(painter, beam, nSamples, nGates, selectedRangeKm);
      break;
      
  }

  // draw the overlays

  _drawOverlays(painter, selectedRangeKm);

  // legends
  
  char text[1024];
  vector<string> legendsLeft;
  painter.setBackgroundMode(Qt::OpaqueMode);
  if (_useAdaptFilt) {
    snprintf(text, 1024, "Adapt filt, width: %.2f", _clutWidthMps);
    legendsLeft.push_back(text);
  } else if (_useRegrFilt) {
    snprintf(text, 1024, "Regr filt, order: %d", _regrOrder);
    legendsLeft.push_back(text);
  }
  if (_medianFiltLen > 1) {
    snprintf(text, 1024, "Median filt len: %d", _medianFiltLen);
    legendsLeft.push_back(text);
  }
  _zoomWorld.drawLegendsTopLeft(painter, legendsLeft);

  // switch (_plotType) {
  //   case Params::WATERFALL_HC: {
  //     vector<string> legendsRight;
  //     painter.setBackgroundMode(Qt::OpaqueMode);
  //     snprintf(text, 1024, "Total power (dBm): %.2f", 10.0 * log10(_powerUnfilt));
  //     legendsRight.push_back(text);
  //     if (_useAdaptFilt) {
  //       snprintf(text, 1024, "Filt power (dBm): %.2f", 10.0 * log10(_powerFilt));
  //       legendsRight.push_back(text);
  //       snprintf(text, 1024, "Clut power (dBm): %.2f", _powerClut);
  //       legendsRight.push_back(text);
  //     }
  //     _zoomWorld.drawLegendsTopRight(painter, legendsRight);
  //   }
  //   case Params::WATERFALL_VC:
  //   case Params::WATERFALL_HX:
  //   case Params::WATERFALL_VX:
  //   default: {}
  // }

  // draw the title

  painter.save();
  painter.setPen(_params.waterfall_title_color);
  string title("Waterfall:");
  title.append(getName(_plotType));
  _zoomWorld.drawTitleTopCenter(painter, title);
  painter.restore();

}

/*************************************************************************
 * plot HC spectrum
 */

void WaterfallPlot::_plotHc(QPainter &painter,
                            Beam *beam,
                            int nSamples,
                            int nGates,
                            double selectedRangeKm)
  
{

  double startRange = beam->getStartRangeKm();
  double gateSpacing = beam->getGateSpacingKm();

  // draw the color scale
  
  if (_readColorMap(_params.waterfall_dbm_color_scale_name) == 0) {
    _zoomWorld.drawColorScale(_cmap, painter,
                              _params.waterfall_color_scale_font_size);
  }
  
  painter.save();

  // loop through the gates
  
  for (int igate = 0; igate < nGates; igate++) {

    // set limits for plotting this gate
    
    double yy = startRange + gateSpacing * (igate-0.5);
    
    // get Iq data for this gate

    const GateData *gateData = beam->getGateData()[igate];
    TaArray<RadarComplex_t> iq_;
    RadarComplex_t *iq = iq_.alloc(nSamples);
    memcpy(iq, gateData->iqhcOrig, nSamples * sizeof(RadarComplex_t));
    // _powerUnfilt = RadarComplex::meanPower(iq, nSamples);
    
    // compute power spectrum
    
    TaArray<double> power_, dbm_;
    double *power = power_.alloc(nSamples);
    double *dbm = dbm_.alloc(nSamples);
    _computePowerSpectrum(beam, nSamples, iq, power, dbm);
    // _powerFilt = RadarComplex::meanPower(power, nSamples);
    // _powerClut = _powerUnfilt - _powerFilt;
    
    // apply 3-pt median filter
    
    FilterUtils::applyMedianFilter(dbm, nSamples, _medianFiltLen);
      
    // plot the samples
    
    for (int ii = 0; ii < nSamples; ii++) {
      
      // get color

      int red, green, blue;
      _cmap.dataColor(dbm[ii], red, green, blue);
      QColor color(red, green, blue);
      QBrush brush(color);
      
      // set x limits

      double xx = ii;
      
      // fill rectangle

      double width = 1.0;
      double height = gateSpacing;
      _zoomWorld.fillRectangle(painter, brush, xx, yy, width * 2, height * 2);

    } // ii

  } // igate
  
  painter.restore();

}

/*************************************************************************
 * plot HC spectrum
 */

void WaterfallPlot::_plotVc(QPainter &painter,
                            Beam *beam,
                            int nSamples,
                            int nGates,
                            double selectedRangeKm)
  
{

  double startRange = beam->getStartRangeKm();
  double gateSpacing = beam->getGateSpacingKm();

  // draw the color scale
  
  if (_readColorMap(_params.waterfall_dbm_color_scale_name) == 0) {
    _zoomWorld.drawColorScale(_cmap, painter,
                              _params.waterfall_color_scale_font_size);
  }
  
  painter.save();

  // loop through the gates
  
  for (int igate = 0; igate < nGates; igate++) {

    // set limits for plotting this gate
    
    double yy = startRange + gateSpacing * (igate-0.5);
    
    // get Iq data for this gate

    const GateData *gateData = beam->getGateData()[igate];
    TaArray<RadarComplex_t> iq_;
    RadarComplex_t *iq = iq_.alloc(nSamples);
    memcpy(iq, gateData->iqvcOrig, nSamples * sizeof(RadarComplex_t));
    
    // compute power spectrum
    
    TaArray<double> power_, dbm_;
    double *power = power_.alloc(nSamples);
    double *dbm = dbm_.alloc(nSamples);
    _computePowerSpectrum(beam, nSamples, iq, power, dbm);
    
    // apply 3-pt median filter
    
    FilterUtils::applyMedianFilter(dbm, nSamples, _medianFiltLen);
      
    // plot the samples
    
    for (int ii = 0; ii < nSamples; ii++) {
      
      // get color

      int red, green, blue;
      _cmap.dataColor(dbm[ii], red, green, blue);
      QColor color(red, green, blue);
      QBrush brush(color);
      
      // set x limits

      double xx = ii;
      
      // fill rectangle

      double width = 1.0;
      double height = gateSpacing;
      _zoomWorld.fillRectangle(painter, brush, xx, yy, width * 2, height * 2);

    } // ii

  } // igate
  
  painter.restore();

}

/*************************************************************************
 * plot Hx spectrum
 */

void WaterfallPlot::_plotHx(QPainter &painter,
                            Beam *beam,
                            int nSamples,
                            int nGates,
                            double selectedRangeKm)
  
{

  double startRange = beam->getStartRangeKm();
  double gateSpacing = beam->getGateSpacingKm();

  // draw the color scale
  
  if (_readColorMap(_params.waterfall_dbm_color_scale_name) == 0) {
    _zoomWorld.drawColorScale(_cmap, painter,
                              _params.waterfall_color_scale_font_size);
  }
  
  painter.save();

  // loop through the gates
  
  for (int igate = 0; igate < nGates; igate++) {

    // set limits for plotting this gate
    
    double yy = startRange + gateSpacing * (igate-0.5);
    
    // get Iq data for this gate

    const GateData *gateData = beam->getGateData()[igate];
    TaArray<RadarComplex_t> iq_;
    RadarComplex_t *iq = iq_.alloc(nSamples);
    memcpy(iq, gateData->iqhxOrig, nSamples * sizeof(RadarComplex_t));
    
    // compute power spectrum
    
    TaArray<double> power_, dbm_;
    double *power = power_.alloc(nSamples);
    double *dbm = dbm_.alloc(nSamples);
    _computePowerSpectrum(beam, nSamples, iq, power, dbm);
    
    // apply 3-pt median filter
    
    FilterUtils::applyMedianFilter(dbm, nSamples, _medianFiltLen);
      
    // plot the samples
    
    for (int ii = 0; ii < nSamples; ii++) {
      
      // get color

      int red, green, blue;
      _cmap.dataColor(dbm[ii], red, green, blue);
      QColor color(red, green, blue);
      QBrush brush(color);
      
      // set x limits

      double xx = ii;
      
      // fill rectangle

      double width = 1.0;
      double height = gateSpacing;
      _zoomWorld.fillRectangle(painter, brush, xx, yy, width * 2, height * 2);

    } // ii

  } // igate
  
  painter.restore();

}

/*************************************************************************
 * plot VX spectrum
 */

void WaterfallPlot::_plotVx(QPainter &painter,
                            Beam *beam,
                            int nSamples,
                            int nGates,
                            double selectedRangeKm)
  
{

  double startRange = beam->getStartRangeKm();
  double gateSpacing = beam->getGateSpacingKm();

  // draw the color scale
  
  if (_readColorMap(_params.waterfall_dbm_color_scale_name) == 0) {
    _zoomWorld.drawColorScale(_cmap, painter,
                              _params.waterfall_color_scale_font_size);
  }
  
  painter.save();

  // loop through the gates
  
  for (int igate = 0; igate < nGates; igate++) {

    // set limits for plotting this gate
    
    double yy = startRange + gateSpacing * (igate-0.5);
    
    // get Iq data for this gate

    const GateData *gateData = beam->getGateData()[igate];
    TaArray<RadarComplex_t> iq_;
    RadarComplex_t *iq = iq_.alloc(nSamples);
    memcpy(iq, gateData->iqvxOrig, nSamples * sizeof(RadarComplex_t));
    
    // compute power spectrum
    
    TaArray<double> power_, dbm_;
    double *power = power_.alloc(nSamples);
    double *dbm = dbm_.alloc(nSamples);
    _computePowerSpectrum(beam, nSamples, iq, power, dbm);
    
    // apply 3-pt median filter
    
    FilterUtils::applyMedianFilter(dbm, nSamples, _medianFiltLen);
      
    // plot the samples
    
    for (int ii = 0; ii < nSamples; ii++) {
      
      // get color

      int red, green, blue;
      _cmap.dataColor(dbm[ii], red, green, blue);
      QColor color(red, green, blue);
      QBrush brush(color);
      
      // set x limits

      double xx = ii;
      
      // fill rectangle

      double width = 1.0;
      double height = gateSpacing;
      _zoomWorld.fillRectangle(painter, brush, xx, yy, width * 2, height * 2);

    } // ii

  } // igate
  
  painter.restore();

}

/*************************************************************************
 * plot ZDR spectrum
 */

void WaterfallPlot::_plotZdr(QPainter &painter,
                             Beam *beam,
                             int nSamples,
                             int nGates,
                             double selectedRangeKm)
  
{

  double startRange = beam->getStartRangeKm();
  double gateSpacing = beam->getGateSpacingKm();

  // draw the color scale
  
  if (_readColorMap(_params.waterfall_zdr_color_scale_name) == 0) {
    _zoomWorld.drawColorScale(_cmap, painter,
                              _params.waterfall_color_scale_font_size);
  }
  
  painter.save();

  // loop through the gates
  
  for (int igate = 0; igate < nGates; igate++) {

    // set limits for plotting this gate
    
    double yy = startRange + gateSpacing * (igate-0.5);
    
    // get Iq data for this gate
    
    const GateData *gateData = beam->getGateData()[igate];
    
    // compute ZDR spectrum
    
    TaArray<double> powerHc_, dbmHc_;
    double *powerHc = powerHc_.alloc(nSamples);
    double *dbmHc = dbmHc_.alloc(nSamples);
    _computePowerSpectrum(beam, nSamples, gateData->iqhcOrig, powerHc, dbmHc);
    
    TaArray<double> powerVc_, dbmVc_;
    double *powerVc = powerVc_.alloc(nSamples);
    double *dbmVc = dbmVc_.alloc(nSamples);
    _computePowerSpectrum(beam, nSamples, gateData->iqvcOrig, powerVc, dbmVc);
    
    TaArray<double> zdr_;
    double *zdr = zdr_.alloc(nSamples);
    for (int ii = 0; ii < nSamples; ii++) {
      zdr[ii] = dbmHc[ii] - dbmVc[ii];
    }

    // apply 3-pt median filter
    
    FilterUtils::applyMedianFilter(zdr, nSamples, _medianFiltLen);
      
    // plot the samples
    
    for (int ii = 0; ii < nSamples; ii++) {
      
      // get color

      int red, green, blue;
      _cmap.dataColor(zdr[ii], red, green, blue);
      QColor color(red, green, blue);
      QBrush brush(color);
      
      // set x limits

      double xx = ii;
      
      // fill rectangle

      double width = 1.0;
      double height = gateSpacing;
      _zoomWorld.fillRectangle(painter, brush, xx, yy, width * 2, height * 2);

    } // ii

  } // igate
  
  painter.restore();

}

/*************************************************************************
 * plot PHIDP spectrum
 */

void WaterfallPlot::_plotPhidp(QPainter &painter,
                               Beam *beam,
                               int nSamples,
                               int nGates,
                               double selectedRangeKm)
  
{

  double startRange = beam->getStartRangeKm();
  double gateSpacing = beam->getGateSpacingKm();

  // draw the color scale
  
  if (_readColorMap(_params.waterfall_phidp_color_scale_name) == 0) {
    _zoomWorld.drawColorScale(_cmap, painter,
                              _params.waterfall_color_scale_font_size);
  }
  
  painter.save();

  // loop through the gates
  
  for (int igate = 0; igate < nGates; igate++) {

    // set limits for plotting this gate
    
    double yy = startRange + gateSpacing * (igate-0.5);
    
    // get Iq data for this gate
    
    const GateData *gateData = beam->getGateData()[igate];
    TaArray<RadarComplex_t> iqHc_, iqVc_;
    RadarComplex_t *iqHc = iqHc_.alloc(nSamples);
    RadarComplex_t *iqVc = iqVc_.alloc(nSamples);
    memcpy(iqHc, gateData->iqhcOrig, nSamples * sizeof(RadarComplex_t));
    memcpy(iqVc, gateData->iqvcOrig, nSamples * sizeof(RadarComplex_t));
    
    // apply window to time series
    
    TaArray<RadarComplex_t> iqWindowedHc_, iqWindowedVc_;
    RadarComplex_t *iqWindowedHc = iqWindowedHc_.alloc(nSamples);
    RadarComplex_t *iqWindowedVc = iqWindowedVc_.alloc(nSamples);
    _applyWindow(iqHc, iqWindowedHc, nSamples);
    _applyWindow(iqVc, iqWindowedVc, nSamples);
    
    // compute spectra
    
    TaArray<RadarComplex_t> specHc_, specVc_;
    RadarComplex_t *specHc = specHc_.alloc(nSamples);
    RadarComplex_t *specVc = specVc_.alloc(nSamples);
    RadarFft fft(nSamples);
    fft.fwd(iqWindowedHc, specHc);
    fft.shift(specHc);
    fft.fwd(iqWindowedVc, specVc);
    fft.shift(specVc);

    // compute phidp spectrum

    TaArray<double> phidp_;
    double *phidp = phidp_.alloc(nSamples);
    for (int ii = 0; ii < nSamples; ii++) {
      RadarComplex_t diff = RadarComplex::conjugateProduct(specHc[ii], specVc[ii]);
      phidp[ii] = RadarComplex::argDeg(diff);
    }
    
    // apply 3-pt median filter
    
    FilterUtils::applyMedianFilter(phidp, nSamples, _medianFiltLen);
      
    // plot the samples
    
    for (int ii = 0; ii < nSamples; ii++) {
      
      // get color

      int red, green, blue;
      _cmap.dataColor(phidp[ii], red, green, blue);
      QColor color(red, green, blue);
      QBrush brush(color);
      
      // set x limits

      double xx = ii;
      
      // fill rectangle

      double width = 1.0;
      double height = gateSpacing;
      _zoomWorld.fillRectangle(painter, brush, xx, yy, width * 2, height * 2);

    } // ii

  } // igate
  
  painter.restore();

}

/*************************************************************************
 * plot SDEV of ZDR spectrum
 */

void WaterfallPlot::_plotSdevZdr(QPainter &painter,
                                 Beam *beam,
                                 int nSamples,
                                 int nGates,
                                 double selectedRangeKm)
  
{

  double startRange = beam->getStartRangeKm();
  double gateSpacing = beam->getGateSpacingKm();

  // draw the color scale
  
  if (_readColorMap(_params.waterfall_sdev_zdr_color_scale_name) == 0) {
    _zoomWorld.drawColorScale(_cmap, painter,
                              _params.waterfall_color_scale_font_size);
  }
  
  // load up 2D array of spectral zdr

  TaArray2D<double> zdr2D_;
  double **zdr2D = zdr2D_.alloc(nGates, nSamples);

  double minVal = 9999.0;
  double maxVal = -9999.0;

  for (int igate = 0; igate < nGates; igate++) {

    // get Iq data for this gate
    
    const GateData *gateData = beam->getGateData()[igate];
    
    // compute ZDR spectrum
    
    TaArray<double> powerHc_, dbmHc_;
    double *powerHc = powerHc_.alloc(nSamples);
    double *dbmHc = dbmHc_.alloc(nSamples);
    _computePowerSpectrum(beam, nSamples, gateData->iqhcOrig, powerHc, dbmHc);
    
    TaArray<double> powerVc_, dbmVc_;
    double *powerVc = powerVc_.alloc(nSamples);
    double *dbmVc = dbmVc_.alloc(nSamples);
    _computePowerSpectrum(beam, nSamples, gateData->iqvcOrig, powerVc, dbmVc);
    
    for (int isample = 0; isample < nSamples; isample++) {
      double zdr = dbmHc[isample] - dbmVc[isample];
      zdr2D[igate][isample] = zdr;
    } // isample

  } // igate
  
  // compute 2D sdev of spectral zdr
  
  TaArray2D<double> sdev2D_;
  double **sdev2D = sdev2D_.alloc(nGates, nSamples);
  int nSamplesSdev = min(_params.waterfall_sdev_zdr_kernel_nsamples, nSamples);
  int nGatesSdev = min(_params.waterfall_sdev_zdr_kernel_ngates, nGates);
  int nSamplesSdevHalf = nSamplesSdev / 2;
  int nGatesSdevHalf = nGatesSdev / 2;
  
  for (int igate = 0; igate < nGates; igate++) {
    for (int isample = 0; isample < nSamples; isample++) {
      
      // compute index limits for computing sdev

      int sampleStart = isample - nSamplesSdevHalf;
      sampleStart = max(0, sampleStart);
      int sampleEnd = sampleStart + nSamplesSdev;
      sampleEnd = min(nSamples, sampleEnd);
      sampleStart = sampleEnd - nSamplesSdev;

      int gateStart = igate - nGatesSdevHalf;
      gateStart = max(0, gateStart);
      int gateEnd = gateStart + nGatesSdev;
      gateEnd = min(nGates, gateEnd);
      gateStart = gateEnd - nGatesSdev;

      // load up zdr values for kernel region

      vector<double> zdrKernel;
      for (int jgate = gateStart; jgate < gateEnd; jgate++) {
        for (int jsample = sampleStart; jsample < sampleEnd; jsample++) {
          zdrKernel.push_back(zdr2D[jgate][jsample]);
        } // jsample
      } // jgate

      // compute sdev of zdr
      
      double zdrSdev = _computeSdevZdr(zdrKernel);
      sdev2D[igate][isample] = zdrSdev;
      
      minVal = min(minVal, zdrSdev);
      maxVal = max(maxVal, zdrSdev);
      
    } // isample
  } // igate

  // plot the data

  painter.save();

  for (int igate = 0; igate < nGates; igate++) {
    double yy = startRange + gateSpacing * (igate-0.5);
    for (int isample = 0; isample < nSamples; isample++) {
      // get color
      int red, green, blue;
      _cmap.dataColor(sdev2D[igate][isample], red, green, blue);
      QColor color(red, green, blue);
      QBrush brush(color);
      // set x limits
      double xx = isample;
      // fill rectangle
      double width = 1.0;
      double height = gateSpacing;
      _zoomWorld.fillRectangle(painter, brush, xx, yy, width * 2, height * 2);
    } // isample
  } // igate
  
  painter.restore();

}

/*************************************************************************
 * plot sdev of PHIDP spectrum
 */

void WaterfallPlot::_plotSdevPhidp(QPainter &painter,
                                   Beam *beam,
                                   int nSamples,
                                   int nGates,
                                   double selectedRangeKm)
  
{

  double startRange = beam->getStartRangeKm();
  double gateSpacing = beam->getGateSpacingKm();

  // draw the color scale
  
  if (_readColorMap(_params.waterfall_sdev_phidp_color_scale_name) == 0) {
    _zoomWorld.drawColorScale(_cmap, painter,
                              _params.waterfall_color_scale_font_size);
  }
  
  // load up 2D array of spectral phidp

  TaArray2D<double> phidp2D_;
  double **phidp2D = phidp2D_.alloc(nGates, nSamples);

  double minVal = 9999.0;
  double maxVal = -9999.0;

  for (int igate = 0; igate < nGates; igate++) {

    // get Iq data for this gate
    
    const GateData *gateData = beam->getGateData()[igate];
    
    // apply window to time series
    
    TaArray<RadarComplex_t> iqWindowedHc_, iqWindowedVc_;
    RadarComplex_t *iqWindowedHc = iqWindowedHc_.alloc(nSamples);
    RadarComplex_t *iqWindowedVc = iqWindowedVc_.alloc(nSamples);
    _applyWindow(gateData->iqhcOrig, iqWindowedHc, nSamples);
    _applyWindow(gateData->iqvcOrig, iqWindowedVc, nSamples);
    
    // compute spectra
    
    TaArray<RadarComplex_t> specHc_, specVc_;
    RadarComplex_t *specHc = specHc_.alloc(nSamples);
    RadarComplex_t *specVc = specVc_.alloc(nSamples);
    RadarFft fft(nSamples);
    fft.fwd(iqWindowedHc, specHc);
    fft.shift(specHc);
    fft.fwd(iqWindowedVc, specVc);
    fft.shift(specVc);

    // compute phidp spectrum, store in 2D array

    for (int isample = 0; isample < nSamples; isample++) {
      RadarComplex_t diff = RadarComplex::conjugateProduct(specHc[isample], specVc[isample]);
      phidp2D[igate][isample] = RadarComplex::argDeg(diff);
    } // isample
    
  } // igate

  // compute the phidp folding range

  _computePhidpFoldingRange(nGates, nSamples, phidp2D);

  // compute 2D sdev of spectral phidp

  TaArray2D<double> sdev2D_;
  double **sdev2D = sdev2D_.alloc(nGates, nSamples);
  int nSamplesSdev = min(_params.waterfall_sdev_phidp_kernel_nsamples, nSamples);
  int nGatesSdev = min(_params.waterfall_sdev_phidp_kernel_ngates, nGates);
  int nSamplesSdevHalf = nSamplesSdev / 2;
  int nGatesSdevHalf = nGatesSdev / 2;
  
  for (int igate = 0; igate < nGates; igate++) {
    for (int isample = 0; isample < nSamples; isample++) {

      // compute index limits for computing sdev

      int sampleStart = isample - nSamplesSdevHalf;
      sampleStart = max(0, sampleStart);
      int sampleEnd = sampleStart + nSamplesSdev;
      sampleEnd = min(nSamples, sampleEnd);
      sampleStart = sampleEnd - nSamplesSdev;

      int gateStart = igate - nGatesSdevHalf;
      gateStart = max(0, gateStart);
      int gateEnd = gateStart + nGatesSdev;
      gateEnd = min(nGates, gateEnd);
      gateStart = gateEnd - nGatesSdev;

      // load up phidp values for kernel region

      vector<double> phidpKernel;
      for (int jgate = gateStart; jgate < gateEnd; jgate++) {
        for (int jsample = sampleStart; jsample < sampleEnd; jsample++) {
          phidpKernel.push_back(phidp2D[jgate][jsample]);
        } // jsample
      } // jgate

      // compute sdev of phidp

      double phidpSdev = _computeSdevPhidp(phidpKernel);
      sdev2D[igate][isample] = phidpSdev;
      
      minVal = min(minVal, phidpSdev);
      maxVal = max(maxVal, phidpSdev);
      
    } // isample
  } // igate

  // plot the data

  painter.save();

  for (int igate = 0; igate < nGates; igate++) {
    double yy = startRange + gateSpacing * (igate-0.5);
    for (int isample = 0; isample < nSamples; isample++) {
      // get color
      int red, green, blue;
      _cmap.dataColor(sdev2D[igate][isample], red, green, blue);
      QColor color(red, green, blue);
      QBrush brush(color);
      // set x limits
      double xx = isample;
      // fill rectangle
      double width = 1.0;
      double height = gateSpacing;
      _zoomWorld.fillRectangle(painter, brush, xx, yy, width * 2, height * 2);
    } // isample
  } // igate
  
  painter.restore();

}

/*************************************************************************
 * plot CMD spectrum
 */

void WaterfallPlot::_plotCmd(QPainter &painter,
                             Beam *beam,
                             int nSamples,
                             int nGates,
                             double selectedRangeKm)
  
{

  double startRange = beam->getStartRangeKm();
  double gateSpacing = beam->getGateSpacingKm();

  // draw the color scale
  
  if (_readColorMap(_params.waterfall_cmd_color_scale_name) == 0) {
    _zoomWorld.drawColorScale(_cmap, painter,
                              _params.waterfall_color_scale_font_size);
  }
  
  // get the noise value
  
  const IwrfCalib &calib = beam->getCalib();
  double calibNoiseHcDb = calib.getNoiseDbmHc();
  
  // load up 2D arrays of spectral power, zdr and phidp

  TaArray2D<double> powerHc2D_;
  double **powerHc2D = powerHc2D_.alloc(nGates, nSamples);
  TaArray2D<double> zdr2D_;
  double **zdr2D = zdr2D_.alloc(nGates, nSamples);
  TaArray2D<double> phidp2D_;
  double **phidp2D = phidp2D_.alloc(nGates, nSamples);

  for (int igate = 0; igate < nGates; igate++) {

    // get Iq data for this gate
    
    const GateData *gateData = beam->getGateData()[igate];
    
    // compute ZDR spectrum
    
    TaArray<double> powerHc_, dbmHc_;
    double *powerHc = powerHc_.alloc(nSamples);
    double *dbmHc = dbmHc_.alloc(nSamples);
    _computePowerSpectrum(beam, nSamples, gateData->iqhcOrig, powerHc, dbmHc);
    
    TaArray<double> powerVc_, dbmVc_;
    double *powerVc = powerVc_.alloc(nSamples);
    double *dbmVc = dbmVc_.alloc(nSamples);
    _computePowerSpectrum(beam, nSamples, gateData->iqvcOrig, powerVc, dbmVc);
    
    for (int isample = 0; isample < nSamples; isample++) {
      double zdr = dbmHc[isample] - dbmVc[isample];
      zdr2D[igate][isample] = zdr;
    } // isample

    // apply window to time series
    
    TaArray<RadarComplex_t> iqWindowedHc_, iqWindowedVc_;
    RadarComplex_t *iqWindowedHc = iqWindowedHc_.alloc(nSamples);
    RadarComplex_t *iqWindowedVc = iqWindowedVc_.alloc(nSamples);
    _applyWindow(gateData->iqhcOrig, iqWindowedHc, nSamples);
    _applyWindow(gateData->iqvcOrig, iqWindowedVc, nSamples);
    
    // compute spectra
    
    TaArray<RadarComplex_t> specHc_, specVc_;
    RadarComplex_t *specHc = specHc_.alloc(nSamples);
    RadarComplex_t *specVc = specVc_.alloc(nSamples);
    RadarFft fft(nSamples);
    fft.fwd(iqWindowedHc, specHc);
    fft.shift(specHc);
    fft.fwd(iqWindowedVc, specVc);
    fft.shift(specVc);

    // compute phidp spectrum, store in 2D array

    for (int isample = 0; isample < nSamples; isample++) {
      RadarComplex_t diff = RadarComplex::conjugateProduct(specHc[isample], specVc[isample]);
      phidp2D[igate][isample] = RadarComplex::argDeg(diff);
      powerHc2D[igate][isample] = RadarComplex::power(specHc[isample]);
    } // isample
    
  } // igate

  // compute the phidp folding range

  _computePhidpFoldingRange(nGates, nSamples, phidp2D);

  // compute 2D sdev of spectral zdr and phidp

  TaArray2D<double> sdevZdr2D_;
  double **sdevZdr2D = sdevZdr2D_.alloc(nGates, nSamples);
  TaArray2D<double> sdevPhidp2D_;
  double **sdevPhidp2D = sdevPhidp2D_.alloc(nGates, nSamples);
  int nSamplesSdev = min(_params.waterfall_sdev_phidp_kernel_nsamples, nSamples);
  int nGatesSdev = min(_params.waterfall_sdev_phidp_kernel_ngates, nGates);
  int nSamplesSdevHalf = nSamplesSdev / 2;
  int nGatesSdevHalf = nGatesSdev / 2;
  
  for (int igate = 0; igate < nGates; igate++) {
    for (int isample = 0; isample < nSamples; isample++) {

      // compute index limits for computing sdev

      int sampleStart = isample - nSamplesSdevHalf;
      sampleStart = max(0, sampleStart);
      int sampleEnd = sampleStart + nSamplesSdev;
      sampleEnd = min(nSamples, sampleEnd);
      sampleStart = sampleEnd - nSamplesSdev;

      int gateStart = igate - nGatesSdevHalf;
      gateStart = max(0, gateStart);
      int gateEnd = gateStart + nGatesSdev;
      gateEnd = min(nGates, gateEnd);
      gateStart = gateEnd - nGatesSdev;

      // load up zdr values for kernel region

      vector<double> zdrKernel;
      for (int jgate = gateStart; jgate < gateEnd; jgate++) {
        for (int jsample = sampleStart; jsample < sampleEnd; jsample++) {
          zdrKernel.push_back(zdr2D[jgate][jsample]);
        } // jsample
      } // jgate

      // compute sdev of zdr
      
      double zdrSdev = _computeSdevZdr(zdrKernel);
      sdevZdr2D[igate][isample] = zdrSdev;
      
      // load up phidp values for kernel region

      vector<double> phidpKernel;
      for (int jgate = gateStart; jgate < gateEnd; jgate++) {
        for (int jsample = sampleStart; jsample < sampleEnd; jsample++) {
          phidpKernel.push_back(phidp2D[jgate][jsample]);
        } // jsample
      } // jgate

      // compute sdev of phidp

      double phidpSdev = _computeSdevPhidp(phidpKernel);
      sdevPhidp2D[igate][isample] = phidpSdev;
      
    } // isample
  } // igate

  // compute CMD

  TaArray2D<double> cmd2D_;
  double **cmd2D = cmd2D_.alloc(nGates, nSamples);

  for (int igate = 0; igate < nGates; igate++) {
    for (int isample = 0; isample < nSamples; isample++) {

      double powerHc = powerHc2D[igate][isample];
      double sdevZdr = sdevZdr2D[igate][isample];
      double sdevPhidp = sdevPhidp2D[igate][isample];

      double powerHcDb = 10.0 * log10(powerHc);
      double snrDb = powerHcDb - calibNoiseHcDb;

      double snrInterest = 0.0;
      if (snrDb > 50) {
        snrInterest = 1.0;
      } else if (snrDb < 0) {
        snrInterest = 0.0;
      } else {
        snrInterest = (snrDb / 50);
      }

      double zdrInterest = 0.0;
      if (sdevZdr > 8.0) {
        zdrInterest = 1.0;
      } else {
        zdrInterest = sdevZdr / 8.0;
      }

      double phidpInterest = 0.0;
      if (sdevPhidp > 64.0) {
        phidpInterest = 1.0;
      } else {
        phidpInterest = sdevPhidp / 64.0;
      }

      double cmd = (snrInterest + zdrInterest + phidpInterest) / 3.0;
      cmd2D[igate][isample] = cmd;
      
      // double maxZdrPhidpInterest = max(zdrInterest, phidpInterest);
      // double cmd = (snrInterest + maxZdrPhidpInterest) / 2.0;


    } // isample
  } // igate

  // plot the data
  
  painter.save();

  for (int igate = 0; igate < nGates; igate++) {
    double yy = startRange + gateSpacing * (igate-0.5);
    for (int isample = 0; isample < nSamples; isample++) {
      // get color
      int red, green, blue;
      _cmap.dataColor(cmd2D[igate][isample], red, green, blue);
      QColor color(red, green, blue);
      QBrush brush(color);
      // set x limits
      double xx = isample;
      // fill rectangle
      double width = 1.0;
      double height = gateSpacing;
      _zoomWorld.fillRectangle(painter, brush, xx, yy, width * 2, height * 2);
    } // isample
  } // igate
  
  painter.restore();

}

/*************************************************************************
 * compute the spectrum
 */

void WaterfallPlot::_computePowerSpectrum(Beam *beam,
                                          int nSamples,
                                          const RadarComplex_t *iq,
                                          double *power,
                                          double *dbm)
  
{

  // init
  
  for (int ii = 0; ii < nSamples; ii++) {
    power[ii] = 1.0e-12;
    dbm[ii] = -120.0;
  }

  // apply window to time series
  
  TaArray<RadarComplex_t> iqWindowed_;
  RadarComplex_t *iqWindowed = iqWindowed_.alloc(nSamples);
  _applyWindow(iq, iqWindowed, nSamples);
  
  // compute power spectrum
  
  TaArray<RadarComplex_t> powerSpec_;
  RadarComplex_t *powerSpec = powerSpec_.alloc(nSamples);
  RadarFft fft(nSamples);
  fft.fwd(iqWindowed, powerSpec);
  fft.shift(powerSpec);
  
  // set up filtering
  
  const IwrfCalib &calib = beam->getCalib();
  double calibNoise = 0.0;
  switch (_plotType) {
    case Params::WATERFALL_HC:
    case Params::WATERFALL_ZDR:
    case Params::WATERFALL_SDEV_ZDR:
    case Params::WATERFALL_PHIDP:
    case Params::WATERFALL_SDEV_PHIDP:
    case Params::WATERFALL_CMD:
      calibNoise = pow(10.0, calib.getNoiseDbmHc() / 10.0);
      break;
    case Params::WATERFALL_VC:
      calibNoise = pow(10.0, calib.getNoiseDbmVc() / 10.0);
      break;
    case Params::WATERFALL_HX:
      calibNoise = pow(10.0, calib.getNoiseDbmHx() / 10.0);
      break;
    case Params::WATERFALL_VX:
      calibNoise = pow(10.0, calib.getNoiseDbmVx() / 10.0);
      break;
  }
  
  RadarMoments moments;
  moments.setNSamples(nSamples);
  moments.init(beam->getPrt(), calib.getWavelengthCm() / 100.0,
               beam->getStartRangeKm(), beam->getGateSpacingKm());
  moments.setCalib(calib);
  moments.setClutterWidthMps(_clutWidthMps);
  moments.setClutterInitNotchWidthMps(3.0);
  
  // filter as appropriate
  
  if (_useAdaptFilt) {

    // adaptive filtering takes precedence over regression
    
    TaArray<RadarComplex_t> filtAdaptWindowed_;
    RadarComplex_t *filtAdaptWindowed = filtAdaptWindowed_.alloc(nSamples);
    double filterRatio, spectralNoise, spectralSnr;
    moments.applyAdaptiveFilter(nSamples, fft,
                                iqWindowed, NULL,
                                calibNoise,
                                filtAdaptWindowed, NULL,
                                filterRatio,
                                spectralNoise,
                                spectralSnr);
    
    TaArray<RadarComplex_t> filtAdaptSpec_;
    RadarComplex_t *filtAdaptSpec = filtAdaptSpec_.alloc(nSamples);
    fft.fwd(filtAdaptWindowed, filtAdaptSpec);
    fft.shift(filtAdaptSpec);
    
    for (int ii = 0; ii < nSamples; ii++) {
      power[ii] = RadarComplex::power(filtAdaptSpec[ii]);
    }
    
  } else if (_useRegrFilt) {

    // regression
    
    RegressionFilter regrF;
    if (beam->getIsStagPrt()) {
      regrF.setupStaggered(nSamples, beam->getStagM(),
                           beam->getStagN(), _regrOrder);
    } else {
      regrF.setup(nSamples, _regrOrder);
    }
    
    TaArray<RadarComplex_t> filtered_;
    RadarComplex_t *filtered = filtered_.alloc(nSamples);
    double filterRatio, spectralNoise, spectralSnr;
    moments.applyRegressionFilter(nSamples, fft,
                                  regrF, _windowCoeff,
                                  iq,
                                  calibNoise,
                                  true,
                                  filtered, NULL,
                                  filterRatio,
                                  spectralNoise,
                                  spectralSnr);
    
    TaArray<RadarComplex_t> filtRegrWindowed_;
    RadarComplex_t *filtRegrWindowed = filtRegrWindowed_.alloc(nSamples);
    _applyWindow(filtered, filtRegrWindowed, nSamples);
    
    TaArray<RadarComplex_t> filtRegrSpec_;
    RadarComplex_t *filtRegrSpec = filtRegrSpec_.alloc(nSamples);
    fft.fwd(filtRegrWindowed, filtRegrSpec);
    fft.shift(filtRegrSpec);
    
    for (int ii = 0; ii < nSamples; ii++) {
      power[ii] = RadarComplex::power(filtRegrSpec[ii]);
    }
    
  } else {

    // no filtering
    
    for (int ii = 0; ii < nSamples; ii++) {
      power[ii] = RadarComplex::power(powerSpec[ii]);
    }
    
  }

  // compute dbm
  
  for (int ii = 0; ii < nSamples; ii++) {
    if (power[ii] <= 1.0e-12) {
      dbm[ii] = -120.0;
    } else {
      dbm[ii] = 10.0 * log10(power[ii]);
    }
  }

}

//////////////////////////////////
// get a string for the field name

string WaterfallPlot::getName(Params::waterfall_type_t wtype)
{
  switch (wtype) {
    case Params::WATERFALL_HC:
      return "HC";
    case Params::WATERFALL_VC:
      return "VC";
    case Params::WATERFALL_HX:
      return "HX";
    case Params::WATERFALL_VX:
      return "VX";
    case Params::WATERFALL_ZDR:
      return "ZDR";
    case Params::WATERFALL_PHIDP:
      return "PHIDP";
    case Params::WATERFALL_SDEV_ZDR:
      return "SDEV_ZDR";
    case Params::WATERFALL_SDEV_PHIDP:
      return "SDEV_PHIDP";
    case Params::WATERFALL_CMD:
      return "SDEV_CMD";
    default:
      return "UNKNOWN";
  }
}

//////////////////////////////////
// get a string for the field units

string WaterfallPlot::getUnits(Params::waterfall_type_t wtype)
{
  switch (wtype) {
    case Params::WATERFALL_HC:
    case Params::WATERFALL_VC:
    case Params::WATERFALL_HX:
    case Params::WATERFALL_VX:
      return "dBm";
    case Params::WATERFALL_ZDR:
    case Params::WATERFALL_SDEV_ZDR:
      return "dB";
    case Params::WATERFALL_PHIDP:
    case Params::WATERFALL_SDEV_PHIDP:
      return "deg";
    case Params::WATERFALL_CMD:
    default:
      return "";
  }
}

/*************************************************************************
 * set the geometry - unzooms
 */

void WaterfallPlot::setWindowGeom(int width, int height,
                                  int xOffset, int yOffset)
{
  _fullWorld.setWindowGeom(width, height, xOffset, yOffset);
  _fullWorld.setColorScaleWidth(_params.waterfall_color_scale_width);
  _zoomWorld = _fullWorld;
}

/*************************************************************************
 * set the world limits - unzooms
 */

void WaterfallPlot::setWorldLimits(double xMinWorld,
                                   double yMinWorld,
                                   double xMaxWorld,
                                   double yMaxWorld)
{
  _fullWorld.setWorldLimits(xMinWorld, yMinWorld,
                            xMaxWorld, yMaxWorld);
  _fullWorld.setColorScaleWidth(_params.waterfall_color_scale_width);
  _zoomWorld = _fullWorld;
}

/*************************************************************************
 * set the zoom limits, from pixel space
 */

void WaterfallPlot::setZoomLimits(int xMin,
                                  int yMin,
                                  int xMax,
                                  int yMax)
{
  _zoomWorld.setZoomLimits(xMin, yMin, xMax, yMax);
  _isZoomed = true;
}

void WaterfallPlot::setZoomLimitsX(int xMin,
                                   int xMax)
{
  _zoomWorld.setZoomLimitsX(xMin, xMax);
  _isZoomed = true;
}

void WaterfallPlot::setZoomLimitsY(int yMin,
                                   int yMax)
{
  _zoomWorld.setZoomLimitsY(yMin, yMax);
  _isZoomed = true;
}

/*************************************************************************
 * Protected methods
 *************************************************************************/

/*************************************************************************
 * Draw the overlays, axes, legends etc
 */

void WaterfallPlot::_drawOverlays(QPainter &painter, double selectedRangeKm)
{

  // save painter state
  
  painter.save();
  
  // store font
  
  QFont origFont = painter.font();
  
  painter.setPen(_params.waterfall_axis_label_color);

  _zoomWorld.drawAxisBottom(painter, "sample",
                            true, true, true, _xGridLinesOn);
  
  _zoomWorld.drawAxisLeft(painter, "km", 
                          true, true, true, _yGridLinesOn);

  _zoomWorld.drawYAxisLabelLeft(painter, "Range");

  // selected range line
  
  QPen pen(painter.pen());
  pen.setColor(_params.waterfall_selected_range_color);
  pen.setStyle(Qt::SolidLine);
  pen.setWidth(2);
  painter.setPen(pen);
  _zoomWorld.drawLine(painter,
                      _zoomWorld.getXMinWorld(), selectedRangeKm,
                      _zoomWorld.getXMaxWorld(), selectedRangeKm);

  painter.restore();

}

//////////////////////////////////////////////////
// read color map
// returns 0 on success, -1 on failure
  
int WaterfallPlot::_readColorMap(string colorScaleName)
{
  
  // check for color map location
  
  string colorMapDir = _params.color_scale_dir;
  Path mapDir(_params.color_scale_dir);
  if (!mapDir.dirExists()) {
    colorMapDir = Path::getPathRelToExec(_params.color_scale_dir);
    mapDir.setPath(colorMapDir);
    if (!mapDir.dirExists()) {
      cerr << "ERROR - WaterfallPlot::_readColorMap()" << endl;
      cerr << "  Cannot find color scale directory" << endl;
      cerr << "  Primary is: " << _params.color_scale_dir << endl;
      cerr << "  Secondary is relative to binary: " << colorMapDir << endl;
      return -1;
    }
    if (_params.debug) {
      cerr << "NOTE - using color scales relative to executable location" << endl;
      cerr << "  Exec path: " << Path::getExecPath() << endl;
      cerr << "  Color scale dir:: " << colorMapDir << endl;
    }
  }

  // create color map
  
  string colorMapPath = colorMapDir;
  colorMapPath += PATH_DELIM;
  colorMapPath += colorScaleName;
  _cmap.setName(getName(_plotType));
  _cmap.setUnits(getUnits(_plotType));

  if (_cmap.readMap(colorMapPath)) {
    cerr << "ERROR - WaterfallPlot::_readColorMap()" << endl;
    cerr << "  Cannot read color map, path: " << colorMapPath << endl;
    return -1;
  }

  return 0;

}

///////////////////////////////////////
// Apply the window to the time series

void WaterfallPlot::_applyWindow(const RadarComplex_t *iq, 
                                 RadarComplex_t *iqWindowed,
                                 int nSamples)
{
  
  // initialize the window
  
  _windowCoeff = _windowCoeff_.alloc(nSamples);
  switch (_fftWindow) {
    case Params::FFT_WINDOW_RECT:
    default:
      RadarMoments::initWindowRect(nSamples, _windowCoeff);
      break;
    case Params::FFT_WINDOW_VONHANN:
      RadarMoments::initWindowVonhann(nSamples, _windowCoeff);
      break;
    case Params::FFT_WINDOW_BLACKMAN:
      RadarMoments::initWindowBlackman(nSamples, _windowCoeff);
      break;
    case Params::FFT_WINDOW_BLACKMAN_NUTTALL:
      RadarMoments::initWindowBlackmanNuttall(nSamples, _windowCoeff);
      break;
    case Params::FFT_WINDOW_TUKEY_10:
      RadarMoments::initWindowTukey(0.1, nSamples, _windowCoeff);
      break;
    case Params::FFT_WINDOW_TUKEY_20:
      RadarMoments::initWindowTukey(0.2, nSamples, _windowCoeff);
      break;
    case Params::FFT_WINDOW_TUKEY_30:
      RadarMoments::initWindowTukey(0.3, nSamples, _windowCoeff);
      break;
    case Params::FFT_WINDOW_TUKEY_50:
      RadarMoments::initWindowTukey(0.5, nSamples, _windowCoeff);
  }

  // compute power spectrum
  
  RadarMoments::applyWindow(iq, _windowCoeff, iqWindowed, nSamples);

}
  
/////////////////////////////////////////////////
// compute for ZDR SDEV

double WaterfallPlot::_computeSdevZdr(const vector<double> &zdr)
  
{
  
  double nZdr = 0.0;
  double sumZdr = 0.0;
  double sumZdrSq = 0.0;
  
  for (size_t ii = 0; ii < zdr.size(); ii++) {
    double val = zdr[ii];
    sumZdr += val;
    sumZdrSq += (val * val);
    nZdr++;
  }
    
  double meanZdr = sumZdr / nZdr;
  double sdev = 0.001;
  if (nZdr > 2) {
    double term1 = sumZdrSq / nZdr;
    double term2 = meanZdr * meanZdr;
    if (term1 >= term2) {
      sdev = sqrt(term1 - term2);
    }
  }

  return sdev;

}

/////////////////////////////////////////////////
// compute SDEV for PHIDP
// takes account of folding

double WaterfallPlot::_computeSdevPhidp(const vector<double> &phidp)
   
{
  
  // compute mean phidp

  double count = 0.0;
  double sumxx = 0.0;
  double sumyy = 0.0;
  for (size_t ii = 0; ii < phidp.size(); ii++) {
    double xx, yy;
    ta_sincos(phidp[ii] * DEG_TO_RAD, &yy, &xx);
    sumxx += xx;
    sumyy += yy;
    count++;
  }
  double meanxx = sumxx / count;
  double meanyy = sumyy / count;
  double phidpMean = atan2(meanyy, meanxx) * RAD_TO_DEG;
  if (_phidpFoldsAt90) {
    phidpMean *= 0.5;
  }
  
  // compute standard deviation centered on the mean value
  
  count = 0.0;
  double sum = 0.0;
  double sumSq = 0.0;
  for (size_t ii = 0; ii < phidp.size(); ii++) {
    double diff = phidp[ii] - phidpMean;
    // constrain diff
    while (diff < -_phidpFoldVal) {
      diff += 2 * _phidpFoldVal;
    }
    while (diff > _phidpFoldVal) {
      diff -= 2 * _phidpFoldVal;
    }
    // sum up
    count++;
    sum += diff;
    sumSq += diff * diff;
  }

  double meanDiff = sum / count;
  double term1 = sumSq / count;
  double term2 = meanDiff * meanDiff;
  double sdev = 0.001;
  if (term1 >= term2) {
    sdev = sqrt(term1 - term2);
  }

  return sdev;
  
}

/////////////////////////////////////////////
// compute the folding values and range
// by inspecting the phidp values

void WaterfallPlot::_computePhidpFoldingRange(int nGates, int nSamples, 
                                              double **phidp2D)
  
{
  
  // check if fold is at 90 or 180
  
  double phidpMin = 9999;
  double phidpMax = -9999;

  for (int igate = 0; igate < nGates; igate++) {
    for (int isample = 0; isample < nSamples; isample++) {
      double phidp = phidp2D[igate][isample];
      phidpMin = min(phidpMin, phidp);
      phidpMax = max(phidpMax, phidp);
    } // isample
  } // igate
  
  _phidpFoldsAt90 = false;
  _phidpFoldVal = 180.0;
  if (phidpMin > -90 && phidpMax < 90) {
    _phidpFoldVal = 90.0;
    _phidpFoldsAt90 = true;
  }
  _phidpFoldRange = _phidpFoldVal * 2.0;
  
}

