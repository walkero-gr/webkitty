#include "config.h"

#include "VideoTrackPrivateAmigaOS.h"

#define D(x) 

#if ENABLE(VIDEO)

namespace WebCore {

VideoTrackPrivateAmigaOS::VideoTrackPrivateAmigaOS(WeakPtr<MediaPlayerPrivateAmigaOS> player, int index)
	: m_index(index)
	, m_player(player)
{
	m_id = "V" + String::number(index);
	D(dprintf("%s(%p)\n", __PRETTY_FUNCTION__, this));
}

VideoTrackPrivate::Kind VideoTrackPrivateAmigaOS::kind() const
{
	return VideoTrackPrivate::Kind();
}

void VideoTrackPrivateAmigaOS::setSelected(bool setselected)
{
	if (setselected != selected())
	{
		VideoTrackPrivate::setSelected(setselected);
		if (m_player)
			m_player->onTrackEnabled(m_index, setselected);
	}
}

void VideoTrackPrivateAmigaOS::disconnect()
{
	D(dprintf("%s(%p)\n", __PRETTY_FUNCTION__, this));
	m_player = nullptr;
}

#if ENABLE(MEDIA_SOURCE)

void VideoTrackPrivateAmigaOSMS::setSelected(bool setselected)
{
	if (setselected != selected())
	{
		VideoTrackPrivate::setSelected(setselected);
        if (m_source)
            m_source->onTrackEnabled(m_index, setselected);
    }
}

void VideoTrackPrivateAmigaOSMS::disconnect()
{
    m_source = nullptr;
}

VideoTrackPrivateAmigaOSMS::VideoTrackPrivateAmigaOSMS(MediaSourceBufferPrivateAmigaOS *source, int index)
    : VideoTrackPrivateAmigaOS(nullptr, index)
    , m_source(source)
{
}

#endif

}

#endif
