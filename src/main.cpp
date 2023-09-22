#include <iostream>
#define SDL_MAIN_HANDLED
#include <SDL.h>
extern "C" {
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
}
#include <chrono>
#include <thread>

int main(int argc, char **args) {
    const char* inputFileName = "D:\\java\\1\\ffmpegJava\\src\\main\\resources\\1.mp3"; // Replace with you own file.
    std::this_thread::sleep_for(std::chrono::milliseconds(5));

    // Initialize SDL2 for audio playback
    if (SDL_Init(SDL_INIT_AUDIO) < 0) {
        std::cerr << "SDL_Init error: " << SDL_GetError() << std::endl;
        return 1;
    }

    // Open the MP3 file with FFmpeg
    AVFormatContext* formatContext = nullptr;
    if (avformat_open_input(&formatContext, inputFileName, nullptr, nullptr) < 0) {
        std::cerr << "Failed to open input file" << std::endl;
        return 1;
    }

    if (avformat_find_stream_info(formatContext, nullptr) < 0) {
        std::cerr << "Failed to retrieve stream information" << std::endl;
        avformat_close_input(&formatContext);
        return 1;
    }

    // Find the audio stream
    int audioStreamIndex = -1;
    for (int i = 0; i < formatContext->nb_streams; ++i) {
        if (formatContext->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_AUDIO) {
            audioStreamIndex = i;
            break;
        }
    }

    if (audioStreamIndex == -1) {
        std::cerr << "No audio stream found in the input file" << std::endl;
        return 1;
    }

    // Create an audio codec context and configure it
    AVCodecContext* codecContext = avcodec_alloc_context3(nullptr);
    avcodec_parameters_to_context(codecContext, formatContext->streams[audioStreamIndex]->codecpar);
    AVCodec* codec = const_cast<AVCodec *>(avcodec_find_decoder(codecContext->codec_id));
    if (!codec) {
        std::cerr << "Codec not found" << std::endl;
        return 1;
    }

    if (avcodec_open2(codecContext, codec, nullptr) < 0) {
        std::cerr << "Failed to open codec" << std::endl;
        return 1;
    }

    // Create an SDL2 audio specification
    SDL_AudioSpec wantedSpec;
    SDL_AudioSpec obtainedSpec;

    wantedSpec.freq = codecContext->sample_rate / 2;
    wantedSpec.format = AUDIO_S16SYS;
    wantedSpec.channels = codecContext->channels;
    wantedSpec.silence = 0;
    wantedSpec.samples = 4096;
    wantedSpec.callback = nullptr; // Will be filled later

    // Open the audio device with SDL2
    if (SDL_OpenAudio(&wantedSpec, &obtainedSpec) < 0) {
        std::cerr << "SDL_OpenAudio error: " << SDL_GetError() << std::endl;
        return 1;
    }

    // Prepare the audio packet and frame
    AVPacket packet;
    av_init_packet(&packet);
    AVFrame* frame = av_frame_alloc();

    // Start audio playback
    SDL_PauseAudio(0);

    // Read and play audio packets
    while (av_read_frame(formatContext, &packet) >= 0) {
        if (packet.stream_index == audioStreamIndex) {
            if (avcodec_send_packet(codecContext, &packet) < 0) {
                std::cerr << "avcodec_send_packet error" << std::endl;
                break;
            }

            while (avcodec_receive_frame(codecContext, frame) >= 0) {
                // Send audio data to SDL for playback
                SDL_QueueAudio(1, frame->data[0], frame->linesize[0]);

            }
        }

        av_packet_unref(&packet);
    }

    // Wait for audio playback to finish
    while (SDL_GetQueuedAudioSize(1) > 0) {
        SDL_Delay(100);
    }

    // Cleanup and close the SDL audio device
    SDL_CloseAudio();

    // Free allocated resources
    av_frame_free(&frame);
    avcodec_close(codecContext);
    avformat_close_input(&formatContext);

    // Quit SDL
    SDL_Quit();

    return 0;
}
