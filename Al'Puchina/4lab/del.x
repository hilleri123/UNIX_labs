
typedef string str<255>;


struct pol {
	str file;
	int seek;
	int size;
	str com;
	str s;
};


program DELPROG
{
	version DELVER
	{
		string DEL(pol) = 1;
	} = 1;
} = 22855;
