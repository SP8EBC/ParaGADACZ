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
 * Root.h
 *
 * 
 */

#ifndef ORG_OPENAPITOOLS_CLIENT_MODEL_Root_H_
#define ORG_OPENAPITOOLS_CLIENT_MODEL_Root_H_


#include "../ModelBase.h"

#include <cpprest/details/basic_types.h>
#include "Davis_current_observation.h"
#include "Image.h"

namespace org {
namespace openapitools {
namespace client {
namespace model {


/// <summary>
/// 
/// </summary>
class  Root
    : public ModelBase
{
public:
    Root();
    virtual ~Root();

    /////////////////////////////////////////////
    /// ModelBase overrides

    void validate() override;

    web::json::value toJson() const override;
    bool fromJson(const web::json::value& json) override;

    void toMultipart(std::shared_ptr<MultipartFormData> multipart, const utility::string_t& namePrefix) const override;
    bool fromMultiPart(std::shared_ptr<MultipartFormData> multipart, const utility::string_t& namePrefix) override;

    /////////////////////////////////////////////
    /// Root members

    /// <summary>
    /// 
    /// </summary>
    utility::string_t getCredit() const;
    bool creditIsSet() const;
    void unsetCredit();

    void setCredit(const utility::string_t& value);

    /// <summary>
    /// 
    /// </summary>
    utility::string_t getCreditURL() const;
    bool creditURLIsSet() const;
    void unsetCredit_URL();

    void setCreditURL(const utility::string_t& value);

    /// <summary>
    /// 
    /// </summary>
    utility::string_t getDisclaimerUrl() const;
    bool disclaimerUrlIsSet() const;
    void unsetDisclaimer_url();

    void setDisclaimerUrl(const utility::string_t& value);

    /// <summary>
    /// 
    /// </summary>
    utility::string_t getCopyrightUrl() const;
    bool copyrightUrlIsSet() const;
    void unsetCopyright_url();

    void setCopyrightUrl(const utility::string_t& value);

    /// <summary>
    /// 
    /// </summary>
    utility::string_t getPrivacyPolicyUrl() const;
    bool privacyPolicyUrlIsSet() const;
    void unsetPrivacy_policy_url();

    void setPrivacyPolicyUrl(const utility::string_t& value);

    /// <summary>
    /// 
    /// </summary>
    std::shared_ptr<Image> getImage() const;
    bool imageIsSet() const;
    void unsetImage();

    void setImage(const std::shared_ptr<Image>& value);

    /// <summary>
    /// 
    /// </summary>
    utility::string_t getSuggestedPickup() const;
    bool suggestedPickupIsSet() const;
    void unsetSuggested_pickup();

    void setSuggestedPickup(const utility::string_t& value);

    /// <summary>
    /// 
    /// </summary>
    utility::string_t getSuggestedPickupPeriod() const;
    bool suggestedPickupPeriodIsSet() const;
    void unsetSuggested_pickup_period();

    void setSuggestedPickupPeriod(const utility::string_t& value);

    /// <summary>
    /// 
    /// </summary>
    utility::string_t getDewpointC() const;
    bool dewpointCIsSet() const;
    void unsetDewpoint_c();

    void setDewpointC(const utility::string_t& value);

    /// <summary>
    /// 
    /// </summary>
    utility::string_t getDewpointF() const;
    bool dewpointFIsSet() const;
    void unsetDewpoint_f();

    void setDewpointF(const utility::string_t& value);

    /// <summary>
    /// 
    /// </summary>
    utility::string_t getDewpointString() const;
    bool dewpointStringIsSet() const;
    void unsetDewpoint_string();

    void setDewpointString(const utility::string_t& value);

    /// <summary>
    /// 
    /// </summary>
    utility::string_t getHeatIndexC() const;
    bool heatIndexCIsSet() const;
    void unsetHeat_index_c();

    void setHeatIndexC(const utility::string_t& value);

    /// <summary>
    /// 
    /// </summary>
    utility::string_t getHeatIndexF() const;
    bool heatIndexFIsSet() const;
    void unsetHeat_index_f();

    void setHeatIndexF(const utility::string_t& value);

    /// <summary>
    /// 
    /// </summary>
    utility::string_t getHeatIndexString() const;
    bool heatIndexStringIsSet() const;
    void unsetHeat_index_string();

    void setHeatIndexString(const utility::string_t& value);

    /// <summary>
    /// 
    /// </summary>
    utility::string_t getLocation() const;
    bool locationIsSet() const;
    void unsetLocation();

    void setLocation(const utility::string_t& value);

    /// <summary>
    /// 
    /// </summary>
    utility::string_t getLatitude() const;
    bool latitudeIsSet() const;
    void unsetLatitude();

    void setLatitude(const utility::string_t& value);

    /// <summary>
    /// 
    /// </summary>
    utility::string_t getLongitude() const;
    bool longitudeIsSet() const;
    void unsetLongitude();

    void setLongitude(const utility::string_t& value);

    /// <summary>
    /// 
    /// </summary>
    utility::string_t getObservationTime() const;
    bool observationTimeIsSet() const;
    void unsetObservation_time();

    void setObservationTime(const utility::string_t& value);

    /// <summary>
    /// 
    /// </summary>
    utility::string_t getObservationTimeRfc822() const;
    bool observationTimeRfc822IsSet() const;
    void unsetObservation_time_rfc822();

    void setObservationTimeRfc822(const utility::string_t& value);

    /// <summary>
    /// 
    /// </summary>
    utility::string_t getPressureIn() const;
    bool pressureInIsSet() const;
    void unsetPressure_in();

    void setPressureIn(const utility::string_t& value);

    /// <summary>
    /// 
    /// </summary>
    utility::string_t getPressureMb() const;
    bool pressureMbIsSet() const;
    void unsetPressure_mb();

    void setPressureMb(const utility::string_t& value);

    /// <summary>
    /// 
    /// </summary>
    utility::string_t getPressureString() const;
    bool pressureStringIsSet() const;
    void unsetPressure_string();

    void setPressureString(const utility::string_t& value);

    /// <summary>
    /// 
    /// </summary>
    utility::string_t getRelativeHumidity() const;
    bool relativeHumidityIsSet() const;
    void unsetRelative_humidity();

    void setRelativeHumidity(const utility::string_t& value);

    /// <summary>
    /// 
    /// </summary>
    utility::string_t getStationId() const;
    bool stationIdIsSet() const;
    void unsetStation_id();

    void setStationId(const utility::string_t& value);

    /// <summary>
    /// 
    /// </summary>
    utility::string_t getTempC() const;
    bool tempCIsSet() const;
    void unsetTemp_c();

    void setTempC(const utility::string_t& value);

    /// <summary>
    /// 
    /// </summary>
    utility::string_t getTempF() const;
    bool tempFIsSet() const;
    void unsetTemp_f();

    void setTempF(const utility::string_t& value);

    /// <summary>
    /// 
    /// </summary>
    utility::string_t getTemperatureString() const;
    bool temperatureStringIsSet() const;
    void unsetTemperature_string();

    void setTemperatureString(const utility::string_t& value);

    /// <summary>
    /// 
    /// </summary>
    utility::string_t getWindDegrees() const;
    bool windDegreesIsSet() const;
    void unsetWind_degrees();

    void setWindDegrees(const utility::string_t& value);

    /// <summary>
    /// 
    /// </summary>
    utility::string_t getWindDir() const;
    bool windDirIsSet() const;
    void unsetWind_dir();

    void setWindDir(const utility::string_t& value);

    /// <summary>
    /// 
    /// </summary>
    utility::string_t getWindKt() const;
    bool windKtIsSet() const;
    void unsetWind_kt();

    void setWindKt(const utility::string_t& value);

    /// <summary>
    /// 
    /// </summary>
    utility::string_t getWindMph() const;
    bool windMphIsSet() const;
    void unsetWind_mph();

    void setWindMph(const utility::string_t& value);

    /// <summary>
    /// 
    /// </summary>
    utility::string_t getWindchillC() const;
    bool windchillCIsSet() const;
    void unsetWindchill_c();

    void setWindchillC(const utility::string_t& value);

    /// <summary>
    /// 
    /// </summary>
    utility::string_t getWindchillF() const;
    bool windchillFIsSet() const;
    void unsetWindchill_f();

    void setWindchillF(const utility::string_t& value);

    /// <summary>
    /// 
    /// </summary>
    utility::string_t getWindchillString() const;
    bool windchillStringIsSet() const;
    void unsetWindchill_string();

    void setWindchillString(const utility::string_t& value);

    /// <summary>
    /// 
    /// </summary>
    std::shared_ptr<Davis_current_observation> getDavisCurrentObservation() const;
    bool davisCurrentObservationIsSet() const;
    void unsetDavis_current_observation();

    void setDavisCurrentObservation(const std::shared_ptr<Davis_current_observation>& value);

    /// <summary>
    /// 
    /// </summary>
    utility::string_t getTimeToGenerate() const;
    bool timeToGenerateIsSet() const;
    void unsetTime_to_generate();

    void setTimeToGenerate(const utility::string_t& value);


protected:
    utility::string_t m_Credit;
    bool m_CreditIsSet;
    utility::string_t m_Credit_URL;
    bool m_Credit_URLIsSet;
    utility::string_t m_Disclaimer_url;
    bool m_Disclaimer_urlIsSet;
    utility::string_t m_Copyright_url;
    bool m_Copyright_urlIsSet;
    utility::string_t m_Privacy_policy_url;
    bool m_Privacy_policy_urlIsSet;
    std::shared_ptr<Image> m_Image;
    bool m_ImageIsSet;
    utility::string_t m_Suggested_pickup;
    bool m_Suggested_pickupIsSet;
    utility::string_t m_Suggested_pickup_period;
    bool m_Suggested_pickup_periodIsSet;
    utility::string_t m_Dewpoint_c;
    bool m_Dewpoint_cIsSet;
    utility::string_t m_Dewpoint_f;
    bool m_Dewpoint_fIsSet;
    utility::string_t m_Dewpoint_string;
    bool m_Dewpoint_stringIsSet;
    utility::string_t m_Heat_index_c;
    bool m_Heat_index_cIsSet;
    utility::string_t m_Heat_index_f;
    bool m_Heat_index_fIsSet;
    utility::string_t m_Heat_index_string;
    bool m_Heat_index_stringIsSet;
    utility::string_t m_Location;
    bool m_LocationIsSet;
    utility::string_t m_Latitude;
    bool m_LatitudeIsSet;
    utility::string_t m_Longitude;
    bool m_LongitudeIsSet;
    utility::string_t m_Observation_time;
    bool m_Observation_timeIsSet;
    utility::string_t m_Observation_time_rfc822;
    bool m_Observation_time_rfc822IsSet;
    utility::string_t m_Pressure_in;
    bool m_Pressure_inIsSet;
    utility::string_t m_Pressure_mb;
    bool m_Pressure_mbIsSet;
    utility::string_t m_Pressure_string;
    bool m_Pressure_stringIsSet;
    utility::string_t m_Relative_humidity;
    bool m_Relative_humidityIsSet;
    utility::string_t m_Station_id;
    bool m_Station_idIsSet;
    utility::string_t m_Temp_c;
    bool m_Temp_cIsSet;
    utility::string_t m_Temp_f;
    bool m_Temp_fIsSet;
    utility::string_t m_Temperature_string;
    bool m_Temperature_stringIsSet;
    utility::string_t m_Wind_degrees;
    bool m_Wind_degreesIsSet;
    utility::string_t m_Wind_dir;
    bool m_Wind_dirIsSet;
    utility::string_t m_Wind_kt;
    bool m_Wind_ktIsSet;
    utility::string_t m_Wind_mph;
    bool m_Wind_mphIsSet;
    utility::string_t m_Windchill_c;
    bool m_Windchill_cIsSet;
    utility::string_t m_Windchill_f;
    bool m_Windchill_fIsSet;
    utility::string_t m_Windchill_string;
    bool m_Windchill_stringIsSet;
    std::shared_ptr<Davis_current_observation> m_Davis_current_observation;
    bool m_Davis_current_observationIsSet;
    utility::string_t m_Time_to_generate;
    bool m_Time_to_generateIsSet;
};


}
}
}
}

#endif /* ORG_OPENAPITOOLS_CLIENT_MODEL_Root_H_ */
