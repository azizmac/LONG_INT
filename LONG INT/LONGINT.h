#include <vector>
#include <string>
#include <ostream>
#include <iomanip>
#include <sstream>

class LONG_INT {
    // ��������� ������� ��������� (1 000 000 000)
    static const int BASE = 1000000000;

    // ���������� ��������� �����
    std::vector<int> _digits;

    // ���� �����
    bool _is_negative;

    void _remove_leading_zeros();
    void _shift_right();

public:
    // �����-����������, ��������� ��� ������� �� ����
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

// ������� ������� ����� ����� �� ��������� 0
LONG_INT::LONG_INT() {
    this->_is_negative = false;
}

// ������� ������� ����� ����� �� C++-������
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

// ������� ������� ����
void LONG_INT::_remove_leading_zeros() {
    while (this->_digits.size() > 1 && this->_digits.back() == 0) {
        this->_digits.pop_back();
    }

    if (this->_digits.size() == 1 && this->_digits[0] == 0) this->_is_negative = false;
}

// �������� ����� � ����� ������
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

// ���������� ��� ����� �� ���������
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

// ���������� ����� ����������� �����
const LONG_INT LONG_INT::operator +() const {
    return LONG_INT(*this);
}

// ���������� ���������� ����� � ������ ������
const LONG_INT LONG_INT::operator -() const {
    LONG_INT copy(*this);
    copy._is_negative = !copy._is_negative;
    return copy;
}

// ���������, �������� �� ����� ������� ������ �������
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

// ���������� ��� ����� �� �����������
bool operator !=(const LONG_INT& left, const LONG_INT& right) {
    return !(left == right);
}

// ���������, �������� �� ����� ������� ������ ���� ����� �������
bool operator <=(const LONG_INT& left, const LONG_INT& right) {
    return (left < right || left == right);
}

// ���������, �������� �� ����� ������� ������ �������
bool operator >(const LONG_INT& left, const LONG_INT& right) {
    return !(left <= right);
}

// ���������, �������� �� ����� ������� ������ ���� ����� �������
bool operator >=(const LONG_INT& left, const LONG_INT& right) {
    return !(left < right);
}

// ���������� ��� �����
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

// ���������� � �������� ����� �����
LONG_INT& LONG_INT::operator +=(const LONG_INT& value) {
    return *this = (*this + value);
}

// ���������� ���������
const LONG_INT LONG_INT::operator++() {
    return (*this += 1);
}

// ����������� ����� � ������
LONG_INT::operator std::string() const {
    std::stringstream ss;
    ss << *this;
    return ss.str();
}

// ����������� signed char � LONG_INT
LONG_INT::LONG_INT(signed char c) {
    if (c < 0) this->_is_negative = true;
    else this->_is_negative = false;
    this->_digits.push_back(std::abs(c));
}

// ����������� unsigned char � LONG_INT
LONG_INT::LONG_INT(unsigned char c) {
    this->_is_negative = false;
    this->_digits.push_back(c);
}

// ����������� signed short � LONG_INT
LONG_INT::LONG_INT(signed short s) {
    if (s < 0) this->_is_negative = true;
    else this->_is_negative = false;
    this->_digits.push_back(std::abs(s));
}

// ����������� unsigned short � LONG_INT
LONG_INT::LONG_INT(unsigned short s) {
    this->_is_negative = false;
    this->_digits.push_back(s);
}

// ����������� signed int � LONG_INT
LONG_INT::LONG_INT(signed int i) {
    if (i < 0) this->_is_negative = true;
    else this->_is_negative = false;
    this->_digits.push_back(std::abs(i) % LONG_INT::BASE);
    i /= LONG_INT::BASE;
    if (i != 0) this->_digits.push_back(std::abs(i));
}

// ����������� unsigned int � LONG_INT
LONG_INT::LONG_INT(unsigned int i) {
    this->_digits.push_back(i % LONG_INT::BASE);
    i /= LONG_INT::BASE;
    if (i != 0) this->_digits.push_back(i);
}

// ����������� signed long � LONG_INT
LONG_INT::LONG_INT(signed long l) {
    if (l < 0) this->_is_negative = true;
    else this->_is_negative = false;
    this->_digits.push_back(std::abs(l) % LONG_INT::BASE);
    l /= LONG_INT::BASE;
    if (l != 0) this->_digits.push_back(std::abs(l));
}

// ����������� unsigned long � LONG_INT
LONG_INT::LONG_INT(unsigned long l) {
    this->_digits.push_back(l % LONG_INT::BASE);
    l /= LONG_INT::BASE;
    if (l != 0) this->_digits.push_back(l);
}

// ����������� signed long long � LONG_INT
LONG_INT::LONG_INT(signed long long l) {
    if (l < 0) { this->_is_negative = true; l = -l; }
    else this->_is_negative = false;
    do {
        this->_digits.push_back(l % LONG_INT::BASE);
        l /= LONG_INT::BASE;
    } while (l != 0);
}

// ����������� unsigned long long � LONG_INT
LONG_INT::LONG_INT(unsigned long long l) {
    this->_is_negative = false;
    do {
        this->_digits.push_back(l % LONG_INT::BASE);
        l /= LONG_INT::BASE;
    } while (l != 0);
}

// ����������� ���������
const LONG_INT LONG_INT::operator ++(int) {
    *this += 1;
    return *this - 1;
}

// ���������� ���������
const LONG_INT LONG_INT::operator --() {
    return *this -= 1;
}

// ����������� ���������
const LONG_INT LONG_INT::operator --(int) {
    *this -= 1;
    return *this + 1;
}

// �������� ��� �����
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

// �������� �� �������� ����� �����
LONG_INT& LONG_INT::operator -=(const LONG_INT& value) {
    return *this = (*this - value);
}

// ����������� ��� �����
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

// ��������� ������� ����� �� ���������
LONG_INT& LONG_INT::operator *=(const LONG_INT& value) {
    return *this = (*this * value);
}

// �������� ��� ������� �� 1 ������ (��������� �� BASE)
void LONG_INT::_shift_right() {
    if (this->_digits.size() == 0) {
        this->_digits.push_back(0);
        return;
    }
    this->_digits.push_back(this->_digits[this->_digits.size() - 1]);
    for (size_t i = this->_digits.size() - 2; i > 0; --i) this->_digits[i] = this->_digits[i - 1];
    this->_digits[0] = 0;
}

// ����� ��� �����
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

// ����� ������� ����� �� ���������
LONG_INT& LONG_INT::operator /=(const LONG_INT& value) {
    return *this = (*this / value);
}

// ���������� ������� �� ������� ���� �����
const LONG_INT operator %(const LONG_INT& left, const LONG_INT& right) {
    LONG_INT result = left - (left / right) * right;
    if (result._is_negative) result += right;
    return result;
}

// ����������� �������� ����� ������� �� ������� �� ������ �����
LONG_INT& LONG_INT::operator %=(const LONG_INT& value) {
    return *this = (*this % value);
}

// ���������, �������� �� ������� ����� ��������
bool LONG_INT::odd() const {
    if (this->_digits.size() == 0) return false;
    return this->_digits[0] & 1;
}

// ���������, �������� �� ������� ����� ������
bool LONG_INT::even() const {
    return !this->odd();
}

// �������� ������� ����� � ��������� �������
const LONG_INT LONG_INT::pow(LONG_INT n) const {
    LONG_INT a(*this), result(1);
    while (n != 0) {
        if (n.odd()) result *= a;
        a *= a;
        n /= 2;
    }

    return result;
}