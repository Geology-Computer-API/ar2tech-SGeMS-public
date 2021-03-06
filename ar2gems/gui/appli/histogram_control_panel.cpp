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
** This file is part of the "gui" module of the Geostatistical Earth
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

#include <gui/appli/histogram_control_panel.h>
#include <appli/project.h>
#include <appli/manager_repository.h>
#include <qtplugins/selectors.h>
#include <grid/grid_model/grid_property.h>
#include <grid/grid_model/grid_region.h>
#include <grid/grid_model/geostat_grid.h>

#include <qgroupbox.h>
#include <qlineedit.h>
#include <qspinbox.h>


Histogram_control_panel::
Histogram_control_panel( GsTL_project* proj,
                         QWidget* parent, const char* name ) 
  : QWidget( parent ) {

  setupUi(this);

  if (name)
    setObjectName(name);

  QVBoxLayout * vbox = new QVBoxLayout(property_selector_box_);
  property_selector_box_->setLayout(vbox);
  object_selector_ = new PropertySelector( property_selector_box_,
                                           "object_selector_" );
  vbox->addWidget(object_selector_);
  
  //TL modified
  QVBoxLayout * vbox1 = new QVBoxLayout(_curves_box);
  _curves_box->setLayout(vbox1);
  _combo = new QComboBox(_curves_box);
  vbox1->addWidget(_combo);
  _combo->addItem("pdf");
  _combo->addItem("cdf");
  _combo->addItem("pdf+cdf");

  init( proj );

  // forward signals
   QObject::connect( object_selector_,
                    SIGNAL( property_changed( const QString& ) ),
                    this,
                    SLOT( forward_var_changed( const QString& ) ) );

   QObject::connect( object_selector_,
                    SIGNAL( region_changed( const QString& ) ),
                    this,
                    SLOT( forward_var_changed( const QString& ) ) );

  QObject::connect( (QObject*) minval_edit_, SIGNAL( returnPressed() ),
                    this, SLOT( forward_low_clip_changed() ) );
  QObject::connect( (QObject*) maxval_edit_, SIGNAL( returnPressed() ),
                    this, SLOT( forward_high_clip_changed() ) );
  QObject::connect( (QObject*) reset_clips_button_, SIGNAL( clicked() ),
                    SIGNAL( reset_clipping_values_clicked() ) );

  QObject::connect( bins_spinbox_, SIGNAL( valueChanged( int ) ),
                    SIGNAL( bins_count_changed( int ) ) );

  QObject::connect( _combo, SIGNAL( activated( const QString & ) ),
                    SIGNAL( comboChanged( const QString & ) ) );


}




Histogram_control_panel::~Histogram_control_panel() {
}

void Histogram_control_panel::init( GsTL_project* project ) {
  object_selector_->init( project );
}

int Histogram_control_panel::bins_count() const {
  return bins_spinbox_->value();
} 



void Histogram_control_panel::set_clipping_values( float low, float high ) {
  QString val;
  val.setNum( low );
  minval_edit_->setText( val );
  val.setNum( high );
  maxval_edit_->setText( val );
}


Grid_continuous_property*
Histogram_control_panel::get_property( const PropertySelector* object_selector ) {
  if( object_selector->selectedGrid().isEmpty() ||
      object_selector->selectedProperty().isEmpty() ) return 0;

  QByteArray tmp;
  tmp = object_selector->selectedGrid().toLatin1() ;
  std::string grid_name( tmp.constData());
  Geostat_grid* grid = dynamic_cast<Geostat_grid*>(
                Root::instance()->interface(
                                            gridModels_manager + "/" + grid_name
                                            ).raw_ptr()
                );

  appli_assert( grid );

  tmp = object_selector->selectedProperty().toLatin1() ;
  std::string prop_name( tmp.constData());
  Grid_continuous_property* prop = grid->property( prop_name );
//  if(prop == 0) return 0;
  appli_assert( prop );
  return prop;
}
 

Grid_region*
Histogram_control_panel::get_region( const PropertySelector* object_selector ) {
  if( object_selector->selectedGrid().isEmpty() ||
      object_selector->selectedRegion().isEmpty() ) return 0;

  std::string grid_name = object_selector->selectedGrid().toStdString();
  Geostat_grid* grid = dynamic_cast<Geostat_grid*>(
                Root::instance()->interface(
                                            gridModels_manager + "/" + grid_name
                                            ).raw_ptr()
                );

  appli_assert( grid );

  std::string region_name = object_selector->selectedRegion().toStdString();
  Grid_region* region = grid->region( region_name );
//  appli_assert( region_name.empty() );
  return region;
}


void Histogram_control_panel::forward_var_changed( const QString& ) {
  emit var_changed( get_property( object_selector_ ), get_region( object_selector_ ) );
}
/*
void Histogram_control_panel::forward_var_changed( const QString& ) {
  emit var_changed( get_property( object_selector_ ), get_region );
}
*/
/*
void Histogram_control_panel::forward_reg_changed( const QString& ) {
  emit var_changed( get_property( object_selector_ ) );
}
*/
void Histogram_control_panel::forward_low_clip_changed() {
  emit low_clip_changed( minval_edit_->text().toFloat() );
}
void Histogram_control_panel::forward_high_clip_changed() {
  emit high_clip_changed( maxval_edit_->text().toFloat() );
}


