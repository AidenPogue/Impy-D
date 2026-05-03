#ifndef IM_MPD_FIFOREADER_HPP
#define IM_MPD_FIFOREADER_HPP
#include <mutex>
#include <thread>
#include <vector>
#include <boost/circular_buffer.hpp>

namespace ImpyD
{
    // Manages a worker thread to read a named pipe/fifo from MPD and provides access to a buffer of its data.
    class FifoReader
    {
    public:

        // Circular buffer of the last .size() samples. Samples with LOWER indices are OLDER.
        // Caller can assume this is always full.
        std::vector<int16_t> buffer;

    private:
        size_t frontIndex = 0;
        std::jthread readerThread;

        void ReaderThreadFunc(std::stop_token st, std::string fifoPath);

    public:
        FifoReader(int bufferSize, std::string fifoPath): buffer(bufferSize), readerThread([this, fifoPath](std::stop_token st) {ReaderThreadFunc(st, fifoPath);})
        {
            buffer.resize(bufferSize);
        }

        // Gets the index in buffer of the latest sample.
        size_t GetFrontIndex();
    };
} // ImpyD

#endif //IM_MPD_FIFOREADER_HPP