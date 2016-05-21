ptask SavePlayers[300000](playerid)
{
	SavePlayer(playerid);
	return 1;
}

timer DelayKick[100](playerid)
{
	Kick(playerid);
	return 1;
}