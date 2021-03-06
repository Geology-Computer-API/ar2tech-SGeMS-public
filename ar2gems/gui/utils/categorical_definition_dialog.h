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
** Author: Alexandre Boucher
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

#ifndef __GSTLAPPLI_GUI_UTILS_CATEGORICAL_DEFINITION_DIALOG_H__
#define __GSTLAPPLI_GUI_UTILS_CATEGORICAL_DEFINITION_DIALOG_H__

#include <gui/common.h>
#include <gui/models/categorical_table_model.h>

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QSpinBox>
#include <QtGui/qlineedit.h>

#include <qdialog.h>


class QRadioButton;
class QTextBrowser;
class QTextEdit;
class MultiCategoricalPropertySelector;
class GridSelectorBasic;
class CategoricalDefinitionViewer;
class CategoricalDefinitionSelector;
class GsTL_project;


QT_BEGIN_NAMESPACE

class GUI_DECL View_category_definition_dialog : public QDialog {

  Q_OBJECT

public:

  View_category_definition_dialog( GsTL_project* proj,QWidget* parent = 0, const char* name = 0 );
  virtual ~View_category_definition_dialog() {}

public slots:
  void display_properties(const QString& def_name);

  /*
signals:
  void selected_definition(QString& cat_def_name);
*/
protected:
  CategoricalDefinitionSelector* def_selector_;
  CategoricalDefinitionViewer* def_viewer_;
  QTextBrowser* properties_viewer_;

  QString current_definition_;
};


class GUI_DECL New_category_definition_dialog : public QDialog {

  Q_OBJECT

public:

  New_category_definition_dialog( GsTL_project* project,  QWidget* parent = 0, const char* name = 0 );
  virtual ~New_category_definition_dialog() {}

public slots:
  void show_color_editor(const QModelIndex& index);

protected slots:
  void clear();
  void create_definition();
  void create_definition_close();
  

protected:
  QLineEdit* cat_def_name_;
  Categorical_table_model* model_;
  QSpinBox* ncat_;
//  QTextEdit* cat_names_text_;
  GsTL_project* project_;
  
};


class GUI_DECL Assign_category_definition_dialog : public QDialog {

  Q_OBJECT

public:

  Assign_category_definition_dialog( GsTL_project* proj,QWidget* parent = 0, const char* name = 0 );
  virtual ~Assign_category_definition_dialog() {}

public slots:
  void assign_definition();
  void assign_definition_close();
  

  /*
signals:
  void selected_definition(QString& cat_def_name);
*/
protected:
  CategoricalDefinitionSelector* def_selector_;
  MultiCategoricalPropertySelector* props_selector_;
  GridSelectorBasic* grid_selector_;

  GsTL_project* project_;

};

QT_END_NAMESPACE
#endif


