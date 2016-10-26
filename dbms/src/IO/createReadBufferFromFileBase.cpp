#include <DB/IO/createReadBufferFromFileBase.h>
#include <DB/IO/ReadBufferFromFile.h>
#ifndef __APPLE__
#include <DB/IO/ReadBufferAIO.h>
#endif
#include <DB/Common/ProfileEvents.h>


namespace ProfileEvents
{
	extern const Event CreatedReadBufferOrdinary;
	extern const Event CreatedReadBufferAIO;
}

namespace DB
{
#ifdef __APPLE__
namespace ErrorCodes
{
        extern const int NOT_IMPLEMENTED;
}
#endif

std::unique_ptr<ReadBufferFromFileBase> createReadBufferFromFileBase(const std::string & filename_, size_t estimated_size,
		size_t aio_threshold, size_t buffer_size_, int flags_, char * existing_memory_, size_t alignment)
{
	if ((aio_threshold == 0) || (estimated_size < aio_threshold))
	{
		ProfileEvents::increment(ProfileEvents::CreatedReadBufferOrdinary);
		return std::make_unique<ReadBufferFromFile>(filename_, buffer_size_, flags_, existing_memory_, alignment);
	}
	else
	{
#ifndef __APPLE__
		ProfileEvents::increment(ProfileEvents::CreatedReadBufferAIO);
		return std::make_unique<ReadBufferAIO>(filename_, buffer_size_, flags_, existing_memory_);
#else
		throw Exception("AIO is not implemented yet on MacOS X", ErrorCodes::NOT_IMPLEMENTED);
#endif
	}
}

}
