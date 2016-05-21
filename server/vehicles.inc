enum E_VEHICLE
{
	ORM:ORM_ID,
	ID,
	Model,
	Float:Pos[4],
	Color[3],
	RespawnTime,
	Mod[13],
	Gang
}
new VehicleInfo[MAX_VEHICLES+1][E_VEHICLE];
new Iterator:Vehicles<MAX_VEHICLES+1>;

stock LoadVehicles()
{
	mysql_pquery(Database,"SELECT * FROM `vehicle_data` ORDER BY `id` ASC","Vehicles_Load","");
	return 1;
}

stock AddVehicle(model,Float:x,Float:y,Float:z,Float:a,color1,color2,respawn_time)
{
	
	return 1;
}

function Vehicles_Load()
{
	new rows = cache_get_row_count(Database);
	if(rows != 0)
	{
		new tModel,Float:tPos[4],color[2],respawn;
		forex(row,rows)
		{
			tModel = cache_get_row_int(row,1,Database);
			tPos[0] = cache_get_row_float(row,2,Database);
			tPos[1] = cache_get_row_float(row,3,Database);
			tPos[2] = cache_get_row_float(row,4,Database);
			tPos[3] = cache_get_row_float(row,5,Database);
			color[0] = cache_get_row_int(row,6,Database);
			color[1] = cache_get_row_int(row,7,Database);
			respawn = cache_get_row_int(row,9,Database);
			
			new vehid = CreateVehicle(tModel,tPos[0],tPos[1],tPos[2],tPos[3],color[0],color[1],respawn);
		
			new ORM:ormid = VehicleInfo[vehid][ORM_ID] = orm_create("vehicle_data",Database);
			orm_addvar_int(ormid,VehicleInfo[vehid][ID],"id");
			orm_addvar_int(ormid,VehicleInfo[vehid][Model],"model");
			orm_addvar_float(ormid,VehicleInfo[vehid][Pos][0],"pos_x");
			orm_addvar_float(ormid,VehicleInfo[vehid][Pos][1],"pos_y");
			orm_addvar_float(ormid,VehicleInfo[vehid][Pos][2],"pos_z");
			orm_addvar_float(ormid,VehicleInfo[vehid][Pos][3],"rot_z");
			orm_addvar_int(ormid,VehicleInfo[vehid][Color][0],"color_1");
			orm_addvar_int(ormid,VehicleInfo[vehid][Color][1],"color_2");
			orm_addvar_int(ormid,VehicleInfo[vehid][Color][2],"color_3");
			orm_addvar_int(ormid,VehicleInfo[vehid][RespawnTime],"respawn");
			orm_addvar_int(ormid,VehicleInfo[vehid][Mod][0],"mod_1");
			orm_addvar_int(ormid,VehicleInfo[vehid][Mod][1],"mod_2");
			orm_addvar_int(ormid,VehicleInfo[vehid][Mod][2],"mod_3");
			orm_addvar_int(ormid,VehicleInfo[vehid][Mod][3],"mod_4");
			orm_addvar_int(ormid,VehicleInfo[vehid][Mod][4],"mod_5");
			orm_addvar_int(ormid,VehicleInfo[vehid][Mod][5],"mod_6");
			orm_addvar_int(ormid,VehicleInfo[vehid][Mod][6],"mod_7");
			orm_addvar_int(ormid,VehicleInfo[vehid][Mod][7],"mod_8");
			orm_addvar_int(ormid,VehicleInfo[vehid][Mod][8],"mod_9");
			orm_addvar_int(ormid,VehicleInfo[vehid][Mod][9],"mod_10");
			orm_addvar_int(ormid,VehicleInfo[vehid][Mod][10],"mod_11");
			orm_addvar_int(ormid,VehicleInfo[vehid][Mod][11],"mod_12");
			orm_addvar_int(ormid,VehicleInfo[vehid][Mod][12],"mod_13");
			orm_addvar_int(ormid,VehicleInfo[vehid][Gang],"gang");
			
			orm_apply_cache(ormid,row);
			orm_setkey(ormid,"id");
			
			forex(i,13)
			{
				AddVehicleComponent(vehid,VehicleInfo[vehid][Mod][i]);
			}
			if(VehicleInfo[vehid][Color][2] != 2)
			{
				ChangeVehiclePaintjob(vehid,VehicleInfo[vehid][Color][2]);
				ChangeVehicleColor(vehid,VehicleInfo[vehid][Color][0],VehicleInfo[vehid][Color][1]);
			}
			Iter_Add(Vehicles,vehid);
		}
	}
	return 1;
}