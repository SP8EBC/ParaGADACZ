/*
 * PlaylistAssemblerAirspace.h
 *
 *  Created on: Jan 17, 2024
 *      Author: mateusz
 */

#ifndef PLAYLISTASSEMBLERAIRSPACE_H_
#define PLAYLISTASSEMBLERAIRSPACE_H_

#include <memory>

#include "ConfigurationFile.h"
#include "PlaylistSampler.h"


class PlaylistAssemblerAirspace {
public:
	PlaylistAssemblerAirspace(std::shared_ptr<PlaylistSampler> & sampler, std::shared_ptr<ConfigurationFile> & config);
	virtual ~PlaylistAssemblerAirspace();
};

#endif /* PLAYLISTASSEMBLERAIRSPACE_H_ */
