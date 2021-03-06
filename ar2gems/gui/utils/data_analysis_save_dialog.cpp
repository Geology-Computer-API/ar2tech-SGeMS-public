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

#include <gui/utils/data_analysis_save_dialog.h>

#include <qcheckbox.h>
#include <qlabel.h>
#include <QLayout>

Data_analysis_save_dialog::
Data_analysis_save_dialog( const QString& dirName, 
                       const QString& filter,
                       QWidget* parent, const char* name ) 
  : QFileDialog( parent, QString(), dirName, filter ) {

  if (name)
    setObjectName(name);

  setModal(true);

  this->layout()->addWidget( new QLabel("Extra Options ", this) );
  
  stats_checkbox_ = new QCheckBox( "Write summary statistics", this);
  stats_checkbox_->setChecked( true );
  this->layout()->addWidget(  stats_checkbox_ );

  grid_checkbox_ = new QCheckBox( "Paint grid", this);
  grid_checkbox_->setChecked( true );
  this->layout()->addWidget( grid_checkbox_);
  
  // No postscript mode in Windows because of some limitations of QPrinter on
  // that plateform
//#if defined(WIN32) || defined(_WIN32)
  //setFilters( QString("PNG format (*.png);;BMP format (*.bmp)") );
//#else
  QStringList lst;
  lst << "PNG format (*.png)"
      << "BMP format (*.bmp)"
      << "Postscript (*.ps)";
  
  setFilters( lst);
//#endif
  
  setFileMode( QFileDialog::AnyFile );
}


bool Data_analysis_save_dialog::write_stats_required() const {
  return stats_checkbox_->isChecked();
}


bool Data_analysis_save_dialog::paint_grid_required() const {
  return grid_checkbox_->isChecked();
}


