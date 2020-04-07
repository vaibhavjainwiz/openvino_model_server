//*****************************************************************************
// Copyright 2020 Intel Corporation
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//*****************************************************************************

#include <condition_variable>
#include <mutex>
#include <vector>
#include <thread>
#include <atomic>

namespace ovms {
/**
* @brief Class representing circular buffer for managing IE streams
*/
class OVStreamsQueue {
 public:
    /**
    * @brief Allocating idle stream for execution
    */
    int getIdleStream();

    /**
    * @brief Release stream after execution
    */
    void returnStream(int streamID);

    /**
    * @brief Sends notification from callback function to predict
    */
    void signalCompletedInference(int streamID);

    /**
    * @brief Wait for async callback notification
    */
    void waitForAsync(int streamID, std::mutex &mx);

    /**
    * @brief Constructor with initialization
    */
    OVStreamsQueue(int streamsLength) :
        front_idx{0},
        back_idx{0},
        streams(streamsLength),
        activeStreams(streamsLength)
    {
        for (int i = 0; i <= streamsLength - 1; i++) {
            streams.push_back(i);
        }
    };

 protected:
    /**
    * @brief Vector representing circular buffer for streams queue
    */
    std::vector<int> streams;

    /**
    * @brief Index of the front of the idle streams list
    */
    std::uint32_t front_idx;

    /**
    * @brief Index of the back of the idle streams list
    */
    std::atomic<std::uint32_t> back_idx;

    /**
    * @brief Vector representing OV streams and used for notification about completed inference operations
    */
    std::vector<std::condition_variable> activeStreams;
    std::mutex front_mut;
    std::condition_variable not_full_cond;
}
