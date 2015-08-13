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


#include "angelinaapplication.h"


/*!
 * \mainpage
 *
 *  This is Angelina, the server for the final presentation of the C++-Course.
 *
 *  \section usage Usage:
 *
 * <tt>-f | -file [filename]</tt>	loads teams from <tt>filename</tt>\n
 * <tt>-h | -help</tt>				displays this text
 *
 *  Example team file ./teams.xml
 *
 *  \author Andreas Rittinger (a.rittinger@web.de)
 */

int main(int argc, char *argv[])
{
	bool success;
	AngelinaApplication angelinaApp(argc, argv, &success);
	angelinaApp.setup();

	if(!success)
	{
		return 0;
	}

	
	return angelinaApp.exec();
}

