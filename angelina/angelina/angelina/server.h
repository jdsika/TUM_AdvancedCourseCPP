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

#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>

#include "qsingleton.h"

class ResultFrame;

//! Handles incoming connections
/*!
 * The server handles incoming connections and assigns
 * the sockets for communication to the ResultFrames.
 */
class Server: public QTcpServer, public QSingleton<Server>
{
	Q_OBJECT
	Q_SINGLETON(Server)

	public:
		//! Sets the Resultframe the server can assign sockets to
		/*!
		 * \param *frameLeft the left frame
		 * \param *frameRight the right frame
		 */
		void setResultFrames(ResultFrame *frameLeft, ResultFrame *frameRight);

    public slots:
        //! Handles incoming connections
        void newConnection();

    private:
		//! Constructs a Server object
		/*!
		 * Is a singleton parent is set after construction.
		 */
		Server();
		//! The left frame
		ResultFrame *myFrameLeft;
		//! the right frame
		ResultFrame *myFrameRight;
};

#endif /* SERVER_H */
