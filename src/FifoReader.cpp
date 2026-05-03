#include "FifoReader.hpp"

#include <fstream>
#include <iostream>

namespace ImpyD
{
    void FifoReader::ReaderThreadFunc(std::stop_token st, std::string fifoPath)
    {
        auto stream = std::fopen(fifoPath.c_str(), "rb");

        std::cout << "Started FIFO reader thread." << std::endl;

        while (!st.stop_requested() && std::fread(buffer.data() + frontIndex, sizeof(int16_t), 2, stream))
        {
            frontIndex = (2 + frontIndex) % buffer.size();
        }
    }

    size_t FifoReader::GetFrontIndex()
    {
        return frontIndex;
    }
}
