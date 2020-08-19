// speedtest.cpp
// Quick and dirty speedtest of many write() calls versus a single call to writev().
//
// Build
//  g++ -Wall -Werror -std=c++20 speedtest.cpp -o speedtest.out

#include <chrono>
#include <thread>
#include <vector>
#include <cstdint>
#include <cstdlib>
#include <iostream>

#include <errno.h>
#include <unistd.h>

#include <fcntl.h>
#include <limits.h>
#include <sys/uio.h>
#include <sys/stat.h>
#include <sys/types.h>

// readv() / writev() only accept an array of IO vectors
// that is less than or equal to IOV_MAX in length.
constexpr static auto const N_BUFFERS  = IOV_MAX;
constexpr static auto const BUFFER_LEN = 1 << 10;

constexpr static char const* VANILLA_FNAME = "vanilla.out";
constexpr static char const* VECTOR_FNAME  = "vector.out";

struct system_error
{
    explicit system_error(int const code_ = errno)
        : code{code_} {}

    int const get() const noexcept
    {
        return code;
    }

private:
    int const code;
};

template <typename Precision>
struct scoped_timer
{
    using time_point_t = std::chrono::high_resolution_clock::time_point;

    scoped_timer(std::uint64_t& duration_)
        : duration{duration_}
        , begin{std::chrono::high_resolution_clock::now()} {}

    ~scoped_timer()
    {
        auto const end   = std::chrono::high_resolution_clock::now();
        auto const delta = std::chrono::duration_cast<Precision>(end - begin);
        duration = delta.count();
    }

    scoped_timer(scoped_timer const&)            = delete;
    scoped_timer& operator=(scoped_timer const&) = delete;

    scoped_timer(scoped_timer&&)            = delete;
    scoped_timer& operator=(scoped_timer&&) = delete;

private:
    // A reference to the output duration.
    std::uint64_t& duration;

    // The begin time for the scoped timer.
    time_point_t begin;
};

template <std::size_t BufferLength>
struct static_buffer
{
    std::size_t const length{BufferLength};
    char buffer[BufferLength];
};

// Invoke write() in a loop until the entire buffer is written to disk.
void write_all(int fd, void const* buffer, std::size_t len)
{
    ssize_t written = 0;
    while (static_cast<std::size_t>(written) < len)
    {
        ssize_t const n_bytes = ::write(fd, reinterpret_cast<char const*>(buffer) + written, len - written);
        if (-1 == n_bytes)
        {
            throw system_error{};
        }
        written += n_bytes;
    }
}

void read_all(int fd, void* buffer, std::size_t len)
{
    ssize_t read = 0;
    while (static_cast<std::size_t>(read) < len)
    {
        ssize_t const n_bytes = ::read(fd, reinterpret_cast<char*>(buffer) + read, len - read);
        if (-1 == n_bytes)
        {
            throw system_error{};
        }
        read += n_bytes;
    }
}

void fill_buffers(std::vector<std::unique_ptr<static_buffer<BUFFER_LEN>>>& buffers)
{
    int const fd = ::open("/dev/urandom", O_RDONLY);
    for (auto i = 0ul; i < N_BUFFERS; ++i)
    {
        buffers.push_back(std::make_unique<static_buffer<BUFFER_LEN>>());
        read_all(fd, buffers.back()->buffer, buffers.back()->length);
    }

    ::close(fd);
}

void time_vanilla_write(
    std::vector<std::unique_ptr<static_buffer<BUFFER_LEN>>>& buffers, 
    std::uint64_t& duration)
{
    int const fd = ::open(VANILLA_FNAME, O_CREAT | O_WRONLY);
    if (-1 == fd)
    {
        throw system_error{};
    }

    {
        scoped_timer<std::chrono::milliseconds> timer{duration};
        for (auto& buffer : buffers)
        {
            write_all(fd, buffer->buffer, buffer->length);
        }
    }

    ::close(fd);
}

void time_vector_write(
    std::vector<std::unique_ptr<static_buffer<BUFFER_LEN>>>& buffers, 
    std::uint64_t& duration)
{
    int const fd = ::open(VECTOR_FNAME, O_CREAT | O_WRONLY);
    if (-1 == fd)
    {
        throw system_error{};
    }

    std::vector<struct iovec> vectors{};
    vectors.reserve(buffers.size());
    for (auto i = 0ul; i < buffers.size(); ++i)
    {
        vectors.emplace_back(buffers[i]->buffer, buffers[i]->length);
    }

    {
        scoped_timer<std::chrono::milliseconds> timer{duration};
        ssize_t const n_written = ::writev(fd, &vectors[0], vectors.size());
        if (-1 == n_written)
        {
            throw system_error{};
        }
    }

    ::close(fd);
}

int main()
{
    std::vector<std::unique_ptr<static_buffer<BUFFER_LEN>>> buffers{};
    fill_buffers(buffers);

    std::uint64_t vanilla_duration{};
    time_vanilla_write(buffers, vanilla_duration);

    std::uint64_t vector_duration{};
    time_vector_write(buffers, vector_duration);

    std::cout 
        << "vanilla duration: " << vanilla_duration << "ms\n"
        << "vector duration:  " << vector_duration << "ms\n";

    return EXIT_SUCCESS;
}