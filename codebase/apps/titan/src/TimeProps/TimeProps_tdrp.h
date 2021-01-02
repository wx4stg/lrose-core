/*******************************************
 * TimeProps_tdrp.h
 *
 * TDRP header file for 'TimeProps' module.
 *
 * Code for program TimeProps
 *
 * This header file has been automatically
 * generated by TDRP, do not modify.
 *
 *******************************************/

#ifndef _TimeProps_tdrp_h
#define _TimeProps_tdrp_h

#ifdef __cplusplus
extern "C" {
#endif

#include <tdrp/tdrp.h>

/*
 * typedefs
 */

typedef enum {
  DEBUG_OFF = 0,
  DEBUG_NORM = 1,
  DEBUG_VERBOSE = 2
} TimeProps_debug;

typedef enum {
  DELTA_TIME_MODE = 0,
  ACTIVITY_MODE = 1
} TimeProps_distribution_class;

/*
 * typedef for main struct - TimeProps_tdrp_struct
 */

typedef struct {

  size_t struct_size;

  /***** debug *****/

  TimeProps_debug debug;

  /***** malloc_debug_level *****/

  long malloc_debug_level;

  /***** instance *****/

  char* instance;

  /***** mode *****/

  TimeProps_distribution_class mode;

  /***** activity_nstorms_min *****/

  long activity_nstorms_min;

  /***** activity_gap_max *****/

  double activity_gap_max;

  /***** activity_time_of_day_offet *****/

  long activity_time_of_day_offet;

  /***** time_label *****/

  char* time_label;

  /***** dur_label *****/

  char* dur_label;

  /***** condition_input_data *****/

  tdrp_bool_t condition_input_data;

  /***** conditional_label *****/

  char* conditional_label;

  /***** cond_min *****/

  double cond_min;

  /***** cond_max *****/

  double cond_max;

  /***** limit_time_data *****/

  tdrp_bool_t limit_time_data;

  /***** time_min *****/

  double time_min;

  /***** time_max *****/

  double time_max;

  /***** limit_dur_data *****/

  tdrp_bool_t limit_dur_data;

  /***** dur_min *****/

  double dur_min;

  /***** dur_max *****/

  double dur_max;

  /***** limit_dtime_data *****/

  tdrp_bool_t limit_dtime_data;

  /***** dtime_min *****/

  double dtime_min;

  /***** dtime_max *****/

  double dtime_max;

} TimeProps_tdrp_struct;

/*
 * function prototypes
 */

/*************************************************************
 * TimeProps_tdrp_load_from_args()
 *
 * Loads up TDRP using the command line args.
 *
 * Check TDRP_usage() for command line actions associated with
 * this function.
 *
 *   argc, argv: command line args
 *
 *   TimeProps_tdrp_struct *params: loads up this struct
 * 
 *   char **override_list: A null-terminated list of overrides
 *     to the parameter file.
 *     An override string has exactly the format of an entry
 *     in the parameter file itself.
 *
 *   char **params_path_p: if non-NULL, this is set to point to
 *                         the path of the params file used.
 *
 *  Returns 0 on success, -1 on failure.
 */

extern int TimeProps_tdrp_load_from_args(int argc, char **argv,
                                TimeProps_tdrp_struct *params,
                                char **override_list,
                                char **params_path_p);

/*************************************************************
 * TimeProps_tdrp_load()
 *
 * Loads up TDRP for a given module.
 *
 * This version of load gives the programmer the option to load
 * up more than one module for a single application. It is a
 * lower-level routine than TimeProps_tdrp_load_from_args,
 * and hence more flexible, but the programmer must do more work.
 *
 *   char *param_file_path: the parameter file to be read in.
 *
 *   TimeProps_tdrp_struct *params: loads up this struct
 *
 *   char **override_list: A null-terminated list of overrides
 *     to the parameter file.
 *     An override string has exactly the format of an entry
 *     in the parameter file itself.
 *
 *  expand_env: flag to control environment variable
 *                expansion during tokenization.
 *              If TRUE, environment expansion is set on.
 *              If FALSE, environment expansion is set off.
 *
 *  Returns 0 on success, -1 on failure.
 */

extern int TimeProps_tdrp_load(char *param_file_path,
                      TimeProps_tdrp_struct *params,
                      char **override_list,
                      int expand_env, int debug);

/*************************************************************
 * TimeProps_tdrp_load_defaults()
 *
 * Loads up defaults for a given module.
 *
 * See TimeProps_tdrp_load() for more details.
 *
 *  Returns 0 on success, -1 on failure.
 */

extern int TimeProps_tdrp_load_defaults(TimeProps_tdrp_struct *params,
                               int expand_env);

/*************************************************************
 * TimeProps_tdrp_sync()
 *
 * Syncs the user struct data back into the parameter table,
 * in preparation for printing.
 */

extern void TimeProps_tdrp_sync(void);

/*************************************************************
 * TimeProps_tdrp_print()
 * 
 * Print params file
 *
 * The modes supported are:
 *
 *   PRINT_SHORT:   main comments only, no help or descriptions
 *                  structs and arrays on a single line
 *   PRINT_NORM:    short + descriptions and help
 *   PRINT_LONG:    norm  + arrays and structs expanded
 *   PRINT_VERBOSE: long  + private params included
 */

extern void TimeProps_tdrp_print(FILE *out, tdrp_print_mode_t mode);

/*************************************************************
 * TimeProps_tdrp_free_all()
 *
 * Frees up all TDRP dynamic memory.
 */

extern void TimeProps_tdrp_free_all(void);

/*************************************************************
 * TimeProps_tdrp_check_all_set()
 *
 * Return TRUE if all set, FALSE if not.
 *
 * If out is non-NULL, prints out warning messages for those
 * parameters which are not set.
 */

extern int TimeProps_tdrp_check_all_set(FILE *out);

/*************************************************************
 * TimeProps_tdrp_check_is_set()
 *
 * Return TRUE if parameter is set, FALSE if not.
 *
 */

extern int TimeProps_tdrp_check_is_set(char *param_name);

/*************************************************************
 * TimeProps_tdrp_array_realloc()
 *
 * Realloc 1D array.
 *
 * If size is increased, the values from the last array entry is
 * copied into the new space.
 *
 * Returns 0 on success, -1 on error.
 */

extern int TimeProps_tdrp_array_realloc(char *param_name,
                              int new_array_n);

/*************************************************************
 * TimeProps_tdrp_array2D_realloc()
 *
 * Realloc 2D array.
 *
 * If size is increased, the values from the last array entry is
 * copied into the new space.
 *
 * Returns 0 on success, -1 on error.
 */

extern int TimeProps_tdrp_array2D_realloc(char *param_name,
                                 int new_array_n1,
                                 int new_array_n2);

/*************************************************************
 * TimeProps_tdrp_table()
 *
 * Returns pointer to static Table for this module.
 */

extern TDRPtable *TimeProps_tdrp_table(void);

/*************************************************************
 * TimeProps_tdrp_init()
 *
 * Module table initialization function.
 *
 *
 * Returns pointer to static Table for this module.
 */

extern TDRPtable *TimeProps_tdrp_init(TimeProps_tdrp_struct *params);

#ifdef __cplusplus
}
#endif

#endif
