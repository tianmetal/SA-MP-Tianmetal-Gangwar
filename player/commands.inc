SSCANF:gangmenu(string[])
{
	if(!strcmp(string,"create",true)) return 1;
	if(!strcmp(string,"add",true)) return 1;
	if(!strcmp(string,"make",true)) return 1;
	if(!strcmp(string,"destroy",true)) return 2;
	if(!strcmp(string,"remove",true)) return 2;
	if(!strcmp(string,"delete",true)) return 2;
	if(!strcmp(string,"spawn",true)) return 3;
	if(!strcmp(string,"setspawn",true)) return 3;
	if(!strcmp(string,"preview",true)) return 4;
	if(!strcmp(string,"setpreview",true)) return 4;
	if(!strcmp(string,"skin",true)) return 5;
	if(!strcmp(string,"setskin",true)) return 5;
	if(!strcmp(string,"name",true)) return 6;
	if(!strcmp(string,"rename",true)) return 6;
	if(!strcmp(string,"setname",true)) return 6;
	if(!strcmp(string,"setlevel",true)) return 7;
	if(!strcmp(string,"setdonater",true)) return 8;
	if(!strcmp(string,"setadmin",true)) return 9;
	if(!strcmp(string,"setcolor",true)) return 10;
	if(!strcmp(string,"setcolour",true)) return 10;
	if(!strcmp(string,"color",true)) return 10;
	if(!strcmp(string,"colour",true)) return 10;
	return 0;
}

CMD:setgang(playerid,params[])
{
	if(PlayerInfo[playerid][AdminLevel] >= 7)
	{
		new action,subparam[100];
		if(!sscanf(params,"k<gangmenu>S()[100]",action,subparam))
		{
			switch(action)
			{
				case 0: SEM(playerid,"KEGUNAAN: /setgang [add/remove/spawn/preview/skin/name/setlevel/setdonater/setadmin/color]");
				case 1:
				{
					if(IsNull(subparam)) return SEM(playerid,"KEGUNAAN: /setgang add [name]");
					new name[MAX_GANG_NAME];
					strmid(name,subparam,0,strlen(subparam),sizeof(name));
					if(CreateGang(playerid,name))
					{
						new string[144];
						format(string,sizeof(string),"GANG: "WHITE"Gang "YELLOW"%s "WHITE"has been created!",name);
						SendClientMessage(playerid,X11_CYAN,string);
					}
					else SEM(playerid,"ERROR: Failed to create gang, gang slot is full");
				}
				case 2:
				{
					if(IsNull(params)) return SEM(playerid,"KEGUNAAN: /setgang remove [gangid]");
					new gangid = strval(subparam);
					if(Iter_Contains(Gangs,gangid))
					{
						new string[144];
						foreach(new i : GroupMember(GangGroup[gangid]))
						{
							RequestGangChange[playerid] = true;
							ForceClassSelection(playerid);
						}
						orm_delete(GangInfo[gangid][ORM_ID],true);
						orm_destroy(GangInfo[gangid][ORM_ID]);
						
						Group_Destroy(GangGroup[gangid]);
						Iter_Remove(Gangs,gangid);
						format(string,sizeof(string),"GANG: "WHITE"Gang with "YELLOW"id %d "WHITE"has been deleted!",gangid);
						SendClientMessage(playerid,X11_CYAN,string);
					}
					else SEM(playerid,"ERROR: Invalid gangid");
				}
				case 3:
				{
					if(IsNull(params)) return SEM(playerid,"KEGUNAAN: /setgang setspawn [gangid]");
					new gangid = strval(subparam);
					if(Iter_Contains(Gangs,gangid))
					{
						new Float:cPos[4];
						GetPlayerPos(playerid,cPos[0],cPos[1],cPos[2]);
						GetPlayerFacingAngle(playerid,cPos[3]);
						GangInfo[gangid][Spawn][0] = cPos[0];
						GangInfo[gangid][Spawn][1] = cPos[1];
						GangInfo[gangid][Spawn][2] = cPos[2];
						GangInfo[gangid][Spawn][3] = cPos[3];
						SendClientMessage(playerid,X11_CYAN,"GANG: "WHITE"Gang spawn has been updated!");
						orm_update(GangInfo[gangid][ORM_ID]);
					}
					else SEM(playerid,"ERROR: Invalid gangid");
				}
				case 4:
				{
					if(IsNull(params)) return SEM(playerid,"KEGUNAAN: /setgang preview [gangid]");
					new gangid = strval(subparam);
					if(Iter_Contains(Gangs,gangid))
					{
						new Float:cPos[4],Float:camPos[3];
						GetPlayerPos(playerid,cPos[0],cPos[1],cPos[2]);
						GetPlayerFacingAngle(playerid,cPos[3]);
						GetPlayerCameraPos(playerid,camPos[0],camPos[1],camPos[2]);
						GangInfo[gangid][PreviewPos][0] = cPos[0];
						GangInfo[gangid][PreviewPos][1] = cPos[1];
						GangInfo[gangid][PreviewPos][2] = cPos[2];
						GangInfo[gangid][PreviewPos][3] = cPos[3];
						GangInfo[gangid][PreviewCamera][0] = camPos[0];
						GangInfo[gangid][PreviewCamera][1] = camPos[1];
						GangInfo[gangid][PreviewCamera][2] = camPos[2];
						SendClientMessage(playerid,X11_CYAN,"GANG: "WHITE"Gang preview location has been updated!");
						orm_update(GangInfo[gangid][ORM_ID]);
					}
					else SEM(playerid,"ERROR: Invalid gangid");
				}
				case 5:
				{
					new gangid,slot,skinid;
					if(sscanf(subparam,"ddd",gangid,slot,skinid)) return SEM(playerid,"KEGUNAAN: /setgang skin [gangid] [slot (0-9)] [skinid]");
					if(Iter_Contains(Gangs,gangid))
					{
						if(slot < 0 || slot > 9) return SEM(playerid,"ERROR: Slot cannot go below 0 or above 9");
						if(skinid > 311 || skinid < 0) return SEM(playerid,"ERROR: Skinid cannot go below 0 or above 311");
						GangInfo[gangid][Skin][slot] = skinid;
						SendClientMessage(playerid,X11_CYAN,"GANG: "WHITE"Gang skins has been updated!");
						orm_update(GangInfo[gangid][ORM_ID]);
					}
					else SEM(playerid,"ERROR: Invalid gangid");
				}
				case 6:
				{
					new gangid,name[MAX_GANG_NAME];
					if(sscanf(subparam,"ds",gangid,name)) return SEM(playerid,"KEGUNAAN: /setgang name [gangid] [new name]");
					if(Iter_Contains(Gangs,gangid))
					{
						strmid(GangInfo[gangid][Name],name,0,strlen(name),MAX_GANG_NAME);
						Group_SetName(GangGroup[gangid],GangInfo[gangid][Name]);
						SendClientMessage(playerid,X11_CYAN,"GANG: "WHITE"Gang name has been updated!");
						orm_update(GangInfo[gangid][ORM_ID]);
					}
					else SEM(playerid,"ERROR: Invalid gangid");
				}
				case 7:
				{
					new gangid,level;
					if(sscanf(subparam,"dd",gangid,level)) return SEM(playerid,"KEGUNAAN: /setgang setlevel [gangid] [level]");
					if(Iter_Contains(Gangs,gangid))
					{
						GangInfo[gangid][RequireLevel] = level;
						SendClientMessage(playerid,X11_CYAN,"GANG: "WHITE"Gang level requirement has been updated!");
						orm_update(GangInfo[gangid][ORM_ID]);
					}
					else SEM(playerid,"ERROR: Invalid gangid");
				}
				case 8:
				{
					new gangid,level;
					if(sscanf(subparam,"dd",gangid,level)) return SEM(playerid,"KEGUNAAN: /setgang setdonate [gangid] [level]");
					if(Iter_Contains(Gangs,gangid))
					{
						GangInfo[gangid][RequireDonate] = level;
						SendClientMessage(playerid,X11_CYAN,"GANG: "WHITE"Gang donate rank requirement has been updated!");
						orm_update(GangInfo[gangid][ORM_ID]);
					}
					else SEM(playerid,"ERROR: Invalid gangid");
				}
				case 9:
				{
					new gangid,level;
					if(sscanf(subparam,"dd",gangid,level)) return SEM(playerid,"KEGUNAAN: /setgang setadmin [gangid] [level]");
					if(Iter_Contains(Gangs,gangid))
					{
						GangInfo[gangid][RequireAdmin] = level;
						SendClientMessage(playerid,X11_CYAN,"GANG: "WHITE"Gang donate admin requirement has been updated!");
						orm_update(GangInfo[gangid][ORM_ID]);
					}
					else SEM(playerid,"ERROR: Invalid gangid");
				}
				case 10:
				{
					new gangid,red,green,blue;
					if(sscanf(subparam,"dddd",gangid,red,green,blue)) return SEM(playerid,"KEGUNAAN: /setgang color [gangid] [red] [green] [blue]");
					if(Iter_Contains(Gangs,gangid))
					{
						GangInfo[gangid][Color] = RGBAToInt(red,green,blue,100);
						foreach(new i : GroupMember(GangGroup[gangid]))
						{
							SetPlayerColor(playerid,GangInfo[gangid][Color]);
						}
						SendClientMessage(playerid,X11_CYAN,"GANG: "WHITE"Gang donate admin requirement has been updated!");
						orm_update(GangInfo[gangid][ORM_ID]);
					}
					else SEM(playerid,"ERROR: Invalid gangid");
				}
			}
		}
		else SEM(playerid,"KEGUNAAN: /setgang [add/remove/spawn/preview/skin/name/setlevel/setdonater/setadmin/color]");
	}
	else SEM(playerid,"ERROR: You don't have enough admin privileges to use this command!");
	return 1;
}
CMD:kill(playerid,params[])
{
    SetPlayerHealth(playerid,0.0);
    return 1;
}
CMD:changegang(playerid,params[])
{
    RequestGangChange[playerid] = true;
    SEM(playerid,"NOTE: Please press F4 and on the next death you will return to gang selection screen!");
    return 1;
}