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



#include "QualityFactor.h"

namespace org {
namespace openapitools {
namespace client {
namespace model {



QualityFactor::QualityFactor()
{
}

QualityFactor::~QualityFactor()
{
}

void QualityFactor::validate()
{
    // TODO: implement validation
}

web::json::value QualityFactor::toJson() const
{
    web::json::value val = web::json::value::object();

    
    if (m_value == eQualityFactor::QualityFactor_FULL_) val = web::json::value::string(U("FULL,"));
    if (m_value == eQualityFactor::QualityFactor_DEGRADED_) val = web::json::value::string(U("DEGRADED,"));
    if (m_value == eQualityFactor::QualityFactor_NOT_AVALIABLE_) val = web::json::value::string(U("NOT_AVALIABLE,"));
    if (m_value == eQualityFactor::QualityFactor_NO_DATA) val = web::json::value::string(U("NO_DATA"));

    return val;
}

bool QualityFactor::fromJson(const web::json::value& val)
{
    auto s = val.as_string();

    
    if (s == utility::conversions::to_string_t("FULL,")) m_value = eQualityFactor::QualityFactor_FULL_;
    if (s == utility::conversions::to_string_t("DEGRADED,")) m_value = eQualityFactor::QualityFactor_DEGRADED_;
    if (s == utility::conversions::to_string_t("NOT_AVALIABLE,")) m_value = eQualityFactor::QualityFactor_NOT_AVALIABLE_;
    if (s == utility::conversions::to_string_t("NO_DATA")) m_value = eQualityFactor::QualityFactor_NO_DATA;
    return true;
}

void QualityFactor::toMultipart(std::shared_ptr<MultipartFormData> multipart, const utility::string_t& prefix) const
{
    utility::string_t namePrefix = prefix;
    if(namePrefix.size() > 0 && namePrefix.substr(namePrefix.size() - 1) != utility::conversions::to_string_t("."))
    {
        namePrefix += utility::conversions::to_string_t(".");
    }

    utility::string_t s;

    
    if (m_value == eQualityFactor::QualityFactor_FULL_) s = utility::conversions::to_string_t("FULL,");
    if (m_value == eQualityFactor::QualityFactor_DEGRADED_) s = utility::conversions::to_string_t("DEGRADED,");
    if (m_value == eQualityFactor::QualityFactor_NOT_AVALIABLE_) s = utility::conversions::to_string_t("NOT_AVALIABLE,");
    if (m_value == eQualityFactor::QualityFactor_NO_DATA) s = utility::conversions::to_string_t("NO_DATA");

    multipart->add(ModelBase::toHttpContent(namePrefix, s));
}

bool QualityFactor::fromMultiPart(std::shared_ptr<MultipartFormData> multipart, const utility::string_t& prefix)
{
    bool ok = true;
    utility::string_t namePrefix = prefix;
    if(namePrefix.size() > 0 && namePrefix.substr(namePrefix.size() - 1) != utility::conversions::to_string_t("."))
    {
        namePrefix += utility::conversions::to_string_t(".");
    }
    {
        utility::string_t s;
        ok = ModelBase::fromHttpContent(multipart->getContent(namePrefix), s);
        eQualityFactor v;

        
        if (s == utility::conversions::to_string_t("FULL,")) v = eQualityFactor::QualityFactor_FULL_;
        if (s == utility::conversions::to_string_t("DEGRADED,")) v = eQualityFactor::QualityFactor_DEGRADED_;
        if (s == utility::conversions::to_string_t("NOT_AVALIABLE,")) v = eQualityFactor::QualityFactor_NOT_AVALIABLE_;
        if (s == utility::conversions::to_string_t("NO_DATA")) v = eQualityFactor::QualityFactor_NO_DATA;

        setValue(v);
    }
    return ok;
}

QualityFactor::eQualityFactor QualityFactor::getValue() const
{
   return m_value;
}

void QualityFactor::setValue(QualityFactor::eQualityFactor const value)
{
   m_value = value;
}


}
}
}
}

