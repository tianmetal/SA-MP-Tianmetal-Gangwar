#define MAX_PLAYERS         (50)

#include <a_samp>           // SA:MP Default Include
#include <sscanf2>          // SSCANF2 plugin by Y_Less
#include <whirlpool>        // Whirlpool plugin by Y_Less
#include <a_mysql>          // MySQL plugin by BlueG
#include <SKY>              // SKY plugin by oscar-broman
#include <ctime>            // CTime plugin by RyDeR`
#include <crashdetect>

#define GAMEMODE            // This is a Gamemode

#include <YSI\y_master>     // YSI Include Start
#include <YSI\y_ini>
#include <YSI\y_iterate>
#include <YSI\y_groups>
#include <YSI\y_timers>
#include <YSI\y_commands>
#include <YSI\y_colours>    // YSI Include End

#include <weapon-config>    // Weapon-config include by oscar-broman

#include "./server/settings.inc"
#include "./server/function.inc"
#include "./server/variables.inc"

#include "./player/player.inc"
#include "./player/ban.inc"
#include "./player/gang.inc"
#include "./player/dialog.inc"
#include "./player/commands.inc"

#include "./server/timer.inc"

main() { }

public OnGameModeInit()
{
    new string[128],version[32];
    Database = mysql_connect(MYSQL_HOST,MYSQL_USER,MYSQL_DATABASE,MYSQL_PASSWORD);

	UsePlayerPedAnims();
	AllowInteriorWeapons(1);
    DisableInteriorEnterExits();
	SetVehiclePassengerDamage(true);
    SetCustomFallDamage(true, 40.0, -0.75);
    SetDisableSyncBugs(true);
    SetCustomVendingMachines(false);

	GetServerVarAsString("version",version,sizeof(version));
	format(string,sizeof(string),"hostname "SERVER_NAME" [%s]",version);
	SendRconCommand(string);
	format(string,sizeof(string),"%s %s",SERVER_NAME_SHORT,GAMEMODE_VERSION);
	SetGameModeText(string);
	SetTeamCount(MAX_GANGS);
	
	AddPlayerClass(1,0.0,0.0,3.0,0.0,0,0,0,0,0,0);
	AddPlayerClass(2,0.0,0.0,3.0,0.0,0,0,0,0,0,0);
	AddPlayerClass(3,0.0,0.0,3.0,0.0,0,0,0,0,0,0);
	
	LoadGangs();
	
	return 1;
}

public OnGameModeExit()
{
	foreach(new i : Player)
	{
		if(!PlayerLogged[i]) continue;
		SavePlayer(i,true);
	}
	foreach(new g : Gangs)
	{
		orm_destroy(GangInfo[g][ORM_ID]);
	}
	mysql_close(Database);
	return 1;
}

public OnPlayerConnect(playerid)
{
	ResetPlayerData(playerid);
	return 1;
}
public OnPlayerDisconnect(playerid,reason)
{
	SavePlayer(playerid,true);
	return 1;
}
public OnPlayerSpawn(playerid)
{
	if(PlayerLogged[playerid])
	{
	    TogglePlayerControllable(playerid,1);
	    SetPlayerVirtualWorld(playerid,0);
	    SetPlayerTeam(playerid,PlayerGang[playerid]);
	}
	return 1;
}
public OnPlayerRequestClass(playerid,classid)
{
	new oldclass = GetPVarInt(playerid,"SelectClass");
	if(PlayerLogged[playerid])
	{
		if(RequestGangChange[playerid])
		{
		    new gangid = GetPVarInt(playerid,"SelectGang");
		    if((oldclass == 0 && classid == 1) || (oldclass == 1 && classid == 2) || (oldclass == 2 && classid == 0))
		    {
		        gangid++;
		        if(gangid >= Iter_Count(Gangs)) gangid = 0;
		    }
		    else
		    {
		        gangid--;
		        if(gangid == -1) gangid = (Iter_Count(Gangs)-1);
		    }
		    PreviewGang(playerid,gangid);
		    SetPVarInt(playerid,"SelectGang",gangid);
		}
		else
		{
		    new gangid = PlayerGang[playerid];
		    new skinslot = GetPVarInt(playerid,"SelectSkin");
		    if((oldclass == 0 && classid == 1) || (oldclass == 1 && classid == 2) || (oldclass == 2 && classid == 0))
		    {
		        skinslot++;
		        if((skinslot == 10) || (GangInfo[gangid][Skin][skinslot] == 0)) skinslot = 0;
		    }
		    else
		    {
		        skinslot--;
		        if(skinslot == -1)
		        {
		            for(new i = 9; i >= 0; i--)
		            {
		                if(GangInfo[gangid][Skin][i] == 0) continue;
		                skinslot = i;
		                break;
		            }
		        }
		    }
		    PreviewGangSkin(playerid,skinslot);
		    SetPVarInt(playerid,"SelectSkin",skinslot);
		}
	}
	else
	{
	    CheckPlayerBanStatus(playerid);
	}
	SetPVarInt(playerid,"SelectClass",classid);
	return 1;
}

public OnPlayerRequestSpawn(playerid)
{
	if(!PlayerLogged[playerid]) return 0;
	if(RequestGangChange[playerid])
	{
	    new string[128];
	    new gangid = GetPVarInt(playerid,"SelectGang");
	    if(PlayerInfo[playerid][Level] >= GangInfo[gangid][RequireLevel])
	    {
	        if(PlayerInfo[playerid][DonateRank] >= GangInfo[gangid][RequireDonate])
	        {
	            if(PlayerInfo[playerid][AdminLevel] >= GangInfo[gangid][RequireAdmin])
	            {
	                SetPlayerGang(playerid,gangid);
	                PreviewGangSkin(playerid,0);
	                RequestGangChange[playerid] = false;
	        		DeletePVar(playerid,"SelectGang");
	            }
	            else
	            {
                    format(string,sizeof(string),"ERROR: This gang requires admin level %d",GangInfo[gangid][RequireAdmin]);
	        		SEM(playerid,string);
	            }
	        }
	        else
	        {
	            format(string,sizeof(string),"ERROR: This gang requires donate rank level %d",GangInfo[gangid][RequireDonate]);
	        	SEM(playerid,string);
	        }
	    }
	    else
	    {
	        format(string,sizeof(string),"ERROR: This gang requires player level %d",GangInfo[gangid][RequireLevel]);
	        SEM(playerid,string);
	    }
	    return 0;
	}
	else
	{
	    new gangid = PlayerGang[playerid];
	    new skinslot = GetPVarInt(playerid,"SelectSkin");
	    SetSpawnInfo(playerid,gangid,GangInfo[gangid][Skin][skinslot],GangInfo[gangid][Spawn][0],GangInfo[gangid][Spawn][1],GangInfo[gangid][Spawn][2],GangInfo[gangid][Spawn][3],0,0,0,0,0,0);
		SetCameraBehindPlayer(playerid);
		DeletePVar(playerid,"SelectSkin");
	}
    return 1;
}

public OnDialogResponse(playerid,dialogid,response,listitem,inputtext[])
{
    return CallDialog(playerid,dialogid,response,listitem,inputtext);
}
