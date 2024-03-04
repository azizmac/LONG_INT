#include <vector>
#include <string>
#include <ostream>
#include <iomanip>
#include <sstream>

class LONG_INT {
    // основание системы счисления (1 000 000 000)
    static const int BASE = 1000000000;

    // внутреннее хранилище числа
    std::vector<int> _digits;

    // знак числа
    bool _is_negative;

    void _remove_leading_zeros();
    void _shift_right();

public:
    // класс-исключение, бросаемое при делении на ноль
    class divide_by_zero : public std::exception {  };

    LONG_INT();
    LONG_INT(std::string);
    LONG_INT(signed char);
    LONG_INT(unsigned char);
    LONG_INT(signed short);
    LONG_INT(unsigned short);
    LONG_INT(signed int);
    LONG_INT(unsigned int);
    LONG_INT(signed long);
    LONG_INT(unsigned long);
    LONG_INT(signed long long);
    LONG_INT(unsigned long long);

    friend std::ostream& operator <<(std::ostream&, const LONG_INT&);
    operator std::string() const;
    const LONG_INT operator +() const;
    const LONG_INT operator -() const;
    const LONG_INT operator ++();
    const LONG_INT operator ++(int);
    const LONG_INT operator --();
    const LONG_INT operator --(int);
    friend bool operator ==(const LONG_INT&, const LONG_INT&);
    friend bool operator <(const LONG_INT&, const LONG_INT&);
    friend bool operator !=(const LONG_INT&, const LONG_INT&);
    friend bool operator <=(const LONG_INT&, const LONG_INT&);
    friend bool operator >(const LONG_INT&, const LONG_INT&);
    friend bool operator >=(const LONG_INT&, const LONG_INT&);
    friend const LONG_INT operator +(LONG_INT, const LONG_INT&);
    LONG_INT& operator +=(const LONG_INT&);
    friend const LONG_INT operator -(LONG_INT, const LONG_INT&);
    LONG_INT& operator -=(const LONG_INT&);
    friend const LONG_INT operator *(const LONG_INT&, const LONG_INT&);
    LONG_INT& operator *=(const LONG_INT&);
    friend const LONG_INT operator /(const LONG_INT&, const LONG_INT&);
    LONG_INT& operator /=(const LONG_INT&);
    friend const LONG_INT operator %(const LONG_INT&, const LONG_INT&);
    LONG_INT& operator %=(const LONG_INT&);

    bool odd() const;
    bool even() const;
    const LONG_INT pow(LONG_INT) const;
};

// создает длинное целое число со значением 0
LONG_INT::LONG_INT() {
    this->_is_negative = false;
}

// создает длинное целое число из C++-строки
LONG_INT::LONG_INT(std::string str) {
    if (str.length() == 0) {
        this->_is_negative = false;
    }
    else {
        if (str[0] == '-') {
            str = str.substr(1);
            this->_is_negative = true;
        }
        else {
            this->_is_negative = false;
        }

        for (long long i = str.length(); i > 0; i -= 9) {
            if (i < 9)
                this->_digits.push_back(atoi(str.substr(0, i).c_str()));
            else
                this->_digits.push_back(atoi(str.substr(i - 9, 9).c_str()));
        }

        this->_remove_leading_zeros();
    }
}

// удаляет ведущие нули
void LONG_INT::_remove_leading_zeros() {
    while (this->_digits.size() > 1 && this->_digits.back() == 0) {
        this->_digits.pop_back();
    }

    if (this->_digits.size() == 1 && this->_digits[0] == 0) this->_is_negative = false;
}

// печатает число в поток вывода
std::ostream& operator <<(std::ostream& os, const LONG_INT& bi) {
    if (bi._digits.empty()) os << 0;
    else {
        if (bi._is_negative) os << '-';
        os << bi._digits.back();
        char old_fill = os.fill('0');
        for (long long i = static_cast<long long>(bi._digits.size()) - 2; i >= 0; --i) os << std::setw(9) << bi._digits[i];
        os.fill(old_fill);
    }

    return os;
}

// сравнивает два числа на равенство
bool operator ==(const LONG_INT& left, const LONG_INT& right) {
    if (left._is_negative != right._is_negative) return false;
    if (left._digits.empty()) {
        if (right._digits.empty() || (right._digits.size() == 1 && right._digits[0] == 0)) return true;
        else return false;
    }

    if (right._digits.empty()) {
        if (left._digits.size() == 1 && left._digits[0] == 0) return true;
        else return false;
    }

    if (left._digits.size() != right._digits.size()) return false;
    for (size_t i = 0; i < left._digits.size(); ++i) if (left._digits[i] != right._digits[i]) return false;

    return true;
}

// возвращает копию переданного числа
const LONG_INT LONG_INT::operator +() const {
    return LONG_INT(*this);
}

// возвращает переданное число с другим знаком
const LONG_INT LONG_INT::operator -() const {
    LONG_INT copy(*this);
    copy._is_negative = !copy._is_negative;
    return copy;
}

// проверяет, является ли левый операнд меньше правого
bool operator <(const LONG_INT& left, const LONG_INT& right) {
    if (left == right) return false;
    if (left._is_negative) {
        if (right._is_negative) return ((-right) < (-left));
        else return true;
    }
    else if (right._is_negative) return false;
    else {
        if (left._digits.size() != right._digits.size()) {
            return left._digits.size() < right._digits.size();
        }
        else {
            for (long long i = left._digits.size() - 1; i >= 0; --i) {
                if (left._digits[i] != right._digits[i]) return left._digits[i] < right._digits[i];
            }

            return false;
        }
    }
}

// сравнивает два числа на неравенство
bool operator !=(const LONG_INT& left, const LONG_INT& right) {
    return !(left == right);
}

// проверяет, является ли левый операнд меньше либо равен правого
bool operator <=(const LONG_INT& left, const LONG_INT& right) {
    return (left < right || left == right);
}

// проверяет, является ли левый операнд больше правого
bool operator >(const LONG_INT& left, const LONG_INT& right) {
    return !(left <= right);
}

// проверяет, является ли левый операнд больше либо равен правого
bool operator >=(const LONG_INT& left, const LONG_INT& right) {
    return !(left < right);
}

// складывает два числа
const LONG_INT operator +(LONG_INT left, const LONG_INT& right) {
    if (left._is_negative) {
        if (right._is_negative) return -(-left + (-right));
        else return right - (-left);
    }
    else if (right._is_negative) return left - (-right);
    int carry = 0;
    for (size_t i = 0; i < std::max(left._digits.size(), right._digits.size()) || carry != 0; ++i) {
        if (i == left._digits.size()) left._digits.push_back(0);
        left._digits[i] += carry + (i < right._digits.size() ? right._digits[i] : 0);
        carry = left._digits[i] >= LONG_INT::BASE;
        if (carry != 0) left._digits[i] -= LONG_INT::BASE;
    }

    return left;
}

// прибавляет к текущему числу новое
LONG_INT& LONG_INT::operator +=(const LONG_INT& value) {
    return *this = (*this + value);
}

// префиксный инкремент
const LONG_INT LONG_INT::operator++() {
    return (*this += 1);
}

// преобразует число к строке
LONG_INT::operator std::string() const {
    std::stringstream ss;
    ss << *this;
    return ss.str();
}

// преобразует signed char к LONG_INT
LONG_INT::LONG_INT(signed char c) {
    if (c < 0) this->_is_negative = true;
    else this->_is_negative = false;
    this->_digits.push_back(std::abs(c));
}

// преобразует unsigned char к LONG_INT
LONG_INT::LONG_INT(unsigned char c) {
    this->_is_negative = false;
    this->_digits.push_back(c);
}

// преобразует signed short к LONG_INT
LONG_INT::LONG_INT(signed short s) {
    if (s < 0) this->_is_negative = true;
    else this->_is_negative = false;
    this->_digits.push_back(std::abs(s));
}

// преобразует unsigned short к LONG_INT
LONG_INT::LONG_INT(unsigned short s) {
    this->_is_negative = false;
    this->_digits.push_back(s);
}

// преобразует signed int к LONG_INT
LONG_INT::LONG_INT(signed int i) {
    if (i < 0) this->_is_negative = true;
    else this->_is_negative = false;
    this->_digits.push_back(std::abs(i) % LONG_INT::BASE);
    i /= LONG_INT::BASE;
    if (i != 0) this->_digits.push_back(std::abs(i));
}

// преобразует unsigned int к LONG_INT
LONG_INT::LONG_INT(unsigned int i) {
    this->_digits.push_back(i % LONG_INT::BASE);
    i /= LONG_INT::BASE;
    if (i != 0) this->_digits.push_back(i);
}

// преобразует signed long к LONG_INT
LONG_INT::LONG_INT(signed long l) {
    if (l < 0) this->_is_negative = true;
    else this->_is_negative = false;
    this->_digits.push_back(std::abs(l) % LONG_INT::BASE);
    l /= LONG_INT::BASE;
    if (l != 0) this->_digits.push_back(std::abs(l));
}

// преобразует unsigned long к LONG_INT
LONG_INT::LONG_INT(unsigned long l) {
    this->_digits.push_back(l % LONG_INT::BASE);
    l /= LONG_INT::BASE;
    if (l != 0) this->_digits.push_back(l);
}

// преобразует signed long long к LONG_INT
LONG_INT::LONG_INT(signed long long l) {
    if (l < 0) { this->_is_negative = true; l = -l; }
    else this->_is_negative = false;
    do {
        this->_digits.push_back(l % LONG_INT::BASE);
        l /= LONG_INT::BASE;
    } while (l != 0);
}

// преобразует unsigned long long к LONG_INT
LONG_INT::LONG_INT(unsigned long long l) {
    this->_is_negative = false;
    do {
        this->_digits.push_back(l % LONG_INT::BASE);
        l /= LONG_INT::BASE;
    } while (l != 0);
}

// постфиксный инкремент
const LONG_INT LONG_INT::operator ++(int) {
    *this += 1;
    return *this - 1;
}

// префиксный декремент
const LONG_INT LONG_INT::operator --() {
    return *this -= 1;
}

// постфиксный декремент
const LONG_INT LONG_INT::operator --(int) {
    *this -= 1;
    return *this + 1;
}

// вычитает два числа
const LONG_INT operator -(LONG_INT left, const LONG_INT& right) {
    if (right._is_negative) return left + (-right);
    else if (left._is_negative) return -(-left + right);
    else if (left < right) return -(right - left);
    int carry = 0;
    for (size_t i = 0; i < right._digits.size() || carry != 0; ++i) {
        left._digits[i] -= carry + (i < right._digits.size() ? right._digits[i] : 0);
        carry = left._digits[i] < 0;
        if (carry != 0) left._digits[i] += LONG_INT::BASE;
    }

    left._remove_leading_zeros();
    return left;
}

// вычитает из текущего числа новое
LONG_INT& LONG_INT::operator -=(const LONG_INT& value) {
    return *this = (*this - value);
}

// перемножает два числа
const LONG_INT operator *(const LONG_INT& left, const LONG_INT& right) {
    LONG_INT result;
    result._digits.resize(left._digits.size() + right._digits.size());
    for (size_t i = 0; i < left._digits.size(); ++i) {
        int carry = 0;
        for (size_t j = 0; j < right._digits.size() || carry != 0; ++j) {
            long long cur = result._digits[i + j] +
                left._digits[i] * 1LL * (j < right._digits.size() ? right._digits[j] : 0) + carry;
            result._digits[i + j] = static_cast<int>(cur % LONG_INT::BASE);
            carry = static_cast<int>(cur / LONG_INT::BASE);
        }
    }

    result._is_negative = left._is_negative != right._is_negative;
    result._remove_leading_zeros();
    return result;
}

// домножает текущее число на указанное
LONG_INT& LONG_INT::operator *=(const LONG_INT& value) {
    return *this = (*this * value);
}

// сдвигает все разряды на 1 вправо (домножает на BASE)
void LONG_INT::_shift_right() {
    if (this->_digits.size() == 0) {
        this->_digits.push_back(0);
        return;
    }
    this->_digits.push_back(this->_digits[this->_digits.size() - 1]);
    for (size_t i = this->_digits.size() - 2; i > 0; --i) this->_digits[i] = this->_digits[i - 1];
    this->_digits[0] = 0;
}

// делит два числа
const LONG_INT operator /(const LONG_INT& left, const LONG_INT& right) {
    if (right == 0) throw LONG_INT::divide_by_zero();
    LONG_INT b = right;
    b._is_negative = false;
    LONG_INT result, current;
    result._digits.resize(left._digits.size());
    for (long long i = static_cast<long long>(left._digits.size()) - 1; i >= 0; --i) {
        current._shift_right();
        current._digits[0] = left._digits[i];
        current._remove_leading_zeros();
        int x = 0, l = 0, r = LONG_INT::BASE;
        while (l <= r) {
            int m = (l + r) / 2;
            LONG_INT t = b * m;
            if (t <= current) {
                x = m;
                l = m + 1;
            }
            else r = m - 1;
        }

        result._digits[i] = x;
        current = current - b * x;
    }

    result._is_negative = left._is_negative != right._is_negative;
    result._remove_leading_zeros();
    return result;
}

// делит текущее число на указанное
LONG_INT& LONG_INT::operator /=(const LONG_INT& value) {
    return *this = (*this / value);
}

// возвращает остаток от деления двух чисел
const LONG_INT operator %(const LONG_INT& left, const LONG_INT& right) {
    LONG_INT result = left - (left / right) * right;
    if (result._is_negative) result += right;
    return result;
}

// присваивает текущему числу остаток от деления на другое число
LONG_INT& LONG_INT::operator %=(const LONG_INT& value) {
    return *this = (*this % value);
}

// проверяет, является ли текущее число нечетным
bool LONG_INT::odd() const {
    if (this->_digits.size() == 0) return false;
    return this->_digits[0] & 1;
}

// проверяет, является ли текущее число четным
bool LONG_INT::even() const {
    return !this->odd();
}

// возводит текущее число в указанную степень
const LONG_INT LONG_INT::pow(LONG_INT n) const {
    LONG_INT a(*this), result(1);
    while (n != 0) {
        if (n.odd()) result *= a;
        a *= a;
        n /= 2;
    }

    return result;
}