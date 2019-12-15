
typedef string str<255>;


struct num {
	short i16;
	long i32;
	float f32;
	double f64;
};
struct s_num {
	str file;
	num num;
};



program SENDERPROG
{
	version SENDERVER
	{
		int SENDER(s_num) = 1;
		num GETTER(string) = 2;
	} = 1;
} = 228;
