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
// Code for program HydroStation2Symprod
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
    tt->comment_hdr = tdrpStrDup("DEBUGGING");
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
    tt->enum_def.nfields = 3;
    tt->enum_def.fields = (enum_field_t *)
        tdrpMalloc(tt->enum_def.nfields * sizeof(enum_field_t));
      tt->enum_def.fields[0].name = tdrpStrDup("DEBUG_OFF");
      tt->enum_def.fields[0].val = DEBUG_OFF;
      tt->enum_def.fields[1].name = tdrpStrDup("DEBUG_NORM");
      tt->enum_def.fields[1].val = DEBUG_NORM;
      tt->enum_def.fields[2].name = tdrpStrDup("DEBUG_VERBOSE");
      tt->enum_def.fields[2].val = DEBUG_VERBOSE;
    tt->single_val.e = DEBUG_OFF;
    tt++;
    
    // Parameter 'Comment 1'
    
    memset(tt, 0, sizeof(TDRPtable));
    tt->ptype = COMMENT_TYPE;
    tt->param_name = tdrpStrDup("Comment 1");
    tt->comment_hdr = tdrpStrDup("PROCESS CONTROL");
    tt->comment_text = tdrpStrDup("");
    tt++;
    
    // Parameter 'instance'
    // ctype is 'char*'
    
    memset(tt, 0, sizeof(TDRPtable));
    tt->ptype = STRING_TYPE;
    tt->param_name = tdrpStrDup("instance");
    tt->descr = tdrpStrDup("Process instance.");
    tt->help = tdrpStrDup("Used for procmap registration and auto restarting.");
    tt->val_offset = (char *) &instance - &_start_;
    tt->single_val.s = tdrpStrDup("");
    tt++;
    
    // Parameter 'no_threads'
    // ctype is 'tdrp_bool_t'
    
    memset(tt, 0, sizeof(TDRPtable));
    tt->ptype = BOOL_TYPE;
    tt->param_name = tdrpStrDup("no_threads");
    tt->descr = tdrpStrDup("Option to prevent server from using a thread per client.");
    tt->help = tdrpStrDup("For debugging purposes it it sometimes useful to suppress the use of threads. Set no_threads to TRUE for this type of debugging.");
    tt->val_offset = (char *) &no_threads - &_start_;
    tt->single_val.b = pFALSE;
    tt++;
    
    // Parameter 'Comment 2'
    
    memset(tt, 0, sizeof(TDRPtable));
    tt->ptype = COMMENT_TYPE;
    tt->param_name = tdrpStrDup("Comment 2");
    tt->comment_hdr = tdrpStrDup("Server details for support from ServerMgr");
    tt->comment_text = tdrpStrDup("");
    tt++;
    
    // Parameter 'port'
    // ctype is 'int'
    
    memset(tt, 0, sizeof(TDRPtable));
    tt->ptype = INT_TYPE;
    tt->param_name = tdrpStrDup("port");
    tt->descr = tdrpStrDup("Port number.");
    tt->help = tdrpStrDup("The server listens on this port for client requests.");
    tt->val_offset = (char *) &port - &_start_;
    tt->single_val.i = 5468;
    tt++;
    
    // Parameter 'qmax'
    // ctype is 'int'
    
    memset(tt, 0, sizeof(TDRPtable));
    tt->ptype = INT_TYPE;
    tt->param_name = tdrpStrDup("qmax");
    tt->descr = tdrpStrDup("Max quiescent period (secs).");
    tt->help = tdrpStrDup("If the server does not receive requests for this time period, it will die gracefully. If set to -1, the server never exits.");
    tt->val_offset = (char *) &qmax - &_start_;
    tt->single_val.i = -1;
    tt++;
    
    // Parameter 'max_clients'
    // ctype is 'int'
    
    memset(tt, 0, sizeof(TDRPtable));
    tt->ptype = INT_TYPE;
    tt->param_name = tdrpStrDup("max_clients");
    tt->descr = tdrpStrDup("Maximum number of clients");
    tt->help = tdrpStrDup("This is the maximum number of threads the application will produce to handle client requests.  If the maximum is reached, new clients will receive a SERVICE_DENIED error message and will have to request the data again.  If set to -1, no maximum is enforced.");
    tt->val_offset = (char *) &max_clients - &_start_;
    tt->single_val.i = 64;
    tt++;
    
    // Parameter 'Comment 3'
    
    memset(tt, 0, sizeof(TDRPtable));
    tt->ptype = COMMENT_TYPE;
    tt->param_name = tdrpStrDup("Comment 3");
    tt->comment_hdr = tdrpStrDup("RENDERING PARAMETERS");
    tt->comment_text = tdrpStrDup("");
    tt++;
    
    // Parameter 'field_render_list'
    // ctype is '_field_render_t'
    
    memset(tt, 0, sizeof(TDRPtable));
    tt->ptype = STRUCT_TYPE;
    tt->param_name = tdrpStrDup("field_render_list");
    tt->descr = tdrpStrDup("Information about how to render the field data values.");
    tt->help = tdrpStrDup("After multiplying by the multiplier field data values will be rendered textually as indicated. ");
    tt->array_offset = (char *) &_field_render_list - &_start_;
    tt->array_n_offset = (char *) &field_render_list_n - &_start_;
    tt->is_array = TRUE;
    tt->array_len_fixed = FALSE;
    tt->array_elem_size = sizeof(field_render_t);
    tt->array_n = 1;
    tt->struct_def.name = tdrpStrDup("field_render_t");
    tt->struct_def.nfields = 12;
    tt->struct_def.fields = (struct_field_t *)
        tdrpMalloc(tt->struct_def.nfields * sizeof(struct_field_t));
      tt->struct_def.fields[0].ftype = tdrpStrDup("field_t");
      tt->struct_def.fields[0].fname = tdrpStrDup("field");
      tt->struct_def.fields[0].ptype = ENUM_TYPE;
      tt->struct_def.fields[0].rel_offset = 
        (char *) &_field_render_list->field - (char *) _field_render_list;
        tt->struct_def.fields[0].enum_def.name = tdrpStrDup("field_t");
        tt->struct_def.fields[0].enum_def.nfields = 12;
        tt->struct_def.fields[0].enum_def.fields = (enum_field_t *) tdrpMalloc
          (tt->struct_def.fields[0].enum_def.nfields * sizeof(enum_field_t));
        tt->struct_def.fields[0].enum_def.fields[0].name = tdrpStrDup("WIND_SPEED_FIELD");
        tt->struct_def.fields[0].enum_def.fields[0].val = WIND_SPEED_FIELD;
        tt->struct_def.fields[0].enum_def.fields[1].name = tdrpStrDup("WIND_DIR_FIELD");
        tt->struct_def.fields[0].enum_def.fields[1].val = WIND_DIR_FIELD;
        tt->struct_def.fields[0].enum_def.fields[2].name = tdrpStrDup("TEMPERATURE_FIELD");
        tt->struct_def.fields[0].enum_def.fields[2].val = TEMPERATURE_FIELD;
        tt->struct_def.fields[0].enum_def.fields[3].name = tdrpStrDup("REL_HUM_FIELD");
        tt->struct_def.fields[0].enum_def.fields[3].val = REL_HUM_FIELD;
        tt->struct_def.fields[0].enum_def.fields[4].name = tdrpStrDup("RAINFALL_FIELD");
        tt->struct_def.fields[0].enum_def.fields[4].val = RAINFALL_FIELD;
        tt->struct_def.fields[0].enum_def.fields[5].name = tdrpStrDup("SOLAR_RAD_FIELD");
        tt->struct_def.fields[0].enum_def.fields[5].val = SOLAR_RAD_FIELD;
        tt->struct_def.fields[0].enum_def.fields[6].name = tdrpStrDup("PRESSURE_FIELD");
        tt->struct_def.fields[0].enum_def.fields[6].val = PRESSURE_FIELD;
        tt->struct_def.fields[0].enum_def.fields[7].name = tdrpStrDup("SOIL_MOIST1_FIELD");
        tt->struct_def.fields[0].enum_def.fields[7].val = SOIL_MOIST1_FIELD;
        tt->struct_def.fields[0].enum_def.fields[8].name = tdrpStrDup("SOIL_MOIST2_FIELD");
        tt->struct_def.fields[0].enum_def.fields[8].val = SOIL_MOIST2_FIELD;
        tt->struct_def.fields[0].enum_def.fields[9].name = tdrpStrDup("SOIL_MOIST3_FIELD");
        tt->struct_def.fields[0].enum_def.fields[9].val = SOIL_MOIST3_FIELD;
        tt->struct_def.fields[0].enum_def.fields[10].name = tdrpStrDup("SOIL_MOIST4_FIELD");
        tt->struct_def.fields[0].enum_def.fields[10].val = SOIL_MOIST4_FIELD;
        tt->struct_def.fields[0].enum_def.fields[11].name = tdrpStrDup("SOIL_TEMP_FIELD");
        tt->struct_def.fields[0].enum_def.fields[11].val = SOIL_TEMP_FIELD;
      tt->struct_def.fields[1].ftype = tdrpStrDup("double");
      tt->struct_def.fields[1].fname = tdrpStrDup("multiplier");
      tt->struct_def.fields[1].ptype = DOUBLE_TYPE;
      tt->struct_def.fields[1].rel_offset = 
        (char *) &_field_render_list->multiplier - (char *) _field_render_list;
      tt->struct_def.fields[2].ftype = tdrpStrDup("string");
      tt->struct_def.fields[2].fname = tdrpStrDup("value_format_string");
      tt->struct_def.fields[2].ptype = STRING_TYPE;
      tt->struct_def.fields[2].rel_offset = 
        (char *) &_field_render_list->value_format_string - (char *) _field_render_list;
      tt->struct_def.fields[3].ftype = tdrpStrDup("int");
      tt->struct_def.fields[3].fname = tdrpStrDup("x_offset");
      tt->struct_def.fields[3].ptype = INT_TYPE;
      tt->struct_def.fields[3].rel_offset = 
        (char *) &_field_render_list->x_offset - (char *) _field_render_list;
      tt->struct_def.fields[4].ftype = tdrpStrDup("int");
      tt->struct_def.fields[4].fname = tdrpStrDup("y_offset");
      tt->struct_def.fields[4].ptype = INT_TYPE;
      tt->struct_def.fields[4].rel_offset = 
        (char *) &_field_render_list->y_offset - (char *) _field_render_list;
      tt->struct_def.fields[5].ftype = tdrpStrDup("string");
      tt->struct_def.fields[5].fname = tdrpStrDup("font_name");
      tt->struct_def.fields[5].ptype = STRING_TYPE;
      tt->struct_def.fields[5].rel_offset = 
        (char *) &_field_render_list->font_name - (char *) _field_render_list;
      tt->struct_def.fields[6].ftype = tdrpStrDup("string");
      tt->struct_def.fields[6].fname = tdrpStrDup("color_name");
      tt->struct_def.fields[6].ptype = STRING_TYPE;
      tt->struct_def.fields[6].rel_offset = 
        (char *) &_field_render_list->color_name - (char *) _field_render_list;
      tt->struct_def.fields[7].ftype = tdrpStrDup("string");
      tt->struct_def.fields[7].fname = tdrpStrDup("background_color_name");
      tt->struct_def.fields[7].ptype = STRING_TYPE;
      tt->struct_def.fields[7].rel_offset = 
        (char *) &_field_render_list->background_color_name - (char *) _field_render_list;
      tt->struct_def.fields[8].ftype = tdrpStrDup("vert_align_t");
      tt->struct_def.fields[8].fname = tdrpStrDup("vert_align");
      tt->struct_def.fields[8].ptype = ENUM_TYPE;
      tt->struct_def.fields[8].rel_offset = 
        (char *) &_field_render_list->vert_align - (char *) _field_render_list;
        tt->struct_def.fields[8].enum_def.name = tdrpStrDup("vert_align_t");
        tt->struct_def.fields[8].enum_def.nfields = 3;
        tt->struct_def.fields[8].enum_def.fields = (enum_field_t *) tdrpMalloc
          (tt->struct_def.fields[8].enum_def.nfields * sizeof(enum_field_t));
        tt->struct_def.fields[8].enum_def.fields[0].name = tdrpStrDup("VERT_ALIGN_TOP");
        tt->struct_def.fields[8].enum_def.fields[0].val = VERT_ALIGN_TOP;
        tt->struct_def.fields[8].enum_def.fields[1].name = tdrpStrDup("VERT_ALIGN_CENTER");
        tt->struct_def.fields[8].enum_def.fields[1].val = VERT_ALIGN_CENTER;
        tt->struct_def.fields[8].enum_def.fields[2].name = tdrpStrDup("VERT_ALIGN_BOTTOM");
        tt->struct_def.fields[8].enum_def.fields[2].val = VERT_ALIGN_BOTTOM;
      tt->struct_def.fields[9].ftype = tdrpStrDup("horiz_align_t");
      tt->struct_def.fields[9].fname = tdrpStrDup("horiz_align");
      tt->struct_def.fields[9].ptype = ENUM_TYPE;
      tt->struct_def.fields[9].rel_offset = 
        (char *) &_field_render_list->horiz_align - (char *) _field_render_list;
        tt->struct_def.fields[9].enum_def.name = tdrpStrDup("horiz_align_t");
        tt->struct_def.fields[9].enum_def.nfields = 3;
        tt->struct_def.fields[9].enum_def.fields = (enum_field_t *) tdrpMalloc
          (tt->struct_def.fields[9].enum_def.nfields * sizeof(enum_field_t));
        tt->struct_def.fields[9].enum_def.fields[0].name = tdrpStrDup("HORIZ_ALIGN_LEFT");
        tt->struct_def.fields[9].enum_def.fields[0].val = HORIZ_ALIGN_LEFT;
        tt->struct_def.fields[9].enum_def.fields[1].name = tdrpStrDup("HORIZ_ALIGN_CENTER");
        tt->struct_def.fields[9].enum_def.fields[1].val = HORIZ_ALIGN_CENTER;
        tt->struct_def.fields[9].enum_def.fields[2].name = tdrpStrDup("HORIZ_ALIGN_RIGHT");
        tt->struct_def.fields[9].enum_def.fields[2].val = HORIZ_ALIGN_RIGHT;
      tt->struct_def.fields[10].ftype = tdrpStrDup("long");
      tt->struct_def.fields[10].fname = tdrpStrDup("font_size");
      tt->struct_def.fields[10].ptype = LONG_TYPE;
      tt->struct_def.fields[10].rel_offset = 
        (char *) &_field_render_list->font_size - (char *) _field_render_list;
      tt->struct_def.fields[11].ftype = tdrpStrDup("font_style_t");
      tt->struct_def.fields[11].fname = tdrpStrDup("font_style");
      tt->struct_def.fields[11].ptype = ENUM_TYPE;
      tt->struct_def.fields[11].rel_offset = 
        (char *) &_field_render_list->font_style - (char *) _field_render_list;
        tt->struct_def.fields[11].enum_def.name = tdrpStrDup("font_style_t");
        tt->struct_def.fields[11].enum_def.nfields = 7;
        tt->struct_def.fields[11].enum_def.fields = (enum_field_t *) tdrpMalloc
          (tt->struct_def.fields[11].enum_def.nfields * sizeof(enum_field_t));
        tt->struct_def.fields[11].enum_def.fields[0].name = tdrpStrDup("TEXT_NORM");
        tt->struct_def.fields[11].enum_def.fields[0].val = TEXT_NORM;
        tt->struct_def.fields[11].enum_def.fields[1].name = tdrpStrDup("TEXT_BOLD");
        tt->struct_def.fields[11].enum_def.fields[1].val = TEXT_BOLD;
        tt->struct_def.fields[11].enum_def.fields[2].name = tdrpStrDup("TEXT_ITALICS");
        tt->struct_def.fields[11].enum_def.fields[2].val = TEXT_ITALICS;
        tt->struct_def.fields[11].enum_def.fields[3].name = tdrpStrDup("TEXT_SUBSCRIPT");
        tt->struct_def.fields[11].enum_def.fields[3].val = TEXT_SUBSCRIPT;
        tt->struct_def.fields[11].enum_def.fields[4].name = tdrpStrDup("TEXT_SUPERSCRIPT");
        tt->struct_def.fields[11].enum_def.fields[4].val = TEXT_SUPERSCRIPT;
        tt->struct_def.fields[11].enum_def.fields[5].name = tdrpStrDup("TEXT_UNDERLINE");
        tt->struct_def.fields[11].enum_def.fields[5].val = TEXT_UNDERLINE;
        tt->struct_def.fields[11].enum_def.fields[6].name = tdrpStrDup("TEXT_STRIKETHROUGH");
        tt->struct_def.fields[11].enum_def.fields[6].val = TEXT_STRIKETHROUGH;
    tt->n_struct_vals = 12;
    tt->struct_vals = (tdrpVal_t *)
        tdrpMalloc(tt->n_struct_vals * sizeof(tdrpVal_t));
      tt->struct_vals[0].e = RAINFALL_FIELD;
      tt->struct_vals[1].d = 1;
      tt->struct_vals[2].s = tdrpStrDup("%d mm");
      tt->struct_vals[3].i = 5;
      tt->struct_vals[4].i = -5;
      tt->struct_vals[5].s = tdrpStrDup("");
      tt->struct_vals[6].s = tdrpStrDup("red");
      tt->struct_vals[7].s = tdrpStrDup("");
      tt->struct_vals[8].e = VERT_ALIGN_CENTER;
      tt->struct_vals[9].e = HORIZ_ALIGN_LEFT;
      tt->struct_vals[10].l = 10;
      tt->struct_vals[11].e = TEXT_NORM;
    tt++;
    
    // trailing entry has param_name set to NULL
    
    tt->param_name = NULL;
    
    return;
  
  }
