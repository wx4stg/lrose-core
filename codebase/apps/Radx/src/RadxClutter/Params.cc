/* *=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=* */
/* ** Copyright UCAR                                                         */
/* ** University Corporation for Atmospheric Research (UCAR)                 */
/* ** National Center for Atmospheric Research (NCAR)                        */
/* ** Boulder, Colorado, USA                                                 */
/* ** BSD licence applies - redistribution and use in source and binary      */
/* ** forms, with or without modification, are permitted provided that       */
/* ** the following conditions are met:                                      */
/* ** 1) If the software is modified to produce derivative works,            */
/* ** such modified software should be clearly marked, so as not             */
/* ** to confuse it with the version available from UCAR.                    */
/* ** 2) Redistributions of source code must retain the above copyright      */
/* ** notice, this list of conditions and the following disclaimer.          */
/* ** 3) Redistributions in binary form must reproduce the above copyright   */
/* ** notice, this list of conditions and the following disclaimer in the    */
/* ** documentation and/or other materials provided with the distribution.   */
/* ** 4) Neither the name of UCAR nor the names of its contributors,         */
/* ** if any, may be used to endorse or promote products derived from        */
/* ** this software without specific prior written permission.               */
/* ** DISCLAIMER: THIS SOFTWARE IS PROVIDED 'AS IS' AND WITHOUT ANY EXPRESS  */
/* ** OR IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED      */
/* ** WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.    */
/* *=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=* */
////////////////////////////////////////////
// Params.cc
//
// TDRP C++ code file for class 'Params'.
//
// Code for program RadxClutter
//
// This file has been automatically
// generated by TDRP, do not modify.
//
/////////////////////////////////////////////

/**
 *
 * @file Params.cc
 *
 * @class Params
 *
 * This class is automatically generated by the Table
 * Driven Runtime Parameters (TDRP) system
 *
 * @note Source is automatically generated from
 *       paramdef file at compile time, do not modify
 *       since modifications will be overwritten.
 *
 *
 * @author Automatically generated
 *
 */
#include "Params.hh"
#include <cstring>

  ////////////////////////////////////////////
  // Default constructor
  //

  Params::Params()

  {

    // zero out table

    memset(_table, 0, sizeof(_table));

    // zero out members

    memset(&_start_, 0, &_end_ - &_start_);

    // class name

    _className = "Params";

    // initialize table

    _init();

    // set members

    tdrpTable2User(_table, &_start_);

    _exitDeferred = false;

  }

  ////////////////////////////////////////////
  // Copy constructor
  //

  Params::Params(const Params& source)

  {

    // sync the source object

    source.sync();

    // zero out table

    memset(_table, 0, sizeof(_table));

    // zero out members

    memset(&_start_, 0, &_end_ - &_start_);

    // class name

    _className = "Params";

    // copy table

    tdrpCopyTable((TDRPtable *) source._table, _table);

    // set members

    tdrpTable2User(_table, &_start_);

    _exitDeferred = false;

  }

  ////////////////////////////////////////////
  // Destructor
  //

  Params::~Params()

  {

    // free up

    freeAll();

  }

  ////////////////////////////////////////////
  // Assignment
  //

  void Params::operator=(const Params& other)

  {

    // sync the other object

    other.sync();

    // free up any existing memory

    freeAll();

    // zero out table

    memset(_table, 0, sizeof(_table));

    // zero out members

    memset(&_start_, 0, &_end_ - &_start_);

    // copy table

    tdrpCopyTable((TDRPtable *) other._table, _table);

    // set members

    tdrpTable2User(_table, &_start_);

    _exitDeferred = other._exitDeferred;

  }

  ////////////////////////////////////////////
  // loadFromArgs()
  //
  // Loads up TDRP using the command line args.
  //
  // Check usage() for command line actions associated with
  // this function.
  //
  //   argc, argv: command line args
  //
  //   char **override_list: A null-terminated list of overrides
  //     to the parameter file.
  //     An override string has exactly the format of an entry
  //     in the parameter file itself.
  //
  //   char **params_path_p:
  //     If this is non-NULL, it is set to point to the path
  //     of the params file used.
  //
  //   bool defer_exit: normally, if the command args contain a 
  //      print or check request, this function will call exit().
  //      If defer_exit is set, such an exit is deferred and the
  //      private member _exitDeferred is set.
  //      Use exidDeferred() to test this flag.
  //
  //  Returns 0 on success, -1 on failure.
  //

  int Params::loadFromArgs(int argc, char **argv,
                           char **override_list,
                           char **params_path_p,
                           bool defer_exit)
  {
    int exit_deferred;
    if (_tdrpLoadFromArgs(argc, argv,
                          _table, &_start_,
                          override_list, params_path_p,
                          _className,
                          defer_exit, &exit_deferred)) {
      return (-1);
    } else {
      if (exit_deferred) {
        _exitDeferred = true;
      }
      return (0);
    }
  }

  ////////////////////////////////////////////
  // loadApplyArgs()
  //
  // Loads up TDRP using the params path passed in, and applies
  // the command line args for printing and checking.
  //
  // Check usage() for command line actions associated with
  // this function.
  //
  //   const char *param_file_path: the parameter file to be read in
  //
  //   argc, argv: command line args
  //
  //   char **override_list: A null-terminated list of overrides
  //     to the parameter file.
  //     An override string has exactly the format of an entry
  //     in the parameter file itself.
  //
  //   bool defer_exit: normally, if the command args contain a 
  //      print or check request, this function will call exit().
  //      If defer_exit is set, such an exit is deferred and the
  //      private member _exitDeferred is set.
  //      Use exidDeferred() to test this flag.
  //
  //  Returns 0 on success, -1 on failure.
  //

  int Params::loadApplyArgs(const char *params_path,
                            int argc, char **argv,
                            char **override_list,
                            bool defer_exit)
  {
    int exit_deferred;
    if (tdrpLoadApplyArgs(params_path, argc, argv,
                          _table, &_start_,
                          override_list,
                          _className,
                          defer_exit, &exit_deferred)) {
      return (-1);
    } else {
      if (exit_deferred) {
        _exitDeferred = true;
      }
      return (0);
    }
  }

  ////////////////////////////////////////////
  // isArgValid()
  // 
  // Check if a command line arg is a valid TDRP arg.
  //

  bool Params::isArgValid(const char *arg)
  {
    return (tdrpIsArgValid(arg));
  }

  ////////////////////////////////////////////
  // isArgValid()
  // 
  // Check if a command line arg is a valid TDRP arg.
  // return number of args consumed.
  //

  int Params::isArgValidN(const char *arg)
  {
    return (tdrpIsArgValidN(arg));
  }

  ////////////////////////////////////////////
  // load()
  //
  // Loads up TDRP for a given class.
  //
  // This version of load gives the programmer the option to load
  // up more than one class for a single application. It is a
  // lower-level routine than loadFromArgs, and hence more
  // flexible, but the programmer must do more work.
  //
  //   const char *param_file_path: the parameter file to be read in.
  //
  //   char **override_list: A null-terminated list of overrides
  //     to the parameter file.
  //     An override string has exactly the format of an entry
  //     in the parameter file itself.
  //
  //   expand_env: flag to control environment variable
  //               expansion during tokenization.
  //               If TRUE, environment expansion is set on.
  //               If FALSE, environment expansion is set off.
  //
  //  Returns 0 on success, -1 on failure.
  //

  int Params::load(const char *param_file_path,
                   char **override_list,
                   int expand_env, int debug)
  {
    if (tdrpLoad(param_file_path,
                 _table, &_start_,
                 override_list,
                 expand_env, debug)) {
      return (-1);
    } else {
      return (0);
    }
  }

  ////////////////////////////////////////////
  // loadFromBuf()
  //
  // Loads up TDRP for a given class.
  //
  // This version of load gives the programmer the option to
  // load up more than one module for a single application,
  // using buffers which have been read from a specified source.
  //
  //   const char *param_source_str: a string which describes the
  //     source of the parameter information. It is used for
  //     error reporting only.
  //
  //   char **override_list: A null-terminated list of overrides
  //     to the parameter file.
  //     An override string has exactly the format of an entry
  //     in the parameter file itself.
  //
  //   const char *inbuf: the input buffer
  //
  //   int inlen: length of the input buffer
  //
  //   int start_line_num: the line number in the source which
  //     corresponds to the start of the buffer.
  //
  //   expand_env: flag to control environment variable
  //               expansion during tokenization.
  //               If TRUE, environment expansion is set on.
  //               If FALSE, environment expansion is set off.
  //
  //  Returns 0 on success, -1 on failure.
  //

  int Params::loadFromBuf(const char *param_source_str,
                          char **override_list,
                          const char *inbuf, int inlen,
                          int start_line_num,
                          int expand_env, int debug)
  {
    if (tdrpLoadFromBuf(param_source_str,
                        _table, &_start_,
                        override_list,
                        inbuf, inlen, start_line_num,
                        expand_env, debug)) {
      return (-1);
    } else {
      return (0);
    }
  }

  ////////////////////////////////////////////
  // loadDefaults()
  //
  // Loads up default params for a given class.
  //
  // See load() for more detailed info.
  //
  //  Returns 0 on success, -1 on failure.
  //

  int Params::loadDefaults(int expand_env)
  {
    if (tdrpLoad(NULL,
                 _table, &_start_,
                 NULL, expand_env, FALSE)) {
      return (-1);
    } else {
      return (0);
    }
  }

  ////////////////////////////////////////////
  // sync()
  //
  // Syncs the user struct data back into the parameter table,
  // in preparation for printing.
  //
  // This function alters the table in a consistent manner.
  // Therefore it can be regarded as const.
  //

  void Params::sync(void) const
  {
    tdrpUser2Table(_table, (char *) &_start_);
  }

  ////////////////////////////////////////////
  // print()
  // 
  // Print params file
  //
  // The modes supported are:
  //
  //   PRINT_SHORT:   main comments only, no help or descriptions
  //                  structs and arrays on a single line
  //   PRINT_NORM:    short + descriptions and help
  //   PRINT_LONG:    norm  + arrays and structs expanded
  //   PRINT_VERBOSE: long  + private params included
  //

  void Params::print(FILE *out, tdrp_print_mode_t mode)
  {
    tdrpPrint(out, _table, _className, mode);
  }

  ////////////////////////////////////////////
  // checkAllSet()
  //
  // Return TRUE if all set, FALSE if not.
  //
  // If out is non-NULL, prints out warning messages for those
  // parameters which are not set.
  //

  int Params::checkAllSet(FILE *out)
  {
    return (tdrpCheckAllSet(out, _table, &_start_));
  }

  //////////////////////////////////////////////////////////////
  // checkIsSet()
  //
  // Return TRUE if parameter is set, FALSE if not.
  //
  //

  int Params::checkIsSet(const char *paramName)
  {
    return (tdrpCheckIsSet(paramName, _table, &_start_));
  }

  ////////////////////////////////////////////
  // freeAll()
  //
  // Frees up all TDRP dynamic memory.
  //

  void Params::freeAll(void)
  {
    tdrpFreeAll(_table, &_start_);
  }

  ////////////////////////////////////////////
  // usage()
  //
  // Prints out usage message for TDRP args as passed
  // in to loadFromArgs().
  //

  void Params::usage(ostream &out)
  {
    out << "TDRP args: [options as below]\n"
        << "   [ -params/--params path ] specify params file path\n"
        << "   [ -check_params/--check_params] check which params are not set\n"
        << "   [ -print_params/--print_params [mode]] print parameters\n"
        << "     using following modes, default mode is 'norm'\n"
        << "       short:   main comments only, no help or descr\n"
        << "                structs and arrays on a single line\n"
        << "       norm:    short + descriptions and help\n"
        << "       long:    norm  + arrays and structs expanded\n"
        << "       verbose: long  + private params included\n"
        << "       short_expand:   short with env vars expanded\n"
        << "       norm_expand:    norm with env vars expanded\n"
        << "       long_expand:    long with env vars expanded\n"
        << "       verbose_expand: verbose with env vars expanded\n"
        << "   [ -tdrp_debug] debugging prints for tdrp\n"
        << "   [ -tdrp_usage] print this usage\n";
  }

  ////////////////////////////////////////////
  // arrayRealloc()
  //
  // Realloc 1D array.
  //
  // If size is increased, the values from the last array 
  // entry is copied into the new space.
  //
  // Returns 0 on success, -1 on error.
  //

  int Params::arrayRealloc(const char *param_name, int new_array_n)
  {
    if (tdrpArrayRealloc(_table, &_start_,
                         param_name, new_array_n)) {
      return (-1);
    } else {
      return (0);
    }
  }

  ////////////////////////////////////////////
  // array2DRealloc()
  //
  // Realloc 2D array.
  //
  // If size is increased, the values from the last array 
  // entry is copied into the new space.
  //
  // Returns 0 on success, -1 on error.
  //

  int Params::array2DRealloc(const char *param_name,
                             int new_array_n1,
                             int new_array_n2)
  {
    if (tdrpArray2DRealloc(_table, &_start_, param_name,
                           new_array_n1, new_array_n2)) {
      return (-1);
    } else {
      return (0);
    }
  }

  ////////////////////////////////////////////
  // _init()
  //
  // Class table initialization function.
  //
  //

  void Params::_init()

  {

    TDRPtable *tt = _table;

    // Parameter 'Comment 0'
    
    memset(tt, 0, sizeof(TDRPtable));
    tt->ptype = COMMENT_TYPE;
    tt->param_name = tdrpStrDup("Comment 0");
    tt->comment_hdr = tdrpStrDup("RadxPersistentClutter identifies persistent clutter in polar radar data, flags it, and writes out the statistics to a CfRadial file.");
    tt->comment_text = tdrpStrDup("This method is based on the following paper: Lakshmanan V., J. Zhang, K. Hondl and C. Langston. A Statistical Approach to Mitigating Persistent Clutter in Radar Reflectivity Data. IEEE Journal of Selected Topics in Applied Earth Observations and Remote Sensing, Vol. 5, No. 2, April 2012.");
    tt++;
    
    // Parameter 'Comment 1'
    
    memset(tt, 0, sizeof(TDRPtable));
    tt->ptype = COMMENT_TYPE;
    tt->param_name = tdrpStrDup("Comment 1");
    tt->comment_hdr = tdrpStrDup("Debugging and process control");
    tt->comment_text = tdrpStrDup("");
    tt++;
    
    // Parameter 'debug'
    // ctype is '_debug_t'
    
    memset(tt, 0, sizeof(TDRPtable));
    tt->ptype = ENUM_TYPE;
    tt->param_name = tdrpStrDup("debug");
    tt->descr = tdrpStrDup("Debug option");
    tt->help = tdrpStrDup("If set, debug messages will be printed appropriately");
    tt->val_offset = (char *) &debug - &_start_;
    tt->enum_def.name = tdrpStrDup("debug_t");
    tt->enum_def.nfields = 4;
    tt->enum_def.fields = (enum_field_t *)
        tdrpMalloc(tt->enum_def.nfields * sizeof(enum_field_t));
      tt->enum_def.fields[0].name = tdrpStrDup("DEBUG_OFF");
      tt->enum_def.fields[0].val = DEBUG_OFF;
      tt->enum_def.fields[1].name = tdrpStrDup("DEBUG_NORM");
      tt->enum_def.fields[1].val = DEBUG_NORM;
      tt->enum_def.fields[2].name = tdrpStrDup("DEBUG_VERBOSE");
      tt->enum_def.fields[2].val = DEBUG_VERBOSE;
      tt->enum_def.fields[3].name = tdrpStrDup("DEBUG_EXTRA");
      tt->enum_def.fields[3].val = DEBUG_EXTRA;
    tt->single_val.e = DEBUG_OFF;
    tt++;
    
    // Parameter 'instance'
    // ctype is 'char*'
    
    memset(tt, 0, sizeof(TDRPtable));
    tt->ptype = STRING_TYPE;
    tt->param_name = tdrpStrDup("instance");
    tt->descr = tdrpStrDup("Program instance for process registration.");
    tt->help = tdrpStrDup("This application registers with procmap. This is the instance used for registration.");
    tt->val_offset = (char *) &instance - &_start_;
    tt->single_val.s = tdrpStrDup("test");
    tt++;
    
    // Parameter 'Comment 2'
    
    memset(tt, 0, sizeof(TDRPtable));
    tt->ptype = COMMENT_TYPE;
    tt->param_name = tdrpStrDup("Comment 2");
    tt->comment_hdr = tdrpStrDup("DATA INPUT");
    tt->comment_text = tdrpStrDup("");
    tt++;
    
    // Parameter 'input_dir'
    // ctype is 'char*'
    
    memset(tt, 0, sizeof(TDRPtable));
    tt->ptype = STRING_TYPE;
    tt->param_name = tdrpStrDup("input_dir");
    tt->descr = tdrpStrDup("Input directory for searching for files.");
    tt->help = tdrpStrDup("Files will be searched for in this directory.");
    tt->val_offset = (char *) &input_dir - &_start_;
    tt->single_val.s = tdrpStrDup(".");
    tt++;
    
    // Parameter 'file_name_substr'
    // ctype is 'char*'
    
    memset(tt, 0, sizeof(TDRPtable));
    tt->ptype = STRING_TYPE;
    tt->param_name = tdrpStrDup("file_name_substr");
    tt->descr = tdrpStrDup("Substring that needs to exist in the file name.");
    tt->help = tdrpStrDup("Set to empty to avoid this check.");
    tt->val_offset = (char *) &file_name_substr - &_start_;
    tt->single_val.s = tdrpStrDup("");
    tt++;
    
    // Parameter 'mode'
    // ctype is '_mode_t'
    
    memset(tt, 0, sizeof(TDRPtable));
    tt->ptype = ENUM_TYPE;
    tt->param_name = tdrpStrDup("mode");
    tt->descr = tdrpStrDup("Operating mode");
    tt->help = tdrpStrDup("In REALTIME mode, the program waits for a new input file.  In ARCHIVE mode, it moves through the data between the start and end times set on the command line. In FILELIST mode, it moves through the list of file names specified on the command line. Paths (in ARCHIVE mode, at least) MUST contain a day-directory above the data file -- ./data_file.ext will not work as a file path, but ./yyyymmdd/data_file.ext will.");
    tt->val_offset = (char *) &mode - &_start_;
    tt->enum_def.name = tdrpStrDup("mode_t");
    tt->enum_def.nfields = 3;
    tt->enum_def.fields = (enum_field_t *)
        tdrpMalloc(tt->enum_def.nfields * sizeof(enum_field_t));
      tt->enum_def.fields[0].name = tdrpStrDup("REALTIME");
      tt->enum_def.fields[0].val = REALTIME;
      tt->enum_def.fields[1].name = tdrpStrDup("ARCHIVE");
      tt->enum_def.fields[1].val = ARCHIVE;
      tt->enum_def.fields[2].name = tdrpStrDup("FILELIST");
      tt->enum_def.fields[2].val = FILELIST;
    tt->single_val.e = ARCHIVE;
    tt++;
    
    // Parameter 'max_realtime_data_age_secs'
    // ctype is 'int'
    
    memset(tt, 0, sizeof(TDRPtable));
    tt->ptype = INT_TYPE;
    tt->param_name = tdrpStrDup("max_realtime_data_age_secs");
    tt->descr = tdrpStrDup("Maximum age of realtime data (secs)");
    tt->help = tdrpStrDup("Only data less old than this will be used.");
    tt->val_offset = (char *) &max_realtime_data_age_secs - &_start_;
    tt->single_val.i = 300;
    tt++;
    
    // Parameter 'Comment 3'
    
    memset(tt, 0, sizeof(TDRPtable));
    tt->ptype = COMMENT_TYPE;
    tt->param_name = tdrpStrDup("Comment 3");
    tt->comment_hdr = tdrpStrDup("ACTION");
    tt->comment_text = tdrpStrDup("There are 2 possible actions: (a) analyze the clutter from a number of volumes, and store the results in a CfRadial file; or (b) use the clutter statistics file in the analysis step to remove clutter from files containing clutter.");
    tt++;
    
    // Parameter 'action'
    // ctype is '_action_t'
    
    memset(tt, 0, sizeof(TDRPtable));
    tt->ptype = ENUM_TYPE;
    tt->param_name = tdrpStrDup("action");
    tt->descr = tdrpStrDup("Action to be performed");
    tt->help = tdrpStrDup("CLUTTER_ANALYSIS: given a series of volumes containing clutter, analyze the clutter and store the cliutter statistics in CfRadialFiles. CLUTTER_REMOVAL: using the clutter statistics analyzed in the first step, remove reflectivity power from those clutter gates that are not overridden by weather. If the weather echo is stronger that the mean clutter, it is left unchanged.");
    tt->val_offset = (char *) &action - &_start_;
    tt->enum_def.name = tdrpStrDup("action_t");
    tt->enum_def.nfields = 2;
    tt->enum_def.fields = (enum_field_t *)
        tdrpMalloc(tt->enum_def.nfields * sizeof(enum_field_t));
      tt->enum_def.fields[0].name = tdrpStrDup("CLUTTER_ANALYSIS");
      tt->enum_def.fields[0].val = CLUTTER_ANALYSIS;
      tt->enum_def.fields[1].name = tdrpStrDup("CLUTTER_REMOVAL");
      tt->enum_def.fields[1].val = CLUTTER_REMOVAL;
    tt->single_val.e = CLUTTER_ANALYSIS;
    tt++;
    
    // Parameter 'Comment 4'
    
    memset(tt, 0, sizeof(TDRPtable));
    tt->ptype = COMMENT_TYPE;
    tt->param_name = tdrpStrDup("Comment 4");
    tt->comment_hdr = tdrpStrDup("SCAN DETAILS");
    tt->comment_text = tdrpStrDup("We specify the scan angles for which the clutter will be analyzed. This is the 'ideal'. The actual measurements are mapped onto this ideal scan.");
    tt++;
    
    // Parameter 'scan_mode'
    // ctype is '_scan_mode_t'
    
    memset(tt, 0, sizeof(TDRPtable));
    tt->ptype = ENUM_TYPE;
    tt->param_name = tdrpStrDup("scan_mode");
    tt->descr = tdrpStrDup("Scan mode");
    tt->help = tdrpStrDup("PPI - horizontal scanning, e.g. surveillance. RHI - vertical scanning");
    tt->val_offset = (char *) &scan_mode - &_start_;
    tt->enum_def.name = tdrpStrDup("scan_mode_t");
    tt->enum_def.nfields = 2;
    tt->enum_def.fields = (enum_field_t *)
        tdrpMalloc(tt->enum_def.nfields * sizeof(enum_field_t));
      tt->enum_def.fields[0].name = tdrpStrDup("PPI");
      tt->enum_def.fields[0].val = PPI;
      tt->enum_def.fields[1].name = tdrpStrDup("RHI");
      tt->enum_def.fields[1].val = RHI;
    tt->single_val.e = PPI;
    tt++;
    
    // Parameter 'sweep_fixed_angles'
    // ctype is 'double'
    
    memset(tt, 0, sizeof(TDRPtable));
    tt->ptype = DOUBLE_TYPE;
    tt->param_name = tdrpStrDup("sweep_fixed_angles");
    tt->descr = tdrpStrDup("List of fixed angles for desired sweeps (deg)");
    tt->help = tdrpStrDup("PPI mode: elevations. RHIs: azimuths.");
    tt->array_offset = (char *) &_sweep_fixed_angles - &_start_;
    tt->array_n_offset = (char *) &sweep_fixed_angles_n - &_start_;
    tt->is_array = TRUE;
    tt->array_len_fixed = FALSE;
    tt->array_elem_size = sizeof(double);
    tt->array_n = 4;
    tt->array_vals = (tdrpVal_t *)
        tdrpMalloc(tt->array_n * sizeof(tdrpVal_t));
      tt->array_vals[0].d = 0.5;
      tt->array_vals[1].d = 1;
      tt->array_vals[2].d = 1.5;
      tt->array_vals[3].d = 2;
    tt++;
    
    // Parameter 'first_ray_angle'
    // ctype is 'double'
    
    memset(tt, 0, sizeof(TDRPtable));
    tt->ptype = DOUBLE_TYPE;
    tt->param_name = tdrpStrDup("first_ray_angle");
    tt->descr = tdrpStrDup("Starting scan angle for rays (deg).");
    tt->help = tdrpStrDup("In PPI mode, this is the first azimuth in a clockwise sweep. In RHI mode, this is the lowest elevation in a sweep.");
    tt->val_offset = (char *) &first_ray_angle - &_start_;
    tt->single_val.d = 0;
    tt++;
    
    // Parameter 'last_ray_angle'
    // ctype is 'double'
    
    memset(tt, 0, sizeof(TDRPtable));
    tt->ptype = DOUBLE_TYPE;
    tt->param_name = tdrpStrDup("last_ray_angle");
    tt->descr = tdrpStrDup("Ending scan angle for rays (deg).");
    tt->help = tdrpStrDup("In PPI mode, this is the last azimuth in a clockwise sweep. In RHI mode, this is the highest elevation in a sweep.");
    tt->val_offset = (char *) &last_ray_angle - &_start_;
    tt->single_val.d = 359.99;
    tt++;
    
    // Parameter 'delta_ray_angle'
    // ctype is 'double'
    
    memset(tt, 0, sizeof(TDRPtable));
    tt->ptype = DOUBLE_TYPE;
    tt->param_name = tdrpStrDup("delta_ray_angle");
    tt->descr = tdrpStrDup("Delta scan angle between consecutive rays (deg).");
    tt->help = tdrpStrDup("In PPI mode, this is the azimuth difference between rays. In PPI mode the delta should be positive, i.e. for a clockwise sweep. In RHI mode, this is the elevation difference between rays.");
    tt->val_offset = (char *) &delta_ray_angle - &_start_;
    tt->single_val.d = 1;
    tt++;
    
    // Parameter 'az_tolerance_deg'
    // ctype is 'double'
    
    memset(tt, 0, sizeof(TDRPtable));
    tt->ptype = DOUBLE_TYPE;
    tt->param_name = tdrpStrDup("az_tolerance_deg");
    tt->descr = tdrpStrDup("Azimumth tolerance");
    tt->help = tdrpStrDup("Allowed degrees difference between azimuth values for rays from measured volumes to be mapped onto the clutter grids");
    tt->val_offset = (char *) &az_tolerance_deg - &_start_;
    tt->single_val.d = 0.1;
    tt++;
    
    // Parameter 'elev_tolerance_deg'
    // ctype is 'double'
    
    memset(tt, 0, sizeof(TDRPtable));
    tt->ptype = DOUBLE_TYPE;
    tt->param_name = tdrpStrDup("elev_tolerance_deg");
    tt->descr = tdrpStrDup("Elevation tolerance");
    tt->help = tdrpStrDup("Allowed degrees difference between elevation values for rays from measured volumes to be mapped onto the clutter grids");
    tt->val_offset = (char *) &elev_tolerance_deg - &_start_;
    tt->single_val.d = 0.1;
    tt++;
    
    // Parameter 'max_range_km'
    // ctype is 'double'
    
    memset(tt, 0, sizeof(TDRPtable));
    tt->ptype = DOUBLE_TYPE;
    tt->param_name = tdrpStrDup("max_range_km");
    tt->descr = tdrpStrDup("Specified maximim range - km.");
    tt->help = tdrpStrDup("Gates beyond this range are removed.");
    tt->val_offset = (char *) &max_range_km - &_start_;
    tt->single_val.d = 60;
    tt++;
    
    // Parameter 'Comment 5'
    
    memset(tt, 0, sizeof(TDRPtable));
    tt->ptype = COMMENT_TYPE;
    tt->param_name = tdrpStrDup("Comment 5");
    tt->comment_hdr = tdrpStrDup("ALGORITHM DETAILS");
    tt->comment_text = tdrpStrDup("");
    tt++;
    
    // Parameter 'dbz_field_name'
    // ctype is 'char*'
    
    memset(tt, 0, sizeof(TDRPtable));
    tt->ptype = STRING_TYPE;
    tt->param_name = tdrpStrDup("dbz_field_name");
    tt->descr = tdrpStrDup("Reflectivity field name.");
    tt->help = tdrpStrDup("Name of field on which clutter will be based.");
    tt->val_offset = (char *) &dbz_field_name - &_start_;
    tt->single_val.s = tdrpStrDup("DBZ");
    tt++;
    
    // Parameter 'clutter_dbz_threshold'
    // ctype is 'double'
    
    memset(tt, 0, sizeof(TDRPtable));
    tt->ptype = DOUBLE_TYPE;
    tt->param_name = tdrpStrDup("clutter_dbz_threshold");
    tt->descr = tdrpStrDup("DBZ threshold for clutter at a gate.");
    tt->help = tdrpStrDup("If data is persistently above this value then the gate is a clutter location.");
    tt->val_offset = (char *) &clutter_dbz_threshold - &_start_;
    tt->single_val.d = 10;
    tt++;
    
    // Parameter 'use_vel_field'
    // ctype is 'tdrp_bool_t'
    
    memset(tt, 0, sizeof(TDRPtable));
    tt->ptype = BOOL_TYPE;
    tt->param_name = tdrpStrDup("use_vel_field");
    tt->descr = tdrpStrDup("Option to make use of the velocity field to identify clutter.");
    tt->help = tdrpStrDup("If true, then we check the absolute value of velocity. If it is outside the limit we do not treat the point as clutter.");
    tt->val_offset = (char *) &use_vel_field - &_start_;
    tt->single_val.b = pTRUE;
    tt++;
    
    // Parameter 'vel_field_name'
    // ctype is 'char*'
    
    memset(tt, 0, sizeof(TDRPtable));
    tt->ptype = STRING_TYPE;
    tt->param_name = tdrpStrDup("vel_field_name");
    tt->descr = tdrpStrDup("Velocity field name.");
    tt->help = tdrpStrDup("");
    tt->val_offset = (char *) &vel_field_name - &_start_;
    tt->single_val.s = tdrpStrDup("VEL");
    tt++;
    
    // Parameter 'max_abs_vel'
    // ctype is 'double'
    
    memset(tt, 0, sizeof(TDRPtable));
    tt->ptype = DOUBLE_TYPE;
    tt->param_name = tdrpStrDup("max_abs_vel");
    tt->descr = tdrpStrDup("Maximimum absolute radial velocity for clutter (m/s).");
    tt->help = tdrpStrDup("If the absolute velocity at a gate exceeds this, the point cannot be clutter.");
    tt->val_offset = (char *) &max_abs_vel - &_start_;
    tt->single_val.d = 1;
    tt++;
    
    // Parameter 'specify_clutter_fraction_threshold'
    // ctype is 'tdrp_bool_t'
    
    memset(tt, 0, sizeof(TDRPtable));
    tt->ptype = BOOL_TYPE;
    tt->param_name = tdrpStrDup("specify_clutter_fraction_threshold");
    tt->descr = tdrpStrDup("Option to specify the clutter threshold write out _latest_data_info files.");
    tt->help = tdrpStrDup("If true, the _latest_data_info files will be written after the converted file is written.");
    tt->val_offset = (char *) &specify_clutter_fraction_threshold - &_start_;
    tt->single_val.b = pFALSE;
    tt++;
    
    // Parameter 'clutter_fraction_threshold'
    // ctype is 'double'
    
    memset(tt, 0, sizeof(TDRPtable));
    tt->ptype = DOUBLE_TYPE;
    tt->param_name = tdrpStrDup("clutter_fraction_threshold");
    tt->descr = tdrpStrDup("Clutter percentile");
    tt->help = tdrpStrDup("Percentile to use in clutter value determination (see the paper, section III. Correction of Radar Reflectivity.");
    tt->val_offset = (char *) &clutter_fraction_threshold - &_start_;
    tt->single_val.d = 0.95;
    tt++;
    
    // Parameter 'Comment 6'
    
    memset(tt, 0, sizeof(TDRPtable));
    tt->ptype = COMMENT_TYPE;
    tt->param_name = tdrpStrDup("Comment 6");
    tt->comment_hdr = tdrpStrDup("Clutter statistics output");
    tt->comment_text = tdrpStrDup("Writing out the results of identifying clutter");
    tt++;
    
    // Parameter 'clutter_stats_output_dir'
    // ctype is 'char*'
    
    memset(tt, 0, sizeof(TDRPtable));
    tt->ptype = STRING_TYPE;
    tt->param_name = tdrpStrDup("clutter_stats_output_dir");
    tt->descr = tdrpStrDup("Location for final clutter statistics.");
    tt->help = tdrpStrDup("Final output is only written at the end of a processing phase, with the output time equal to the time of the first volume processed.");
    tt->val_offset = (char *) &clutter_stats_output_dir - &_start_;
    tt->single_val.s = tdrpStrDup("unknown");
    tt++;
    
    // Parameter 'dbz_mean_field_name'
    // ctype is 'char*'
    
    memset(tt, 0, sizeof(TDRPtable));
    tt->ptype = STRING_TYPE;
    tt->param_name = tdrpStrDup("dbz_mean_field_name");
    tt->descr = tdrpStrDup("Field name for mean dbz.");
    tt->help = tdrpStrDup("The dbz mean field is added to the output data set.");
    tt->val_offset = (char *) &dbz_mean_field_name - &_start_;
    tt->single_val.s = tdrpStrDup("dbzMean");
    tt++;
    
    // Parameter 'dbz_sdev_field_name'
    // ctype is 'char*'
    
    memset(tt, 0, sizeof(TDRPtable));
    tt->ptype = STRING_TYPE;
    tt->param_name = tdrpStrDup("dbz_sdev_field_name");
    tt->descr = tdrpStrDup("Field name for standard deviation of dbz.");
    tt->help = tdrpStrDup("The dbz sdev field is added to the output data set.");
    tt->val_offset = (char *) &dbz_sdev_field_name - &_start_;
    tt->single_val.s = tdrpStrDup("dbzSdev");
    tt++;
    
    // Parameter 'clut_freq_field_name'
    // ctype is 'char*'
    
    memset(tt, 0, sizeof(TDRPtable));
    tt->ptype = STRING_TYPE;
    tt->param_name = tdrpStrDup("clut_freq_field_name");
    tt->descr = tdrpStrDup("Field name for clutter frequency.");
    tt->help = tdrpStrDup("This is the fraction of time that a gate has dbz above the threshold.");
    tt->val_offset = (char *) &clut_freq_field_name - &_start_;
    tt->single_val.s = tdrpStrDup("clutFreq");
    tt++;
    
    // Parameter 'clut_flag_field_name'
    // ctype is 'char*'
    
    memset(tt, 0, sizeof(TDRPtable));
    tt->ptype = STRING_TYPE;
    tt->param_name = tdrpStrDup("clut_flag_field_name");
    tt->descr = tdrpStrDup("Field name for clutter flag.");
    tt->help = tdrpStrDup("This flag indicates that the gate has persistent clutter.");
    tt->val_offset = (char *) &clut_flag_field_name - &_start_;
    tt->single_val.s = tdrpStrDup("clutFlag");
    tt++;
    
    // Parameter 'write_latest_data_info'
    // ctype is 'tdrp_bool_t'
    
    memset(tt, 0, sizeof(TDRPtable));
    tt->ptype = BOOL_TYPE;
    tt->param_name = tdrpStrDup("write_latest_data_info");
    tt->descr = tdrpStrDup("Option to write out _latest_data_info files.");
    tt->help = tdrpStrDup("If true, the _latest_data_info files will be written after the converted file is written.");
    tt->val_offset = (char *) &write_latest_data_info - &_start_;
    tt->single_val.b = pFALSE;
    tt++;
    
    // Parameter 'Comment 7'
    
    memset(tt, 0, sizeof(TDRPtable));
    tt->ptype = COMMENT_TYPE;
    tt->param_name = tdrpStrDup("Comment 7");
    tt->comment_hdr = tdrpStrDup("Clutter-removed output");
    tt->comment_text = tdrpStrDup("Writing out volumes with clutter removed.");
    tt++;
    
    // Parameter 'clutter_removed_output_dir'
    // ctype is 'char*'
    
    memset(tt, 0, sizeof(TDRPtable));
    tt->ptype = STRING_TYPE;
    tt->param_name = tdrpStrDup("clutter_removed_output_dir");
    tt->descr = tdrpStrDup("Location for final clutter statistics.");
    tt->help = tdrpStrDup("Final output is only written at the end of a processing phase, with the output time equal to the time of the first volume processed.");
    tt->val_offset = (char *) &clutter_removed_output_dir - &_start_;
    tt->single_val.s = tdrpStrDup("unknown");
    tt++;
    
    // trailing entry has param_name set to NULL
    
    tt->param_name = NULL;
    
    return;
  
  }
