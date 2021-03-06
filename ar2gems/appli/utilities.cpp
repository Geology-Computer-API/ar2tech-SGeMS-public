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
** This file is part of the "appli" module of the Geostatistical Earth
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

#include <appli/utilities.h>
#include <appli/manager_repository.h>

#include <algorithm>

namespace utils {

SmartPtr<Progress_notifier> 
create_notifier( const std::string& title,
                 int total_steps,
                 int frequency ) {

  Manager::type_iterator found = 
    std::find( Root::instance()->begin(), Root::instance()->end(),
               "progress_notifier" );
  if( found == Root::instance()->end() ) {
    // bind a factory method that creates a default notifier
    Root::instance()->factory( "progress_notifier", Void_notifier::create_new_interface );
  }

  SmartPtr<Named_interface> ni = 
    Root::instance()->new_interface( "progress_notifier", "/" );

  Progress_notifier* notifier = dynamic_cast<Progress_notifier*>( ni.raw_ptr() );
  notifier->title( title );
  notifier->total_steps( total_steps );
  notifier->frequency( frequency );

  return SmartPtr<Progress_notifier>( notifier );
}

} // end of utils namespace

