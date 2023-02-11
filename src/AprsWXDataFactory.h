/*
 * AprsWXDataFactory.h
 *
 *  Created on: Feb 11, 2023
 *      Author: mateusz
 */

#ifndef APRSWXDATAFACTORY_H_
#define APRSWXDATAFACTORY_H_

#include "AprsWXData.h"

// api
#include "StationApi.h"

class AprsWXDataFactory {
private:
	AprsWXDataFactory();
	virtual ~AprsWXDataFactory();
	AprsWXDataFactory(const AprsWXDataFactory &other);
	AprsWXDataFactory(AprsWXDataFactory &&other);
	AprsWXDataFactory& operator=(const AprsWXDataFactory &other);
	AprsWXDataFactory& operator=(AprsWXDataFactory &&other);

public:


	static AprsWXData FromSummaryApiModel(org::openapitools::client::model::Summary & model);

};

#endif /* APRSWXDATAFACTORY_H_ */
