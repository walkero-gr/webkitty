#pragma once

#if ENABLE(VIDEO)

#include <wtf/Function.h>

#define EP_PROFILING 0
#include <libeventprofiler.h>

namespace WebCore {

class NetworkingContext;
class MediaPlayer;
class Page;

struct MediaPlayerAmigaOSInfoTrack
{
	String      m_url;
	WTF::Vector<WTF::String> m_codecs;
	int         m_fps = 0;
	int         m_width = 0;
	int         m_height = 0;
	int         m_bitRate = 0;
};

struct MediaPlayerAmigaOSInfo
{
	WTF::String m_audioCodec;
	WTF::String m_videoCodec;
	float       m_duration = 0;
	int         m_frequency = 0;
	int         m_bits;
	int         m_channels = 0;
	int         m_width = 0;
	int         m_height;
	int         m_bitRate = 0;
	bool        m_isLive = false;
	bool        m_isHLS = false;
    bool        m_isDownloadable = false;
    bool        m_isMediaSource = false;
    bool        m_clearKeyDRM = false;
    WTF::Vector<MediaPlayerAmigaOSInfoTrack> m_hlsStreams;
    String      m_selectedHLSStreamURL;
};

struct MediaPlayerAmigaOSStreamSettings
{
	bool m_decodeVideo = true;
	
    // NOTE: keep in sync with WkGlobalSettings_LoopFilter
    enum class SkipLoopFilter {
        Default,
        NonRef,
        BiDirectional,
        NonIntra,
        NonKey,
        All
    };

    SkipLoopFilter m_loopFilter = SkipLoopFilter::NonKey;
};

struct MediaPlayerAmigaOSSettings
{
public:
    static MediaPlayerAmigaOSSettings &settings();

	NetworkingContext *m_networkingContextForRequests = nullptr;

	Function<bool(WebCore::Page *page, const String &host)> m_supportMediaForHost;
	Function<bool(WebCore::Page *page, const String &host)> m_supportMediaSourceForHost;
	Function<bool(WebCore::Page *page, const String &host)> m_supportHLSForHost;
	Function<bool(WebCore::Page *page, const String &host)> m_supportVP9ForHost;
	Function<bool(WebCore::Page *page, const String &host)> m_supportHVCForHost;

	Function<void(WebCore::MediaPlayer *player, const String &url,
		MediaPlayerAmigaOSInfo &info, MediaPlayerAmigaOSStreamSettings &settings,
		Function<void()> &&yieldFunc)> m_load;
	Function<void(WebCore::MediaPlayer *player, MediaPlayerAmigaOSInfo &info)> m_update;
	Function<void(WebCore::MediaPlayer *player)> m_loadCancelled;
	Function<void(WebCore::MediaPlayer *player)> m_willPlay;
	Function<void(WebCore::MediaPlayer *player)> m_pausedOrFinished;

	Function<void(WebCore::MediaPlayer *player,
		Function<void(void *windowPtr, int scrollX, int scrollY, int left, int top, int right, int bottom, int width, int height)>&&)> m_overlayRequest;
	Function<void(WebCore::MediaPlayer *player)> m_overlayUpdate;
};

}

#endif
