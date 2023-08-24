/*
 * TimeTools.h
 *
 *  Created on: Apr 8, 2023
 *      Author: mateusz
 */

#ifndef TIMETOOLS_H_
#define TIMETOOLS_H_

#include <boost/date_time.hpp>
#include <boost/date_time/local_time_adjustor.hpp>
#include <boost/date_time/c_local_time_adjustor.hpp>


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

	/**
	 *
	 * @param in ptime to convert to epoch timestamp
	 * @param universalOrLocal set to true if ptime contains time in local timezone instead of UTC
	 */
	static long getEpochFromPtime(boost::posix_time::ptime & in, bool universalOrLocal) {

		// get current local and utc time
		boost::posix_time::ptime currentUniversal = boost::posix_time::second_clock::universal_time();
		boost::posix_time::ptime currentLocal = boost::posix_time::second_clock::local_time();

		// calculate current TZ offset in second
		boost::posix_time::time_duration offset = currentLocal - currentUniversal;
		const long seconds = offset.total_seconds();

		// epoch
		boost::posix_time::ptime epoch = boost::posix_time::ptime(boost::gregorian::date(1970, 1, 1), boost::posix_time::time_duration(0,0,0,0));

		// timestamp
		long ts = (in - epoch).total_seconds();

		// if a input ptime constains local time
		if (universalOrLocal) {
			// but we subtracted it from January 1st 1970 UTC, not a local tz

			// so we need to subtract timezone offset
			ts = ts - seconds;
		}

		return ts;
	}

	TimeTools();
	virtual ~TimeTools();
};

#endif /* TIMETOOLS_H_ */
