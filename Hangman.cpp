// Hangman.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <string>
#include <time.h>
#include <conio.h>
#include <fstream>
#include <map>


class Letter {
public:
	Letter(char input)
	{
		load_letter(input);
		check_letter();
	}

	unsigned char letter;
	bool is_spolgloska;

	void load_letter(char ipt);
	void check_letter();
};
// Funckja sprawdzajaca litere, czy jest spolgloska
void Letter::check_letter() {
	if (letter == 'A' || letter == 'E' || letter == 'I' || letter == 'O' || letter == 'U' || letter == 'Y') {
		this->is_spolgloska = false;
	}
	else
	{
		this->is_spolgloska = true;
	}
}
// Funkcja ladujaca litere
void Letter::load_letter(char liter) {
	if (liter > 91) {
		this->letter = liter - 32;
	}
	else {
		this->letter = liter;
	}
}
 // Procedura do automatycznego tworzenia szablonu rozwiazania
std::string create_solution(int size) {
	std::string solution;
	solution.reserve(size);

	for (int i = 0; i < size; ++i) {
		solution.push_back('_');
	}
	return solution;
}

// Procedura sprawdzajaca czy dana litera wystepuje w slowie
bool check(std::string inpt, Letter c) {
	for (auto p : inpt) {
		if (c.letter == p || c.letter + 32 == p) return true;
	}
	return false;
}

// Procedura sprawdzajaca czy slowa sie zgadzaja
bool check(std::string inpt, std::string out) {
	int val = 0;
	for (auto i = inpt.begin(), j = out.begin(); i != inpt.end(), j != out.end(); ++i, ++j) {
		if (*i == *j || std::abs(*j - *i) == 32) val++;
	}
	if (val == inpt.size())return true;
	else return false;
}

// Wyswietlanie slowa
void Print(std::string out) {
	for (auto p : out) {
		std::cout << p;
	}
	std::cout << std::endl;
}

// Wyswietlanie slowa po odgadnieciu litery
void Print(std::string in, std::string& out, unsigned char c) {
	for (auto in_iter = in.begin(), out_iter = out.begin(); in_iter != in.end(), out_iter != out.end(); in_iter++, out_iter++) {
		if (c == *in_iter || c + 32 == *in_iter) {
			*out_iter = c;
		}
		std::cout << *out_iter;
	}
	std::cout << std::endl;
}

// Funkcja sprawdzajaca czy dana litera wystapila w slowie i umieszczajaca ja w rozwiazaniu
void CheckAndInsert(std::string in, std::string& out, unsigned char c) {
	for (auto in_iter = in.begin(), out_iter = out.begin(); in_iter != in.end(), out_iter != out.end(); in_iter++, out_iter++) {
		if (c == *in_iter || c + 32 == *in_iter) {
			*out_iter = c;
		}
	}
}

void Interfejs() {
	std::cout << "Hangman" << std::endl;
	std::cout << "Wybierz tryb gry: " << std::endl;
	std::cout << "1. Gra manualna -podstawowa " << std::endl;
	std::cout << "2. Losowy wybor liter z alfabetu " << std::endl;
	std::cout << "3. Wybor zgodnie z prawdopodbienstwem wystapienia litery w j. polskim " << std::endl;
	std::cout << "4. Zakoncz gre " << std::endl << std::endl;
}

void RandomSolution(std::string input) {
	std::string word = create_solution(input.size());
	srand(time(NULL));
	int counter = 0;
	bool solved = false;
	while (!solved) {
		char temp = rand() % (90 - 64) + 65; // Losowanie litery z przedzia³u liter wielkich ASCII
		CheckAndInsert(input, word, temp);
		++counter;
		if (check(word, input))solved = true;
	}
	std::cout << "Podane slowo to: " << word << std::endl;
	std::cout << "Liczba prob: " << counter << std::endl;
	std::cout << "Wcisnij klawisz aby przejsc dalej" << std::endl;
	_getch();
	system("cls");
	Interfejs();
}

void ManualSolution(std::string input) {
	int liczba_prob;
	system("cls");
	std::cout << "Zasady: " << std::endl;
	std::cout << "Podanie odpowiedniej spolgoski nie zmniejsza liczby prob " << std::endl;
	std::cout << "Podanie smogloski niezaleznie zmniejsza liczbe prob " << std::endl;
	std::cout << "Podanie nieodpowiedniego slowa : przegrana " << std::endl;
	std::cout << "Podanie odpowiedniego slowa : wygrana " << std::endl;
	std::cout << "Podaj Liczbe prob: ";
	std::cin >> liczba_prob;
	while (!(std::cin >> liczba_prob)) {
		std::cout<<std::endl;
		std::cout << "Podano nieodpowiednia liczbe, Podaj liczbe naturalna"<<std::endl;
		std::cin.clear();
		std::cin.ignore();
	}
	system("cls");
	std::string solution = create_solution(input.size());
	std::string temp_string;
	std::cout << "Podaj spolgloske lub rozwiazanie" << std::endl;
	bool solved = false;
	for (int i = liczba_prob; i > -1; --i) {
		if (solved)break;
		else {
			std::cin >> temp_string;
			if (temp_string.size() == 1) {
				Letter temp_l(temp_string[0]);
				if (temp_l.is_spolgloska) {
					if (check(input, temp_l)) {
						Print(input, solution, temp_l.letter);
						++i;
						if (check(solution, input))solved = true;
					}
					else std::cout << "Pozostalo prob: " << i-1 << std::endl;
				}
				else
				{
					if (check(input, temp_l)) {
						Print(input, solution, temp_l.letter);
						std::cout << "Pozostalo prob: " << i - 1 << std::endl;
						if (check(solution, input))solved = true;
					}
					else std::cout << "Pozostalo prob: " << i-1 << std::endl;
				}

			}
			else {
				if (temp_string.size() == input.size()) {
					if (check(temp_string, input))solved = true;
					else break;
				}
				else {
					std::cout << "Nieprawidlowy rozmiar= nieprawidlowe slowo, przegrana" << std::endl;
				}
			}
		}
		std::cout << std::endl;
	}
	if (solved)std::cout << "Wygrana" << std::endl;
	else std::cout << "Przegrana" << std::endl;
	std::cout << "Wcisnij klawisz aby przejsc dalej" << std::endl;
	_getch();
	system("cls");
	Interfejs();

}

// Wytrzowenie bazy danych ze specjalnie przygotowanego pliku
void BazaDanych(std::map<float, char>& m) {
	std::fstream file("freq.txt", std::ios::in);
	if (file.good()) {
		std::string temp;
		while (!file.eof()) {
			std::getline(file, temp);
			int offset = 48;
			float temp_f = (temp[2] - offset) + 0.1*(temp[4] - offset) + 0.01*(temp[5] - offset);
			m.insert(std::make_pair(temp_f, temp[0])); //Do³o¿enie do mapy pary : prawdopodobienstwo wystapienia + znak
		}
	}
	file.close();
}

void ProbabilitySolution(std::string input) {
	std::string word = create_solution(input.size());
	std::cout << "Na podstawie bazy danych PWN" << std::endl;
	std::map<float, char> probab; // Tworzê mapê której kluczem jest prawdopodbienstwo wyst¹pienia a wartoœci¹ litera
	BazaDanych(probab);
	int counter = 0;
	bool solved = false;
	auto iter = probab.crbegin(); // Iteracja od koñca, reverse const begin
	while ((!solved) && (iter != probab.crend())) { // Dopóki nie rozwi¹¿emy s³owa i nie dojdziemy do koñca
		char temp = iter->second;
		Letter l(temp);
		CheckAndInsert(input, word, l.letter);
		++counter;
		++iter;
		if (check(word, input))solved = true; 
	}
	std::cout << std::endl;
	std::cout << "Podane slowo to: " << word << std::endl;
	std::cout << "Liczba prob: " << counter << std::endl << std::endl;
	std::cout << "Wcisnij klawisz aby przejsc dalej" << std::endl;
	_getch();
	system("cls");
	Interfejs();
}


int main()
{
	std::string input_word;
	std::cout << "Podaj slowo do odgadniecia: ";
	std::cin >> input_word;
	system("cls");
	Interfejs();
	int s;
	std::cin >> s;
	while (s != 4) {
		switch (s)
		{
		case 1:
			ManualSolution(input_word);
			break;
		case 2:
			RandomSolution(input_word);
			break;

		case 3:
			ProbabilitySolution(input_word);
			break;
		case 4:
			return 0;
			break;
		default:
			std::cout << "Podaj odpowiedni numer" << std::endl;
		}
		std::cin >> s;
	}
	return 0;
}

