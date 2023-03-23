#include <sol/sol.hpp>
#include <string>
#include <vector>

class Person
{
  public:
    std::string name_;
    int age_;

    Person() = default;
    Person(std::string name, int age) : name_(name), age_(age)
    {
    }

    Person(std::string name) : name_(name), age_(22)
    {
    }
    std::vector<double> GetNumbs() const
    {
        return {-1.3, 3.3, 3.9};
    }

    virtual std::string GetName() const
    {
        return name_;
    }
    virtual int GetAge() const
    {
        return age_;
    }
    virtual std::string GetOccupation() const
    {
        return "Unemployed";
    }
};

class Student : public Person
{
  private:
    std::string major_;

  public:
    Student() = default;
    Student(std::string name, int age, std::string major) : Person(name, age), major_(major)
    {
    }

    std::string GetMajor() const
    {
        return major_;
    }
    std::string GetOccupation() const override
    {
        return "Student";
    }
};

int main()
{
    sol::state lua;
    lua.open_libraries(sol::lib::base);

    lua.new_usertype<Person>("Person", "name", &Person::name_, "get_name", &Person::GetName, "get_numbs",
                             &Person::GetNumbs, "get_age", &Person::GetAge, "get_occupation", &Person::GetOccupation,
                             "age2", [](Person &self) { return 2 * self.GetAge(); });
    lua["Person"]["test"] = [](Person &self) { std::cout << "person's name is " << self.GetName() << std::endl; };

    auto c = [](int age_) -> Student { return Student("haha", age_, "cs"); };
    auto s = c(10);

    lua.new_usertype<Student>("Student", sol::constructors<Student(std::string, int, std::string)>(), sol::base_classes,
                              sol::bases<Person>(), "major", &Student::GetMajor);

    lua.script(R"(
        s = Student.new("Mike", 20, "CS")
        p = Person.new("social")
        p2 = Person.new("xi")
        print(s:get_name())
        print(p:get_name())
        print(p2:get_name())
        s:test()
        print(s:get_age())
        print(s:age2())
        v = s:get_numbs()
        print(v:at(1))
        print(v:at(2))
        print(v:at(3))
        print(s:major())
        print(s:get_occupation())
    )");
    return 0;
}
