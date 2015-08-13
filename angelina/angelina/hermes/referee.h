/*!
 * \mainpage
 *
 *
 *
 *  Das ist die Schnittstelle zu Angelina, dem Server. Sie kümmert sich um das Netzwerk
 *  und ermöglicht es euch, Angelina eure Ergebnisse mitzuteilen.	
 *
 *  Einbinden der Referee-Schnittstelle:
 *			-CMake Script für libreferee.a und referee.h (./cmake/FindReferee.cmake)
 *
 *	
 *	  Der Referee kann unabhängig vom Server benutzt werden. Im Verbose-Modus weden die Aktionen via qDebug() auf der
 *   Konsole ausgegeben. Alle Längenangaben sind in Metern!
 *   Das Programm testgui stellt eine beispielhafte Verwendung der Schnittstelle dar.
 *   Bugs in Angelina oder dem Referee bitte an die Mailingliste (cpp-tutor@ldv.ei.tum.de). Wenn möglich mit einer Beschreibung wie man sie
 *   reproduziert.
 *
 * 
 *
 *  \author Andreas Rittinger (a.rittinger@web.de)
 *  \author Tim Habigt (tim@tum.de)
 *  \author Julian Habigt (jh@tum.de)
 *
 */

#ifndef REFEREE_H
#define REFEREE_H

#include <QObject>
#include <QMap>

class Hermes;

//! Enum dient zur Angabe der Farbe des eigenen Teams
enum TeamColor
{
	yellow, blue
};

//! Die Schiedsrichterklasse
/*!
 *	Die Schiedsrichterklasse kommuniziert mit Angelina und dient dazu dem Server die Ergebnisse mitzuteilen
 */
class Referee: public QObject
{
	Q_OBJECT

	public:
		//! Erstellt einen Schiedsrichter
		/*!
		 *	\param teamID Eure teamID (einfach die Nummer eurer CPP Gruppe).
		 *	\param *parent Das Eltern-Objekt.
		 */
		Referee(int teamID, QObject *parent = 0);

		//! Destruktor
		~Referee();

		//! Gibt Meldungen auf der Konsole aus.
		/*!
		 \param enabled Der Ein- Aus-Schalter.
		 */
		void setVerbose(bool enabled);

		//! Fragt ab, ob meldungen auf der Konsole ausgegeben werden.
		bool isVerbose();

		//! Verbindet zum Server Angelina.
		/*!
		 *	\param &ip Die IP-Adresse z.B. 127.0.0.1 (localhost) zum Testen an einem Computer.
		 *	\param port Der Port z.B. 10000.
		 */
		void connectToServer(const QString &ip, int port);

		//! Gibt das Zeichen, dass ihr bereit seid.
		void reportReady();

		//! Gibt das Zeichen, dass ihr Fertig seid bzw. aufhören möchtet  (DONE).
		void reportDone();

		//! Ruft diese Funktion mindestens alle 45 Sekunden auf um Angelina mitzuteilen, dass ihr noch funktioniert ;-).
		void sendAlive();

		//! Sendet das Seitenverhältnis an Angelina.
		/*!
		 * \param ratio Ergebnis von a/b z.B 0.5 (a und b in Metern).
		 */
		void tellAbRatio(double ratio);

		//! Sendet die eigene Farbe.
		/*!
		 * \param color Die Farbe des eigenen Teams (yellow oder blue)
		 */
		void tellTeamColor(TeamColor color);

		//! Teilt Angelina mit, dass ein Tor geschossen wurde
		void reportGoal();

		//! Sendet die eigene Position an Angelina (optional).
		/*!
		 * \param posX Position in a-Richtung in Metern vom linken oberen Spielfeldrand
		 * \param posY Position in b-Richtung in Metern vom linken oberen Spielfeldrand
		 */
		void tellEgoPos(double posX, double posY);

                //! Returns true, wenn die Verbindung zum Server existiert
                bool isConnected();

	Q_SIGNALS:
		//! Wird gesendet falls die Verbindung getrennt wird.
		void disconnected();

		//! Wird gesendet, wenn die Zeit zur Spielfelderkennung beginnt
		void detectionStart();

		//! Wird gesendet wenn der Wettkampf beginnt.
		void gameStart();

		//! Wird gesendet wenn der Wettkampf auf Serverseite beendet wird.
		void gameOver();

		//! Gibt euch die richtigen Seitenlängen.
		void abValues(double a, double b);

		//! Gibt euch die richtige Teamfarbe.
		void trueColorOfTeam(TeamColor color);

		//! Roboter muss seine Bewegung sofort stoppen
		void stopMovement();

                //! Wird gesendet, falls die Verbindung zum Server erfolgreich war.
                void connected();

                //! Wird gesendet, falls die Verbindung zum Server nicht aufgebaut werden konnte.
                void connectFailed();

	private Q_SLOTS:
		void slotRead();
		void slotConnected();
		void slotDisconnected();

	private:
		Hermes *messengerOfTheGods;
		unsigned int wLimit;
		int myTeamID;
		int messageSize;
		bool connection;
		bool testMode;
		bool verbose;
		bool ready;
};

#endif /* REFEREE_H */
