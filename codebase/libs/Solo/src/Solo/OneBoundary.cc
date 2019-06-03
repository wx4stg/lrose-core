
#include "OneBoundary.hh"



OneBoundary::OneBoundary() {}

OneBoundary::~OneBoundary() {}

void OneBoundary::AddBoundaryPoint(SoloClickInfo *sci) {
  /* add a point to the boundary based on the x & y coordinates                                             
   * in the click struct                                                                                    
   */
  BoundaryStuff *sebs, *return_se_bnd_struct();
  struct point_in_space *pisp, *solo_return_radar_pisp();
  struct solo_edit_stuff *seds, *return_sed_stuff();

  BoundaryPointManagement *bpm, *se_pop_bpms();
  double d, sqrt(), fabs();
  double x = sci->x, y = sci->y;

  seds = return_sed_stuff();
  sebs = return_se_bnd_struct();
  bpm = se_pop_bpms();        /* nab a fresh boundary point */
  se_append_bpm(&top_bpm, bpm); /* the append happens first so                                          
                                     * the last pointers are fresh */
  bpm->x = sci->x;
  bpm->y = sci->y;

  if(++num_points > 1) {  /* avoid duplicates */
    if((bpm->x == bpm->last->x) && (bpm->y == bpm->last->y)) {
      num_points--;
      se_delete_bnd_pt(bpm);
      return;
    }
  }
  bpm->which_frame = sci->frame;

  if(!(sebs->view_bounds || sebs->absorbing_boundary)) {
    PointInSpace::LocateThisPoint(sci, bpm); /* this routine is in                                                 
                                     * ../perusal/sp_clkd.c */
    // PointInSpace structure that is filled resides inside bpm
    // to access it, use bpm->pisp
  }
  else if(sebs->absorbing_boundary) {
    memcpy(bpm->pisp, sebs->pisp, sizeof(struct point_in_space));
    bpm->r = KM_TO_M(bpm->pisp->range);
  }
  // state can be PISP_TIME_SERIES, or PISP_AZELRG or PISP_EARTH
  if(bpm->pisp->state & PISP_TIME_SERIES) {
    x = bpm->pisp->time;
    y = bpm->pisp->range;
  }
  /* the rasterization code sets sebs->view_bounds to YES and also uses                                     
   * this routine and others to bound the rasterization                                                     
   */
  if(num_points > 1) {
    if(!sebs->view_bounds) {
      se_draw_bnd(bpm, 2, NO);
      seds->boundary_exists = YES;
      sii_set_boundary_pt_count (sci->frame, num_points);
    }
    if(x > max_x)
      max_x = x;
    if(x < min_x)
      min_x = x;
    if(y > max_y)
      max_y = y;
    if(y < min_y)
      min_y = y;

    se_bnd_pt_atts(bpm);
    xse_x_insert(bpm, ob);
    xse_y_insert(bpm, ob);

    /*                                                                                                    
     * now do it for the line between this point and the first point                                      
     */
    bpm = top_bpm;
    se_bnd_pt_atts(bpm);
  }
  else {                      /* first point */
    if(!(sebs->absorbing_boundary || sebs->view_bounds)) {
      /* get the radar origin and radar name from the first point                                       
       */
      solo_return_radar_name(sci->frame, bh->radar_name);
      memcpy(sebs->origin, bpm->pisp, sizeof(struct point_in_space));
      strcpy(sebs->origin->id, "BND_ORIGIN");
    }
    min_x = max_x = x;
    min_y = max_y = y;
  }

}

int OneBoundary::FindIntersections() {
  // int xse_find_intxns(angle, range, ob)
  // double angle, range;
  //struct one_boundary *ob;
  // {
    /* this routine creates a list of endpoints of lines that                                                 
     * intersect the current ray                                                                              
     */
    struct bnd_point_mgmt *bpm, *bpmx;
    double theta = RADIANS(CART_ANGLE(angle)), cos(), sin();
    double slope=0, xx, yy;
    long x, y;
    int ii, mm, nn, nx=0, mark;


    ob->num_intxns = 0;
    ob->first_intxn = NULL;
    /*                                                                                                        
     * compute the endpoints of the ray                                                                       
     */
    xx = range * cos(theta);
    yy = range * sin(theta);
    x = xx < 0 ? xx -.5 : xx +.5;
    y = yy < 0 ? yy -.5 : yy +.5;
# ifdef OLD_XNS
    if(x) slope = yy/xx;
# else
    if(xx) slope = yy/xx;
# endif

    bpm = ob->top_bpm;          /* find the first point that is not                                           
                                 * on the line */
    /*                                                                                                        
     * when we are doing intersections, we want to use the shifted                                            
     * x,y values because the boundary may be for another radar                                               
     */
    for(bpmx=NULL; bpm; bpm = bpm->next) {
# ifdef OLD_XNS
      bpm->which_side = se_ccw(bpm->_x, bpm->_y, x, y);
# else
      bpm->which_side = xse_ccw((double)bpm->_x, (double)bpm->_y
                                , xx, yy);
# endif
      if(bpm->which_side) {
        bpmx = bpm;
        break;
      }
    }
    if(!bpmx)
      return(0);            /* no intersections! */
    mm = ob->num_points;
    /*                                                                                                        
     * since "->last" links all points we loop through all the                                                
     * points using the last pointer                                                                          
     */
    for(; mm--; bpm = bpm->last) {
      /*                                                                                                    
       * a return of 0 from se_ccw says this point is                                                       
       * colinear witht the ray. So we do nothing.                                                          
       */
# ifdef OLD_XNS
      bpm->last->which_side = se_ccw(bpm->last->_x, bpm->last->_y
                                     , x, y);
# else
        bpm->last->which_side = xse_ccw
          ((double)bpm->last->_x, (double)bpm->last->_y, xx, yy);
# endif
        if(bpm->last->which_side) {
          if(bpm->last->which_side != bpmx->which_side) {
            /*                                                                                            
             * we may have an intersection between "bpm"                                                  
             * and "bpm->last". See if it's real.                                                         
             */
# ifdef OLD_XNS
            xse_set_intxn(x, y, slope, bpm, ob);
# else
            xse_set_intxn(xx, yy, slope, bpm, ob);
# endif
          }
          bpmx = bpm->last;
        }
    }
# ifdef obsolete
    xse_num_segments(ob);
# endif
    return(ob->num_intxns);
}



void OneBoundary::TimeSeriesFindIntersections() {
  //  void se_ts_find_intxns(radar_alt, d_max_range, ob, d_time, d_pointing
  //                       , automatic, down, d_ctr)
  //  double radar_alt, d_max_range, d_time, d_pointing, d_ctr;
  // struct one_boundary *ob;
  // int down, automatic;
  
    int mm;
    struct bnd_point_mgmt *bpm, *bpmx, *bpma, *bpmb;
    double d, time_span, ta, tb, xx, zz;



    num_intxns = 0;
    first_intxn = NULL;
    mm = num_points;
    bpm = top_bpm;
    d_time += .005;
    d = RADIANS(CART_ANGLE(d_pointing));
    /*                                                                                                         
     * we bump the time by 5 milliseconds so a ray and a vertex                                                
     * are not coincident in time                                                                              
     */
    if(automatic) {
      down = sin(d) < 0;
    }
    else {
      radar_alt = down ? 2. * d_ctr : 0;
    }

    for(; mm--; bpm = bpm->last) {
      if((bpm->last->pisp->time) < (bpm->pisp->time)) {
        bpma = bpm->last;
        bpmb = bpm;
      }
      else {
        bpma = bpm;
        bpmb = bpm->last;
      }
      ta = bpma->pisp->time;
      tb = bpmb->pisp->time;

      if(d_time >= ta && d_time < tb) {
        /* possible intxn */
        zz = ((d_time - ta)/(tb - ta)) * (bpmb->pisp->z - bpma->pisp->z)
          + bpma->pisp->z;
        if((down && zz < radar_alt) || (!down && zz > radar_alt)) {
          /* intxn! */
            bpm->rx = down
              ? KM_TO_M(radar_alt -zz) : KM_TO_M(zz - radar_alt);
            num_intxns++;
            bpm->next_intxn = NULL;
            se_merge_intxn(bpm, ob);
        }
      }
    }
    radar_inside_boundary = num_intxns & 1;
    /* i.e. an odd number of intxns implies radar inside */
    if(bh->force_inside_outside) {
      if(bh->force_inside_outside == BND_FIX_RADAR_INSIDE) {
        radar_inside_boundary = YES;
      }
      if(bh->force_inside_outside == BND_FIX_RADAR_OUTSIDE) {
        radar_inside_boundary = NO;
      }
    }
}

