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
** This file is part of the "grid" module of the Geostatistical Earth
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

#ifndef __GSTL_APPLI_GRID_GRID_TOPOLOGY_H__ 
#define __GSTL_APPLI_GRID_GRID_TOPOLOGY_H__ 
 
#include <grid/common.h>
#include <utils/gstl_types.h> 
#include <grid/grid_model/rgrid_geometry.h> 
#include <iostream> 
 

//This file has been renamed from GsTLGridTopology to grid_connection
 
class GRID_DECL RGrid_cell_connection { 
public: 
    RGrid_cell_connection( 
        RGrid_geometry* geo  
    ); 
     
    ~RGrid_cell_connection(); 
 
     
    void initialize(); 
 
    bool full_connection( GsTLInt idx ) const ; 
     
    bool next_i( GsTLInt idx ) const ; 
    bool next_j( GsTLInt idx ) const ; 
    bool next_k( GsTLInt idx ) const ; 
     
    bool prev_i( GsTLInt idx ) const ; 
    bool prev_j( GsTLInt idx ) const ; 
    bool prev_k( GsTLInt idx ) const ; 
 
 
    void break_next_i( GsTLInt idx ); 
    void break_next_j( GsTLInt idx ); 
    void break_next_k( GsTLInt idx ); 
 
    void break_prev_i( GsTLInt idx ); 
    void break_prev_j( GsTLInt idx ); 
    void break_prev_k( GsTLInt idx ); 
     
    void print_flag( std::ostream& os, GsTLInt idx ); 
 
    GsTLInt nx() const; 
    GsTLInt ny() const; 
    GsTLInt nz() const; 
     
protected: 
enum GridFlag { 
        NextI = 1, 
        NextJ = 2, 
        NextK = 4, 
        PrevI = 8, 
        PrevJ = 16, 
        PrevK = 32 
     }; 
protected: 
protected: 
 
    RGrid_geometry* geom_; 
    GsTLUInt * flags_; 
private: 
    GsTLUInt full_connection_; 
}; 
 
//______________ INLINE FUNCTIONS __________________ 
 
inline 
RGrid_cell_connection::RGrid_cell_connection( 
     RGrid_geometry* geo  
) : geom_(geo) { 
    flags_ = new GsTLUInt [nx()*ny()*nz()]; 
    full_connection_ = NextI | NextJ | NextK | 
                       PrevI | PrevJ | PrevK; 
    initialize(); 
} 
 
inline 
RGrid_cell_connection::~RGrid_cell_connection() { 
    delete [] flags_; 
} 
 
inline 
GsTLInt RGrid_cell_connection::nx()  const { 
    return (geom_->dim(0)); 
} 
 
inline 
GsTLInt RGrid_cell_connection::ny()  const { 
    return (geom_->dim(1)); 
} 
 
inline 
GsTLInt RGrid_cell_connection::nz()  const { 
    return (geom_->dim(2)); 
} 
 
inline 
void RGrid_cell_connection::initialize() { 
    GsTLInt nx1 = nx()-1; 
    GsTLInt ny1 = ny()-1; 
    GsTLInt nz1 = nz()-1; 
     
    GsTLInt ijk = 0; 
    for( GsTLInt k = 0; k < nz(); ++k ) { 
        for( GsTLInt j = 0; j < ny(); ++j ) { 
            for( GsTLInt i = 0; i < nx(); ++i ) { 
                // Init with a full conectivity flags 
                flags_[ijk] = full_connection_; 
                // Break connections on first face 
                if ( i == 0 ) break_prev_i(ijk); 
                if ( j == 0 ) break_prev_j(ijk); 
                if ( k == 0 ) break_prev_k(ijk); 
                // Break connections on last face 
                if ( i == nx1 ) break_next_i(ijk); 
                if ( j == ny1 ) break_next_j(ijk); 
                if ( k == nz1 ) break_next_k(ijk); 
                ijk++; 
            } 
        } 
    } 
     
} 
 
inline  
bool RGrid_cell_connection::full_connection( GsTLInt idx ) const { 
    return (!((flags_[idx] ^ full_connection_) & full_connection_)); 
} 
 
inline 
bool RGrid_cell_connection::next_i( GsTLInt idx ) const { 
    return (flags_[idx] & NextI ); 
} 
 
inline 
bool RGrid_cell_connection::next_j( GsTLInt idx ) const { 
    return (flags_[idx] & NextJ ); 
} 
 
inline 
bool RGrid_cell_connection::next_k( GsTLInt idx ) const { 
    return (flags_[idx] & NextK ); 
} 
 
inline 
bool RGrid_cell_connection::prev_i( GsTLInt idx ) const { 
    return (flags_[idx] & PrevI ); 
} 
 
inline 
bool RGrid_cell_connection::prev_j( GsTLInt idx ) const { 
    return (flags_[idx] & PrevJ ); 
} 
 
inline 
bool RGrid_cell_connection::prev_k( GsTLInt idx ) const { 
    return (flags_[idx] & PrevK ); 
} 
 
inline 
void RGrid_cell_connection::break_next_i( GsTLInt idx ) { 
    flags_[idx] &= ~NextI; 
} 
 
inline 
void RGrid_cell_connection::break_next_j( GsTLInt idx ) { 
    flags_[idx] &= ~NextJ; 
} 
 
inline 
void RGrid_cell_connection::break_next_k( GsTLInt idx ) { 
    flags_[idx] &= ~NextK; 
} 
 
inline 
void RGrid_cell_connection::break_prev_i( GsTLInt idx ) { 
    flags_[idx] &= ~PrevI; 
} 
 
inline 
void RGrid_cell_connection::break_prev_j( GsTLInt idx ) { 
    flags_[idx] &= ~PrevJ; 
} 
 
inline 
void RGrid_cell_connection::break_prev_k( GsTLInt idx ) { 
    flags_[idx] &= ~PrevK; 
} 
 
inline 
void RGrid_cell_connection::print_flag( std::ostream& os, GsTLInt idx )  { 
    GsTLInt nxyz = nx()*ny()*nz(); 
    if( idx < 0 || idx >= nxyz ) { 
        os << "Index OUT of limits ...." << idx << std::endl; 
        return; 
    } 
    os << next_i(idx) << " " 
    << next_j(idx) << " " 
    << next_k(idx) << " " 
    << prev_i(idx) << " " 
    << prev_j(idx) << " " 
    << prev_k(idx) << std::endl; 
     
} 
 
 
#endif 
