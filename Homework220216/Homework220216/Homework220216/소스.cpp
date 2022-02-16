#include <string>
#include <cctype>
#include <iostream>

static void Replace(std::string& _Text, const std::string& _Prev, const std::string& _Next, int _Count)
{
	if (_Count <= 0) return;

	int cnt = 0;
	size_t found = _Text.find(_Prev);

	while (found != std::string::npos)
	{
		_Text.replace(found, _Prev.size(), _Next);
		cnt++;

		if (cnt == _Count) break;
		found = _Text.find(_Prev);
	}
}

static void Insert(std::string& _Text, int _Index, const std::string& _InsertText)
{
	if (_Index < 0 || _Index >= _Text.size()) return;

	_Text.insert(_Index, _InsertText);
}

static void Remove(std::string& _Text, const std::string& _DeleteText)
{
	size_t found = _Text.find(_DeleteText);

	while (found != std::string::npos)
	{
		_Text.erase(found, _DeleteText.size());
		found = _Text.find(_DeleteText);
	}
}

static void TrimRemove(std::string& _Text)
{
	size_t found = _Text.find(" ");

	while (found != std::string::npos)
	{
		_Text.erase(found, 1);
		found = _Text.find(" ");
	}
}

static void ToUpper(std::string& _Text)
{
	for (int i = 0; i < _Text.size(); i++)
	{
		if(_Text[i] >= 'a' && _Text[i] <= 'z')
			_Text[i] = toupper(_Text[i]);
	}
}

int main()
{
	std::string temp = "°¡³ª´Ùabc";
	ToUpper(temp);
	std::cout << temp;
}