#include <stdexcept>
#include <iostream>
#include <iomanip>

using namespace std;

class ChessBoardArray {
	protected:
		class Row {
			public:
				Row(ChessBoardArray &a, int i) : chess(a), row(i) {}

				int &operator[](int i) const {
					return chess.select(row, i);
				}

			private:
				ChessBoardArray &chess;
				int row;
		};

		class ConstRow {
			public:
				ConstRow(const ChessBoardArray &a, int i) : chess(a), row(i) {}

				const int &operator[](int i) const {
					return chess.select(row, i);
				}

			private:
				const ChessBoardArray &chess;
				int row;
		};

	public:
		ChessBoardArray(int s = 0, int b = 0) : data(new int[(s * s + 1) / 2]), size(s), base(b) {}

		ChessBoardArray(const ChessBoardArray &a) : data(new int[(a.size * a.size + 1) / 2]), size(a.size), base(a.base) {
			for (int i = 0; i < (size * size + 1) / 2; ++i) {
				data[i] = a.data[i];
			}
		}

		~ChessBoardArray() {
			delete[] data;
		}

		ChessBoardArray &operator=(const ChessBoardArray &a) {
			delete[] data;
			data = new int[(a.size * a.size + 1) / 2];
			size = a.size;
			base = a.base;

			for (int i = 0; i < (size * size + 1) / 2; ++i) {
				data[i] = a.data[i];
			}

			return *this;
		}

		int &select(int i, int j) {
			return data[loc(i, j)];
		}

		const int &select(int i, int j) const {
			return data[loc(i, j)];
		}

		Row operator[](int i) {
			return Row(*this, i);
		}

		ConstRow operator[](int i) const {
			return ConstRow(*this, i);
		}

		friend ostream &operator<<(ostream &out, const ChessBoardArray &a) {
			int i = 0;
			for (int j = 0; j < a.size; ++j) {
				if (j % 2 == 0) {
					for (int k = 0; k < a.size / 2; ++k) {
						out << setw(4) << a.data[i++] << setw(4) << 0;
					}
					if (a.size % 2 != 0) {
						out << setw(4) << a.data[i++];
					}
				}
				else {
					for (int k = 0; k < a.size / 2; ++k) {
						out << setw(4) << 0 << setw(4) << a.data[i++];
					}
					if (a.size % 2 != 0) {
						out << setw(4) << 0;
					}
				}
				out << endl;
			}
			return out;
		}

	private:
		int *data, size, base;

		int loc(int i, int j) const throw(out_of_range) {
			int di = i - base, dj = j - base;
			if (di < 0 || di > size - 1 || dj < 0 || dj > size - 1) {
				throw out_of_range("index out of bounds");
			}
			else if ((di % 2 == 0 && dj % 2 != 0) || (di % 2 != 0 && dj % 2 == 0)) {
				throw out_of_range("this square is black!");
			}
			else {
				if (size % 2 == 0) {
					return di * (size / 2) + dj / 2;
				}
				else {
					return (di/2) * size + dj/2 + (di%2)*(size/2 + 1);
				}
			}
		}
};


//Driver programm
int main() {
	ChessBoardArray a(4, 1); // size 4x4, rows and columns numbered from 1
	a[3][1] = 42;
	a[4][4] = 17;
	try { a[2][1] = 7; }
	catch(out_of_range &e) { cout << ”a[2][1] is black” << endl; }
	try { cout << a[1][2] << endl; }
	catch(out_of_range &e) { cout << ”and so is a[1][2]” << endl; }
	cout << a;
}
