#pragma once
/*
Typ odpowiedzialny za zwroty liczb w ustawieniu w trakcie działania algorytmu wyliczania permutacji.
*/
enum direction { left, right };

/*
Struktura rozszerzająca int
m_direction - kierunek
m_index - numer porządkowy
m_value - wartość
*/
template <typename TYPE>
struct kint
{
	bool m_direction = left;// 0 lewo 1 prawo
	int m_index = 0;
	TYPE m_value = 0;
};

/*
Struktura rozszerzająca int
m_direction - kierunek
m_index - numer porządkowy
m_value - wartość
*/

template <typename TYPE>
struct kint2
{
	int m_direction = left;// 0 lewo 1 prawo
	int m_index = 0;
	TYPE m_value = 0;
};
