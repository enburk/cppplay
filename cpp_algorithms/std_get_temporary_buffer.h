// A sad story about get_temporary_buffer
// https://www.reddit.com/r/programming/comments/3qtkce/a_sad_story_about_get_temporary_buffer/
// 
// Quote from From Mathematics to Generic Programming: http://www.fm2gp.com/
//  When the C++ STL library was designed by the first author of this book,
//  he realized that it would be helpful to have a function get_temporary_buffer
//  that takes a size n and returns the largest available buffer up to size n that fits into physical memory.
//  As a placeholder (since the correct version needs to have knowledge that only the operating system has),
//  he wrote a simplistic and impractical implementation, which repeatedly calls malloc
//  asking for initially huge and gradually smaller chunks of memory until it returns a valid pointer.
//  He put a prominent comment in the code saying something like, "This is a bogus implementation, replace it!"
//  To his surprise, he discovered years later that all the major vendors that provide STL implementations
//  are still using this terrible implementation—but they removed his comment.

// https://en.cppreference.com/w/cpp/memory/get_temporary_buffer
// std::get_temporary_buffer : deprecated in C++17, removed in C++20
// std::return_temporary_buffer : deprecated in C++17, removed in C++20

// VS 2017 implementation:

#pragma warning (push, 0)
#pragma warning (disable : 26409) // Avoid calling new and delete explicitly, use std::make_unique<T> instead
#pragma warning (disable : 26400) // Do not assign the result of an allocation to a raw pointer, use owner<T> instead
#pragma warning (disable : 26472) // Don't use a static_cast for arithmetic conversions

template<class _Ty> inline
	std::pair<_Ty *, ptrdiff_t> std_get_temporary_buffer(ptrdiff_t _Count) noexcept
	{	// get raw temporary buffer of up to _Count elements
	if (static_cast<size_t>(_Count) <= static_cast<size_t>(-1) / sizeof(_Ty))
		{
		for (; 0 < _Count; _Count /= 2)
			{
			const auto _Size = static_cast<size_t>(_Count) * sizeof(_Ty);
			void * _Pbuf;
				{
				_Pbuf = ::operator new(_Size, std::nothrow);
				}
			if (_Pbuf)
				{
				return {static_cast<_Ty *>(_Pbuf), _Count};
				}
			}
		}

	return {nullptr, 0};
	}

template<class _Ty> inline
	void std_return_temporary_buffer(_Ty * const _Pbuf) noexcept
	{	// delete raw temporary buffer
		::operator delete(_Pbuf);
	}

TEST_OFF
{
    constexpr auto n = std::numeric_limits<ptrdiff_t>::max();

    const auto p = std_get_temporary_buffer<std::byte>(n);

    cout << "get_temporary_buffer: asked for " << n << " bytes" << endl;
    cout << "get_temporary_buffer: achieved " << p.second << " bytes" << endl;

    std_return_temporary_buffer(p.first);
};

// Possible output:
// get_temporary_buffer: asked for 9223372036854775807 bytes
// get_temporary_buffer: achieved 17179869183 bytes

#pragma warning (pop)
