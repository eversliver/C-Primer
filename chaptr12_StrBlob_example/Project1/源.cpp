#include <iostream>
#include <string>
#include <memory>
#include <vector>
#include <initializer_list>
using namespace std;
class StrBlobPtr;
class StrBlob{
public:
	friend class StrBlobPtr;
	typedef vector<string>::size_type size_type;
	StrBlob() :data(make_shared<vector<string>>()){}
	//一开始默认构造函数出现了问题啊,这里的这个很关键啊 
	StrBlob(initializer_list<string> il);
	size_type size()const;
	void Push_back(const string & t);
	void pop_back();
	string & front();
	string & back();
	string front()const;
	string back()const;
	friend bool operator==(const StrBlob & lhs, const StrBlob & rhs);
	friend bool operator!=(const StrBlob & lhs, const StrBlob & rhs);
private:
	shared_ptr<vector<string>> data;
	void check(size_type i, const string & msg)const;

};

bool operator==(const StrBlob & lhs, const StrBlob & rhs)
{
	return *lhs.data == *rhs.data;
}

bool operator!=(const StrBlob & lhs, const StrBlob & rhs)
{
	return !(lhs == rhs);
}

StrBlob::StrBlob(initializer_list<string> il)
	:data(make_shared<vector<string>>(il))
{}

StrBlob::size_type StrBlob::size()const
{
	return data->size();
}

void StrBlob::Push_back(const string & t)
{
	data->push_back(t);
}

string & StrBlob::front()
{
	return data->front();
}

string & StrBlob::back()
{
	return data->back();
}

string StrBlob::front()const
{
	return data->front();
}

string StrBlob::back()const
{
	return data->back();
}


class StrBlobPtr{
public:
	StrBlobPtr():curr(0){}
	StrBlobPtr(StrBlob & s, size_t sz = 0): wptr(s.data), curr(sz){}
	string &deference();
	StrBlobPtr & incr();
private:
	std::shared_ptr<vector<string>>
		check(std::size_t, const string &)const;
	std::weak_ptr<vector<string>> wptr;
	std::size_t curr;
};

std::shared_ptr<vector<string>>
StrBlobPtr::check(std::size_t cur, const string & s)const
{
	auto ret = wptr.lock();
	if (!ret)
		throw std::runtime_error("UnboundPtr !");
	else if (cur > ret->size())
		throw std::out_of_range(s);
	return ret;
}

string & StrBlobPtr::deference()
{
	auto ret = check(curr, "deference past the end");
	return (*ret)[curr];
}

StrBlobPtr & StrBlobPtr::incr()
{
	check(curr, "curr past the end now!");
	++curr;
	return *this;
}

int main()
{
	cout << "Please enter some information: ";
	string tmp;
	StrBlob strVec;
	while (cin >> tmp)
	{
		strVec.Push_back(tmp);
	}
	StrBlobPtr strPtr(strVec);
	for (int i = 0; i < strVec.size(); i++)
	{
		cout << strPtr.deference() << endl;
		strPtr.incr();
	}
	cin.get();
	return 0;
}