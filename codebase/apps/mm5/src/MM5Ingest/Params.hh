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
// Params.hh
//
// TDRP header file for 'Params' class.
//
// Code for program MM5Ingest
//
// This header file has been automatically
// generated by TDRP, do not modify.
//
/////////////////////////////////////////////

/**
 *
 * @file Params.hh
 *
 * This class is automatically generated by the Table
 * Driven Runtime Parameters (TDRP) system
 *
 * @class Params
 *
 * @author automatically generated
 *
 */

#ifndef Params_hh
#define Params_hh

#include <tdrp/tdrp.h>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <climits>
#include <cfloat>

using namespace std;

// Class definition

class Params {

public:

  // enum typedefs

  typedef enum {
    DEBUG_OFF = 0,
    DEBUG_WARNINGS = 1,
    DEBUG_NORM = 2,
    DEBUG_VERBOSE = 3
  } debug_t;

  typedef enum {
    ARCHIVE = 0,
    REALTIME = 1
  } mode_t;

  typedef enum {
    U_FIELD = 0,
    V_FIELD = 1,
    TK_FIELD = 2,
    Q_FIELD = 3,
    CLW_FIELD = 4,
    RNW_FIELD = 5,
    ICE_FIELD = 6,
    SNOW_FIELD = 7,
    GRAUPEL_FIELD = 8,
    NCI_FIELD = 9,
    RAD_TEND_FIELD = 10,
    W_FIELD = 11,
    P_FIELD = 12,
    U_CORNER_FIELD = 13,
    V_CORNER_FIELD = 14,
    TC_FIELD = 15,
    WSPD_FIELD = 16,
    WDIR_FIELD = 17,
    Z_FIELD = 18,
    DIVERGENCE_FIELD = 19,
    PRESSURE_FIELD = 20,
    RH_FIELD = 21,
    DEWPT_FIELD = 22,
    TURB_FIELD = 23,
    ICING_FIELD = 24,
    CLW_G_FIELD = 25,
    RNW_G_FIELD = 26,
    Q_G_FIELD = 27,
    THETA_FIELD = 28,
    THETAE_FIELD = 29,
    THETAV_FIELD = 30,
    DBZ_3D_FIELD = 31,
    TOT_CLD_CON_FIELD = 32,
    BROWN1_FIELD = 33,
    BROWN2_FIELD = 34,
    CCAT_FIELD = 35,
    COLSON_PANOFSKY_FIELD = 36,
    DEF_SQR_FIELD = 37,
    ELLROD1_FIELD = 38,
    ELLROD2_FIELD = 39,
    DUTTON_FIELD = 40,
    ENDLICH_FIELD = 41,
    HSHEAR_FIELD = 42,
    LAZ_FIELD = 43,
    PVORT_FIELD = 44,
    PVORT_GRADIENT_FIELD = 45,
    NGM1_FIELD = 46,
    NGM2_FIELD = 47,
    RICHARDSON_FIELD = 48,
    RIT_FIELD = 49,
    SAT_RI_FIELD = 50,
    STABILITY_FIELD = 51,
    VORT_SQR_FIELD = 52,
    VWSHEAR_FIELD = 53,
    TKE_KH3_FIELD = 54,
    TKE_KH4_FIELD = 55,
    TKE_KH5_FIELD = 56,
    START_2D_FIELDS = 57,
    GROUND_T_FIELD = 58,
    RAIN_CON_FIELD = 59,
    RAIN_NON_FIELD = 60,
    TERRAIN_FIELD = 61,
    CORIOLIS_FIELD = 62,
    RES_TEMP_FIELD = 63,
    LAND_USE_FIELD = 64,
    SNOWCOVR_FIELD = 65,
    TSEASFC_FIELD = 66,
    PBL_HGT_FIELD = 67,
    REGIME_FIELD = 68,
    SHFLUX_FIELD = 69,
    LHFLUX_FIELD = 70,
    UST_FIELD = 71,
    SWDOWN_FIELD = 72,
    LWDOWN_FIELD = 73,
    SOIL_T_1_FIELD = 74,
    SOIL_T_2_FIELD = 75,
    SOIL_T_3_FIELD = 76,
    SOIL_T_4_FIELD = 77,
    SOIL_T_5_FIELD = 78,
    SOIL_T_6_FIELD = 79,
    SOIL_M_1_FIELD = 80,
    SOIL_M_2_FIELD = 81,
    SOIL_M_3_FIELD = 82,
    SOIL_M_4_FIELD = 83,
    T2_FIELD = 84,
    Q2_FIELD = 85,
    U10_FIELD = 86,
    V10_FIELD = 87,
    WEASD_FIELD = 88,
    SNOWH_FIELD = 89,
    LON_FIELD = 90,
    LAT_FIELD = 91,
    HOURLY_CONV_RAIN_FIELD = 92,
    HOURLY_NONC_RAIN_FIELD = 93,
    FZLEVEL_FIELD = 94,
    RAIN_TOTAL_FIELD = 95,
    HOURLY_RAIN_TOTAL_FIELD = 96,
    RH2_FIELD = 97,
    DEWPT2_FIELD = 98,
    WSPD10_FIELD = 99,
    WDIR10_FIELD = 100,
    THETA2_FIELD = 101,
    THETAE2_FIELD = 102,
    THETAV2_FIELD = 103,
    MSLP2_FIELD = 104,
    T2C_FIELD = 105,
    Q2_G_FIELD = 106,
    DBZ_2D_FIELD = 107,
    CLOUD_FRACT_FIELD = 108,
    TWP_FIELD = 109,
    RWP_FIELD = 110,
    TOT_CLD_CONP_FIELD = 111,
    CLWP_FIELD = 112
  } output_field_name_t;

  typedef enum {
    OUT_INT8 = 1,
    OUT_INT16 = 2,
    OUT_FLOAT32 = 5
  } output_encoding_t;

  typedef enum {
    OUTPUT_PROJ_FLAT = 0,
    OUTPUT_PROJ_LATLON = 1,
    OUTPUT_PROJ_LAMBERT = 2,
    OUTPUT_PROJ_STEREOGRAPHIC = 3,
    OUTPUT_PROJ_MERCATOR = 4,
    OUTPUT_PROJ_NATIVE = 5
  } output_projection_t;

  typedef enum {
    FLIGHT_LEVELS = 0,
    PRESSURE_LEVELS = 1,
    HEIGHT_LEVELS = 2,
    NATIVE_SIGMA_LEVELS = 3
  } output_levels_t;

  typedef enum {
    _MDV_COMPRESSION_NONE = 0,
    _MDV_COMPRESSION_RLE = 1,
    _MDV_COMPRESSION_LZO = 2,
    _MDV_COMPRESSION_ZLIB = 3,
    _MDV_COMPRESSION_BZIP = 4
  } compression_t;

  typedef enum {
    SCALING_ROUNDED = 0,
    SCALING_DYNAMIC = 1
  } scaling_t;

  typedef enum {
    DATA_MEASURED = 0,
    DATA_EXTRAPOLATED = 1,
    DATA_FORECAST = 2,
    DATA_SYNTHESIS = 3,
    DATA_MIXED = 4,
    DATA_IMAGE = 5,
    DATA_GRAPHIC = 6,
    DATA_CLIMO_ANA = 7,
    DATA_CLIMO_OBS = 8
  } data_collection_type_t;

  typedef enum {
    LESS_THAN = 0,
    GREATER_THAN = 1,
    INSIDE_INTERVAL = 2,
    OUTSIDE_INTERVAL = 3
  } itfa_index_sense_t;

  typedef enum {
    BROWN1 = 0,
    BROWN2 = 1,
    CCAT = 2,
    COLSON_PANOFSKY = 3,
    DEF_SQR = 4,
    ELLROD1 = 5,
    ELLROD2 = 6,
    DUTTON = 7,
    ENDLICH = 8,
    HSHEAR = 9,
    LAZ = 10,
    PVORT = 11,
    PVORT_GRADIENT = 12,
    NGM1 = 13,
    NGM2 = 14,
    RICHARDSON = 15,
    RIT = 16,
    SAT_RI = 17,
    STABILITY = 18,
    VORT_SQR = 19,
    VWSHEAR = 20,
    TKE_GWB = 21,
    TKE_KH3 = 22,
    TKE_KH4 = 23,
    TKE_KH5 = 24
  } itfa_derived_index_name;

  typedef enum {
    W_ITFA = 0,
    WSPD_ITFA = 1,
    DIVERGENCE_ITFA = 2
  } itfa_model_index_name;

  // struct typedefs

  typedef struct {
    tdrp_bool_t round_lead_times;
    int lead_time_resolution_seconds;
  } round_lead_times_t;

  typedef struct {
    output_field_name_t name;
    output_encoding_t encoding;
  } output_field_t;

  typedef struct {
    double lat;
    double lon;
  } output_origin_t;

  typedef struct {
    int nx;
    int ny;
    double minx;
    double miny;
    double dx;
    double dy;
  } output_grid_t;

  typedef struct {
    itfa_derived_index_name name;
    float weight;
    itfa_index_sense_t sense;
    float threshold_1;
    float threshold_2;
    int min_flight_level;
    int max_flight_level;
  } itfa_derived_index_t;

  typedef struct {
    itfa_model_index_name name;
    float weight;
    itfa_index_sense_t sense;
    float threshold_1;
    float threshold_2;
    int min_flight_level;
    int max_flight_level;
  } itfa_model_index_t;

  ///////////////////////////
  // Member functions
  //

  ////////////////////////////////////////////
  // Default constructor
  //

  Params ();

  ////////////////////////////////////////////
  // Copy constructor
  //

  Params (const Params&);

  ////////////////////////////////////////////
  // Destructor
  //

  virtual ~Params ();

  ////////////////////////////////////////////
  // Assignment
  //

  void operator=(const Params&);

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

  int loadFromArgs(int argc, char **argv,
                   char **override_list,
                   char **params_path_p,
                   bool defer_exit = false);

  bool exitDeferred() { return (_exitDeferred); }

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

  int loadApplyArgs(const char *params_path,
                    int argc, char **argv,
                    char **override_list,
                    bool defer_exit = false);

  ////////////////////////////////////////////
  // isArgValid()
  // 
  // Check if a command line arg is a valid TDRP arg.
  //

  static bool isArgValid(const char *arg);

  ////////////////////////////////////////////
  // isArgValid()
  // 
  // Check if a command line arg is a valid TDRP arg.
  // return number of args consumed.
  //

  static int isArgValidN(const char *arg);

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

  int load(const char *param_file_path,
           char **override_list,
           int expand_env, int debug);

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

  int loadFromBuf(const char *param_source_str,
                  char **override_list,
                  const char *inbuf, int inlen,
                  int start_line_num,
                  int expand_env, int debug);

  ////////////////////////////////////////////
  // loadDefaults()
  //
  // Loads up default params for a given class.
  //
  // See load() for more detailed info.
  //
  //  Returns 0 on success, -1 on failure.
  //

  int loadDefaults(int expand_env);

  ////////////////////////////////////////////
  // sync()
  //
  // Syncs the user struct data back into the parameter table,
  // in preparation for printing.
  //
  // This function alters the table in a consistent manner.
  // Therefore it can be regarded as const.
  //

  void sync() const;

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

  void print(FILE *out, tdrp_print_mode_t mode = PRINT_NORM);

  ////////////////////////////////////////////
  // checkAllSet()
  //
  // Return TRUE if all set, FALSE if not.
  //
  // If out is non-NULL, prints out warning messages for those
  // parameters which are not set.
  //

  int checkAllSet(FILE *out);

  //////////////////////////////////////////////////////////////
  // checkIsSet()
  //
  // Return TRUE if parameter is set, FALSE if not.
  //
  //

  int checkIsSet(const char *param_name);

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

  int arrayRealloc(const char *param_name,
                   int new_array_n);

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

  int array2DRealloc(const char *param_name,
                     int new_array_n1,
                     int new_array_n2);

  ////////////////////////////////////////////
  // freeAll()
  //
  // Frees up all TDRP dynamic memory.
  //

  void freeAll(void);

  ////////////////////////////////////////////
  // usage()
  //
  // Prints out usage message for TDRP args as passed
  // in to loadFromArgs().
  //

  static void usage(ostream &out);

  ///////////////////////////
  // Data Members
  //

  char _start_; // start of data region
                // needed for zeroing out data
                // and computing offsets

  debug_t debug;

  tdrp_bool_t dbzConstantIntercepts;

  char* instance;

  mode_t mode;

  char* realtime_input_dir;

  char* DomainString;

  tdrp_bool_t use_ldata;

  int max_realtime_valid_age;

  int min_forecast_dtime;

  tdrp_bool_t specify_lead_times;

  int *_lead_times;
  int lead_times_n;

  round_lead_times_t round_leads;

  char* output_url;

  tdrp_bool_t use_filename_for_gen_time;

  tdrp_bool_t use_parent_path_for_gen_time;

  output_field_t *_output_fields;
  int output_fields_n;

  output_field_name_t *_output_fields_as_index;
  int output_fields_as_index_n;

  tdrp_bool_t wind_speed_in_knots;

  output_projection_t output_projection;

  output_origin_t output_origin;

  double lambert_lat1;

  double lambert_lat2;

  double stereographic_tangent_lat;

  double stereographic_tangent_lon;

  double stereographic_central_scale;

  output_grid_t output_grid;

  output_levels_t output_levels;

  int *_flight_levels;
  int flight_levels_n;

  double *_pressure_levels;
  int pressure_levels_n;

  double *_height_levels;
  int height_levels_n;

  scaling_t output_scaling;

  tdrp_bool_t copy_lowest_downwards;

  compression_t output_compression;

  tdrp_bool_t output_path_in_forecast_format;

  data_collection_type_t data_collection_type;

  char* data_set_info;

  char* data_set_name;

  char* data_set_source;

  itfa_derived_index_t *_itfa_derived_indices;
  int itfa_derived_indices_n;

  itfa_model_index_t *_itfa_model_indices;
  int itfa_model_indices_n;

  float min_turb_severity_threshold;

  tdrp_bool_t itfa_fill_edges;

  float trace_icing_clw;

  float light_icing_clw;

  float moderate_icing_clw;

  float severe_icing_clw;

  float clear_ice_temp;

  char _end_; // end of data region
              // needed for zeroing out data

private:

  void _init();

  mutable TDRPtable _table[57];

  const char *_className;

  bool _exitDeferred;

};

#endif

