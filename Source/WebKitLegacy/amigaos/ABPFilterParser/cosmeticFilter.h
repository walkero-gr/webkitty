/* Copyright (c) 2015 Brian R. Bondy. Distributed under the MPL2 license.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#pragma once

#include <math.h>
#include <string.h>
#include "hash_set.h"

namespace ABP {

class CosmeticFilter {
 public:
  uint64_t GetHash() const;

  ~CosmeticFilter() {
    if (data) {
      delete[] data;
    }
  }
  explicit CosmeticFilter(const char *data) {
    size_t len = strlen(data) + 1;
    this->data = new char[len];
    snprintf(this->data, len, "%s", data);
  }

  CosmeticFilter(const CosmeticFilter &rhs) {
    data = new char[strlen(rhs.data) + 1];
    memcpy(data, rhs.data, strlen(rhs.data) + 1);
  }

  CosmeticFilter() : data(nullptr) {
  }

  bool operator==(const CosmeticFilter &rhs) const {
    return !strcmp(data, rhs.data);
  }

  bool operator!=(const CosmeticFilter &rhs) const {
    return !(*this == rhs);
  }

  // Nothing needs to be updated for multiple adds
  void Update(const CosmeticFilter &) {}

  uint32_t Serialize(char *buffer) {
    if (buffer) {
      memcpy(buffer, data, strlen(data) + 1);
    }
    return static_cast<uint32_t>(strlen(data)) + 1;
  }

  uint32_t Deserialize(char *buffer, uint32_t /*bufferSize*/) {
    int len = static_cast<int>(strlen(buffer));
    data = new char[len + 1];
    memcpy(data, buffer, len + 1);
    return len + 1;
  }

  char *data;
};

class CosmeticFilterHashSet : public HashSet<CosmeticFilter> {
 public:
  CosmeticFilterHashSet() : HashSet<CosmeticFilter>(1000, false) {
  }
  char * toStylesheet(uint32_t *len) {
    *len = fillStylesheetBuffer(nullptr);
    char *buffer = new char[*len];
    memset(buffer, 0, *len);
    fillStylesheetBuffer(buffer);
    return buffer;
  }

 private:
  uint32_t fillStylesheetBuffer(char *buffer) {
    uint32_t len = 0;
    for (uint32_t bucketIndex = 0; bucketIndex < bucket_count_; bucketIndex++) {
      HashItem<CosmeticFilter> *hashItem = buckets_[bucketIndex];
      len = 0;
      while (hashItem) {
        CosmeticFilter *cosmeticFilter = hashItem->hash_item_storage_;
        // [cosmeticFilter],[space]
        int cosmeticFilterLen =
          static_cast<int>(strlen(cosmeticFilter->data));
        if (buffer) {
          memcpy(buffer + len, cosmeticFilter->data, cosmeticFilterLen);
        }
        len += cosmeticFilterLen;
        if (hashItem->next_) {
          if (buffer) {
            memcpy(buffer + len, ", ", 2);
          }
          len += 2;
        }
        hashItem = hashItem->next_;
      }
    }
    return len;
  }
};

}

