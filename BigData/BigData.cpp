#include"BigData.h"

BigData::BigData(Int64 Idata)
	:_Idata(Idata)
	, _Sdata("")
{
	Int64ToString();
}
BigData::BigData(const char * Sdata)
{
	assert(Sdata != NULL);
	char cSymbol = Sdata[0];
	char* StringData = (char*)Sdata;
	//atoi 
	if (cSymbol == '+' || cSymbol == '-')
	{
		StringData++;
	}
	else if (*StringData >= '0' && *StringData <= '9')
	{
		cSymbol = '+';
	}
	else
	{
		_Idata = 0;
		_Sdata = "0";
		return;
	}
	//去掉前置0
	while (*StringData == '0')
	{
		StringData++;
	}
	_Sdata.resize(strlen(StringData) + 1);//开辟空间
	_Idata = 0;
	_Sdata[0] = cSymbol;
	int Count = 1;
	while (StringData)
	{
		if (*StringData >= '0' && *StringData <= '9')
		{
			_Idata = _Idata * 10 + *StringData - '0';
			_Sdata[Count++] = *StringData++;
		}
		else
		{
			break;
		}
	}
	_Sdata.resize(Count);
	if (cSymbol == '-')
	{
		_Idata = 0 - _Idata;
	}
}
//重载加法
BigData BigData:: operator+(const BigData& bigData)
{	//左没溢出，右没溢出
	if (!IsINT64Overflow() && !bigData.IsINT64Overflow())
	{
		if (_Sdata[0] != bigData._Sdata[0])//异号相加，不会溢出
		{
			return BigData(_Idata + bigData._Idata);
		}
		//2 + 7   -1 
		//10
		else
		{
			if ((_Sdata[0] == '+'&& MAX_INT64 - _Idata >= bigData._Idata) ||
				(_Sdata[0] == '-'&& MIN_INT64 - _Idata <= bigData._Idata))//未溢出的条件
			{
				return BigData(_Idata + bigData._Idata);
			}
		}
	}
		//

		string  strRet;
		if (_Sdata[0] == bigData._Sdata[0])
		{
			strRet = Add(_Sdata, bigData._Sdata);
		}
		else
		{
			strRet = Sub(_Sdata, bigData._Sdata);
		}
		return BigData(strRet.c_str());
}
//重载减法
BigData BigData:: operator-(const BigData& bigData)
{
	if (!IsINT64Overflow() && !bigData.IsINT64Overflow())
	{
		if (_Sdata[0] == bigData._Sdata[0])//同号相减，不会溢出
		{
			return BigData(_Idata - bigData._Idata);
		}
		else
		{
			if ((_Sdata[0] == '+'&& MAX_INT64 + bigData._Idata >= _Idata) ||
				(_Sdata[0] == '-'&& MIN_INT64 + bigData._Idata <= _Idata))//未溢出的条件
			{
				return BigData(_Idata - bigData._Idata);
			}
		}
	}
	//至少有一个溢出，相减的结果一定溢出
	string strRet;
	if (_Sdata[0] != bigData._Sdata[0])//异号相减
	{
		strRet = Add(_Sdata, bigData._Sdata);
	}
	else
	{
		strRet = Sub(_Sdata, bigData._Sdata);
	}
	return BigData (strRet.c_str());
}
//重载乘法
BigData BigData:: operator *(const BigData& bigData)
{
	if (_Idata == 0 || bigData._Idata == 0)
	{
		return BigData(Int64(0));
	}

	if (!IsINT64Overflow() && !bigData.IsINT64Overflow())
	{
		if (_Sdata[0] == bigData._Sdata[0])//同号
		{
			//
			if ((_Sdata[0] == '+'&& MAX_INT64 / _Idata >= bigData._Idata) ||
				(_Sdata[0] == '-'&& MAX_INT64 / _Idata <= bigData._Idata))
			{
				return BigData(_Idata*bigData._Idata);
			}
		}
		else
		{
			if ((_Sdata[0] == '+'&& MIN_INT64 / _Idata >= bigData._Idata) ||
				(_Sdata[0] == '-'&& MIN_INT64 / _Idata <= bigData._Idata))
			{
				return BigData(_Idata*bigData._Idata);
			}
		}
	}
	return	BigData(Mul(_Sdata, bigData._Sdata).c_str());
}
//重载除法
BigData BigData:: operator/(const BigData& bigData)
{
	if (bigData._Idata == 0)
	{
		assert("除数不能为0");
		return BigData(Int64(0));
	}
	if (!IsINT64Overflow() && !bigData.IsINT64Overflow())
	{
		return BigData(_Idata / bigData._Idata);
	}

	return BigData(Div(_Sdata, bigData._Sdata).c_str());
}
//加法
string BigData::Add(string left, string right)
{
	//默认做操作数>右操作数，确定符号位
	int leftSize = left.size();
	int rightSize = right.size();
	char Csymbol = left[0];
	if ((leftSize < rightSize)||(leftSize==rightSize && left<right))
	{
		swap(left, right);
		swap(leftSize, rightSize);
	}
	
	string strRet;//保存结果
	strRet.resize(leftSize + 1);
	strRet[0] = left[0];
	char cStep = 0;//进位

	for (int i = 1; i < leftSize; i++)
	{
		char cRet = left[leftSize - i] - '0' + cStep;
		if (i < rightSize)
		{
			cRet += (right[rightSize - i] - '0');
		}
		strRet[leftSize - i + 1] = (cRet % 10 + '0');
		cStep = cStep / 10;
	}

	strRet[1] = (cStep + '0');
	return strRet;
}

string BigData::Sub(string left, string right)
{
	//默认做操作数>右操作数，确定符号位
	int leftSize = left.size();
	int rightSize = right.size();
	char Csymbol = left[0];
	if ((leftSize < rightSize) || (leftSize == rightSize && left<right))
	{
		swap(left, right);
		swap(leftSize, rightSize);
		if (Csymbol == '+')
		{
			Csymbol = '-';
		}
		else
		{
			Csymbol = '+';
		}
	}
	string strRet;
	strRet.resize(leftSize);
	strRet[0] = Csymbol;
	
	for (int Index = 1; Index < leftSize; Index++)
	{
		char cRet = left[leftSize - Index] - '0';
		if (Index < rightSize)
		{
			cRet -= (right[rightSize - Index] - '0');
		}
		if (cRet < 0)
		{
			left[leftSize - Index - 1] -= 1;
			cRet += 10;
		}
		strRet[leftSize - Index] = (cRet + '0');
	}
	return strRet;

}
//乘法
string BigData::Mul(string left, string right)
{
	//默认做操作数>右操作数，确定符号位
	int leftSize = left.size();
	int rightSize = right.size();
	char Csymbol = left[0];
	if (leftSize < rightSize)
	{
		swap(left, right);
		swap(leftSize, rightSize);
	}

	char cSymbol = '+';
	if (left[0] != right[0])
	{
		cSymbol = '-';
	}

	string strRet;
	strRet.assign(leftSize + rightSize - 1, '0');//assign(赋值)
	strRet[0] = cSymbol;
	int iDataLen = strRet.size();
	int iOffset = 0;
	for (int Index = 1; Index < leftSize; Index++)
	{
		char cLeft = left[leftSize - Index] - '0';
		char cStep = 0;
		if (cLeft == 0)
		{
			iOffset++;
			continue;
		}
		for (int Index = 1; Index < rightSize; Index++)
		{
			char cRet = cLeft* (right[rightSize - Index] - '0');
			cRet += cStep;
			cRet += (strRet[iDataLen - iOffset - Index] - '0');
			strRet[iDataLen - iOffset - Index] = cRet % 10 +'0';
			cStep = cRet / 10;
		}

		strRet[iDataLen - iOffset - rightSize] += cStep;
		iOffset++;
	}

	return	strRet;
}
string BigData::Div(string left, string right)
{
	char cSymbol = '+';
	if (left[0] != right[0])
	{
		cSymbol = '-';
	}
	int leftSize = left.size();
	int rightSize = right.size();
	if ((leftSize < rightSize) || (leftSize == rightSize 
		  && strcmp(left.c_str(), right.c_str()+1)<0))//调用strcmp函数来对字符串进行比较
	{
		return "0";//1%1111111111111等于0；
	}
	else
	{
		if (right == "+1" || right == "-1")//123%1=123;123%-1=-123;
		{
			left[0] = cSymbol; 
			return left;	
		}
	}

	string strRet;
	strRet.append(1, cSymbol);
	char *Pleft = (char*)(left.c_str() + 1);
	char *Pright = (char*)(right.c_str() + 1);
	int iDdataLen = 1;
	leftSize -= 1;

	for (int Index = 0; Index < leftSize;)
	{
		if (*Pleft == '0') //被除数连续0时，++
		{
			strRet.append(1, '0');
			Pleft++;
			Index++;
			continue;
		}
		if (!IsLeftStringBig(Pleft, iDdataLen, Pright, rightSize - 1))
		{
			strRet.append(1, '0');
			iDdataLen++;
			if (Index + iDdataLen > leftSize)
			{
				break;
			}
			continue;
		}
		else
		{
			//循环相减
			strRet.append(1, SubLoop(Pleft, iDdataLen, Pright, rightSize - 1));

			while (*Pleft == '0'&& iDdataLen > 0)
			{
				Pleft++;
				Index++;
				iDdataLen--;
			}
			iDdataLen++;
			if (Index + iDdataLen > leftSize)
			{
				break;
			}
		}
	}
	return  strRet;
}
bool BigData::operator=(const BigData& bidData)
{
	if (!IsINT64Overflow() && !bidData.IsINT64Overflow())
	{
		return (_Idata == bidData._Idata);
	}
	else
	{
		return (_Sdata == bidData._Sdata);
	}
	
}
bool BigData::IsLeftStringBig(char *Pleft, int leftSize, char* Pright, int rightSize)
{
	assert(Pleft != NULL && Pright != NULL);
	if (leftSize > rightSize ||
		leftSize == rightSize && strncmp(Pleft, Pright, leftSize) >= 0)
	{
		return true;
	}
	return false;
}
char BigData::SubLoop(char* Pleft, int leftsize, char *Pright, int rightSize)
{
	assert(Pleft != NULL && Pright != NULL);
	char cRet = '0';
	while (true)
	{
		if (!IsLeftStringBig(Pleft, leftsize, Pright, rightSize))
		{
			break;
		}
		//-=
		int iLdataLen = leftsize - 1;
		int iRdataLen = rightSize - 1;
		while (iRdataLen >= 0 && iLdataLen >= 0)
		{
			if (Pleft[iLdataLen] < Pright[iRdataLen])
			{
				Pleft[iLdataLen - 1] -= 1;
				Pleft[iLdataLen] += 10;
			}

			Pleft[iLdataLen] = Pleft[iLdataLen] - Pright[iRdataLen] + '0';
			iLdataLen--;
			iRdataLen--;
		}

		while ('0' == *Pleft && leftsize > 0)
		{
			Pleft++;
			leftsize--;
		}
		cRet++;
	}
	return cRet;
}
void BigData::Int64ToString()
{
	char cSymbol = '+';
	Int64 tem = _Idata;
	if (tem < 0)
	{
		cSymbol = '-';
		tem = 0 - tem;
	}
	_Sdata.append(1, cSymbol);//追加
	int Count = 1;
	while (tem)
	{
		_Sdata.append(1, tem % 10 + '0');
		tem /= 10;
	}
	char *pLeft = (char *)(_Sdata.c_str() + 1);
	char *pRight = (char*)(_Sdata.c_str() + _Sdata.size() - 1);//字符串逆序
	while (pLeft < pRight)
	{
		char temp = *pLeft;
		*pLeft = *pRight;
		*pRight = temp;
		pLeft++;
		pRight--;
	}
}

bool BigData::IsINT64Overflow() const//判断是否溢出
{
	string strTem;
	if (_Sdata[0] == '+')
	{
		strTem = "+9223372036854775807";
	}
	else
	{
		strTem = "-9223372036854775808";
	}

	if (_Sdata.size() > strTem.size())
	{
		return true;
	}
	else if (_Sdata.size() == strTem.size() && _Sdata > strTem)
	{
		return true;
	}
		return false;
}
ostream& operator <<(ostream & output, BigData &bigData)
{
	if (!bigData.IsINT64Overflow())//没有溢出按照整数输出
	{
		output << bigData._Idata;
	}
	else
	{
		char* StringData = (char*)bigData._Sdata.c_str();
		if (StringData[0] == '+')
		{
			StringData++;
		}
		output << StringData;
	}
	return output;
}
int main(void)
{
	/*BigData a("+40000000000000000000000000000000");
	BigData a1("20");
	cout << a/a1 << endl;*/

	BigData a("12111111111111111111111111111111111");
	BigData a1(12);
	cout << a*a1 << endl;

	/*BigData a("0");
	BigData a1("1");
	cout << a1/a << endl;*/
}
