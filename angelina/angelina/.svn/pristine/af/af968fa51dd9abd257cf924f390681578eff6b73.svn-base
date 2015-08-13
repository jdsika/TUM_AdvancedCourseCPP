/***************************************************************************
 *   Server GUI "angelina" for "Projektkurs C++" WS 07/08				   *
 *   																	   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/


#ifndef EDITOPTIONSDIALOG_H
#define EDITOPTIONSDIALOG_H

#include <QDialog>

class QCheckBox;

struct Options;

//! Struct for checkboxes to toggle the options
/*
 */
struct CheckBoxes
{
	public:

		//! Checkbox for the display mode
		/*
		 */
		QCheckBox *displayMode;

		//! Checkbox for the disqualify mode
		/*
		 */
		QCheckBox *disqualifyMode;

		//! Checkbox for the connection mode
		/*
		 */
		QCheckBox *connectionMode;

		//! Checkbox for the robot count
		/*
		 */
		QCheckBox *robotCount;

};


//! GUI dialog to change Playground dimensions
/*!
 * Dialog is executed modal and prevents user from wrong dimensions.
 */
class EditOptionsDialog:public QDialog 
{

	Q_OBJECT

	public:

		//! Creates the dialog modal to its parent
		/*!
		 * \param *options the options to edit
		 */
		EditOptionsDialog(Options *options);

		//! Destroys the object, does nothing
		/*
		 */
		~EditOptionsDialog();




	private:

		//! Checkboxes for the options
		/*
		 */
		CheckBoxes checkBoxes;

		//! Options to edit
		/* 
		 */
		Options *myOptions;




	public slots:

		//! The dialog is accepted by the user
		/*!
		 * Writes the values from the checkboxes to the options and saves the configfile.
		 * Closes the dialog. Rejection see Qt documnetation.
		 */
		void accept();

};

#endif
