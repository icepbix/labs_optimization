#include <algorithm>
#include <exception>
#include <functional>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <string>
#include <utility>
#include <vector>

namespace lab1 {

class Lab1 {
   public:
    Lab1(const char* mn, const std::function<double(double)>& f)
        : methodName(mn),
          f(f),
          min({-1, -1}),
          countOfIterations(0),
          countOfCallFunc(0) {}
    virtual bool findMin(const double&, const double&, const double&) = 0;
    friend std::ostream& operator<<(std::ostream& out, const Lab1& l);
    virtual ~Lab1() = default;

   protected:
    const char* methodName;
    std::function<double(double)> f;
    virtual void isValidFunc() const final {
        if (true /*unimodal??*/)
            ;
        else
            throw std::runtime_error("Функция не унимодальна!");
    }
    std::pair<double, double> min;
    int countOfIterations;
    int countOfCallFunc;
};

std::ostream& operator<<(std::ostream& out, const Lab1& l) {
    out << l.methodName << std::endl;
    out << std::setprecision(5) << "x: " << l.min.first
        << "\nf(x): " << l.min.second
        << "\nКоличество итераций: " << l.countOfIterations
        << "\nКоличество вызова функций: " << l.countOfCallFunc << std::endl;
    return out;
}

std::function<double(double)> func = [](double i) { return i * i * i * i - i; };

class Method1 : public Lab1 {
   public:
    Method1(const std::function<double(double)>& f = func)
        : Lab1("Метод равномерного поиска", f) {}
    bool findMin(const double& a, const double& b, const double& e) {
        try {
            isValidFunc();
        } catch (const std::exception& e) {
            std::cout << e.what() << std::endl;
            return false;
        }

        std::vector<std::pair<double, double>> v;
        for (double i = a; i < b; i += e) {
            v.push_back(std::pair<double, double>(i, f(i)));
            ++countOfIterations;
        }
        countOfCallFunc = countOfIterations;
        std::sort(v.begin(), v.end(),
                  [](const std::pair<double, double>& p1,
                     const std::pair<double, double>& p2) {
                      return p1.second < p2.second;
                  });

        min = v.front();

        return true;
    }
};

class Method2 : public Lab1 {
   public:
    Method2(const std::function<double(double)>& f = func)
        : Lab1("Метод дихотомии", f) {}
    virtual bool findMin(const double& a_, const double& b_, const double& e_) {
        try {
            isValidFunc();
        } catch (const std::exception& e) {
            std::cout << e.what() << std::endl;
            return false;
        }

        double a{a_}, b{b_}, e{e_}, d(e_ / 2), x1{0}, x2{0}, f1, f2;
        f1 = f2 = 0;
        e = (b - a) / 2;
        while (e > e_) {
            e = (b - a) / 2;
            x1 = (a + b - d) / 2;
            x2 = (a + b + d) / 2;
            f1 = f(x1);
            f2 = f(x2);
            if (f1 < f2)
                b = x2;
            else
                a = x1;
            ++countOfIterations;
        }
        countOfCallFunc = countOfIterations * 2;

        double tmp = (x1 + x2) / 2;
        min = std::make_pair(tmp, f(tmp));

        return true;
    }
};

// TODO: Method3 - метод квадратичной интерполяции

};  // namespace lab1

int main() {
    using namespace lab1;

    const double a = 0, b = 1, epsilon = 0.0001;

    Method1 m1;
    if (m1.findMin(a, b, epsilon)) {
        std::cout << m1 << std::endl;
    }

    Method2 m2;
    if (m2.findMin(a, b, epsilon)) {
        std::cout << m2 << std::endl;
    }

    return 0;
}

