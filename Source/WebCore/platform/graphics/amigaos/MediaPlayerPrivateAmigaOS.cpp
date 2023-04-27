#include "MediaPlayerPrivateAmigaOS.h"

#if ENABLE(VIDEO)

#include "GraphicsContext.h"
#include "MediaPlayer.h"
#include "MediaSourcePrivateClient.h"
#include "NotImplemented.h"
#include "AcinerellaContainer.h"
#include "AudioTrackPrivateAmigaOS.h"
#include "VideoTrackPrivateAmigaOS.h"
#include "PlatformMediaResourceLoader.h"

#include "HTMLMediaElement.h"
#include "Frame.h"
#include "Page.h"
#include "CommonVM.h"
#include <proto/exec.h>
#include <exec/exec.h>

#define D(x)
#define DM(x)
#define DMHOST(x) 

namespace WebCore {

MediaPlayerAmigaOSSettings &MediaPlayerAmigaOSSettings::settings()
{
	static MediaPlayerAmigaOSSettings m_playerSettings;
	return m_playerSettings;
}

class MediaPlayerFactoryMediaSourceAmigaOS final : public MediaPlayerFactory {
public:
    MediaPlayerEnums::MediaEngineIdentifier identifier() const final { return MediaPlayerEnums::MediaEngineIdentifier::AmigaOS; };

    std::unique_ptr<MediaPlayerPrivateInterface> createMediaEnginePlayer(MediaPlayer* player) const final { return makeUnique<MediaPlayerPrivateAmigaOS>(player); }

    static bool isCGXVideoValid()
    {
        static bool checkDone = false;
        static bool valid;
        if (!checkDone)
        {
            struct Library *cgx = OpenLibrary("cgxvideo.library", 43);
            if (cgx)
            {
                valid = false;
                if (cgx->lib_Version > 43)
                    valid = true;
                else if (cgx->lib_Revision >= 18)
                    valid = true;
                CloseLibrary(cgx);
            }
            checkDone = true;
            if (!valid)
                dprintf("Wayfarer: please make sure cgxvideo.library 43.18 is installed in MOSSYS:Libs!\n");
        }
        return valid;
    }

    static void s_getSupportedTypes(HashSet<String, ASCIICaseInsensitiveHash>& types, bool withHLS)
    {
		// Audio
		types.add(String("audio/aac"));
		types.add(String("audio/basic"));
		types.add(String("audio/mp3"));
		types.add(String("audio/mp4"));
		types.add(String("audio/flac"));
		types.add(String("audio/mpeg"));
		types.add(String("audio/vnd.wave"));
		types.add(String("audio/wav"));
		types.add(String("audio/wave"));

		types.add(String("audio/x-aiff"));
		types.add(String("audio/x-flac"));
		types.add(String("audio/x-m4a"));
		types.add(String("audio/x-pn-wav"));
		types.add(String("audio/x-wav"));

		types.add(String("audio/ogg"));
		types.add(String("audio/webm"));

		types.add(String("audio/x-scpls"));
		types.add(String("audio/mpa"));
		types.add(String("audio/mpa-robust"));

		// Video
		types.add(String("video/avi"));
		types.add(String("video/flv"));
		types.add(String("video/mp4"));
		types.add(String("video/3gpp")); // this is mp4
		types.add(String("video/vnd.objectvideo"));
		types.add(String("video/x-flv"));

		types.add(String("video/ogg"));
		types.add(String("video/x-theora+ogg"));
		types.add(String("video/webm"));

		// HLS
		if (withHLS)
		{
			types.add(String("audio/x-mpegurl"));
			types.add(String("application/x-mpegurl"));
			types.add(String("application/vnd.apple.mpegurl"));
		}
    }

	void getSupportedTypes(HashSet<String, ASCIICaseInsensitiveHash>& types) const final
	{
		s_getSupportedTypes(types, true);
	}

    static MediaPlayer::SupportsType s_supportsTypeAndCodecs(const MediaEngineSupportParameters& parameters)
    {
		String host;
		Page *page = parameters.page;
		
		DMHOST(dprintf("%s: page %p\n", __func__, page));
		
		if (nullptr == page)
		{
			Frame* frame = lexicalFrameFromCommonVM();
			DMHOST(dprintf("%s: vmframe %p\n", __func__, frame));
			if (frame)
			{
				Document *doc = frame->mainFrame().document();
				DMHOST(dprintf("%s: doc %p\n", __func__, doc));
				if (doc)
				{
					host = doc->url().host().toString();
					page = doc->page();
				}
			}
			else if (parameters.url.isValid())
			{
				host = parameters.url.host().toString();
			}
		}
		else
		{
			Document *doc = page->mainFrame().document();
			DMHOST(dprintf("%s: doc %p urlvalid %d (%s)\n", __func__, doc, parameters.url.isValid(), parameters.url.string().utf8().data()));
			if (doc)
			{
				host = doc->url().host().toString();
			}
			else if (parameters.url.isValid())
			{
				host = parameters.url.host().toString();
			}
		}
    
    	if (startsWithLettersIgnoringASCIICase(parameters.type.raw(), "image/"))
    	{
    		return MediaPlayer::SupportsType::IsNotSupported;
		}
		
    	if (startsWithLettersIgnoringASCIICase(parameters.url.string(), "data:"))
    	{
    		return MediaPlayer::SupportsType::IsNotSupported;
		}

        if (!isCGXVideoValid())
        {
    		return MediaPlayer::SupportsType::IsNotSupported;
		}

		bool withHLS = MediaPlayerAmigaOSSettings::settings().m_supportHLSForHost ? MediaPlayerAmigaOSSettings::settings().m_supportHLSForHost(page, host) : true;

		DM(dprintf("%s: url '%s' content '%s' ctype '%s' isource %d istream %d profiles %d hlsOK %d host '%s' page %p\n", __func__,
			parameters.url.string().utf8().data(), parameters.type.raw().utf8().data(), parameters.type.containerType().utf8().data(),
			parameters.isMediaSource, parameters.isMediaStream, parameters.type.profiles().size(), withHLS,
			host.utf8().data(), page));

		if (MediaPlayerAmigaOSSettings::settings().m_supportMediaForHost && !parameters.isMediaSource &&
			!MediaPlayerAmigaOSSettings::settings().m_supportMediaForHost(page, host))
		{
			DM(dprintf("%s: rejecting due to supportMediaForHost check on host '%s'...\n", __func__, host.utf8().data()));
			return MediaPlayer::SupportsType::IsNotSupported;
		}

		if (MediaPlayerAmigaOSSettings::settings().m_supportMediaSourceForHost && parameters.isMediaSource &&
			!MediaPlayerAmigaOSSettings::settings().m_supportMediaSourceForHost(page, host))
		{
			DM(dprintf("%s: rejecting due to supportMediaSourceForHost check...\n", __func__));
			return MediaPlayer::SupportsType::IsNotSupported;
		}

    	if (startsWithLettersIgnoringASCIICase(parameters.url.string(), "blob:"))
    	{
#if ENABLE(MEDIA_SOURCE)
			if (!parameters.isMediaSource)
#endif
			return MediaPlayer::SupportsType::IsNotSupported;
		}
		
       	auto containerType = parameters.type.containerType();
		if (containerType.isEmpty())
		{
			DM(dprintf("%s: container empty, assume 'maybe'\n", __func__));
			return MediaPlayer::SupportsType::MayBeSupported;
		}
		HashSet<String, ASCIICaseInsensitiveHash> types;
		s_getSupportedTypes(types, withHLS);
		DM(dprintf("%s: '%s' contained in list? %d\n", __func__, parameters.type.containerType().utf8().data(), types.contains(containerType)));
		if (types.contains(containerType))
		{
			auto codecs = parameters.type.codecs();
			if (codecs.isEmpty())
			{
				DM(dprintf("%s: codecs empty, assume 'maybe'\n", __func__));
				return MediaPlayer::SupportsType::MayBeSupported;
			}

			DM(dprintf("%s: lists %d codecs\n", __func__, codecs.size()));
			for (size_t i = 0; i < codecs.size(); i++)
			{
				auto &codec = codecs.at(i);
				if (startsWithLettersIgnoringASCIICase(codec, "av01") || startsWithLettersIgnoringASCIICase(codec, "av1")) // requires ffmpeg 4.0 + additional libs
				{
					DM(dprintf("%s: rejecting unsupported codec %s\n", __func__, codec.utf8().data()));
					return MediaPlayer::SupportsType::IsNotSupported;
				}
#if 0
				// higher profile h264 seem to fail decoding (on vimeo, but work on yt!)
				else if (startsWithLettersIgnoringASCIICase(codec, "avc1.5") || startsWithLettersIgnoringASCIICase(codec, "avc1.6") || startsWithLettersIgnoringASCIICase(codec, "avc1.7") || startsWithLettersIgnoringASCIICase(codec, "avc1.8") || startsWithLettersIgnoringASCIICase(codec, "avc1.f"))
				{
					DM(dprintf("%s: rejecting unsupported codec %s\n", __func__, codec.utf8().data()));
					return MediaPlayer::SupportsType::IsNotSupported;
				}
#endif
				else if (startsWithLettersIgnoringASCIICase(codec, "hvc1")) // not enabled in ffmpeg (h265 variant)
				{
					if (MediaPlayerAmigaOSSettings::settings().m_supportHVCForHost && !MediaPlayerAmigaOSSettings::settings().m_supportHVCForHost(page, host))
					{
						DM(dprintf("%s: rejecting user disabled codec %s\n", __func__, codec.utf8().data()));
						return MediaPlayer::SupportsType::IsNotSupported;
					}
				}
                else if (startsWithLettersIgnoringASCIICase(codec, "vp9"))
                {
					if (MediaPlayerAmigaOSSettings::settings().m_supportVP9ForHost && !MediaPlayerAmigaOSSettings::settings().m_supportVP9ForHost(page, host))
					{
						DM(dprintf("%s: rejecting user disabled codec %s\n", __func__, codec.utf8().data()));
						return MediaPlayer::SupportsType::IsNotSupported;
					}
                }
				else
				{
					DM(dprintf("%s: we should be OK with codec %s\n", __func__, codec.utf8().data()));
				}
			}

			return MediaPlayer::SupportsType::IsSupported;
		}
		DM(dprintf("%s: not supported!\n", __func__));
        return MediaPlayer::SupportsType::IsNotSupported;
    }
	
    MediaPlayer::SupportsType supportsTypeAndCodecs(const MediaEngineSupportParameters& parameters) const final
    {
    	return s_supportsTypeAndCodecs(parameters);
	}
};

MediaPlayerPrivateAmigaOS::MediaPlayerPrivateAmigaOS(MediaPlayer* player)
	: m_player(player)
{
	notImplemented();
}

MediaPlayerPrivateAmigaOS::~MediaPlayerPrivateAmigaOS()
{
	if (m_acinerella)
		m_acinerella->terminate();

	// remove all pending requests that could be referencing 'this'
	if (MediaPlayerAmigaOSSettings::settings().m_loadCancelled)
		MediaPlayerAmigaOSSettings::settings().m_loadCancelled(m_player);
}

void MediaPlayerPrivateAmigaOS::registerMediaEngine(MediaEngineRegistrar registrar)
{
	registrar(makeUnique<MediaPlayerFactoryMediaSourceAmigaOS>());
}

MediaPlayer::SupportsType MediaPlayerPrivateAmigaOS::extendedSupportsType(const MediaEngineSupportParameters& parameters, MediaPlayer::SupportsType type)
{
	(void)type;
	return MediaPlayerFactoryMediaSourceAmigaOS::s_supportsTypeAndCodecs(parameters);
}

bool MediaPlayerPrivateAmigaOS::supportsKeySystem(const String& keySystem, const String& mimeType)
{
	if (equalIgnoringASCIICase(keySystem, "org.w3c.clearkey") && !mimeType.isEmpty() && equalIgnoringASCIICase(mimeType, "application/x-mpegurl"))
		return true;
	return false;
}

void MediaPlayerPrivateAmigaOS::load(const String& url)
{
	D(dprintf("%s: %s\n", __PRETTY_FUNCTION__, url.utf8().data()));

	cancelLoad();

	if (startsWithLettersIgnoringASCIICase(url, "about:"))
		return;

	if (!canLoad(false))
		return;

	m_networkState = MediaPlayer::NetworkState::Loading;
	m_player->networkStateChanged();
	m_readyState = MediaPlayer::ReadyState::HaveNothing;
	m_player->readyStateChanged();

	m_acinerella = Acinerella::Acinerella::create(this, url);
}

#if ENABLE(MEDIA_SOURCE)
void MediaPlayerPrivateAmigaOS::load(const URL& url, const ContentType&, MediaSourcePrivateClient* client)
{
	D(dprintf("%s: %s\n", __PRETTY_FUNCTION__, url.string().utf8().data()));
	cancelLoad();

	if (startsWithLettersIgnoringASCIICase(url.string(), "about:"))
		return;
		
	if (!canLoad(true))
		return;

	m_networkState = MediaPlayer::NetworkState::Loading;
	m_player->networkStateChanged();
	m_readyState = MediaPlayer::ReadyState::HaveNothing;
	m_player->readyStateChanged();

	m_mediaSourcePrivate = MediaSourcePrivateAmigaOS::create(*this, *client, url.string());
}
#endif

bool MediaPlayerPrivateAmigaOS::canLoad(bool isMediaSource)
{
	Page *page = m_player->client().mediaPlayerPage();
	String host;
	Document *doc = page ? page->mainFrame().document() : nullptr;
	if (doc)
	{
		host = doc->url().host().toString();
	}

	D(dprintf("%s: page %p doc %p host %s\n", __PRETTY_FUNCTION__, page, doc, host.utf8().data()));

	bool ok = false;

	if (page)
	{
		if (isMediaSource)
		{
			ok = !MediaPlayerAmigaOSSettings::settings().m_supportMediaSourceForHost ||
				MediaPlayerAmigaOSSettings::settings().m_supportMediaSourceForHost(page, host);
		}
		else
		{
			ok = !MediaPlayerAmigaOSSettings::settings().m_supportMediaForHost ||
				MediaPlayerAmigaOSSettings::settings().m_supportMediaForHost(page, host);
		}
	}

	if (!ok)
	{
		m_networkState = WebCore::MediaPlayerEnums::NetworkState::FormatError;
		m_readyState = WebCore::MediaPlayerEnums::ReadyState::HaveNothing;
		m_player->networkStateChanged();
		m_player->readyStateChanged();
	}

	return ok;
}

void MediaPlayerPrivateAmigaOS::cancelLoad()
{
	D(dprintf("%s:\n", __PRETTY_FUNCTION__));

	if (MediaPlayerAmigaOSSettings::settings().m_loadCancelled)
		MediaPlayerAmigaOSSettings::settings().m_loadCancelled(m_player);

#if ENABLE(MEDIA_SOURCE)
	if (m_mediaSourcePrivate)
		m_mediaSourcePrivate->orphan();
	m_mediaSourcePrivate = nullptr;
#endif

	m_prepareToPlay = m_acInitialized = false;
	pause();

	if (m_acinerella)
		m_acinerella->terminate();
}

void MediaPlayerPrivateAmigaOS::prepareToPlay()
{
	D(dprintf("%s:\n", __PRETTY_FUNCTION__));
	m_prepareToPlay = true;

#if ENABLE(MEDIA_SOURCE)
	if (m_mediaSourcePrivate)
		m_mediaSourcePrivate->warmUp();
#endif

	if (m_acinerella && m_acInitialized)
		m_acinerella->warmUp();
}

bool MediaPlayerPrivateAmigaOS::canSaveMediaData() const
{
	D(dprintf("%s:\n", __PRETTY_FUNCTION__));
	if (m_acinerella && !m_acinerella->isLive())
		return true;
	return false;
}

void MediaPlayerPrivateAmigaOS::play()
{
	if (MediaPlayerAmigaOSSettings::settings().m_willPlay)
		MediaPlayerAmigaOSSettings::settings().m_willPlay(m_player);

	if (m_acinerella)
		m_acinerella->play();
#if ENABLE(MEDIA_SOURCE)
	else if (m_mediaSourcePrivate)
		m_mediaSourcePrivate->play();
#endif

	D(dprintf("%s:\n", __PRETTY_FUNCTION__));
	
	if (m_player)
	{
		if (m_acinerella)
		{
			m_player->muteChanged(m_acinerella->muted());
			m_player->volumeChanged(m_acinerella->volume());
		}
		
		m_player->rateChanged();
		m_player->playbackStateChanged();
	}
}

void MediaPlayerPrivateAmigaOS::pause()
{
	if (m_acinerella)
		m_acinerella->pause();
#if ENABLE(MEDIA_SOURCE)
	else if (m_mediaSourcePrivate)
		m_mediaSourcePrivate->pause();
#endif
	D(dprintf("%s:\n", __PRETTY_FUNCTION__));

// NO, this will break the internal PLAYING state of HTMLMediaElement
//	m_player->playbackStateChanged();

	if (MediaPlayerAmigaOSSettings::settings().m_pausedOrFinished)
		MediaPlayerAmigaOSSettings::settings().m_pausedOrFinished(m_player);
}

void MediaPlayerPrivateAmigaOS::setVolume(float volume)
{
	D(dprintf("%s: vol %f\n", __PRETTY_FUNCTION__, volume));
	if (m_acinerella)
		m_acinerella->setVolume(volume);
#if ENABLE(MEDIA_SOURCE)
	else if (m_mediaSourcePrivate)
		m_mediaSourcePrivate->setVolume(volume);
#endif
}

void MediaPlayerPrivateAmigaOS::setMuted(bool muted)
{
	D(dprintf("%s: %d\n", __PRETTY_FUNCTION__, muted));
	if (m_acinerella)
		m_acinerella->setMuted(muted);
#if ENABLE(MEDIA_SOURCE)
	else if (m_mediaSourcePrivate)
		m_mediaSourcePrivate->setMuted(muted);
#endif
}

FloatSize MediaPlayerPrivateAmigaOS::naturalSize() const
{
	return { float(m_width), float(m_height) };
}

float MediaPlayerPrivateAmigaOS::duration() const
{
	return durationDouble();
}

double MediaPlayerPrivateAmigaOS::durationDouble() const
{
#if ENABLE(MEDIA_SOURCE)
	if (m_mediaSourcePrivate)
		return m_mediaSourcePrivate->duration().toDouble();
#endif
	return m_duration;
}

MediaTime MediaPlayerPrivateAmigaOS::durationMediaTime() const
{
	if (m_acinerella && m_acinerella->isLive())
		return MediaTime::invalidTime();
#if ENABLE(MEDIA_SOURCE)
	if (m_mediaSourcePrivate)
		return m_mediaSourcePrivate->duration();
#endif
	return MediaTime::createWithDouble(durationDouble());
}

bool MediaPlayerPrivateAmigaOS::hasVideo() const
{
	if (m_acinerella)
		return m_acinerella->hasVideo();
#if ENABLE(MEDIA_SOURCE)
    else if (m_mediaSourcePrivate)
        return m_mediaSourcePrivate->hasVideo();
#endif
	return false;
}

bool MediaPlayerPrivateAmigaOS::hasAudio() const
{
	if (m_acinerella)
		return m_acinerella->hasAudio();
#if ENABLE(MEDIA_SOURCE)
    else if (m_mediaSourcePrivate)
        return m_mediaSourcePrivate->hasAudio();
#endif
	return false;
}

void MediaPlayerPrivateAmigaOS::setPageIsVisible(bool visible)
{
	m_visible = visible;
//	D(dprintf("%s: visible %d\n", __PRETTY_FUNCTION__, visible));
}

bool MediaPlayerPrivateAmigaOS::seeking() const
{
	D(dprintf("%s: %d\n", __PRETTY_FUNCTION__, m_acinerella?m_acinerella->isSeeking():false));
	if (m_acinerella)
		return m_acinerella->isSeeking();
#if ENABLE(MEDIA_SOURCE)
	else if (m_mediaSourcePrivate)
		return m_mediaSourcePrivate->isSeeking();
#endif
	return false;
}

void MediaPlayerPrivateAmigaOS::seek(float time)
{
	D(dprintf("%s: %f\n", __PRETTY_FUNCTION__, time));
	if (m_acinerella)
		return m_acinerella->seek(time);
#if ENABLE(MEDIA_SOURCE)
	else if (m_mediaSourcePrivate)
		return m_mediaSourcePrivate->seek(time);
#endif
}

bool MediaPlayerPrivateAmigaOS::ended() const
{
	if (m_acinerella)
		return m_acinerella->ended();
#if ENABLE(MEDIA_SOURCE)
	else if (m_mediaSourcePrivate)
		return m_mediaSourcePrivate->ended();
#endif
	return true;
}

bool MediaPlayerPrivateAmigaOS::paused() const
{
	if (m_acinerella)
		return m_acinerella->paused();
#if ENABLE(MEDIA_SOURCE)
	else if (m_mediaSourcePrivate)
		return m_mediaSourcePrivate->paused();
#endif
	return true;
}

std::optional<VideoPlaybackQualityMetrics> MediaPlayerPrivateAmigaOS::videoPlaybackQualityMetrics()
{
	VideoPlaybackQualityMetrics metrics;
	metrics.totalVideoFrames = m_decodedFrameCount;
	metrics.droppedVideoFrames = m_droppedFrameCount;
	return metrics;
}

MediaPlayer::NetworkState MediaPlayerPrivateAmigaOS::networkState() const
{
	return m_networkState;
}

MediaPlayer::ReadyState MediaPlayerPrivateAmigaOS::readyState() const
{
	return m_readyState;
}

std::unique_ptr<PlatformTimeRanges> MediaPlayerPrivateAmigaOS::buffered() const
{
#if ENABLE(MEDIA_SOURCE)
	if (m_mediaSourcePrivate)
		return m_mediaSourcePrivate->buffered();
#endif
	return makeUnique<PlatformTimeRanges>(MediaTime::createWithDouble(std::max(0.0, m_currentTime - 1.0 )),
		MediaTime::createWithDouble(m_currentTime + 10.0));
}

void MediaPlayerPrivateAmigaOS::paint(GraphicsContext& gc, const FloatRect& rect)
{
	if (gc.paintingDisabled() || !m_visible)
		return;

	if (m_acinerella)
		m_acinerella->paint(gc, rect);
#if ENABLE(MEDIA_SOURCE)
	else if (m_mediaSourcePrivate)
		m_mediaSourcePrivate->paint(gc, rect);
#endif
}

void MediaPlayerPrivateAmigaOS::accNextFrameReady()
{
	if (!m_didDrawFrame)
	{
		if (m_player)
		{
			m_player->firstVideoFrameAvailable();
			m_player->repaint();
		}

		m_didDrawFrame = true;

		if (MediaPlayerAmigaOSSettings::settings().m_overlayRequest)
		{
			MediaPlayerAmigaOSSettings::settings().m_overlayRequest(m_player,
				[weak = WeakPtr{*this}](void *ptr, int sx, int sy, int ml, int mt, int mr, int mb, int w, int h) {
				if (weak) {
					if (weak->m_acinerella)
						weak->m_acinerella->setOverlayWindowCoords((struct ::Window *)ptr, sx, sy, ml, mt, mr, mb, w, h);
#if ENABLE(MEDIA_SOURCE)
					else if (weak->m_mediaSourcePrivate)
						weak->m_mediaSourcePrivate->setOverlayWindowCoords((struct ::Window *)ptr, sx, sy, ml, mt, mr, mb, w, h);
#endif
				}
			});
		}
	}
    else
    {
        m_player->repaint();
    }
}

void MediaPlayerPrivateAmigaOS::accNoFramesReady()
{
	// TODO: overlay shutdown?
	m_didDrawFrame = false;
	m_player->repaint();
}

void MediaPlayerPrivateAmigaOS::accSetVideoSize(int width, int height)
{
	m_width = width;
	m_height = height;
	if (m_player)
		m_player->sizeChanged();
}

void MediaPlayerPrivateAmigaOS::accFrameUpdateNeeded() 
{
	if (MediaPlayerAmigaOSSettings::settings().m_overlayUpdate)
		MediaPlayerAmigaOSSettings::settings().m_overlayUpdate(m_player);
}

bool MediaPlayerPrivateAmigaOS::accCodecSupported(const String &codec)
{
	MediaEngineSupportParameters parameters;
	parameters.page = m_player->client().mediaPlayerPage();
	String ct; // audio/mp4; codecs="mp4a.40.2"
	ct = "video/mp4; codecs=\"";
	ct.append(codec);
	ct.append("\"");
	parameters.type = ContentType(ct);
	return MediaPlayerFactoryMediaSourceAmigaOS::s_supportsTypeAndCodecs(parameters) == MediaPlayer::SupportsType::IsSupported;
}

bool MediaPlayerPrivateAmigaOS::accIsURLValid(const String& url)
{
	if (m_failedHLSStreamURIs.contains(url))
		return false;
	return true;
}

void MediaPlayerPrivateAmigaOS::accSetFrameCounts(unsigned decoded, unsigned dropped)
{
	m_decodedFrameCount = decoded;
	m_droppedFrameCount = dropped;
}

bool MediaPlayerPrivateAmigaOS::didLoadingProgress() const
{
	if (m_didLoadingProgress)
	{
		m_didLoadingProgress = false;
		return true;
	}

	return false;
}

MediaPlayer::MovieLoadType MediaPlayerPrivateAmigaOS::movieLoadType() const
{
	if (m_acinerella)
		return m_acinerella->isLive() ? MediaPlayer::MovieLoadType::LiveStream : MediaPlayer::MovieLoadType::Download;
	return MediaPlayer::MovieLoadType::Download;
}

float MediaPlayerPrivateAmigaOS::maxTimeSeekable() const
{
	if (m_acinerella && m_acinerella->canSeek())
		return m_duration;
#if ENABLE(MEDIA_SOURCE)
	return m_duration;
#endif
	return 0.f;
}

void MediaPlayerPrivateAmigaOS::accInitialized(MediaPlayerAmigaOSInfo info)
{
	if (MediaPlayerAmigaOSSettings::settings().m_load)
	{
		String url;

		if (info.m_width)
		{
			m_width = info.m_width;
			m_height = info.m_height;
		}

		accSetVideoSize(m_width, m_height);
		accSetReadyState(WebCore::MediaPlayerEnums::ReadyState::HaveMetadata);

#if ENABLE(MEDIA_SOURCE)
		if (m_mediaSourcePrivate)
			url = m_mediaSourcePrivate->url();
		else
#endif
			url = m_acinerella->url();

		MediaPlayerAmigaOSSettings::settings().m_load(m_player, url, info, m_streamSettings,
			[this]() {
				if (m_acinerella) {
					m_acinerella->pause();
					m_acinerella->coolDown();
				}
#if ENABLE(MEDIA_SOURCE)
				else if (m_mediaSourcePrivate) {
					m_mediaSourcePrivate->pause();
					m_mediaSourcePrivate->coolDown();
				}
#endif
				m_didDrawFrame = false;
				m_player->playbackStateChanged();
			});

		m_acInitialized = true;
		m_player->characteristicChanged();

		// MediaSource has its own track handling!
		if (m_acinerella)
		{
			if (info.m_width)
			{
				m_videoTrack = VideoTrackPrivateAmigaOS::create(WeakPtr{*this}, 0);
				m_player->addVideoTrack(*m_videoTrack.get());
			}
			
			if (info.m_channels)
			{
				m_audioTrack = AudioTrackPrivateAmigaOS::create(WeakPtr{*this}, 0);
				m_player->addAudioTrack(*m_audioTrack.get());
			}
		}

		if (m_prepareToPlay && m_acinerella)
			m_acinerella->warmUp();
	#if ENABLE(MEDIA_SOURCE)
		else if (m_prepareToPlay && m_mediaSourcePrivate)
			m_mediaSourcePrivate->warmUp();
	#endif
	}
}

void MediaPlayerPrivateAmigaOS::accUpdated(MediaPlayerAmigaOSInfo info)
{
	if (MediaPlayerAmigaOSSettings::settings().m_update)
	{
		MediaPlayerAmigaOSSettings::settings().m_update(m_player, info);
	}
}

void MediaPlayerPrivateAmigaOS::accSetNetworkState(WebCore::MediaPlayerEnums::NetworkState state)
{
	m_networkState = state;
	m_player->networkStateChanged();
}

void MediaPlayerPrivateAmigaOS::accSetReadyState(WebCore::MediaPlayerEnums::ReadyState state)
{
	m_readyState = state;
	m_player->readyStateChanged();
}

void MediaPlayerPrivateAmigaOS::accSetBufferLength(double buffer)
{
	(void)buffer;
	m_player->bufferedTimeRangesChanged();
	m_player->seekableTimeRangesChanged();
}

void MediaPlayerPrivateAmigaOS::accSetPosition(double pos)
{
	D(dprintf("%s: timechanged to %f\n", __func__, this, float(pos)));
	m_currentTime = pos;
	m_player->timeChanged();
}

void MediaPlayerPrivateAmigaOS::accSetDuration(double dur)
{
#if ENABLE(MEDIA_SOURCE)
	if (m_mediaSourcePrivate)
	{
		m_player->durationChanged();
		return;
	}
#endif
	if (abs(dur - m_duration) >= 1.0)
	{
		D(dprintf("%s: changed to %f\n", __func__, this, float(dur)));
		m_duration = ceil(dur);
		m_player->durationChanged();
	}
}

void MediaPlayerPrivateAmigaOS::accEnded()
{
	m_currentTime = m_duration;
	m_player->timeChanged();
	m_player->characteristicChanged();
	m_player->playbackStateChanged();

	if (MediaPlayerAmigaOSSettings::settings().m_pausedOrFinished)
		MediaPlayerAmigaOSSettings::settings().m_pausedOrFinished(m_player);
}

void MediaPlayerPrivateAmigaOS::accFailed()
{
	if (m_acinerella && m_acinerella->isLive() && !m_acinerella->hlsStreamURL().isEmpty())
	{
		if (!m_failedHLSStreamURIs.contains(m_acinerella->hlsStreamURL()))
		{
			D(dprintf("%s: blacklist stream URL %s and retry\n", __func__, m_acinerella->hlsStreamURL().utf8().data()));
			m_failedHLSStreamURIs.add(m_acinerella->hlsStreamURL());

			String url = m_acinerella->url();
			m_acinerella->terminate();
			m_acinerella = Acinerella::Acinerella::create(this, url);

			if (m_acinerella)
				return;
		}
	}

	m_networkState = WebCore::MediaPlayerEnums::NetworkState::FormatError;
	m_readyState = WebCore::MediaPlayerEnums::ReadyState::HaveNothing;
	m_player->networkStateChanged();
	m_player->readyStateChanged();
	if (MediaPlayerAmigaOSSettings::settings().m_pausedOrFinished)
		MediaPlayerAmigaOSSettings::settings().m_pausedOrFinished(m_player);
}

RefPtr<PlatformMediaResourceLoader> MediaPlayerPrivateAmigaOS::accCreateResourceLoader()
{
	return m_player->createResourceLoader();
}

String MediaPlayerPrivateAmigaOS::accReferrer()
{
	return m_player->referrer();
}

void MediaPlayerPrivateAmigaOS::onTrackEnabled(int index, bool enabled)
{
	(void)index;
	(void)enabled;
	D(dprintf("%s: %p, track %p enabled %d\n", __func__, this, index, enabled));
}

void MediaPlayerPrivateAmigaOS::selectHLSStream(const String& url)
{
	if (m_acinerella)
		m_acinerella->selectStream(url, m_currentTime);
}

}

#undef D
#undef DM
#endif
