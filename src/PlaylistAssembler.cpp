/*
 * PlaylistAssembler.cpp
 *
 *  Created on: Jan 11, 2023
 *      Author: mateusz
 */

#include "PlaylistAssembler.h"

PlaylistAssembler::PlaylistAssembler(PlaylistSampler & sampler, std::string audioBasePath) : playlistSampler(sampler) {
	// TODO Auto-generated constructor stub

}

PlaylistAssembler::~PlaylistAssembler() {
	// TODO Auto-generated destructor stub
}

PlaylistAssembler::PlaylistAssembler(const PlaylistAssembler &other) : playlistSampler(other.playlistSampler) {
	// TODO Auto-generated constructor stub

}

PlaylistAssembler::PlaylistAssembler(PlaylistAssembler &&other) : playlistSampler(other.playlistSampler) {
	// TODO Auto-generated constructor stub

}

PlaylistAssembler& PlaylistAssembler::operator=(
		const PlaylistAssembler &other) {
	// TODO Auto-generated method stub

}

PlaylistAssembler& PlaylistAssembler::operator=(PlaylistAssembler &&other) {
	// TODO Auto-generated method stub

}

