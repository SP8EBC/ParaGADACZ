/*
 * PlaylistAssemblerAirspace.cpp
 *
 *  Created on: Jan 17, 2024
 *      Author: mateusz
 */

#include "PlaylistAssemblerAirspace.h"

#ifdef PANSA_AIRSPACE

#pragma push_macro("U")
#undef U
// pragma required as a workaround of possible conflict with cpprestsdk.
// more info here: https://github.com/fmtlib/fmt/issues/3330
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#pragma pop_macro("U")


PlaylistAssemblerAirspace::PlaylistAssemblerAirspace(std::shared_ptr<PlaylistSampler> & sampler, std::shared_ptr<ConfigurationFile> & config) {
	// TODO Auto-generated constructor stub

}


PlaylistAssemblerAirspace::~PlaylistAssemblerAirspace() {
	// TODO Auto-generated destructor stub
}

void PlaylistAssemblerAirspace::reservationsAroundPoint(
		ConfigurationFile_Airspace_AroundPoint & point,
		const std::map<std::string, PansaAirspace_Zone>& reservations) {

	if (!playlist.has_value()) {
		SPDLOG_ERROR("Existing playlist hasn't been set!");
		return;
	}


}

#endif
