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


#ifndef EDITPLAYGROUNDDIALOG_H
#define EDITPLAYGROUNDDIALOG_H

#include <QDialog>

class QDoubleSpinBox;

//! GUI dialog to change Playground dimensions
/*!
 * Dialog is executed modal and prevents user from wrong dimensions.
 */
class EditPlayGroundDialog:public QDialog 
{

	Q_OBJECT

	public:

		//! Creates the dialog modal to its parent
		/*!
		 * \param *a the a value (see Playground)
		 * \param *b the b value (see Playground)
		 */
		EditPlayGroundDialog(double *a, double *b);

		//! Destroys the object, does nothing
		/*
		 */
		~EditPlayGroundDialog();




	private:

		//! Spinbox to edit the a value
		/*
		 */
		QDoubleSpinBox *spA;

		//! Spinbox to edit the b value
		/*
		 */
		QDoubleSpinBox *spB;

		//! The a value
		/*
		 */
		double *myA;

		//! The b value
		/*
		 */
		double *myB;

	public slots:

		//! The dialog is accepted by the user
		/*!
		 * Writes the values from the spinboxes to a and b.
		 * Closes the dialog. Rejection see Qt documnetation.
		 */
		void accept();




	private slots:

		//! Recieves changes in the spinbox for a
		/*!
		 * Used for error checks.
		 * \param value the new a value
		 */
		void slotAChanged(double value);

		//! Recieves changes in the spinbox for b
		/*!
		 * Used for error checks.
		 * \param value the new b value
		 */
		void slotBChanged(double value);
		
};

#endif

