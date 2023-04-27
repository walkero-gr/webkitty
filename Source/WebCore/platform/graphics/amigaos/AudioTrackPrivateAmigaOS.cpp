#include "config.h"

#include "AudioTrackPrivateAmigaOS.h"

#define D(x) 

#if ENABLE(VIDEO)

namespace WebCore {

AudioTrackPrivateAmigaOS::AudioTrackPrivateAmigaOS(WeakPtr<MediaPlayerPrivateAmigaOS> player, int index)
	: m_index(index)
	, m_player(player)
{
	m_id = "A" + String::number(index);
	D(dprintf("%s(%p)\n", __PRETTY_FUNCTION__, this));
}

AudioTrackPrivate::Kind AudioTrackPrivateAmigaOS::kind() const
{
	return AudioTrackPrivate::Kind();
}

void AudioTrackPrivateAmigaOS::disconnect()
{
	D(dprintf("%s(%p)\n", __PRETTY_FUNCTION__, this));
	m_player = nullptr;
}

void AudioTrackPrivateAmigaOS::setEnabled(bool setenabled)
{
	if (setenabled != enabled())
	{
		AudioTrackPrivate::setEnabled(setenabled);
		if (m_player)
			m_player->onTrackEnabled(m_index, setenabled);
	}
}

#if ENABLE(MEDIA_SOURCE)

AudioTrackPrivateAmigaOSMS::AudioTrackPrivateAmigaOSMS(MediaSourceBufferPrivateAmigaOS *source, int index)
    : AudioTrackPrivateAmigaOS(nullptr, index)
    , m_source(source)
{

}

void AudioTrackPrivateAmigaOSMS::disconnect()
{
    m_source = nullptr;
}

void AudioTrackPrivateAmigaOSMS::setEnabled(bool setenabled)
{
	if (setenabled != enabled())
	{
		AudioTrackPrivate::setEnabled(setenabled);
		if (m_source)
			m_source->onTrackEnabled(m_index, setenabled);
	}

}

#endif

}

#endif
