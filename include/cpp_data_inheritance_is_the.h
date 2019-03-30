// Sean Parent - Inheritance is the base class of evil [GN 2013]
// Sean Parent - Better Code: Runtime Polymorphism [NDC 2017]
namespace iibce
{
    using namespace std;

    class my_class_t { /* ... */ };
    void draw(const my_class_t&, ostream& out, size_t position)
    { out << string(position, ' ') << "my_class_t" << endl; }

    template <typename T>
    void draw(const T& x, ostream& out, size_t position)
    { out << string(position, ' ') << x << endl; }

    struct object_t
    {
        template<class type> object_t (type data) : self (std::make_shared<model<type>>(std::move (data))) {}

        friend void draw(const object_t& x, ostream& out, size_t position)
        { x.self->draw_(out, position); }

        private: struct concept
        {
            virtual ~concept () = default;
            virtual void draw_(ostream&, size_t) const = 0;
        };
        template<class type> struct model final : public concept
        {
            type data;
            model (type data) noexcept : data (std::move (data)) {}
            void draw_(ostream& out, size_t position) const override
            { draw(data, out, position); }
        };

        std::shared_ptr<const concept> self;
    };

    using document_t = vector<object_t>;

    void draw(const document_t& x, ostream& out, size_t position)
    {
        out << string(position, ' ') << "<document>" << endl;
        for (const auto& e: x) draw(e, out, position + 2);
        out << string(position, ' ') << "</document>" << endl;
    }

    TEST_OFF
    {
        document_t document;

        document.emplace_back(0);
        document.emplace_back(string("Hello!"));
        document.emplace_back(document);
        document.emplace_back(my_class_t());

        draw(document, cout, 0);
    };

    using history_t = vector<document_t>;
    document_t& current (history_t& x) { assert(x.size()); return x.back(); }
    void        commit (history_t& x) { assert(x.size()); x.push_back(x.back()); }
    void        undo (history_t& x) { assert(x.size()); x.pop_back(); }

    TEST_OFF
    {
        history_t h(1);

        current(h).emplace_back(0);
        current(h).emplace_back(string("Hello!"));

        draw(current(h), cout, 0);
        cout << "--------------------------" << endl;
        
        commit(h);
        
        current(h)[0] = 42.5;

        auto saving = async([document = current(h)]() {
            this_thread::sleep_for(chrono::seconds(3));
            cout << "-- 'save' --" << endl;
            draw(document, cout, 0);
        });

        current(h)[1] = string("World");
        current(h).emplace_back(current(h));
        current(h).emplace_back(my_class_t());
        
        draw(current(h), cout, 0);
        cout << "--------------------------" << endl;
        
        undo(h);
        draw(current(h), cout, 0);
    };

    TEST_ON
    {
        // oops ( object_t o = element ('1'); ), "", { "ctor: 1", "move: 1", "move: 1", "dtor: ",  "dtor: ", "-------", "dtor: 1" });
    };
}
