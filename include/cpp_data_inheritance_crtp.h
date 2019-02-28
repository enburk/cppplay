// https://en.wikipedia.org/wiki/Curiously_recurring_template_pattern


General form
// The Curiously Recurring Template Pattern (CRTP)
template<class T>
class Base
{
    // methods within Base can use template to access members of Derived
};
class Derived : public Base<Derived>
{
    // ...
};



Static polymorphism
Typically, the base class template will take advantage of the fact that member function bodies (definitions) are not instantiated until long after their declarations, and will use members of the derived class within its own member functions, via the use of a cast; e.g.:

template <class T> 
struct Base
{
    void interface()
    {
        // ...
        static_cast<T*>(this)->implementation();
        // ...
    }

    static void static_func()
    {
        // ...
        T::static_sub_func();
        // ...
    }
};

struct Derived : Base<Derived>
{
    void implementation();
    static void static_sub_func();
};




template <typename T>
struct counter
{
    static int objects_created;
    static int objects_alive;

    counter()
    {
        ++objects_created;
        ++objects_alive;
    }
    
    counter(const counter&)
    {
        ++objects_created;
        ++objects_alive;
    }
protected:
    ~counter() // objects should never be removed through pointers of this type
    {
        --objects_alive;
    }
};
template <typename T> int counter<T>::objects_created( 0 );
template <typename T> int counter<T>::objects_alive( 0 );

class X : counter<X>
{
    // ...
};

class Y : counter<Y>
{
    // ...
};




The CRTP can be used to avoid such problem and to implement "Polymorphic chaining":[11]

// Base class
template <typename ConcretePrinter>
class Printer
{
public:
    Printer(ostream& pstream) : m_stream(pstream) {}
 
    template <typename T>
    ConcretePrinter& print(T&& t)
    {
        m_stream << t;
        return static_cast<ConcretePrinter&>(*this);
    }
 
    template <typename T>
    ConcretePrinter& println(T&& t)
    {
        m_stream << t << endl;
        return static_cast<ConcretePrinter&>(*this);
    }
private:
    ostream& m_stream;
};
 
// Derived class
class CoutPrinter : public Printer<CoutPrinter>
{
public:
    CoutPrinter() : Printer(cout) {}
 
    CoutPrinter& SetConsoleColor(Color c)
    {
        // ...
        return *this;
    }
};
 
// usage
CoutPrinter().print("Hello ").SetConsoleColor(Color.red).println("Printer!");





// Base class has a pure virtual function for cloning
class Shape {
public:
    virtual ~Shape() {};
    virtual Shape *clone() const = 0;
};
// This CRTP class implements clone() for Derived
template <typename Derived>
class Shape_CRTP : public Shape {
public:
    virtual Shape *clone() const {
        return new Derived(static_cast<Derived const&>(*this));
    }
};

// Nice macro which ensures correct CRTP usage
#define Derive_Shape_CRTP(Type) class Type: public Shape_CRTP<Type>

// Every derived class inherits from Shape_CRTP instead of Shape
Derive_Shape_CRTP(Square) {};
Derive_Shape_CRTP(Circle) {};
This allows obtaining copies of squares, circles or any other shapes by shapePtr->clone().

Pitfalls
One issue with static polymorphism is that without using a general base class like "Shape" from the above example, derived classes cannot be stored homogeneously as each CRTP base class is a unique type.
For this reason, it is more common to inherit from a shared base class with a virtual destructor, like the example above.



// http://b.atch.se/posts/constexpr-counter/

HOW TO IMPLEMENT A CONSTANT-EXPRESSION COUNTER IN C++

// https://stackoverflow.com/questions/8596490/counting-with-template-metaprogramming/8596825#8596825

class Base
{
    virtual int getId() const = 0;
};

// Returns 0, 1, 2 etc. on each successive call.
static int makeUniqueId()
{
    static int id = 0;
    return id++;
}

template <typename Derived>
class BaseWithId : public Base
{
    static int getStaticId()
    {
        static int id = makeUniqueId();
        return id;
    }

    int getId() const { return getStaticId(); }
};

class Derived1 : public BaseWithId<Derived1> { ... };
class Derived2 : public BaseWithId<Derived2> { ... };
class Derived3 : public BaseWithId<Derived3> { ... };
This gives you unique IDs for each class:

Derived1::getStaticId(); // 0
Derived2::getStaticId(); // 1
Derived3::getStaticId(); // 2
However, those IDs are assigned lazily, so the order you call getId() affects the ID's returned.

Derived3::getStaticId(); // 0
Derived2::getStaticId(); // 1
Derived1::getStaticId(); // 2
Whether or not this is OK for your application depends on your particular needs (e.g. would be no good for serialisation).