#include <libdragon.h>
#include "../../src/video/profile.h"

#define NUM_DISPLAY   3

void audio_poll(void) {	
	if (audio_can_write()) {    	
		PROFILE_START(PS_AUDIO, 0);
		short *buf = audio_write_begin();
		mixer_poll(buf, audio_get_buffer_length());
		audio_write_end();
		PROFILE_STOP(PS_AUDIO, 0);
	}
}

void video_poll(void) {


}

int main(void) {
	controller_init();
	//debug_init_isviewer();
	//debug_init_usblog();

	const resolution_t RESOLUTION_160x96 = {160, 96, false};

	display_init(RESOLUTION_160x96, DEPTH_16_BPP, NUM_DISPLAY, GAMMA_NONE, ANTIALIAS_RESAMPLE_FETCH_ALWAYS);
	dfs_init(DFS_DEFAULT_LOCATION);
	rdpq_init();

	audio_init(44100, 4);
	mixer_init(4);

	mpeg2_t mp2;
	mpeg2_open(&mp2, "rom:/bbb.m1v");

	wav64_t music;
	wav64_open(&music, "bbb.wav64");

	float fps = mpeg2_get_framerate(&mp2);
	throttle_init(fps, 0, 8);

	mixer_ch_play(0, &music.wave);

	debugf("start\n");
	int nframes = 0;
	display_context_t disp = 0;

	while (1) {
		mixer_throttle(44100.0f / fps);

		if (!mpeg2_next_frame(&mp2))
			break;

		disp = display_get();

		rdpq_attach(disp, NULL);
		//rdpq_attach_clear(disp, NULL);

		mpeg2_draw_frame(&mp2, disp);

		rdpq_detach_show();

		audio_poll();

		nframes++;
		// uint32_t t0 = TICKS_READ();
		// uint32_t t1 = TICKS_READ();
		// if (TICKS_DISTANCE(t0, t1) > TICKS_PER_SECOND && nframes) {
		// 	float fps = (float)nframes / (float)TICKS_DISTANCE(t0,t1) * TICKS_PER_SECOND;
		// 	debugf("FPS: %.2f\n", fps);
		// 	t0 = t1;
		// 	nframes = 0;
		// }

		int ret = throttle_wait();
		if (ret < 0) {
			debugf("videoplayer: frame %d too slow (%d Kcycles)\n", nframes, -ret);
		}

		audio_poll();

		// PROFILE_START(PS_SYNC, 0);
		 rspq_wait();
		// PROFILE_STOP(PS_SYNC, 0);
	}
}