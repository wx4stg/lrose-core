// *=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=* 
// ** Copyright UCAR (c) 1990 - 2021                                         
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
// TimeNavController.hh
//
// Coordinates navigation through archive file names indexed by date and time
//
// Brenda Javornik, EOL, NCAR, P.O.Box 3000, Boulder, CO, 80307-3000, USA
//
// Nov 2021
//
///////////////////////////////////////////////////////////////
//
// coordinates movement through the archive file names
// 
///////////////////////////////////////////////////////////////

#include "TimeNavController.hh"

TimeNavController::TimeNavController(TimeNavView *view) {

	_view = view;
	_model = new TimeNavModel();
  _tempDirIndex = -1;

}

TimeNavController::~TimeNavController() {
  if (_view != NULL) delete _view;
  if (_model != NULL) delete _model;
  _clearTempStack();
}

void TimeNavController::fetchArchiveFiles(string seedPath, string seedFileName,
    bool keepTimeRange) {

  _model->findArchiveFileList(seedPath, keepTimeRange);
  _setGuiFromArchiveStartTime();  
  _setGuiFromArchiveEndTime();
  _model->setSelectedFile(seedFileName);
  _setGuiFromSelectedTime();

  _view->setNTicks(_model->getNArchiveFiles());
  //setSliderPosition(_model->getPositionOfSelection());

  _view->showTimeControl();
}


void TimeNavController::setSliderPosition() {
  int value = _model->getPositionOfSelection();
  _view->setSliderPosition(value);
}

string &TimeNavController::getSelectedArchiveFile() {
  return _model->getSelectedArchiveFile();
}

string TimeNavController::_no_yyyymmdd(string s) {
  if (s.size() > 8) {
    return s.substr(0, s.size()-8);
  }
}

// push base dir on the stack, and clear the stack when
// setting a new base dir

string TimeNavController::getTempDir() {
  string nextTempDir = _model->getTempDir();
  _tempDirStack.push_back(_no_yyyymmdd(nextTempDir)); // _model->getCurrentPath());
  _tempDirIndex += 1;
  return nextTempDir;
}

string TimeNavController::getSelectedArchiveFileName() {
  return _model->getSelectedArchiveFileName();
}

// return true if the current directory is a temp dir
// i.e. of the form .../.tmp_N/yyyymmdd
bool TimeNavController::isSelectedFileInTempDir() {
  return (_tempDirIndex >= 0);
}


// ALWAYS point at current directory!!!
//
// return the previous temp directory, or the base
// directory if we are out of the temp dirs in the stack,
// i.e. idx < 0; or use a stack????
// return empty string, if there are no more directories
// in the stack
string TimeNavController::getPreviousTempDir() {
  if (_tempDirIndex <= 0) {
    return "";
  } else {
    _tempDirIndex -= 1;
    string previousDir = _tempDirStack.at(_tempDirIndex);
    return previousDir;
  }
}

// return the next temp directory, or the
// empty string if out of the temp dirs in the stack,
// i.e. idx < 0; or use a stack????
// return empty string, if there are no more directories
// in the stack
string TimeNavController::getNextTempDir() {
  if (_tempDirIndex >= _tempDirStack.size()-1) {
    return "";
  } else {
    _tempDirIndex += 1;
    string nextDir = _tempDirStack.at(_tempDirIndex);
    return nextDir;
  }
}

void TimeNavController::fileOpened() {

  // TODO: delete all temporary directories in stack

  // clear the undo/redo stack
  _tempDirStack.clear();

  // set the base directory for any edits
  _tempDirIndex = 0;
  _tempDirStack.push_back(_model->getCurrentPath());
}

void TimeNavController::updateGui() {
  _setGuiFromArchiveStartTime();
  _setGuiFromArchiveEndTime();
}

////////////////////////////////////////////////////////
// set times from gui widgets

void TimeNavController::_setArchiveStartEndTimeFromGui(int startYear, int startMonth, int startDay,
                       int startHour, int startMinute, int startSecond,
                       int endYear, int endMonth, int endDay,
                       int endHour, int endMinute, int endSecond) {

  _model->setArchiveStartEndTime(startYear, startMonth, startDay,
    startHour, startMinute, startSecond,
    endYear, endMonth, endDay,
    endHour, endMinute, endSecond);

  _setGuiFromSelectedTime();

  _view->setNTicks(_model->getNArchiveFiles());
  setSliderPosition();
  _setGuiFromArchiveStartTime();
  _setGuiFromArchiveEndTime();
  //_view->showTimeControl();
}

/*
void TimeNavController::_setArchiveEndTimeFromGui(int year, int month, int day,
  int hour, int minute, int seconds)
{
  //QDate date = qdt.date();
  //QTime time = qdt.time();
  //_guiEndTime.set(date.year(), date.month(), date.day(),
  //                time.hour(), time.minute(), time.second());
  _model->setArchiveEndTime(year, month, day,
    hour, minute, seconds);  
}
*/

/*
void TimeNavController::_acceptGuiTimes()
{
  _archiveStartTime = _guiStartTime;
  _archiveEndTime = _guiEndTime;
  loadArchiveFileList();
}

void TimeNavController::_cancelGuiTimes()
{
  _setGuiFromArchiveStartTime();
  _setGuiFromArchiveEndTime();
}
*/
////////////////////////////////////////////////////////
// set gui widget from archive start time

void TimeNavController::_setGuiFromArchiveStartTime()
{
  int year;
  int month; 
  int day;
  int hour; 
  int minute; 
  int seconds;
  _model->getArchiveStartTime(&year, &month, &day, &hour, &minute, &seconds);
  _view->setGuiFromArchiveStartTime(year, month, day, hour,
    minute, seconds);
}

////////////////////////////////////////////////////////
// set gui widget from archive end time

void TimeNavController::_setGuiFromArchiveEndTime()
{
  int year;
  int month; 
  int day;
  int hour; 
  int minute; 
  int seconds;
  _model->getArchiveEndTime(&year, &month, &day, &hour, &minute, &seconds);
  _view->setGuiFromArchiveEndTime(year, month, day, hour,
    minute, seconds);
}

////////////////////////////////////////////////////////
// set gui selected time label

void TimeNavController::_setGuiFromSelectedTime()
{
  int year;
  int month; 
  int day;
  int hour; 
  int minute; 
  int seconds;
  _model->getSelectedTime(&year, &month, &day, &hour, &minute, &seconds);
  /*
  char text[128];
  snprintf(text, 128, "%.4d/%.2d/%.2d %.2d:%.2d:%.2d",
           _selectedTime.getYear(),
           _selectedTime.getMonth(),
           _selectedTime.getDay(),
           _selectedTime.getHour(),
           _selectedTime.getMin(),
           _selectedTime.getSec());
           */
  _view->setGuiFromSelectedTime(year, month, day, hour,
    minute, seconds);
  //_selectedTimeLabel->setText(text);
}

vector<string> &TimeNavController::getArchiveFileList(string path,
  int startYear, int startMonth, int startDay,
  int startHour, int startMinute, int startSecond,
  int endYear, int endMonth, int endDay,
  int endHour, int endMinute, int endSecond) {
  //string startTime, string endTime) {
  return _model->getArchiveFileListOnly(path, //startTime, endTime);
      startYear, startMonth, startDay, startHour, startMinute, startSecond,
      endYear, endMonth, endDay, endHour, endMinute, endSecond);

}

/*
////////////////////////////////////////////////////////
// set archive start time

void TimeNavController::_setArchiveStartTime(const RadxTime &rtime)

{
  _archiveStartTime = rtime;
  if (!_archiveStartTime.isValid()) {
    _archiveStartTime.set(RadxTime::NOW);
  }
  _setGuiFromArchiveStartTime();
}

////////////////////////////////////////////////////////
// set archive end time

void TimeNavController::_setArchiveEndTime(const RadxTime &rtime)

{
  _archiveEndTime = rtime;
  if (!_archiveEndTime.isValid()) {
    _archiveEndTime.set(RadxTime::NOW);
  }
  _setGuiFromArchiveEndTime();
}

////////////////////////////////////////////////////////
// change start time

void TimeNavController::_goBack1()
{
  if (_archiveScanIndex > 0) {
    _archiveScanIndex -= 1;
    _setArchiveRetrievalPending();
  } else {

      LOG(DEBUG) << "At start of data, cannot go back";
    
  }
  _timeSlider->setSliderPosition(_archiveScanIndex);
}

void TimeNavController::_goBackPeriod()
{

  int archiveSpanSecs = _archiveEndTime - _archiveStartTime;
  _archiveStartTime -= archiveSpanSecs;
  _archiveEndTime -= archiveSpanSecs;
  loadArchiveFileList();
  if (_archiveScanIndex > (int) _archiveFileList.size() - 1) {
    _archiveScanIndex = (int) _archiveFileList.size() - 1;
  }
  _timeSlider->setSliderPosition(_archiveScanIndex);

}

void TimeNavController::_goFwd1()
{
  if (_archiveScanIndex < (int) _archiveFileList.size() - 1) {
    _archiveScanIndex += 1;
    _setArchiveRetrievalPending();
  } else {

      LOG(DEBUG) << "At end of data, cannot go forward";
    
  }
  _timeSlider->setSliderPosition(_archiveScanIndex);
}

void TimeNavController::_goFwdPeriod()
{

  int archiveSpanSecs = _archiveEndTime - _archiveStartTime;
  _archiveStartTime += archiveSpanSecs;
  _archiveEndTime += archiveSpanSecs;
  loadArchiveFileList();
  if (_archiveScanIndex > (int) _archiveFileList.size() - 1) {
    _archiveScanIndex = (int) _archiveFileList.size() - 1;
  }
  _timeSlider->setSliderPosition(_archiveScanIndex);

}


*/

void TimeNavController::setTimeSliderPosition(int value) {
  _model->setSelectedFile(value);
  _view->setSliderPosition(value);
}

void TimeNavController::timeSliderValueChanged(int value) 
{

  _model->setSelectedFile(value);
  _setGuiFromSelectedTime();
  /*
  size_t nFiles = _model->getNArchiveFiles();
  if (value < 0 || value > (int) nFiles - 1) {
    return;
  }
  // get path for this value
  string path = _archiveFileList[value];
  // get time for this path
  RadxTime pathTime;
  NcfRadxFile::getTimeFromPath(path, pathTime);
  // set selected time
  _selectedTime = pathTime;
  _setGuiFromSelectedTime();
  */
}

void TimeNavController::timeSliderReleased(int value) 
{

  _model->setSelectedFile(value);

  /*
  if (value < 0 || value > (int) _archiveFileList.size() - 1) {
    return;
  }
  // get path for this value
  string path = _archiveFileList[value];
  // get time for this path
  RadxTime pathTime;
  NcfRadxFile::getTimeFromPath(path, pathTime);
  // set selected time
  _selectedTime = pathTime;
  _setGuiFromSelectedTime();
  // request data
  if (_archiveScanIndex != value) {
    _archiveScanIndex = value;
  }
  */

}

bool TimeNavController::moreFiles() {
  return _model->moreFiles();
}

void TimeNavController::_clearTempStack() {
  vector<string>::iterator it;
  for (it = _tempDirStack.begin(); it != _tempDirStack.end(); ++it) {
    //_model->removeTempDirs(); // ?? or in model destructor clear all tmp dirs??
    // TODO: how to clean up temp dirs???
    // before changing to a new base directory, prompt to save any temp dirs?
    // otherwise, the temp dirs will be deleted.
    // new method ... movingToNewBaseDir ...
    // if moving to new base dir, clear the stack
  }
}

