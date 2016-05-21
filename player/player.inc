enum E_PLAYER
{
	ORM:ORM_ID,
	ID,
	Name[MAX_PLAYER_NAME],
	Key[130],
	AdminLevel,
	DonateRank,
	DonateTime,
	PlayTime,
	Level,
	EXP,
	Kills,
	Deaths,
	Job,
	JobLevel[10],
	JobSkill[10],
	Cash,
	Coin,
	Warnings
}
new PlayerInfo[MAX_PLAYERS][E_PLAYER];

stock ResetPlayerData(playerid)
{
	PlayerInfo[playerid][ID] = 0;
	PlayerInfo[playerid][Name] = '\0';
	PlayerInfo[playerid][Key] = '\0';
	PlayerInfo[playerid][AdminLevel] = 0;
	PlayerInfo[playerid][DonateRank] = 0;
	PlayerInfo[playerid][DonateTime] = 0;
	PlayerInfo[playerid][PlayTime] = 0;
	PlayerInfo[playerid][Level] = 0;
	PlayerInfo[playerid][EXP] = 0;
	PlayerInfo[playerid][Kills] = 0;
	PlayerInfo[playerid][Deaths] = 0;
	PlayerInfo[playerid][Job] = 0;
	forex(i,10)
	{
		PlayerInfo[playerid][JobLevel][i] = 0;
		PlayerInfo[playerid][JobSkill][i] = 0;
	}
	PlayerInfo[playerid][Cash] = 0;
	PlayerInfo[playerid][Coin] = 0;
	PlayerInfo[playerid][Warnings] = 0;

    RequestGangChange[playerid] = true;
	PlayerLogged[playerid] = false;
	PlayerGang[playerid] = -1;
	PlayerClass[playerid] = -1;
	return 1;
}

stock RegisterPlayer(playerid,const password[])
{
	new ORM:ormid = PlayerInfo[playerid][ORM_ID];
	WP_Hash(PlayerInfo[playerid][Key],129,password);
	orm_insert(ormid,"Player_Register","d",playerid);
	return 1;
}

stock bool:CheckPassword(playerid,const password[])
{
	if(IsNull(password)) return false;
	new digest[130];
	WP_Hash(digest,129,password);
	if(strcmp(PlayerInfo[playerid][Key],digest,true)) return false;
	return true;
}

stock FetchPlayerPassword(playerid)
{
	new name[MAX_PLAYER_NAME],ORM:ormid;
	GetPlayerName(playerid,name,sizeof(name));
	strToLower(name);

	ormid = PlayerInfo[playerid][ORM_ID] = orm_create("user_data",Database);
	strmid(PlayerInfo[playerid][Name],name,0,strlen(name),MAX_PLAYER_NAME);

	orm_addvar_int(ormid,PlayerInfo[playerid][ID],"id");
	orm_addvar_string(ormid,PlayerInfo[playerid][Name],MAX_PLAYER_NAME,"name");
	orm_addvar_string(ormid,PlayerInfo[playerid][Key],130,"password");
	
	orm_setkey(ormid,"name");
	orm_select(ormid,"Player_FetchPassword","d",playerid);
	return 1;
}

stock FetchPlayerData(playerid)
{
    new ORM:ormid = PlayerInfo[playerid][ORM_ID];

	orm_addvar_int(ormid,PlayerInfo[playerid][AdminLevel],"adminlevel");
	orm_addvar_int(ormid,PlayerInfo[playerid][DonateRank],"donaterank");
	orm_addvar_int(ormid,PlayerInfo[playerid][DonateTime],"donatetime");
	orm_addvar_int(ormid,PlayerInfo[playerid][PlayTime],"playtime");
	orm_addvar_int(ormid,PlayerInfo[playerid][Level],"level");
	orm_addvar_int(ormid,PlayerInfo[playerid][EXP],"exp");
	orm_addvar_int(ormid,PlayerInfo[playerid][Kills],"kills");
	orm_addvar_int(ormid,PlayerInfo[playerid][Deaths],"deaths");
	orm_addvar_int(ormid,PlayerInfo[playerid][Job],"job");
	orm_addvar_int(ormid,PlayerInfo[playerid][JobLevel][0],"joblevel1");
	orm_addvar_int(ormid,PlayerInfo[playerid][JobLevel][1],"joblevel2");
	orm_addvar_int(ormid,PlayerInfo[playerid][JobLevel][2],"joblevel3");
	orm_addvar_int(ormid,PlayerInfo[playerid][JobLevel][3],"joblevel4");
	orm_addvar_int(ormid,PlayerInfo[playerid][JobLevel][4],"joblevel5");
	orm_addvar_int(ormid,PlayerInfo[playerid][JobLevel][5],"joblevel6");
	orm_addvar_int(ormid,PlayerInfo[playerid][JobLevel][6],"joblevel7");
	orm_addvar_int(ormid,PlayerInfo[playerid][JobLevel][7],"joblevel8");
	orm_addvar_int(ormid,PlayerInfo[playerid][JobLevel][8],"joblevel9");
	orm_addvar_int(ormid,PlayerInfo[playerid][JobLevel][9],"joblevel10");
	orm_addvar_int(ormid,PlayerInfo[playerid][JobSkill][0],"jobexp1");
	orm_addvar_int(ormid,PlayerInfo[playerid][JobSkill][1],"jobexp2");
	orm_addvar_int(ormid,PlayerInfo[playerid][JobSkill][2],"jobexp3");
	orm_addvar_int(ormid,PlayerInfo[playerid][JobSkill][3],"jobexp4");
	orm_addvar_int(ormid,PlayerInfo[playerid][JobSkill][4],"jobexp5");
	orm_addvar_int(ormid,PlayerInfo[playerid][JobSkill][5],"jobexp6");
	orm_addvar_int(ormid,PlayerInfo[playerid][JobSkill][6],"jobexp7");
	orm_addvar_int(ormid,PlayerInfo[playerid][JobSkill][7],"jobexp8");
	orm_addvar_int(ormid,PlayerInfo[playerid][JobSkill][8],"jobexp9");
	orm_addvar_int(ormid,PlayerInfo[playerid][JobSkill][9],"jobexp10");
	orm_addvar_int(ormid,PlayerInfo[playerid][Cash],"cash");
	orm_addvar_int(ormid,PlayerInfo[playerid][Coin],"coin");
	orm_addvar_int(ormid,PlayerInfo[playerid][Warnings],"warnings");

	orm_setkey(ormid,"id");
	orm_select(ormid,"Player_Login","d",playerid);
	return 1;
}
stock SavePlayer(playerid,bool:logout=false)
{
	orm_update(PlayerInfo[playerid][ORM_ID]);
	if(logout) orm_destroy(PlayerInfo[playerid][ORM_ID]);
	return 1;
}

function Player_FetchPassword(playerid)
{
    switch(orm_errno(PlayerInfo[playerid][ORM_ID]))
	{
		case ERROR_OK:
		{
			ShowPlayerDialog(playerid,DIALOG_USER_LOGIN,DIALOG_STYLE_PASSWORD,"Login to "SERVER_NAME_SHORT"","This account is already registered, please input password to login\n"WHITE"Login Attempts: "YELLOW"0/5","Login","Quit");
		}
		case ERROR_NO_DATA:
		{
		    ShowPlayerDialog(playerid,DIALOG_USER_REGISTER,DIALOG_STYLE_PASSWORD,"Register to "SERVER_NAME_SHORT"","You are not registered here, please input password to register this account","Register","Quit");
		}
	}
	orm_setkey(PlayerInfo[playerid][ORM_ID],"id");
	return 1;
}
function Player_Login(playerid)
{
	PlayerLogged[playerid] = true;
	PreviewGang(playerid,0);
	return 1;
}

function Player_Register(playerid)
{
    if(orm_errno(PlayerInfo[playerid][ORM_ID]) == ERROR_OK)
    {
		FetchPlayerData(playerid);
    }
	return 1;
}