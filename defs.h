/*
 * Copyright (c) 2015-2016 Stephane Poirier
 *
 * stephane.poirier@oifii.org
 *
 * Stephane Poirier
 * 3532 rue Ste-Famille, #3
 * Montreal, QC, H2X 2L1
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

//#define SAMPLE double	//spi, 2016
#define MY_PI 3.14159265358979
//#define SRATE 44100	//spi, 2016

// Select sample format. 
#if 1
#define PA_SAMPLE_TYPE  paFloat32
typedef float SAMPLE;
#define SAMPLE_SILENCE  (0.0f)
#define PRINTF_S_FORMAT "%.8f"
#elif 1
#define PA_SAMPLE_TYPE  paInt16
typedef short SAMPLE;
#define SAMPLE_SILENCE  (0)
#define PRINTF_S_FORMAT "%d"
#elif 0
#define PA_SAMPLE_TYPE  paInt8
typedef char SAMPLE;
#define SAMPLE_SILENCE  (0)
#define PRINTF_S_FORMAT "%d"
#else
#define PA_SAMPLE_TYPE  paUInt8
typedef unsigned char SAMPLE;
#define SAMPLE_SILENCE  (128)
#define PRINTF_S_FORMAT "%d"
#endif

//#define SAMPLE_RATE  (44100)
#define SAMPLE_RATE  (48000) //remotedroide's m-audio asio driver is fixed to 48kHz 
//2048 latency works
#define FRAMES_PER_BUFFER (2048) //i.e. 2048, 1024, 512, 256, 128, 64 
//4096 seems too big of a latency
//#define FRAMES_PER_BUFFER (4096) //i.e. 4096, 2048, 1024, 512, 256, 128, 64 
//#define NUM_CHANNELS    (1)
#define NUM_CHANNELS    (2)

//2022oct25, spi, begin
/*
#define SRATE (SAMPLE_RATE)	//spi, 2016
*/
//2022oct25, spi, end

