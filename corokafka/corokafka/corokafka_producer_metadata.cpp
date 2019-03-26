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
#include <corokafka/corokafka_producer_metadata.h>

namespace Bloomberg {
namespace corokafka {

//=============================================================================
//                              PRODUCER METADATA
//=============================================================================

ProducerMetadata::ProducerMetadata(const std::string& topic,
                                   BufferedProducer<ByteArray>& producer) :
    Metadata(topic, Topic(), producer.get_producer()),
    _bufferedProducer(producer)
{
}

ProducerMetadata::ProducerMetadata(const std::string& topic,
                                   const Topic& kafkaTopic,
                                   BufferedProducer<ByteArray>& producer) :
    Metadata(topic, kafkaTopic, producer.get_producer()),
    _bufferedProducer(producer)
{
}

const TopicPartitionList& ProducerMetadata::getTopicPartitions() const
{
    if (_partitions.empty()) {
        for (const auto& meta : getTopicMetadata().get_partitions()) {
            _partitions.emplace_back(_topic, meta.get_id());
        }
    }
    return _partitions;
}

Metadata::OffsetWatermarkList ProducerMetadata::queryOffsetWatermarks() const
{
    OffsetWatermarkList offsets;
    for (const auto& partition : getTopicPartitions()) {
        offsets.emplace_back(partition.get_partition(), _handle.query_offsets(partition));
    }
    return offsets;
}

TopicPartitionList ProducerMetadata::queryOffsetsAtTime(Timestamp timestamp) const
{
    KafkaHandleBase::TopicPartitionsTimestampsMap timestampMap;
    std::chrono::milliseconds epochTime = timestamp.time_since_epoch();
    for (const auto& partition : getTopicPartitions()) {
        timestampMap[partition] = epochTime;
    }
    return _handle.get_offsets_for_times(timestampMap);
}

size_t ProducerMetadata::getOutboundQueueLength() const
{
    return _handle.get_out_queue_length();
}

size_t ProducerMetadata::getInternalQueueLength() const
{
    return _bufferedProducer.get_buffer_size();
}

}
}
