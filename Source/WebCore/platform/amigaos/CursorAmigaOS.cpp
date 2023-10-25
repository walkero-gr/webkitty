/*
 * Copyright (C) 2020-2022 Jacek Piszczek
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE INC. AND ITS CONTRIBUTORS ``AS IS''
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL APPLE INC. OR ITS CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 */
 
#include "config.h"
#include "Cursor.h"
#include <intuition/pointerclass.h>

namespace WebCore {

void Cursor::ensurePlatformCursor() const
{
    if (m_platformCursor || m_type == Cursor::Pointer)
        return;

    switch (m_type) {
    case Cursor::Pointer:
        m_platformCursor = WebCore::PlatformCursor(POINTERTYPE_NORMAL);
        break;
    case Cursor::Cross:
        m_platformCursor = WebCore::PlatformCursor(POINTERTYPE_CROSS);
        break;
    case Cursor::Hand:
        m_platformCursor = WebCore::PlatformCursor(POINTERTYPE_LINK);
        break;
    case Cursor::IBeam:
        m_platformCursor = WebCore::PlatformCursor(POINTERTYPE_TEXT);
        break;
    case Cursor::Wait:
        m_platformCursor = WebCore::PlatformCursor(POINTERTYPE_BUSY);
        break;
    case Cursor::Help:
        m_platformCursor = WebCore::PlatformCursor(POINTERTYPE_HELP);
        break;
    case Cursor::Move:
    case Cursor::MiddlePanning:
        m_platformCursor = WebCore::PlatformCursor(POINTERTYPE_HAND);
        break;
    case Cursor::EastResize:
    case Cursor::EastPanning:
        m_platformCursor = WebCore::PlatformCursor(POINTERTYPE_EASTRESIZE);
        break;
    case Cursor::NorthResize:
    case Cursor::NorthPanning:
        m_platformCursor = WebCore::PlatformCursor(POINTERTYPE_NORTHRESIZE);
        break;
    case Cursor::NorthEastResize:
    case Cursor::NorthEastPanning:
        m_platformCursor = WebCore::PlatformCursor(POINTERTYPE_NORTHEASTRESIZE);
        break;
    case Cursor::NorthWestResize:
    case Cursor::NorthWestPanning:
        m_platformCursor = WebCore::PlatformCursor(POINTERTYPE_NORTHWESTRESIZE);
        break;
    case Cursor::SouthResize:
    case Cursor::SouthPanning:
        m_platformCursor = WebCore::PlatformCursor(POINTERTYPE_SOUTHRESIZE);
        break;
    case Cursor::SouthEastResize:
    case Cursor::SouthEastPanning:
        m_platformCursor = WebCore::PlatformCursor(POINTERTYPE_SOUTHEASTRESIZE);
        break;
    case Cursor::SouthWestResize:
    case Cursor::SouthWestPanning:
        m_platformCursor = WebCore::PlatformCursor(POINTERTYPE_SOUTHWESTRESIZE);
        break;
    case Cursor::WestResize:
    case Cursor::WestPanning:
        m_platformCursor = WebCore::PlatformCursor(POINTERTYPE_WESTRESIZE);
        break;
    case Cursor::NorthSouthResize:
        m_platformCursor = WebCore::PlatformCursor(POINTERTYPE_NORTHSOUTHRESIZE);
        break;
    case Cursor::EastWestResize:
        m_platformCursor = WebCore::PlatformCursor(POINTERTYPE_EASTWESTRESIZE);
        break;
    case Cursor::NorthEastSouthWestResize:
        m_platformCursor = WebCore::PlatformCursor(POINTERTYPE_NORTHEASTSOUTHWESTRESIZE);
        break;
    case Cursor::NorthWestSouthEastResize:
        m_platformCursor = WebCore::PlatformCursor(POINTERTYPE_NORTHWESTSOUTHEASTRESIZE);
        break;
    case Cursor::ColumnResize:
        m_platformCursor = WebCore::PlatformCursor(POINTERTYPE_COLUMNRESIZE);
        break;
    case Cursor::RowResize:
        m_platformCursor = WebCore::PlatformCursor(POINTERTYPE_ROWRESIZE);
        break;
    case Cursor::VerticalText:
        m_platformCursor = WebCore::PlatformCursor(POINTERTYPE_VERTICALTEXT);
        break;
    case Cursor::Cell:
        m_platformCursor = WebCore::PlatformCursor(POINTERTYPE_CELL);
        break;
    case Cursor::ContextMenu:
        m_platformCursor = WebCore::PlatformCursor(POINTERTYPE_CONTEXTMENU);
        break;
    case Cursor::Alias:
        m_platformCursor = WebCore::PlatformCursor(POINTERTYPE_ALIAS);
        break;
    case Cursor::Progress:
        m_platformCursor = WebCore::PlatformCursor(POINTERTYPE_PROGRESS);
        break;
    case Cursor::NoDrop:
        m_platformCursor = WebCore::PlatformCursor(POINTERTYPE_NODROP);
        break;
    case Cursor::NotAllowed:
        m_platformCursor = WebCore::PlatformCursor(POINTERTYPE_NOTALLOWED);
        break;
    case Cursor::Copy:
        m_platformCursor = WebCore::PlatformCursor(POINTERTYPE_COPY);
        break;
    case Cursor::None:
        m_platformCursor = WebCore::PlatformCursor(POINTERTYPE_NONE);
        break;
    case Cursor::ZoomIn:
        m_platformCursor = WebCore::PlatformCursor(POINTERTYPE_ZOOMIN);
        break;
    case Cursor::ZoomOut:
        m_platformCursor = WebCore::PlatformCursor(POINTERTYPE_ZOOMOUT);
        break;
    case Cursor::Grab:
        m_platformCursor = WebCore::PlatformCursor(POINTERTYPE_NORMAL);
        break;
    case Cursor::Grabbing:
        m_platformCursor = WebCore::PlatformCursor(POINTERTYPE_DRAGANDDROP);
        break;
    case Cursor::Custom:
        m_platformCursor = WebCore::PlatformCursor(POINTERTYPE_NORMAL);
        break;
    }
}

}

