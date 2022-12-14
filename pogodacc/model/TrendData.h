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
 * TrendData.h
 *
 * 
 */

#ifndef ORG_OPENAPITOOLS_CLIENT_MODEL_TrendData_H_
#define ORG_OPENAPITOOLS_CLIENT_MODEL_TrendData_H_


#include "../ModelBase.h"


namespace org {
namespace openapitools {
namespace client {
namespace model {


/// <summary>
/// 
/// </summary>
class  TrendData
    : public ModelBase
{
public:
    TrendData();
    virtual ~TrendData();

    /////////////////////////////////////////////
    /// ModelBase overrides

    void validate() override;

    web::json::value toJson() const override;
    bool fromJson(const web::json::value& json) override;

    void toMultipart(std::shared_ptr<MultipartFormData> multipart, const utility::string_t& namePrefix) const override;
    bool fromMultiPart(std::shared_ptr<MultipartFormData> multipart, const utility::string_t& namePrefix) override;

    /////////////////////////////////////////////
    /// TrendData members

    /// <summary>
    /// 
    /// </summary>
    float getCurentValue() const;
    bool curentValueIsSet() const;
    void unsetCurent_value();

    void setCurentValue(float value);

    /// <summary>
    /// 
    /// </summary>
    float getTwoHoursValue() const;
    bool twoHoursValueIsSet() const;
    void unsetTwo_hours_value();

    void setTwoHoursValue(float value);

    /// <summary>
    /// 
    /// </summary>
    float getFourHoursValue() const;
    bool fourHoursValueIsSet() const;
    void unsetFour_hours_value();

    void setFourHoursValue(float value);

    /// <summary>
    /// 
    /// </summary>
    float getSixHoursValue() const;
    bool sixHoursValueIsSet() const;
    void unsetSix_hours_value();

    void setSixHoursValue(float value);

    /// <summary>
    /// 
    /// </summary>
    float getEightHoursValue() const;
    bool eightHoursValueIsSet() const;
    void unsetEight_hours_value();

    void setEightHoursValue(float value);


protected:
    float m_Curent_value;
    bool m_Curent_valueIsSet;
    float m_Two_hours_value;
    bool m_Two_hours_valueIsSet;
    float m_Four_hours_value;
    bool m_Four_hours_valueIsSet;
    float m_Six_hours_value;
    bool m_Six_hours_valueIsSet;
    float m_Eight_hours_value;
    bool m_Eight_hours_valueIsSet;
};


}
}
}
}

#endif /* ORG_OPENAPITOOLS_CLIENT_MODEL_TrendData_H_ */
