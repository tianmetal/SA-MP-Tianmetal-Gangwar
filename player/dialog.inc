stock CallDialog(playerid,dialogid,response,listitem,inputtext[])
{
	#pragma unused listitem
    switch(dialogid)
    {
        case DIALOG_USER_REGISTER:
        {
			if(response)
			{
				if(IsNull(inputtext) || (strlen(inputtext) < 6))
				{
					ShowPlayerDialog(playerid,DIALOG_USER_REGISTER,DIALOG_STYLE_PASSWORD,"Register to "SERVER_NAME_SHORT"","You are not registered here, please input password to register this account\n"WHITE"HINT: "CYAN"Use at least 6 characters password","Register","Quit");
				}
				else
				{
					RegisterPlayer(playerid,inputtext);
				}
			}
			else
			{
				defer DelayKick(playerid);
			}
        }
        case DIALOG_USER_LOGIN:
        {
			if(response)
			{
				if(CheckPassword(playerid,inputtext))
				{
					DeletePVar(playerid,"LoginTries");
					FetchPlayerData(playerid);
				}
				else
				{
					new string[128],
						attempts = GetPVarInt(playerid,"LoginTries");
					attempts++;
					if(attempts == 5)
					{
						SEM(playerid,"ERROR: You've used all 5 login attempts, please relog and try again!");
						defer DelayKick(playerid);
					}
					else
					{
						format(string,sizeof(string),"This account is already registered, please input password to login\n"WHITE"Login Attempts: "YELLOW"%d/5",attempts);
						ShowPlayerDialog(playerid,DIALOG_USER_LOGIN,DIALOG_STYLE_PASSWORD,"Login to "SERVER_NAME_SHORT"",string,"Login","Quit");
                        SetPVarInt(playerid,"LoginTries",attempts);
                    }
				}
			}
			else
			{
				defer DelayKick(playerid);
			}
        }
    }
	return 1;
}