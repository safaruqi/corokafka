/*
** Copyright 2019 Bloomberg Finance L.P.
**
** Licensed under the Apache License, Version 2.0 (the "License");
** you may not use this file except in compliance with the License.
** You may obtain a copy of the License at
**
**     http://www.apache.org/licenses/LICENSE-2.0
**
** Unless required by applicable law or agreed to in writing, software
** distributed under the License is distributed on an "AS IS" BASIS,
** WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
** See the License for the specific language governing permissions and
** limitations under the License.
*/
#ifndef BLOOMBERG_COROKAFKA_PRODUCER_TOPIC_ENTRY_H
#define BLOOMBERG_COROKAFKA_PRODUCER_TOPIC_ENTRY_H

#include <memory>
#include <chrono>
#include <corokafka/corokafka_utils.h>
#include <corokafka/corokafka_message.h>
#include <corokafka/corokafka_producer_configuration.h>
#include <corokafka/corokafka_connector_configuration.h>
#include <quantum/quantum.h>

using namespace BloombergLP;

namespace Bloomberg {
namespace corokafka {

using ProducerType = BufferedProducer<ByteArray>;
using ProducerPtr = std::unique_ptr<ProducerType>;

enum class QueueFullNotification {
    OncePerMessage,
    EachOccurence,
    EdgeTriggered
};

struct ProducerTopicEntry : TopicEntry {
    ProducerTopicEntry(ProducerPtr producer,
                       const ConnectorConfiguration& connectorConfiguration,
                       const ProducerConfiguration& configuration) :
        _connectorConfiguration(connectorConfiguration),
        _configuration(configuration),
        _producer(std::move(producer))
    {}
    ProducerTopicEntry(ProducerPtr producer,
                       const ConnectorConfiguration& connectorConfiguration,
                       ProducerConfiguration&& configuration) :
        _connectorConfiguration(connectorConfiguration),
        _configuration(std::move(configuration)),
        _producer(std::move(producer))
    {}
    const ConnectorConfiguration&       _connectorConfiguration;
    ProducerConfiguration               _configuration;
    ProducerPtr                         _producer;
    size_t                              _topicHash{0};
    quantum::ThreadFuture<int>::Ptr     _pollFuture{nullptr};
    std::chrono::steady_clock::time_point _throttleTime;
    std::chrono::milliseconds           _throttleDuration{0};
    bool                                _autoThrottle{false};
    uint16_t                            _throttleMultiplier{1};
    bool                                _waitForAcks{false};
    bool                                _flushWaitForAcks{false};
    std::chrono::milliseconds           _waitForAcksTimeout{0};
    std::chrono::milliseconds           _flushWaitForAcksTimeout{0};
    bool                                _forceSyncFlush{false};
    bool                                _preserveMessageOrder{false};
    bool                                _skipUnknownHeaders{true};
    Producer::PayloadPolicy             _payloadPolicy{Producer::PayloadPolicy::COPY_PAYLOAD};
    size_t                              _maxQueueLength{10000};
    LogLevel                            _logLevel{LogLevel::LogInfo};
    QueueFullNotification               _queueFullNotification{QueueFullNotification::OncePerMessage};
    bool                                _queueFullTrigger{true};
};

}
}

#endif //BLOOMBERG_COROKAFKA_PRODUCER_TOPIC_ENTRY_H