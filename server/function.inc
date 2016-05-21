#define forex(%0,%1) for(new %0 = 0; %0 < %1; %0++)
#define RGBAToInt(%0,%1,%2,%3) ((16777216 * (%0)) + (65536 * (%1)) + (256 * (%2)) + (%3))
#define strToLower(%0) \
    for(new i; %0[i] != EOS; ++i) \
        %0[i] = ('A' <= %0[i] <= 'Z') ? (%0[i] += 'a' - 'A') : (%0[i])
#define IsOdd(%0)  ((%0)&1)
#define IsEven(%0) (!IsOdd(%0))
#define SEM(%0,%1) SendClientMessage(%0,0xBFC0C200,%1)
#define IsNull(%1) ((!(%1[0])) || (((%1[0]) == '\1') && (!(%1[1]))))
#define Pressed(%0) ((newkeys & %0) && !(oldkeys & %0))
#define Holding(%0) ((newkeys & (%0)) == (%0))
#define Released(%0) (((newkeys & (%0)) != (%0)) && ((oldkeys & (%0)) == (%0)))
#define IsValidComponent(%0) (1194 > (%0) >= 1000)
#define IntToBool(%0) bool:(%0)
#define IsValidSkin(%0) (311 >= (%0) > 0)
#define function%0(%1) \
        forward%0(%1); public%0(%1)

stock ConvertTimestamp(timestamp,bool:date=true)
{
    new tm <tmStamp>;
	localtime(Time:timestamp, tmStamp);
	new string[64];
	if(date) strftime(string, sizeof(string), "%a %d %b %Y, %H:%M:%S", tmStamp);
	else strftime(string, sizeof(string), "%H:%M:%S", tmStamp);
	return string;
}

stock ValidateLine(string[])
{
	new len = strlen(string);
	if(string[0]==0) return ;
	if((string[len - 1] == '\n') || (string[len - 1] == '\r'))
	{
		string[len - 1] = 0;
		if(string[0]==0) return ;
		if((string[len - 2] == '\n') || (string[len - 2] == '\r')) string[len - 2] = 0;
	}
}