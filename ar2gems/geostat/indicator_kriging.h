/* -----------------------------------------------------------------------------
** Copyright (c) 2012 Advanced Resources and Risk Technology, LLC
** All rights reserved.
**
** This file is part of Advanced Resources and Risk Technology, LLC (AR2TECH) 
** version of the open source software sgems.  It is a derivative work by 
** AR2TECH (THE LICENSOR) based on the x-free license granted in the original 
** version of the software (see notice below) and now sublicensed such that it 
** cannot be distributed or modified without the explicit and written permission 
** of AR2TECH.
**
** Only AR2TECH can modify, alter or revoke the licensing terms for this 
** file/software.
**
** This file cannot be modified or distributed without the explicit and written 
** consent of AR2TECH.
**
** Contact Dr. Alex Boucher (aboucher@ar2tech.com) for any questions regarding
** the licensing of this file/software
**
** The open-source version of sgems can be downloaded at 
** sourceforge.net/projects/sgems.
** ----------------------------------------------------------------------------*/



/**********************************************************************
** Author: Nicolas Remy
** Copyright (C) 2002-2004 The Board of Trustees of the Leland Stanford Junior
**   University
** All rights reserved.
**
** This file is part of the "geostat" module of the Geostatistical Earth
** Modeling Software (GEMS)
**
** This file may be distributed and/or modified under the terms of the 
** license defined by the Stanford Center for Reservoir Forecasting and 
** appearing in the file LICENSE.XFREE included in the packaging of this file.
**
** This file may be distributed and/or modified under the terms of the
** GNU General Public License version 2 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
** See http://www.gnu.org/copyleft/gpl.html for GPL licensing information.
**
** Contact the Stanford Center for Reservoir Forecasting, Stanford University
** if any conditions of this licensing are not clear to you.
**
**********************************************************************/

#ifndef __GSTLAPPLI_GEOSTAT_INDICATOR_KRIGING_H__ 
#define __GSTLAPPLI_GEOSTAT_INDICATOR_KRIGING_H__ 
 
 
#include <geostat/common.h>
#include <geostat/geostat_algo.h> 
#include <grid/grid_model/geostat_grid.h> 
 
#include <GsTL/geometry/covariance.h> 
#include <GsTL/utils/smartptr.h> 

#include <grid/grid_model/grid_region_temp_selector.h> 

#include <string> 
#include <vector> 
 
class Progress_notifier;
class Neighborhood; 
class MultiRealization_property; 
template<class T> class Non_parametric_cdf; 
 
 
class GEOSTAT_DECL Indicator_kriging : public Geostat_algo { 
 public: 
  Indicator_kriging(); 
  virtual ~Indicator_kriging(); 
 
  virtual bool initialize( const Parameters_handler* parameters, 
			   Error_messages_handler* errors ); 
  virtual int execute( GsTL_project* proj=0 ); 
  virtual std::string name() const { return "indicator_kriging"; } 
   
 public: 
  static Named_interface* create_new_interface( std::string& ); 
 

 protected: 
  int median_ik( Progress_notifier* progress_notifier ); 
  int full_ik( Progress_notifier* progress_notifier ); 


 protected: 
  typedef Geostat_grid::location_type Location; 
 
  Geostat_grid* simul_grid_; 
  MultiRealization_property* multireal_property_; 
   
  Geostat_grid* hdata_grid_; 
  std::vector< Grid_continuous_property* > hdata_properties_; 
 
  Non_parametric_cdf<float>* ccdf_; 
  std::vector<double> marginal_probs_; 
 
  bool do_median_ik_; 
 
  // For median IK   
  Covariance<Location> covar_; 
  SmartPtr<Neighborhood> neighborhood_; 
  
  // For full IK 
  std::vector< Covariance<Location> > covar_vector_; 
  std::vector< SmartPtr<Neighborhood> > neighborhoods_vector_; 

  Temporary_gridRegion_Selector grid_region_;
  Temporary_gridRegion_Selector prim_hd_grid_region_;
 
  int thres_count_; 

  int min_neigh_;
  
}; 
 
#endif 
