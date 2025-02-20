/*
 * Copyright (C) 2017 Apple Inc. All rights reserved.
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

#include "WebKit.h"
#include "WebCacheStorageConnection.h"

#include "CacheStorageEngine.h"
#include "CacheStorageEngineConnection.h"
#include "WebCacheStorageProvider.h"
#include "WebProcess.h"
#include <wtf/MainThread.h>

#define D(x) 

namespace WebKit {
using namespace WebCore::DOMCacheEngine;
using namespace CacheStorage;

WebCacheStorageConnection::WebCacheStorageConnection(WebCacheStorageProvider& provider)
    : m_provider(provider)
{
}

WebCacheStorageConnection::~WebCacheStorageConnection()
{
    ASSERT(isMainRunLoop());
}

CacheStorageEngineConnection& WebCacheStorageConnection::connection()
{
    D(dprintf("%s\n", __PRETTY_FUNCTION__));
    return WebProcess::singleton().cacheStorageEngineConnection();
}

void WebCacheStorageConnection::open(const WebCore::ClientOrigin& origin, const String& cacheName, WebCore::DOMCacheEngine::CacheIdentifierCallback&& callback)
{
    D(dprintf("%s\n", __PRETTY_FUNCTION__));
    connection().open(WebCore::ClientOrigin(origin), String(cacheName), WTFMove(callback));
}

void WebCacheStorageConnection::remove(uint64_t cacheIdentifier, WebCore::DOMCacheEngine::CacheIdentifierCallback&& callback)
{
    D(dprintf("%s\n", __PRETTY_FUNCTION__));
    connection().remove(cacheIdentifier, WTFMove(callback));
}

void WebCacheStorageConnection::retrieveCaches(const WebCore::ClientOrigin& origin, uint64_t updateCounter, WebCore::DOMCacheEngine::CacheInfosCallback&& callback)
{
    D(dprintf("%s\n", __PRETTY_FUNCTION__));
    connection().caches(WebCore::ClientOrigin(origin), updateCounter, WTFMove(callback));
}

void WebCacheStorageConnection::retrieveRecords(uint64_t cacheIdentifier, const WebCore::RetrieveRecordsOptions& options, WebCore::DOMCacheEngine::RecordsCallback&& callback)
{
    D(dprintf("%s\n", __PRETTY_FUNCTION__));
    connection().retrieveRecords(cacheIdentifier, WebCore::RetrieveRecordsOptions(options), WTFMove(callback));
}

void WebCacheStorageConnection::batchDeleteOperation(uint64_t cacheIdentifier, const WebCore::ResourceRequest& request, WebCore::CacheQueryOptions&& options, WebCore::DOMCacheEngine::RecordIdentifiersCallback&& callback)
{
    D(dprintf("%s\n", __PRETTY_FUNCTION__));
    connection().deleteMatchingRecords(cacheIdentifier, WebCore::ResourceRequest(request), WTFMove(options), WTFMove(callback));
}

void WebCacheStorageConnection::batchPutOperation(uint64_t cacheIdentifier, Vector<Record>&& records, WebCore::DOMCacheEngine::RecordIdentifiersCallback&& callback)
{
    D(dprintf("%s\n", __PRETTY_FUNCTION__));
    connection().putRecords(cacheIdentifier, WTFMove(records), WTFMove(callback));
}

void WebCacheStorageConnection::reference(uint64_t cacheIdentifier)
{
    connection().reference(cacheIdentifier);
}

void WebCacheStorageConnection::dereference(uint64_t cacheIdentifier)
{
    connection().dereference(cacheIdentifier);
}

void WebCacheStorageConnection::clearMemoryRepresentation(const WebCore::ClientOrigin& origin, CompletionCallback&& callback)
{
    connection().clearMemoryRepresentation(WebCore::ClientOrigin(origin), WTFMove(callback));
}

void WebCacheStorageConnection::engineRepresentation(CompletionHandler<void(const String&)>&& callback)
{
    connection().engineRepresentation(WTFMove(callback));
}

void WebCacheStorageConnection::updateQuotaBasedOnSpaceUsage(const WebCore::ClientOrigin& origin)
{
//    connection().send(Messages::NetworkConnectionToWebProcess::UpdateQuotaBasedOnSpaceUsageForTesting(origin), 0);
}

}
