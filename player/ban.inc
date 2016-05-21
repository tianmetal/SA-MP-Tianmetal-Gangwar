stock CheckPlayerBanStatus(playerid)
{
	new string[512],name[MAX_PLAYER_NAME],ip[20];
	GetPlayerName(playerid,name,sizeof(name));
	GetPlayerIp(playerid,ip,sizeof(ip));
	format(string,sizeof(string),"SELECT `user`,`ip`,`issuer`,`reason`,UNIX_TIMESTAMP(`date`),UNIX_TIMESTAMP(`unbandate`) FROM `ban_data` WHERE (`user`='%s' OR `ip`='%s') AND (`unbandate` IS NULL OR `unbandate`>FROM_UNIXTIME(%d)) LIMIT 1",
		name,ip,gettime());
	mysql_pquery(Database,string,"BanList_Check","d",playerid);
	return 1;
}
function BanList_Check(playerid)
{
	if(cache_get_row_count(Database) == 1)
	{
	    new string[512],name[MAX_PLAYER_NAME],issuer[MAX_PLAYER_NAME],ip[20],reason[128],bandate,unbandate;
		cache_get_row(0,0,name,Database,sizeof(name));
		cache_get_row(0,1,ip,Database,sizeof(ip));
		cache_get_row(0,2,issuer,Database,sizeof(issuer));
		cache_get_row(0,3,reason,Database,sizeof(reason));
		bandate = cache_get_row_int(0,4,Database);
		unbandate = cache_get_row_int(0,5,Database);
		format(string,sizeof(string),"{00ffff}This user or ip is currently banned!\n\n{ffffff}Name: {ffff00}%s\n{ffffff}IP: {ffff00}%s\n{ffffff}Issuer: {ffff00}%s\n{ffffff}Reason: {ffff00}%s\n{ffffff}Ban date: {ffff00}%s\n{ffffff}Unban date: {ffff00}%s\n\n",
		    name,ip,issuer,reason,ConvertTimestamp(bandate),((unbandate == 0) ? ("Not Available") : (ConvertTimestamp(unbandate))));
		strcat(string,"{00ffff}If you are wrongfully banned, please make an unban appeal at {ff0000}http://jogjagamers.com/forum/",sizeof(string));
		ShowPlayerDialog(playerid,0,DIALOG_STYLE_MSGBOX,"You are banned",string,"Close","");
		defer DelayKick(playerid);
	}
	else
	{
        FetchPlayerPassword(playerid);
	}
	return 1;
}
function BanList_InsertNormal(playerid,targetid,reason[])
{
	if(mysql_affected_rows(Database))
	{
	    new string[144];
	    format(string,sizeof(string),"AdmCmd: %s has been banned by %s.",PlayerInfo[targetid][Name],PlayerInfo[playerid][Name]);
		SendClientMessageToAll(X11_INDIANRED,string);
		format(string,sizeof(string),"Reason: %s",reason);
		SendClientMessageToAll(X11_INDIANRED,string);
		defer DelayKick(targetid);
	}
	return 1;
}
function BanList_InsertAccount(playerid,targetid,reason[])
{
	if(mysql_affected_rows(Database))
	{
	    new string[144];
	    format(string,sizeof(string),"AdmCmd: %s has been account banned by %s.",PlayerInfo[targetid][Name],PlayerInfo[playerid][Name]);
		SendClientMessageToAll(X11_INDIANRED,string);
		format(string,sizeof(string),"Reason: %s",reason);
		SendClientMessageToAll(X11_INDIANRED,string);
		defer DelayKick(targetid);
	}
	return 1;
}
function BanList_InsertIP(playerid,ip[],reason[])
{
	if(mysql_affected_rows(Database))
	{
	    new string[144],cip[20];
	    format(string,sizeof(string),"AdmCmd: %s has banned IP %s",PlayerInfo[playerid][Name],ip);
		SendClientMessageToAll(X11_INDIANRED,string);
		format(string,sizeof(string),"Reason: %s",reason);
		SendClientMessageToAll(X11_INDIANRED,string);
		foreach(new i : Player)
		{
			GetPlayerIp(i,cip,sizeof(cip));
			if(strcmp(ip,cip,true)) continue;
			defer DelayKick(i);
		}
	}
	return 1;
}
function BanList_InsertTimed(playerid,targetid,time,reason[])
{
	if(mysql_affected_rows(Database))
	{
	    new string[144];
	    format(string,sizeof(string),"AdmCmd: %s has been account banned by %s for %d day%s",
			PlayerInfo[targetid][Name],PlayerInfo[playerid][Name],time,((time == 1) ? (".") : ("s.")));
		SendClientMessageToAll(X11_INDIANRED,string);
		format(string,sizeof(string),"Reason: %s",reason);
		SendClientMessageToAll(X11_INDIANRED,string);
		defer DelayKick(targetid);
	}
	return 1;
}