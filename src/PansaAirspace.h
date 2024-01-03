/*
 * PansaAirspace.h
 *
 *  Created on: Jan 3, 2024
 *      Author: mateusz
 */
#ifdef PANSA_AIRSPACE

#ifndef PANSAAIRSPACE_H_
#define PANSAAIRSPACE_H_

class PansaAirspace {
public:
	PansaAirspace();
	virtual ~PansaAirspace();

	void connect();

	void disconnect();
};

#endif /* PANSAAIRSPACE_H_ */

#endif
