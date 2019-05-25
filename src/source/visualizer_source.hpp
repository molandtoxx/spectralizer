/**
 * This file is part of spectralizer
 * which is licensed under the GPL v2.0
 * See LICENSE or http://www.gnu.org/licenses
 * github.com/univrsal/spectralizer
 */
#pragma once

#include <cstdint>
#include <obs-module.h>
#include "../util/util.hpp"

namespace audio {
    class audio_visualizer;
}

namespace source {

enum filter_mode {
    FILTER_NONE,
    FILTER_MCAT,
    FILTER_WAVES
};

struct config {
    visual_mode mode = VISUAL_BARS;
    /* Detail: Amount of bars / nodes in wire path */
    uint16_t detail = 32, cx = 50, cy = 50;
    uint16_t fps = 30;
    float refresh_rate = 1.f / fps;
    float refresh_counter = 0.f;
    bool stereo = false;
    obs_source_t* source = nullptr;
    obs_data_t* settings = nullptr;
    uint32_t sample_rate = 44100;
    uint32_t color;
    uint16_t audio_source = 0; /* Audio source id, 0 is fifo */
    source::filter_mode filter_mode = source::FILTER_NONE;
    bool auto_sens = false;
    const char* fifo_path = nullptr;

    /* Bar visualizer_source settings */
    float gravity = 1.f, integral = 10.f / 100, sens = 100.f;
    uint16_t bar_space = 2;
    uint16_t bar_width = 5;
    uint16_t bar_height = 100;
    double mcat_strength = 0; /* Used in monstercat filter */

    double ignore = 0; /* Cut off lower peaks */
    /* Frequency cutting */
    uint16_t freq_cutoff_low = 50, freq_cutoff_high = 10000;
    uint8_t buffer_size = 16; /* Only values 8 - 16, will be squared in audio_processor */
};

class visualizer_source
{
    config m_config;
    audio::audio_visualizer* m_visualizer = nullptr;
public:
    visualizer_source(obs_source_t* source, obs_data_t* settings);
    ~visualizer_source();

    inline void update(obs_data_t* settings);
    inline void tick(float seconds);
    inline void render(gs_effect_t* effect);

    uint32_t get_width() const
    { return m_config.cx; }

    uint32_t get_height() const
    { return m_config.cy; }
};

/* Util for registering the source */
static obs_properties_t* get_properties_for_visualiser(void* data);

void register_visualiser();
void cleanup();
}