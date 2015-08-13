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


#ifndef QSINGLETON_H
#define QSINGLETON_H



//! Base for all singletons
/*
 */
template <typename T> class QSingleton
{
	public:

		//! Get or creates an instance
		/*
		 */
		static T* getInstance()
		{
			if(!instance)
			{
				instance = new T;
			}
			return instance;
		}
		
		//! Destroys the instance
		/*
		 */
		static void destroy()
		{
			delete instance;
			instance = 0;
		}




	protected:

		//! Locked constructor
		/*
		 */
		QSingleton()
		{
		}

		//! Locked destructor
		/*
		 */
		virtual ~QSingleton()
		{
		}


		

	private:
		
		//! Locked for copying
		/*
		 */
		QSingleton(const QSingleton &)
		{
		}

		//! The instance
		/*
		 */
		static T *instance;

				
};

template <typename T> T* QSingleton<T>::instance = 0;


//! For singleton creation
#define Q_SINGLETON(CLASS) \
	friend class QSingleton<CLASS>;


#endif