/**
 * WeatherLink API v1
 * First version of an API to get weather data from Davis stations
 *
 * The version of the OpenAPI document: 1.0.11
 *
 * NOTE: This class is auto generated by OpenAPI-Generator 4.3.1.
 * https://openapi-generator.tech
 * Do not edit the class manually.
 */

/*
 * ConditionsApi.h
 *
 * 
 */

#ifndef ORG_OPENAPITOOLS_CLIENT_API_ConditionsApi_H_
#define ORG_OPENAPITOOLS_CLIENT_API_ConditionsApi_H_


#include "../ApiClient.h"

#include "Root.h"
#include <cpprest/details/basic_types.h>


#include <boost/optional.hpp>

namespace org {
namespace openapitools {
namespace client {
namespace api {

using namespace org::openapitools::client::model;



class  ConditionsApi 
{
public:

    explicit ConditionsApi( std::shared_ptr<const ApiClient> apiClient );

    virtual ~ConditionsApi();

    /// <summary>
    /// Current Conditions
    /// </summary>
    /// <remarks>
    /// ...
    /// </remarks>
    /// <param name="user">This is a Device ID from &gt;&gt;Device Info&lt;&lt; subpage on user account</param>
    /// <param name="pass">Password uset to login to weatherlink.com</param>
    /// <param name="token">Password uset to login to weatherlink.com</param>
    pplx::task<std::shared_ptr<Root>> noaaExtJsonGet(
        utility::string_t user,
        utility::string_t pass,
        utility::string_t token
    ) const;

protected:
    std::shared_ptr<const ApiClient> m_ApiClient;
};

}
}
}
}

#endif /* ORG_OPENAPITOOLS_CLIENT_API_ConditionsApi_H_ */
