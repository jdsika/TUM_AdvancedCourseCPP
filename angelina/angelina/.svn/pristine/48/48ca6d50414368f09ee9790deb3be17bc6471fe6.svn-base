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


#include "editplaygrounddialog.h"

#include <QVBoxLayout>
#include <QGroupBox>
#include <QLabel>
#include <QDoubleSpinBox>
#include <QPushButton>
#include <QDebug>

#include "defines.h"

EditPlayGroundDialog::EditPlayGroundDialog(double *a, double *b):
	QDialog(),
	myA(a),
	myB(b)
{
	QVBoxLayout *mainLayout = new QVBoxLayout;
	setLayout(mainLayout);
        QGroupBox *grBox = new QGroupBox("Length", this);
	QVBoxLayout *grBoxLayout = new QVBoxLayout;
	grBox->setLayout(grBoxLayout);
	mainLayout->addWidget(grBox);
	
	QHBoxLayout *spALayout = new QHBoxLayout;
	QLabel *infoA=new QLabel("A:", grBox);
	spA = new QDoubleSpinBox(grBox);
	
	spA->setDecimals(2);
	spA->setSuffix(" m");
	spA->setSingleStep(0.01);
	spA->setValue(*myA);
	spALayout->addWidget(infoA);
	spALayout->addWidget(spA);
	connect(spA, SIGNAL(valueChanged(double)), this, SLOT(slotAChanged(double)));
	
	QHBoxLayout *spBLayout = new QHBoxLayout;
	QLabel *infoB = new QLabel("B:",  grBox);
	spB = new QDoubleSpinBox(grBox);
	spB->setDecimals(2);
	spB->setSuffix(" m");
	spB->setSingleStep(0.01);
	spB->setValue(*myB);
	spBLayout->addWidget(infoB);
	spBLayout->addWidget(spB);
	connect(spB, SIGNAL(valueChanged(double)), this, SLOT(slotBChanged(double)));
	
	spA->setMaximum((2.0 * spB->value()) - SPACE);
	spB->setMinimum((spA->value() / 2.0) + SPACE);
	
	grBoxLayout->addLayout(spALayout);
	grBoxLayout->addLayout(spBLayout);
	
	QPushButton *bOk = new QPushButton("OK", this);
	connect(bOk, SIGNAL(clicked()), this, SLOT(accept()));
        QPushButton *bCancel = new QPushButton("Cancel", this);
	connect(bCancel, SIGNAL(clicked()), this, SLOT(reject()));
	
	QHBoxLayout *resultLayout = new QHBoxLayout;
	mainLayout->addLayout(resultLayout);
	resultLayout->addWidget(bOk);
	resultLayout->addWidget(bCancel);
}

EditPlayGroundDialog::~EditPlayGroundDialog()
{
}


void EditPlayGroundDialog::accept()
{
	*myA = spA->value();
	*myB = spB->value();
	QDialog::accept();
}


void EditPlayGroundDialog::slotAChanged(double value)
{
	spB->setMinimum((value / 2.0) + SPACE);
}

void EditPlayGroundDialog::slotBChanged(double value)
{
	spA->setMaximum((2.0 * value) - SPACE);
}

