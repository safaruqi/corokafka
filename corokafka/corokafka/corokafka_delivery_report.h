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
#ifndef BLOOMBERG_COROKAFKA_DELIVERY_REPORT_H
#define BLOOMBERG_COROKAFKA_DELIVERY_REPORT_H

#include <corokafka/detail/corokafka_namespace_forwarding.h>

namespace Bloomberg {
namespace corokafka {

/**
 * @brief Represents the result of a successful or failed delivery
 * of a message
 */
class DeliveryReport
{
public:
    /**
     * @brief Constructs an instance of a message delivery report
     * @param topicPartition the partition the message was sent to
     * @param error the error associated with the message delivery, if any
     * @param opaque a user-provided pointer to an opaque data associated with the message
     */
    DeliveryReport(TopicPartition topicPartition, Error error, void * opaque);
    /**
     * @brief Gets the message topic partition
     */
    const TopicPartition & getTopicPartition() const;
    /**
     * @brief Gets the message delivery error
     */    
    const Error& getError() const;
    /**
     * @brief Gets the opaque data associated with the message
     */
    void* getOpaque() const;
    
private:
    TopicPartition _topicPartition;
    Error _error;
    void* _opaque;
};
 
}}

#endif // BLOOMBERG_COROKAFKA_DELIVERY_REPORT_H