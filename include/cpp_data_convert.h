// Kevlin Henney - boost::lexical_cast
template<typename Target, typename Source>
Target lexical_cast(Source arg)
{
  std::stringstream interpreter;
  Target result;

  if(!(interpreter << arg) ||
     !(interpreter >> result) ||
     !(interpreter >> std::ws).eof())
    throw bad_lexical_cast();

  return result;
}

    if (true)
    {
        std::array<char, 64> str;
        if(auto [p, ec] = std::to_chars(str.data(), str.data() + str.size(), a, std::chars_format::hex);
            ec == std::errc())
             std::cout << std::string_view(str.data(), p - str.data());
        else std::cout << std::string_view(str.data(), p - str.data());
    }


// https://stackoverflow.com/questions/5100718/integer-to-hex-string-in-c

template <typename I> std::string n2hexstr(I w, size_t hex_len = sizeof(I)<<1) {
    static const char* digits = "0123456789ABCDEF";
    std::string rc(hex_len,'0');
    for (size_t i=0, j=(hex_len-1)*4 ; i<hex_len; ++i,j-=4)
        rc[i] = digits[(w>>j) & 0x0f];
    return rc;
}

template <typename T>
inline std::string int_to_hex(T val, size_t width=sizeof(T)*2)
{
    std::stringstream ss;
    ss << std::setfill('0') << std::setw(width) << std::hex << (val|0);
    return ss.str();
}

TEST_ASSERT(int_to_hex(char(0x12)) == "12");
TEST_ASSERT(int_to_hex(short(0x1234)) == "1234");
TEST_ASSERT(int_to_hex(long(0x12345678)) == "12345678");
TEST_ASSERT(int_to_hex((long long)(0x123456789abcdef0)) == "123456789abcdef0");
TEST_ASSERT(int_to_hex(0x123, 1) == "123");
TEST_ASSERT(int_to_hex(0x123, 8) == "00000123");
TEST_ASSERT(int_to_hex(short(0x12)) == "0012"); 

https://en.cppreference.com/w/cpp/io/manip/quoted

{
    std::stringstream ss;
    std::string in = "String with spaces, and embedded \"quotes\" too";
    std::string out;
 
    ss << std::quoted(in);
    std::cout << "read in     [" << in << "]\n"
              << "stored as   [" << ss.str() << "]\n";
 
    ss >> std::quoted(out);
    std::cout << "written out [" << out << "]\n";

// Output:
// 
// read in     [String with spaces, and embedded "quotes" too]
// stored as   ["String with spaces, and embedded \"quotes\" too"]
// written out [String with spaces, and embedded "quotes" too]
}