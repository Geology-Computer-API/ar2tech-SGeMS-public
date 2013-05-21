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

#ifndef __GSTLAPPLI_GEOSTAT_UTILITIES_H__ 
#define __GSTLAPPLI_GEOSTAT_UTILITIES_H__ 
 
#include <GsTLAppli/geostat/common.h>
#include <GsTLAppli/grid/grid_model/geostat_grid.h> 
#include <GsTLAppli/math/gstlpoint.h> 
#include <GsTLAppli/math/non_parametric_distribution.h> 
#include <GsTLAppli/math/continuous_distribution.h>
#include <GsTLAppli/grid/grid_model/neighborhood.h> 
#include <GsTLAppli/utils/progress_notifier.h>
 
#include <GsTL/kriging/kriging_constraints.h> 
#include <GsTL/kriging/cokriging_constraints.h> 
#include <GsTL/kriging/kriging_combiner.h> 
#include <GsTL/kriging/cokriging_combiner.h> 
#include <GsTL/kriging/covariance_set.h>
#include <GsTL/cdf/gaussian_cdf.h>
#include <GsTL/cdf/non_param_cdf.h> 


#include <string> 
#include <vector> 
#include <map>

class Parameters_handler; 
class Error_messages_handler; 
template<class T> class Covariance; 
 
 

namespace geostat_utils {
 
  typedef Geostat_grid::location_type Location; 
  typedef Location::difference_type   EuclideanVector;

  typedef Covariance_set< Covariance<Location> > CovarianceSet; 

  typedef GsTL_neighborhood< SmartPtr<Neighborhood> >       NeighborhoodHandle;
  typedef std::vector< NeighborhoodHandle >                 NeighborhoodVector; 
  typedef std::vector< NeighborhoodHandle >::const_iterator NeighIterator; 
 
  typedef std::vector< double >::const_iterator               WeightIterator; 
  typedef Kriging_combiner< WeightIterator, Neighborhood >    KrigingCombiner; 
  typedef Kriging_constraints< Neighborhood, Geovalue >       KrigingConstraints;
  typedef CoKriging_combiner< WeightIterator, NeighIterator > CoKrigingCombiner;   
  typedef CoKriging_constraints< NeighIterator, Geovalue >    CoKrigingConstraints;
 
  typedef Non_param_cdf<> GsTLNonParametricCdfType;
  typedef Non_param_cdf<> NonParametricCdfType;
 
  enum Cokriging_type { UNDEF=0, FULL, MM1, MM2 }; 
  enum Kriging_type { ERROR, SK, OK, KT, LVM }; 
 
  typedef std::map< geostat_utils::Kriging_type, std::string > KrigTagMap;
  typedef std::map< geostat_utils::Cokriging_type, std::string > CokrigTagMap;
  typedef std::map< geostat_utils::Kriging_type, std::string > KrigDefaultsMap;
  typedef std::map< geostat_utils::Cokriging_type, std::string > CokrigDefaultsMap;

  // typedef for kriging with a local mean
  //	  typedef std::map<GeostatGrid*, Colocated_neighborhood> coloc_neighs;	


  /** Extract the parameters defining the covariance from the set of parameters 
   * and initialize the covariance 
   * @return false if an error occurred 
   * 
   * @param cov will be modified to contain the covariance specified by  
   * \a tag_name 
   * @param tag_name is the name of the parameter containing the covariance 
   * information. 
   * @param parameters is the parameter handler from which \a tag_name will 
   * be retrieved 
   * @param errors is where possible errors are reported 
   */ 
  GEOSTAT_DECL bool 
    initialize_covariance( Covariance<Location>* cov, 
                           const std::string& tag_name, 
			                     const Parameters_handler* parameters, 
			                     Error_messages_handler* errors ); 
 
 

  /** Initializes a kriging combiner and a set of kriging constraints from the  
   * parameters. 
   * \c ktype is the kriging type (SK, OK, KT or LVM). \c Kcombiner and 
   * \c Kconstraints will be made to the newly initialized kriging combiner
   * and constraints. \c tags_map is a map containing the names of the
   * parameters for each type of kriging. Finally, \c simulation_grid is
   * the grid on which the simulation (or estimation) is performed. This
   * parameter is actually only needed in the case of LVM (the property
   * containing the local mean must be defined on \c simulation_grid).
   */ 
   GEOSTAT_DECL bool initialize( Kriging_type ktype,
                                 KrigingCombiner*& Kcombiner,
                                 KrigingConstraints*& Kconstraints,
                                 KrigTagMap& tags_map,
                                 const Parameters_handler* parameters,
                                 Error_messages_handler* errors,
                                 Geostat_grid* simulation_grid = 0,
                                 KrigDefaultsMap defaults = KrigDefaultsMap() ); 

   
  /** Initializes a kriging combiner and a set of kriging constraints from the  
   * parameters. 
   * \c ktype is the kriging type (SK, OK, KT or LVM). \c Kcombiner and 
   * \c Kconstraints will be made to the newly initialized kriging combiner
   * and constraints. \c tags_map is a map containing the names of the
   * parameters for each type of kriging. Finally, \c kriging_grid and \c hard_data_grid 
   * is the grid on which the simulation (or estimation) is performed and the grid containing the hard data. This
   * parameter is actually only needed in the case of LVM (the property
   * containing the local mean must be defined on \c simulation_grid).
 */
   GEOSTAT_DECL bool initialize_kriging_system( std::string kriging_type_base_name,
                                 KrigingCombiner*& Kcombiner,
                                 KrigingConstraints*& Kconstraints,
                                 const Parameters_handler* parameters,
                                 Error_messages_handler* errors,
                                 Geostat_grid* kriging_grid = 0,
                                 Geostat_grid* hard_data_grid = 0,
                                 KrigDefaultsMap defaults = KrigDefaultsMap());
   

  /** Initializes a cokriging combiner and a set of kriging constraints from the  
   * parameters. 
   * @return false if errors occurred. 
   * 
   * @param Kcombiner will be changed to point to the initialized cokriging 
   * combiner. 
   * @param Kconstraints will be changed to point to the initialized kriging 
   * constraints, 
   * @param type is the type of kriging (only SK and OK are considered) 
   * @param tag_name is the name of the parameter containing the information 
   * needed to initialize the kriging constraints (e.g. the simple kriging 
   * means, in case \a type is SK ) 
   */ 
  GEOSTAT_DECL bool initialize( CoKrigingCombiner*& Kcombiner, 
			  CoKrigingConstraints*& Kconstraints, 
			  Kriging_type type, 
			  const std::string& tag_name, 
			  const Parameters_handler* parameters, 
			  Error_messages_handler* errors,
        const std::string& defaults = ""); 
				   

  /** Don't forget to delete the returned covariance set. Could return an
  * auto_ptr<CovarianceSet> to make it explicit that it is the responsability
  * of the user to delete the returned pointer.
  * FULL: tag for C12(h) and C22(h)
  * MM1: tag for C12(0), and C22(0)
  * MM2: tag for C12(0) and C22(h)
  */
  GEOSTAT_DECL CovarianceSet* 
    init_covariance_set( Cokriging_type type, 
                         const Covariance<Location>& C11,
                         CokrigTagMap& tags_map,
                         const Parameters_handler* parameters, 
                         Error_messages_handler* errors,
                         CokrigDefaultsMap defaults = CokrigDefaultsMap() );
  

 
  GEOSTAT_DECL NeighborhoodHandle
    init_secondary_neighborhood( Cokriging_type type, 
                                 Geostat_grid* hdata_grid,
                                 const Grid_continuous_property* secondary_prop,
                                 const Parameters_handler* parameters = 0, 
                                 Error_messages_handler* errors = 0,
                                 const std::string& max_size_tag = "",
                                 const std::string& ellipsoid_tag = "",
                                 const std::string& C22_tag = "",
                                 const Grid_region* region=0);
  


  /** Create a geostat grid called \a grid_name and report potential errors 
   * to the error messages handler.  
   * @param grid will be modified to point to the newly created grid 
   * @param tag_name is used only if an error needs to be reported. In that 
   * case, the error will show that parameter \a tag_name was wrong 
   * @param errors is where possible errors are reported 
   */ 
  GEOSTAT_DECL bool create( Geostat_grid*& grid, 
		      const std::string& grid_name, 
		      const std::string& tag_name, 
		      Error_messages_handler* errors ); 
	 
				     
  /** Extracts the definition (ie ranges and angles) of an ellipsoid from the 
   * set of parameters and reports any error. 
   * @return false if an error occurred. 
   * 
   * @param ranges is modified to contain the extracted ranges 
   * @param angles is modified to contain the extracted angles 
   * @param tag_name is the name of the parameter containing the range 
   * and angle information. 
   * @param parameters is the parameter handler from which \a tag_name will 
   * be retrieved 
   * @param errors is where possible errors are reported 
   */ 
  GEOSTAT_DECL bool extract_ellipsoid_definition( GsTLTriplet& ranges, 
					    GsTLTriplet& angles,  
					    const std::string& tag_name, 
					    const Parameters_handler* parameters, 
					    Error_messages_handler* errors ); 
 
 
  /** Extracts the definition (ie ranges and angles) of an ellipsoid from the 
   * set of parameters and reports any error. Returns false if an error occurred. 
   * This is an overloaded function provided for convenience  
   */ 
  GEOSTAT_DECL bool extract_ellipsoid_definition( float* ranges, float* angles, 
					    const std::string& tag_name, 
					    const Parameters_handler* parameters, 
					    Error_messages_handler* errors ); 
 
 
  /** Retrieves the cokriging type (e.g. full cokriging, MM1 or MM2 ) specified 
   * by parameter \a tag_name.  
   * @return the cokriging type, or \c Cokriging_type::undef if an error occurred 
   * 
   * @param tag_name is the name of the parameter that contains the cokriging 
   * type information 
   * @param parameters is the parameter handler from which \a tag_name will 
   * be retrieved 
   * @param errors is where possible errors are reported 
   */ 
  GEOSTAT_DECL Cokriging_type cokriging_type( const std::string& tag_name, 
					const Parameters_handler* parameters, 
					Error_messages_handler* errors ); 
 
 
 
  /** Retrieves the kriging type (e.g. SK, OK, KT or LVM ) specified 
   * by parameter \a tag_name.  
   * @return the kriging type, or \c Kriging_type::undef if an error occurred 
   * 
   * @param tag_name is the name of the parameter that contains the kriging 
   * type information 
   * @param parameters is the parameter handler from which \a tag_name will 
   * be retrieved 
   * @param errors is where possible errors are reported 
   */ 
  GEOSTAT_DECL Kriging_type kriging_type( const std::string& tag_name, 
				    const Parameters_handler* parameters, 
				    Error_messages_handler* errors ); 
 


  /** This function adds a new property to Geostat_grid \c grid. Contrary to 
  * the \c add_property member function of Geostat_grid, \c add_property_to_grid
  * ensures that a new property is added to the grid: if \c grid already has
  * a property called \c prop_name, the name of the new property will be 
  * modified so that it is unique (while the \c add_property member function 
  * of Geostat_grid would return null pointer). 
  * The returned pointer is ensured to be non-null. The actual name of the new
  * function can be accessed from the returned property:
  * \code
  * Grid_continuous_property* new_prop = add_property_to_grid( grid, name );
  * std::string actual_name = new_prop->name();
  * \endcode
  */
  GEOSTAT_DECL Grid_continuous_property* 
    add_property_to_grid( Geostat_grid* grid, 
                          const std::string& prop_name );




  /** This function adds a new categorical property to Geostat_grid \c grid. Contrary to 
  * the \c add_property member function of Geostat_grid, \c add_categorical_property_to_grid
  * ensures that a new property is added to the grid: if \c grid already has
  * a property called \c prop_name, the name of the new property will be 
  * modified so that it is unique (while the \c add_property member function 
  * of Geostat_grid would return null pointer). 
  * The returned pointer is ensured to be non-null. The actual name of the new
  * function can be accessed from the returned property:
  * \code
  * Grid_continuous_property* new_prop = add_property_to_grid( grid, name );
  * std::string actual_name = new_prop->name();
  * \endcode
  */
  GEOSTAT_DECL Grid_categorical_property* 
    add_categorical_property_to_grid( Geostat_grid* grid, 
                          const std::string& prop_name, std::string cdef_name="Default" );

  /** This function adds a new group to Geostat_grid \c grid. Contrary to 
  * the \c add_group member function of Geostat_grid, \c add_group_to_grid
  * ensures that a new group is added to the grid: if \c grid already has
  * a group called \c group_name, the name of the new group will be 
  * modified so that it is unique (while the \c add_group member function 
  * of Geostat_grid would return null pointer). 
  * The returned pointer is ensured to be non-null. The actual name of the new
  * function can be accessed from the returned group:
  * \code
  * GsTLGridPropertyGroup* new_group = add_property_to_grid( grid, name );
  * std::string actual_name = new_group->name();
  * \endcode
  */
  GEOSTAT_DECL GsTLGridPropertyGroup* 
    add_group_to_grid( Geostat_grid* grid, 
                          const std::string& group_name,
                          std::string group_type = "General" );


  /** This function sets-up the tail extrapolators of cdf \c cdf.
  * \c min_tag anc \c max_tag are the tags containing the min and
  * max values of the distribution. This function replaces the 
  * tail interpolators of \c cdf by power tail interpolators.
  * Note that \c cdf must be a valid cdf when passed to that function.
  * If the specified min is greater than the smallest values defining
  * the \c cdf, an error is reported (and similarly with the max)
  */
  GEOSTAT_DECL void setup_cdf_tails( NonParametricCdfType* cdf,
                                     const std::string& min_tag, 
                                     const std::string& max_tag,
                                     const Parameters_handler* parameters, 
				                             Error_messages_handler* errors);
  
  GEOSTAT_DECL void setup_cdf_tails( GsTLNonParametricCdfType* cdf,
                                     const std::string& min_tag, 
                                     const std::string& max_tag,
                                     const Parameters_handler* parameters, 
				                             Error_messages_handler* errors);

  /** Retrieves the advanced search neighborhood associated 
   * by parameter \a tag_name.  
   * 
   * @return a Search_filter object correctly parameterized
   * @param neigh Name of the neighborhood where the options applied
   * @param tag_name is the name of the parameter that contains the advanced
   * searach parameters
   * @param parameters is the parameter handler from which \a tag_name will 
   * be retrieved 
   * @param errors is where possible errors are reported 
   */ 
  GEOSTAT_DECL void set_advanced_search( Neighborhood* neigh,
                                   const std::string& tag_name,
			                             const Parameters_handler* parameters,
                                   Error_messages_handler* errors );


  GEOSTAT_DECL void set_advanced_search( NeighborhoodHandle neigh,
                                   const std::string& tag_name,
			                             const Parameters_handler* parameters,
                                   Error_messages_handler* errors );





  GEOSTAT_DECL Search_filter* set_advanced_search( const std::string& tag_name,
			                             const Parameters_handler* parameters,
                                   Error_messages_handler* errors );


  GEOSTAT_DECL Search_filter* set_octant_search( const std::string& tag_name,
			                    const Parameters_handler* parameters,
                          Error_messages_handler* errors );

  /** Creates a new property called \c new_prop_name in geostat_grid \c grid 
  * containing the cdf transform of property \c original_prop. \c original_prop
  * must either be a property of \c grid or have the same size as properties of
  * \c grid. Range [original_distribution_begin, original_distribution_end) defines
  * the reference distribution to use for the cdf transformation. It is usually
  * equal to [original_prop->begin(), original_prop->end() ), but doesn't have to.
  * Finally, \c original_cdf is the cdf in which the computed cdf of range
  * [original_distribution_begin, original_distribution_end) will be stored. That
  * cdf can later be used to back-transform the data.
  * The newly created property is returned.
  */


  template < class InputIterator, class Cdf >
  Grid_continuous_property* gaussian_transform_property( InputIterator original_distribution_begin,
                                                 InputIterator original_distribution_end,
                                                 Grid_continuous_property* original_prop,
                                                 Cdf& original_cdf,
                                                 Geostat_grid* grid ) {
    build_cdf_copy( original_distribution_begin, original_distribution_end,
	                  original_cdf );

    std::string transformed_prop_name = 
      "__" + original_prop->name() + "transformed__";

    Grid_continuous_property* transf_prop = 
      geostat_utils::add_property_to_grid( grid, transformed_prop_name );
    appli_assert( transf_prop );

    for( GsTLInt i = 0; i < original_prop->size() ; i++ ) {
      if( original_prop->is_informed( i ) ) {
        transf_prop->set_value( original_prop->get_value( i ), i );
      }
    }

    // transform the values
    Gaussian_cdf normal(0,1);
    cdf_transform(transf_prop, original_cdf, normal);
//    cdf_transform( transf_prop->begin(), transf_prop->end(), 
//                   original_cdf, normal );

    return transf_prop;
  }


  /** Creates a new property called \c new_prop_name in geostat_grid \c grid 
  * containing the cdf transform of property \c original_prop. \c original_prop
  * must either be a property of \c grid or have the same size as properties of
  * \c grid. Finally, \c original_cdf is a complete cdf. That
  * cdf can later be used to back-transform the data.
  * The newly created property is returned.
  */

  template < class Cdf >
  Grid_continuous_property* gaussian_transform_property( Grid_continuous_property* original_prop,
                                                 Cdf& original_cdf,
                                                 Geostat_grid* grid ) {
    std::string transformed_prop_name = 
      "__" + original_prop->name() + "transformed__";

    Grid_continuous_property* transf_prop = 
      geostat_utils::add_property_to_grid( grid, transformed_prop_name );
    appli_assert( transf_prop );

    for( GsTLInt i = 0; i < original_prop->size() ; i++ ) {
      if( original_prop->is_informed( i ) ) {
        transf_prop->set_value( original_prop->get_value( i ), i );
      }
    }

    // transform the values
    Gaussian_cdf normal(0,1);
    cdf_transform(transf_prop, original_cdf, normal);
//    cdf_transform( transf_prop->begin(), transf_prop->end(), 
//                   original_cdf, normal );

    return transf_prop;
  }



//This function transform the informed nodes from the source
// cdf to the target cdf	

template<class Cdf1, class Cdf2>
void cdf_transform( Grid_continuous_property* prop, Cdf1 cdf_source, Cdf2 cdf_target )
{
	for( int node_id=0; node_id< prop->size(); node_id++ )
	{
		if( prop->is_informed( node_id ) ) {
			double p = cdf_source.prob(prop->get_value( node_id));
			prop->set_value(cdf_target.inverse(p), node_id );
		}
	}
}




  /** Checks that triplet \c (major,medium,minor) defines a valid ellispoid ranges,
  *  i.e.: major >= medium >= minor > 0
  * If all the values are not strictly greater than 0, they are reset to a slightly
  * greater than 0 value.
  */
  
  template< class T >
  bool is_valid_range_triplet( T& major, T& medium, T& minor ) {
    bool is_sorted = ( major >= medium ) && ( medium >= minor );
    if( !is_sorted ) return false;

    if( minor > 0 ) return true;

    // Some values are <= 0: make them slightly greater than 0 
    const double epsilon = 0.000001;
    if( major <= 0  ) major = epsilon ;
    if( medium <= 0 ) medium = (major >= 1) ? epsilon : major/epsilon;
    if( minor <= 0 ) minor = (medium >= 1) ? epsilon : medium/epsilon;
    return true;
  }

  template< class Triplet >
  bool is_valid_range_triplet( Triplet& triplet ) {
    return is_valid_range_triplet( triplet[0], triplet[1], triplet[2] );
  }



    GEOSTAT_DECL bool get_non_param_cdf( GsTLNonParametricCdfType& cdf_non_param,
					              const Parameters_handler* parameters, 
				                Error_messages_handler* errors, std::string tag_name);


  GEOSTAT_DECL bool set_cdf_extrapolation_tail( const Parameters_handler* parameters,
	                               Error_messages_handler* errors,
                                 GsTLNonParametricCdfType& nparam_cdf,
	                               const std::string& LTI_str, 
                                 const std::string& UTI_str);

} // geostat_utils namespace
 
 
namespace distribution_utils {


GEOSTAT_DECL void cdf_transform( Grid_continuous_property* prop, 
          Continuous_distribution* cdf_source, 
          Continuous_distribution* cdf_target );
/*
{
	for( int node_id=0; node_id< prop->size(); node_id++ )
	{
		if( prop->is_informed( node_id ) ) {
			double p = cdf_source->prob(prop->get_value( node_id));
			prop->set_value(cdf_target->inverse(p), node_id );
		}
	}
}
*/

GEOSTAT_DECL Grid_continuous_property* gaussian_transform_property( Grid_continuous_property* original_prop,
                                                 Continuous_distribution* cdf_source,
                                                 Geostat_grid* grid );
/*{
    std::string transformed_prop_name = 
      "__" + original_prop->name() + "transformed__";

    Grid_continuous_property* transf_prop = 
      geostat_utils::add_property_to_grid( grid, transformed_prop_name );
    appli_assert( transf_prop );

    for( GsTLInt i = 0; i < original_prop->size() ; i++ ) {
      if( original_prop->is_informed( i ) ) {
        transf_prop->set_value( original_prop->get_value( i ), i );
      }
    }

    // transform the values
    //Gaussian_cdf normal(0,1);
   // Continuous_distribution *normal = new Gaussian_distribution(0,1);
    SmartPtr<Continuous_distribution> normal(new Gaussian_distribution(0,1));
    cdf_transform(transf_prop, cdf_source, normal.raw_ptr());
//    cdf_transform( transf_prop->begin(), transf_prop->end(), 
//                   original_cdf, normal );

    return transf_prop;
  }
  */



GEOSTAT_DECL bool get_non_param_cdf( Non_parametric_distribution& cdf_non_param,
					              const Parameters_handler* parameters, 
				                Error_messages_handler* errors, std::string tag_name);

GEOSTAT_DECL bool get_continuous_cdf(SmartPtr<Continuous_distribution>& cdist,
                        const Parameters_handler* parameters, 
				                Error_messages_handler* errors, std::string tag_name);

GEOSTAT_DECL bool set_cdf_extrapolation_tail( const Parameters_handler* parameters,
	                               Error_messages_handler* errors,
                                 Non_parametric_distribution& nparam_cdf,
	                               const std::string& LTI_str, 
                                 const std::string& UTI_str);

}


class GEOSTAT_DECL Trend_function_impl {
public :
	virtual double operator()(const Geovalue& gval) const  = 0;

	virtual Trend_function_impl* clone() = 0;
};

class GEOSTAT_DECL Trend_id_function_impl : public Trend_function_impl {
public:
	virtual double operator()(const Geovalue& gval)  const { return 1.0; }

	virtual Trend_function_impl* clone(){return new Trend_id_function_impl();}
};

class GEOSTAT_DECL Trend_x_function_impl : public Trend_function_impl {
public:
	virtual double operator()(const Geovalue& gval)  const { return gval.location().x();}
	virtual Trend_function_impl* clone(){return new Trend_x_function_impl();}
};

class GEOSTAT_DECL Trend_y_function_impl : public Trend_function_impl {
public:
	virtual double operator()(const Geovalue& gval)  const { return gval.location().y();}
	virtual Trend_function_impl* clone(){return new Trend_y_function_impl();}
};

class GEOSTAT_DECL Trend_z_function_impl : public Trend_function_impl {
public:
	virtual double operator()(const Geovalue& gval)  const { return gval.location().z();}
	virtual Trend_function_impl* clone(){return new Trend_z_function_impl();}
};

class GEOSTAT_DECL Trend_x2_function_impl : public Trend_function_impl {
public:
	virtual double operator()(const Geovalue& gval)  const {
	  GsTLCoord x = gval.location().x();
	  return x*x;
	}
	virtual Trend_function_impl* clone(){return new Trend_x2_function_impl();}
};

class GEOSTAT_DECL Trend_y2_function_impl : public Trend_function_impl {
public:
	virtual double operator()(const Geovalue& gval)  const {
	  GsTLCoord y = gval.location().y();
	  return y*y;
	}
	virtual Trend_function_impl* clone(){return new Trend_y2_function_impl();}
};

class GEOSTAT_DECL Trend_z2_function_impl : public Trend_function_impl {
public:
	virtual double operator()(const Geovalue& gval)  const {
	  GsTLCoord z = gval.location().z();
	  return z*z;
	}
	virtual Trend_function_impl* clone(){return new Trend_z2_function_impl();}
};
class GEOSTAT_DECL Trend_xy_function_impl : public Trend_function_impl {
public:
	virtual double operator()(const Geovalue& gval)  const {
	  Geovalue::location_type loc = gval.location();
	  return loc[0]*loc[1];
	}
	virtual Trend_function_impl* clone(){return new Trend_xy_function_impl();}
};

class GEOSTAT_DECL Trend_xz_function_impl : public Trend_function_impl {
public:
	virtual double operator()(const Geovalue& gval)  const {
	  Geovalue::location_type loc = gval.location();
	  return loc[0]*loc[2];
	}
	virtual Trend_function_impl* clone(){return new Trend_xz_function_impl();}
};

class GEOSTAT_DECL Trend_yz_function_impl : public Trend_function_impl {
public:
	virtual double operator() (const Geovalue& gval)  const {
	  Geovalue::location_type loc = gval.location();
	  return loc[1]*loc[2];
	}
	virtual Trend_function_impl* clone(){return new Trend_yz_function_impl();}
};

class GEOSTAT_DECL Drift_function_impl : public Trend_function_impl {
public:
	Drift_function_impl(const Geostat_grid* grid, const Grid_continuous_property* prop) {
		drift_[grid] = prop;
	}
	Drift_function_impl(const Geostat_grid* grid1, const Grid_continuous_property* prop1,
						const Geostat_grid* grid2, const Grid_continuous_property* prop2){
		drift_[grid1] = prop1;
		drift_[grid2] = prop2;
	}
	Drift_function_impl(std::map<const Geostat_grid*,const Grid_continuous_property*> drift){
		drift_ = drift;
	}
	void add_drift_property(const Geostat_grid* grid, const Grid_continuous_property* prop){
		drift_[grid] = prop;
	}

	virtual double operator()(const Geovalue& gval)  const {
		std::map<const Geostat_grid*,const Grid_continuous_property*>::const_iterator it = drift_.find(gval.grid());
		return it->second->get_value(gval.node_id());
	}

	virtual Trend_function_impl* clone(){return new Drift_function_impl(drift_);}

protected :
	std::map<const Geostat_grid*,const Grid_continuous_property*> drift_;
};

class GEOSTAT_DECL Trend_functor{
public:

  Trend_functor() : f_(0) {}
  Trend_functor( Trend_function_impl* f ) : f_(f) {}
  Trend_functor(const Trend_functor& rhs  ) {
	  f_ = rhs.f_->clone();
  }

  ~Trend_functor(){delete f_;}

  inline double operator()( const Geovalue& gval) const {
    return (*f_)(gval);
  }

private:
  Trend_function_impl* f_;
};


class GEOSTAT_DECL Trend_functions { 
 public: 
  typedef Geostat_grid::location_type Location; 
   
  static double id( const Geovalue& ) { return 1.0; }
  static double x( const Geovalue& gval ) { return gval.location().x(); };
  static double y( const Geovalue& gval ) { return gval.location().y(); };
  static double z( const Geovalue& gval ) { return gval.location().z(); };
  static double x2( const Geovalue& gval ) {
	  GsTLCoord x = gval.location().x();
	  return x*x;
  };
  static double y2( const Geovalue& gval ) {
	  GsTLCoord y = gval.location().y();
	  return y*y;
  };
  static double z2( const Geovalue& gval ) {
	  GsTLCoord z = gval.location().z();
	  return z*z;
  };
  static double xy( const Geovalue& gval )  {
	  Location loc = gval.location();
	  return loc[0]*loc[1];
  };
  static double xz( const Geovalue& gval ) {
	  Location loc = gval.location();
	  return loc[0]*loc[2];
  };
  static double yz( const Geovalue& gval ) {
	  Location loc = gval.location();
	  return loc[1]*loc[2];
  };
}; 
 
/*
class GEOSTAT_DECL Trend_functor{ 
public: 
  typedef Geostat_grid::location_type Location; 
  typedef double (*Functor)( const Geovalue& );
 
  Trend_functor() : f_(0),prop_(0) {}
  Trend_functor( Functor f ) : f_(f) {} 
 
  inline double operator()( const Geovalue& gval) const {
    return f_(gval);
  } 
 
private: 
  Functor f_;
}; 
*/
/*
class GEOSTAT_DECL Trend_functor{
public:
  typedef Geostat_grid::location_type Location;
  typedef double (*Functor)( const Location& );

  Trend_functor() : f_(0) {}
  Trend_functor( Functor f ) : f_(f) {}

  inline double operator()( const Location& loc) const {
    return f_(loc);
  }

private:
  Functor f_;
};
*/



#endif 
