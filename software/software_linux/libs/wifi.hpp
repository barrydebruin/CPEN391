/*
 * wifi.h
 *
 *  Created on: Feb 6, 2018
 *      Author: Jingrui
 */

#ifndef WIFI_H_
#define WIFI_H_

#include <stdio.h>
#include <string>
#include <termios.h>

class Wifi {
	public:
		Wifi(const char name[]);

		void LedOn();
		void LedOff();
		void LightOn();
		void LightOff();
		void SendUsername(std::string username, std::string password);
		void SendPicture(std::string picture);
		std::string ReadResponse();

	private:
	 	int wifi_uart;
	 	void SendPicture_part(std::string picture);

};


#endif /* WIFI_H_ */

