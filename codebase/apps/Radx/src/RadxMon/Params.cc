// *=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
// ** Copyright UCAR (c) 1992 - 2016
// ** University Corporation for Atmospheric Research(UCAR)
// ** National Center for Atmospheric Research(NCAR)
// ** Boulder, Colorado, USA
// *=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
////////////////////////////////////////////
// Params.cc
//
// TDRP C++ code file for class 'Params'.
//
// Code for program RadxMon
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
using namespace std;

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
    tt->comment_hdr = tdrpStrDup("Read moments data, print out in a variety of ways.");
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
    tt->descr = tdrpStrDup("Process instance");
    tt->help = tdrpStrDup("Used for registration with the process mapper");
    tt->val_offset = (char *) &instance - &_start_;
    tt->single_val.s = tdrpStrDup("test");
    tt++;
    
    // Parameter 'Comment 1'
    
    memset(tt, 0, sizeof(TDRPtable));
    tt->ptype = COMMENT_TYPE;
    tt->param_name = tdrpStrDup("Comment 1");
    tt->comment_hdr = tdrpStrDup("OPS MODE");
    tt->comment_text = tdrpStrDup("");
    tt++;
    
    // Parameter 'ops_mode'
    // ctype is '_ops_mode_t'
    
    memset(tt, 0, sizeof(TDRPtable));
    tt->ptype = ENUM_TYPE;
    tt->param_name = tdrpStrDup("ops_mode");
    tt->descr = tdrpStrDup("How the app operates.");
    tt->help = tdrpStrDup("\tMODE_PRINT: read moments, print to stdout. MODE_SERVER: reads moments and keeps current state. Serves out current data in response to request for information.");
    tt->val_offset = (char *) &ops_mode - &_start_;
    tt->enum_def.name = tdrpStrDup("ops_mode_t");
    tt->enum_def.nfields = 2;
    tt->enum_def.fields = (enum_field_t *)
        tdrpMalloc(tt->enum_def.nfields * sizeof(enum_field_t));
      tt->enum_def.fields[0].name = tdrpStrDup("OPS_MODE_PRINT");
      tt->enum_def.fields[0].val = OPS_MODE_PRINT;
      tt->enum_def.fields[1].name = tdrpStrDup("OPS_MODE_SERVER");
      tt->enum_def.fields[1].val = OPS_MODE_SERVER;
    tt->single_val.e = OPS_MODE_PRINT;
    tt++;
    
    // Parameter 'Comment 2'
    
    memset(tt, 0, sizeof(TDRPtable));
    tt->ptype = COMMENT_TYPE;
    tt->param_name = tdrpStrDup("Comment 2");
    tt->comment_hdr = tdrpStrDup("DATA INPUT");
    tt->comment_text = tdrpStrDup("");
    tt++;
    
    // Parameter 'input_mode'
    // ctype is '_input_mode_t'
    
    memset(tt, 0, sizeof(TDRPtable));
    tt->ptype = ENUM_TYPE;
    tt->param_name = tdrpStrDup("input_mode");
    tt->descr = tdrpStrDup("Input mode - how the data is read.");
    tt->help = tdrpStrDup("\tFMQ_INPUT: read moments from a file message queue./n/tTCP_INPUT: read the moments data from a server, via a socket./n/tFILE_LIST: read data from files specified on the command line.\n\tFILE_REALTIME: watch an input directory, read each new file as it arrives.");
    tt->val_offset = (char *) &input_mode - &_start_;
    tt->enum_def.name = tdrpStrDup("input_mode_t");
    tt->enum_def.nfields = 4;
    tt->enum_def.fields = (enum_field_t *)
        tdrpMalloc(tt->enum_def.nfields * sizeof(enum_field_t));
      tt->enum_def.fields[0].name = tdrpStrDup("FMQ_INPUT");
      tt->enum_def.fields[0].val = FMQ_INPUT;
      tt->enum_def.fields[1].name = tdrpStrDup("TCP_INPUT");
      tt->enum_def.fields[1].val = TCP_INPUT;
      tt->enum_def.fields[2].name = tdrpStrDup("FILE_LIST");
      tt->enum_def.fields[2].val = FILE_LIST;
      tt->enum_def.fields[3].name = tdrpStrDup("FILE_REALTIME");
      tt->enum_def.fields[3].val = FILE_REALTIME;
    tt->single_val.e = FMQ_INPUT;
    tt++;
    
    // Parameter 'fmq_url'
    // ctype is 'char*'
    
    memset(tt, 0, sizeof(TDRPtable));
    tt->ptype = STRING_TYPE;
    tt->param_name = tdrpStrDup("fmq_url");
    tt->descr = tdrpStrDup("FMQ url. For FMQ_INPUT only.");
    tt->help = tdrpStrDup("URL to FMQ files. There are 2 files, one with a .buf extension and one with a .stat extention. This path does not include the extensions. For local FMQ, this can be the file path. For a remote FMQ, this will be a full URL: 'fmqp:://host::dir'.");
    tt->val_offset = (char *) &fmq_url - &_start_;
    tt->single_val.s = tdrpStrDup("/tmp/fmq/moments");
    tt++;
    
    // Parameter 'seek_to_start_of_fmq'
    // ctype is 'tdrp_bool_t'
    
    memset(tt, 0, sizeof(TDRPtable));
    tt->ptype = BOOL_TYPE;
    tt->param_name = tdrpStrDup("seek_to_start_of_fmq");
    tt->descr = tdrpStrDup("Option to seek to the start of the input FMQ.");
    tt->help = tdrpStrDup("If FALSE, the program will seek to the end of the fmq and only read in new data. If TRUE, it will start reading from the beginning of the FMQ.");
    tt->val_offset = (char *) &seek_to_start_of_fmq - &_start_;
    tt->single_val.b = pFALSE;
    tt++;
    
    // Parameter 'input_tcp_host'
    // ctype is 'char*'
    
    memset(tt, 0, sizeof(TDRPtable));
    tt->ptype = STRING_TYPE;
    tt->param_name = tdrpStrDup("input_tcp_host");
    tt->descr = tdrpStrDup("Name of host for TCP server. For TCP_INPUT only.");
    tt->help = tdrpStrDup("");
    tt->val_offset = (char *) &input_tcp_host - &_start_;
    tt->single_val.s = tdrpStrDup("localhost");
    tt++;
    
    // Parameter 'input_tcp_port'
    // ctype is 'int'
    
    memset(tt, 0, sizeof(TDRPtable));
    tt->ptype = INT_TYPE;
    tt->param_name = tdrpStrDup("input_tcp_port");
    tt->descr = tdrpStrDup("Port for TCP server. For TCP_INPUT only.");
    tt->help = tdrpStrDup("");
    tt->val_offset = (char *) &input_tcp_port - &_start_;
    tt->single_val.i = 12000;
    tt++;
    
    // Parameter 'files_input_dir'
    // ctype is 'char*'
    
    memset(tt, 0, sizeof(TDRPtable));
    tt->ptype = STRING_TYPE;
    tt->param_name = tdrpStrDup("files_input_dir");
    tt->descr = tdrpStrDup("Input dir for files. For FILE_REALTIME only.");
    tt->help = tdrpStrDup("Will watch this directory for new data.");
    tt->val_offset = (char *) &files_input_dir - &_start_;
    tt->single_val.s = tdrpStrDup("/tmp/files/moments");
    tt++;
    
    // Parameter 'Comment 3'
    
    memset(tt, 0, sizeof(TDRPtable));
    tt->ptype = COMMENT_TYPE;
    tt->param_name = tdrpStrDup("Comment 3");
    tt->comment_hdr = tdrpStrDup("PRINT MODE");
    tt->comment_text = tdrpStrDup("");
    tt++;
    
    // Parameter 'print_mode'
    // ctype is '_print_mode_t'
    
    memset(tt, 0, sizeof(TDRPtable));
    tt->ptype = ENUM_TYPE;
    tt->param_name = tdrpStrDup("print_mode");
    tt->descr = tdrpStrDup("Sets the type of printout. All information will be printed to stdout.");
    tt->help = tdrpStrDup("\nMONITOR: single line which overwrites itself. Suitable for a small window with a single output line. SUMMARY: scrolling summary. \n\nFULL: radar, field and beam params in full. \n\nDATA: data as well - very verbose. \n\nSINGLE_GATE: data for a single gate at specficied range. \n\nPOWER_AND_FREQ: power and frequency summary \n\nPLATFORM_GEOREF: platform georeference info with each beam, if available");
    tt->val_offset = (char *) &print_mode - &_start_;
    tt->enum_def.name = tdrpStrDup("print_mode_t");
    tt->enum_def.nfields = 7;
    tt->enum_def.fields = (enum_field_t *)
        tdrpMalloc(tt->enum_def.nfields * sizeof(enum_field_t));
      tt->enum_def.fields[0].name = tdrpStrDup("PRINT_MONITOR");
      tt->enum_def.fields[0].val = PRINT_MONITOR;
      tt->enum_def.fields[1].name = tdrpStrDup("PRINT_SUMMARY");
      tt->enum_def.fields[1].val = PRINT_SUMMARY;
      tt->enum_def.fields[2].name = tdrpStrDup("PRINT_FULL");
      tt->enum_def.fields[2].val = PRINT_FULL;
      tt->enum_def.fields[3].name = tdrpStrDup("PRINT_DATA");
      tt->enum_def.fields[3].val = PRINT_DATA;
      tt->enum_def.fields[4].name = tdrpStrDup("PRINT_SINGLE_GATE");
      tt->enum_def.fields[4].val = PRINT_SINGLE_GATE;
      tt->enum_def.fields[5].name = tdrpStrDup("PRINT_POWER_AND_FREQ");
      tt->enum_def.fields[5].val = PRINT_POWER_AND_FREQ;
      tt->enum_def.fields[6].name = tdrpStrDup("PRINT_PLATFORM_GEOREF");
      tt->enum_def.fields[6].val = PRINT_PLATFORM_GEOREF;
    tt->single_val.e = PRINT_SUMMARY;
    tt++;
    
    // Parameter 'range_for_single_gate'
    // ctype is 'double'
    
    memset(tt, 0, sizeof(TDRPtable));
    tt->ptype = DOUBLE_TYPE;
    tt->param_name = tdrpStrDup("range_for_single_gate");
    tt->descr = tdrpStrDup("Range for printing out data for single gate - km.");
    tt->help = tdrpStrDup("For print_type = SINGLE_GATE.");
    tt->val_offset = (char *) &range_for_single_gate - &_start_;
    tt->single_val.d = 1;
    tt++;
    
    // Parameter 'Comment 4'
    
    memset(tt, 0, sizeof(TDRPtable));
    tt->ptype = COMMENT_TYPE;
    tt->param_name = tdrpStrDup("Comment 4");
    tt->comment_hdr = tdrpStrDup("PRINT DETAILS");
    tt->comment_text = tdrpStrDup("");
    tt++;
    
    // Parameter 'labels_in_monitor_mode'
    // ctype is 'tdrp_bool_t'
    
    memset(tt, 0, sizeof(TDRPtable));
    tt->ptype = BOOL_TYPE;
    tt->param_name = tdrpStrDup("labels_in_monitor_mode");
    tt->descr = tdrpStrDup("Option to print label line in monitor mode.");
    tt->help = tdrpStrDup("If TRUE, a line containing labels is printed ahead of the data line in monitor mode.");
    tt->val_offset = (char *) &labels_in_monitor_mode - &_start_;
    tt->single_val.b = pFALSE;
    tt++;
    
    // Parameter 'volume_num_in_monitor_mode'
    // ctype is 'tdrp_bool_t'
    
    memset(tt, 0, sizeof(TDRPtable));
    tt->ptype = BOOL_TYPE;
    tt->param_name = tdrpStrDup("volume_num_in_monitor_mode");
    tt->descr = tdrpStrDup("Option to print volume number in monitor mode.");
    tt->help = tdrpStrDup("If TRUE, the volume number is printed at the start of the line in monitor mode.");
    tt->val_offset = (char *) &volume_num_in_monitor_mode - &_start_;
    tt->single_val.b = pFALSE;
    tt++;
    
    // Parameter 'flags_in_monitor_mode'
    // ctype is 'tdrp_bool_t'
    
    memset(tt, 0, sizeof(TDRPtable));
    tt->ptype = BOOL_TYPE;
    tt->param_name = tdrpStrDup("flags_in_monitor_mode");
    tt->descr = tdrpStrDup("Option to print start of tilt, end of volume, etc. flags in monitor mode.");
    tt->help = tdrpStrDup("If TRUE, a 3-letter string is added to the end of the line. The strings are EOT (end-of-tilt), EOV (end-of-volume), SOT (start-of-tilt), SOV (start-of-volume) and NST (new-scan-type).");
    tt->val_offset = (char *) &flags_in_monitor_mode - &_start_;
    tt->single_val.b = pFALSE;
    tt++;
    
    // Parameter 'flags_in_summary_mode'
    // ctype is 'tdrp_bool_t'
    
    memset(tt, 0, sizeof(TDRPtable));
    tt->ptype = BOOL_TYPE;
    tt->param_name = tdrpStrDup("flags_in_summary_mode");
    tt->descr = tdrpStrDup("Option to print start/end of tilt flags in summary mode.");
    tt->help = tdrpStrDup("If TRUE, the start and end of tilts will be printed in summary mode.");
    tt->val_offset = (char *) &flags_in_summary_mode - &_start_;
    tt->single_val.b = pFALSE;
    tt++;
    
    // Parameter 'subsecs_precision_in_summary_mode'
    // ctype is 'int'
    
    memset(tt, 0, sizeof(TDRPtable));
    tt->ptype = INT_TYPE;
    tt->param_name = tdrpStrDup("subsecs_precision_in_summary_mode");
    tt->descr = tdrpStrDup("Number of decimals for partial secs in summary mode.");
    tt->help = tdrpStrDup("If TRUE, the time will be printed out to this number of decimals.");
    tt->val_offset = (char *) &subsecs_precision_in_summary_mode - &_start_;
    tt->single_val.i = 0;
    tt++;
    
    // Parameter 'header_interval_in_summary_mode'
    // ctype is 'int'
    
    memset(tt, 0, sizeof(TDRPtable));
    tt->ptype = INT_TYPE;
    tt->param_name = tdrpStrDup("header_interval_in_summary_mode");
    tt->descr = tdrpStrDup("Number of lines between headers in summary mode.");
    tt->help = tdrpStrDup("A line header will be printed at regular intervals.");
    tt->val_offset = (char *) &header_interval_in_summary_mode - &_start_;
    tt->single_val.i = 10;
    tt++;
    
    // Parameter 'update_interval'
    // ctype is 'int'
    
    memset(tt, 0, sizeof(TDRPtable));
    tt->ptype = INT_TYPE;
    tt->param_name = tdrpStrDup("update_interval");
    tt->descr = tdrpStrDup("Update interval for display in secs.");
    tt->help = tdrpStrDup("Set to negative values to update based on count rather than seconds between beam timestamps. If 0, prints on every beam.");
    tt->val_offset = (char *) &update_interval - &_start_;
    tt->single_val.i = -1;
    tt++;
    
    // Parameter 'n_monitor_line_feeds'
    // ctype is 'int'
    
    memset(tt, 0, sizeof(TDRPtable));
    tt->ptype = INT_TYPE;
    tt->param_name = tdrpStrDup("n_monitor_line_feeds");
    tt->descr = tdrpStrDup("Number of line feeds in monitor mode.");
    tt->help = tdrpStrDup("The number of line feeds inserted before refreshing the line in monitor mode.");
    tt->val_offset = (char *) &n_monitor_line_feeds - &_start_;
    tt->single_val.i = 0;
    tt++;
    
    // Parameter 'Comment 5'
    
    memset(tt, 0, sizeof(TDRPtable));
    tt->ptype = COMMENT_TYPE;
    tt->param_name = tdrpStrDup("Comment 5");
    tt->comment_hdr = tdrpStrDup("CHECK FOR MISSING DATA");
    tt->comment_text = tdrpStrDup("");
    tt++;
    
    // Parameter 'check_for_missing_beams'
    // ctype is 'tdrp_bool_t'
    
    memset(tt, 0, sizeof(TDRPtable));
    tt->ptype = BOOL_TYPE;
    tt->param_name = tdrpStrDup("check_for_missing_beams");
    tt->descr = tdrpStrDup("Option to check for missing beams.");
    tt->help = tdrpStrDup("If TRUE, RadxMon will check for missing beams by computing the change in angle since the previous beam. If this exceeds 'max_delta_angle', a warning message will be printed.");
    tt->val_offset = (char *) &check_for_missing_beams - &_start_;
    tt->single_val.b = pFALSE;
    tt++;
    
    // Parameter 'max_delta_angle'
    // ctype is 'double'
    
    memset(tt, 0, sizeof(TDRPtable));
    tt->ptype = DOUBLE_TYPE;
    tt->param_name = tdrpStrDup("max_delta_angle");
    tt->descr = tdrpStrDup("Max delta angle for missing beam check (deg).");
    tt->help = tdrpStrDup("If the angle of the latest beam differs from the angle of the previous beam by more than this value, it will be assumed that one or more beams are missing. For RHI scans, the 'angle' referred to here is the 'elevation angle'. For other scans, it is the 'azimuth angle'.");
    tt->val_offset = (char *) &max_delta_angle - &_start_;
    tt->single_val.d = 1;
    tt++;
    
    // Parameter 'Comment 6'
    
    memset(tt, 0, sizeof(TDRPtable));
    tt->ptype = COMMENT_TYPE;
    tt->param_name = tdrpStrDup("Comment 6");
    tt->comment_hdr = tdrpStrDup("SERVER MODE");
    tt->comment_text = tdrpStrDup("");
    tt++;
    
    // Parameter 'output_tcp_port'
    // ctype is 'int'
    
    memset(tt, 0, sizeof(TDRPtable));
    tt->ptype = INT_TYPE;
    tt->param_name = tdrpStrDup("output_tcp_port");
    tt->descr = tdrpStrDup("TCP port for server mode.");
    tt->help = tdrpStrDup("Listens for connections on this port.");
    tt->val_offset = (char *) &output_tcp_port - &_start_;
    tt->single_val.i = 10000;
    tt++;
    
    // Parameter 'nsecs_for_antenna_rate'
    // ctype is 'double'
    
    memset(tt, 0, sizeof(TDRPtable));
    tt->ptype = DOUBLE_TYPE;
    tt->param_name = tdrpStrDup("nsecs_for_antenna_rate");
    tt->descr = tdrpStrDup("Time for computing antenna rate (sec).");
    tt->help = tdrpStrDup("The rate computed over this time period. The rate is used to adjust for latency in the angles.");
    tt->val_offset = (char *) &nsecs_for_antenna_rate - &_start_;
    tt->single_val.d = 2;
    tt++;
    
    // trailing entry has param_name set to NULL
    
    tt->param_name = NULL;
    
    return;
  
  }
