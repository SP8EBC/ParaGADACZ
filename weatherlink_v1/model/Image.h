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
 * Image.h
 *
 * 
 */

#ifndef ORG_OPENAPITOOLS_CLIENT_MODEL_Image_H_
#define ORG_OPENAPITOOLS_CLIENT_MODEL_Image_H_


#include "../ModelBase.h"

#include <cpprest/details/basic_types.h>

namespace org {
namespace openapitools {
namespace client {
namespace model {


/// <summary>
/// 
/// </summary>
class  Image
    : public ModelBase
{
public:
    Image();
    virtual ~Image();

    /////////////////////////////////////////////
    /// ModelBase overrides

    void validate() override;

    web::json::value toJson() const override;
    bool fromJson(const web::json::value& json) override;

    void toMultipart(std::shared_ptr<MultipartFormData> multipart, const utility::string_t& namePrefix) const override;
    bool fromMultiPart(std::shared_ptr<MultipartFormData> multipart, const utility::string_t& namePrefix) override;

    /////////////////////////////////////////////
    /// Image members

    /// <summary>
    /// 
    /// </summary>
    utility::string_t getUrl() const;
    bool urlIsSet() const;
    void unsetUrl();

    void setUrl(const utility::string_t& value);

    /// <summary>
    /// 
    /// </summary>
    utility::string_t getTitle() const;
    bool titleIsSet() const;
    void unsetTitle();

    void setTitle(const utility::string_t& value);

    /// <summary>
    /// 
    /// </summary>
    utility::string_t getLink() const;
    bool linkIsSet() const;
    void unsetLink();

    void setLink(const utility::string_t& value);


protected:
    utility::string_t m_Url;
    bool m_UrlIsSet;
    utility::string_t m_Title;
    bool m_TitleIsSet;
    utility::string_t m_Link;
    bool m_LinkIsSet;
};


}
}
}
}

#endif /* ORG_OPENAPITOOLS_CLIENT_MODEL_Image_H_ */