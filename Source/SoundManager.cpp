/**
 * Copyright (c) 2019 Levi van Rheenen - All rights reserved
 */

#include "SoundManager.h"

#include <iostream>

SoundManager::SoundManager() {
	_device = std::shared_ptr<ALCdevice>(alcOpenDevice(nullptr), [](ALCdevice *device) {
		alcCloseDevice(device);
		std::clog << "Audio device closed" << std::endl;
	});

	if (!_device) {
		return;
	}

	_valid = true;
	std::clog << "Audio device opened" << std::endl;

	_context = std::shared_ptr<ALCcontext>(alcCreateContext(_device.get(), nullptr), [](ALCcontext *context) {
		alcMakeContextCurrent(nullptr);
		alcDestroyContext(context);
		std::clog << "Audio context destroyed" << std::endl;
	});

	std::clog << "Audio context created" << std::endl;
	alcMakeContextCurrent(_context.get());

	// create the background source
	alGenSources(1, &_background_source);

	// load all the sounds
	_LoadSounds();
}

SoundManager::~SoundManager() {
	alDeleteSources(1, &_background_source);
	alDeleteBuffers(_buffers.size(), _buffers.data());
}

void SoundManager::PlayBackground() {
	if (!_valid) {
		return;
	}

	std::cout << "play background" << std::endl;

	alSourcei(_background_source, AL_LOOPING, 1);
	alSourcei(_background_source, AL_BUFFER, _sounds[BACKGROUND]);
	alSourcePlay(_background_source);
}

void SoundManager::_LoadSounds() {
	_LoadSound("resources/background.wav", BACKGROUND);
}

void SoundManager::_LoadSound(const std::string& filename, Sound sound) {
	// load the WAV file
	ALenum format;
	void *data;
	ALsizei size, frequency;
	ALboolean loop;
	alutLoadWAVFile(const_cast<char *>(filename.c_str()), &format, &data, &size, &frequency, &loop);

	// create the buffer
	ALuint buffer;
	alGenBuffers(1, &buffer);
	alBufferData(buffer, format, data, size, frequency);

	_buffers.push_back(buffer);
	_sounds[sound] = buffer;

	// delete the temporary buffer
	alutUnloadWAV(format, data, size, frequency);
}
