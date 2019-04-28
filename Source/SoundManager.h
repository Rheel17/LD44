/**
 * Copyright (c) 2019 Levi van Rheenen - All rights reserved
 */

#ifndef SOUNDMANAGER_H_
#define SOUNDMANAGER_H_

#include <AL/alut.h>

#include <map>
#include <memory>
#include <string>
#include <vector>

class SoundManager {

public:
	enum Sound {
		BACKGROUND
	};

	SoundManager();
	~SoundManager();

	// disable copying/moving
	SoundManager(const SoundManager&) = delete;
	SoundManager& operator=(const SoundManager&) = delete;
	SoundManager(SoundManager&&) = delete;
	SoundManager& operator=(SoundManager&&) = delete;

	void PlayBackground();

private:
	void _LoadSounds();
	void _LoadSound(const std::string& filename, Sound sound);

	std::shared_ptr<ALCdevice> _device;
	std::shared_ptr<ALCcontext> _context;

	std::vector<ALuint> _buffers;
	std::map<Sound, ALuint> _sounds;

	ALuint _background_source;

	bool _valid = false;


};

#endif
