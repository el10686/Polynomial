#include <iostream>
#include <cmath>

using namespace std;

class Polynomial {
	protected:
		class Term {
			protected:
				int exponent, coefficient;
				Term *next;

				Term(int exp, int coeff, Term *n) : exponent(exp), coefficient(coeff), next(n) {}

				friend class Polynomial;
				friend Polynomial operator+(const Polynomial &p, const Polynomial &q);
				friend Polynomial operator*(const Polynomial &p, const Polynomial &q);
				friend ostream &operator<<(ostream &out, const Polynomial &p);
		};

	public:
		Polynomial() : head(nullptr) {}

		Polynomial(const Polynomial &p) : head(nullptr) {
			copy(p);
		}

		~Polynomial() {
			purge();
		}

		Polynomial &operator=(const Polynomial &p) {
			clear();
			copy(p);
			return *this;
		}

		void addTerm(int exp, int coeff) {
			if (head == nullptr) {
				Term *p = new Term(exp, coeff, nullptr);
				head = p;
				return;
			}

			Term *prev = head;
			Term *curr = head;

			while (curr != nullptr && curr->exponent > exp) {
				prev = curr;
				curr = curr->next;
			}

			if (curr != nullptr && curr->exponent == exp) {
				curr->coefficient += coeff;
				if (curr->coefficient == 0) {
					if (curr == head) {
						head = curr->next;
					}
					prev->next = curr->next;
					delete curr;
				}
				return;
			}
			if (curr == head) {
				Term *p = new Term(exp, coeff, head);
				head = p;
			} else {
				Term *p = new Term(exp, coeff, curr);
				prev->next = p;
			}
		}

		double evaluate(double x) {
			double sum = 0;
			for (Term *p = head; p != nullptr; p = p->next) {
				sum += p->coefficient * pow(x, p->exponent);
			}
			return sum;
		}

		friend Polynomial operator+(const Polynomial &p, const Polynomial &q) {
			Polynomial ret;
			for (Term *tmp = p.head; tmp != nullptr; tmp = tmp->next) {
				ret.addTerm(tmp->exponent, tmp->coefficient);
			}
			for (Term *tmp = q.head; tmp != nullptr; tmp = tmp->next) {
				ret.addTerm(tmp->exponent, tmp->coefficient);
			}
			return ret;
		}

		friend Polynomial operator*(const Polynomial &p, const Polynomial &q) {
			Polynomial ret;
			for (Term *i = p.head; i != nullptr; i = i->next) {
				for (Term *j = q.head; j != nullptr; j = j->next) {
					ret.addTerm(i->exponent + j->exponent, i->coefficient * j->coefficient);
				}
			}
			return ret;
		}

		friend ostream &operator<<(ostream &out, const Polynomial &p) {
			for (Term *q = p.head; q != nullptr; q = q->next) {
				if (q == p.head && q->coefficient < 0) {
					out << "- ";
				}
				if (abs(q->coefficient) != 1 || (abs(q->coefficient) == 1 && q->exponent == 0)) {
					out << abs(q->coefficient);
				}
				if (q->exponent != 0 && q->exponent != 1) {
					out << "x^" << q->exponent;
				} else if (q->exponent == 1) {
					out << "x";
				}
				if (q->next && q->next->coefficient > 0) {
					out << " + ";
				} else if (q->next && q->next->coefficient < 0) {
					out << " - ";
				}
			}
			if (p.head == nullptr) {
				out << 0;
			}
			return out;
		}

	private:
		Term *head;

		void purge() {
			Term *p = head;
			while (p != nullptr) {
				Term *tmp = p;
				p = p->next;
				delete tmp;
			}
		}

		void clear() {
			purge();
			head = nullptr;
		}

		void copy(const Polynomial &q) {
			for (Term *p = q.head; p != nullptr; p = p->next) {
				addTerm(p->exponent, p->coefficient);
			}
		}
};

//Driver programm
int main() {
	Polynomial p;
	p.addTerm(1, 3);
	p.addTerm(2, 1);
	p.addTerm(0, -1);

	Polynomial q(p);
	q.addTerm(1, -3);

	cout << "P(x) = " << p << endl;
	cout << "P(1) = " << p.evaluate(1) << endl;
	cout << "Q(x) = " << q << endl;
	cout << "Q(1) = " << q.evaluate(1) << endl;
	cout << "(P+Q)(x) = " << p + q << endl;
	cout << "(P*Q)(x) = " << p * q << endl;
}
