#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stddef.h>
#include <unistd.h>
#include <pulse/simple.h>
#include <pulse/error.h>

#define BUFSIZE 32

#define RATE 44100

//#define RATE 16000

int16_t buffer[BUFSIZE];
int error;
static pa_simple *s = NULL;
static pa_simple *s1 = NULL;
static char name_buf[] = "PulseAudio default device";

/** 
 * Connection initialization: check connectivity and open for recording.
 * 
 * @param sfreq [in] required sampling frequency
 * @param dummy [in] a dummy data
 * 
 * @return TRUE on success, FALSE on failure.
 */
int adin_pulseaudio_standby(int sfreq, void *dummy)
{
  return 0;
}
 
/** 
 * Start recording.
 * @a pathname is dummy.
 *
 * @param arg [in] argument
 * 
 * @return TRUE on success, FALSE on failure.
 */
int adin_pulseaudio_begin()
{
  int error;

  static const pa_sample_spec ss = {
  	.format = PA_SAMPLE_S16LE,
  	.rate = RATE,
  	.channels = 2
  };

  if (!(s = pa_simple_new(NULL, "Julius", PA_STREAM_RECORD, NULL, "record", &ss, NULL, NULL, &error))) {
    printf("Error: adin_pulseaudio: pa_simple_new() failed: %s\n", pa_strerror(error));
    return 1;
  }
  if (!(s1 = pa_simple_new(NULL, "abc", PA_STREAM_PLAYBACK, NULL, "playback", &ss, NULL, NULL, &error))) {
	fprintf(stderr, __FILE__": pa_simple_new() failed: %s\n", pa_strerror(error));
	return 1;
  }
  return 0;
}

/** 
 * Stop recording.
 * 
 * @return TRUE on success, FALSE on failure.
 */
int adin_pulseaudio_end()
{
  if (s != NULL) {
    pa_simple_free(s);
    s = NULL;
  }
  if (s1 != NULL) {
    pa_simple_free(s1);
    s1 = NULL;
  }
  return 0;
}

/**
 * @brief  Read samples from device
 * 
 * Try to read @a sampnum samples and returns actual number of recorded
 * samples currently available.  This function will block until
 * at least one sample was obtained.
 * 
 * @param buf [out] samples obtained in this function
 * @param sampnum [in] wanted number of samples to be read
 * 
 * @return actural number of read samples, -2 if an error occured.
 */
int adin_pulseaudio_read (int16_t *buf, int sampnum)
{
  int error;
  int cnt, bufsize;

  bufsize = sampnum * sizeof(int16_t);
  if (bufsize > BUFSIZE) bufsize = BUFSIZE;

  if (pa_simple_read(s, buf, bufsize, &error) < 0) {
        printf("Error: pa_simple_read() failed: %s\n", pa_strerror(error));
  }
  cnt = bufsize / sizeof(int16_t);
  return (cnt);
}


int main()
{
	adin_pulseaudio_begin();


while(1)
{
	adin_pulseaudio_read(buffer, 32);
	if (pa_simple_write(s1, buffer, 32, &error) < 0) {
	fprintf(stderr, __FILE__": pa_simple_write() failed: %s\n", pa_strerror(error));
	goto finish;
	}
        /* And write it to STDOUT */
        if (write(STDOUT_FILENO, buffer, 32) != 32) {
            fprintf(stderr, __FILE__": write() failed: %s\n", strerror(errno));
            goto finish;
        }
	usleep(5);
}

finish:
	adin_pulseaudio_end();

	return 0;
}
