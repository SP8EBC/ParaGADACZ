/*
 * TimeTools.h
 *
 *  Created on: Apr 8, 2023
 *      Author: mateusz
 */

#ifndef TIMETOOLS_H_
#define TIMETOOLS_H_

#include <boost/date_time.hpp>


class TimeTools {
public:

	static long getEpoch() {
		// get currrent timestamp
		boost::posix_time::ptime current = boost::posix_time::second_clock::universal_time();

		// epoch
		boost::posix_time::ptime epoch = boost::posix_time::ptime(boost::gregorian::date(1970, 1, 1), boost::posix_time::time_duration(0,0,0,0));

		// timestamp
		long ts = (current - epoch).total_seconds();

		return ts;
	}

	TimeTools();
	virtual ~TimeTools();
};

#endif /* TIMETOOLS_H_ */
