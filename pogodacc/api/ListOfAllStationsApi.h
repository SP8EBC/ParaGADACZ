/**
 * Pogoda.cc
 * Backend API specification for pogoda.cc system
 *
 * The version of the OpenAPI document: 1.0.11
 * Contact: sp8ebc@interia.pl
 *
 * NOTE: This class is auto generated by OpenAPI-Generator 4.3.1.
 * https://openapi-generator.tech
 * Do not edit the class manually.
 */

/*
 * ListOfAllStationsApi.h
 *
 * 
 */

#ifndef ORG_OPENAPITOOLS_CLIENT_API_ListOfAllStationsApi_H_
#define ORG_OPENAPITOOLS_CLIENT_API_ListOfAllStationsApi_H_


#include "../ApiClient.h"

#include "StationDefinitionModel.h"


#include <boost/optional.hpp>

namespace org {
namespace openapitools {
namespace client {
namespace api {

using namespace org::openapitools::client::model;



class  ListOfAllStationsApi 
{
public:

    explicit ListOfAllStationsApi( std::shared_ptr<const ApiClient> apiClient );

    virtual ~ListOfAllStationsApi();

    /// <summary>
    /// 
    /// </summary>
    /// <remarks>
    /// 
    /// </remarks>
    pplx::task<std::vector<std::shared_ptr<StationDefinitionModel>>> listOfAllStationsGet(
    ) const;

protected:
    std::shared_ptr<const ApiClient> m_ApiClient;
};

}
}
}
}

#endif /* ORG_OPENAPITOOLS_CLIENT_API_ListOfAllStationsApi_H_ */
