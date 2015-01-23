#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <cassert>
#include <vector>
#include <list>
using namespace std;

struct Node
{
	string seq;
	vector<int> loc;
	Node(const string& s, int i) : seq(s){loc.push_back(i);}
};

//Hash table with chaining
class HashTable
{
	public:
		HashTable(int i) : m_size(i), m_hashtable(m_size / 8){}
		unsigned int HashFunc(string s);
		int Size(){return m_hashtable.size();}
		void stringToHash(string s, int SEQUENCE_SIZE);
		void Add(unsigned int loc, Node* node);
		int Find(string s);
		Node* findNode(string s);
	private:
		int m_size;
		vector<list<Node> > m_hashtable;
};

unsigned int HashTable::HashFunc(string s)
{
	unsigned long hash = 5381;
	for(int i = 0; i < s.size(); i++)
		hash = (hash << 5) + hash + s[i];
	return hash;
}

void HashTable::Add(unsigned int location, Node* node)
{
	if(m_hashtable[location].size() == 0)
	{	
		list<Node> nodeList;
		nodeList.push_front(*node);
		m_hashtable[location] = nodeList;
	}
	else
	{
		if(Find(node->seq) == -1)
			m_hashtable[location].push_front(*node);
		else
			findNode(node->seq)->loc.push_back(node->loc[0]);//keeps track of all positions at which this string has occurred
	}
}

Node* HashTable::findNode(string s)
{
	unsigned int k = HashFunc(s) % m_hashtable.size();

	if(m_hashtable[k].size() == 0)
		return NULL;
	else
	{
		list<Node>::iterator it;
		for(it = m_hashtable[k].begin(); it != m_hashtable[k].end(); it++)
		{	
			if((*it).seq == s)
				return &(*it);
		}
	}
	return NULL;
}

void HashTable::stringToHash(string s, int SEQUENCE_SIZE)
{
	unsigned int m;
	if(s.length() > 110000)
		m = 4;
	else
		m = 1;
	for(int i = 0; i * m < s.length(); i++)
	{
		string temp = s.substr(i * m, SEQUENCE_SIZE);
		Node* j = new Node(temp, i * m);
		unsigned int k = HashFunc(j->seq) % Size();
		Add(k, j);
	}
}

int HashTable::Find(string s)
{
	unsigned int k = HashFunc(s) % m_hashtable.size();

	if(m_hashtable[k].size() == 0)
		return -1;
	else
	{
		list<Node>::iterator it;
		for(it = m_hashtable[k].begin(); it != m_hashtable[k].end(); it++)
		{	
			if((*it).seq == s)
				return k;
		}
	}
	return -1;
}

string fileToString(istream& file)
{
	string temp, fileString;
	
	int counter = 0;
	while(getline(file, temp))
	{
		if(counter != 0)
			fileString += '\n';
		fileString += temp;
		temp.clear();
		counter++;
	}
	return fileString;
}

string createDeltah(istream& oldf, istream& newf, string oldfstring, string newfstring, int SEQUENCE_SIZE)
{
	string searchstring, temp, returnstring;
	stringstream out;
	int j = 0, k, l = SEQUENCE_SIZE;
	int bigloc = 0, bigsize = 0;

	HashTable hashtable(oldfstring.length());
	hashtable.stringToHash(oldfstring, SEQUENCE_SIZE);//creates hash table

	while(j < newfstring.length())
	{	
		searchstring = newfstring.substr(j, SEQUENCE_SIZE);
		k = hashtable.Find(searchstring);
		if(k != -1) //found it
		{	
			if(temp.length() != 0)//if there's anything in the 'buffer' do an add command before we do the copy command
			{	
				returnstring += "A";
				out << temp.length();
				returnstring += out.str();
				out.str("");
				returnstring += ":";
				returnstring += temp;
				temp.clear();
			}
			Node* node = hashtable.findNode(searchstring);
			
			for(int m = 0; m < node->loc.size(); m++)
			{
				k = node->loc[m];
				while(k + l < oldfstring.length() && j + l < newfstring.length() && oldfstring[k + l] == newfstring[j + l])//how many extra characters can we find?
					l++;
				if(l > bigsize)//we want to copy the most characters as possible
				{	
					bigsize = l;
					bigloc = k;
				}
			}
				returnstring += "C";
				out << bigsize;
				returnstring += out.str();
				out.str("");
				returnstring += ",";
				out << bigloc;
				returnstring += out.str();
				out.str("");
				j += bigsize;
				l = SEQUENCE_SIZE;
				bigsize = 0;
				bigloc = 0;
		}
		else
		{
			temp += newfstring[j];
			j++;
		}
	}

	if(temp.length() != 0)//the final add command
	{	
		returnstring += "A";
		out << temp.length();
		returnstring += out.str();
		out.str("");
		returnstring += ":";
		returnstring += temp;
	}

	return returnstring;
}

void createDelta(istream& oldf, istream& newf, ostream& deltaf)
{
	string oldfstring, newfstring;
	
	oldfstring = fileToString(oldf);
	newfstring = fileToString(newf);
	
	string str1 = createDeltah(oldf, newf, oldfstring, newfstring, 4);
	string str2 = createDeltah(oldf, newf, oldfstring, newfstring, 10);
	
	//cout << str1.length() << endl;
	//cout << str2.length() << endl;

	if(str1.length() < str2.length())//helps optimize for different file sizes
		deltaf << str1;
	else
		deltaf << str2;
}

//This function takes the content of a file A and a delta file, and will apply the instructions 
//in the delta file to produce a new file A'. Each day, every device will use this function to 
//update the previous day's inventory file.

bool getInt(istream& inf, int& n)
{
	char ch;
	if (!inf.get(ch)  ||  !isascii(ch)  ||  !isdigit(ch))
	    return false;
	inf.unget();
	inf >> n;
	return true;
}

bool getCommand(istream& inf, char& cmd, int& length, int& offset)
{
	if (!inf.get(cmd)  ||  (cmd == '\n'  &&  !inf.get(cmd)) )
	{
	    cmd = 'x';  // signals end of file
	    return true;
	}
	char ch;
	switch (cmd)
	{
	    case 'A':
	    return getInt(inf, length) && inf.get(ch) && ch == ':';
	    case 'C':
	    return getInt(inf, length) && inf.get(ch) && ch == ',' && getInt(inf, offset);
	}
	return false;
}

bool applyDelta(istream& oldf, istream& deltaf, ostream& newf)
{
	//if(theres a character other than an A or C where a command is expected, or an offset or length is invalid)
		//return false;
	char cmd;
	bool run;
	int length, offset = 0;
	string oldfstring;
	oldfstring = fileToString(oldf);
	run = getCommand(deltaf, cmd, length, offset);
	while(cmd != 'x' && run == true)
	{	
		if(length < 0 || offset < 0 || !(cmd == 'C' || cmd == 'A'))
			return false;
		else if(cmd == 'C')
			newf << oldfstring.substr(offset, length);
		else if(cmd == 'A')
		{	
			for(int i = 0; i < length; i++)
			{
				char ch;
				deltaf.get(ch);
				newf << ch;
			}
		}
		run = getCommand(deltaf, cmd, length, offset);
	}
	if(cmd == 'x' && run == true)
		return true;
	else
		return false;
}


//This function was written by Professor Smallberg at UCLA and provided to students
bool runtest(string oldname, string newname, string deltaname, string newname2)
{
	ifstream oldfile(oldname);
	if (!oldfile)
	{
	    cerr << "Cannot open " << oldname << endl;
	    return false;
	}
	ifstream newfile(newname);
	if (!newfile)
	{
	    cerr << "Cannot open " << newname << endl;
	    return false;
	}
	ofstream deltafile(deltaname);
	if (!deltafile)
	{
	    cerr << "Cannot create " << deltaname << endl;
	    return false;
	}
	createDelta(oldfile, newfile, deltafile);
	deltafile.close();

	oldfile.clear();   // clear the end of file condition
	oldfile.seekg(0);  // reset back to beginning of the file
	ifstream deltafile2(deltaname);
	if (!deltafile2)
	{
	    cerr << "Cannot read the " << deltaname << " that was just created!" << endl;
	    return false;
	}
	ofstream newfile2(newname2);
	if (!newfile2)
	{
	    cerr << "Cannot create " << newname2 << endl;
	    return false;
	}
	assert(applyDelta(oldfile, deltafile2, newfile2));
	cout << "You must compare " << newname << " and " << newname2 << endl;
	cout << "If they are not identical, you failed this test." << endl;
	return true;
}

int main()
{
	ifstream oldf, newf;
	ofstream deltaf;

	oldf.open("oldf.txt");
	deltaf.open("deltaf.txt");
	newf.open("newf.txt");

	createDelta(oldf, newf, deltaf);
	deltaf.close();
	newf.close();
	oldf.close();
	ifstream deltafnew, oldfnew;
	ofstream newfnew;
	deltafnew.open("deltaf.txt");
	oldfnew.open("oldf.txt");
	newfnew.open("output.txt");
	applyDelta(oldfnew, deltafnew, newfnew);

	oldf.close();
	deltafnew.close();
	newfnew.close();

	assert(runtest("myoldfile.txt", "mynewfile.txt", "mydeltafile.txt", "mynewfile2.txt"));
}