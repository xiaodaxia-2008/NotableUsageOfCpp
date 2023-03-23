/// main.cpp

#include <algorithm>
#include <array>
#include <chaiscript/chaiscript.hpp>
#include <iostream>
#include <numeric>
#include <vector>

double function(int i, double j)
{
    return i * j;
}

double sum(const std::vector<double> &arr)
{
    auto v = std::accumulate(arr.begin(), arr.end(), 0.0);
    std::cout << "the sum is " << v << std::endl;
    return v;
}
// Define the Person class
class Person
{
  public:
    std::string name;

    void say_hello()
    {
        std::cout << "Hello, my name is " << name << std::endl;
    }
};

// Define the Student class, which inherits from Person
class Student : public Person
{
  public:
    int grade_level;

    void study()
    {
        std::cout << name << " is studying at grade level " << grade_level << std::endl;
    };
};

int main()
{
    chaiscript::ChaiScript chai;
    chai.add(chaiscript::vector_conversion<std::vector<int>>());
    chai.add(chaiscript::vector_conversion<std::vector<double>>());
    chai.add(chaiscript::fun(&function), "function");
    chai.add(chaiscript::fun(&sum), "sum");

    // Bind the Person class
    chai.add(chaiscript::user_type<Person>(), "Person");
    chai.add(chaiscript::constructor<Person()>(), "Person");
    chai.add(chaiscript::fun(&Person::say_hello), "say_hello");
    chai.add(chaiscript::fun(&Person::name), "name");

    // Bind the Student class
    chai.add(chaiscript::base_class<Person, Student>());
    chai.add(chaiscript::user_type<Student>(), "Student");
    chai.add(chaiscript::constructor<Student()>(), "Student");
    chai.add(chaiscript::fun(&Student::study), "study");
    chai.add(chaiscript::fun(&Student::grade_level), "grade_level");
    chai.add(chaiscript::fun([](Person &p) { std::cout << "Hi, I'm " << p.name << std::endl; }), "greet");

    double d = chai.eval<double>("function(3, 4.75);");
    std::cout << d << std::endl;
    auto alice = Person();
    auto jack = Student();
    Person* pjack = &jack;
    jack.name = "jack";
    chai.add(chaiscript::var(std::ref(alice)), "alice");
    chai.add(chaiscript::var(pjack), "jack");
    try
    {
        chai.eval(R"(
alice.name = "Alice";
alice.greet();
function(2.3, 3.6);
alice.say_hello(); // outputs "Hello, my name is Alice"
var vec = [1.2, 3.0, 5];
sum(vec);

var bob = Student();
bob.name = "Bob";
bob.grade_level = 10;
bob.study(); // outputs "Bob is studying at grade level 10"
jack.study();

    )");
    }
    catch (const chaiscript::exception::eval_error &e)
    {
        std::cout << e.pretty_print() << '\n';
    }

    alice.say_hello();
    return 0;
}