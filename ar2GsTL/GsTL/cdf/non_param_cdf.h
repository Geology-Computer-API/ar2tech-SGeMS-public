/* GsTL: the Geostatistics Template Library
 * 
 * Author: Nicolas Remy
 * Copyright (c) 2000 The Board of Trustees of the Leland Stanford Junior University
 * 
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without modification, 
 * are permitted provided that the following conditions are met:
 * 
 *   1.Redistributions of source code must retain the above copyright notice, this 
 *     list of conditions and the following disclaimer. 
 *   2.Redistributions in binary form must reproduce the above copyright notice, this 
 *     list of conditions and the following disclaimer in the documentation and/or other
 *     materials provided with the distribution. 
 *   3.The name of the author may not be used to endorse or promote products derived 
 *     from this software without specific prior written permission. 
 * 
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED 
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY 
 * AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHOR BE 
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

 

#ifndef __GSTL_CDF_NON_PARAM_CDF_H___
#define __GSTL_CDF_NON_PARAM_CDF_H___
#ifdef __GNUC__
#pragma interface
#endif

#include <GsTL/cdf/cdf_basics.h>
#include <GsTL/cdf/interpolators.h>
#include <GsTL/utils/gstl_error_messages.h>
#include <GsTL/univariate_stats/build_cdf.h>

#include <vector>
#include <algorithm>

#include <cstdlib>



template<
  class LowerTailInterp = Tail_interpolator, 
  class MiddleInterp    = Linear_interpol,
  class UpperTailInterp = Tail_interpolator
>
class Non_param_cdf : public Non_parametric_cdf< float > {
private:
  typedef LowerTailInterp LTI;
  typedef MiddleInterp    MI;
  typedef UpperTailInterp UTI;

public:
  typedef typename Non_parametric_cdf< float >::value_type value_type;
  typedef GsTL::continuous_variable_tag variable_category;
  

  //-----------------
  // constructors 

  Non_param_cdf(
    const LTI& low_interp = Tail_interpolator( new Exponential_LTI ),
		const MI& mid_interp  = Linear_interpol(),
		const UTI& up_interp  = Tail_interpolator( new Hyperbolic_UTI() )
		) 
  : low_interp_(low_interp),
    mid_interp_(mid_interp),
    up_interp_(up_interp) {
  };
  

  template<class Z_iterator>
  Non_param_cdf(Z_iterator z_begin, Z_iterator z_end, 
    const LTI& low_interp = Tail_interpolator( new Exponential_LTI ),
		const MI& mid_interp  = Linear_interpol(),
		const UTI& up_interp  = Tail_interpolator( new Hyperbolic_UTI() )
		) 
  : low_interp_(low_interp),
    mid_interp_(mid_interp),
    up_interp_(up_interp)  {

    for(Z_iterator it=z_begin; it != z_end ; it++)
      z_values_.push_back(*it);
    
    //Assume that the z are equiprobable
    p_values_.clear();
    p_values_.insert( p_values_.begin(), z_values_.size(), 1.0/z_values_.size() );
    //p_values_.resize(z_values_.size());

    range = z_values_.back() - z_values_.front();
  };


  template<class Z_iterator, class P_iterator>
  Non_param_cdf(Z_iterator z_begin, Z_iterator z_end,
		P_iterator p_begin,
    const LTI& low_interp = Tail_interpolator( new Exponential_LTI ),
		const MI& mid_interp  = Linear_interpol(),
		const UTI& up_interp  = Tail_interpolator( new Hyperbolic_UTI() )
		)
  : low_interp_(low_interp),
    mid_interp_(mid_interp),
    up_interp_(up_interp)  {

    P_iterator p_it = p_begin;
    for(Z_iterator it=z_begin; it != z_end ; ++it, ++p_it) {
      z_values_.push_back(*it);
      p_values_.push_back(*p_it);
    }

    range = z_values_.back() - z_values_.front();
  };

  
  void upper_tail_interpolator( UTI interpolator ) {
    up_interp_ = interpolator;
  } 
  void lower_tail_interpolator( LTI interpolator ) {
    low_interp_ = interpolator;
  } 
  void middle_interpolator( MI interpolator ) {
    mid_interp_ = interpolator;
  } 

  UpperTailInterp upper_tail_interpolator( ) const {
    return UpperTailInterp(up_interp_);
  } 
  LowerTailInterp lower_tail_interpolator( ) const{
    return LowerTailInterp(low_interp_);
  } 
  MiddleInterp middle_interpolator( ) const{
    return MiddleInterp(mid_interp_);
  }

  virtual ~Non_param_cdf() {}

  //-----------------

  template<class ForwardIterator>
  inline void z_set(ForwardIterator z_begin, ForwardIterator z_end);
 
  template<class ForwardIterator>
  inline void p_set(ForwardIterator p_begin, ForwardIterator p_end);

 
  virtual bool make_valid() { return make_cdf_valid( *this); } 
  virtual double prob(value_type z) const ;
  virtual value_type inverse(double p) const ;
  virtual double pdf(value_type z) const ;

private:
  
  LowerTailInterp low_interp_;
  MiddleInterp mid_interp_;
  UpperTailInterp up_interp_;

  value_type range;

}; // end of class Non_param_cdf



//___________________________
// Inline functions

template<class lower_tail_interp, class middle_interp, class upper_tail_interp>
template<class forward_iterator>
inline void Non_param_cdf<lower_tail_interp, middle_interp,
			  upper_tail_interp>::z_set(forward_iterator z_begin, 
						      forward_iterator z_end)
{
  p_values_.clear();
  z_values_.resize(z_end - z_begin);
  p_values_.resize(z_values_.size());

  int i=0;
  
  for(forward_iterator it = z_begin; it!=z_end; it++)
    z_values_[i++] = *it;
  
  range = z_values_.back() - z_values_.front();
}



template<class lower_tail_interp, class middle_interp, class upper_tail_interp>
template<class forward_iterator>
inline void Non_param_cdf<lower_tail_interp, middle_interp,
			  upper_tail_interp>::p_set(forward_iterator p_begin, 
						      forward_iterator p_end)
{
  gstl_assert(p_end - p_begin <= p_values_.size());
  int i=0;
  for(forward_iterator it = p_begin; it!=p_end; it++)
    p_values_[i++] = *it;
    
}


//_________________________________________
// Definition of member function "prob"

template<class lower_tail_interp, class middle_interp, class upper_tail_interp>
inline double 
Non_param_cdf<lower_tail_interp,
              middle_interp,
              upper_tail_interp>::
prob(typename Non_param_cdf::value_type z) const {
  gstl_assert_2( is_valid_cdf( *this ) );

  // use STL lower_bound algo to find where in the z-range, z should be placed
  //const_z_iterator pos = std::lower_bound(z_values_.begin(),z_values_.end(),z);
  const_z_iterator pos_first = std::lower_bound(z_values_.begin(),z_values_.end(),z);
  const_z_iterator pos_last = std::upper_bound(z_values_.begin(),z_values_.end(),z);

  const_z_iterator pos;
  if( ( pos_last - pos_first ) <= 1 ) pos = pos_first;
  else {
    int n_ties = pos_last - pos_first;
    pos = z_values_.begin() + std::distance(z_values_.begin(), pos_first) +  n_ties*std::rand()/RAND_MAX;
  }


  // z <= z1: use lower tail interpolator
  if(pos == z_values_.begin()) {
    if( GsTL::equals( z, z_values_[0] ) )
      return p_values_[0];

    return low_interp_.p(*(z_values_.begin()), *(p_values_.begin()), z);
  }

  // z > zN (N is the number of z-values): use upper tail interpolator
  if(pos == z_values_.end())
    return up_interp_.p( *(z_values_.end()-1), *(p_values_.end()-1), z);
    
  // otherwise, use the "middle" interpolator, unless z==z2
  value_type z2 = *pos;
  double p2 = p_values_[pos - z_values_.begin()];
  if( GsTL::equals( z, z2 ) )
    return p2;

  value_type z1 = *(pos-1);
  double p1 = p_values_[pos-1 - z_values_.begin()];
  
  return mid_interp_.p(z1,p1,z2,p2,z);
}

template<class lower_tail_interp, class middle_interp, class upper_tail_interp>
inline double 
Non_param_cdf<lower_tail_interp,
              middle_interp,
              upper_tail_interp>::
pdf(typename Non_param_cdf::value_type z) const {

  double delta = range/1000;
  double p_low = this->prob(z - delta);
  double p_high = this->prob(z + delta);
  return (p_high - p_low)/2/delta;

  /*
  const_z_iterator z_pos_first = std::lower_bound(z_values_.begin(),z_values_.end(),z);
  // z <= z1: use lower tail interpolator
  if(z_pos_first == z_values_.begin()) {
//    if( GsTL::equals( z, z_values_[0] ) )
//      return p_values_[0];
    double high = low_interp_.p(*(z_values_.begin()), *(p_values_.begin()), z+0.0001);
    double low = low_interp_.p(*(z_values_.begin()), *(p_values_.begin()), z-0.0001);
    return (high-low)/0.0002;

//    return low_interp_.p(*(z_values_.begin()), *(p_values_.begin()), z);
  }
  else if (z_pos_first == z_values_.end()){
    double high = up_interp_.p(*(z_values_.end()-1), *(p_values_.end()-1), z+0.0001);
    double low = up_interp_.p(*(z_values_.end()-1), *(p_values_.end()-1), z-0.0001);
    return (high-low)/0.0002;
  }
  else {
    const_z_iterator z_pos_last = std::upper_bound(z_values_.begin(),z_values_.end(),z);

    int n_ties = z_pos_last - z_pos_first;

    const_z_iterator z_pos_low = z_pos_first-1;
    value_type dz = *z_pos_last - *z_pos_low;

    const_p_iterator p_pos_low = p_values_.begin() + std::distance(z_values_.begin(),z_pos_low );
    const_p_iterator p_pos_up = p_values_.begin() + std::distance(z_values_.begin(),z_pos_last );
    if(dz == 0) {  //Should not happen
      double high = up_interp_.p(*(z_values_.end()-1), *(p_values_.end()-1), z+0.0001);
      double low = up_interp_.p(*(z_values_.end()-1), *(p_values_.end()-1), z-0.0001);
      return (high-low)/0.0002;

    }   
    else {
      return (*p_pos_up - *p_pos_low)/dz;
    }

  }
  */
}



//_________________________________________
// Definition of member function "inverse"

template<class lower_tail_interp, class middle_interp, class upper_tail_interp>
inline typename Non_param_cdf<lower_tail_interp, middle_interp,
			      upper_tail_interp>::value_type
Non_param_cdf<lower_tail_interp, 
              middle_interp,
              upper_tail_interp>::
inverse(double p) const {
  gstl_assert_2( is_valid_cdf( *this ) );

  // use STL lower_bound algo to where in the p-range, p should be placed
  const_p_iterator pos = std::lower_bound(p_values_.begin(), p_values_.end(), p);

  // p <= p1: use lower tail interpolator
  if(pos == p_values_.begin())
    return low_interp_.z( *(z_values_.begin()), *(p_values_.begin()), p);
    
  // p >= pN (N is the number of p-values): use upper tail interpolator
  if(pos == p_values_.end())
    return up_interp_.z( *(z_values_.end()-1), *(p_values_.end()-1), p);
    
  // otherwise, use the "middle" interpolator 
  value_type p2 = *pos;
  value_type p1 = *(pos-1);
  double z2 = z_values_[pos - p_values_.begin()];
  double z1 = z_values_[pos-1 - p_values_.begin()];
  
  value_type result = mid_interp_.z(z1,p1,z2,p2,p);

  return result;
}



#endif
