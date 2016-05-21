enum E_GANG
{
	ORM:ORM_ID,
	ID,
	Name[MAX_GANG_NAME],
	Color,
	RequireLevel,
	RequireDonate,
	RequireAdmin,
	Float:Spawn[4],
	SpawnInt,
	Skin[10],
	Float:PreviewPos[4],
	Float:PreviewCamera[3],
}
new GangInfo[MAX_GANGS][E_GANG];
new Iterator:Gangs<MAX_GANGS>;
new Group:GangGroup[MAX_GANGS];

stock LoadGangs()
{
	mysql_pquery(Database,"SELECT * FROM `gang_data` ORDER BY `id` ASC","Gang_Load","");
	return 1;
}
stock bool:CreateGang(playerid,name[])
{
	new id = Iter_Free(Gangs);
	if(id == -1) return false;
    if(Iter_Contains(Gangs,id)) return false;
	
	new Float:cPos[4],Float:camPos[3];
	new ORM:ormid = GangInfo[id][ORM_ID] = orm_create("gang_data",Database);
	orm_addvar_int(ormid,GangInfo[id][ID],"id");
	orm_addvar_string(ormid,GangInfo[id][Name],MAX_GANG_NAME,"name");
	orm_addvar_int(ormid,GangInfo[id][Color],"color");
	orm_addvar_float(ormid,GangInfo[id][Spawn][0],"spawn_x");
	orm_addvar_float(ormid,GangInfo[id][Spawn][1],"spawn_y");
	orm_addvar_float(ormid,GangInfo[id][Spawn][2],"spawn_z");
	orm_addvar_float(ormid,GangInfo[id][Spawn][3],"spawn_a");
	orm_addvar_int(ormid,GangInfo[id][SpawnInt],"spawn_int");
	orm_addvar_float(ormid,GangInfo[id][PreviewPos][0],"preview_pos_x");
	orm_addvar_float(ormid,GangInfo[id][PreviewPos][1],"preview_pos_y");
	orm_addvar_float(ormid,GangInfo[id][PreviewPos][2],"preview_pos_z");
	orm_addvar_float(ormid,GangInfo[id][PreviewPos][3],"preview_pos_a");
	orm_addvar_float(ormid,GangInfo[id][PreviewCamera][0],"preview_camera_x");
	orm_addvar_float(ormid,GangInfo[id][PreviewCamera][1],"preview_camera_y");
	orm_addvar_float(ormid,GangInfo[id][PreviewCamera][2],"preview_camera_z");
	
	GetPlayerPos(playerid,cPos[0],cPos[1],cPos[2]);
	GetPlayerFacingAngle(playerid,cPos[3]);
	GetPlayerCameraPos(playerid,camPos[0],camPos[1],camPos[2]);
	
	GangInfo[id][ID] = id;
	strmid(GangInfo[id][Name],name,0,strlen(name),MAX_GANG_NAME);
	GangInfo[id][Color] = RGBAToInt(random(256),random(256),random(256),100);
	forex(i,3)
	{
		GangInfo[id][Spawn][i] = cPos[i];
		GangInfo[id][PreviewPos][i] = cPos[i];
		GangInfo[id][PreviewCamera][i] = camPos[i];
	}
	GangInfo[id][Spawn][3] = cPos[3];
	GangInfo[id][PreviewPos][3] = cPos[3];
	GangInfo[id][SpawnInt] = GetPlayerInterior(playerid);
	
	orm_insert(GangInfo[id][ORM_ID],"Gang_Create","d",id);
	return true;
}

stock SetPlayerGang(playerid,gangid)
{
	PlayerGang[playerid] = gangid;
	foreach(new gang : Gangs)
	{
		if(gangid != gang) Group_SetPlayer(GangGroup[gang],playerid,false);
		else Group_SetPlayer(GangGroup[gang],playerid,true);
	}
	return 1;
}
stock PreviewGang(playerid,id)
{
	if(Iter_Contains(Gangs,id))
	{
        SetPlayerVirtualWorld(playerid,1);
        TogglePlayerControllable(playerid,0);
        SetPlayerPos(playerid,GangInfo[id][PreviewPos][0],GangInfo[id][PreviewPos][1],GangInfo[id][PreviewPos][2]-3.0);
		SetPlayerFacingAngle(playerid,GangInfo[id][PreviewPos][3]);
		SetPlayerCameraPos(playerid,GangInfo[id][PreviewCamera][0],GangInfo[id][PreviewCamera][1],GangInfo[id][PreviewCamera][2]);
		SetPlayerCameraLookAt(playerid,GangInfo[id][PreviewPos][0],GangInfo[id][PreviewPos][1],GangInfo[id][PreviewPos][2],CAMERA_MOVE);
	}
	return 1;
}
stock PreviewGangSkin(playerid,skinslot)
{
	new id = PlayerGang[playerid];
	if(id != -1)
	{
        SetPlayerVirtualWorld(playerid,1);
        TogglePlayerControllable(playerid,0);
		SetPlayerSkin(playerid,GangInfo[id][Skin][skinslot]);
		SetPlayerPos(playerid,GangInfo[id][PreviewPos][0],GangInfo[id][PreviewPos][1],GangInfo[id][PreviewPos][2]);
		SetPlayerFacingAngle(playerid,GangInfo[id][PreviewPos][3]);
		SetPlayerCameraPos(playerid,GangInfo[id][PreviewCamera][0],GangInfo[id][PreviewCamera][1],GangInfo[id][PreviewCamera][2]);
		SetPlayerCameraLookAt(playerid,GangInfo[id][PreviewPos][0],GangInfo[id][PreviewPos][1],GangInfo[id][PreviewPos][2]);
	}
	return 1;
}

function Gang_Create(id)
{
    new ORM:ormid = GangInfo[id][ORM_ID];
    orm_addvar_int(ormid,GangInfo[id][RequireLevel],"require_level");
    orm_addvar_int(ormid,GangInfo[id][RequireDonate],"require_donate");
    orm_addvar_int(ormid,GangInfo[id][RequireAdmin],"require_admin");
    orm_addvar_int(ormid,GangInfo[id][Skin][0],"skin1");
    orm_addvar_int(ormid,GangInfo[id][Skin][1],"skin2");
    orm_addvar_int(ormid,GangInfo[id][Skin][2],"skin3");
    orm_addvar_int(ormid,GangInfo[id][Skin][3],"skin4");
    orm_addvar_int(ormid,GangInfo[id][Skin][4],"skin5");
    orm_addvar_int(ormid,GangInfo[id][Skin][5],"skin6");
    orm_addvar_int(ormid,GangInfo[id][Skin][6],"skin7");
    orm_addvar_int(ormid,GangInfo[id][Skin][7],"skin8");
    orm_addvar_int(ormid,GangInfo[id][Skin][8],"skin9");
    orm_addvar_int(ormid,GangInfo[id][Skin][9],"skin10");
    orm_setkey(ormid,"id");
    
    Iter_Add(Gangs,id);
    GangGroup[id] = Group_Create(GangInfo[id][Name]);
    
    orm_select(ormid);
	return 1;
}
function Gang_Load()
{
	new rows = cache_get_row_count(Database);
	if(rows != 0)
	{
	    forex(row,rows)
	    {
	        new id = cache_get_row_int(row,0,Database);
	        new ORM:ormid = GangInfo[id][ORM_ID] = orm_create("gang_data",Database);
	        orm_addvar_int(ormid,GangInfo[id][ID],"id");
	        orm_addvar_string(ormid,GangInfo[id][Name],MAX_GANG_NAME,"name");
	        orm_addvar_int(ormid,GangInfo[id][Color],"color");
	        orm_addvar_int(ormid,GangInfo[id][RequireLevel],"require_level");
	        orm_addvar_int(ormid,GangInfo[id][RequireDonate],"require_donate");
	        orm_addvar_int(ormid,GangInfo[id][RequireAdmin],"require_admin");
	        orm_addvar_float(ormid,GangInfo[id][Spawn][0],"spawn_x");
	        orm_addvar_float(ormid,GangInfo[id][Spawn][1],"spawn_y");
	        orm_addvar_float(ormid,GangInfo[id][Spawn][2],"spawn_z");
	        orm_addvar_float(ormid,GangInfo[id][Spawn][3],"spawn_a");
			orm_addvar_int(ormid,GangInfo[id][SpawnInt],"spawn_int");
	        orm_addvar_int(ormid,GangInfo[id][Skin][0],"skin1");
	        orm_addvar_int(ormid,GangInfo[id][Skin][1],"skin2");
	        orm_addvar_int(ormid,GangInfo[id][Skin][2],"skin3");
	        orm_addvar_int(ormid,GangInfo[id][Skin][3],"skin4");
	        orm_addvar_int(ormid,GangInfo[id][Skin][4],"skin5");
	        orm_addvar_int(ormid,GangInfo[id][Skin][5],"skin6");
	        orm_addvar_int(ormid,GangInfo[id][Skin][6],"skin7");
	        orm_addvar_int(ormid,GangInfo[id][Skin][7],"skin8");
	        orm_addvar_int(ormid,GangInfo[id][Skin][8],"skin9");
	        orm_addvar_int(ormid,GangInfo[id][Skin][9],"skin10");
	        orm_addvar_float(ormid,GangInfo[id][PreviewPos][0],"preview_pos_x");
	        orm_addvar_float(ormid,GangInfo[id][PreviewPos][1],"preview_pos_y");
	        orm_addvar_float(ormid,GangInfo[id][PreviewPos][2],"preview_pos_z");
			orm_addvar_float(ormid,GangInfo[id][PreviewPos][3],"preview_pos_a");
	        orm_addvar_float(ormid,GangInfo[id][PreviewCamera][0],"preview_camera_x");
	        orm_addvar_float(ormid,GangInfo[id][PreviewCamera][1],"preview_camera_y");
	        orm_addvar_float(ormid,GangInfo[id][PreviewCamera][2],"preview_camera_z");
	        orm_apply_cache(ormid,row);
	        orm_setkey(ormid,"id");
	        
			Iter_Add(Gangs,id);
			GangGroup[id] = Group_Create(GangInfo[id][Name]);
	    }
        printf("[INIT] %d gangs loaded!",rows);
	}
	return 1;
}