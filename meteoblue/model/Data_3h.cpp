/**
 * Meteoblue Package API - day & 3h
 * This yaml contains a description of basic_day and basic_3h from Package API provided by Meteoblue
 *
 * The version of the OpenAPI document: 1.0.11
 *
 * NOTE: This class is auto generated by OpenAPI-Generator 4.3.1.
 * https://openapi-generator.tech
 * Do not edit the class manually.
 */



#include "Data_3h.h"

namespace org {
namespace openapitools {
namespace client {
namespace model {




Data_3h::Data_3h()
{
    m_RainspotIsSet = false;
    m_TimeIsSet = false;
    m_PictocodeIsSet = false;
    m_PrecipitationIsSet = false;
    m_SnowfractionIsSet = false;
    m_Precipitation_probabilityIsSet = false;
    m_TemperatureIsSet = false;
    m_FelttemperatureIsSet = false;
    m_WindspeedIsSet = false;
    m_WinddirectionIsSet = false;
    m_RelativehumidityIsSet = false;
    m_SealevelpressureIsSet = false;
    m_Convective_precipitationIsSet = false;
    m_IsdaylightIsSet = false;
    m_UvindexIsSet = false;
}

Data_3h::~Data_3h()
{
}

void Data_3h::validate()
{
    // TODO: implement validation
}

web::json::value Data_3h::toJson() const
{

    web::json::value val = web::json::value::object();
    
    if(m_RainspotIsSet)
    {
        val[utility::conversions::to_string_t("rainspot")] = ModelBase::toJson(m_Rainspot);
    }
    if(m_TimeIsSet)
    {
        val[utility::conversions::to_string_t("time")] = ModelBase::toJson(m_Time);
    }
    if(m_PictocodeIsSet)
    {
        val[utility::conversions::to_string_t("pictocode")] = ModelBase::toJson(m_Pictocode);
    }
    if(m_PrecipitationIsSet)
    {
        val[utility::conversions::to_string_t("precipitation")] = ModelBase::toJson(m_Precipitation);
    }
    if(m_SnowfractionIsSet)
    {
        val[utility::conversions::to_string_t("snowfraction")] = ModelBase::toJson(m_Snowfraction);
    }
    if(m_Precipitation_probabilityIsSet)
    {
        val[utility::conversions::to_string_t("precipitation_probability")] = ModelBase::toJson(m_Precipitation_probability);
    }
    if(m_TemperatureIsSet)
    {
        val[utility::conversions::to_string_t("temperature")] = ModelBase::toJson(m_Temperature);
    }
    if(m_FelttemperatureIsSet)
    {
        val[utility::conversions::to_string_t("felttemperature")] = ModelBase::toJson(m_Felttemperature);
    }
    if(m_WindspeedIsSet)
    {
        val[utility::conversions::to_string_t("windspeed")] = ModelBase::toJson(m_Windspeed);
    }
    if(m_WinddirectionIsSet)
    {
        val[utility::conversions::to_string_t("winddirection")] = ModelBase::toJson(m_Winddirection);
    }
    if(m_RelativehumidityIsSet)
    {
        val[utility::conversions::to_string_t("relativehumidity")] = ModelBase::toJson(m_Relativehumidity);
    }
    if(m_SealevelpressureIsSet)
    {
        val[utility::conversions::to_string_t("sealevelpressure")] = ModelBase::toJson(m_Sealevelpressure);
    }
    if(m_Convective_precipitationIsSet)
    {
        val[utility::conversions::to_string_t("convective_precipitation")] = ModelBase::toJson(m_Convective_precipitation);
    }
    if(m_IsdaylightIsSet)
    {
        val[utility::conversions::to_string_t("isdaylight")] = ModelBase::toJson(m_Isdaylight);
    }
    if(m_UvindexIsSet)
    {
        val[utility::conversions::to_string_t("uvindex")] = ModelBase::toJson(m_Uvindex);
    }

    return val;
}

bool Data_3h::fromJson(const web::json::value& val)
{
    bool ok = true;
    
    if(val.has_field(utility::conversions::to_string_t("rainspot")))
    {
        const web::json::value& fieldValue = val.at(utility::conversions::to_string_t("rainspot"));
        if(!fieldValue.is_null())
        {
            std::vector<utility::string_t> refVal_rainspot;
            ok &= ModelBase::fromJson(fieldValue, refVal_rainspot);
            setRainspot(refVal_rainspot);
        }
    }
    if(val.has_field(utility::conversions::to_string_t("time")))
    {
        const web::json::value& fieldValue = val.at(utility::conversions::to_string_t("time"));
        if(!fieldValue.is_null())
        {
            std::vector<int32_t> refVal_time;
            ok &= ModelBase::fromJson(fieldValue, refVal_time);
            setTime(refVal_time);
        }
    }
    if(val.has_field(utility::conversions::to_string_t("pictocode")))
    {
        const web::json::value& fieldValue = val.at(utility::conversions::to_string_t("pictocode"));
        if(!fieldValue.is_null())
        {
            std::vector<int32_t> refVal_pictocode;
            ok &= ModelBase::fromJson(fieldValue, refVal_pictocode);
            setPictocode(refVal_pictocode);
        }
    }
    if(val.has_field(utility::conversions::to_string_t("precipitation")))
    {
        const web::json::value& fieldValue = val.at(utility::conversions::to_string_t("precipitation"));
        if(!fieldValue.is_null())
        {
            std::vector<float> refVal_precipitation;
            ok &= ModelBase::fromJson(fieldValue, refVal_precipitation);
            setPrecipitation(refVal_precipitation);
        }
    }
    if(val.has_field(utility::conversions::to_string_t("snowfraction")))
    {
        const web::json::value& fieldValue = val.at(utility::conversions::to_string_t("snowfraction"));
        if(!fieldValue.is_null())
        {
            std::vector<float> refVal_snowfraction;
            ok &= ModelBase::fromJson(fieldValue, refVal_snowfraction);
            setSnowfraction(refVal_snowfraction);
        }
    }
    if(val.has_field(utility::conversions::to_string_t("precipitation_probability")))
    {
        const web::json::value& fieldValue = val.at(utility::conversions::to_string_t("precipitation_probability"));
        if(!fieldValue.is_null())
        {
            std::vector<int32_t> refVal_precipitation_probability;
            ok &= ModelBase::fromJson(fieldValue, refVal_precipitation_probability);
            setPrecipitationProbability(refVal_precipitation_probability);
        }
    }
    if(val.has_field(utility::conversions::to_string_t("temperature")))
    {
        const web::json::value& fieldValue = val.at(utility::conversions::to_string_t("temperature"));
        if(!fieldValue.is_null())
        {
            std::vector<float> refVal_temperature;
            ok &= ModelBase::fromJson(fieldValue, refVal_temperature);
            setTemperature(refVal_temperature);
        }
    }
    if(val.has_field(utility::conversions::to_string_t("felttemperature")))
    {
        const web::json::value& fieldValue = val.at(utility::conversions::to_string_t("felttemperature"));
        if(!fieldValue.is_null())
        {
            std::vector<float> refVal_felttemperature;
            ok &= ModelBase::fromJson(fieldValue, refVal_felttemperature);
            setFelttemperature(refVal_felttemperature);
        }
    }
    if(val.has_field(utility::conversions::to_string_t("windspeed")))
    {
        const web::json::value& fieldValue = val.at(utility::conversions::to_string_t("windspeed"));
        if(!fieldValue.is_null())
        {
            std::vector<float> refVal_windspeed;
            ok &= ModelBase::fromJson(fieldValue, refVal_windspeed);
            setWindspeed(refVal_windspeed);
        }
    }
    if(val.has_field(utility::conversions::to_string_t("winddirection")))
    {
        const web::json::value& fieldValue = val.at(utility::conversions::to_string_t("winddirection"));
        if(!fieldValue.is_null())
        {
            std::vector<int32_t> refVal_winddirection;
            ok &= ModelBase::fromJson(fieldValue, refVal_winddirection);
            setWinddirection(refVal_winddirection);
        }
    }
    if(val.has_field(utility::conversions::to_string_t("relativehumidity")))
    {
        const web::json::value& fieldValue = val.at(utility::conversions::to_string_t("relativehumidity"));
        if(!fieldValue.is_null())
        {
            std::vector<int32_t> refVal_relativehumidity;
            ok &= ModelBase::fromJson(fieldValue, refVal_relativehumidity);
            setRelativehumidity(refVal_relativehumidity);
        }
    }
    if(val.has_field(utility::conversions::to_string_t("sealevelpressure")))
    {
        const web::json::value& fieldValue = val.at(utility::conversions::to_string_t("sealevelpressure"));
        if(!fieldValue.is_null())
        {
            std::vector<float> refVal_sealevelpressure;
            ok &= ModelBase::fromJson(fieldValue, refVal_sealevelpressure);
            setSealevelpressure(refVal_sealevelpressure);
        }
    }
    if(val.has_field(utility::conversions::to_string_t("convective_precipitation")))
    {
        const web::json::value& fieldValue = val.at(utility::conversions::to_string_t("convective_precipitation"));
        if(!fieldValue.is_null())
        {
            std::vector<int32_t> refVal_convective_precipitation;
            ok &= ModelBase::fromJson(fieldValue, refVal_convective_precipitation);
            setConvectivePrecipitation(refVal_convective_precipitation);
        }
    }
    if(val.has_field(utility::conversions::to_string_t("isdaylight")))
    {
        const web::json::value& fieldValue = val.at(utility::conversions::to_string_t("isdaylight"));
        if(!fieldValue.is_null())
        {
            std::vector<int32_t> refVal_isdaylight;
            ok &= ModelBase::fromJson(fieldValue, refVal_isdaylight);
            setIsdaylight(refVal_isdaylight);
        }
    }
    if(val.has_field(utility::conversions::to_string_t("uvindex")))
    {
        const web::json::value& fieldValue = val.at(utility::conversions::to_string_t("uvindex"));
        if(!fieldValue.is_null())
        {
            std::vector<int32_t> refVal_uvindex;
            ok &= ModelBase::fromJson(fieldValue, refVal_uvindex);
            setUvindex(refVal_uvindex);
        }
    }
    return ok;
}

void Data_3h::toMultipart(std::shared_ptr<MultipartFormData> multipart, const utility::string_t& prefix) const
{
    utility::string_t namePrefix = prefix;
    if(namePrefix.size() > 0 && namePrefix.substr(namePrefix.size() - 1) != utility::conversions::to_string_t("."))
    {
        namePrefix += utility::conversions::to_string_t(".");
    }
    if(m_RainspotIsSet)
    {
        multipart->add(ModelBase::toHttpContent(namePrefix + utility::conversions::to_string_t("rainspot"), m_Rainspot));
    }
    if(m_TimeIsSet)
    {
        multipart->add(ModelBase::toHttpContent(namePrefix + utility::conversions::to_string_t("time"), m_Time));
    }
    if(m_PictocodeIsSet)
    {
        multipart->add(ModelBase::toHttpContent(namePrefix + utility::conversions::to_string_t("pictocode"), m_Pictocode));
    }
    if(m_PrecipitationIsSet)
    {
        multipart->add(ModelBase::toHttpContent(namePrefix + utility::conversions::to_string_t("precipitation"), m_Precipitation));
    }
    if(m_SnowfractionIsSet)
    {
        multipart->add(ModelBase::toHttpContent(namePrefix + utility::conversions::to_string_t("snowfraction"), m_Snowfraction));
    }
    if(m_Precipitation_probabilityIsSet)
    {
        multipart->add(ModelBase::toHttpContent(namePrefix + utility::conversions::to_string_t("precipitation_probability"), m_Precipitation_probability));
    }
    if(m_TemperatureIsSet)
    {
        multipart->add(ModelBase::toHttpContent(namePrefix + utility::conversions::to_string_t("temperature"), m_Temperature));
    }
    if(m_FelttemperatureIsSet)
    {
        multipart->add(ModelBase::toHttpContent(namePrefix + utility::conversions::to_string_t("felttemperature"), m_Felttemperature));
    }
    if(m_WindspeedIsSet)
    {
        multipart->add(ModelBase::toHttpContent(namePrefix + utility::conversions::to_string_t("windspeed"), m_Windspeed));
    }
    if(m_WinddirectionIsSet)
    {
        multipart->add(ModelBase::toHttpContent(namePrefix + utility::conversions::to_string_t("winddirection"), m_Winddirection));
    }
    if(m_RelativehumidityIsSet)
    {
        multipart->add(ModelBase::toHttpContent(namePrefix + utility::conversions::to_string_t("relativehumidity"), m_Relativehumidity));
    }
    if(m_SealevelpressureIsSet)
    {
        multipart->add(ModelBase::toHttpContent(namePrefix + utility::conversions::to_string_t("sealevelpressure"), m_Sealevelpressure));
    }
    if(m_Convective_precipitationIsSet)
    {
        multipart->add(ModelBase::toHttpContent(namePrefix + utility::conversions::to_string_t("convective_precipitation"), m_Convective_precipitation));
    }
    if(m_IsdaylightIsSet)
    {
        multipart->add(ModelBase::toHttpContent(namePrefix + utility::conversions::to_string_t("isdaylight"), m_Isdaylight));
    }
    if(m_UvindexIsSet)
    {
        multipart->add(ModelBase::toHttpContent(namePrefix + utility::conversions::to_string_t("uvindex"), m_Uvindex));
    }
}

bool Data_3h::fromMultiPart(std::shared_ptr<MultipartFormData> multipart, const utility::string_t& prefix)
{
    bool ok = true;
    utility::string_t namePrefix = prefix;
    if(namePrefix.size() > 0 && namePrefix.substr(namePrefix.size() - 1) != utility::conversions::to_string_t("."))
    {
        namePrefix += utility::conversions::to_string_t(".");
    }

    if(multipart->hasContent(utility::conversions::to_string_t("rainspot")))
    {
        std::vector<utility::string_t> refVal_rainspot;
        ok &= ModelBase::fromHttpContent(multipart->getContent(utility::conversions::to_string_t("rainspot")), refVal_rainspot );
        setRainspot(refVal_rainspot);
    }
    if(multipart->hasContent(utility::conversions::to_string_t("time")))
    {
        std::vector<int32_t> refVal_time;
        ok &= ModelBase::fromHttpContent(multipart->getContent(utility::conversions::to_string_t("time")), refVal_time );
        setTime(refVal_time);
    }
    if(multipart->hasContent(utility::conversions::to_string_t("pictocode")))
    {
        std::vector<int32_t> refVal_pictocode;
        ok &= ModelBase::fromHttpContent(multipart->getContent(utility::conversions::to_string_t("pictocode")), refVal_pictocode );
        setPictocode(refVal_pictocode);
    }
    if(multipart->hasContent(utility::conversions::to_string_t("precipitation")))
    {
        std::vector<float> refVal_precipitation;
        ok &= ModelBase::fromHttpContent(multipart->getContent(utility::conversions::to_string_t("precipitation")), refVal_precipitation );
        setPrecipitation(refVal_precipitation);
    }
    if(multipart->hasContent(utility::conversions::to_string_t("snowfraction")))
    {
        std::vector<float> refVal_snowfraction;
        ok &= ModelBase::fromHttpContent(multipart->getContent(utility::conversions::to_string_t("snowfraction")), refVal_snowfraction );
        setSnowfraction(refVal_snowfraction);
    }
    if(multipart->hasContent(utility::conversions::to_string_t("precipitation_probability")))
    {
        std::vector<int32_t> refVal_precipitation_probability;
        ok &= ModelBase::fromHttpContent(multipart->getContent(utility::conversions::to_string_t("precipitation_probability")), refVal_precipitation_probability );
        setPrecipitationProbability(refVal_precipitation_probability);
    }
    if(multipart->hasContent(utility::conversions::to_string_t("temperature")))
    {
        std::vector<float> refVal_temperature;
        ok &= ModelBase::fromHttpContent(multipart->getContent(utility::conversions::to_string_t("temperature")), refVal_temperature );
        setTemperature(refVal_temperature);
    }
    if(multipart->hasContent(utility::conversions::to_string_t("felttemperature")))
    {
        std::vector<float> refVal_felttemperature;
        ok &= ModelBase::fromHttpContent(multipart->getContent(utility::conversions::to_string_t("felttemperature")), refVal_felttemperature );
        setFelttemperature(refVal_felttemperature);
    }
    if(multipart->hasContent(utility::conversions::to_string_t("windspeed")))
    {
        std::vector<float> refVal_windspeed;
        ok &= ModelBase::fromHttpContent(multipart->getContent(utility::conversions::to_string_t("windspeed")), refVal_windspeed );
        setWindspeed(refVal_windspeed);
    }
    if(multipart->hasContent(utility::conversions::to_string_t("winddirection")))
    {
        std::vector<int32_t> refVal_winddirection;
        ok &= ModelBase::fromHttpContent(multipart->getContent(utility::conversions::to_string_t("winddirection")), refVal_winddirection );
        setWinddirection(refVal_winddirection);
    }
    if(multipart->hasContent(utility::conversions::to_string_t("relativehumidity")))
    {
        std::vector<int32_t> refVal_relativehumidity;
        ok &= ModelBase::fromHttpContent(multipart->getContent(utility::conversions::to_string_t("relativehumidity")), refVal_relativehumidity );
        setRelativehumidity(refVal_relativehumidity);
    }
    if(multipart->hasContent(utility::conversions::to_string_t("sealevelpressure")))
    {
        std::vector<float> refVal_sealevelpressure;
        ok &= ModelBase::fromHttpContent(multipart->getContent(utility::conversions::to_string_t("sealevelpressure")), refVal_sealevelpressure );
        setSealevelpressure(refVal_sealevelpressure);
    }
    if(multipart->hasContent(utility::conversions::to_string_t("convective_precipitation")))
    {
        std::vector<int32_t> refVal_convective_precipitation;
        ok &= ModelBase::fromHttpContent(multipart->getContent(utility::conversions::to_string_t("convective_precipitation")), refVal_convective_precipitation );
        setConvectivePrecipitation(refVal_convective_precipitation);
    }
    if(multipart->hasContent(utility::conversions::to_string_t("isdaylight")))
    {
        std::vector<int32_t> refVal_isdaylight;
        ok &= ModelBase::fromHttpContent(multipart->getContent(utility::conversions::to_string_t("isdaylight")), refVal_isdaylight );
        setIsdaylight(refVal_isdaylight);
    }
    if(multipart->hasContent(utility::conversions::to_string_t("uvindex")))
    {
        std::vector<int32_t> refVal_uvindex;
        ok &= ModelBase::fromHttpContent(multipart->getContent(utility::conversions::to_string_t("uvindex")), refVal_uvindex );
        setUvindex(refVal_uvindex);
    }
    return ok;
}

std::vector<utility::string_t>& Data_3h::getRainspot()
{
    return m_Rainspot;
}

void Data_3h::setRainspot(const std::vector<utility::string_t>& value)
{
    m_Rainspot = value;
    m_RainspotIsSet = true;
}

bool Data_3h::rainspotIsSet() const
{
    return m_RainspotIsSet;
}

void Data_3h::unsetRainspot()
{
    m_RainspotIsSet = false;
}
std::vector<int32_t>& Data_3h::getTime()
{
    return m_Time;
}

void Data_3h::setTime(std::vector<int32_t> value)
{
    m_Time = value;
    m_TimeIsSet = true;
}

bool Data_3h::timeIsSet() const
{
    return m_TimeIsSet;
}

void Data_3h::unsetTime()
{
    m_TimeIsSet = false;
}
std::vector<int32_t>& Data_3h::getPictocode()
{
    return m_Pictocode;
}

void Data_3h::setPictocode(std::vector<int32_t> value)
{
    m_Pictocode = value;
    m_PictocodeIsSet = true;
}

bool Data_3h::pictocodeIsSet() const
{
    return m_PictocodeIsSet;
}

void Data_3h::unsetPictocode()
{
    m_PictocodeIsSet = false;
}
std::vector<float>& Data_3h::getPrecipitation()
{
    return m_Precipitation;
}

void Data_3h::setPrecipitation(std::vector<float> value)
{
    m_Precipitation = value;
    m_PrecipitationIsSet = true;
}

bool Data_3h::precipitationIsSet() const
{
    return m_PrecipitationIsSet;
}

void Data_3h::unsetPrecipitation()
{
    m_PrecipitationIsSet = false;
}
std::vector<float>& Data_3h::getSnowfraction()
{
    return m_Snowfraction;
}

void Data_3h::setSnowfraction(std::vector<float> value)
{
    m_Snowfraction = value;
    m_SnowfractionIsSet = true;
}

bool Data_3h::snowfractionIsSet() const
{
    return m_SnowfractionIsSet;
}

void Data_3h::unsetSnowfraction()
{
    m_SnowfractionIsSet = false;
}
std::vector<int32_t>& Data_3h::getPrecipitationProbability()
{
    return m_Precipitation_probability;
}

void Data_3h::setPrecipitationProbability(std::vector<int32_t> value)
{
    m_Precipitation_probability = value;
    m_Precipitation_probabilityIsSet = true;
}

bool Data_3h::precipitationProbabilityIsSet() const
{
    return m_Precipitation_probabilityIsSet;
}

void Data_3h::unsetPrecipitation_probability()
{
    m_Precipitation_probabilityIsSet = false;
}
std::vector<float>& Data_3h::getTemperature()
{
    return m_Temperature;
}

void Data_3h::setTemperature(std::vector<float> value)
{
    m_Temperature = value;
    m_TemperatureIsSet = true;
}

bool Data_3h::temperatureIsSet() const
{
    return m_TemperatureIsSet;
}

void Data_3h::unsetTemperature()
{
    m_TemperatureIsSet = false;
}
std::vector<float>& Data_3h::getFelttemperature()
{
    return m_Felttemperature;
}

void Data_3h::setFelttemperature(std::vector<float> value)
{
    m_Felttemperature = value;
    m_FelttemperatureIsSet = true;
}

bool Data_3h::felttemperatureIsSet() const
{
    return m_FelttemperatureIsSet;
}

void Data_3h::unsetFelttemperature()
{
    m_FelttemperatureIsSet = false;
}
std::vector<float>& Data_3h::getWindspeed()
{
    return m_Windspeed;
}

void Data_3h::setWindspeed(std::vector<float> value)
{
    m_Windspeed = value;
    m_WindspeedIsSet = true;
}

bool Data_3h::windspeedIsSet() const
{
    return m_WindspeedIsSet;
}

void Data_3h::unsetWindspeed()
{
    m_WindspeedIsSet = false;
}
std::vector<int32_t>& Data_3h::getWinddirection()
{
    return m_Winddirection;
}

void Data_3h::setWinddirection(std::vector<int32_t> value)
{
    m_Winddirection = value;
    m_WinddirectionIsSet = true;
}

bool Data_3h::winddirectionIsSet() const
{
    return m_WinddirectionIsSet;
}

void Data_3h::unsetWinddirection()
{
    m_WinddirectionIsSet = false;
}
std::vector<int32_t>& Data_3h::getRelativehumidity()
{
    return m_Relativehumidity;
}

void Data_3h::setRelativehumidity(std::vector<int32_t> value)
{
    m_Relativehumidity = value;
    m_RelativehumidityIsSet = true;
}

bool Data_3h::relativehumidityIsSet() const
{
    return m_RelativehumidityIsSet;
}

void Data_3h::unsetRelativehumidity()
{
    m_RelativehumidityIsSet = false;
}
std::vector<float>& Data_3h::getSealevelpressure()
{
    return m_Sealevelpressure;
}

void Data_3h::setSealevelpressure(std::vector<float> value)
{
    m_Sealevelpressure = value;
    m_SealevelpressureIsSet = true;
}

bool Data_3h::sealevelpressureIsSet() const
{
    return m_SealevelpressureIsSet;
}

void Data_3h::unsetSealevelpressure()
{
    m_SealevelpressureIsSet = false;
}
std::vector<int32_t>& Data_3h::getConvectivePrecipitation()
{
    return m_Convective_precipitation;
}

void Data_3h::setConvectivePrecipitation(std::vector<int32_t> value)
{
    m_Convective_precipitation = value;
    m_Convective_precipitationIsSet = true;
}

bool Data_3h::convectivePrecipitationIsSet() const
{
    return m_Convective_precipitationIsSet;
}

void Data_3h::unsetConvective_precipitation()
{
    m_Convective_precipitationIsSet = false;
}
std::vector<int32_t>& Data_3h::getIsdaylight()
{
    return m_Isdaylight;
}

void Data_3h::setIsdaylight(std::vector<int32_t> value)
{
    m_Isdaylight = value;
    m_IsdaylightIsSet = true;
}

bool Data_3h::isdaylightIsSet() const
{
    return m_IsdaylightIsSet;
}

void Data_3h::unsetIsdaylight()
{
    m_IsdaylightIsSet = false;
}
std::vector<int32_t>& Data_3h::getUvindex()
{
    return m_Uvindex;
}

void Data_3h::setUvindex(std::vector<int32_t> value)
{
    m_Uvindex = value;
    m_UvindexIsSet = true;
}

bool Data_3h::uvindexIsSet() const
{
    return m_UvindexIsSet;
}

void Data_3h::unsetUvindex()
{
    m_UvindexIsSet = false;
}
}
}
}
}

