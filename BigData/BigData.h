#pragma once
#include<iostream>
using namespace std;
#include<string.h>
#include<assert.h>


typedef long long Int64;

#define UNinit 0xcccccccccccccccc
#define MAX_INT64 0x7fffffffffffffff
#define MIN_INT64 0x8000000000000000

class BigData
{
public:
	BigData();
	BigData(Int64 Idata=UNinit);
	BigData(const char * Sdata);
	
	BigData operator+(const BigData& bigData);
	BigData operator-(const BigData& bigData);
	BigData operator*(const BigData& bigData);
	BigData operator/(const BigData& bigData);
	BigData operator%(const BigData& bigData);
	
	bool operator<(const BigData& bidData);
	bool operator>(const BigData& bidData);
	bool operator=(const BigData& bidData);
	
	friend ostream& operator <<(ostream & output, BigData &bigData);
protected:
	void Int64ToString();
	bool IsINT64Overflow()const;
	bool IsLeftStringBig(char *Pleft, int leftSize, char* Pright, int rightSize);
	char SubLoop(char* Pleft, int leftsize, char *Pright, int rightSize);

	string Add(string left, string right);
	string Sub(string left, string right);
	string Mul(string left, string right);
	string Div(string left, string right);
private:
	Int64 _Idata;
	string _Sdata;
};