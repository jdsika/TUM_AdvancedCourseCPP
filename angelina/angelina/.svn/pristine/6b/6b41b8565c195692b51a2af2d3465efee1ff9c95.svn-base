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


#include "editoptionsdialog.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QCheckBox>
#include <QPushButton>
#include <QDebug>

#include "angelinaapplication.h"


EditOptionsDialog::EditOptionsDialog(Options *options):
	QDialog(),
	myOptions(options)

{
	QVBoxLayout *mainLayout = new QVBoxLayout;
	setLayout(mainLayout);

	checkBoxes.displayMode = new QCheckBox("Windowed", this);
	mainLayout->addWidget(checkBoxes.displayMode);
	if(myOptions->displayMode == AngelinaApplication::DisplayWindowed)
	{
		checkBoxes.displayMode->setChecked(true);
	}

	checkBoxes.disqualifyMode = new QCheckBox("Disconnect on disqualification", this);
	mainLayout->addWidget(checkBoxes.disqualifyMode);
	if(myOptions->disqualifyMode == AngelinaApplication::DisconnectOnDisqualify)
	{
		checkBoxes.disqualifyMode->setChecked(true);
	}

	checkBoxes.connectionMode = new QCheckBox("Auto connection", this);
	mainLayout->addWidget(checkBoxes.connectionMode);
	if(myOptions->connectionMode == AngelinaApplication::ConnectionAuto)
	{
		checkBoxes.connectionMode->setChecked(true);
	}
	checkBoxes.robotCount = new QCheckBox("Single mode", this);
	mainLayout->addWidget(checkBoxes.robotCount);
	if(myOptions->robotCount == AngelinaApplication::OneRobot)
	{
		checkBoxes.robotCount->setChecked(true);
	}
	
	QPushButton *bOk = new QPushButton("OK", this);
	connect(bOk, SIGNAL(clicked()), this, SLOT(accept()));
	QPushButton *bCancel = new QPushButton("Cancle", this);
	connect(bCancel, SIGNAL(clicked()), this, SLOT(reject()));
	
	QHBoxLayout *resultLayout = new QHBoxLayout;
	mainLayout->addLayout(resultLayout);
	resultLayout->addWidget(bOk);
	resultLayout->addWidget(bCancel);
}

EditOptionsDialog::~EditOptionsDialog()
{
}


void EditOptionsDialog::accept()
{
	if(checkBoxes.displayMode->isChecked())
	{
		myOptions->displayMode = AngelinaApplication::DisplayWindowed;
	}
	else
	{
		myOptions->displayMode = AngelinaApplication::DisplayFullscreen;
	}

	if(checkBoxes.disqualifyMode->isChecked())
	{
		myOptions->disqualifyMode = AngelinaApplication::DisconnectOnDisqualify;
	}
	else
	{
		myOptions->disqualifyMode = AngelinaApplication::MessageOnDisqualify;
	}

	if(checkBoxes.connectionMode->isChecked())
	{
		myOptions->connectionMode = AngelinaApplication::ConnectionAuto;
	}
	else
	{
		myOptions->connectionMode = AngelinaApplication::ConnectionManual;
	}

	if(checkBoxes.robotCount->isChecked())
	{
		myOptions->robotCount = AngelinaApplication::OneRobot;
	}
	else
	{
		myOptions->robotCount = AngelinaApplication::TwoRobot;
	}

	angelinaApp->setOptions(*myOptions);
	QDialog::accept();
}




