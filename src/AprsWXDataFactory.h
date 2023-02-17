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

public:


	static AprsWXData FromSummaryApiModel(org::openapitools::client::model::Summary & model);

};

#endif /* APRSWXDATAFACTORY_H_ */
