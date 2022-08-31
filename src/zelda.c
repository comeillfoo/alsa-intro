#define _POSIX_SOURCE // otherwise there is timespec redefinition error
#include <alsa/asoundlib.h>
#include <stdio.h>
#include <assert.h>
#include <signal.h>
#include <unistd.h>
#include <stdbool.h>


typedef unsigned int u_int;


static bool is_stop = false;

static void on_sigint() { is_stop = true; }


int main(int argc, char** argv) {
	// Properly handle SIGINT from user
	struct sigaction sa = {};
	sa.sa_handler = on_sigint;
	sigaction(SIGINT, &sa, NULL);

	int error = 0;

	// Attach audio buffer to device
	snd_pcm_t* pcm;
	const char* device_id = "plughw:0,0"; // Use default device
	int mode = SND_PCM_STREAM_PLAYBACK;
	
	error = snd_pcm_open(&pcm, device_id, mode, 0);
	if (error != 0) {
        printf("Access type not available for playback: %s\n", snd_strerror(error));
        return error;
    }

    // Get device property-set
	snd_pcm_hw_params_t* params;
	snd_pcm_hw_params_malloc(&params);
	error = snd_pcm_hw_params_any(pcm, params);
	if (error < 0) {
		snd_pcm_close(pcm);
        printf("Broken configuration for playback: no configurations available: %s\n", snd_strerror(error));
        return error;
	}

	// Specify how we want to access audio data
	int access = SND_PCM_ACCESS_RW_INTERLEAVED;
	error = snd_pcm_hw_params_set_access(pcm, params, access);
	if (error < 0) {
		snd_pcm_hw_params_free(params);
		snd_pcm_close(pcm);
        printf("Access type not available for playback: %s\n", snd_strerror(error));
        return error;
    }

	// Set sample format
	int format = SND_PCM_FORMAT_S16_LE;
	error = snd_pcm_hw_params_set_format(pcm, params, format);
	if (error < 0) {
		snd_pcm_hw_params_free(params);
		snd_pcm_close(pcm);
        printf("Sample format not available for playback: %s\n", snd_strerror(error));
        return error;
    }

	// Set channels
	u_int channels = 2;
	error = snd_pcm_hw_params_set_channels_near(pcm, params, &channels);
	if (error < 0) {
		snd_pcm_hw_params_free(params);
		snd_pcm_close(pcm);
        printf("Channels count (%u) not available for playbacks: %s\n", channels, snd_strerror(error));
        return error;
    }

	// Set sample rate
	u_int sample_rate = 48000;
	u_int requested_rate = sample_rate;
	error = snd_pcm_hw_params_set_rate_near(pcm, params, &sample_rate, 0);
	if (error < 0) {
        printf("Rate %uHz not available for playback: %s\n", requested_rate, snd_strerror(error));
        return error;
    }
    if (requested_rate != sample_rate) {
        printf("Rate doesn't match (requested %uHz, get %iHz)\n", requested_rate, error); // otherwise it returns number of herz
        return -EINVAL;
    }

	fprintf(stderr, "Using format int16, sample rate %u, channels %u\n", sample_rate, channels);

	// Set audio buffer length
	// u_int buffer_length_usec = 500 * 1000;
	// error = snd_pcm_hw_params_set_buffer_time_near(pcm, params, &buffer_length_usec, NULL);
	// if (error < 0) {
    //     printf("Unable to set buffer time %u for playback: %s\n", buffer_length_usec, snd_strerror(error));
    //     return error;
    // }

	// Apply configuration
	error = snd_pcm_hw_params(pcm, params);
	if (error < 0) {
        printf("Unable to set hw params for playback: %s\n", snd_strerror(error));
        return error;
    }

	snd_pcm_uframes_t period_size;
	error = snd_pcm_hw_params_get_period_size(params, &period_size, NULL);
    if (error < 0) {
		snd_pcm_hw_params_free(params);
		snd_pcm_close(pcm);
        printf("Unable to get period size for playback: %s\n", snd_strerror(error));
        return error;
    }

	u_int buf_size = period_size * channels * 2;

	// load melody file
	// TODO: load music file of zelda
	bool is_end_of_file = false;
	FILE* zelda = fopen("zelda.wav", "rb");
	if (zelda == NULL) {
		snd_pcm_hw_params_free(params);
		snd_pcm_close(pcm);
		printf("Unable to find file zelda.wav\n");
		return -EINVAL;
	}

	printf("Using buffer size %u and frames %lu\n", buf_size, period_size);

	while (!is_stop && !is_end_of_file) {
		if (feof(zelda))
			is_end_of_file = true;

		char buffer[buf_size];
		size_t count = fread(buffer, sizeof(char), buf_size, zelda);
		if (count < buf_size) {
			if (ferror(zelda)) {
				fclose(zelda);
				snd_pcm_hw_params_free(params);
				snd_pcm_close(pcm);
				printf("Error while reading file: zelda.wav\n");
				return -1;
			}
		}

		error = snd_pcm_writei(pcm, buffer, period_size);
		if (-EPIPE == error)
			snd_pcm_prepare(pcm);
		else if (error < 0) {
			fclose(zelda);
			snd_pcm_hw_params_free(params);
			snd_pcm_close(pcm);
        	printf("Can't write to PCM device. %s\n", snd_strerror(error));
        	return error;
		}
	}

	fclose(zelda);
	snd_pcm_drain(pcm);
	snd_pcm_close(pcm);
	snd_pcm_hw_params_free(params);
    return 0;
}