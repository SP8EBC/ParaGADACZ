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



#include "StationGroupStationsStation.h"

namespace org {
namespace openapitools {
namespace client {
namespace model {




StationGroupStationsStation::StationGroupStationsStation()
{
    m_StationId = 0;
    m_StationIdIsSet = false;
    m_StationName = utility::conversions::to_string_t("");
    m_StationNameIsSet = false;
}

StationGroupStationsStation::~StationGroupStationsStation()
{
}

void StationGroupStationsStation::validate()
{
    // TODO: implement validation
}

web::json::value StationGroupStationsStation::toJson() const
{

    web::json::value val = web::json::value::object();
    
    if(m_StationIdIsSet)
    {
        val[utility::conversions::to_string_t("stationId")] = ModelBase::toJson(m_StationId);
    }
    if(m_StationNameIsSet)
    {
        val[utility::conversions::to_string_t("stationName")] = ModelBase::toJson(m_StationName);
    }

    return val;
}

bool StationGroupStationsStation::fromJson(const web::json::value& val)
{
    bool ok = true;
    
    if(val.has_field(utility::conversions::to_string_t("stationId")))
    {
        const web::json::value& fieldValue = val.at(utility::conversions::to_string_t("stationId"));
        if(!fieldValue.is_null())
        {
            int32_t refVal_stationId;
            ok &= ModelBase::fromJson(fieldValue, refVal_stationId);
            setStationId(refVal_stationId);
        }
    }
    if(val.has_field(utility::conversions::to_string_t("stationName")))
    {
        const web::json::value& fieldValue = val.at(utility::conversions::to_string_t("stationName"));
        if(!fieldValue.is_null())
        {
            utility::string_t refVal_stationName;
            ok &= ModelBase::fromJson(fieldValue, refVal_stationName);
            setStationName(refVal_stationName);
        }
    }
    return ok;
}

void StationGroupStationsStation::toMultipart(std::shared_ptr<MultipartFormData> multipart, const utility::string_t& prefix) const
{
    utility::string_t namePrefix = prefix;
    if(namePrefix.size() > 0 && namePrefix.substr(namePrefix.size() - 1) != utility::conversions::to_string_t("."))
    {
        namePrefix += utility::conversions::to_string_t(".");
    }
    if(m_StationIdIsSet)
    {
        multipart->add(ModelBase::toHttpContent(namePrefix + utility::conversions::to_string_t("stationId"), m_StationId));
    }
    if(m_StationNameIsSet)
    {
        multipart->add(ModelBase::toHttpContent(namePrefix + utility::conversions::to_string_t("stationName"), m_StationName));
    }
}

bool StationGroupStationsStation::fromMultiPart(std::shared_ptr<MultipartFormData> multipart, const utility::string_t& prefix)
{
    bool ok = true;
    utility::string_t namePrefix = prefix;
    if(namePrefix.size() > 0 && namePrefix.substr(namePrefix.size() - 1) != utility::conversions::to_string_t("."))
    {
        namePrefix += utility::conversions::to_string_t(".");
    }

    if(multipart->hasContent(utility::conversions::to_string_t("stationId")))
    {
        int32_t refVal_stationId;
        ok &= ModelBase::fromHttpContent(multipart->getContent(utility::conversions::to_string_t("stationId")), refVal_stationId );
        setStationId(refVal_stationId);
    }
    if(multipart->hasContent(utility::conversions::to_string_t("stationName")))
    {
        utility::string_t refVal_stationName;
        ok &= ModelBase::fromHttpContent(multipart->getContent(utility::conversions::to_string_t("stationName")), refVal_stationName );
        setStationName(refVal_stationName);
    }
    return ok;
}

int32_t StationGroupStationsStation::getStationId() const
{
    return m_StationId;
}

void StationGroupStationsStation::setStationId(int32_t value)
{
    m_StationId = value;
    m_StationIdIsSet = true;
}

bool StationGroupStationsStation::stationIdIsSet() const
{
    return m_StationIdIsSet;
}

void StationGroupStationsStation::unsetStationId()
{
    m_StationIdIsSet = false;
}
utility::string_t StationGroupStationsStation::getStationName() const
{
    return m_StationName;
}

void StationGroupStationsStation::setStationName(const utility::string_t& value)
{
    m_StationName = value;
    m_StationNameIsSet = true;
}

bool StationGroupStationsStation::stationNameIsSet() const
{
    return m_StationNameIsSet;
}

void StationGroupStationsStation::unsetStationName()
{
    m_StationNameIsSet = false;
}
}
}
}
}

