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

#pragma once

#include "CacheStorageEngine.h"
#include <WebCore/CacheStorageConnection.h>
#include <pal/SessionID.h>
#include <wtf/Forward.h>
#include <wtf/RefCounted.h>

namespace WebKit {

class NetworkSession;

class CacheStorageEngineConnection : public RefCounted<CacheStorageEngineConnection> {
public:
    static Ref<CacheStorageEngineConnection> create() { return adoptRef(*new CacheStorageEngineConnection()); }
    ~CacheStorageEngineConnection();

    void open(WebCore::ClientOrigin&&, String&& cacheName, WebCore::DOMCacheEngine::CacheIdentifierCallback&&);
    void remove(uint64_t cacheIdentifier, WebCore::DOMCacheEngine::CacheIdentifierCallback&&);
    void caches(WebCore::ClientOrigin&&, uint64_t updateCounter, WebCore::DOMCacheEngine::CacheInfosCallback&&);

    void retrieveRecords(uint64_t cacheIdentifier, WebCore::RetrieveRecordsOptions&&, WebCore::DOMCacheEngine::RecordsCallback&&);
    void deleteMatchingRecords(uint64_t cacheIdentifier, WebCore::ResourceRequest&&, WebCore::CacheQueryOptions&&, WebCore::DOMCacheEngine::RecordIdentifiersCallback&&);
    void putRecords(uint64_t cacheIdentifier, Vector<WebCore::DOMCacheEngine::Record>&&, WebCore::DOMCacheEngine::RecordIdentifiersCallback&&);

    void reference(uint64_t cacheIdentifier);
    void dereference(uint64_t cacheIdentifier);

    void clearMemoryRepresentation(WebCore::ClientOrigin&&, CompletionHandler<void(std::optional<WebCore::DOMCacheEngine::Error>&&)>&&);
    void engineRepresentation( CompletionHandler<void(String&&)>&&);

private:
    explicit CacheStorageEngineConnection();
   
    PAL::SessionID sessionID() const;

    struct NetworkConnectionDummy {
        NetworkSession* networkSession();
        PAL::SessionID sessionID() const;
    };
    NetworkConnectionDummy m_connection;
    HashMap<CacheStorage::CacheIdentifier, CacheStorage::LockCount> m_cachesLocks;
};

}
