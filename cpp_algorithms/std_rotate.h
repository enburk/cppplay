// possible implementation of std::rotate
// https://en.cppreference.com/w/cpp/algorithm/rotate

template <typename I> // ForwardIterator 

auto test_rotate (I first, I will_be_first, I last) -> I
{
   // return: first + (last - will_be_first)

   if (will_be_first == first) return last;
   if (will_be_first == last) return first;
 
   I read      = will_be_first;
   I write     = first;
   I next_read = first; // read position for when "read" hits "last"
 
   /**/ cout << "rotate..." << endl << endl;
 
   while (read != last)
   {
      if (write == next_read) next_read = read; // track where "first" went

      /**/ auto c = [=] (I i) { return
      /**/     i == read      ? 'r' :
      /**/     i == write     ? 'w' :
      /**/     i == next_read ? 'n' : ' '; };
      /**/ 
      /**/ for (auto i=first; i!=last; ++i) cout << c (i) << ' '; cout << endl;
      /**/ for (auto i=first; i!=last; ++i) cout <<   *i  << ' '; cout << endl;

      std::iter_swap (write++, read++);
   }

   /**/ cout << endl;
 
   test_rotate (write, next_read, last); // rotate the remaining sequence into place

   return write;
}

TEST_OFF
{
    std::vector<char> v (26);

    std::iota (v.begin (), v.end (), 'A');

    test_rotate (v.begin (), v.begin () + 5, v.end ());
};

// Output:
// 
// rotate...
// 
// w         r
// A B C D E F G H I J K L M N O P Q R S T U V W X Y Z
//   w       n r
// F B C D E A G H I J K L M N O P Q R S T U V W X Y Z
//     w     n   r
// F G C D E A B H I J K L M N O P Q R S T U V W X Y Z
//       w   n     r
// F G H D E A B C I J K L M N O P Q R S T U V W X Y Z
//         w n       r
// F G H I E A B C D J K L M N O P Q R S T U V W X Y Z
//           w         r
// F G H I J A B C D E K L M N O P Q R S T U V W X Y Z
//             w       n r
// F G H I J K B C D E A L M N O P Q R S T U V W X Y Z
//               w     n   r
// F G H I J K L C D E A B M N O P Q R S T U V W X Y Z
//                 w   n     r
// F G H I J K L M D E A B C N O P Q R S T U V W X Y Z
//                   w n       r
// F G H I J K L M N E A B C D O P Q R S T U V W X Y Z
//                     w         r
// F G H I J K L M N O A B C D E P Q R S T U V W X Y Z
//                       w       n r
// F G H I J K L M N O P B C D E A Q R S T U V W X Y Z
//                         w     n   r
// F G H I J K L M N O P Q C D E A B R S T U V W X Y Z
//                           w   n     r
// F G H I J K L M N O P Q R D E A B C S T U V W X Y Z
//                             w n       r
// F G H I J K L M N O P Q R S E A B C D T U V W X Y Z
//                               w         r
// F G H I J K L M N O P Q R S T A B C D E U V W X Y Z
//                                 w       n r
// F G H I J K L M N O P Q R S T U B C D E A V W X Y Z
//                                   w     n   r
// F G H I J K L M N O P Q R S T U V C D E A B W X Y Z
//                                     w   n     r
// F G H I J K L M N O P Q R S T U V W D E A B C X Y Z
//                                       w n       r
// F G H I J K L M N O P Q R S T U V W X E A B C D Y Z
//                                         w         r
// F G H I J K L M N O P Q R S T U V W X Y A B C D E Z
// 
// rotate...
// 
// w       r
// B C D E A
// 
// rotate...
// 
// w     r
// C D E B
// 
// rotate...
// 
// w   r
// D E C
// 
// rotate...
// 
// w r
// E D

template <typename I> // ForwardIterator 

auto test_rotate_(I first, I will_be_first, I last) -> I 
{
   if (will_be_first == first) return last;
   if (will_be_first == last) return first;
 
   I read      = will_be_first;
   I write     = first;
   I next_read = first; // read position for when "read" hits "last"
 
   while (read != last) {
      if (write == next_read) next_read = read; // track where "first" went
      std::iter_swap (write++, read++);
   }
 
   // rotate the remaining sequence into place
   return test_rotate_(write, next_read, last);
}

TEST_OFF
{
    std::vector<char> v0 (10'000);

    std::iota (v0.begin (), v0.end (), 0);

    auto v1 = v0, v2 = v0; 

    Time t0; test_rotate_(v1.begin(), v1.begin()+v0.size()/3, v1.end());
    Time t1; std::rotate (v2.begin(), v2.begin()+v0.size()/3, v2.end());
    Time t2;

    cout << "test_rotate " << t1-t0 << " sec" << endl;
    cout << "std::rotate " << t2-t1 << " sec" << endl;
    cout <<  endl;

    assert (v1 == v2);
};

// Possible output:
// 
// test_rotate 0.000'169'967 sec
// std::rotate 0.000'001'232 sec
